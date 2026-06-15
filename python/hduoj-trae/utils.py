import json
from typing import Dict, List, Optional, Any
from dataclasses import dataclass, asdict
from pathlib import Path
import config


@dataclass
class Problem:
    problem_id: int
    title: str
    time_limit: str
    total_submissions: str
    accepted_submissions: str
    problem_description: str
    input: str
    output: str
    sample_input: str
    sample_output: str
    author: str
    source: str

    @classmethod
    def from_dict(cls, data: Dict) -> "Problem":
        return cls(
            problem_id=data.get("problem_id", 0),
            title=data.get("title", ""),
            time_limit=data.get("time_limit", ""),
            total_submissions=data.get("total_submissions", "0"),
            accepted_submissions=data.get("accepted_submissions", "0"),
            problem_description=data.get("problem_description", ""),
            input=data.get("input", ""),
            output=data.get("output", ""),
            sample_input=data.get("sample_input", ""),
            sample_output=data.get("sample_output", ""),
            author=data.get("author", ""),
            source=data.get("source", "")
        )

    def to_dict(self) -> Dict:
        return asdict(self)

    def get_hduoj_url(self) -> str:
        return config.HDUOJ_PROBLEM_URL.format(problem_id=self.problem_id)

    def get_acceptance_rate(self) -> float:
        try:
            total = int(self.total_submissions)
            accepted = int(self.accepted_submissions)
            if total > 0:
                return (accepted / total) * 100
            return 0.0
        except ValueError:
            return 0.0


@dataclass
class AnalysisResult:
    algorithms: List[str]
    data_structures: List[str]
    concepts: List[str]
    difficulty: str
    analysis: str

    @classmethod
    def from_dict(cls, data: Dict) -> "AnalysisResult":
        return cls(
            algorithms=data.get("algorithms", []),
            data_structures=data.get("data_structures", []),
            concepts=data.get("concepts", []),
            difficulty=data.get("difficulty", "未知"),
            analysis=data.get("analysis", "")
        )

    def to_dict(self) -> Dict:
        return asdict(self)


@dataclass
class AnalyzedProblem:
    problem: Problem
    analysis: Optional[AnalysisResult] = None

    def to_dict(self) -> Dict:
        result = self.problem.to_dict()
        if self.analysis:
            result["analysis"] = self.analysis.to_dict()
        return result

    @classmethod
    def from_dict(cls, data: Dict) -> "AnalyzedProblem":
        problem = Problem.from_dict(data)
        analysis = None
        if "analysis" in data and data["analysis"]:
            analysis = AnalysisResult.from_dict(data["analysis"])
        return cls(problem=problem, analysis=analysis)

    def is_analyzed(self) -> bool:
        return self.analysis is not None


def load_raw_problems() -> List[Problem]:
    file_path = config.RAW_DATA_FILE
    if not file_path.exists():
        return []
    
    with open(file_path, "r", encoding="utf-8") as f:
        data = json.load(f)
    
    return [Problem.from_dict(item) for item in data]


def load_analyzed_problems() -> Dict[int, AnalyzedProblem]:
    file_path = config.ANALYZED_DATA_FILE
    if not file_path.exists():
        return {}
    
    with open(file_path, "r", encoding="utf-8") as f:
        data = json.load(f)
    
    result = {}
    for item in data:
        analyzed = AnalyzedProblem.from_dict(item)
        result[analyzed.problem.problem_id] = analyzed
    
    return result


def save_analyzed_problems(problems: Dict[int, AnalyzedProblem]) -> None:
    file_path = config.ANALYZED_DATA_FILE
    data = [p.to_dict() for p in problems.values()]
    
    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=2)


def merge_problem_with_analysis(
    raw_problems: List[Problem],
    analyzed_problems: Dict[int, AnalyzedProblem]
) -> List[AnalyzedProblem]:
    result = []
    for problem in raw_problems:
        if problem.problem_id in analyzed_problems:
            result.append(analyzed_problems[problem.problem_id])
        else:
            result.append(AnalyzedProblem(problem=problem))
    return result


def get_all_tags(problems: List[AnalyzedProblem]) -> Dict[str, List[str]]:
    algorithms = set()
    data_structures = set()
    concepts = set()
    
    for ap in problems:
        if ap.analysis:
            algorithms.update(ap.analysis.algorithms)
            data_structures.update(ap.analysis.data_structures)
            concepts.update(ap.analysis.concepts)
    
    return {
        "algorithms": sorted(list(algorithms)),
        "data_structures": sorted(list(data_structures)),
        "concepts": sorted(list(concepts))
    }


def filter_problems_by_tags(
    problems: List[AnalyzedProblem],
    algorithms: List[str] = None,
    data_structures: List[str] = None,
    concepts: List[str] = None
) -> List[AnalyzedProblem]:
    result = []
    
    for ap in problems:
        if not ap.analysis:
            continue
        
        match = True
        
        if algorithms:
            if not any(alg in ap.analysis.algorithms for alg in algorithms):
                match = False
        
        if data_structures:
            if not any(ds in ap.analysis.data_structures for ds in data_structures):
                match = False
        
        if concepts:
            if not any(con in ap.analysis.concepts for con in concepts):
                match = False
        
        if match:
            result.append(ap)
    
    return result


def get_statistics(problems: List[AnalyzedProblem]) -> Dict[str, Any]:
    total = len(problems)
    analyzed = sum(1 for p in problems if p.is_analyzed())
    
    difficulty_count = {
        "入门": 0,
        "简单": 0,
        "中等": 0,
        "困难": 0,
        "挑战": 0,
        "未知": 0
    }
    
    for ap in problems:
        if ap.analysis:
            diff = ap.analysis.difficulty
            if diff in difficulty_count:
                difficulty_count[diff] += 1
            else:
                difficulty_count["未知"] += 1
    
    return {
        "total": total,
        "analyzed": analyzed,
        "unanalyzed": total - analyzed,
        "analysis_rate": (analyzed / total * 100) if total > 0 else 0,
        "difficulty_distribution": difficulty_count
    }
