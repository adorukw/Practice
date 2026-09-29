from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.author import Author
from app.schemas.author import AuthorCreate, AuthorUpdate


async def get_author(db: AsyncSession) -> Author | None:
    result = await db.execute(select(Author).limit(1))
    return result.scalar_one_or_none()


async def create_author(db: AsyncSession, data: AuthorCreate) -> Author:
    author = Author(**data.model_dump())
    db.add(author)
    await db.commit()
    await db.refresh(author)
    return author


async def update_author(db: AsyncSession, author: Author, data: AuthorUpdate) -> Author:
    update_data = data.model_dump(exclude_unset=True)
    for key, value in update_data.items():
        setattr(author, key, value)
    await db.commit()
    await db.refresh(author)
    return author
