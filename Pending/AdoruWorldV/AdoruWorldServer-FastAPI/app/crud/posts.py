from sqlalchemy import func, select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import selectinload

from app.models.post import Post, Category, Tag
from app.schemas.post import PostCreate, PostUpdate


async def get_posts(
    db: AsyncSession,
    published: bool | None = None,
    featured: bool | None = None,
    category_slug: str | None = None,
    tag_slug: str | None = None,
    skip: int = 0,
    limit: int = 20,
) -> list[Post]:
    stmt = select(Post).options(
        selectinload(Post.categories),
        selectinload(Post.tags),
    )
    if published is not None:
        stmt = stmt.where(Post.published == published)
    if featured is not None:
        stmt = stmt.where(Post.featured == featured)
    if category_slug:
        stmt = stmt.join(Post.categories).where(Category.slug == category_slug)
    if tag_slug:
        stmt = stmt.join(Post.tags).where(Tag.slug == tag_slug)
    stmt = stmt.order_by(Post.created_at.desc()).offset(skip).limit(limit)
    result = await db.execute(stmt)
    return list(result.scalars().all())


async def get_post_by_slug(db: AsyncSession, slug: str) -> Post | None:
    stmt = select(Post).options(
        selectinload(Post.categories),
        selectinload(Post.tags),
    ).where(Post.slug == slug)
    result = await db.execute(stmt)
    return result.scalar_one_or_none()


async def get_post_by_id(db: AsyncSession, post_id: str) -> Post | None:
    stmt = select(Post).options(
        selectinload(Post.categories),
        selectinload(Post.tags),
    ).where(Post.id == post_id)
    result = await db.execute(stmt)
    return result.scalar_one_or_none()


async def create_post(db: AsyncSession, data: PostCreate) -> Post:
    post_data = data.model_dump(exclude={"category_ids", "tag_ids"})
    post = Post(**post_data)
    if data.category_ids:
        cats = (await db.execute(select(Category).where(Category.id.in_(data.category_ids)))).scalars().all()
        post.categories = list(cats)
    if data.tag_ids:
        tags = (await db.execute(select(Tag).where(Tag.id.in_(data.tag_ids)))).scalars().all()
        post.tags = list(tags)
    db.add(post)
    await db.commit()
    await db.refresh(post)
    return post


async def update_post(db: AsyncSession, post: Post, data: PostUpdate) -> Post:
    update_data = data.model_dump(exclude_unset=True, exclude={"category_ids", "tag_ids"})
    for key, value in update_data.items():
        setattr(post, key, value)
    if data.category_ids is not None:
        cats = (await db.execute(select(Category).where(Category.id.in_(data.category_ids)))).scalars().all()
        post.categories = list(cats)
    if data.tag_ids is not None:
        tags = (await db.execute(select(Tag).where(Tag.id.in_(data.tag_ids)))).scalars().all()
        post.tags = list(tags)
    await db.commit()
    await db.refresh(post)
    return post


async def delete_post(db: AsyncSession, post: Post) -> None:
    await db.delete(post)
    await db.commit()


async def increment_views(db: AsyncSession, post: Post) -> Post:
    post.views += 1
    await db.commit()
    await db.refresh(post)
    return post


async def get_total_posts(db: AsyncSession, published_only: bool = True) -> int:
    stmt = select(func.count()).select_from(Post)
    if published_only:
        stmt = stmt.where(Post.published == True)
    result = await db.execute(stmt)
    return result.scalar_one()


async def get_total_words(db: AsyncSession) -> int:
    stmt = select(func.coalesce(func.sum(Post.word_count), 0)).where(Post.published == True)
    result = await db.execute(stmt)
    return result.scalar_one()


async def get_total_views(db: AsyncSession) -> int:
    stmt = select(func.coalesce(func.sum(Post.views), 0)).where(Post.published == True)
    result = await db.execute(stmt)
    return result.scalar_one()
