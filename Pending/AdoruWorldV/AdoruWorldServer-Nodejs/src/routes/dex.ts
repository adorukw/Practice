import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';
import type { DexCategory, DexStatus } from '../types';

const router = Router();

interface DexEntryRow {
  id: string;
  slug: string;
  title: string;
  original_title: string | null;
  cover_image: string;
  category: DexCategory;
  status: DexStatus;
  rating: number;
  start_date: string | null;
  finish_date: string | null;
  comments: string | null;
  creator: string | null;
  year: number | null;
}

interface DexEntryGenreRow {
  id: string;
  name: string;
  dex_entry_id: string;
}

interface DexStatsRow {
  id: string;
  total: number;
  by_category: string;
  by_status: string;
  average_rating: number;
  updated_at: string;
}

function formatDexEntry(row: DexEntryRow, genres: DexEntryGenreRow[] = []) {
  return {
    id: row.id,
    slug: row.slug,
    title: row.title,
    originalTitle: row.original_title,
    coverImage: row.cover_image,
    category: row.category,
    status: row.status,
    rating: row.rating,
    startDate: row.start_date,
    finishDate: row.finish_date,
    comments: row.comments,
    creator: row.creator,
    year: row.year,
    genres: genres.map((g) => ({ id: g.id, name: g.name, dexEntryId: g.dex_entry_id })),
  };
}

function formatDexStats(row: DexStatsRow) {
  return {
    id: row.id,
    total: row.total,
    byCategory: JSON.parse(row.by_category),
    byStatus: JSON.parse(row.by_status),
    averageRating: row.average_rating,
    updatedAt: row.updated_at,
  };
}

const validCategories: DexCategory[] = ['ANIME', 'MOVIE', 'TV', 'GAME', 'BOOK', 'MUSIC', 'OTHER'];
const validStatuses: DexStatus[] = ['COMPLETED', 'WATCHING', 'PLAYING', 'READING', 'LISTENING', 'DROPPED', 'PLANNED'];

router.get('/entries', (req: Request, res: Response) => {
  const { category, status, page = '1', limit = '10' } = req.query;
  let sql = 'SELECT * FROM dex_entry WHERE 1=1';
  const params: unknown[] = [];

  if (category) {
    sql += ' AND category = ?';
    params.push(category);
  }
  if (status) {
    sql += ' AND status = ?';
    params.push(status);
  }

  const countRow = db.prepare(`SELECT COUNT(*) as total FROM (${sql})`).get(...params) as { total: number };
  const total = countRow.total;

  const offset = (Number(page) - 1) * Number(limit);
  sql += ' ORDER BY start_date DESC LIMIT ? OFFSET ?';
  params.push(Number(limit), offset);

  const rows = db.prepare(sql).all(...params) as DexEntryRow[];
  const result = rows.map((row) => {
    const genres = db.prepare('SELECT * FROM dex_entry_genre WHERE dex_entry_id = ?').all(row.id) as DexEntryGenreRow[];
    return formatDexEntry(row, genres);
  });

  res.json({ data: result, total, page: Number(page), limit: Number(limit) });
});

router.get('/entries/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM dex_entry WHERE id = ? OR slug = ?').get(req.params.id, req.params.id) as DexEntryRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Dex entry not found' });
    return;
  }

  const genres = db.prepare('SELECT * FROM dex_entry_genre WHERE dex_entry_id = ?').all(row.id) as DexEntryGenreRow[];
  res.json({ data: formatDexEntry(row, genres) });
});

router.post('/entries', (req: Request, res: Response) => {
  const { slug, title, originalTitle, coverImage, category, status, rating, startDate, finishDate, comments, creator, year, genres } = req.body;

  if (!slug || !title || !coverImage || !category || !status || rating === undefined) {
    res.status(400).json({ error: 'slug, title, coverImage, category, status, and rating are required' });
    return;
  }

  if (!validCategories.includes(category)) {
    res.status(400).json({ error: `Invalid category. Valid: ${validCategories.join(', ')}` });
    return;
  }

  if (!validStatuses.includes(status)) {
    res.status(400).json({ error: `Invalid status. Valid: ${validStatuses.join(', ')}` });
    return;
  }

  const existing = db.prepare('SELECT id FROM dex_entry WHERE slug = ?').get(slug);
  if (existing) {
    res.status(409).json({ error: 'Slug already exists' });
    return;
  }

  const id = uuidv4();

  const insertEntry = db.transaction(() => {
    db.prepare(
      `INSERT INTO dex_entry (id, slug, title, original_title, cover_image, category, status, rating, start_date, finish_date, comments, creator, year)
       VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`
    ).run(id, slug, title, originalTitle || null, coverImage, category, status, rating, startDate || null, finishDate || null, comments || null, creator || null, year || null);

    if (genres && Array.isArray(genres)) {
      const insertGenre = db.prepare('INSERT INTO dex_entry_genre (id, name, dex_entry_id) VALUES (?, ?, ?)');
      for (const genre of genres) {
        const genreId = typeof genre === 'string' ? uuidv4() : genre.id || uuidv4();
        const genreName = typeof genre === 'string' ? genre : genre.name;
        insertGenre.run(genreId, genreName, id);
      }
    }
  });

  insertEntry();
  const row = db.prepare('SELECT * FROM dex_entry WHERE id = ?').get(id) as DexEntryRow;
  const genreRows = db.prepare('SELECT * FROM dex_entry_genre WHERE dex_entry_id = ?').all(id) as DexEntryGenreRow[];
  res.status(201).json({ data: formatDexEntry(row, genreRows) });
});

