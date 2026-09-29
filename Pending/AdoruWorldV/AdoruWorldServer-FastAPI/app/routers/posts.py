from datetime import datetime
from collections import defaultdict

from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.ext.asyncio import AsyncSession

from app.database import get_db
from app.crud import posts as post_crud
from app.crud import categories as cat_crud
from app.crud import tags as tag_crud
from app.schemas.post import (
    Post, PostCreate, PostUpdate, PostListItem,
    PostCategory, PostCategoryCreate, PostCategoryUpdate,
    PostTag, PostTagCreate, PostTagUpdate,
    ArchiveItem,
)
from app.utils import format_post

router = APIRouter(prefix="/posts", tags=["posts"])


@router.get("", response_model=list[PostListItem])
async def list_posts(
    published: bool | None = None,
    featured: bool | None = None,
    category: str | None = None,
    tag: str | None = None,
    skip: int = Query(0, ge=0),
    limit: int = Query(20, ge=1, le=100),
    db: AsyncSession = Depends(get_db),
):
    rows = await post_crud.get_posts(db, published=published, featured=featured,
                                     category_slug=category, tag_slug=tag,
                                     skip=skip, limit=limit)
    return [PostListItem(**format_post(p)) for p in rows]


@router.get("/archives", response_model=list[ArchiveItem])
async def get_archives(db: AsyncSession = Depends(get_db)):
    rows = await post_crud.get_posts(db, published=True, skip=0, limit=10000)
    grouped: dict[tuple[int, int], list] = defaultdict(list)
    for p in rows:
        data = format_post(p)
        dt = p.created_at
        key = (dt.year, dt.month)
        grouped[key].append(PostListItem(**data))
    result = []
    for (year, month), items in sorted(grouped.items(), reverse=True):
        result.append(ArchiveItem(year=year, month=month, posts=items))
    return result


@router.get("/slug/{slug}", response_model=Post)
async def get_post_by_slug(slug: str, db: AsyncSession = Depends(get_db)):
    post = await post_crud.get_post_by_slug(db, slug)
    if not post:
        raise HTTPException(status_code=404, detail="Post not found")
    await post_crud.increment_views(db, post)
    return Post(**format_post(post))


@router.get("/{post_id}", response_model=Post)
async def get_post(post_id: str, db: AsyncSession = Depends(get_db)):
    post = await post_crud.get_post_by_id(db, post_id)
    if not post:
        raise HTTPException(status_code=404, detail="Post not found")
    return Post(**format_post(post))


@router.post("", response_model=Post, status_code=201)
async def create_post(data: PostCreate, db: AsyncSession = Depends(get_db)):
    post = await post_crud.create_post(db, data)
    return Post(**format_post(post))


@router.put("/{post_id}", response_model=Post)
async def update_post(post_id: str, data: PostUpdate, db: AsyncSession = Depends(get_db)):
    post = await post_crud.get_post_by_id(db, post_id)
    if not post:
        raise HTTPException(status_code=404, detail="Post not found")
    post = await post_crud.update_post(db, post, data)
    return Post(**format_post(post))


@router.delete("/{post_id}", status_code=204)
async def delete_post(post_id: str, db: AsyncSession = Depends(get_db)):
    post = await post_crud.get_post_by_id(db, post_id)
    if not post:
        raise HTTPException(status_code=404, detail="Post not found")
    await post_crud.delete_post(db, post)
