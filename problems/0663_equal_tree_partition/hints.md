# 663. Equal Tree Partition

## 核心思路

先用 DFS 计算整棵树的总和 totalSum。如果 totalSum 是奇数则不可能等分，直接返回 false。如果 totalSum 是偶数，再次 DFS 检查是否存在一棵子树的和等于 totalSum / 2。移除该子树与父节点之间的边，就能把树分成两个和相等的部分。

## 思维链

1. 移除一条边会把树分成两部分，设一部分的和为 S，另一部分的和为 totalSum - S
2. 要求 S = totalSum - S，即 S = totalSum / 2
3. 所以问题转化为：是否存在一棵子树（不是整棵树本身），其节点值之和 = totalSum / 2
4. 第一步：DFS 计算 totalSum
5. 第二步：DFS 计算每棵子树的和，检查是否有子树和等于 totalSum / 2
6. 注意：不能选整棵树本身（即根节点的子树和 = totalSum 不算），因为移除整棵树后另一部分为空
7. 特殊情况：totalSum = 0 时，需要至少有一棵非根子树的和为 0

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 两遍 DFS | O(n) | O(n) | 逻辑清晰，易于实现 |
| 一遍 DFS + 集合 | O(n) | O(n) | 收集所有子树和到 multiset |
| 一遍 DFS + 数组 | O(n) | O(n) | 存所有子树和到数组 |

## 关键提示

1. totalSum 必须是偶数（否则无法等分），但注意 totalSum=0 是偶数，此时需要找一棵子树和为 0
2. 不能选择整棵树作为"子树"，必须是 root 的某个后代节点为根的子树
3. 用 multiset 或 vector 收集所有子树和时，需要排除根节点本身的和（即 totalSum）
4. 节点值可以为负数！所以不能做"子树和 > totalSum"的剪枝
5. totalSum = 0 的陷阱：只有根节点时和为 0，但无法分割，需要检查是否真有非根子树和为 0

## 解法详解

### 解法一：两遍 DFS（推荐）

**第一遍 DFS**：计算整棵树的 totalSum。

**第二遍 DFS**：计算每棵子树的和。如果某棵子树（不包含根）的和等于 totalSum / 2，则返回 true。

**实现关键**：
- 第二遍 DFS 返回子树和
- 在 DFS 过程中检查子树和是否等于 target = totalSum / 2
- 注意对根节点的子树和不做检查（因为它等于 totalSum，移除后另一边为空）
- 实现方式：DFS 检查左右子树（非根），如果任一子树和 == target，设置标记为 true

### 解法二：一遍 DFS + multiset

**思路**：一遍 DFS 收集所有子树的和到一个 multiset 中（包括根）。完成后，从 multiset 中移除根节点的和（totalSum），然后检查 multiset 中是否存在 totalSum / 2。

**为什么用 multiset 而不是 set？** 因为可能有多棵子树的和相同。当 totalSum = 0 时，需要有至少两棵子树和为 0（一棵是根，另一棵是非根）。

**步骤**：
1. DFS 后序遍历，收集所有子树和到 multiset
2. 检查 totalSum 是否为偶数
3. 从 multiset 中 erase 一个 totalSum（移除根的贡献）
4. 查找 multiset 中是否有 totalSum / 2

### 解法三：一遍 DFS + 数组

**思路**：和解法二类似，但用 vector 存所有子树和，最后遍历检查（跳过最后一个元素即根的和）。

## 易错点

1. **totalSum = 0 的陷阱**：根节点的子树和也是 0，必须确保找到的是非根子树。如果只有一个节点，应返回 false
2. **忘记 totalSum 奇数直接返回 false**：奇数不可能分成两个相等的整数
3. **把根节点也算作有效分割**：根的子树和 = totalSum，另一边为空树（和为 0），不算有效分割（除非 totalSum=0 且确实有非根子树和为 0）
4. **使用 set 而非 multiset**：当 totalSum=0 时，根的和和某个非根子树的和都是 0，set 会合并导致误判
5. **节点值含负数时的误判**：负数节点值会使子树和可能等于各种值，不能假设子树和单调

## 面试追问

1. **能否扩展到分成 k 个等和部分？** 变成 k-way partition 问题，需要找 k-1 条边使得分出的 k 棵子树和均为 totalSum/k
2. **如果不是二叉树而是多叉树？** DFS 逻辑类似，只需遍历所有子节点
3. **如何返回具体的分割边？** 在 DFS 中记录和 == target 的子树的根节点及其父节点
4. **时间空间能否优化？** 时间已是 O(n)；空间可通过 Morris 遍历减到 O(1)，但实现复杂
5. **如果要求两部分的和之差最小（而非相等）？** 转化为树上背包/最接近目标的子集和问题

## 相关题型

- [124. Binary Tree Maximum Path Sum](../0124_binary_tree_maximum_path_sum/) - 子树求和的经典问题
- [543. Diameter of Binary Tree](../0543_diameter_of_binary_tree/) - DFS 求子树属性
- [508. Most Frequent Subtree Sum](../0508_most_frequent_subtree_sum/) - 收集子树和
- [100. Same Tree](../0100_same_tree/) - 树的 DFS 基础
- [572. Subtree of Another Tree](../0572_subtree_of_another_tree/) - 子树判定
