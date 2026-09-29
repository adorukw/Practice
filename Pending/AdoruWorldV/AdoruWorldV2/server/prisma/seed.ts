import { PrismaClient } from '@prisma/client'

const prisma = new PrismaClient()

async function main() {
  const categories = [
    { name: '技术笔记', slug: 'tech', description: '编程、开发、技术教程', icon: '💻', color: '#3C5AA6' },
    { name: '生活随笔', slug: 'life', description: '日常记录、思考、见闻', icon: '🌸', color: '#FF7300' },
    { name: '项目记录', slug: 'projects', description: '个人项目开发日志', icon: '🚀', color: '#FF0000' },
    { name: '读书笔记', slug: 'reading', description: '书籍阅读心得', icon: '📚', color: '#7B5BA6' },
    { name: '游戏人生', slug: 'gaming', description: '游戏评测、心得', icon: '🎮', color: '#FFDE00' },
    { name: '创意思考', slug: 'creative', description: '设计、艺术、创意分享', icon: '✨', color: '#9CBB0F' }
  ]

  for (const cat of categories) {
    await prisma.category.upsert({
      where: { slug: cat.slug },
      update: cat,
      create: cat
    })
  }

  const tags = [
    { name: 'Vue', slug: 'vue', color: '#42b883' },
    { name: 'TypeScript', slug: 'typescript', color: '#3178c6' },
    { name: 'React', slug: 'react', color: '#61dafb' },
    { name: 'Node.js', slug: 'nodejs', color: '#68a063' },
    { name: '前端', slug: 'frontend', color: '#FF7300' },
    { name: '后端', slug: 'backend', color: '#3C5AA6' },
    { name: 'CSS', slug: 'css', color: '#264de4' },
    { name: '像素艺术', slug: 'pixel-art', color: '#FFDE00' },
    { name: '游戏开发', slug: 'game-dev', color: '#FF0000' },
    { name: '生活', slug: 'life', color: '#9CBB0F' }
  ]

  for (const tag of tags) {
    await prisma.tag.upsert({
      where: { slug: tag.slug },
      update: tag,
      create: tag
    })
  }

  const projects = [
    { name: 'AdoruWorld Blog', description: '口袋妖怪风格的像素艺术个人博客', tech: JSON.stringify(['Vue 3', 'TypeScript', 'Tailwind CSS', 'Vite']), link: '', github: '', image: 'https://picsum.photos/seed/project-blog/600/400', status: 'in-progress' },
    { name: 'Pixel Adventure', description: '一款像素风格的冒险游戏', tech: JSON.stringify(['Phaser 3', 'TypeScript', 'Aseprite']), link: '', github: '', image: 'https://picsum.photos/seed/project-game/600/400', status: 'in-progress' },
    { name: 'Task Manager', description: '一个简洁的任务管理应用', tech: JSON.stringify(['React', 'Node.js', 'MongoDB']), link: 'https://example.com', github: 'https://github.com', image: 'https://picsum.photos/seed/project-task/600/400', status: 'completed' }
  ]

  for (const proj of projects) {
    await prisma.project.create({ data: proj })
  }

  const authorConfig = {
    name: 'AdoruKw',
    avatar: 'https://picsum.photos/seed/avatar/200/200',
    bio: '一个热爱编程和游戏的开发者，正在探索像素艺术与Web开发的结合。',
    location: '中国',
    social: { github: 'https://github.com', twitter: 'https://twitter.com', email: 'hello@example.com' }
  }

  await prisma.siteConfig.upsert({
    where: { key: 'author' },
    update: { value: JSON.stringify(authorConfig) },
    create: { key: 'author', value: JSON.stringify(authorConfig) }
  })

  console.log('Seed data inserted successfully!')
}

main()
  .catch(console.error)
  .finally(() => prisma.$disconnect())
