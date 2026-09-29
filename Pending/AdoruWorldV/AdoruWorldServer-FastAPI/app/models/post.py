import uuid
from datetime import datetime, timezone

from sqlalchemy import Boolean, DateTime, Float, ForeignKey, Integer, String, Text, JSON, Enum as SAEnum, Table, Column
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.database import Base


def _utcnow():
    return datetime.now(timezone.utc)


def _uuid():
    return str(uuid.uuid4())


post_categories = Table(
    "post_categories",
    Base.metadata,
    Column("post_id", String, ForeignKey("posts.id", ondelete="CASCADE"), primary_key=True),
    Column("category_id", String, ForeignKey("categories.id", ondelete="CASCADE"), primary_key=True),
)

post_tags = Table(
    "post_tags",
    Base.metadata,
    Column("post_id", String, ForeignKey("posts.id", ondelete="CASCADE"), primary_key=True),
    Column("tag_id", String, ForeignKey("tags.id", ondelete="CASCADE"), primary_key=True),
)


class Category(Base):
    __tablename__ = "categories"

    id: Mapped[str] = mapped_column(String, primary_key=True, default=_uuid)
    name: Mapped[str] = mapped_column(String, nullable=False)
    slug: Mapped[str] = mapped_column(String, unique=True, nullable=False)
    description: Mapped[str | None] = mapped_column(String, nullable=True)
    icon: Mapped[str | None] = mapped_column(String, nullable=True)
    color: Mapped[str | None] = mapped_column(String, nullable=True)

    posts: Mapped[list["Post"]] = relationship(secondary=post_categories, back_populates="categories")


class Tag(Base):
    __tablename__ = "tags"

    id: Mapped[str] = mapped_column(String, primary_key=True, default=_uuid)
    name: Mapped[str] = mapped_column(String, nullable=False)
    slug: Mapped[str] = mapped_column(String, unique=True, nullable=False)
    color: Mapped[str | None] = mapped_column(String, nullable=True)

    posts: Mapped[list["Post"]] = relationship(secondary=post_tags, back_populates="tags")


class Post(Base):
    __tablename__ = "posts"

    id: Mapped[str] = mapped_column(String, primary_key=True, default=_uuid)
    slug: Mapped[str] = mapped_column(String, unique=True, nullable=False)
    title: Mapped[str] = mapped_column(String, nullable=False)
    description: Mapped[str | None] = mapped_column(Text, nullable=True)
    content: Mapped[str] = mapped_column(Text, nullable=False)
    cover_image: Mapped[str | None] = mapped_column(String, nullable=True)
    created_at: Mapped[datetime] = mapped_column(DateTime, default=_utcnow)
    updated_at: Mapped[datetime] = mapped_column(DateTime, default=_utcnow, onupdate=_utcnow)
    published: Mapped[bool] = mapped_column(Boolean, default=False)
    reading_time: Mapped[int] = mapped_column(Integer, default=0)
    word_count: Mapped[int] = mapped_column(Integer, default=0)
    views: Mapped[int] = mapped_column(Integer, default=0)
    featured: Mapped[bool] = mapped_column(Boolean, default=False)

    categories: Mapped[list["Category"]] = relationship(secondary=post_categories, back_populates="posts", lazy="selectin")
    tags: Mapped[list["Tag"]] = relationship(secondary=post_tags, back_populates="posts", lazy="selectin")
