# 690. Employee Importance

## 核心思路

本质是一道**多叉树子树求和**问题。给定员工的层级关系（每个员工有 id、importance 值和直接下属列表），要求计算某个员工及其所有直接和间接下属的 importance 总和。关键在于先用哈希表建立 id -> Employee 的映射，然后从目标员工出发做 DFS 或 BFS 遍历整棵子树并累加 importance。

## 思维链

1. 输入是一个 Employee 数组，员工之间通过 subordinates 列表形成树形结构
2. 目标：给定一个 id，求该员工及其所有下属（递归）的 importance 总和
3. 首先需要快速通过 id 找到对应 Employee 对象 -> 用 unordered_map<int, Employee*>
4. 从目标 id 出发，遍历其 subordinates，对每个下属递归求和
5. BFS 同样可行：用队列逐层展开所有下属
6. 时间 O(N)，空间 O(N)，N 为员工总数

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| DFS + HashMap | O(N) | O(N) | 代码最简洁，递归遍历子树 |
| BFS + HashMap | O(N) | O(N) | 迭代方式，避免递归栈溢出 |

## 关键提示

1. **不要线性查找**：输入是数组，如果每次通过遍历数组来查找 id 对应的员工，复杂度会退化为 O(N^2)。用哈希表预处理可以将查找降为 O(1)。
2. **树形结构隐含**：虽然题目说"每个员工最多有一个直接领导"，但输入并非显式的树，而是一个扁平数组。需要自己建立映射关系。
3. **subordinates 存储的是 id 而非指针**：每次递归时需要通过 id 在映射表中查找对应的 Employee 对象。
4. **importance 可以为负值**：题目约束 importance 范围是 [-100, 100]，所以不能做提前剪枝。

## 解法详解

### 解法一：DFS + HashMap（推荐）

**思路**：
- 第一步：遍历 employees 数组，建立 id -> Employee* 的哈希映射
- 第二步：从目标 id 出发，递归地累加当前员工的 importance，然后对每个 subordinate 递归

**步骤**：
1. 建表：`for (auto e : employees) mp[e->id] = e;`
2. DFS 函数：返回当前员工的 importance + 所有下属的 importance 总和
3. 递归终止：当 subordinates 为空时自然终止

**复杂度**：
- 时间：O(N)，每个员工恰好被访问一次
- 空间：O(N)，哈希表 O(N) + 递归栈最坏 O(N)

### 解法二：BFS + HashMap

**思路**：
- 建立同样的哈希映射
- 用队列从目标 id 出发，逐层将所有下属加入队列
- 每次出队时累加该员工的 importance

**步骤**：
1. 建表同上
2. 初始化队列，放入目标 id
3. 循环：取出队首 id，累加 importance，将其所有 subordinates 入队
4. 队列为空时返回总和

**复杂度**：
- 时间：O(N)
- 空间：O(N)

## 易错点

1. **忘记建哈希表直接遍历**：每次递归都线性搜索 employees 数组找目标 id，导致 O(N^2) 超时或效率极差。
2. **混淆 subordinates 类型**：subordinates 是 int 列表（id），不是 Employee 指针列表，需要通过映射表转换。
3. **忽略根节点自身**：求和时必须包含目标员工自身的 importance，不要只算下属。
4. **BFS 中重复入队**：虽然题目保证树形结构（无环），但如果数据异常，没有 visited 检查可能导致死循环。正常情况下树结构不需要 visited。

## 面试追问

1. **如果员工数量非常大（百万级），如何优化？**
   - 哈希表仍然适用，O(N) 建表 + O(子树大小) 查询。如果需要多次查询不同 id，可以预处理每个节点的子树和。

2. **如果需要频繁更新某个员工的 importance 并查询子树和？**
   - 使用 DFS 序 + 树状数组（BIT）或线段树，支持 O(log N) 的单点更新和区间求和。

3. **如果图中有环（不是严格的树），怎么处理？**
   - 需要加 visited 集合防止重复访问，本质变成图的遍历问题。

4. **DFS vs BFS 在这题中哪个更优？**
   - 性能相当，DFS 代码更短。BFS 在极深的树中避免栈溢出更安全。

5. **能否用拓扑排序从叶子节点向上累加？**
   - 可以，但更复杂且不必要。拓扑排序适合全局处理，本题只需要单棵子树的结果。

## 相关题型

- [339. Nested List Weight Sum](../0339_nested_list_weight_sum/) - 类似的递归求和（嵌套列表）
- [364. Nested List Weight Sum II](../0364_nested_list_weight_sum_ii/) - 反向权重的递归求和
- [559. Maximum Depth of N-ary Tree](../0559_maximum_depth_of_n_ary_tree/) - 多叉树遍历
- [429. N-ary Tree Level Order Traversal](../0429_n_ary_tree_level_order_traversal/) - 多叉树 BFS
- [1376. Time Needed to Inform All Employees](../1376_time_needed_to_inform_all_employees/) - 类似的员工树结构问题
