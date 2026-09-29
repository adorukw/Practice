from pydantic import BaseModel


class SiteStats(BaseModel):
    totalPosts: int
    totalWords: int
    totalViews: int
    runningDays: int
