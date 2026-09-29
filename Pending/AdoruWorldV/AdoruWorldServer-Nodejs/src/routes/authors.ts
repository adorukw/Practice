import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';

const router = Router();

interface AuthorRow {
  id: string;
  name: string;
  avatar: string;
  bio: string;
  location: string;
  social: string | null;
}

function formatAuthor(row: AuthorRow) {
  return {
    id: row.id,
    name: row.name,
    avatar: row.avatar,
    bio: row.bio,
    location: row.location,
    social: row.social ? JSON.parse(row.social) : null,
  };
}

router.get('/', (_req: Request, res: Response) => {
  const rows = db.prepare('SELECT * FROM author ORDER BY name').all() as AuthorRow[];
  res.json({ data: rows.map(formatAuthor) });
});

router.get('/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM author WHERE id = ?').get(req.params.id) as AuthorRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Author not found' });
    return;
  }
  res.json({ data: formatAuthor(row) });
});

router.post('/', (req: Request, res: Response) => {
  const { name, avatar, bio, location, social } = req.body;

  if (!name || !avatar || !bio || !location) {
    res.status(400).json({ error: 'name, avatar, bio, and location are required' });
    return;
  }

  const id = uuidv4();
  db.prepare(
    `INSERT INTO author (id, name, avatar, bio, location, social) VALUES (?, ?, ?, ?, ?, ?)`
  ).run(id, name, avatar, bio, location, social ? JSON.stringify(social) : null);

  const row = db.prepare('SELECT * FROM author WHERE id = ?').get(id) as AuthorRow;
  res.status(201).json({ data: formatAuthor(row) });
});

router.put('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM author WHERE id = ?').get(req.params.id) as AuthorRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Author not found' });
    return;
  }

  const { name, avatar, bio, location, social } = req.body;

  db.prepare(
    `UPDATE author SET name = ?, avatar = ?, bio = ?, location = ?, social = ? WHERE id = ?`
  ).run(
    name ?? existing.name,
    avatar ?? existing.avatar,
    bio ?? existing.bio,
    location ?? existing.location,
    social !== undefined ? (social ? JSON.stringify(social) : null) : existing.social,
    req.params.id
  );

  const row = db.prepare('SELECT * FROM author WHERE id = ?').get(req.params.id) as AuthorRow;
  res.json({ data: formatAuthor(row) });
});

router.delete('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM author WHERE id = ?').get(req.params.id) as AuthorRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Author not found' });
    return;
  }

  db.prepare('DELETE FROM author WHERE id = ?').run(req.params.id);
  res.json({ message: 'Author deleted' });
});

export default router;
