# 108. 将有序数组转换为二叉搜索树

## 核心思路

**本质问题**：给定一个升序数组，构造一棵高度平衡的 BST。升序数组就是 BST 的中序遍历结果，而"高度平衡"意味着每次要选**中间元素**作为根，使左右子树节点数尽量相等——这就是经典的**分治**思想。

## 思维链

1. **读完题第一反应**：升序数组 → BST 的中序遍历就是升序的，所以这个数组天然就是目标 BST 的中序遍历结果。问题变成了"从中序遍历还原一棵平衡 BST"。

2. **怎么保证"高度平衡"？** 如果我每次都选第一个元素当根，那所有剩余元素都在右子树，退化成链表。要平衡，应该让左右子树的节点数尽量相等。

3. **关键洞察**：选数组**中间元素**作为根！中间元素左边的构成左子树，右边的构成右子树，两边节点数最多差 1，天然平衡。

4. **递归结构自然浮现**：每次选中间元素 → 递归处理左半部分构建左子树 → 递归处理右半部分构建右子树。这就是**分治法**：分解 → 递归解决 → 合并。

5. **能不能用迭代？** 可以！用栈模拟递归过程，每次从栈中取出一个待处理区间，找中点建节点，再把左右子区间入栈。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归分治 | 每次选中间元素为根，递归构建左右子树 | O(n) | O(log n) 递归栈 | ⭐ 必须写出 |
| 迭代（栈模拟） | 用栈存储待处理的区间和父节点信息 | O(n) | O(log n) 栈空间 | 加分项 |

## 关键提示

- **提示1**：升序数组的中间元素就是根节点。为什么？因为这样左右两边的元素数量最接近，保证了"高度平衡"。

- **提示2**：递归的终止条件是什么？当区间为空时（`left > right`），返回 `nullptr`。

- **提示3**：中间位置的选取 `mid = left + (right - left) / 2`，偏左还是偏右都可以，只是得到的合法 BST 不同。

- **提示4**：画出递归过程帮助理解：
```
数组: [-10, -3, 0, 5, 9]
索引:   0    1  2  3  4

第1层: mid=2 → 根=0
       左区间[0,1]  右区间[3,4]

第2层: 左 mid=0 → 根=-10     右 mid=3 → 根=5
       左[空] 右[1,1]         左[空] 右[4,4]

第3层: 右[1,1] mid=1 → -3    右[4,4] mid=4 → 9

结果:
          0
         / \
       -10   5
         \    \
         -3    9
```

## 解法详解

### 解法1: 递归分治 — O(n) / O(log n) ⭐ 面试首选

