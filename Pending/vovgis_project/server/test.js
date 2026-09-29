const fs = require('fs')
const path = require('path')
const GeoTIFF = require('geotiff.js')

async function main() {
    const tifPath = `C:/AAAPAN/Project/vovgis_project/server/data/森林结构/森林类型/2014.tif`
    const file = fs.readFileSync(tifPath)
    const tif = await GeoTIFF.fromArrayBuffer(file.buffer)
    const image = await tif.getImage()
    const raster = await image.readRasters()
    const data = raster[0]

    let validCount = 0;
    for (const val of data) {
      if (val !== -9999 && !isNaN(val)) validCount++;
    }
    
    // 根据统计结果，预分配紧凑的 TypedArray
    const validData = new data.constructor(validCount); // 继承原始数据类型（如 Float32Array）
    
    // 第二次遍历：填充有效数据
    let index = 0;
    for (const val of data) {
      if (val !== -9999 && !isNaN(val)) {
        validData[index] = val;
        index++;
      }
    }

    let min = Infinity
    let max = -Infinity
    let sum = 0

    for (let i = 0; i < validData.length; i++) {
        const val = validData[i]
        if (val < min) min = val
        if (val > max) max = val
        sum += val
    }

    let mean = sum / validData.length

    const memoryUsage = process.memoryUsage()
    console.log(`RSS: ${memoryUsage.rss / 1024 / 1024} MB`)
}

main()