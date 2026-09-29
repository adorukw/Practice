from app.routers.posts import router as posts_router
from app.routers.categories import router as categories_router
from app.routers.tags import router as tags_router
from app.routers.projects import router as projects_router
from app.routers.dex import router as dex_router
from app.routers.author import router as author_router
from app.routers.nav import router as nav_router
from app.routers.stats import router as stats_router

__all__ = [
    "posts_router",
    "categories_router",
    "tags_router",
    "projects_router",
    "dex_router",
    "author_router",
    "nav_router",
    "stats_router",
]
