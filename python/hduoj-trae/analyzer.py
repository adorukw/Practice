import json
import re
from typing import Optional, Tuple
import requests

from langchain_community.llms import Ollama
from langchain_community.chat_models import ChatOpenAI
from langchain_core.prompts import PromptTemplate

import config
from utils import Problem, AnalysisResult


class ProblemAnalyzer:
    def __init__(self, provider: str = None):
        self.llm = None
        self.provider = provider or config.LLM_PROVIDER
        self.prompt_template = PromptTemplate.from_template(config.ANALYSIS_PROMPT_TEMPLATE)

    def _init_llm(self) -> bool:
        try:
            if self.provider == "ollama":
                self.llm = Ollama(
                    model=config.OLLAMA_MODEL,
                    base_url=config.OLLAMA_BASE_URL
                )
            elif self.provider == "openai":
                if not config.OPENAI_API_KEY:
                    print("OpenAI API Key未设置")
                    return False
                self.llm = ChatOpenAI(
                    model=config.OPENAI_MODEL,
                    openai_api_key=config.OPENAI_API_KEY,
                    openai_api_base=config.OPENAI_BASE_URL,
                    temperature=0.1
                )
            elif self.provider == "dashscope":
                if not config.DASHSCOPE_API_KEY:
                    print("DashScope API Key未设置")
                    return False
                self.llm = ChatOpenAI(
                    model=config.DASHSCOPE_MODEL,
                    openai_api_key=config.DASHSCOPE_API_KEY,
                    openai_api_base="https://dashscope.aliyuncs.com/compatible-mode/v1",
                    temperature=0.1
                )
            elif self.provider == "deepseek":
                if not config.DEEPSEEK_API_KEY:
                    print("DeepSeek API Key未设置")
                    return False
                self.llm = ChatOpenAI(
                    model=config.DEEPSEEK_MODEL,
                    openai_api_key=config.DEEPSEEK_API_KEY,
                    openai_api_base=config.DEEPSEEK_BASE_URL,
                    temperature=0.1
                )
            else:
                print(f"不支持的LLM提供商: {self.provider}")
                return False
            return True
        except Exception as e:
            print(f"初始化LLM失败: {e}")
            return False

    def check_connection(self) -> Tuple[bool, str]:
        if self.provider == "ollama":
            return self._check_ollama_connection()
        elif self.provider in ["openai", "dashscope", "deepseek"]:
            return self._check_api_connection()
        else:
            return False, f"不支持的LLM提供商: {self.provider}"

    def _check_ollama_connection(self) -> Tuple[bool, str]:
        try:
            response = requests.get(f"{config.OLLAMA_BASE_URL}/api/tags", timeout=5)
            if response.status_code == 200:
                models = response.json().get("models", [])
                model_names = [m.get("name", "") for m in models]
                if config.OLLAMA_MODEL in model_names:
                    return True, f"Ollama服务正常，模型 {config.OLLAMA_MODEL} 可用"
                else:
                    return False, f"模型 {config.OLLAMA_MODEL} 未找到，可用模型: {', '.join(model_names)}"
            return False, "Ollama服务响应异常"
        except requests.exceptions.ConnectionError:
            return False, "无法连接到Ollama服务，请确保Ollama已启动"
        except Exception as e:
            return False, f"检查Ollama服务时出错: {str(e)}"

    def _check_api_connection(self) -> Tuple[bool, str]:
        provider_config = config.LLM_CONFIG.get(self.provider, {})
        api_key = provider_config.get("api_key", "")
        model = provider_config.get("model", "")
        
        if not api_key:
            return False, f"{self.provider.upper()}_API_KEY 未设置，请在环境变量中配置"
        
        return True, f"{self.provider.upper()} API已配置，模型: {model}"

    def analyze_problem(self, problem: Problem) -> Optional[AnalysisResult]:
        if not self.llm:
            if not self._init_llm():
                return None

        try:
            prompt = self.prompt_template.format(
                problem_id=problem.problem_id,
                title=problem.title,
                problem_description=problem.problem_description,
                input=problem.input,
                output=problem.output,
                sample_input=problem.sample_input,
                sample_output=problem.sample_output,
                source=problem.source
            )

            response = self.llm.invoke(prompt)
            
            if hasattr(response, 'content'):
                response_text = response.content
            else:
                response_text = str(response)
            
            json_str = self._extract_json(response_text)
            
            if json_str:
                data = json.loads(json_str)
                return AnalysisResult.from_dict(data)
            
            return None

        except Exception as e:
            print(f"分析题目 {problem.problem_id} 时出错: {e}")
            return None

    def _extract_json(self, text: str) -> Optional[str]:
        json_pattern = r'\{[^{}]*"algorithms"[^{}]*\}'
        matches = re.findall(json_pattern, text, re.DOTALL)
        
        if matches:
            return matches[0]
        
        json_pattern = r'\{[\s\S]*?\}'
        matches = re.findall(json_pattern, text)
        
        for match in matches:
            try:
                data = json.loads(match)
                if "algorithms" in data:
                    return match
            except json.JSONDecodeError:
                continue
        
        return None

    def analyze_batch(
        self, 
        problems: list, 
        start_index: int = 0, 
        batch_size: int = 10,
        skip_analyzed: bool = True,
        analyzed_problems: dict = None,
        progress_callback=None
    ) -> dict:
        if analyzed_problems is None:
            analyzed_problems = {}
        
        if not self.llm:
            if not self._init_llm():
                return analyzed_problems

        end_index = min(start_index + batch_size, len(problems))
        analyzed_count = 0

        for i in range(start_index, end_index):
            problem = problems[i]
            
            if skip_analyzed and problem.problem_id in analyzed_problems:
                if analyzed_problems[problem.problem_id].is_analyzed():
                    if progress_callback:
                        progress_callback(i, problem.problem_id, "skipped")
                    continue

            if progress_callback:
                progress_callback(i, problem.problem_id, "analyzing")

            result = self.analyze_problem(problem)
            
            if result:
                from utils import AnalyzedProblem
                analyzed_problems[problem.problem_id] = AnalyzedProblem(
                    problem=problem,
                    analysis=result
                )
                analyzed_count += 1
                if progress_callback:
                    progress_callback(i, problem.problem_id, "completed")
            else:
                if progress_callback:
                    progress_callback(i, problem.problem_id, "failed")

        return analyzed_problems
