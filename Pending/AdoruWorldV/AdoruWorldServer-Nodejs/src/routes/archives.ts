import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';

const router = Router();

interface ArchiveItemRow {
  id: string;
  year: number;
  month: number;
}

function formatArchiveItem(row: ArchiveItemRow) {
  return {
    id: row.id,
    year: row.year,
    month: row.month,
  };
}

router.get('/', (_req: Request, res: Response) => {
  const rows = db.prepare('SELECT * FROM archive_item ORDER BY year DESC, month DESC').all() as ArchiveItemRow[];
  const result = rows.map((row) => {
    const posts = db.prepare(
      `SELECT p.* FROM post p
       INNER JOIN archive_item_post_relation aipr ON p.id = aipr.post_id
       WHERE aipr.archive_item_id = ?
       ORDER BY p.created_at DESC`
    ).all(row.id);
    return { ...formatArchiveItem(row), posts };
  });
  res.json({ data: result });
});

router.get('/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM archive_item WHERE id = ?').get(req.params.id) as ArchiveItemRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Archive item not found' });
    return;
  }

  const posts = db.prepare(
    `SELECT p.* FROM post p
     INNER JOIN archive_item_post_relation aipr ON p.id = aipr.post_id
     WHERE aipr.archive_item_id = ?
     ORDER BY p.created_at DESC`
  ).all(row.id);

  res.json({ data: { ...formatArchiveItem(row), posts } });
});

router.post('/', (req: Request, res: Response) => {
  const { year, month, postIds } = req.body;

  if (year === undefined || month === undefined) {
    res.status(400).json({ error: 'year and month are required' });
    return;
  }

  const existing = db.prepare('SELECT * FROM archive_item WHERE year = ? AND month = ?').get(year, month) as ArchiveItemRow | undefined;
  if (existing) {
    res.status(409).json({ error: 'Archive item for this year/month already exists' });
    return;
  }

  const id = uuidv4();

  const insertArchive = db.transaction(() => {
    db.prepare('INSERT INTO archive_item (id, year, month) VALUES (?, ?, ?)').run(id, year, month);

    if (postIds && Array.isArray(postIds)) {
      const insertRel = db.prepare('INSERT INTO archive_item_post_relation (archive_item_id, post_id) VALUES (?, ?)');
      for (const postId of postIds) {
        insertRel.run(id, postId);
      }
    }
  });

  insertArchive();
  const row = db.prepare('SELECT * FROM archive_item WHERE id = ?').get(id) as ArchiveItemRow;
  res.status(201).json({ data: formatArchiveItem(row) });
});

router.put('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM archive_item WHERE id = ?').get(req.params.id) as ArchiveItemRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Archive item not found' });
    return;
  }

  const { year, month, postIds } = req.body;

  if ((year !== undefined || month !== undefined) && (year ?? existing.year) !== existing.year || (month ?? existing.month) !== existing.month) {
    const newYear = year ?? existing.year;
    const newMonth = month ?? existing.month;
    const conflict = db.prepare('SELECT id FROM archive_item WHERE year = ? AND month = ? AND id != ?').get(newYear, newMonth, req.params.id);
    if (conflict) {
      res.status(409).json({ error: 'Archive item for this year/month already exists' });
      return;
    }
  }

  const updateArchive = db.transaction(() => {
    db.prepare('UPDATE archive_item SET year = ?, month = ? WHERE id = ?').run(
      year ?? existing.year,
      month ?? existing.month,
      req.params.id
    );

    if (postIds !== undefined) {
      db.prepare('DELETE FROM archive_item_post_relation WHERE archive_item_id = ?').run(req.params.id);
      const insertRel = db.prepare('INSERT INTO archive_item_post_relation (archive_item_id, post_id) VALUES (?, ?)');
      for (const postId of postIds) {
        insertRel.run(req.params.id, postId);
      }
    }
  });

  updateArchive();
  const row = db.prepare('SELECT * FROM archive_item WHERE id = ?').get(req.params.id) as ArchiveItemRow;
  res.json({ data: formatArchiveItem(row) });
});

router.delete('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM archive_item WHERE id = ?').get(req.params.id) as ArchiveItemRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Archive item not found' });
    return;
  }

  db.prepare('DELETE FROM archive_item WHERE id = ?').run(req.params.id);
  res.json({ message: 'Archive item deleted' });
});

export default router;
