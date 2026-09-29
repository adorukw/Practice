import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';

const router = Router();

interface TagRow {
  id: string;
  name: string;
  slug: string;
  color: string | null;
  count: number;
}

function formatTag(row: TagRow) {
  return {
    id: row.id,
    name: row.name,
    slug: row.slug,
    color: row.color,
    count: row.count,
  };
}

router.get('/', (_req: Request, res: Response) => {
  const rows = db.prepare('SELECT * FROM post_tag ORDER BY name').all() as TagRow[];
  res.json({ data: rows.map(formatTag) });
});

router.get('/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM post_tag WHERE id = ? OR slug = ?').get(req.params.id, req.params.id) as TagRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Tag not found' });
    return;
  }

  const posts = db.prepare(
    `SELECT p.* FROM post p
     INNER JOIN post_tag_relation ptr ON p.id = ptr.post_id
     WHERE ptr.tag_id = ?
     ORDER BY p.created_at DESC`
  ).all(row.id);

  res.json({ data: { ...formatTag(row), posts } });
});

router.post('/', (req: Request, res: Response) => {
  const { name, slug, color } = req.body;

  if (!name || !slug) {
    res.status(400).json({ error: 'name and slug are required' });
    return;
  }

  const existing = db.prepare('SELECT id FROM post_tag WHERE slug = ?').get(slug);
  if (existing) {
    res.status(409).json({ error: 'Slug already exists' });
    return;
  }

  const id = uuidv4();
  db.prepare(
    `INSERT INTO post_tag (id, name, slug, color, count) VALUES (?, ?, ?, ?, 0)`
  ).run(id, name, slug, color || null);

  const row = db.prepare('SELECT * FROM post_tag WHERE id = ?').get(id) as TagRow;
  res.status(201).json({ data: formatTag(row) });
});

router.put('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM post_tag WHERE id = ?').get(req.params.id) as TagRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Tag not found' });
    return;
  }

  const { name, slug, color } = req.body;

  if (slug && slug !== existing.slug) {
    const slugExists = db.prepare('SELECT id FROM post_tag WHERE slug = ? AND id != ?').get(slug, req.params.id);
    if (slugExists) {
      res.status(409).json({ error: 'Slug already exists' });
      return;
    }
  }

  db.prepare(
    `UPDATE post_tag SET name = ?, slug = ?, color = ? WHERE id = ?`
  ).run(name ?? existing.name, slug ?? existing.slug, color ?? existing.color, req.params.id);

  const row = db.prepare('SELECT * FROM post_tag WHERE id = ?').get(req.params.id) as TagRow;
  res.json({ data: formatTag(row) });
});

router.delete('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM post_tag WHERE id = ?').get(req.params.id) as TagRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Tag not found' });
    return;
  }

  db.prepare('DELETE FROM post_tag WHERE id = ?').run(req.params.id);
  res.json({ message: 'Tag deleted' });
});

export default router;
