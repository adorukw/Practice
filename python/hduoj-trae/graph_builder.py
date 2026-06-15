import json
from typing import List, Dict, Any, Set
import networkx as nx
from pyvis.network import Network

import config
from utils import AnalyzedProblem


class KnowledgeGraph:
    def __init__(self):
        self.graph = nx.Graph()
        self.node_types = {
            "problem": "题目",
            "algorithm": "算法",
            "data_structure": "数据结构",
            "concept": "概念"
        }

    def build_graph(self, problems: List[AnalyzedProblem]) -> None:
        self.graph.clear()
        
        for ap in problems:
            if not ap.analysis:
                continue
            
            problem_id = f"problem_{ap.problem.problem_id}"
            self.graph.add_node(
                problem_id,
                label=f"P{ap.problem.problem_id}\n{ap.problem.title}",
                type="problem",
                title=f"题目 {ap.problem.problem_id}: {ap.problem.title}\n点击查看原题",
                url=ap.problem.get_hduoj_url(),
                color=config.GRAPH_CONFIG["node_colors"]["problem"],
                size=25
            )
            
            for alg in ap.analysis.algorithms:
                alg_id = f"alg_{alg}"
                if not self.graph.has_node(alg_id):
                    self.graph.add_node(
                        alg_id,
                        label=alg,
                        type="algorithm",
                        title=f"算法: {alg}",
                        color=config.GRAPH_CONFIG["node_colors"]["algorithm"],
                        size=20
                    )
                self.graph.add_edge(problem_id, alg_id)
            
            for ds in ap.analysis.data_structures:
                ds_id = f"ds_{ds}"
                if not self.graph.has_node(ds_id):
                    self.graph.add_node(
                        ds_id,
                        label=ds,
                        type="data_structure",
                        title=f"数据结构: {ds}",
                        color=config.GRAPH_CONFIG["node_colors"]["data_structure"],
                        size=20
                    )
                self.graph.add_edge(problem_id, ds_id)
            
            for concept in ap.analysis.concepts:
                concept_id = f"concept_{concept}"
                if not self.graph.has_node(concept_id):
                    self.graph.add_node(
                        concept_id,
                        label=concept,
                        type="concept",
                        title=f"概念: {concept}",
                        color=config.GRAPH_CONFIG["node_colors"]["concept"],
                        size=15
                    )
                self.graph.add_edge(problem_id, concept_id)

    def get_statistics(self) -> Dict[str, int]:
        type_counts = {"problem": 0, "algorithm": 0, "data_structure": 0, "concept": 0}
        
        for node, data in self.graph.nodes(data=True):
            node_type = data.get("type", "")
            if node_type in type_counts:
                type_counts[node_type] += 1
        
        return {
            "total_nodes": self.graph.number_of_nodes(),
            "total_edges": self.graph.number_of_edges(),
            **type_counts
        }

    def get_node_neighbors(self, node_id: str) -> List[Dict[str, Any]]:
        neighbors = []
        for neighbor in self.graph.neighbors(node_id):
            node_data = self.graph.nodes[neighbor]
            neighbors.append({
                "id": neighbor,
                "label": node_data.get("label", ""),
                "type": node_data.get("type", ""),
                "url": node_data.get("url", "")
            })
        return neighbors

    def visualize(self, output_file: str = None, height: str = "750px", width: str = "100%") -> str:
        if output_file is None:
            output_file = str(config.DATA_DIR / "knowledge_graph.html")
        
        net = Network(
            height=height,
            width=width,
            bgcolor=config.GRAPH_CONFIG["bgcolor"],
            font_color=config.GRAPH_CONFIG["font_color"],
            directed=False
        )
        
        net.from_nx(self.graph)
        
        net.set_options("""
        {
            "physics": {
                "enabled": true,
                "barnesHut": {
                    "gravitationalConstant": -3000,
                    "centralGravity": 0.3,
                    "springLength": 100,
                    "springConstant": 0.04
                }
            },
            "interaction": {
                "hover": true,
                "tooltipDelay": 200,
                "zoomView": true
            },
            "nodes": {
                "font": {
                    "size": 14,
                    "face": "Arial"
                },
                "borderWidth": 2,
                "shadow": true
            },
            "edges": {
                "color": {
                    "inherit": "from"
                },
                "smooth": {
                    "type": "continuous"
                },
                "width": 1.5
            }
        }
        """)
        
        net.save_graph(output_file)
        
        return output_file

    def export_to_json(self, output_file: str = None) -> Dict[str, Any]:
        if output_file is None:
            output_file = str(config.GRAPH_DATA_FILE)
        
        nodes = []
        for node, data in self.graph.nodes(data=True):
            node_info = {
                "id": node,
                "label": data.get("label", ""),
                "type": data.get("type", ""),
                "url": data.get("url", "")
            }
            nodes.append(node_info)
        
        edges = []
        for source, target in self.graph.edges():
            edges.append({"source": source, "target": target})
        
        graph_data = {
            "nodes": nodes,
            "edges": edges,
            "statistics": self.get_statistics()
        }
        
        with open(output_file, "w", encoding="utf-8") as f:
            json.dump(graph_data, f, ensure_ascii=False, indent=2)
        
        return graph_data

    def get_related_problems(self, problem_id: int, max_depth: int = 2) -> List[Dict[str, Any]]:
        node_id = f"problem_{problem_id}"
        
        if not self.graph.has_node(node_id):
            return []
        
        related = set()
        
        for neighbor in self.graph.neighbors(node_id):
            for second_neighbor in self.graph.neighbors(neighbor):
                if second_neighbor.startswith("problem_") and second_neighbor != node_id:
                    related.add(second_neighbor)
        
        result = []
        for rel_id in related:
            node_data = self.graph.nodes[rel_id]
            result.append({
                "id": rel_id,
                "label": node_data.get("label", ""),
                "url": node_data.get("url", "")
            })
        
        return result[:20]
