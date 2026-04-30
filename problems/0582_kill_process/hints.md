# 582. 杀掉进程 (Kill Process)

**难度**: Medium | **标签**: Tree, DFS, BFS, Array, Hash Table | **链接**: [LeetCode 582](https://leetcode.com/problems/kill-process/)

## 核心思路

本质是一个**树的遍历**问题：给定父子关系数组，构建树结构，然后从指定节点开始做 BFS/DFS，收集该节点及其所有后代节点。

关键步骤：
1. 用 `unordered_map<int, vector<int>>` 将 ppid->pid 的关系转换为**邻接表**（父节点->子节点列表）
2. 从 kill 节点开始 BFS/DFS，收集所有可达节点

## 思维链

1. **理解数据结构**: pid 和 ppid 是平行数组，pid[i] 的父亲是 ppid[i]
2. **构建树**: 遍历一次数组，建立 parent->children 的映射
3. **选择遍历方式**: BFS（队列）或 DFS（栈/递归）都可以
4. **收集结果**: 从 kill 节点出发，所有访问到的节点都是要被杀掉的
5. **边界情况**: kill 是根节点时，返回所有进程

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| BFS | 建邻接表 + 队列层序遍历 | O(n) | O(n) | ⭐⭐⭐⭐⭐ |
| DFS 递归 | 建邻接表 + 递归遍历子树 | O(n) | O(n) | ⭐⭐⭐⭐ |
| DFS 迭代 | 建邻接表 + 栈模拟 | O(n) | O(n) | ⭐⭐⭐⭐ |
| 暴力 | 不建邻接表，反复扫描数组 | O(n^2) | O(n) | ⭐ |

## 关键提示

1. **邻接表方向**: 建立的是 `parent -> [children]` 映射，不是 `child -> parent`
2. **根节点**: ppid[i]=0 表示根节点，无需特殊处理，BFS/DFS 自然不会往上走
3. **pid 值不一定连续**: 不能用数组下标做映射，必须用 HashMap
4. **一次遍历建图**: O(n) 建邻接表，O(n) 遍历，总体 O(n)

## 解法详解

### 解法一：BFS（推荐）

```cpp
class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        unordered_map<int, vector<int>> children;
        for (int i = 0; i < pid.size(); ++i) {
            children[ppid[i]].push_back(pid[i]);
        }

        vector<int> result;
        queue<int> q;
        q.push(kill);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            result.push_back(curr);
            for (int child : children[curr]) {
                q.push(child);
            }
        }
        return result;
    }
};
```

**执行流程**（以 pid=[1,3,10,5], ppid=[3,0,5,3], kill=5 为例）：
- 建图: {0:[3], 3:[1,5], 5:[10]}
- BFS: 队列=[5] -> 取出5，加入结果，孩子[10]入队
- 队列=[10] -> 取出10，加入结果，无孩子
- 结果: [5, 10]

### 解法二：DFS 递归

```cpp
class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        unordered_map<int, vector<int>> children;
        for (int i = 0; i < pid.size(); ++i) {
            children[ppid[i]].push_back(pid[i]);
        }

        vector<int> result;
        dfs(children, kill, result);
        return result;
    }

private:
    void dfs(unordered_map<int, vector<int>>& children,
             int node, vector<int>& result) {
        result.push_back(node);
        for (int child : children[node]) {
            dfs(children, child, result);
        }
    }
};
```

### 解法三：DFS 迭代（栈）

```cpp
class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        unordered_map<int, vector<int>> children;
        for (int i = 0; i < pid.size(); ++i) {
            children[ppid[i]].push_back(pid[i]);
        }

        vector<int> result;
        stack<int> stk;
        stk.push(kill);

        while (!stk.empty()) {
            int curr = stk.top();
            stk.pop();
            result.push_back(curr);
            for (int child : children[curr]) {
                stk.push(child);
            }
        }
        return result;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `map<int, vector<int>>` | `unordered_map<int, vector<int>>` | map是O(log n)查找，unordered_map是O(1)，性能差异大 |
| `children[pid[i]].push_back(ppid[i])` | `children[ppid[i]].push_back(pid[i])` | 方向反了！是 parent->child，不是 child->parent |
| 忘记把 kill 节点本身加入结果 | `result.push_back(kill)` 或 BFS/DFS 起点就是 kill | kill 节点自身也要被杀掉 |
| 用 `vector<vector<int>>` 做邻接表 | 用 `unordered_map` | pid值可能很大(5*10^4)且不连续，数组浪费空间 |
| DFS递归不传引用 | `void dfs(..., vector<int>& result)` | 不传引用会导致结果无法收集或大量拷贝 |

## 面试追问

**Q1: BFS 和 DFS 在这道题中有什么区别？哪个更适合？**

A1: 功能上完全等价，都能正确收集所有后代节点。BFS 的优势在于结果按层序排列（先父后子），更符合"杀进程"的直觉语义。DFS 递归代码更简洁，但树很深时可能栈溢出。对于 n <= 5*10^4 的约束，两者都没问题。面试中推荐 BFS，代码清晰且无栈溢出风险。

**Q2: 如果要求返回被杀进程的层级关系（即每个进程距离 kill 的深度），如何修改？**

A2: BFS 天然支持层级信息。在 BFS 循环中加入层级计数：
```cpp
int depth = 0;
while (!q.empty()) {
    int sz = q.size();
    while (sz--) {
        int curr = q.front(); q.pop();
        result.push_back({curr, depth});
        for (int child : children[curr]) q.push(child);
    }
    depth++;
}
```

**Q3: 如果进程之间可能存在循环引用（不是严格的树），如何处理？**

A3: 需要加 visited 集合防止死循环：
```cpp
unordered_set<int> visited;
// BFS 中: if (visited.count(child) == 0) { visited.insert(child); q.push(child); }
```
但题目保证是树结构（每个节点只有一个父节点），所以不需要 visited。

## 相关题型

- [207. Course Schedule](https://leetcode.com/problems/course-schedule/) - 图的 BFS/拓扑排序
- [210. Course Schedule II](https://leetcode.com/problems/course-schedule-ii/) - 拓扑排序输出
- [559. Maximum Depth of N-ary Tree](https://leetcode.com/problems/maximum-depth-of-n-ary-tree/) - N叉树遍历
- [690. Employee Importance](https://leetcode.com/problems/employee-importance/) - 类似的树形累计
- [1443. Minimum Time to Collect All Apples in a Tree](https://leetcode.com/problems/minimum-time-to-collect-all-apples-in-a-tree/) - 树遍历
