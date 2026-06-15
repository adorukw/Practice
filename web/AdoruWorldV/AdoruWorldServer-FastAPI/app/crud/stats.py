from sqlalchemy import func, select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.post import Post


async def get_total_posts(db: AsyncSession) -> int:
    result = await db.execute(
        select(func.count()).select_from(Post).where(Post.published == True)
    )
    return result.scalar_one()


async def get_total_words(db: AsyncSession) -> int:
    result = await db.execute(
        select(func.coalesce(func.sum(Post.word_count), 0)).where(Post.published == True)
    )
    return result.scalar_one()


async def get_total_views(db: AsyncSession) -> int:
    result = await db.execute(
        select(func.coalesce(func.sum(Post.views), 0)).where(Post.published == True)
    )
    return result.scalar_one()
