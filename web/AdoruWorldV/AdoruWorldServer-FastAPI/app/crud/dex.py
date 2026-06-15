from sqlalchemy import func, select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.dex import DexEntry
from app.schemas.dex import DexEntryCreate, DexEntryUpdate


async def get_dex_entries(
    db: AsyncSession,
    category: str | None = None,
    status: str | None = None,
    skip: int = 0,
    limit: int = 20,
) -> list[DexEntry]:
    stmt = select(DexEntry)
    if category:
        stmt = stmt.where(DexEntry.category == category)
    if status:
        stmt = stmt.where(DexEntry.status == status)
    stmt = stmt.order_by(DexEntry.title).offset(skip).limit(limit)
    result = await db.execute(stmt)
    return list(result.scalars().all())


async def get_dex_entry_by_slug(db: AsyncSession, slug: str) -> DexEntry | None:
    result = await db.execute(select(DexEntry).where(DexEntry.slug == slug))
    return result.scalar_one_or_none()


async def get_dex_entry_by_id(db: AsyncSession, entry_id: str) -> DexEntry | None:
    result = await db.execute(select(DexEntry).where(DexEntry.id == entry_id))
    return result.scalar_one_or_none()


async def create_dex_entry(db: AsyncSession, data: DexEntryCreate) -> DexEntry:
    entry = DexEntry(**data.model_dump())
    db.add(entry)
    await db.commit()
    await db.refresh(entry)
    return entry


async def update_dex_entry(db: AsyncSession, entry: DexEntry, data: DexEntryUpdate) -> DexEntry:
    update_data = data.model_dump(exclude_unset=True)
    for key, value in update_data.items():
        setattr(entry, key, value)
    await db.commit()
    await db.refresh(entry)
    return entry


async def delete_dex_entry(db: AsyncSession, entry: DexEntry) -> None:
    await db.delete(entry)
    await db.commit()


async def get_dex_stats(db: AsyncSession) -> dict:
    total_result = await db.execute(select(func.count()).select_from(DexEntry))
    total = total_result.scalar_one()

    cat_result = await db.execute(
        select(DexEntry.category, func.count()).group_by(DexEntry.category)
    )
    by_category = {row[0]: row[1] for row in cat_result.all()}

    status_result = await db.execute(
        select(DexEntry.status, func.count()).group_by(DexEntry.status)
    )
    by_status = {row[0]: row[1] for row in status_result.all()}

    avg_result = await db.execute(select(func.coalesce(func.avg(DexEntry.rating), 0.0)))
    average_rating = round(avg_result.scalar_one(), 1)

    return {
        "total": total,
        "byCategory": by_category,
        "byStatus": by_status,
        "averageRating": average_rating,
    }
