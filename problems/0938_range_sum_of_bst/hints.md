# 938. 二叉搜索树的范围和

## 核心思路

本质上在问：**遍历 BST，把值在 [low, high] 范围内的节点值加起来**。关键是利用 BST 的有序性来剪枝——不需要遍历所有节点。

## 思维链

1. **读完题第一反应**：遍历整棵树，对每个节点判断值是否在 [low, high] 范围内，在就累加。这是最朴素的做法，适用于任何二叉树。
2. **暴力解的瓶颈**：遍历了所有 n 个节点，但很多节点的值明显不在范围内。比如 `root.val < low`，那 root 的整个左子树都不可能在范围内（因为 BST 左子树所有值都小于 root）。
3. **怎么突破瓶颈？**：利用 BST 性质剪枝！
   - 当前节点值 < low → 左子树全部 < low，直接跳过左子树，只搜右子树
   - 当前节点值 > high → 右子树全部 > high，直接跳过右子树，只搜左子树
   - 当前节点值在 [low, high] → 累加当前值，左右子树都搜
4. **具体怎么用？**：递归或迭代 DFS，每次根据当前节点值和 [low, high] 的关系决定搜索方向。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 DFS | 遍历所有节点，逐个判断 | O(n) | O(h) | 能说出即可 |
| BST 剪枝递归 | 利用 BST 性质跳过不可能的子树 | O(n) 最坏，实际远小于 n | O(h) | ⭐ 必须写出 |
| BST 剪枝迭代 | 用栈模拟递归 + 同样的剪枝 | O(n) 最坏，实际远小于 n | O(h) | 加分项 |

> h 是树高，平衡 BST 时 h = O(log n)，最坏退化为 O(n)。

## 关键提示

- **提示1**：BST 最核心的性质是什么？左子树所有值 < 根 < 右子树所有值。这意味着你可以根据根的值判断"整棵子树"是否需要搜索。

- **提示2**：想想什么时候可以完全跳过左子树？什么时候可以完全跳过右子树？画图验证一下。

- **提示3**：剪枝后的复杂度取决于有多少节点在 [low, high] 范围内或其路径上。如果 [low, high] 覆盖范围很小，剪枝效果非常显著。

- **BST 示例图（示例1）**：
```
//           10          low=7, high=15
//          /  \
//         5    15       
//        / \     \
//       3   7    18
//
// 从根10开始: 7 ≤ 10 ≤ 15 → 累加10，左右都搜
//   左子树5: 5 < 7 → 不累加5，跳过左子树(3)，只搜右子树
//     右子7: 7 ≤ 7 ≤ 15 → 累加7，无子节点
//   右子树15: 7 ≤ 15 ≤ 15 → 累加15，跳过右子树(18>15)
// 结果: 10 + 7 + 15 = 32
```

## 解法详解

### 解法1: 暴力 DFS（不利用 BST 性质） — O(n) / O(h)

**思考过程**：最直觉的做法——把这道题当作普通二叉树来做。遍历每个节点，值在范围内就加，不在就不加。

```cpp
class Solution {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        
        int sum = 0;
        // 当前节点在范围内就累加
        if (root->val >= low && root->val <= high) {
            sum += root->val;
        }
        // 无论如何都遍历左右子树 — 没有利用 BST 性质
        sum += rangeSumBST(root->left, low, high);
        sum += rangeSumBST(root->right, low, high);
        
        return sum;
    }
};
```

**关键点**：这个解法正确但低效——对于 BST 而言，很多子树可以直接跳过。

---

### 解法2: BST 剪枝递归 — O(n) 最坏 / O(h) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是"遍历了所有节点"。BST 的性质告诉我们：
- `root->val < low` → 根和整个左子树都 < low，只需搜右子树
- `root->val > high` → 根和整个右子树都 > high，只需搜左子树

这就是剪枝——砍掉不可能包含答案的分支。

```cpp
class Solution {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        
        // 当前值太小，左子树更小，只搜右子树
        if (root->val < low) {
            return rangeSumBST(root->right, low, high);
        }
        // 当前值太大，右子树更大，只搜左子树
        if (root->val > high) {
            return rangeSumBST(root->left, low, high);
        }
        // 当前值在 [low, high] 内 → 累加 + 左右都搜
        return root->val 
             + rangeSumBST(root->left, low, high)
             + rangeSumBST(root->right, low, high);
    }
};
```

