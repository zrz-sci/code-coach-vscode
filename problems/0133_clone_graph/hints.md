# 133. 克隆图

## 核心思路

本质上这道题在问：**如何遍历一个图，同时为每个节点创建一个副本，并正确地重建副本之间的邻居关系？** 核心难点不在于遍历本身，而在于图有环（无向图的双向边就是环），遍历时需要记录"哪些节点已经克隆过了"，避免无限循环，同时保证同一个原始节点只对应一个克隆节点。

## 思维链

1. **读完题第一反应**：需要对每个节点 new 一个新节点，然后把邻居关系也复制过去。最直觉的做法就是遍历图（BFS 或 DFS），遇到一个节点就克隆它。

2. **马上发现问题**：图是无向连通图，节点 1 的邻居有节点 2，节点 2 的邻居又有节点 1。如果不做任何记录，DFS/BFS 会陷入无限循环！

3. **怎么解决环的问题？** 需要一个"已访问"记录。但这里不只是标记"是否访问过"，还需要**找到已经克隆好的那个副本节点**——因为填充邻居列表时，需要的是克隆节点的指针，不是原始节点的指针。

4. **自然想到哈希表**：`map<原始节点, 克隆节点>`。遍历时，如果当前节点已经在 map 中，直接返回对应的克隆节点；否则创建新节点并存入 map，然后递归/迭代地克隆邻居。

5. **DFS vs BFS**：两种遍历方式都可以，核心逻辑完全相同——都是"遇到新节点就克隆并存 map，遇到已克隆节点就直接取 map 中的副本"。

6. **边界情况**：输入为空（nullptr）直接返回 nullptr。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 递归 | 递归遍历图，哈希表记录已克隆节点 | O(N+E) | O(N) | ⭐ 必须写出 |
| BFS 迭代 | 队列层序遍历图，哈希表记录已克隆节点 | O(N+E) | O(N) | ⭐ 必须写出 |

> N = 节点数，E = 边数。两种解法本质相同，只是遍历顺序不同。面试中两种都要能写。

## 关键提示

- **提示1**：图的遍历（DFS/BFS）你已经很熟了，克隆图就是在遍历的同时"边走边建"。关键是：怎么避免重复创建同一个节点的克隆？

- **提示2**：哈希表 `map<Node*, Node*>` 同时承担两个角色——①判断是否已访问（相当于 visited 集合），②存储原始节点到克隆节点的映射。

- **提示3**：DFS 递归版本非常简洁——函数定义就是"给我一个原始节点，我返回它的克隆"。基线条件是 nullptr 和"已经克隆过"。

- **提示4**：注意，克隆节点的邻居列表中放的必须是**克隆节点的指针**，不是原始节点的指针！这是最容易犯的概念错误。

```
// 图的结构示意 (示例1):
//
//     1 ------- 2
//     |         |
//     |         |
//     4 ------- 3
//
// 克隆后:
//     1' ------ 2'       (每个节点都是新 new 出来的)
//     |         |        (邻居指针指向克隆节点，不是原始节点)
//     |         |
//     4' ------ 3'
```

## 解法详解

### 解法1: DFS 递归 + 哈希表 — O(N+E) / O(N) ⭐ 面试首选

**思考过程**：

最自然的思路——递归地克隆。函数 `cloneGraph(node)` 的语义是："给我原始图中的某个节点，返回它在克隆图中的对应节点。"

- 如果 `node == nullptr`，返回 nullptr
- 如果 `node` 已经在哈希表中（说明之前已克隆过），直接返回哈希表中的克隆节点
- 否则：创建克隆节点，立即存入哈希表（**先存后递归，防止环导致无限递归**），然后对每个邻居递归调用，将结果加入克隆节点的邻居列表

