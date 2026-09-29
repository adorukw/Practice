import { Router, Request, Response } from 'express'
import prisma from '../db'

const router = Router()

router.get('/', async (_req: Request, res: Response) => {
  try {
    const { category, tag, published, featured, search } = _req.query
    
    const where: any = {}
    
    if (published !== undefined) where.published = published === 'true'
    if (featured !== undefined) where.featured = featured === 'true'
    
    if (category) {
      where.categories = { some: { category: { slug: String(category) } } }
    }
    
    if (tag) {
      where.tags = { some: { tag: { slug: String(tag) } } }
    }
    
    if (search) {
      const q = String(search)
      where.OR = [
        { title: { contains: q } },
        { description: { contains: q } },
        { tags: { some: { tag: { name: { contains: q } } } } }
      ]
    }
    
    const posts = await prisma.post.findMany({
      where,
      include: {
        categories: { include: { category: true } },
        tags: { include: { tag: true } }
      },
      orderBy: { createdAt: 'desc' }
    })
    
    const result = posts.map(post => ({
      ...post,
      category: post.categories.map(pc => pc.category.name),
      categories: undefined,
      tagNames: post.tags.map(pt => pt.tag.name),
      tags: undefined,
      reading_time: Math.max(1, Math.round(post.wordCount / 300))
    }))
    
    res.json(result)
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.get('/:slug', async (req: Request, res: Response) => {
  try {
    const post = await prisma.post.findUnique({
      where: { slug: req.params.slug },
      include: {
        categories: { include: { category: true } },
        tags: { include: { tag: true } }
      }
    })
    
    if (!post) {
      return res.status(404).json({ error: 'Post not found' })
    }
    
    await prisma.post.update({
      where: { id: post.id },
      data: { views: { increment: 1 } }
    })
    
    res.json({
      ...post,
      category: post.categories.map(pc => pc.category.name),
      categories: undefined,
      tagNames: post.tags.map(pt => pt.tag.name),
      tags: undefined,
      reading_time: Math.max(1, Math.round(post.wordCount / 300))
    })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.post('/', async (req: Request, res: Response) => {
  try {
    const { title, slug, description, content, coverImage, published, featured, categoryNames, tagNames } = req.body
    
    const wordCount = content ? content.replace(/\s/g, '').length : 0
    
    const post = await prisma.post.create({
      data: {
        title,
        slug,
        description: description || '',
        content: content || '',
        coverImage,
        published: published ?? false,
        featured: featured ?? false,
        wordCount
      }
    })
    
    if (categoryNames?.length) {
      for (const name of categoryNames) {
        const category = await prisma.category.upsert({
          where: { name },
          update: {},
          create: { name, slug: name.toLowerCase().replace(/\s+/g, '-') }
        })
        await prisma.postCategory.create({
          data: { postId: post.id, categoryId: category.id }
        })
      }
    }
    
    if (tagNames?.length) {
      for (const name of tagNames) {
        const tag = await prisma.tag.upsert({
          where: { name },
          update: {},
          create: { name, slug: name.toLowerCase().replace(/\s+/g, '-') }
        })
        await prisma.postTag.create({
          data: { postId: post.id, tagId: tag.id }
        })
      }
    }
    
    res.json(post)
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.put('/:id', async (req: Request, res: Response) => {
  try {
    const { title, slug, description, content, coverImage, published, featured, categoryNames, tagNames } = req.body
    
    const wordCount = content ? content.replace(/\s/g, '').length : 0
    
    const post = await prisma.post.update({
      where: { id: req.params.id },
      data: {
        title,
        slug,
        description,
        content,
        coverImage,
        published,
        featured,
        wordCount
      }
    })
    
    if (categoryNames !== undefined) {
      await prisma.postCategory.deleteMany({ where: { postId: post.id } })
      for (const name of categoryNames) {
        const category = await prisma.category.upsert({
          where: { name },
          update: {},
          create: { name, slug: name.toLowerCase().replace(/\s+/g, '-') }
        })
        await prisma.postCategory.create({
          data: { postId: post.id, categoryId: category.id }
        })
      }
    }
    
    if (tagNames !== undefined) {
      await prisma.postTag.deleteMany({ where: { postId: post.id } })
      for (const name of tagNames) {
        const tag = await prisma.tag.upsert({
          where: { name },
          update: {},
          create: { name, slug: name.toLowerCase().replace(/\s+/g, '-') }
        })
        await prisma.postTag.create({
          data: { postId: post.id, tagId: tag.id }
        })
      }
    }
    
    res.json(post)
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.delete('/:id', async (req: Request, res: Response) => {
  try {
    await prisma.postTag.deleteMany({ where: { postId: req.params.id } })
    await prisma.postCategory.deleteMany({ where: { postId: req.params.id } })
    await prisma.post.delete({ where: { id: req.params.id } })
    res.json({ success: true })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

export default router
