/*
 * LeetCode 133: Clone Graph (克隆图)
 *
 * 【题目本质】
 * 深拷贝一个无向连通图：遍历图的同时创建每个节点的副本，
 * 并正确重建克隆节点之间的邻居关系。
 * 核心难点：图有环，必须用哈希表避免重复克隆和无限循环。
 *
 * 【解法总览】
 * 解法1: DFS 递归 + 哈希表 — O(N+E) / O(N) — 面试首选，代码最简洁
 * 解法2: BFS 迭代 + 哈希表 — O(N+E) / O(N) — 追问时展示，无栈溢出风险
 *
 * 【图的结构示意 (示例1)】
 *
 *   原始图:                  克隆图:
 *     1 ------- 2             1' ------ 2'
 *     |         |             |         |
 *     |         |             |         |
 *     4 ------- 3             4' ------ 3'
 *
 *   每个克隆节点是全新 new 出来的，邻居指针指向克隆节点。
 *   哈希表: {1→1', 2→2', 3→3', 4→4'}
 */

// Definition for a Node (题目给定)
// class Node {
// public:
//     int val;
//     vector<Node*> neighbors;
//     Node() { val = 0; neighbors = vector<Node*>(); }
//     Node(int _val) { val = _val; neighbors = vector<Node*>(); }
//     Node(int _val, vector<Node*> _neighbors) { val = _val; neighbors = _neighbors; }
// };

// ============================================================
// 解法1: DFS 递归 + 哈希表
// 时间: O(N + E)  空间: O(N) — N个节点存入哈希表 + 递归栈深度
//
// 【思路】
// 把 cloneGraph(node) 定义为"返回 node 的克隆副本"。
// 三要素:
//   1. 终止条件: node 为空 或 node 已经克隆过(在 map 中)
//   2. 单层逻辑: 创建克隆节点 → 存入 map → 递归克隆每个邻居
//   3. 返回值: 克隆节点的指针
//
// 为什么先存 map 再递归？
//   因为图有环。如果先递归邻居再存 map，邻居可能又指回自己，
//   此时 map 中还没有自己 → 再次创建 → 无限递归。
//
// 【DFS 递归过程】
//  cloneGraph(1):
//    创建 1', map[1]=1'
//    ├─ cloneGraph(2):
//    |    创建 2', map[2]=2'
//    |    ├─ cloneGraph(1): 已在 map → 返回 1'  ← 环被打断
//    |    └─ cloneGraph(3):
//    |         创建 3', map[3]=3'
//    |         ├─ cloneGraph(2): 已在 map → 返回 2'
//    |         └─ cloneGraph(4):
//    |              创建 4', map[4]=4'
//    |              ├─ cloneGraph(1): 已在 map → 返回 1'
//    |              └─ cloneGraph(3): 已在 map → 返回 3'
//    |              4'.neighbors = [1', 3']
//    |         3'.neighbors = [2', 4']
//    |    2'.neighbors = [1', 3']
//    └─ cloneGraph(4): 已在 map → 返回 4'
//    1'.neighbors = [2', 4']
// ============================================================
class Solution1 {
public:
    unordered_map<Node*, Node*> visited;

    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        // 已经克隆过 → 直接返回克隆节点（打破环的关键）
        if (visited.count(node)) return visited[node];

        // 创建克隆节点，立即存入 map（必须在递归之前！）
        Node* clone = new Node(node->val);
        visited[node] = clone;

        // 递归克隆每个邻居，cloneGraph 返回的就是克隆节点
        for (Node* neighbor : node->neighbors) {
            clone->neighbors.push_back(cloneGraph(neighbor));
        }

        return clone;
    }
};


