from contextlib import asynccontextmanager

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from app.config import API_PREFIX, PROJECT_NAME, VERSION, DESCRIPTION
from app.database import init_db
from app.routers import (
    posts_router,
    categories_router,
    tags_router,
    projects_router,
    dex_router,
    author_router,
    nav_router,
    stats_router,
)


@asynccontextmanager
async def lifespan(app: FastAPI):
    await init_db()
    yield


app = FastAPI(
    title=PROJECT_NAME,
    version=VERSION,
    description=DESCRIPTION,
    lifespan=lifespan,
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(posts_router, prefix=API_PREFIX)
app.include_router(categories_router, prefix=API_PREFIX)
app.include_router(tags_router, prefix=API_PREFIX)
app.include_router(projects_router, prefix=API_PREFIX)
app.include_router(dex_router, prefix=API_PREFIX)
app.include_router(author_router, prefix=API_PREFIX)
app.include_router(nav_router, prefix=API_PREFIX)
app.include_router(stats_router, prefix=API_PREFIX)


@app.get("/")
async def root():
    return {"name": PROJECT_NAME, "version": VERSION}
