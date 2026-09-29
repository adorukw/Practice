from pydantic import BaseModel


DexCategoryType = str
DexStatusType = str


class DexEntryBase(BaseModel):
    slug: str
    title: str
    original_title: str | None = None
    cover_image: str
    category: DexCategoryType
    status: DexStatusType
    rating: float = 0.0
    start_date: str | None = None
    finish_date: str | None = None
    comment: str | None = None
    creator: str | None = None
    year: int | None = None
    genre: list[str] | None = None


class DexEntryCreate(DexEntryBase):
    pass


class DexEntryUpdate(BaseModel):
    slug: str | None = None
    title: str | None = None
    original_title: str | None = None
    cover_image: str | None = None
    category: DexCategoryType | None = None
    status: DexStatusType | None = None
    rating: float | None = None
    start_date: str | None = None
    finish_date: str | None = None
    comment: str | None = None
    creator: str | None = None
    year: int | None = None
    genre: list[str] | None = None


class DexEntry(DexEntryBase):
    id: str

    model_config = {"from_attributes": True}


class DexCategoryInfo(BaseModel):
    id: str
    name: str
    icon: str
    color: str
    bgColor: str


class DexStatusInfo(BaseModel):
    id: str
    name: str
    icon: str
    color: str


class DexStats(BaseModel):
    total: int
    byCategory: dict[str, int]
    byStatus: dict[str, int]
    averageRating: float
