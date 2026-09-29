import { Router, Request, Response } from 'express'
import prisma from '../db'

const router = Router()

router.get('/', async (_req: Request, res: Response) => {
  try {
    const { category, status, search } = _req.query
    
    const where: any = {}
    if (category) where.category = String(category)
    if (status) where.status = String(status)
    if (search) {
      const q = String(search)
      where.OR = [
        { title: { contains: q } },
        { originalTitle: { contains: q } }
      ]
    }
    
    const entries = await prisma.dexEntry.findMany({
      where,
      orderBy: { createdAt: 'desc' }
    })
    
    res.json(entries.map(e => ({
      ...e,
      tags: JSON.parse(e.tags),
      genre: JSON.parse(e.genre)
    })))
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.get('/:slug', async (req: Request, res: Response) => {
  try {
    const entry = await prisma.dexEntry.findUnique({ where: { slug: req.params.slug } })
    if (!entry) return res.status(404).json({ error: 'Entry not found' })
    res.json({ ...entry, tags: JSON.parse(entry.tags), genre: JSON.parse(entry.genre) })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.post('/', async (req: Request, res: Response) => {
  try {
    const { tags, genre, ...data } = req.body
    const entry = await prisma.dexEntry.create({
      data: { ...data, tags: JSON.stringify(tags || []), genre: JSON.stringify(genre || []) }
    })
    res.json({ ...entry, tags: JSON.parse(entry.tags), genre: JSON.parse(entry.genre) })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.put('/:id', async (req: Request, res: Response) => {
  try {
    const { tags, genre, ...data } = req.body
    const entry = await prisma.dexEntry.update({
      where: { id: req.params.id },
      data: { ...data, tags: JSON.stringify(tags || []), genre: JSON.stringify(genre || []) }
    })
    res.json({ ...entry, tags: JSON.parse(entry.tags), genre: JSON.parse(entry.genre) })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.delete('/:id', async (req: Request, res: Response) => {
  try {
    await prisma.dexEntry.delete({ where: { id: req.params.id } })
    res.json({ success: true })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

export default router
