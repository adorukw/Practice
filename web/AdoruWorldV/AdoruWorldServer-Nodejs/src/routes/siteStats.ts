import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';

const router = Router();

interface SiteStatsRow {
  id: string;
  total_posts: number;
  total_words: number;
  total_views: number;
  running_days: number;
  updated_at: string;
}

function formatSiteStats(row: SiteStatsRow) {
  return {
    id: row.id,
    totalPosts: row.total_posts,
    totalWords: row.total_words,
    totalViews: row.total_views,
    runningDays: row.running_days,
    updatedAt: row.updated_at,
  };
}

router.get('/', (_req: Request, res: Response) => {
  let row = db.prepare('SELECT * FROM site_stats LIMIT 1').get() as SiteStatsRow | undefined;
  if (!row) {
    const id = uuidv4();
    db.prepare(
      `INSERT INTO site_stats (id, total_posts, total_words, total_views, running_days) VALUES (?, 0, 0, 0, 0)`
    ).run(id);
    row = db.prepare('SELECT * FROM site_stats WHERE id = ?').get(id) as SiteStatsRow;
  }
  res.json({ data: formatSiteStats(row) });
});

router.put('/', (req: Request, res: Response) => {
  let row = db.prepare('SELECT * FROM site_stats LIMIT 1').get() as SiteStatsRow | undefined;
  if (!row) {
    const id = uuidv4();
    db.prepare(
      `INSERT INTO site_stats (id, total_posts, total_words, total_views, running_days) VALUES (?, 0, 0, 0, 0)`
    ).run(id);
    row = db.prepare('SELECT * FROM site_stats WHERE id = ?').get(id) as SiteStatsRow;
  }

  const { totalPosts, totalWords, totalViews, runningDays } = req.body;

  db.prepare(
    `UPDATE site_stats SET total_posts = ?, total_words = ?, total_views = ?, running_days = ?, updated_at = datetime('now') WHERE id = ?`
  ).run(
    totalPosts ?? row.total_posts,
    totalWords ?? row.total_words,
    totalViews ?? row.total_views,
    runningDays ?? row.running_days,
    row.id
  );

  const updated = db.prepare('SELECT * FROM site_stats WHERE id = ?').get(row.id) as SiteStatsRow;
  res.json({ data: formatSiteStats(updated) });
});

router.post('/refresh', (_req: Request, res: Response) => {
  let row = db.prepare('SELECT * FROM site_stats LIMIT 1').get() as SiteStatsRow | undefined;
  if (!row) {
    const id = uuidv4();
    db.prepare(
      `INSERT INTO site_stats (id, total_posts, total_words, total_views, running_days) VALUES (?, 0, 0, 0, 0)`
    ).run(id);
    row = db.prepare('SELECT * FROM site_stats WHERE id = ?').get(id) as SiteStatsRow;
  }

  const postStats = db.prepare(
    `SELECT COALESCE(COUNT(*), 0) as total_posts, COALESCE(SUM(word_count), 0) as total_words, COALESCE(SUM(views), 0) as total_views FROM post WHERE published = 1`
  ).get() as { total_posts: number; total_words: number; total_views: number };

  db.prepare(
    `UPDATE site_stats SET total_posts = ?, total_words = ?, total_views = ?, updated_at = datetime('now') WHERE id = ?`
  ).run(postStats.total_posts, postStats.total_words, postStats.total_views, row.id);

  const updated = db.prepare('SELECT * FROM site_stats WHERE id = ?').get(row.id) as SiteStatsRow;
  res.json({ data: formatSiteStats(updated) });
});

export default router;
