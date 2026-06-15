const { workerData, parentPort } = require('worker_threads')
const { promisify } = require('util')
const fs = require('fs')
const path = require('path')
const GeoTIFF = require('geotiff.js')

async function ProcessTiff(filePath) {
    const buffer = await fs.promises.readFile(filePath)
    const tiff = await GeoTIFF.fromArrayBuffer(buffer.buffer)
    const image = await tiff.getImage()

    const raster = await image.readRasters()
    const data = raster[0]

    let [min, max] = [Infinity, -Infinity];
    for (const val of data) {
        if (val !== -9999) {
            min = Math.min(min, val);
            max = Math.max(max, val);
        }
    }

    const range = max - min
    const uint8Data = new Uint8Array(data.length)
    for (let i = 0; i < data.length; i++) {
        uint8Data[i] = data[i] === -9999 ? 0 : Math.round((data[i] - min) / range * 255)
    }

    // 生成元数据
    const fileDirectory = image.getFileDirectory()
    const metadata = {
        width: fileDirectory.ImageWidth,
        height: fileDirectory.ImageLength,
        ModelPixelScale: fileDirectory.ModelPixelScale,
        ModelTiepoint: fileDirectory.ModelTiepoint,
        GeographicTypeGeoKey: fileDirectory.GeographicTypeGeoKey || 4326,
        BitsPerSample: [8],
        SampleFormat: [1],
        PhotometricInterpretation: 1,
        PlanarConfiguration: 1,
        SamplesPerPixel: 1,
        GTRasterTypeGeoKey: 1,
    }

    return GeoTIFF.writeArrayBuffer(uint8Data, metadata)
}

ProcessTiff()
    .then(arrayBuffer => {
        parentPort.postMessage({ buffer: arrayBuffer }, [arrayBuffer.buffer])
    })
    .catch(error => {
        parentPort.postMessage({ error: error.message });
    })