**思考过程**：BST 的中序遍历是升序的。要构建平衡 BST，关键在于选哪个元素当根。选中间元素，左右子树节点数最接近 → 高度差 ≤ 1 → 平衡。对左半和右半分别递归，就是分治。

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
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return build(nums, 0, nums.size() - 1);
    }
    
    // 用 nums[left..right] 构建平衡 BST
    TreeNode* build(vector<int>& nums, int left, int right) {
        if (left > right) return nullptr; // 区间为空，返回空节点
        
        // 选中间位置作为根，保证左右子树节点数差 ≤ 1
        int mid = left + (right - left) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        
        // 左半部分递归构建左子树，右半部分递归构建右子树
        root->left = build(nums, left, mid - 1);
        root->right = build(nums, mid + 1, right);
        
        return root;
    }
};
```

**关键点**：
- 递归三要素：返回值是 `TreeNode*`（构建好的子树根节点），终止条件是 `left > right`，单层逻辑是选中点建根、递归建左右子树。
- 中间位置的计算用 `left + (right - left) / 2` 防止整数溢出（虽然本题数据量小不会溢出，但这是好习惯）。

### 解法2: 迭代（栈模拟分治） — O(n) / O(log n)

**从解法1出发**：递归本质上用系统调用栈存储每次递归的 `(left, right)` 区间。我们可以自己用栈来模拟这个过程。难点在于：建完根节点后，还需要把左右子树"接"到正确的位置，所以栈里除了区间信息，还需要存储父节点和方向。

```cpp
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if (nums.empty()) return nullptr;
        
        // 栈中存储: {节点指针, left, right}
        // 节点已创建但左右子树尚未构建
        int mid = (0 + (int)nums.size() - 1) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        
        // 三元组: (当前节点, 左边界, 右边界)
        // 用两个栈分别存左子区间和右子区间待处理信息
        stack<tuple<TreeNode*, int, int>> stk;
        stk.push({root, 0, (int)nums.size() - 1});
        
        while (!stk.empty()) {
            auto [node, left, right] = stk.top();
            stk.pop();
            
            int m = left + (right - left) / 2;
            
            // 处理左子区间 [left, m-1]
            if (left <= m - 1) {
                int lmid = left + (m - 1 - left) / 2;
                node->left = new TreeNode(nums[lmid]);
                stk.push({node->left, left, m - 1});
            }
            
            // 处理右子区间 [m+1, right]
            if (m + 1 <= right) {
                int rmid = (m + 1) + (right - m - 1) / 2;
                node->right = new TreeNode(nums[rmid]);
                stk.push({node->right, m + 1, right});
            }
        }
        
        return root;
    }
};
```

**关键点**：
- 迭代版本的核心思想完全相同：对每个区间找中点建节点，然后把左右子区间入栈等待处理。
- 比递归版本复杂不少，面试中优先写递归版，被追问时能说出迭代思路即可。

## 解法对比

| | 递归分治 | 迭代（栈模拟） |
|---|---|---|
| 时间复杂度 | O(n) | O(n) |
| 空间复杂度 | O(log n) 递归栈 | O(log n) 显式栈 |
| 代码简洁度 | ⭐⭐⭐ 极简 | ⭐ 较复杂 |
| 面试推荐 | ✅ 首选 | 追问时展示 |
| 本质区别 | 系统栈管理递归 | 手动用栈模拟相同逻辑 |

**结论**：这道题递归解法非常自然优雅，面试中直接写递归即可。迭代版本作为"如果面试官追问能否不用递归"的备选。

## 易错点

- **✗** `int mid = (left + right) / 2;` 当 `left + right` 超过 INT_MAX 时溢出
  **✓** `int mid = left + (right - left) / 2;` 安全写法

- **✗** 传入 `build(nums, 0, nums.size())` — `size()` 返回的是无符号类型，且右边界应该是 `size()-1`（闭区间）
  **✓** `build(nums, 0, nums.size() - 1)` — 但注意当 `nums` 为空时 `size()-1` 会下溢成巨大值
  **✓✓** 最安全：先判空，或用 `(int)nums.size() - 1`

- **✗** 递归时写成 `build(nums, left, mid)` — 中间元素已经用作根了，左子树应该是 `[left, mid-1]`
  **✓** `build(nums, left, mid - 1)` 和 `build(nums, mid + 1, right)`

## 面试追问

**Q1: 为什么选中间元素就能保证平衡？**
> 选中间元素后，左半部分有 ⌊n/2⌋ 个节点，右半部分有 ⌈n/2⌉-1 个节点（或反过来），两边节点数最多差 1。递归下去每层都满足这个性质，所以整棵树的高度差 ≤ 1，即高度平衡。

**Q2: 如果输入不是数组而是链表呢？（LeetCode 109）**
> 数组可以 O(1) 随机访问中间元素，链表不行。两种方案：
> - 方案A：先把链表转成数组，然后用本题解法。O(n) 时间 + O(n) 额外空间。
> - 方案B：用快慢指针找链表中点，O(n log n) 时间 + O(log n) 空间。
> - 方案C：模拟中序遍历，边遍历链表边构建树，O(n) 时间 + O(log n) 空间（最优）。

**Q3: 答案是否唯一？怎么枚举所有合法的平衡 BST？**
> 不唯一。当数组长度为偶数时，中间位置可以选偏左或偏右，两种都合法。如果要枚举所有可能，需要在 mid 的选择上做分支——这就变成了一道回溯题（类似 LeetCode 95 不同的二叉搜索树 II）。

## 相关题型

- **109. 有序链表转换二叉搜索树** — 输入从数组变成链表，核心分治思路相同，区别在于找中点的方式（快慢指针 vs 直接计算索引）。
- **95. 不同的二叉搜索树 II** — 本题只需构造一棵合法 BST，95 要枚举所有可能的 BST。复用同样的分治框架，但每层循环枚举所有可能的根。
- **96. 不同的二叉搜索树** — 95 的计数版本，用 DP 代替实际构造树。
- **1382. 将二叉搜索树变平衡** — 先中序遍历得到有序数组，然后直接复用本题的解法。