import uuid

from sqlalchemy import Float, Integer, String, Text, JSON, Enum as SAEnum
from sqlalchemy.orm import Mapped, mapped_column

from app.database import Base


def _uuid():
    return str(uuid.uuid4())


DexCategoryEnum = SAEnum(
    "anime", "movie", "tv", "game", "book", "music", "other",
    name="dex_category_enum",
)

DexStatusEnum = SAEnum(
    "completed", "watching", "playing", "reading", "listening", "dropped", "planned",
    name="dex_status_enum",
)


class DexEntry(Base):
    __tablename__ = "dex_entries"

    id: Mapped[str] = mapped_column(String, primary_key=True, default=_uuid)
    slug: Mapped[str] = mapped_column(String, unique=True, nullable=False)
    title: Mapped[str] = mapped_column(String, nullable=False)
    original_title: Mapped[str | None] = mapped_column(String, nullable=True)
    cover_image: Mapped[str] = mapped_column(String, nullable=False)
    category: Mapped[str] = mapped_column(DexCategoryEnum, nullable=False)
    status: Mapped[str] = mapped_column(DexStatusEnum, nullable=False)
    rating: Mapped[float] = mapped_column(Float, default=0.0)
    start_date: Mapped[str | None] = mapped_column(String, nullable=True)
    finish_date: Mapped[str | None] = mapped_column(String, nullable=True)
    comment: Mapped[str | None] = mapped_column(Text, nullable=True)
    creator: Mapped[str | None] = mapped_column(String, nullable=True)
    year: Mapped[int | None] = mapped_column(Integer, nullable=True)
    genre: Mapped[str | None] = mapped_column(JSON, nullable=True, default=list)
