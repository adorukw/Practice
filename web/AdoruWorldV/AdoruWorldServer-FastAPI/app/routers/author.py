from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.crud import author as crud
from app.schemas.author import Author, AuthorCreate, AuthorUpdate, AuthorSocial

router = APIRouter(prefix="/author", tags=["author"])


def _to_author_schema(row) -> Author:
    return Author(
        name=row.name,
        avatar=row.avatar,
        bio=row.bio,
        location=row.location,
        social=AuthorSocial(github=row.github, email=row.email),
    )


@router.get("", response_model=Author)
async def get_author(db: AsyncSession = Depends(get_db)):
    author = await crud.get_author(db)
    if not author:
        raise HTTPException(status_code=404, detail="Author not found")
    return _to_author_schema(author)


@router.post("", response_model=Author, status_code=201)
async def create_author(data: AuthorCreate, db: AsyncSession = Depends(get_db)):
    existing = await crud.get_author(db)
    if existing:
        raise HTTPException(status_code=400, detail="Author already exists. Use PUT to update.")
    author = await crud.create_author(db, data)
    return _to_author_schema(author)


@router.put("", response_model=Author)
async def update_author(data: AuthorUpdate, db: AsyncSession = Depends(get_db)):
    author = await crud.get_author(db)
    if not author:
        raise HTTPException(status_code=404, detail="Author not found. Create first with POST.")
    author = await crud.update_author(db, author, data)
    return _to_author_schema(author)
