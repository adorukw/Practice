const express = require('express')
const path = require('path')
const cors = require('cors')
const fs = require('fs')
const GeoTIFF = require('geotiff.js')
const multer = require('multer')


const app = express()
const PORT = process.env.PORT || 3000

const { Pool } = require('pg')
const pool = new Pool({
    connectionString: 'postgres://postgres:postgres@localhost:5432/vovgis_db',
    ssl: process.env.NODE_ENV === 'production' ? { rejectUnauthorized: false } : false
})

app.use(express.json())
app.use(cors())
app.use('/public', express.static(path.join(__dirname, '/public')))
app.use('/data', express.static(path.join(__dirname, '/data')))

async function InitializeDatabase() {
    try {
        await pool.query(`
            CREATE TABLE IF NOT EXISTS metadata (
                id SERIAL PRIMARY KEY,
                category VARCHAR(255),
                subcategory VARCHAR(255),
                data_name VARCHAR(255),
                min FLOAT,
                max FLOAT,
                mean FLOAT,
                std FLOAT,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                UNIQUE (category, subcategory, data_name)
            )
        `)
        console.log('metadata 表已就绪')
        await pool.query(`
            CREATE TABLE IF NOT EXISTS users (
                id SERIAL PRIMARY KEY,
                username VARCHAR(255) UNIQUE NOT NULL,
                password VARCHAR(255) NOT NULL,
                role VARCHAR(50) NOT NULL DEFAULT 'admin',
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        `)
        await pool.query(`
            INSERT INTO users (username, password, role) 
            VALUES ('admin', 'admin123', 'admin')
            ON CONFLICT (username) DO NOTHING
        `)
        console.log('users 表已就绪')
    } catch (error) {
        console.error('初始化数据库表失败:', error)
        throw error
    }
}

async function ComputeTif(filePath) {
    const buffer = await fs.promises.readFile(filePath)
    const tif = await GeoTIFF.fromArrayBuffer(buffer.buffer)
    const image = await tif.getImage()
    const raster = await image.readRasters()
    const data = raster[0]

    let validCount = 0
    let min = Infinity
    let max = -Infinity
    let mean = 0
    let M2 = 0

    for (let i = 0; i < data.length; i++) {
        const val = data[i]
        if (val === -9999 || isNaN(val)) continue

        validCount++

        if (val < min) min = val
        if (val > max) max = val

        const delta = val - mean
        mean += delta / validCount
        const delta2 = val - mean
        M2 += delta * delta2
    }

    if (validCount === 0) {
        return { min: NaN, max: NaN, mean: NaN, std: NaN }
    }

    const variance = M2 / validCount
    const std = Math.sqrt(variance)

    return { min, max, mean, std }
}

async function ComputeDataInfo(dataType) {
    const filePath = path.resolve(path.join(__dirname, '/data/', dataType))

    const pathParts = dataType.split('/').filter(p => p)
    const category = pathParts[0]
    const subcategory = pathParts[1] || ''

    const allFiles = fs.readdirSync(filePath)

    for (const file of allFiles) {
        const fullPath = path.join(filePath, file)
        try {
            const tifInfo = await ComputeTif(fullPath)
            const dataName = path.parse(file).name

            const insertQuery = `
                INSERT INTO metadata 
                (category, subcategory, data_name, min, max, mean, std)
                VALUES ($1, $2, $3, $4, $5, $6, $7)
                ON CONFLICT (category, subcategory, data_name) DO UPDATE SET
                    min = EXCLUDED.min,
                    max = EXCLUDED.max,
                    mean = EXCLUDED.mean,
                    std = EXCLUDED.std,
                    created_at = CURRENT_TIMESTAMP
            `
            await pool.query(insertQuery, [
                category,
                subcategory,
                dataName,
                tifInfo.min,
                tifInfo.max,
                tifInfo.mean,
                tifInfo.std
            ])
        } catch (error) {
            console.error(`处理文件 ${file} 时出错:`, error)
        }
    }
}

