from typing import Any

from app.models.post import Post as PostModel


def format_post(post: PostModel) -> dict[str, Any]:
    return {
        "id": post.id,
        "slug": post.slug,
        "title": post.title,
        "description": post.description,
        "content": post.content,
        "cover_image": post.cover_image,
        "created_at": post.created_at.isoformat() if post.created_at else "",
        "updated_at": post.updated_at.isoformat() if post.updated_at else "",
        "published": post.published,
        "category": [c.slug for c in post.categories],
        "tags": [t.slug for t in post.tags],
        "reading_time": post.reading_time,
        "word_count": post.word_count,
        "views": post.views,
        "featured": post.featured,
    }
