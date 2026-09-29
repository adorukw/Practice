from sqlalchemy import func, select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.post import Tag, post_tags
from app.schemas.post import PostTagCreate, PostTagUpdate


async def get_tags(db: AsyncSession) -> list[Tag]:
    result = await db.execute(select(Tag))
    return list(result.scalars().all())


async def get_tag_by_slug(db: AsyncSession, slug: str) -> Tag | None:
    result = await db.execute(select(Tag).where(Tag.slug == slug))
    return result.scalar_one_or_none()


async def get_tag_by_id(db: AsyncSession, tag_id: str) -> Tag | None:
    result = await db.execute(select(Tag).where(Tag.id == tag_id))
    return result.scalar_one_or_none()


async def create_tag(db: AsyncSession, data: PostTagCreate) -> Tag:
    tag = Tag(**data.model_dump())
    db.add(tag)
    await db.commit()
    await db.refresh(tag)
    return tag


async def update_tag(db: AsyncSession, tag: Tag, data: PostTagUpdate) -> Tag:
    update_data = data.model_dump(exclude_unset=True)
    for key, value in update_data.items():
        setattr(tag, key, value)
    await db.commit()
    await db.refresh(tag)
    return tag


async def delete_tag(db: AsyncSession, tag: Tag) -> None:
    await db.delete(tag)
    await db.commit()


async def get_tag_count(db: AsyncSession, tag_id: str) -> int:
    result = await db.execute(
        select(func.count()).select_from(post_tags).where(post_tags.c.tag_id == tag_id)
    )
    return result.scalar_one()
