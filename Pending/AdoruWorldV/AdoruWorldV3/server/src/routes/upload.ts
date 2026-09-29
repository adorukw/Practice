import { Router, Request, Response } from 'express'
import multer from 'multer'
import path from 'path'
import fs from 'fs'

const uploadsDir = path.resolve(__dirname, '../../uploads')

if (!fs.existsSync(uploadsDir)) {
  fs.mkdirSync(uploadsDir, { recursive: true })
}

const storage = multer.diskStorage({
  destination: (_req, _file, cb) => {
    cb(null, uploadsDir)
  },
  filename: (_req, file, cb) => {
    const ext = path.extname(file.originalname)
    const name = Date.now().toString(36) + '-' + Math.random().toString(36).slice(2, 8) + ext
    cb(null, name)
  }
})

const upload = multer({
  storage,
  limits: { fileSize: 10 * 1024 * 1024 },
  fileFilter: (_req, file, cb) => {
    const allowed = /\.(jpg|jpeg|png|gif|webp|svg)$/i
    if (allowed.test(path.extname(file.originalname))) {
      cb(null, true)
    } else {
      cb(new Error('仅支持 jpg / png / gif / webp / svg 格式'))
    }
  }
})

const router = Router()

router.post('/', upload.single('file'), (req: Request, res: Response) => {
  if (!req.file) {
    return res.status(400).json({ error: '请选择文件' })
  }
  const url = `/uploads/${req.file.filename}`
  res.json({ url, filename: req.file.filename })
})

export default router
