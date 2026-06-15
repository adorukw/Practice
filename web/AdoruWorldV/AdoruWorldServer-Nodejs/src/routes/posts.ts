import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';

const router = Router();

interface PostRow {
  id: string;
  slug: string;
  title: string;
  description: string | null;
  content: string;
  cover_image: string | null;
  created_at: string;
  updated_at: string;
  published: number;
  word_count: number;
  views: number;
  featured: number;
}

interface CategoryRow {
  id: string;
  name: string;
  slug: string;
  description: string | null;
  count: number;
  icon: string | null;
  color: string | null;
}

interface TagRow {
  id: string;
  name: string;
  slug: string;
  color: string | null;
  count: number;
}

function formatPost(row: PostRow) {
  return {
    id: row.id,
    slug: row.slug,
    title: row.title,
    description: row.description,
    content: row.content,
    coverImage: row.cover_image,
    createdAt: row.created_at,
    updatedAt: row.updated_at,
    published: Boolean(row.published),
    wordCount: row.word_count,
    views: row.views,
    featured: Boolean(row.featured),
  };
}

router.get('/', (_req: Request, res: Response) => {
  const { published, featured, page = '1', limit = '10' } = _req.query;
  let sql = 'SELECT * FROM post WHERE 1=1';
  const params: unknown[] = [];

  if (published !== undefined) {
    sql += ' AND published = ?';
    params.push(published === 'true' ? 1 : 0);
  }
  if (featured !== undefined) {
    sql += ' AND featured = ?';
    params.push(featured === 'true' ? 1 : 0);
  }

  const countRow = db.prepare(`SELECT COUNT(*) as total FROM (${sql})`).get(...params) as { total: number };
  const total = countRow.total;

  const offset = (Number(page) - 1) * Number(limit);
  sql += ' ORDER BY created_at DESC LIMIT ? OFFSET ?';
  params.push(Number(limit), offset);

  const rows = db.prepare(sql).all(...params) as PostRow[];
  const posts = rows.map(formatPost);

  res.json({ data: posts, total, page: Number(page), limit: Number(limit) });
});

router.get('/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM post WHERE id = ? OR slug = ?').get(req.params.id, req.params.id) as PostRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Post not found' });
    return;
  }

  const categories = db.prepare(
    `SELECT pc.* FROM post_category pc
     INNER JOIN post_category_relation pcr ON pc.id = pcr.category_id
     WHERE pcr.post_id = ?`
  ).all(row.id) as CategoryRow[];

  const tags = db.prepare(
    `SELECT pt.* FROM post_tag pt
     INNER JOIN post_tag_relation ptr ON pt.id = ptr.tag_id
     WHERE ptr.post_id = ?`
  ).all(row.id) as TagRow[];

  res.json({ data: { ...formatPost(row), categories, tags } });
});

router.post('/', (req: Request, res: Response) => {
  const { slug, title, description, content, coverImage, published, wordCount, featured, categoryIds, tagIds } = req.body;

  if (!slug || !title || !content) {
    res.status(400).json({ error: 'slug, title, and content are required' });
    return;
  }

  const existing = db.prepare('SELECT id FROM post WHERE slug = ?').get(slug);
  if (existing) {
    res.status(409).json({ error: 'Slug already exists' });
    return;
  }

  const id = uuidv4();
  const now = new Date().toISOString();

  const insertPost = db.transaction(() => {
    db.prepare(
      `INSERT INTO post (id, slug, title, description, content, cover_image, created_at, updated_at, published, word_count, views, featured)
       VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`
    ).run(id, slug, title, description || null, content, coverImage || null, now, now, published ? 1 : 0, wordCount || 0, 0, featured ? 1 : 0);

    if (categoryIds && Array.isArray(categoryIds)) {
      const insertCat = db.prepare('INSERT INTO post_category_relation (post_id, category_id) VALUES (?, ?)');
      for (const catId of categoryIds) {
        insertCat.run(id, catId);
      }
      for (const catId of categoryIds) {
        db.prepare('UPDATE post_category SET count = count + 1 WHERE id = ?').run(catId);
      }
    }

    if (tagIds && Array.isArray(tagIds)) {
      const insertTag = db.prepare('INSERT INTO post_tag_relation (post_id, tag_id) VALUES (?, ?)');
      for (const tagId of tagIds) {
        insertTag.run(id, tagId);
      }
      for (const tagId of tagIds) {
        db.prepare('UPDATE post_tag SET count = count + 1 WHERE id = ?').run(tagId);
      }
    }
  });

  insertPost();
  const row = db.prepare('SELECT * FROM post WHERE id = ?').get(id) as PostRow;
  res.status(201).json({ data: formatPost(row) });
});

