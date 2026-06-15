import { Router, Request, Response } from 'express'
import prisma from '../db'

const router = Router()

router.get('/', async (_req: Request, res: Response) => {
  try {
    const tags = await prisma.tag.findMany({
      include: {
        _count: { select: { posts: true } }
      },
      orderBy: { name: 'asc' }
    })
    
    const result = tags.map(tag => ({
      ...tag,
      count: tag._count.posts,
      _count: undefined
    }))
    
    res.json(result)
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.post('/', async (req: Request, res: Response) => {
  try {
    const tag = await prisma.tag.create({ data: req.body })
    res.json(tag)
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.put('/:id', async (req: Request, res: Response) => {
  try {
    const tag = await prisma.tag.update({
      where: { id: req.params.id },
      data: req.body
    })
    res.json(tag)
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.delete('/:id', async (req: Request, res: Response) => {
  try {
    await prisma.tag.delete({ where: { id: req.params.id } })
    res.json({ success: true })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

export default router
