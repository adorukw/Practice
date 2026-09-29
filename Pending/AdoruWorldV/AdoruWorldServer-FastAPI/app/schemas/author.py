from pydantic import BaseModel


class AuthorSocial(BaseModel):
    github: str | None = None
    email: str | None = None


class AuthorBase(BaseModel):
    name: str
    avatar: str
    bio: str
    location: str
    github: str | None = None
    email: str | None = None


class AuthorCreate(AuthorBase):
    pass


class AuthorUpdate(BaseModel):
    name: str | None = None
    avatar: str | None = None
    bio: str | None = None
    location: str | None = None
    github: str | None = None
    email: str | None = None


class Author(BaseModel):
    name: str
    avatar: str
    bio: str
    location: str
    social: AuthorSocial = AuthorSocial()

    model_config = {"from_attributes": True}
