# 742. Closest Leaf in a Binary Tree - 二叉树最近的叶节点

## 难度: Medium

## 标签: Tree, Depth-First Search, Breadth-First Search, Binary Tree

## 核心思路

二叉树中边是从父到子的单向关系，但"最近叶节点"可能在目标节点的祖先方向（向上走）。因此核心思路是**将二叉树转化为无向图**，然后从目标节点出发做 **BFS**，第一个遇到的叶节点就是距离最近的。具体步骤：DFS 遍历建立邻接表（双向边），在无向图上从目标节点 BFS 找最近叶节点。

## 思维链

1. **认清方向限制**: 二叉树只能从父到子遍历，但叶节点可能在目标的祖先分支上
2. **想到向上走**: 需要让节点能"向上"遍历到父节点 -> 构建无向图或记录 parent 指针
3. **选择 BFS**: 在无向图中找最近目标 -> BFS 天然按层扩展，第一个叶节点就是最近的
4. **叶节点判定**: 在原树中叶节点是没有子节点的节点（度为0的节点），在图中不能简单用度判断，需要单独标记
5. **唯一值保证**: 题目保证每个节点值唯一，可以用值作为图的节点标识

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 建图 + BFS（推荐） | O(n) | O(n) | 最直观，先建无向图再 BFS |
| Parent 指针 + BFS | O(n) | O(n) | 不建完整图，只记录父指针 |
| DFS + 注解距离 | O(n) | O(n) | 纯树上操作，不建图 |

## 关键提示

1. **无向图表示**: 用 `unordered_map<int, vector<int>>` 作邻接表，对每条父子关系添加双向边
2. **叶节点集合**: 用 `unordered_set<int>` 记录所有叶节点，BFS 时快速判断
3. **BFS 起点**: 从值为 k 的节点开始，不是从 root 开始
4. **visited 防重**: BFS 必须用 visited 集合避免无向图中的重复访问和死循环
5. **题目保证 k 存在**: 不需要处理 k 不存在的情况，题目约束保证树中存在值为 k 的节点

## 解法详解

### 解法一：建图 + BFS（推荐）

**步骤**:
1. **DFS 建图**: 遍历二叉树，对每个节点和其左右孩子建立双向边。同时标记所有叶节点
2. **BFS 搜索**: 从值为 k 的节点开始 BFS，逐层扩展
3. **首个叶节点**: BFS 遇到的第一个在叶节点集合中的节点即为答案

**为什么 BFS 保证最近**: BFS 按层扩展，每层距离递增 1。第一个触及的叶节点一定是距离最短的。

**时间复杂度**: O(n) - 建图 O(n) + BFS O(n)

**空间复杂度**: O(n) - 邻接表 + BFS 队列

### 解法二：Parent 指针 + BFS

**步骤**:
1. DFS 遍历树，记录每个节点的 parent 指针（`unordered_map<TreeNode*, TreeNode*>`）
2. 找到值为 k 的节点
3. 从该节点出发 BFS，扩展方向包括 left、right 和 parent
4. 第一个叶节点即为答案

**优势**: 不需要显式建图，直接利用树结构加 parent 指针

**时间复杂度**: O(n)

**空间复杂度**: O(n)

### 解法三：DFS 注解距离

**步骤**:
1. DFS 找到目标节点 k，记录从 k 到其子树中各叶节点的距离
2. 回溯时向上传播距离信息，同时检查兄弟子树中的叶节点
3. 维护全局最小距离和对应叶节点值

**特点**: 纯树上操作，不建图，但逻辑较复杂

**时间复杂度**: O(n)

**空间复杂度**: O(n) 递归栈

## 易错点

1. **忘记向上走**: 如果只在目标节点的子树中搜索，会漏掉通过祖先到达的更近叶节点。例如 [1,2,3,4,null,null,null,5,null,6] 中 k=2，最近叶节点是 3（通过父节点 1 到达），而非子树中的 6
2. **叶节点判定错误**: 在无向图中不能用"度为1"判断叶节点（根节点度也可能为1），必须在建图时独立标记原树中的叶节点
3. **BFS 未用 visited**: 无向图中不标记已访问节点会导致死循环
4. **混淆节点值和节点指针**: 题目保证值唯一，用值作标识是安全的；但如果值不唯一则必须用指针
5. **根节点是叶节点**: 如果树只有一个节点，根节点本身就是叶节点，k 必然等于 root，直接返回 root.val
6. **目标节点本身是叶节点**: k 对应的节点可能就是叶节点，此时距离为 0，直接返回 k

## 面试追问

1. **如果节点值不唯一怎么办?** 用 TreeNode* 指针代替值作为图的节点标识，需要先 DFS 找到目标指针
2. **能否不建图直接在树上操作?** 可以。用 DFS + 回溯标注法：DFS 时记录目标到当前节点的距离，检查兄弟子树。但代码复杂度较高
3. **如果要返回所有最近叶节点（可能有多个距离相同的）?** BFS 找到第一个叶节点后记录其层级，继续处理同一层的所有节点，收集所有叶节点
4. **如何求目标节点到所有叶节点的距离排序?** 完整执行 BFS，记录遇到每个叶节点时的层级（距离），最后排序返回
5. **时间复杂度能否做到优于 O(n)?** 不能。最坏情况下叶节点可能在树的任何位置，必须遍历所有节点

## 相关题型

- [863. All Nodes Distance K in Binary Tree](https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree/) - 同样的"树转图 + BFS"模式
- [236. Lowest Common Ancestor of a Binary Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/) - 树中向上遍历的另一种场景
- [199. Binary Tree Right Side View](https://leetcode.com/problems/binary-tree-right-side-view/) - BFS 层序遍历
- [515. Find Largest Value in Each Tree Row](https://leetcode.com/problems/find-largest-value-in-each-tree-row/) - BFS 层序遍历
- [993. Cousins in Binary Tree](https://leetcode.com/problems/cousins-in-binary-tree/) - 需要 parent 信息的树问题
- [1161. Maximum Level Sum of a Binary Tree](https://leetcode.com/problems/maximum-level-sum-of-a-binary-tree/) - BFS 层序处理
