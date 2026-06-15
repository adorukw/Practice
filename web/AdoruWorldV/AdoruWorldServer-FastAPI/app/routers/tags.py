from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.crud import tags as crud
from app.schemas.post import PostTag, PostTagCreate, PostTagUpdate

router = APIRouter(prefix="/tags", tags=["tags"])


@router.get("", response_model=list[PostTag])
async def list_tags(db: AsyncSession = Depends(get_db)):
    rows = await crud.get_tags(db)
    result = []
    for t in rows:
        count = await crud.get_tag_count(db, t.id)
        result.append(PostTag(
            id=t.id, name=t.name, slug=t.slug,
            color=t.color, count=count,
        ))
    return result


@router.get("/slug/{slug}", response_model=PostTag)
async def get_tag_by_slug(slug: str, db: AsyncSession = Depends(get_db)):
    tag = await crud.get_tag_by_slug(db, slug)
    if not tag:
        raise HTTPException(status_code=404, detail="Tag not found")
    count = await crud.get_tag_count(db, tag.id)
    return PostTag(
        id=tag.id, name=tag.name, slug=tag.slug,
        color=tag.color, count=count,
    )


@router.get("/{tag_id}", response_model=PostTag)
async def get_tag(tag_id: str, db: AsyncSession = Depends(get_db)):
    tag = await crud.get_tag_by_id(db, tag_id)
    if not tag:
        raise HTTPException(status_code=404, detail="Tag not found")
    count = await crud.get_tag_count(db, tag.id)
    return PostTag(
        id=tag.id, name=tag.name, slug=tag.slug,
        color=tag.color, count=count,
    )


@router.post("", response_model=PostTag, status_code=201)
async def create_tag(data: PostTagCreate, db: AsyncSession = Depends(get_db)):
    tag = await crud.create_tag(db, data)
    return PostTag(
        id=tag.id, name=tag.name, slug=tag.slug,
        color=tag.color, count=0,
    )


@router.put("/{tag_id}", response_model=PostTag)
async def update_tag(tag_id: str, data: PostTagUpdate, db: AsyncSession = Depends(get_db)):
    tag = await crud.get_tag_by_id(db, tag_id)
    if not tag:
        raise HTTPException(status_code=404, detail="Tag not found")
    tag = await crud.update_tag(db, tag, data)
    count = await crud.get_tag_count(db, tag.id)
    return PostTag(
        id=tag.id, name=tag.name, slug=tag.slug,
        color=tag.color, count=count,
    )


@router.delete("/{tag_id}", status_code=204)
async def delete_tag(tag_id: str, db: AsyncSession = Depends(get_db)):
    tag = await crud.get_tag_by_id(db, tag_id)
    if not tag:
        raise HTTPException(status_code=404, detail="Tag not found")
    await crud.delete_tag(db, tag)
