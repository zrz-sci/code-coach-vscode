# 1373. 二叉搜索子树的最大键值和 (Maximum Sum BST in Binary Tree)

## 难度: Hard | 标签: Tree, DFS, BST, Binary Tree

---

## 题目理解

给一棵二叉树 `root`，找到其中**任意子树**，使得该子树是一棵合法的 BST（二叉搜索树），并且该 BST 中所有节点键值之和最大。返回这个最大键值和。如果不存在正数和的 BST 子树，返回 0（空 BST 的和为 0）。

**BST 定义回顾**：
- 左子树所有节点值 **严格小于** 根节点值
- 右子树所有节点值 **严格大于** 根节点值
- 左右子树也必须都是 BST

---

## ASCII 图示

### Example 1: root = [1,4,3,2,4,2,5,null,null,null,null,null,null,4,6]

```
              1
            /   \
          4       3         <-- 以 3 为根的子树是 BST
         / \     / \
        2   4   2   5       <-- 右子树: 2 < 3, 5 > 3
                   / \          且 5 的子树 {4,6} 也满足
                  4   6

以 3 为根的子树展开:
          3
         / \
        2   5
           / \
          4   6

验证 BST:
- 左子树 {2}: 2 < 3  OK
- 右子树 {5, 4, 6}: 所有值 > 3  OK
  - 4 < 5 < 6  OK
Sum = 3 + 2 + 5 + 4 + 6 = 20  --> 答案
```

### Example 2: root = [4,3,null,1,2]

```
        4
       /
      3
     / \
    1   2

以 3 为根的子树 {3,1,2}:
- 左子树 {1}: 1 < 3  OK
- 右子树 {2}: 2 < 3  FAIL! (右子树值必须 > 根)
  --> 不是 BST

单节点 {2} 是 BST, sum = 2
单节点 {1} 是 BST, sum = 1
单节点 {4} 是 BST, sum = 4... 但 4 不是叶子，以 4 为根的整棵树不是 BST

答案 = max(2, 1) = 2... 等等，4 作为单节点不成立因为它有子树。
实际上每个叶节点的子树就是它自己。
答案 = 2
```

### Example 3: root = [-4,-2,-5]

```
       -4
       / \
     -2  -5

以 -4 为根: -2 > -4 在左子树 -> 不是 BST
单节点 -2: sum = -2 < 0
单节点 -5: sum = -5 < 0
单节点 -4: sum = -4 < 0

所有 BST 的和都为负数 -> 返回 0 (空 BST)
```

---

## 核心思路

**后序遍历 + 自底向上信息聚合**

关键洞察：判断一棵子树是否为 BST，需要同时知道：
1. 左子树是否是 BST
2. 右子树是否是 BST
3. 左子树的最大值（必须 < 当前节点值）
4. 右子树的最小值（必须 > 当前节点值）

这些信息只能从子节点**向上传递**，所以采用后序遍历（左-右-根），每个节点返回一个四元组 `{isBST, sum, minVal, maxVal}`。

---

## 思维链

```
1. 读完题 --> 对每个节点，检查以它为根的子树是否是 BST，若是则算键值和
2. 暴力做法 --> 对每个节点调用 isValidBST() + sumOfTree()
   时间 O(n^2)，每个节点最坏遍历整棵子树
3. 优化思考 --> 能否一次遍历就搞定？
   后序遍历，自底向上传递信息，O(n)
4. 需要传什么？ --> {isBST, sum, min, max}
   - isBST: 该子树是否合法 BST
   - sum: 该子树所有节点之和
   - min: 该子树最小值（用于父节点右子树检查）
   - max: 该子树最大值（用于父节点左子树检查）
5. 合并逻辑:
   当前节点是 BST <=> 左是BST AND 右是BST AND val > left.max AND val < right.min
   若是: sum = left.sum + right.sum + val, 更新全局答案
   若否: 向上报告"不是BST"，后续字段不再需要
6. 边界:
   空节点 -> {true, 0, INT_MAX, INT_MIN}
   全负 -> ans 初始化为 0（空BST的和）
```

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 特点 |
|------|------|------|------|------|
| **后序DFS + 信息传递** | 自底向上判断BST + 累计和 | O(n) | O(h) | 推荐，一次遍历 |

只有一种最优解法，核心是信息传递的设计。

---

## 解法详解: 后序DFS + 信息传递 -- O(n) / O(h)

### 算法步骤

```
Step 1: 定义 dfs(node) 返回 {isBST, sum, minVal, maxVal}

Step 2: 递归基 -- 空节点
        返回 {true, 0, INT_MAX, INT_MIN}
        解释: 空树是合法 BST
              min=INT_MAX 使得 "node->val > left.max" 对任何 val 都成立
              max=INT_MIN 使得 "node->val < right.min" 对任何 val 都成立

Step 3: 递归调用左右子树
        auto [lBST, lSum, lMin, lMax] = dfs(node->left)
        auto [rBST, rSum, rMin, rMax] = dfs(node->right)

Step 4: 判断当前子树是否为 BST
        条件: lBST && rBST && node->val > lMax && node->val < rMin

Step 5: 若是 BST:
        sum = lSum + rSum + node->val
        ans = max(ans, sum)              // 更新全局答案
        return {true, sum, min(lMin, node->val), max(rMax, node->val)}

Step 6: 若不是 BST:
        return {false, 0, 0, 0}          // 后续字段无意义
```

### 信息传递图解

```
          node(5)
         /      \
      left(3)   right(8)

left 返回:  {true, sum=3, min=3, max=3}  (叶节点)
right 返回: {true, sum=8, min=8, max=8}  (叶节点)

检查: true && true && 5 > 3 && 5 < 8 --> YES, 是 BST!
sum = 3 + 8 + 5 = 16
返回: {true, 16, min(3,5)=3, max(8,5)=8}
```

