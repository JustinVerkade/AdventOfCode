from collections import defaultdict
from queue import PriorityQueue
from typing import DefaultDict, List, Set


def create_file_tree(terminal_list: List[str]) -> DefaultDict:
    tree: DefaultDict = defaultdict(lambda: {"parent": None, "size": 0, "type": "dir", "depth": 0})
    cwd = "/"
    for line in terminal_list:
        line_split = line.split()
        if line == "$ cd ..":
            cwd = tree[cwd].get("parent", "/")
        elif line == "$ cd /":
            cwd = "/"
        elif line.startswith("$ cd"):
            cwd = f"{cwd}{line_split[-1]}/"
        elif line_split[0] == "dir":
            tree[f"{cwd}{line_split[-1]}/"]["parent"] = cwd
            tree[f"{cwd}{line_split[-1]}/"]["depth"] = tree[cwd]["depth"] + 1
        elif line_split[0].isdigit():
            tree[f"{cwd}{line_split[-1]}"]["parent"] = cwd
            tree[f"{cwd}{line_split[-1]}"]["depth"] = tree[cwd]["depth"] + 1
            tree[f"{cwd}{line_split[-1]}"]["type"] = "file"
            tree[f"{cwd}{line_split[-1]}"]["size"] = int(line_split[0])
    return tree


def bfs_fill_file_sizes(terminal_list: List[str]) -> DefaultDict:
    file_tree = create_file_tree(terminal_list)
    pqueue: PriorityQueue = PriorityQueue()
    seen_objs: Set = set()
    max_depth = max(obj["depth"] for obj in file_tree.values())
    for leaf in [(fp, obj["depth"]) for fp, obj in file_tree.items() if obj["type"] == "file"]:
        pqueue.put((max_depth - leaf[1], leaf[0]))
    while not pqueue.empty():
        cur_priority, cur_obj = pqueue.get()
        if file_tree[cur_obj]["parent"] is not None and cur_obj not in seen_objs:
            pqueue.put((cur_priority + 1, file_tree[cur_obj]["parent"]))
            file_tree[file_tree[cur_obj]["parent"]]["size"] += file_tree[cur_obj]["size"]
        seen_objs.add(cur_obj)
    assert set(file_tree.keys()).difference(seen_objs) == set()
    assert sum(obj["size"] for obj in file_tree.values() if obj["type"] == "file") == file_tree["/"]["size"]
    return file_tree


def part1(terminal_list: List[str]) -> int:
    file_tree = bfs_fill_file_sizes(terminal_list)
    return sum(obj["size"] for obj in file_tree.values() if obj["size"] <= 100000 and obj["type"] == "dir")




def part2(terminal_list: List[str]) -> int:
    file_tree = bfs_fill_file_sizes(terminal_list)
    print(file_tree["/"]["size"])
    space_target = 30000000 + file_tree["/"]["size"] - 70000000
    return min(obj["size"] for obj in file_tree.values() if obj["type"] == "dir" and obj["size"] >= space_target)



print("Part 1: ", part1(open("data.txt").read().split("\n")))
print("Part 2: ", part2(open("data.txt").read().split("\n")))