import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';

const router = Router();

interface CategoryRow {
  id: string;
  name: string;
  slug: string;
  description: string | null;
  count: number;
  icon: string | null;
  color: string | null;
}

function formatCategory(row: CategoryRow) {
  return {
    id: row.id,
    name: row.name,
    slug: row.slug,
    description: row.description,
    count: row.count,
    icon: row.icon,
    color: row.color,
  };
}

router.get('/', (_req: Request, res: Response) => {
  const rows = db.prepare('SELECT * FROM post_category ORDER BY name').all() as CategoryRow[];
  res.json({ data: rows.map(formatCategory) });
});

router.get('/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM post_category WHERE id = ? OR slug = ?').get(req.params.id, req.params.id) as CategoryRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Category not found' });
    return;
  }

  const posts = db.prepare(
    `SELECT p.* FROM post p
     INNER JOIN post_category_relation pcr ON p.id = pcr.post_id
     WHERE pcr.category_id = ?
     ORDER BY p.created_at DESC`
  ).all(row.id);

  res.json({ data: { ...formatCategory(row), posts } });
});

router.post('/', (req: Request, res: Response) => {
  const { name, slug, description, icon, color } = req.body;

  if (!name || !slug) {
    res.status(400).json({ error: 'name and slug are required' });
    return;
  }

  const existing = db.prepare('SELECT id FROM post_category WHERE slug = ?').get(slug);
  if (existing) {
    res.status(409).json({ error: 'Slug already exists' });
    return;
  }

  const id = uuidv4();
  db.prepare(
    `INSERT INTO post_category (id, name, slug, description, count, icon, color)
     VALUES (?, ?, ?, ?, 0, ?, ?)`
  ).run(id, name, slug, description || null, icon || null, color || null);

  const row = db.prepare('SELECT * FROM post_category WHERE id = ?').get(id) as CategoryRow;
  res.status(201).json({ data: formatCategory(row) });
});

router.put('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM post_category WHERE id = ?').get(req.params.id) as CategoryRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Category not found' });
    return;
  }

  const { name, slug, description, icon, color } = req.body;

  if (slug && slug !== existing.slug) {
    const slugExists = db.prepare('SELECT id FROM post_category WHERE slug = ? AND id != ?').get(slug, req.params.id);
    if (slugExists) {
      res.status(409).json({ error: 'Slug already exists' });
      return;
    }
  }

  db.prepare(
    `UPDATE post_category SET name = ?, slug = ?, description = ?, icon = ?, color = ? WHERE id = ?`
  ).run(
    name ?? existing.name,
    slug ?? existing.slug,
    description ?? existing.description,
    icon ?? existing.icon,
    color ?? existing.color,
    req.params.id
  );

  const row = db.prepare('SELECT * FROM post_category WHERE id = ?').get(req.params.id) as CategoryRow;
  res.json({ data: formatCategory(row) });
});

router.delete('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM post_category WHERE id = ?').get(req.params.id) as CategoryRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Category not found' });
    return;
  }

  db.prepare('DELETE FROM post_category WHERE id = ?').run(req.params.id);
  res.json({ message: 'Category deleted' });
});

export default router;
