from datetime import date

from fastapi import APIRouter, Depends
from sqlalchemy.ext.asyncio import AsyncSession

from app.config import SITE_START_DATE
from app.database import get_db
from app.crud import stats as crud
from app.schemas.stats import SiteStats

router = APIRouter(prefix="/stats", tags=["stats"])


@router.get("", response_model=SiteStats)
async def get_site_stats(db: AsyncSession = Depends(get_db)):
    total_posts = await crud.get_total_posts(db)
    total_words = await crud.get_total_words(db)
    total_views = await crud.get_total_views(db)
    start = date.fromisoformat(SITE_START_DATE)
    running_days = (date.today() - start).days
    return SiteStats(
        totalPosts=total_posts,
        totalWords=total_words,
        totalViews=total_views,
        runningDays=running_days,
    )