```
// DFS 递归过程 (示例1):
//
//  cloneGraph(1):
//    创建 1', map[1]=1'
//    遍历邻居 [2, 4]:
//      |
//      ├─ cloneGraph(2):
//      |    创建 2', map[2]=2'
//      |    遍历邻居 [1, 3]:
//      |      ├─ cloneGraph(1): 1 已在 map → 返回 1'
//      |      └─ cloneGraph(3):
//      |           创建 3', map[3]=3'
//      |           遍历邻居 [2, 4]:
//      |             ├─ cloneGraph(2): 2 已在 map → 返回 2'
//      |             └─ cloneGraph(4):
//      |                  创建 4', map[4]=4'
//      |                  遍历邻居 [1, 3]:
//      |                    ├─ cloneGraph(1): 已在 map → 返回 1'
//      |                    └─ cloneGraph(3): 已在 map → 返回 3'
//      |                  4'.neighbors = [1', 3']
//      |           3'.neighbors = [2', 4']
//      |    2'.neighbors = [1', 3']
//      |
//      └─ cloneGraph(4): 4 已在 map → 返回 4'
//
//    1'.neighbors = [2', 4']
//    返回 1'
```

```cpp
/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() { val = 0; neighbors = vector<Node*>(); }
    Node(int _val) { val = _val; neighbors = vector<Node*>(); }
    Node(int _val, vector<Node*> _neighbors) { val = _val; neighbors = _neighbors; }
};
*/

class Solution {
public:
    unordered_map<Node*, Node*> visited; // 原始节点 → 克隆节点
    
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        
        // 如果已经克隆过，直接返回克隆节点（打破环的关键！）
        if (visited.count(node)) return visited[node];
        
        // 创建克隆节点，先存入 map（必须在递归邻居之前存！）
        Node* clone = new Node(node->val);
        visited[node] = clone;
        
        // 递归克隆每个邻居，加入克隆节点的邻居列表
        for (Node* neighbor : node->neighbors) {
            clone->neighbors.push_back(cloneGraph(neighbor));
        }
        
        return clone;
    }
};
```

**关键点**：
- `visited[node] = clone` 必须在递归邻居**之前**执行。如果先递归再存 map，遇到环时会无限递归。
- 函数返回值就是克隆节点的指针，所以 `cloneGraph(neighbor)` 返回的就是邻居的克隆，可以直接 push_back。

---

### 解法2: BFS 迭代 + 哈希表 — O(N+E) / O(N) ⭐ 面试首选

**从解法1转化**：DFS 用递归栈，BFS 用显式队列。核心逻辑完全一样——哈希表记录映射，只是遍历顺序从深度优先变成了广度优先。

BFS 的特点是：先把起始节点克隆好放入队列，然后逐个出队，为每个出队节点处理其所有邻居。

```
// BFS 过程 (示例1):
//
// 初始: queue = [1], map = {1: 1'}
//
// 出队 1, 处理邻居 [2, 4]:
//   2 不在 map → 创建 2', map[2]=2', 入队
//   4 不在 map → 创建 4', map[4]=4', 入队
//   1'.neighbors = [2', 4']
//   queue = [2, 4]
//
// 出队 2, 处理邻居 [1, 3]:
//   1 在 map → 直接用 1'
//   3 不在 map → 创建 3', map[3]=3', 入队
//   2'.neighbors = [1', 3']
//   queue = [4, 3]
//
// 出队 4, 处理邻居 [1, 3]:
//   1 在 map → 直接用 1'
//   3 在 map → 直接用 3'
//   4'.neighbors = [1', 3']
//   queue = [3]
//
// 出队 3, 处理邻居 [2, 4]:
//   2 在 map → 直接用 2'
//   4 在 map → 直接用 4'
//   3'.neighbors = [2', 4']
//   queue = [] → 结束
```

```cpp
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        
        unordered_map<Node*, Node*> visited;
        queue<Node*> q;
        
        // 克隆起始节点
        visited[node] = new Node(node->val);
        q.push(node);
        
        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();
            
            // 遍历当前原始节点的所有邻居
            for (Node* neighbor : curr->neighbors) {
                if (!visited.count(neighbor)) {
                    // 邻居还没被克隆过 → 创建克隆并入队
                    visited[neighbor] = new Node(neighbor->val);
                    q.push(neighbor);
                }
                // 不管是新建的还是已有的，都加入当前克隆节点的邻居列表
                visited[curr]->neighbors.push_back(visited[neighbor]);
            }
        }
        
        return visited[node];
    }
};
```

**关键点**：
- BFS 中，克隆节点是在**发现邻居时**创建的（而不是出队时），这保证了同一个节点不会被重复创建。
- `visited[curr]->neighbors.push_back(visited[neighbor])` —— 注意是往**克隆节点的**邻居列表中加**克隆邻居**的指针。