router.put('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM post WHERE id = ?').get(req.params.id) as PostRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Post not found' });
    return;
  }

  const { slug, title, description, content, coverImage, published, wordCount, featured, categoryIds, tagIds } = req.body;

  if (slug && slug !== existing.slug) {
    const slugExists = db.prepare('SELECT id FROM post WHERE slug = ? AND id != ?').get(slug, req.params.id);
    if (slugExists) {
      res.status(409).json({ error: 'Slug already exists' });
      return;
    }
  }

  const now = new Date().toISOString();

  const updatePost = db.transaction(() => {
    db.prepare(
      `UPDATE post SET slug = ?, title = ?, description = ?, content = ?, cover_image = ?, updated_at = ?, published = ?, word_count = ?, featured = ?
       WHERE id = ?`
    ).run(
      slug ?? existing.slug,
      title ?? existing.title,
      description ?? existing.description,
      content ?? existing.content,
      coverImage ?? existing.cover_image,
      now,
      published !== undefined ? (published ? 1 : 0) : existing.published,
      wordCount ?? existing.word_count,
      featured !== undefined ? (featured ? 1 : 0) : existing.featured,
      req.params.id
    );

    if (categoryIds !== undefined) {
      const oldCats = db.prepare('SELECT category_id FROM post_category_relation WHERE post_id = ?').all(req.params.id) as { category_id: string }[];
      for (const c of oldCats) {
        db.prepare('UPDATE post_category SET count = MAX(0, count - 1) WHERE id = ?').run(c.category_id);
      }
      db.prepare('DELETE FROM post_category_relation WHERE post_id = ?').run(req.params.id);
      const insertCat = db.prepare('INSERT INTO post_category_relation (post_id, category_id) VALUES (?, ?)');
      for (const catId of categoryIds) {
        insertCat.run(req.params.id, catId);
        db.prepare('UPDATE post_category SET count = count + 1 WHERE id = ?').run(catId);
      }
    }

    if (tagIds !== undefined) {
      const oldTags = db.prepare('SELECT tag_id FROM post_tag_relation WHERE post_id = ?').all(req.params.id) as { tag_id: string }[];
      for (const t of oldTags) {
        db.prepare('UPDATE post_tag SET count = MAX(0, count - 1) WHERE id = ?').run(t.tag_id);
      }
      db.prepare('DELETE FROM post_tag_relation WHERE post_id = ?').run(req.params.id);
      const insertTag = db.prepare('INSERT INTO post_tag_relation (post_id, tag_id) VALUES (?, ?)');
      for (const tagId of tagIds) {
        insertTag.run(req.params.id, tagId);
        db.prepare('UPDATE post_tag SET count = count + 1 WHERE id = ?').run(tagId);
      }
    }
  });

  updatePost();
  const row = db.prepare('SELECT * FROM post WHERE id = ?').get(req.params.id) as PostRow;
  res.json({ data: formatPost(row) });
});

router.delete('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM post WHERE id = ?').get(req.params.id) as PostRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Post not found' });
    return;
  }

  const deletePost = db.transaction(() => {
    const oldCats = db.prepare('SELECT category_id FROM post_category_relation WHERE post_id = ?').all(req.params.id) as { category_id: string }[];
    const oldTags = db.prepare('SELECT tag_id FROM post_tag_relation WHERE post_id = ?').all(req.params.id) as { tag_id: string }[];

    for (const c of oldCats) {
      db.prepare('UPDATE post_category SET count = MAX(0, count - 1) WHERE id = ?').run(c.category_id);
    }
    for (const t of oldTags) {
      db.prepare('UPDATE post_tag SET count = MAX(0, count - 1) WHERE id = ?').run(t.tag_id);
    }

    db.prepare('DELETE FROM post WHERE id = ?').run(req.params.id);
  });

  deletePost();
  res.json({ message: 'Post deleted' });
});

router.post('/:id/view', (req: Request, res: Response) => {
  db.prepare('UPDATE post SET views = views + 1 WHERE id = ?').run(req.params.id);
  const row = db.prepare('SELECT * FROM post WHERE id = ?').get(req.params.id) as PostRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Post not found' });
    return;
  }
  res.json({ data: formatPost(row) });
});

export default router;
