from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.crud import categories as crud
from app.schemas.post import PostCategory, PostCategoryCreate, PostCategoryUpdate

router = APIRouter(prefix="/categories", tags=["categories"])


@router.get("", response_model=list[PostCategory])
async def list_categories(db: AsyncSession = Depends(get_db)):
    rows = await crud.get_categories(db)
    result = []
    for c in rows:
        count = await crud.get_category_count(db, c.id)
        result.append(PostCategory(
            id=c.id, name=c.name, slug=c.slug,
            description=c.description, icon=c.icon,
            color=c.color, count=count,
        ))
    return result


@router.get("/slug/{slug}", response_model=PostCategory)
async def get_category_by_slug(slug: str, db: AsyncSession = Depends(get_db)):
    cat = await crud.get_category_by_slug(db, slug)
    if not cat:
        raise HTTPException(status_code=404, detail="Category not found")
    count = await crud.get_category_count(db, cat.id)
    return PostCategory(
        id=cat.id, name=cat.name, slug=cat.slug,
        description=cat.description, icon=cat.icon,
        color=cat.color, count=count,
    )


@router.get("/{category_id}", response_model=PostCategory)
async def get_category(category_id: str, db: AsyncSession = Depends(get_db)):
    cat = await crud.get_category_by_id(db, category_id)
    if not cat:
        raise HTTPException(status_code=404, detail="Category not found")
    count = await crud.get_category_count(db, cat.id)
    return PostCategory(
        id=cat.id, name=cat.name, slug=cat.slug,
        description=cat.description, icon=cat.icon,
        color=cat.color, count=count,
    )


@router.post("", response_model=PostCategory, status_code=201)
async def create_category(data: PostCategoryCreate, db: AsyncSession = Depends(get_db)):
    cat = await crud.create_category(db, data)
    return PostCategory(
        id=cat.id, name=cat.name, slug=cat.slug,
        description=cat.description, icon=cat.icon,
        color=cat.color, count=0,
    )


@router.put("/{category_id}", response_model=PostCategory)
async def update_category(category_id: str, data: PostCategoryUpdate, db: AsyncSession = Depends(get_db)):
    cat = await crud.get_category_by_id(db, category_id)
    if not cat:
        raise HTTPException(status_code=404, detail="Category not found")
    cat = await crud.update_category(db, cat, data)
    count = await crud.get_category_count(db, cat.id)
    return PostCategory(
        id=cat.id, name=cat.name, slug=cat.slug,
        description=cat.description, icon=cat.icon,
        color=cat.color, count=count,
    )


@router.delete("/{category_id}", status_code=204)
async def delete_category(category_id: str, db: AsyncSession = Depends(get_db)):
    cat = await crud.get_category_by_id(db, category_id)
    if not cat:
        raise HTTPException(status_code=404, detail="Category not found")
    await crud.delete_category(db, cat)
