from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.models.nav import NavItem as NavItemModel
from app.schemas.nav import NavItem, NavItemCreate, NavItemUpdate

router = APIRouter(prefix="/nav", tags=["nav"])


@router.get("", response_model=list[NavItem])
async def list_nav_items(db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(NavItemModel).order_by(NavItemModel.sort_order))
    return list(result.scalars().all())


@router.post("", response_model=NavItem, status_code=201)
async def create_nav_item(data: NavItemCreate, db: AsyncSession = Depends(get_db)):
    item = NavItemModel(**data.model_dump())
    db.add(item)
    await db.commit()
    await db.refresh(item)
    return item


@router.put("/{item_id}", response_model=NavItem)
async def update_nav_item(item_id: int, data: NavItemUpdate, db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(NavItemModel).where(NavItemModel.id == item_id))
    item = result.scalar_one_or_none()
    if not item:
        raise HTTPException(status_code=404, detail="Nav item not found")
    update_data = data.model_dump(exclude_unset=True)
    for key, value in update_data.items():
        setattr(item, key, value)
    await db.commit()
    await db.refresh(item)
    return item


@router.delete("/{item_id}", status_code=204)
async def delete_nav_item(item_id: int, db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(NavItemModel).where(NavItemModel.id == item_id))
    item = result.scalar_one_or_none()
    if not item:
        raise HTTPException(status_code=404, detail="Nav item not found")
    await db.delete(item)
    await db.commit()