**剪枝过程可视化（示例1）**：
```
//           10          [low=7, high=15]
//          /  \
//         5    15       
//        / \     \
//       3   7    18
//
// 递归(10): 7 ≤ 10 ≤ 15 → 累加10, 搜左+右
//   递归(5):  5 < 7 → 跳过左子树(3), 只搜右
//     递归(7):  7 ≤ 7 ≤ 15 → 累加7, 左右皆null
//   递归(15): 7 ≤ 15 ≤ 15 → 累加15, 搜左(null)+右
//     递归(18): 18 > 15 → 跳过右子树, 搜左(null)
//
// 总共访问: 10, 5, 7, 15, 18 = 5个节点 (跳过了3)
// 如果范围更窄，跳过的更多
```

**关键点**：三个分支条件互斥且覆盖所有情况，不会遗漏节点。

---

### 解法3: BST 剪枝迭代（栈） — O(n) 最坏 / O(h)

**思考过程**：面试中可能被追问"能否不用递归？"。用栈模拟 DFS，核心剪枝逻辑完全一样。

```cpp
class Solution {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        
        int sum = 0;
        stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            
            if (!node) continue;
            
            if (node->val < low) {
                // 只需搜右子树
                stk.push(node->right);
            } else if (node->val > high) {
                // 只需搜左子树
                stk.push(node->left);
            } else {
                // 在范围内：累加 + 左右都搜
                sum += node->val;
                stk.push(node->left);
                stk.push(node->right);
            }
        }
        
        return sum;
    }
};
```

**关键点**：迭代版的剪枝和递归版完全一致，只是用栈代替了函数调用栈。`push(null)` 在循环开头被 `continue` 跳过，所以安全。

## 解法对比

| | 暴力 DFS | BST 剪枝递归 | BST 剪枝迭代 |
|---|---|---|---|
| 利用 BST 性质 | ✗ | ✓ | ✓ |
| 实现复杂度 | 最简单 | 简单 | 中等 |
| 栈溢出风险 | 树极深时有 | 树极深时有 | 无（显式栈） |
| 面试推荐 | 作为起点 | **首选** | 追问时给出 |

## 易错点

- ✗ **只判断 `root->val >= low && root->val <= high` 就加，忘记剪枝**：功能正确但没展示对 BST 的理解，面试中会被认为"没抓住重点"。

- ✗ **剪枝条件写反**：`root->val < low` 时应该搜**右子树**（值更大的方向），不是左子树。
  ```
  ✗: if (root->val < low) return rangeSumBST(root->left, ...);
  ✓: if (root->val < low) return rangeSumBST(root->right, ...);
  ```

- ✗ **迭代版忘记处理 null 节点**：push 进栈的可能是 null，弹出时不判断会空指针访问。
  ```
  ✓: if (!node) continue;  // 必须在弹出后立即检查
  ```

- ✗ **误以为只需判断等于 low 或等于 high 的节点**：范围是闭区间 [low, high]，中间的值也要累加。

## 面试追问

**Q1（基础理解）**：如果这不是 BST 而是普通二叉树，你怎么做？复杂度是多少？
> 遍历所有节点逐个判断，O(n)。无法剪枝因为没有有序性。

**Q2（优化理解）**：BST 剪枝后最好情况和最坏情况的时间复杂度分别是多少？
> 最好 O(log n)：当 [low, high] 很窄时只需走一条从根到叶的路径。最坏 O(n)：当 [low, high] 覆盖整棵树时退化为全遍历。

**Q3（变体）**：如果需要多次查询不同的 [low, high] 范围和，怎么优化？
> 预处理：对 BST 做中序遍历得到有序数组，然后计算前缀和。每次查询用二分找到 low 和 high 的位置，O(log n) 完成查询。预处理 O(n)，每次查询 O(log n)。

**Q4（变体）**：如果要求返回范围内节点的个数而不是和呢？
> 代码几乎不变，把 `sum += root->val` 改成 `count += 1` 即可，剪枝逻辑完全一样。

## 相关题型

- **270. 最接近的二叉搜索树值** — 复用"BST 中根据大小关系决定搜索方向"的思路，区别：本题累加范围内所有值，270 只找最近的一个值。
- **530. 二叉搜索树的最小绝对差** — 复用 BST 中序遍历有序的性质，中序遍历后比较相邻差值。
- **653. 两数之和 IV - 输入 BST** — BST + 哈希表/双指针，复用 BST 遍历框架。
- **1038. 从二叉搜索树到更大和树** — 复用 BST 的"反向中序遍历"，理解 BST 有序性的另一种应用。