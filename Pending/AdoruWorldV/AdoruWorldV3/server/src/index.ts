import express from 'express'
import cors from 'cors'
import path from 'path'
import postsRouter from './routes/posts'
import categoriesRouter from './routes/categories'
import tagsRouter from './routes/tags'
import projectsRouter from './routes/projects'
import dexRouter from './routes/dex'
import siteRouter from './routes/site'
import uploadRouter from './routes/upload'

const app = express()
const PORT = process.env.PORT || 3002

app.use(cors())
app.use(express.json({ limit: '10mb' }))

app.use('/uploads', express.static(path.resolve(__dirname, '../uploads')))

app.use('/api/posts', postsRouter)
app.use('/api/categories', categoriesRouter)
app.use('/api/tags', tagsRouter)
app.use('/api/projects', projectsRouter)
app.use('/api/dex', dexRouter)
app.use('/api/site', siteRouter)
app.use('/api/upload', uploadRouter)

app.get('/api/health', (_req, res) => {
  res.json({ status: 'ok' })
})

app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`)
})

export default app
