import uuid

from sqlalchemy import Boolean, String, Text, JSON, Enum as SAEnum
from sqlalchemy.orm import Mapped, mapped_column

from app.database import Base


def _uuid():
    return str(uuid.uuid4())


class Project(Base):
    __tablename__ = "projects"

    id: Mapped[str] = mapped_column(String, primary_key=True, default=_uuid)
    name: Mapped[str] = mapped_column(String, nullable=False)
    description: Mapped[str | None] = mapped_column(Text, nullable=True)
    tech: Mapped[str | None] = mapped_column(JSON, nullable=True, default=list)
    link: Mapped[str | None] = mapped_column(String, nullable=True)
    github: Mapped[str | None] = mapped_column(String, nullable=True)
    image: Mapped[str | None] = mapped_column(String, nullable=True)
    status: Mapped[str] = mapped_column(
        SAEnum("completed", "in-progress", "archived", name="project_status_enum"),
        default="in-progress",
    )
