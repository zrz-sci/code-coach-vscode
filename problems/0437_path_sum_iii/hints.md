# 437. 路径总和 III

## 核心思路

本质上是在一棵二叉树中，找所有**从上到下的连续路径**，使得路径上节点值之和等于 `targetSum`。这和"在数组中找和为 target 的连续子数组"是同构问题——前缀和 + 哈希表的经典套路可以直接搬到树上。

## 思维链

1. **读完题第一反应**：路径可以从任意节点开始、任意节点结束（只要向下），那最暴力的做法就是——对每个节点，都尝试以它为起点向下走，累加路径和看是否等于 target。

2. **暴力解的瓶颈**：对每个节点都要 DFS 向下遍历一次，等于"遍历树"套"遍历子树"，O(n²) 最坏情况（退化为链表时）。

3. **类比数组问题**：如果把根到当前节点的路径看成一个"数组"，那"路径和 = target"就等价于"找一段连续子数组和 = target"。数组上这个问题怎么做？**前缀和 + 哈希表**！`prefixSum[j] - prefixSum[i] == target` → 查找 `prefixSum[j] - target` 是否在之前出现过。

4. **搬到树上**：DFS 过程中维护从根到当前节点的前缀和 `currSum`，用哈希表记录"从根到路径上各节点的前缀和出现次数"。到达当前节点时，查找 `currSum - targetSum` 在哈希表中出现了几次，就有几条满足条件的路径。

5. **关键细节——回溯**：树不同于数组，DFS 离开当前节点回到父节点时，必须把当前前缀和从哈希表中移除，因为这个前缀和只对当前路径有效。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力双重 DFS | 对每个节点，以它为起点 DFS 求和 | O(n²) | O(n) | 能说出即可 |
| 前缀和 + 哈希表 | 类比数组前缀和，DFS 中用 map 查配对 | O(n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **暴力思路提示**：你需要两层递归——外层选"起点"，内层从该起点向下走。内层递归不要提前终止（节点值可以为负，后面的路径可能"拉回来"）。

2. **前缀和提示**：如果 root→A 的前缀和是 `currSum`，root→B 的前缀和是 `currSum - target`，且 B 是 A 的祖先，那 B→A 这段路径和就是 `target`。

3. **哈希表初始化提示**：`map[0] = 1`，代表"空前缀"，即从根节点开始的路径恰好和为 target 的情况。

4. **回溯提示**：离开当前子树时必须把 `currSum` 的计数减回去，否则左子树的前缀和会"污染"右子树的查询。

5. **ASCII 示意图**：

```
//        10               前缀和（从根到当前节点）:
//       /  \              10: prefixSum = 10
//      5   -3             5:  prefixSum = 15
//     / \    \            -3: prefixSum = 7
//    3   2    11          3:  prefixSum = 18
//   / \   \               2:  prefixSum = 17
//  3  -2   1              11: prefixSum = 18
//
// 以节点2(prefixSum=17)为例：
// 查找 17 - 8 = 9 → map中没有9 → 不贡献路径
// 以节点1(prefixSum=18)为例：
// 查找 18 - 8 = 10 → map中有10(来自根节点) → 贡献1条路径(5→2→1)
```

## 解法详解

### 解法1: 暴力双重 DFS — O(n²) / O(n)

**思考过程**：最直觉的想法——路径可以从任何节点开始，那就对树上每个节点都试一遍。以某个节点为起点，向下 DFS，累加路径和，等于 target 就计数 +1。

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int pathSum(TreeNode* root, int targetSum) {
        if (!root) return 0;
        // 以 root 为起点的路径数 + 左子树中的路径数 + 右子树中的路径数
        return countFrom(root, targetSum) 
             + pathSum(root->left, targetSum) 
             + pathSum(root->right, targetSum);
    }
    
    // 以 node 为起点向下走，统计和为 remain 的路径数
    int countFrom(TreeNode* node, long long remain) {
        if (!node) return 0;
        int count = 0;
        if (node->val == remain) count = 1; // 当前节点恰好凑齐
        // 继续向下——不能 return，因为后续可能有负数"拉回来"
        count += countFrom(node->left, remain - node->val);
        count += countFrom(node->right, remain - node->val);
        return count;
    }
};
```

**关键点**：
- `countFrom` 中即使 `node->val == remain` 也不能提前 return，因为节点值可以为负，后面可能还有路径和为 0 的子路径。
- `remain` 用 `long long` 避免中间结果溢出（节点值范围 ±10⁹）。

---

### 解法2: 前缀和 + 哈希表 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是"每个节点都要向下遍历整棵子树"，导致 O(n²)。类比数组"连续子数组和 = target"的前缀和解法，我们可以在一次 DFS 中用哈希表记录根到当前路径上所有前缀和出现的次数。

**核心公式**：`当前前缀和 - 某祖先前缀和 == target` → 查找 `currSum - target` 在 map 中的计数。

```
// 数组类比：
// arr:       [a, b, c, d, e]
// prefix:  [0, a, a+b, a+b+c, ...]
// 子数组 [b,c] 的和 = prefix[3] - prefix[1]
// 如果 prefix[3] - prefix[1] == target
// 那么 prefix[1] == prefix[3] - target
// 
// 树上同理：根→当前节点的前缀和 currSum
// 查找路径上有多少祖先的前缀和 == currSum - target
```

```cpp
class Solution {
public:
    int pathSum(TreeNode* root, int targetSum) {
        unordered_map<long long, int> prefixCount;
        prefixCount[0] = 1; // 空前缀：从根开始的路径恰好为 target
        return dfs(root, 0, targetSum, prefixCount);
    }
    
