from datetime import datetime

from pydantic import BaseModel, Field


class PostCategoryBase(BaseModel):
    name: str
    slug: str
    description: str | None = None
    icon: str | None = None
    color: str | None = None


class PostCategoryCreate(PostCategoryBase):
    pass


class PostCategoryUpdate(BaseModel):
    name: str | None = None
    slug: str | None = None
    description: str | None = None
    icon: str | None = None
    color: str | None = None


class PostCategory(PostCategoryBase):
    id: str
    count: int = 0

    model_config = {"from_attributes": True}


class PostTagBase(BaseModel):
    name: str
    slug: str
    color: str | None = None


class PostTagCreate(PostTagBase):
    pass


class PostTagUpdate(BaseModel):
    name: str | None = None
    slug: str | None = None
    color: str | None = None


class PostTag(PostTagBase):
    id: str
    count: int = 0

    model_config = {"from_attributes": True}


class PostBase(BaseModel):
    slug: str
    title: str
    description: str | None = None
    content: str
    cover_image: str | None = None
    published: bool = False
    featured: bool = False
    category_ids: list[str] = Field(default_factory=list)
    tag_ids: list[str] = Field(default_factory=list)


class PostCreate(PostBase):
    reading_time: int = 0
    word_count: int = 0


class PostUpdate(BaseModel):
    slug: str | None = None
    title: str | None = None
    description: str | None = None
    content: str | None = None
    cover_image: str | None = None
    published: bool | None = None
    featured: bool | None = None
    reading_time: int | None = None
    word_count: int | None = None
    category_ids: list[str] | None = None
    tag_ids: list[str] | None = None


class Post(BaseModel):
    id: str
    slug: str
    title: str
    description: str | None = None
    content: str
    cover_image: str | None = None
    created_at: str
    updated_at: str
    published: bool
    category: list[str] = Field(default_factory=list)
    tags: list[str] = Field(default_factory=list)
    reading_time: int
    word_count: int
    views: int
    featured: bool

    model_config = {"from_attributes": True}


class PostListItem(BaseModel):
    id: str
    slug: str
    title: str
    description: str | None = None
    cover_image: str | None = None
    created_at: str
    updated_at: str
    published: bool
    category: list[str] = Field(default_factory=list)
    tags: list[str] = Field(default_factory=list)
    reading_time: int
    word_count: int
    views: int
    featured: bool

    model_config = {"from_attributes": True}


class ArchiveItem(BaseModel):
    year: int
    month: int
    posts: list[PostListItem]
