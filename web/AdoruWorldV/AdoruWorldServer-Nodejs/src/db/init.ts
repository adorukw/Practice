import db from './index';

const createTables = db.transaction(() => {
  db.exec(`
    CREATE TABLE IF NOT EXISTS post (
      id TEXT PRIMARY KEY,
      slug TEXT UNIQUE NOT NULL,
      title TEXT NOT NULL,
      description TEXT,
      content TEXT NOT NULL,
      cover_image TEXT,
      created_at TEXT NOT NULL DEFAULT (datetime('now')),
      updated_at TEXT NOT NULL DEFAULT (datetime('now')),
      published INTEGER NOT NULL DEFAULT 0,
      word_count INTEGER NOT NULL DEFAULT 0,
      views INTEGER NOT NULL DEFAULT 0,
      featured INTEGER NOT NULL DEFAULT 0
    );

    CREATE INDEX IF NOT EXISTS idx_post_slug ON post(slug);
    CREATE INDEX IF NOT EXISTS idx_post_created_at ON post(created_at);
    CREATE INDEX IF NOT EXISTS idx_post_published ON post(published);
    CREATE INDEX IF NOT EXISTS idx_post_featured ON post(featured);

    CREATE TABLE IF NOT EXISTS post_category (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      slug TEXT UNIQUE NOT NULL,
      description TEXT,
      count INTEGER NOT NULL DEFAULT 0,
      icon TEXT,
      color TEXT
    );

    CREATE INDEX IF NOT EXISTS idx_post_category_slug ON post_category(slug);

    CREATE TABLE IF NOT EXISTS post_tag (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      slug TEXT UNIQUE NOT NULL,
      color TEXT,
      count INTEGER NOT NULL DEFAULT 0
    );

    CREATE INDEX IF NOT EXISTS idx_post_tag_slug ON post_tag(slug);

    CREATE TABLE IF NOT EXISTS post_category_relation (
      post_id TEXT NOT NULL,
      category_id TEXT NOT NULL,
      PRIMARY KEY (post_id, category_id),
      FOREIGN KEY (post_id) REFERENCES post(id) ON DELETE CASCADE,
      FOREIGN KEY (category_id) REFERENCES post_category(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS post_tag_relation (
      post_id TEXT NOT NULL,
      tag_id TEXT NOT NULL,
      PRIMARY KEY (post_id, tag_id),
      FOREIGN KEY (post_id) REFERENCES post(id) ON DELETE CASCADE,
      FOREIGN KEY (tag_id) REFERENCES post_tag(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS archive_item (
      id TEXT PRIMARY KEY,
      year INTEGER NOT NULL,
      month INTEGER NOT NULL,
      UNIQUE(year, month)
    );

    CREATE INDEX IF NOT EXISTS idx_archive_item_year_month ON archive_item(year, month);

    CREATE TABLE IF NOT EXISTS archive_item_post_relation (
      archive_item_id TEXT NOT NULL,
      post_id TEXT NOT NULL,
      PRIMARY KEY (archive_item_id, post_id),
      FOREIGN KEY (archive_item_id) REFERENCES archive_item(id) ON DELETE CASCADE,
      FOREIGN KEY (post_id) REFERENCES post(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS site_stats (
      id TEXT PRIMARY KEY,
      total_posts INTEGER NOT NULL DEFAULT 0,
      total_words INTEGER NOT NULL DEFAULT 0,
      total_views INTEGER NOT NULL DEFAULT 0,
      running_days INTEGER NOT NULL DEFAULT 0,
      updated_at TEXT NOT NULL DEFAULT (datetime('now'))
    );

    CREATE TABLE IF NOT EXISTS author (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      avatar TEXT NOT NULL,
      bio TEXT NOT NULL,
      location TEXT NOT NULL,
      social TEXT
    );

    CREATE INDEX IF NOT EXISTS idx_author_name ON author(name);

    CREATE TABLE IF NOT EXISTS project (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      description TEXT,
      link TEXT,
      github TEXT,
      image TEXT,
      status TEXT NOT NULL DEFAULT 'IN_PROGRESS' CHECK(status IN ('COMPLETED', 'IN_PROGRESS', 'ARCHIVED'))
    );

    CREATE INDEX IF NOT EXISTS idx_project_status ON project(status);

    CREATE TABLE IF NOT EXISTS project_tech (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      project_id TEXT NOT NULL,
      FOREIGN KEY (project_id) REFERENCES project(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS dex_entry (
      id TEXT PRIMARY KEY,
      slug TEXT UNIQUE NOT NULL,
      title TEXT NOT NULL,
      original_title TEXT,
      cover_image TEXT NOT NULL,
      category TEXT NOT NULL CHECK(category IN ('ANIME', 'MOVIE', 'TV', 'GAME', 'BOOK', 'MUSIC', 'OTHER')),
      status TEXT NOT NULL CHECK(status IN ('COMPLETED', 'WATCHING', 'PLAYING', 'READING', 'LISTENING', 'DROPPED', 'PLANNED')),
      rating INTEGER NOT NULL,
      start_date TEXT,
      finish_date TEXT,
      comments TEXT,
      creator TEXT,
      year INTEGER
    );

    CREATE INDEX IF NOT EXISTS idx_dex_entry_slug ON dex_entry(slug);
    CREATE INDEX IF NOT EXISTS idx_dex_entry_category ON dex_entry(category);
    CREATE INDEX IF NOT EXISTS idx_dex_entry_status ON dex_entry(status);
    CREATE INDEX IF NOT EXISTS idx_dex_entry_rating ON dex_entry(rating);
    CREATE INDEX IF NOT EXISTS idx_dex_entry_start_date ON dex_entry(start_date);
    CREATE INDEX IF NOT EXISTS idx_dex_entry_finish_date ON dex_entry(finish_date);

    CREATE TABLE IF NOT EXISTS dex_entry_genre (
      id TEXT PRIMARY KEY,
      name TEXT NOT NULL,
      dex_entry_id TEXT NOT NULL,
      FOREIGN KEY (dex_entry_id) REFERENCES dex_entry(id) ON DELETE CASCADE
    );

    CREATE TABLE IF NOT EXISTS dex_stats (
      id TEXT PRIMARY KEY,
      total INTEGER NOT NULL DEFAULT 0,
      by_category TEXT NOT NULL DEFAULT '{}',
      by_status TEXT NOT NULL DEFAULT '{}',
      average_rating REAL NOT NULL DEFAULT 0.0,
      updated_at TEXT NOT NULL DEFAULT (datetime('now'))
    );
  `);
});

createTables();
console.log('Database tables created successfully.');
