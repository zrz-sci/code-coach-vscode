# 98. 验证二叉搜索树

## 核心思路

BST 的定义是**全局约束**：左子树的**所有**节点都小于根，右子树的**所有**节点都大于根。因此不能只看"当前节点和它的左右孩子"，而要传递一个合法的值域范围 `(min, max)` 向下递归，或者利用"BST 的中序遍历是严格递增序列"这一性质来判断。

## 思维链

1. **读完题第一反应**：对每个节点检查 `left->val < node->val < right->val`？→ 这**只检查了直接孩子**，没有检查更深层的节点。例如下图中节点 3 在右子树中但比根 5 小，直接比孩子是发现不了的。
2. **为什么只比孩子不够？** 因为 BST 是全局约束——右子树中**所有**节点都必须 > root。如果我们暴力地对每个节点收集整个子树的值再判断，时间 O(n²)。
3. **突破口1 — 上下界传递**：换个角度思考——每个节点都有一个"合法的取值范围"。根节点范围 (-∞, +∞)，左子树范围 (-∞, root->val)，右子树范围 (root->val, +∞)。递归时把范围传下去，每个节点只需 O(1) 判断是否在范围内，总时间 O(n)。
4. **突破口2 — 中序遍历**：BST 中序遍历的结果是严格递增的。所以做一次中序遍历，检查每个值是否严格大于前一个值即可。这是一种非常优雅的等价转换。
5. **递归 vs 迭代**：中序遍历可以用递归实现，也可以用栈模拟迭代实现。面试中两种都要能写。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 上下界递归 | 传递 (min, max) 范围，DFS 检查每个节点 | O(n) | O(h) | ⭐ 必须写出 |
| 中序遍历（递归） | BST 中序严格递增，记录 prev 判断 | O(n) | O(h) | ⭐ 必须写出 |
| 中序遍历（迭代栈） | 用栈模拟中序遍历 + prev 判断 | O(n) | O(h) | 加分项 |

> h = 树高，平衡树 O(log n)，最坏链状 O(n)

## 关键提示

1. **经典陷阱**：不能只比较 `node->val > node->left->val`，要确保左子树**所有**节点都小于 node。想想下面这棵树为什么不是 BST：
   ```
   //       5
   //      / \
   //     1   4      ← 4 < 5，右孩子比根小！
   //        / \
   //       3   6    ← 3 < 5，在右子树中但比根小
   ```

2. **节点值范围是 INT_MIN ~ INT_MAX**：如果你用 `int` 类型做边界，遇到节点值恰好等于 INT_MIN 或 INT_MAX 会出 bug。用 `long long` 或用指针/optional 表示"无穷大/无穷小"。

3. **中序遍历的等价视角**：BST ←→ 中序遍历严格递增。记住这个等价关系，很多 BST 题都用得到。

4. **prev 用什么类型？** 中序遍历法中 `prev` 记录上一个访问的节点。建议用 `TreeNode*` 指针（初始 nullptr），避免 `int` 的边界问题。

## 解法详解

### 解法1: 上下界递归 — O(n) / O(h) ⭐ 面试首选

**思考过程**：每个节点必须在一个合法范围内。根节点范围 (-∞, +∞)，往左走上界变为当前值，往右走下界变为当前值。这样每个节点只需 O(1) 判断。

```
//  以 [5,1,4,null,null,3,6] 为例，传递范围:
//
//         5 (-∞, +∞)          ✓ 5在范围内
//        / \
//   1(-∞,5)  4(5,+∞)         ✓ 1在范围内; ✗ 4不在(5,+∞)中! → false
//            / \
//       3(5,4) 6(4,+∞)       3的范围已经矛盾了(5 > 4)
```

```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LONG_MIN, LONG_MAX);
    }
    
    bool validate(TreeNode* node, long long lower, long long upper) {
        if (!node) return true;  // 空节点不违反任何约束
        
        // 当前节点值必须严格在 (lower, upper) 范围内
        if (node->val <= lower || node->val >= upper)
            return false;
        
        // 左子树的上界收紧为 node->val
        // 右子树的下界收紧为 node->val
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }
};
```

**关键点**：
- 用 `long long` 避免节点值等于 INT_MIN/INT_MAX 时的边界问题
- 范围是**开区间** (lower, upper)，所以判断用 `<=` 和 `>=`（严格小于/大于）

---

### 解法2: 中序遍历（递归）— O(n) / O(h)

**从另一个角度切入**：BST 的中序遍历一定是严格递增序列。只要在中序遍历过程中，每次检查当前值是否严格大于前一个值即可。

```
//  正确的 BST:            中序遍历: [1, 2, 3] ← 严格递增 ✓
//       2
//      / \
//     1   3
//
//  错误的 BST:            中序遍历: [1, 5, 3, 4, 6]
//       5                                ↑ 3 < 5，不递增 ✗
//      / \
//     1   4
//        / \
//       3   6
```

```cpp
class Solution {
public:
    TreeNode* prev = nullptr;  // 记录中序遍历中的前一个节点
    
    bool isValidBST(TreeNode* root) {
        return inorder(root);
    }
    
    bool inorder(TreeNode* node) {
        if (!node) return true;
        
        // 左子树
        if (!inorder(node->left)) return false;
        
        // 处理当前节点：检查是否严格大于前一个
        if (prev && node->val <= prev->val) return false;
        prev = node;  // 更新 prev
        
        // 右子树
        return inorder(node->right);
    }
};
```