async function InitializeMetadata() {
    const dataTypeList = [
        '/碳循环/GPP', '/碳循环/NEP', '/碳循环/NPP', '/碳循环/TER',
        '/土壤/土壤PH', '/土壤/土壤有效含水量AWC', '/土壤/土壤质地TEXTURE',
        '/气象/Pre', '/气象/Rad', '/气象/Rhu', '/气象/Tavg', '/气象/Tmax', '/气象/Tmin',
        '/森林结构/森林覆盖率', '/森林结构/森林类型', '/森林结构/森林年龄',
        '/冠层数据/EVI', '/冠层数据/FPAR', '/冠层数据/LAI',
        '/社会经济/GDP', '/社会经济/POP', '/打分情况/TIF'
    ]

    try {
        await pool.query('TRUNCATE metadata RESTART IDENTITY')

        for (const dataType of dataTypeList) {
            console.log(`正在初始化: ${dataType}`)
            await ComputeDataInfo(dataType)
        }
        console.log('元数据初始化完成')
    } catch (error) {
        console.error('初始化元数据时出错:', error)
    }
}

app.get('/getAllDataInfo', async (req, res) => {
    try {
        const query = `
            SELECT 
                CONCAT(category, '/', subcategory) as data_type,
                data_name,
                min,
                max,
                mean,
                std
            FROM metadata
            ORDER BY data_type
        `
        const result = await pool.query(query)

        const transformed = result.rows.reduce((acc, row) => {
            const key = row.data_type.split('/').pop()
            if (!acc[key]) acc[key] = {}
            acc[key][row.data_name] = {
                min: row.min,
                max: row.max,
                mean: row.mean,
                std: row.std
            }
            return acc
        }, {})

        res.json(transformed)
    } catch (error) {
        console.error(error)
        res.status(500).json({ error: '获取元数据失败' })
    }
})

app.get('/getFilteredDataInfo', async (req, res) => {
    try {
        const { mainType, subType, name } = req.query
        let queryParams = []
        let whereClauses = []

        if (mainType) {
            whereClauses.push(`category = $${queryParams.length + 1}`)
            queryParams.push(mainType)
        }

        if (subType) {
            whereClauses.push(`subcategory = $${queryParams.length + 1}`)
            queryParams.push(subType)
        }

        if (name) {
            whereClauses.push(`data_name = $${queryParams.length + 1}`)
            queryParams.push(name)
        }

        let query = `
            SELECT 
                category,
                subcategory,
                data_name,
                min,
                max,
                mean,
                std
            FROM metadata
            ${whereClauses.length > 0 ? 'WHERE ' + whereClauses.join(' AND ') : ''}
            ORDER BY category, subcategory, data_name
        `

        const result = await pool.query(query, queryParams)

        const transformed = result.rows.reduce((acc, row) => {
            const key = `${row.category}/${row.subcategory}`
            if (!acc[key]) acc[key] = {}
            acc[key][row.data_name] = {
                min: row.min,
                max: row.max,
                mean: row.mean,
                std: row.std
            }
            return acc
        }, {})

        res.json(transformed)
    } catch (error) {
        console.error(error)
        res.status(500).json({ error: '查询元数据失败' })
    }
})

app.delete('/delete', async (req, res) => {
    const { category, subcategory, data_name } = req.query

    if (!category || !data_name) {
        return res.status(400).json({ error: '缺少必要参数' })
    }

    const client = await pool.connect()
    try {
        await client.query('BEGIN')

        const deleteQuery = `
            DELETE FROM metadata 
            WHERE category = $1 
            AND subcategory = $2 
            AND data_name = $3
        `
        await client.query(deleteQuery, [category, subcategory || '', data_name])

        const filePath = path.join(
            __dirname,
            'data',
            category,
            subcategory || '',
            `${data_name}.tif`
        )

        if (fs.existsSync(filePath)) {
            fs.unlinkSync(filePath)
        } else {
            console.warn(`文件不存在: ${filePath}`)
        }

        await client.query('COMMIT')
        res.json({ success: true })
    } catch (error) {
        await client.query('ROLLBACK')
        console.error('删除失败:', error)
        res.status(500).json({ error: '删除操作失败' })
    } finally {
        client.release();
    }
});

const upload = multer({ dest: 'uploads/' })