### 代码实现

```cpp
class Solution {
    int ans = 0;

    // 返回 {isBST, sum, minVal, maxVal}
    tuple<bool, int, int, int> dfs(TreeNode* node) {
        if (!node) return {true, 0, INT_MAX, INT_MIN};

        auto [lBST, lSum, lMin, lMax] = dfs(node->left);
        auto [rBST, rSum, rMin, rMax] = dfs(node->right);

        if (lBST && rBST && node->val > lMax && node->val < rMin) {
            int sum = lSum + rSum + node->val;
            ans = max(ans, sum);
            return {true, sum, min(lMin, node->val), max(rMax, node->val)};
        }
        return {false, 0, 0, 0};
    }

public:
    int maxSumBST(TreeNode* root) {
        dfs(root);
        return ans;
    }
};
```

---

## 四元组设计的精妙之处

### 为什么空节点要用 {true, 0, INT_MAX, INT_MIN}？

```
考虑叶节点 node(7):

left  = nullptr -> {true, 0, INT_MAX, INT_MIN}
right = nullptr -> {true, 0, INT_MAX, INT_MIN}

检查: true && true && 7 > INT_MIN && 7 < INT_MAX -> YES!
sum = 0 + 0 + 7 = 7
min = min(INT_MAX, 7) = 7
max = max(INT_MIN, 7) = 7
返回: {true, 7, 7, 7}  <-- 完美！

如果 min 设为 INT_MIN:
min = min(INT_MIN, 7) = INT_MIN  <-- 错误！
```

### 为什么非BST节点返回 {false, 0, 0, 0}？

```
一旦某个子树不是 BST:
  --> 包含它的所有更大子树也不可能是 BST
  --> sum/min/max 不会被任何父节点使用
  --> 返回什么都无所谓，{false, 0, 0, 0} 最简单
```

---

## 关键提示序列（渐进式）

| # | 提示 | 思路方向 |
|---|------|----------|
| 1 | 暴力做法：对每个节点验证BST+求和，复杂度是多少？ | O(n^2)，需要优化 |
| 2 | 能否在一次遍历中同时完成验证和求和？ | 后序遍历 |
| 3 | 子节点需要向父节点传递什么信息？ | isBST + sum + min + max |
| 4 | 空节点应该返回什么？ | {true, 0, INT_MAX, INT_MIN} |
| 5 | 全局答案应该初始化为什么？为什么？ | 0，因为空BST的和为0 |

---

## 易错点

1. **空节点的 min/max 设置错误**
   - 错误: `min = 0, max = 0` --> 节点值为 0 时判断出错
   - 正确: `min = INT_MAX, max = INT_MIN` --> 保证任何值都满足条件

2. **ans 初始化为 0 而非 INT_MIN**
   - 题意: 如果所有 BST 子树的和都为负数，答案是 0（空 BST）
   - Example 3 就是这种情况：所有值为负

3. **严格不等号**
   - BST 定义是 `<` 和 `>`，不是 `<=` 和 `>=`
   - `node->val > lMax` 且 `node->val < rMin`，不能有等号

4. **忘记更新 min 和 max**
   - 返回时: `min = min(lMin, node->val)`, `max = max(rMax, node->val)`
   - 不能只返回 `{true, sum, lMin, rMax}`，因为叶节点的子树 min/max 就是自己

5. **整数溢出**
   - 节点值范围 [-4*10^4, 4*10^4]，最多 4*10^4 个节点
   - 最大 sum = 4*10^4 * 4*10^4 = 1.6*10^9，刚好在 int 范围内
   - 但如果用 long long 更安全

---

## 复杂度分析

- **时间: O(n)** -- 每个节点恰好访问一次
- **空间: O(h)** -- 递归栈深度等于树高
  - 最坏情况（链状树）: O(n)
  - 平均/平衡情况: O(log n)

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 98 | Validate Binary Search Tree | BST 验证基础 |
| 333 | Largest BST Subtree | 找最大 BST 子树（按节点数） |
| 124 | Binary Tree Maximum Path Sum | 后序DFS + 全局最优，类似信息传递模式 |
| 337 | House Robber III | 树上 DP，后序传递多个值 |

---

## 面试追问

1. **如果要求返回最大 BST 子树的节点数而非键值和，怎么改？**
   - 四元组改为 `{isBST, count, min, max}`，合并时 `count = lCount + rCount + 1`
   - 即 LeetCode 333: Largest BST Subtree

2. **如果树非常深（百万层），递归栈溢出怎么办？**
   - 方案1: 将递归改为迭代后序遍历，用显式栈 + 状态机
   - 方案2: Morris 遍历（但后序 Morris 较复杂，面试不推荐）
   - 实际工程中设置系统栈大小或使用协程

3. **如果允许修改一个节点的值使其变成 BST，最大和是多少？**
   - 更难的变体，需要对每个非 BST 子树考虑哪个节点破坏了 BST 性质
   - 可以在 DFS 中额外记录"差一个违规节点"的状态
   - 开放性问题，面试中展示思路即可

---

## 面试技巧

1. **先说暴力再优化**：先提到 O(n^2) 暴力，展示你能想到基础解法，再自然引出优化
2. **画出四元组**：面试时在白板上画出 `{isBST, sum, min, max}` 的传递过程
3. **用 Example 1 走一遍**：手动模拟叶节点 -> 中间节点 -> 根节点的信息传递
4. **主动提到边界**：空节点、全负数、单节点树
