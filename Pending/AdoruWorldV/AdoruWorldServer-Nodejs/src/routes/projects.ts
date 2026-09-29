import { Router, type Request, type Response } from 'express';
import { v4 as uuidv4 } from 'uuid';
import db from '../db';
import type { ProjectStatus } from '../types';

const router = Router();

interface ProjectRow {
  id: string;
  name: string;
  description: string | null;
  link: string | null;
  github: string | null;
  image: string | null;
  status: ProjectStatus;
}

interface ProjectTechRow {
  id: string;
  name: string;
  project_id: string;
}

function formatProject(row: ProjectRow, techs: ProjectTechRow[] = []) {
  return {
    id: row.id,
    name: row.name,
    description: row.description,
    link: row.link,
    github: row.github,
    image: row.image,
    status: row.status,
    techs: techs.map((t) => ({ id: t.id, name: t.name, projectId: t.project_id })),
  };
}

router.get('/', (req: Request, res: Response) => {
  const { status } = req.query;
  let sql = 'SELECT * FROM project WHERE 1=1';
  const params: unknown[] = [];

  if (status) {
    sql += ' AND status = ?';
    params.push(status);
  }

  sql += ' ORDER BY name';
  const rows = db.prepare(sql).all(...params) as ProjectRow[];
  const result = rows.map((row) => {
    const techs = db.prepare('SELECT * FROM project_tech WHERE project_id = ?').all(row.id) as ProjectTechRow[];
    return formatProject(row, techs);
  });

  res.json({ data: result });
});

router.get('/:id', (req: Request, res: Response) => {
  const row = db.prepare('SELECT * FROM project WHERE id = ?').get(req.params.id) as ProjectRow | undefined;
  if (!row) {
    res.status(404).json({ error: 'Project not found' });
    return;
  }

  const techs = db.prepare('SELECT * FROM project_tech WHERE project_id = ?').all(row.id) as ProjectTechRow[];
  res.json({ data: formatProject(row, techs) });
});

router.post('/', (req: Request, res: Response) => {
  const { name, description, link, github, image, status, techs } = req.body;

  if (!name) {
    res.status(400).json({ error: 'name is required' });
    return;
  }

  const validStatuses: ProjectStatus[] = ['COMPLETED', 'IN_PROGRESS', 'ARCHIVED'];
  const projectStatus: ProjectStatus = validStatuses.includes(status) ? status : 'IN_PROGRESS';

  const id = uuidv4();

  const insertProject = db.transaction(() => {
    db.prepare(
      `INSERT INTO project (id, name, description, link, github, image, status) VALUES (?, ?, ?, ?, ?, ?, ?)`
    ).run(id, name, description || null, link || null, github || null, image || null, projectStatus);

    if (techs && Array.isArray(techs)) {
      const insertTech = db.prepare('INSERT INTO project_tech (id, name, project_id) VALUES (?, ?, ?)');
      for (const tech of techs) {
        const techId = typeof tech === 'string' ? uuidv4() : tech.id || uuidv4();
        const techName = typeof tech === 'string' ? tech : tech.name;
        insertTech.run(techId, techName, id);
      }
    }
  });

  insertProject();
  const row = db.prepare('SELECT * FROM project WHERE id = ?').get(id) as ProjectRow;
  const techRows = db.prepare('SELECT * FROM project_tech WHERE project_id = ?').all(id) as ProjectTechRow[];
  res.status(201).json({ data: formatProject(row, techRows) });
});

router.put('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM project WHERE id = ?').get(req.params.id) as ProjectRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Project not found' });
    return;
  }

  const { name, description, link, github, image, status, techs } = req.body;

  const validStatuses: ProjectStatus[] = ['COMPLETED', 'IN_PROGRESS', 'ARCHIVED'];
  const projectStatus = status && validStatuses.includes(status) ? status : existing.status;

  const updateProject = db.transaction(() => {
    db.prepare(
      `UPDATE project SET name = ?, description = ?, link = ?, github = ?, image = ?, status = ? WHERE id = ?`
    ).run(
      name ?? existing.name,
      description ?? existing.description,
      link ?? existing.link,
      github ?? existing.github,
      image ?? existing.image,
      projectStatus,
      req.params.id
    );

    if (techs !== undefined) {
      db.prepare('DELETE FROM project_tech WHERE project_id = ?').run(req.params.id);
      const insertTech = db.prepare('INSERT INTO project_tech (id, name, project_id) VALUES (?, ?, ?)');
      for (const tech of techs) {
        const techId = typeof tech === 'string' ? uuidv4() : tech.id || uuidv4();
        const techName = typeof tech === 'string' ? tech : tech.name;
        insertTech.run(techId, techName, req.params.id);
      }
    }
  });

  updateProject();
  const row = db.prepare('SELECT * FROM project WHERE id = ?').get(req.params.id) as ProjectRow;
  const techRows = db.prepare('SELECT * FROM project_tech WHERE project_id = ?').all(req.params.id) as ProjectTechRow[];
  res.json({ data: formatProject(row, techRows) });
});

router.delete('/:id', (req: Request, res: Response) => {
  const existing = db.prepare('SELECT * FROM project WHERE id = ?').get(req.params.id) as ProjectRow | undefined;
  if (!existing) {
    res.status(404).json({ error: 'Project not found' });
    return;
  }

  db.prepare('DELETE FROM project WHERE id = ?').run(req.params.id);
  res.json({ message: 'Project deleted' });
});

export default router;
