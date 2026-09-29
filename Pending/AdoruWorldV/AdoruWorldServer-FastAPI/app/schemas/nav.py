from pydantic import BaseModel


class NavItemBase(BaseModel):
    name: str
    path: str
    icon: str | None = None
    sort_order: int = 0


class NavItemCreate(NavItemBase):
    pass


class NavItemUpdate(BaseModel):
    name: str | None = None
    path: str | None = None
    icon: str | None = None
    sort_order: int | None = None


class NavItem(NavItemBase):
    id: int

    model_config = {"from_attributes": True}