router.put('/entries/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM dex_entry WHERE id = ?').get(req.params.id) as DexEntryRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Dex entry not found' });
    return;
  }

  const { slug, title, originalTitle, coverImage, category, status, rating, startDate, finishDate, comments, creator, year, genres } = req.body;

  if (slug && slug !== existing.slug) {
    const slugExists = db.prepare('SELECT id FROM dex_entry WHERE slug = ? AND id != ?').get(slug, req.params.id);
    if (slugExists) {
      res.status(409).json({ error: 'Slug already exists' });
      return;
    }
  }

  if (category && !validCategories.includes(category)) {
    res.status(400).json({ error: `Invalid category. Valid: ${validCategories.join(', ')}` });
    return;
  }

  if (status && !validStatuses.includes(status)) {
    res.status(400).json({ error: `Invalid status. Valid: ${validStatuses.join(', ')}` });
    return;
  }

  const updateEntry = db.transaction(() => {
    db.prepare(
      `UPDATE dex_entry SET slug = ?, title = ?, original_title = ?, cover_image = ?, category = ?, status = ?, rating = ?, start_date = ?, finish_date = ?, comments = ?, creator = ?, year = ? WHERE id = ?`
    ).run(
      slug ?? existing.slug,
      title ?? existing.title,
      originalTitle ?? existing.original_title,
      coverImage ?? existing.cover_image,
      category ?? existing.category,
      status ?? existing.status,
      rating ?? existing.rating,
      startDate ?? existing.start_date,
      finishDate ?? existing.finish_date,
      comments ?? existing.comments,
      creator ?? existing.creator,
      year ?? existing.year,
      req.params.id
    );

    if (genres !== undefined) {
      db.prepare('DELETE FROM dex_entry_genre WHERE dex_entry_id = ?').run(req.params.id);
      const insertGenre = db.prepare('INSERT INTO dex_entry_genre (id, name, dex_entry_id) VALUES (?, ?, ?)');
      for (const genre of genres) {
        const genreId = typeof genre === 'string' ? uuidv4() : genre.id || uuidv4();
        const genreName = typeof genre === 'string' ? genre : genre.name;
        insertGenre.run(genreId, genreName, req.params.id);
      }
    }
  });

  updateEntry();
  const row = db.prepare('SELECT * FROM dex_entry WHERE id = ?').get(req.params.id) as DexEntryRow;
  const genreRows = db.prepare('SELECT * FROM dex_entry_genre WHERE dex_entry_id = ?').all(req.params.id) as DexEntryGenreRow[];
  res.json({ data: formatDexEntry(row, genreRows) });
});

router.delete('/entries/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM dex_entry WHERE id = ?').get(req.params.id) as DexEntryRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Dex entry not found' });
    return;
  }

  db.prepare('DELETE FROM dex_entry WHERE id = ?').run(req.params.id);
  res.json({ message: 'Dex entry deleted' });
});

router.get('/stats', (_req: Request, res: Response) => {
  let row = db.prepare('SELECT * FROM dex_stats LIMIT 1').get() as DexStatsRow | undefined;
  if (!row) {
    const id = uuidv4();
    db.prepare(
      `INSERT INTO dex_stats (id, total, by_category, by_status, average_rating) VALUES (?, 0, '{}', '{}', 0.0)`
    ).run(id);
    row = db.prepare('SELECT * FROM dex_stats WHERE id = ?').get(id) as DexStatsRow;
  }
  res.json({ data: formatDexStats(row) });
});

router.post('/stats/refresh', (_req: Request, res: Response) => {
  let row = db.prepare('SELECT * FROM dex_stats LIMIT 1').get() as DexStatsRow | undefined;
  if (!row) {
    const id = uuidv4();
    db.prepare(
      `INSERT INTO dex_stats (id, total, by_category, by_status, average_rating) VALUES (?, 0, '{}', '{}', 0.0)`
    ).run(id);
    row = db.prepare('SELECT * FROM dex_stats WHERE id = ?').get(id) as DexStatsRow;
  }

  const total = (db.prepare('SELECT COUNT(*) as count FROM dex_entry').get() as { count: number }).count;

  const byCategoryRows = db.prepare('SELECT category, COUNT(*) as count FROM dex_entry GROUP BY category').all() as { category: string; count: number }[];
  const byCategory: Record<string, number> = {};
  for (const r of byCategoryRows) {
    byCategory[r.category] = r.count;
  }

  const byStatusRows = db.prepare('SELECT status, COUNT(*) as count FROM dex_entry GROUP BY status').all() as { status: string; count: number }[];
  const byStatus: Record<string, number> = {};
  for (const r of byStatusRows) {
    byStatus[r.status] = r.count;
  }

  const avgRow = db.prepare('SELECT AVG(rating) as avg FROM dex_entry').get() as { avg: number | null };
  const averageRating = avgRow.avg ?? 0;

  db.prepare(
    `UPDATE dex_stats SET total = ?, by_category = ?, by_status = ?, average_rating = ?, updated_at = datetime('now') WHERE id = ?`
  ).run(total, JSON.stringify(byCategory), JSON.stringify(byStatus), averageRating, row.id);

  const updated = db.prepare('SELECT * FROM dex_stats WHERE id = ?').get(row.id) as DexStatsRow;
  res.json({ data: formatDexStats(updated) });
});

export default router;