app.post('/upload', upload.single('file'), async (req, res) => {
    try {
        const { category, subcategory } = req.body
        const file = req.file

        if (!category || !file) {
            throw new Error('缺少必要参数')
        }

        if (!file.originalname.endsWith('.tif')) {
            throw new Error('仅支持TIF格式文件')
        }

        const targetDir = path.join(
            __dirname,
            'data',
            category,
            subcategory || ''
        )
        if (!fs.existsSync(targetDir)) {
            fs.mkdirSync(targetDir, { recursive: true })
        }

        const finalPath = path.join(targetDir, file.originalname)
        fs.renameSync(file.path, finalPath)

        const dataName = path.parse(file.originalname).name
        const tifInfo = await ComputeTif(finalPath)

        const insertQuery = `
            INSERT INTO metadata 
            (category, subcategory, data_name, min, max, mean, std)
            VALUES ($1, $2, $3, $4, $5, $6, $7)
            ON CONFLICT (category, subcategory, data_name) DO UPDATE SET
                min = EXCLUDED.min,
                max = EXCLUDED.max,
                mean = EXCLUDED.mean,
                std = EXCLUDED.std,
                created_at = CURRENT_TIMESTAMP
        `
        await pool.query(insertQuery, [
            category,
            subcategory || '',
            dataName,
            tifInfo.min,
            tifInfo.max,
            tifInfo.mean,
            tifInfo.std
        ])

        res.status(200).json({ success: true })
    } catch (error) {
        console.error('上传处理失败:', error)
        res.status(500).json({
            success: false,
            message: error.message || '文件处理失败'
        })
    }
})

app.get('/show/*', async (req, res) => {
    const filePath = req.params[0]
    const fullPath = path.resolve(path.join(__dirname, '/data', filePath))

    try {
        const buffer = await fs.promises.readFile(fullPath)
        const tif = await GeoTIFF.fromArrayBuffer(buffer.buffer)
        const image = await tif.getImage()

        const raster = await image.readRasters()
        const data = raster[0]

        let min = Infinity, max = -Infinity
        for (let i = 0; i < data.length; i++) {
            const val = data[i]
            if (val !== -9999) {
                if (val < min) min = val
                if (val > max) max = val
            }
        }

        const uint8Data = new Uint8Array(data.length)
        const range = max - min
        for (let i = 0; i < data.length; i++) {
            const val = data[i]
            uint8Data[i] = val === -9999 ? 0 : Math.round((val - min) / range * 255)
        }

        const fileDirectory = image.getFileDirectory()

        const newMetadata = {
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

        const newArrayBuffer = await GeoTIFF.writeArrayBuffer(uint8Data, newMetadata)

        res.send(Buffer.from(newArrayBuffer))
    } catch (error) {
        console.error(error)
        res.status(500).json({ error: '处理文件时出错' })
    }
})

app.get('/list/*', (req, res) => {
    try {
        const filePath = req.params[0]
        const fullPath = path.resolve(path.join(__dirname, '/data', filePath))

        const allFiles = fs.readdirSync(fullPath)
        const allFilesName = allFiles.map(file => {
            return { value: `${path.parse(file).name}`, label: `${path.parse(file).name}` }
        })

        res.json(allFilesName)
    }
    catch (error) {
        res.status(500).json({ error: '获取文件列表时出错' })
    }
})

app.post('/login', async (req, res) => {
    const { username, password } = req.body
    try {
        const result = await pool.query(
            'SELECT * FROM users WHERE username = $1 AND password = $2',
            [username, password]
        )
        if (result.rows.length === 0) {
            return res.status(401).json({ error: '用户名或密码错误' })
        }
        const user = result.rows[0]
        res.json({
            success: true,
            role: user.role,
            username: user.username
        })
    } catch (error) {
        console.error('登录失败:', error)
        res.status(500).json({ error: '登录失败' })
    }
})

app.get('/users', async (req, res) => {
    const result = await pool.query('SELECT id, username, role FROM users')
    res.json(result.rows)
})

app.post('/users', async (req, res) => {
    const { username, password } = req.body
    await pool.query('INSERT INTO users (username, password) VALUES ($1, $2)', [username, password])
    res.json({ success: true })
})

app.delete('/users/:id', async (req, res) => {
    await pool.query('DELETE FROM users WHERE id = $1', [req.params.id])
    res.json({ success: true })
})

async function StartServer() {
    try {
        await InitializeDatabase()
        await InitializeMetadata()
        app.listen(PORT, () => {
            console.log(`服务在${PORT}端口启动`)
            console.log('元数据已持久化到数据库')
        })
    } catch (error) {
        console.error('服务启动失败:', error)
        process.exit(1)
    }
}

StartServer()