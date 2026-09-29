import { Router, Request, Response } from 'express'
import prisma from '../db'

const router = Router()

router.get('/', async (_req: Request, res: Response) => {
  try {
    const projects = await prisma.project.findMany({ orderBy: { name: 'asc' } })
    res.json(projects.map(p => ({ ...p, tech: JSON.parse(p.tech) })))
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.get('/:id', async (req: Request, res: Response) => {
  try {
    const project = await prisma.project.findUnique({ where: { id: req.params.id } })
    if (!project) return res.status(404).json({ error: 'Project not found' })
    res.json({ ...project, tech: JSON.parse(project.tech) })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.post('/', async (req: Request, res: Response) => {
  try {
    const { tech, ...data } = req.body
    const project = await prisma.project.create({
      data: { ...data, tech: JSON.stringify(tech || []) }
    })
    res.json({ ...project, tech: JSON.parse(project.tech) })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.put('/:id', async (req: Request, res: Response) => {
  try {
    const { tech, ...data } = req.body
    const project = await prisma.project.update({
      where: { id: req.params.id },
      data: { ...data, tech: JSON.stringify(tech || []) }
    })
    res.json({ ...project, tech: JSON.parse(project.tech) })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

router.delete('/:id', async (req: Request, res: Response) => {
  try {
    await prisma.project.delete({ where: { id: req.params.id } })
    res.json({ success: true })
  } catch (error: any) {
    res.status(500).json({ error: error.message })
  }
})

export default router