**关键点**：
- `prev` 用 `TreeNode*` 而不是 `int`，避免初始值和 INT_MIN 冲突
- 一旦发现不满足立即返回 false，**提前终止**，不需要遍历完整棵树

---

### 解法3: 中序遍历（迭代栈）— O(n) / O(h)

**从解法2扩展**：递归本质上用系统栈，我们用显式栈模拟中序遍历，逻辑一致但更好控制。

```
//  迭代中序遍历 [2,1,3]:
//  Step1: 栈=[], cur=2 → 一路向左入栈
//  Step2: 栈=[2], cur=1 → 继续向左
//  Step3: 栈=[2,1], cur=null → 弹出1, 访问1, prev=null→1, cur=null(1无右子)
//  Step4: 栈=[2], cur=null → 弹出2, 访问2, 2>1✓, prev=1→2, cur=3
//  Step5: 栈=[3], cur=null → 弹出3, 访问3, 3>2✓, prev=2→3, cur=null
//  Step6: 栈=[], cur=null → 结束, return true
```

```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        TreeNode* prev = nullptr;
        
        while (cur || !stk.empty()) {
            // 一路向左，全部入栈
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            
            // 弹出栈顶 = 当前最小的未访问节点
            cur = stk.top();
            stk.pop();
            
            // 检查是否严格递增
            if (prev && cur->val <= prev->val) return false;
            prev = cur;
            
            // 转向右子树
            cur = cur->right;
        }
        
        return true;
    }
};
```

**关键点**：
- 迭代中序遍历的经典模板：**一路向左入栈 → 弹出处理 → 转右子树**
- 面试中如果被追问"不用递归怎么做"，就给这个版本

---

## 解法对比

| | 上下界递归 | 中序递归 | 中序迭代 |
|---|---|---|---|
| 核心思想 | 每个节点有合法值域 | BST中序=严格递增 | 同左，用栈模拟 |
| 代码量 | 最少 | 适中 | 较多 |
| 直觉性 | 非常直觉 | 需要知道BST性质 | 需要理解栈模拟 |
| 适用场景 | 面试首选 | 优雅的替代方案 | 追问不用递归时 |

三种解法时间空间复杂度完全一样：O(n) / O(h)。面试中**上下界递归**最好写、最好解释；**中序遍历**展示你理解 BST 的核心性质。

## 易错点

1. **只比较直接孩子而不是整个子树**
   - ✗ `if (node->left && node->left->val >= node->val) return false;` — 这只检查了左孩子，没检查左子树更深的节点
   - ✓ 用上下界传递或中序遍历确保**全局约束**

2. **用 int 类型做边界导致溢出**
   - ✗ `validate(root, INT_MIN, INT_MAX)` — 如果节点值恰好是 INT_MIN，`node->val <= lower` 即 `INT_MIN <= INT_MIN` 为 true，会误判
   - ✓ 用 `long long`：`validate(root, LONG_MIN, LONG_MAX)`
   - ✓ 或用 `TreeNode*` / `optional<int>` 表示无穷

3. **中序遍历的 prev 初始值**
   - ✗ `int prev = INT_MIN;` — 如果第一个节点值恰好是 INT_MIN 就会误判
   - ✓ `TreeNode* prev = nullptr;` 然后 `if (prev && ...)` 跳过第一次比较

4. **忘记 BST 是严格不等式**
   - ✗ `if (node->val < lower || node->val > upper)` — 允许了等于
   - ✓ `if (node->val <= lower || node->val >= upper)` — BST 不允许重复

## 面试追问

**Q1: 你的解法为什么不能只比较每个节点和它的左右孩子？**
→ 因为 BST 的约束是全局的。右子树中所有节点都要大于根，不只是直接右孩子。例如 `[5,1,4,null,null,3,6]` 中，3 是 5 的右子树中的节点但 3 < 5。

**Q2: 节点值可以是 INT_MIN 或 INT_MAX，你怎么处理边界？**
→ 方案A：用 `long long` 做边界。方案B：不传具体值，传 `TreeNode*` 指针，nullptr 表示无穷大/小。方案C：用中序遍历法，prev 初始为 nullptr 天然避免边界问题。

**Q3: 如果不允许递归（栈溢出风险），怎么做？**
→ 用迭代栈模拟中序遍历（解法3），显式维护栈代替系统调用栈，空间仍为 O(h) 但不会栈溢出。

**Q4: 如果要找出 BST 中第一个违反规则的节点呢？（变体，类似 LeetCode 99）**
→ 中序遍历法天然支持：当 `cur->val <= prev->val` 时，prev 和 cur 就是两个"错位"的节点。这正是 LeetCode 99 "恢复二叉搜索树" 的思路。

## 相关题型

- **LeetCode 94 二叉树的中序遍历** — 本题解法2/3直接复用中序遍历模板，区别只是遍历过程中做判断而非收集结果
- **LeetCode 99 恢复二叉搜索树** — 复用中序遍历 + prev 框架，区别是找到两个错位节点并交换值
- **LeetCode 700 二叉搜索树中的搜索** — 复用 BST 的"比根小往左，比根大往右"的搜索逻辑
- **LeetCode 230 二叉搜索树中第K小的元素** — 复用中序遍历框架，遍历到第 K 个时返回
- **LeetCode 501 二叉搜索树中的众数** — 复用中序遍历 + prev 框架，额外维护频率计数