## 解法对比

| | DFS 递归 | BFS 迭代 |
|---|---|---|
| 遍历方式 | 深度优先（递归栈） | 广度优先（显式队列） |
| 代码量 | 更简洁，约10行 | 稍长，约15行 |
| 栈溢出风险 | 图很深时递归可能溢出 | 无此风险 |
| 哈希表角色 | 同时充当 visited + 映射 | 同时充当 visited + 映射 |
| 面试推荐 | 首选（代码短，容易写对） | 追问时展示（证明两种都会） |

两种解法时间空间复杂度完全相同：O(N+E) / O(N)。面试中 DFS 递归版更容易快速写出，BFS 版可以作为 follow-up 展示。

## 易错点

1. **✗ 克隆后再存 map，导致无限递归**
   ```cpp
   // 错误：先递归邻居，再存 map
   Node* clone = new Node(node->val);
   for (Node* nb : node->neighbors)
       clone->neighbors.push_back(cloneGraph(nb));  // 邻居指回 node 时，map 里还没有 node！
   visited[node] = clone;  // 太晚了！
   ```
   **✓ 正确做法**：创建克隆节点后**立即**存入 map，然后再递归邻居。

2. **✗ 把原始节点指针放进克隆节点的邻居列表**
   ```cpp
   // 错误：邻居列表中放的是原始节点
   clone->neighbors.push_back(neighbor);  // neighbor 是原始图的节点！
   ```
   **✓ 正确做法**：
   ```cpp
   clone->neighbors.push_back(cloneGraph(neighbor));  // 递归返回的是克隆节点
   // 或 BFS 中:
   visited[curr]->neighbors.push_back(visited[neighbor]);
   ```

3. **✗ 忘记处理空图**
   ```cpp
   // 如果不判空，对 nullptr 调用 node->val 会段错误
   ```
   **✓ 函数开头加 `if (!node) return nullptr;`**

4. **✗ BFS 中出队时才创建克隆节点，导致重复创建**
   ```cpp
   // 错误：两个不同节点可能都有同一个邻居，入队两次
   while (!q.empty()) {
       Node* curr = q.front(); q.pop();
       visited[curr] = new Node(curr->val);  // 可能覆盖之前创建的！
   }
   ```
   **✓ 正确做法**：在**入队时**（发现新邻居时）就创建克隆节点。

## 面试追问

**Q1（基础理解）**：为什么需要哈希表？不用行吗？
> 图有环（无向边本身就构成长度为2的环）。不用哈希表记录已访问节点，DFS/BFS 会陷入无限循环。同时哈希表保证了同一个原始节点只创建一个克隆副本。

**Q2（DFS vs BFS）**：两种遍历方式有什么区别？什么时候选哪个？
> 本题两种都 O(N+E)。DFS 递归代码更短但有栈溢出风险（图非常大且退化为链状时）；BFS 用显式队列没有栈溢出问题。面试中推荐先写 DFS 递归（快），被追问再写 BFS。

**Q3（变体追问）**：如果图可能不连通怎么办？
> 题目保证了连通图，所以从任意节点出发能遍历所有节点。如果不连通，需要一个外部循环，对每个未访问的连通分量分别进行克隆。但要注意题目只给了一个节点的引用，如果图不连通且没有额外信息，是无法克隆到其他连通分量的。

**Q4（深入追问）**：这道题的思路能推广到哪些场景？
> 本质是"深拷贝一个含有环引用的数据结构"。同样的哈希表记录映射的思路适用于：深拷贝带 random 指针的链表（138题）、序列化/反序列化图、任何存在循环引用的对象图的深拷贝。

## 相关题型

- **138. 随机链表的复制** — 复用"哈希表记录 原始→克隆 映射"的核心技巧。区别：链表是线性结构+random指针，图是任意连接。138 还有经典的"三步法"原地克隆优化。
- **207/210. 课程表 I/II** — 同样是图遍历（BFS/DFS），但目标不同：207是检测环，210是拓扑排序。复用图的 BFS/DFS 遍历模板。
- **261. 以图判树** — 同样使用 BFS/DFS 遍历连通图，复用 visited 集合避免重复访问的技巧。
- **323. 无向图中连通分量的数目** — 复用图的遍历框架，每次遍历标记一个连通分量。