// ============================================================
// 解法2: BFS 迭代 + 哈希表
// 时间: O(N + E)  空间: O(N) — 队列 + 哈希表
//
// 【思路】
// DFS 用递归栈遍历，BFS 用显式队列遍历。核心映射逻辑完全相同。
//
// BFS 的节奏：
//   1. 克隆起始节点，入队
//   2. 每次出队一个原始节点 curr
//   3. 遍历 curr 的所有邻居：
//      - 如果邻居没克隆过 → 创建克隆并入队
//      - 把邻居的克隆加入 curr 克隆的邻居列表
//
// 为什么在"发现邻居时"创建克隆，而不是"出队时"？
//   因为同一个节点可能被多个节点引用为邻居，如果出队才创建，
//   在入队时无法判断是否已经入过队，导致重复入队和重复创建。
//
// 【BFS 过程】
//  初始: queue=[1], map={1:1'}
//
//  出队1, 邻居[2,4]:
//    2不在map → 创建2', 入队, map={1:1', 2:2'}
//    4不在map → 创建4', 入队, map={..., 4:4'}
//    1'.neighbors = [2', 4']
//
//  出队2, 邻居[1,3]:
//    1在map → 用1'
//    3不在map → 创建3', 入队, map={..., 3:3'}
//    2'.neighbors = [1', 3']
//
//  出队4, 邻居[1,3]:
//    1在map → 用1',  3在map → 用3'
//    4'.neighbors = [1', 3']
//
//  出队3, 邻居[2,4]:
//    2在map → 用2',  4在map → 用4'
//    3'.neighbors = [2', 4']
//
//  队列空 → 返回 map[node] = 1'
// ============================================================
class Solution2 {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        unordered_map<Node*, Node*> visited;
        queue<Node*> q;

        // 克隆起始节点并入队
        visited[node] = new Node(node->val);
        q.push(node);

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            for (Node* neighbor : curr->neighbors) {
                if (!visited.count(neighbor)) {
                    // 邻居还没被克隆 → 创建克隆并入队
                    visited[neighbor] = new Node(neighbor->val);
                    q.push(neighbor);
                }
                // 将邻居的克隆加入当前克隆节点的邻居列表
                // 注意：visited[curr] 是 curr 的克隆，visited[neighbor] 是邻居的克隆
                visited[curr]->neighbors.push_back(visited[neighbor]);
            }
        }

        return visited[node];
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | DFS 递归          | BFS 迭代          |
// |-------------|-------------------|-------------------|
// | 时间复杂度    | O(N+E)           | O(N+E)           |
// | 空间复杂度    | O(N) + 递归栈     | O(N) + 队列      |
// | 代码量       | ~10行，更简洁      | ~15行，稍长       |
// | 栈溢出风险    | 图退化为链时有风险  | 无                |
// | 遍历顺序     | 深度优先           | 广度优先          |
// | 面试推荐     | 首选（写得快）      | 追问时展示        |
//
// 两种方法核心思想完全一样：哈希表记录 原始→克隆 映射，
// 同时充当 visited 集合。选哪个取决于个人习惯和面试追问。
//
// ============================================================
// 【易错点】
//
// 1. ✗ visited[node] = clone 放在递归邻居之后
//    → 环会导致无限递归，因为邻居指回 node 时 map 里还没有 node
//    ✓ 必须在递归/入队之前就存入 map
//
// 2. ✗ clone->neighbors.push_back(neighbor)
//    → neighbor 是原始节点指针！克隆图的邻居列表应该放克隆节点
//    ✓ clone->neighbors.push_back(cloneGraph(neighbor))  // DFS
//    ✓ visited[curr]->neighbors.push_back(visited[neighbor])  // BFS
//
// 3. ✗ 忘记判空 if (!node) return nullptr;
//    → 输入为空图时，对 nullptr 解引用导致段错误
//
// 4. ✗ BFS 中出队时才创建克隆节点
//    → 同一节点可能被多个邻居引用，被入队多次，出队时重复创建
//    ✓ 在发现新邻居时（入队时）就创建克隆并存入 map
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么需要哈希表？只用 visited 集合(set)行吗？
// A1: 不行。set 只能判断是否访问过，但我们还需要通过原始节点
//     找到对应的克隆节点指针，用来填充邻居列表。所以必须是 map。
//
// Q2: 如果图非常大（百万节点），DFS 递归会有什么问题？怎么解决？
// A2: 递归深度可能导致栈溢出（特别是图退化为链时，深度=N）。
//     解决方案：用 BFS 迭代版，或者用显式栈模拟 DFS。
//
// Q3: 这个克隆思路能扩展到什么场景？
// A3: 任何含循环引用的数据结构的深拷贝。例如：
//     - 138题：带 random 指针的链表
//     - 有向图的克隆
//     - 通用对象图的序列化/反序列化
//     核心都是"哈希表记录已创建副本，遇到已创建的直接复用"。
//
// Q4: 如果节点的 val 不唯一，哈希表的 key 应该用什么？
// A4: 用原始节点的指针(地址)作为 key，而不是 val。
//     本题的哈希表 key 就是 Node*，与 val 是否唯一无关。
// ============================================================
