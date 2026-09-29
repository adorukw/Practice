from pydantic import BaseModel


class ProjectBase(BaseModel):
    name: str
    description: str | None = None
    tech: list[str] | None = None
    link: str | None = None
    github: str | None = None
    image: str | None = None
    status: str = "in-progress"


class ProjectCreate(ProjectBase):
    pass


class ProjectUpdate(BaseModel):
    name: str | None = None
    description: str | None = None
    tech: list[str] | None = None
    link: str | None = None
    github: str | None = None
    image: str | None = None
    status: str | None = None


class Project(ProjectBase):
    id: str

    model_config = {"from_attributes": True}
