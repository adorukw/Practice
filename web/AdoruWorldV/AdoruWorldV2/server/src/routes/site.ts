import { Router, Request, Response } from 'express'
import prisma from '../db'

const router = Router()

router.get('/stats', async (_req: Request, res: Response) => {
  try {
    const totalPosts = await prisma.post.count({ where: { published: true } })
    const totalViews = await prisma.post.aggregate({ _sum: { views: true } })
    const totalWords = await prisma.post.aggregate({ _sum: { wordCount: true } })
    
    const firstPost = await prisma.post.findFirst({ orderBy: { createdAt: 'asc' } })
    const runningDays = firstPost
      ? Math.floor((Date.now() - new Date(firstPost.createdAt).getTime()) / (1000 * 60 * 60 * 24))
      : 0
    
    res.json({
      totalPosts,
      totalWords: totalWords._sum.wordCount || 0,
      totalViews: totalViews._sum.views || 0,
      runningDays
    })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.get('/author', async (_req: Request, res: Response) => {
  try {
    const config = await prisma.siteConfig.findUnique({ where: { key: 'author' } })
    res.json(config ? JSON.parse(config.value) : {
      name: 'AdoruKw',
      avatar: '',
      bio: '',
      location: '',
      social: { github: '', twitter: '', email: '' }
    })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.put('/author', async (req: Request, res: Response) => {
  try {
    const config = await prisma.siteConfig.upsert({
      where: { key: 'author' },
      update: { value: JSON.stringify(req.body) },
      create: { key: 'author', value: JSON.stringify(req.body) }
    })
    res.json(JSON.parse(config.value))
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

export default router