    int dfs(TreeNode* node, long long currSum, int target,
            unordered_map<long long, int>& prefixCount) {
        if (!node) return 0;
        
        currSum += node->val;
        
        // 查找有多少祖先的前缀和 == currSum - target
        int count = prefixCount[currSum - target];
        
        // 把当前前缀和加入 map（供子节点查询）
        prefixCount[currSum]++;
        
        // 递归左右子树
        count += dfs(node->left, currSum, target, prefixCount);
        count += dfs(node->right, currSum, target, prefixCount);
        
        // 【回溯】离开当前节点，撤销当前前缀和
        prefixCount[currSum]--;
        
        return count;
    }
};
```

**关键点**：
- `prefixCount[0] = 1` 是必需的初始化，处理"从根到当前节点的路径和恰好 == target"的情况。
- 回溯 `prefixCount[currSum]--` 确保左子树的前缀和不会影响右子树的查询。

## 解法对比

| | 暴力双重 DFS | 前缀和 + 哈希表 |
|---|---|---|
| 时间 | O(n²) 最坏 | O(n) |
| 空间 | O(n) 递归栈 | O(n) 哈希表 + 递归栈 |
| 思路难度 | 简单直觉 | 需要前缀和类比 |
| 面试推荐 | 作为起点展示理解 | ⭐ 最终答案 |
| 核心区别 | 对每个起点重复遍历 | 一次遍历，哈希表记录历史 |

## 易错点

1. **忘记 `prefixCount[0] = 1`**
   - ✗ 不初始化 → 漏掉从根节点开始的路径
   - ✓ `prefixCount[0] = 1`，代表"空前缀"存在一次

2. **忘记回溯**
   - ✗ 不写 `prefixCount[currSum]--` → 左子树的前缀和"泄漏"到右子树
   - ✓ DFS 返回前必须撤销当前节点的贡献

3. **用 `int` 存前缀和导致溢出**
   - ✗ `int currSum` → 节点值 ±10⁹，路径长 1000，累加可能溢出 int
   - ✓ `long long currSum`

4. **暴力解中 `countFrom` 提前 return**
   - ✗ `if (node->val == remain) return 1;` → 漏掉后续子路径（负数可能拉回来）
   - ✓ 记录 count 后继续递归

5. **查询和插入的顺序搞反**
   - ✗ 先 `prefixCount[currSum]++` 再查 `prefixCount[currSum - target]` → 当 target == 0 时自己匹配自己
   - ✓ 先查再插入

## 面试追问

**Q1: 暴力解的复杂度分析？最坏情况是什么？**
> 最坏情况是树退化为链表（只有左孩子），外层遍历每个节点 O(n)，内层从该节点向下遍历 O(n)，总计 O(n²)。平衡树时内层遍历 O(log n)，总计 O(n log n)。

**Q2: 为什么前缀和方法需要回溯？数组上用前缀和不需要回溯啊？**
> 数组是线性的，从左到右一趟扫完，每个前缀和只属于一条路径。但树有分叉，左子树路径上的前缀和不应该出现在右子树的查询中。回溯就是确保哈希表只包含"从根到当前节点这一条路径"上的前缀和。

**Q3: 如果允许路径不止向下（可以拐弯），怎么做？**
> 那就变成了"树上任意两点间路径和 = target"。可以用树上每个节点为 LCA 的方式思考，或用更复杂的树形 DP。不过这已经是 Hard 级别问题了。

**Q4: 如果要输出所有满足条件的路径（不只是计数），怎么改？**
> 在 DFS 中额外维护一个 `path` 数组记录当前路径。当发现 `currSum - target` 存在时，从 path 末尾回溯找到起始位置，截取子路径加入结果。

## 相关题型

- **112. 路径总和 (Path Sum)** — 本题的简化版：只需判断是否存在根→叶路径和为 target。复用 DFS 框架，但只检查叶节点处的累加和。
- **113. 路径总和 II (Path Sum II)** — 需要输出所有根→叶的路径。复用 DFS + 路径回溯，区别是起点固定为根、终点固定为叶。
- **560. 和为 K 的子数组 (Subarray Sum Equals K)** — 本题的数组版本！完全相同的前缀和 + 哈希表思路，只是在线性数组上不需要回溯。学会本题后 560 可以秒杀。
- **124. 二叉树中的最大路径和** — 路径可以拐弯（经过根），需要不同的 DFS 策略，但同样是树上路径问题。