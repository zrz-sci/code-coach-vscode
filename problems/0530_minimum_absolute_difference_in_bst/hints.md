# 530. 二叉搜索树的最小绝对差

## 核心思路

BST 的中序遍历是**有序递增**序列，有序序列中最小差值一定出现在**相邻元素**之间。所以这道题本质上是：对 BST 做中序遍历，比较每对相邻节点的差值，取最小值。

## 思维链

1. **读完题第一反应**：任意两个节点的差值最小 → 暴力做法是枚举所有节点对，算差值取最小，O(n²)。
2. **关键观察**：这不是普通二叉树，而是 **BST**！BST 有什么特殊性质？→ 中序遍历是有序的。
3. **有序数组的最小差值在哪？** → 一定在排序后的**相邻元素**之间（如果 a < b < c，则 |a-c| > |a-b| 或 |b-c|）。
4. **所以**：中序遍历 BST，只需比较**当前节点与前一个节点**的差值，维护全局最小值即可。
5. **优化空间**：不需要真的存整个有序数组，只需记住"上一个访问的节点值"就够了 → O(1) 额外空间（不算递归栈）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 中序遍历存数组 | 中序遍历得到有序数组，遍历数组求相邻最小差 | O(n) | O(n) | 能说出即可 |
| 中序递归 + prev 指针 | 递归中序遍历，用 prev 记录前一个节点 | O(n) | O(h) | ⭐ 必须写出 |
| 中序迭代 + prev 指针 | 用栈模拟中序遍历，同样维护 prev | O(n) | O(h) | 加分项 |

## 关键提示

1. **BST 中序遍历 = 有序序列**，这是解题的核心突破口。画出示例1的中序遍历顺序：
   ```
   //       4
   //      / \
   //     2   6        中序遍历: 1 → 2 → 3 → 4 → 6
   //    / \
   //   1   3
   //
   //   相邻差值: |2-1|=1, |3-2|=1, |4-3|=1, |6-4|=2
   //   最小差值 = 1
   ```

2. **为什么最小差值一定在相邻元素？** 如果有序序列 a ≤ b ≤ c，那么 c - a = (c - b) + (b - a) ≥ min(c-b, b-a)。所以跨过中间元素的差值不可能更小。

3. **prev 变量的初始化**：第一个访问的节点没有"前一个节点"，所以 prev 初始为 `nullptr`，第一次不计算差值。

4. **递归 vs 迭代**：递归更直觉，迭代用栈模拟——两种都要会写，面试可能追问。

## 解法详解

### 解法1: 中序遍历存数组 — O(n) / O(n)

**思考过程**: 最直接的做法——先中序遍历拿到有序数组，然后线性扫描相邻元素的差值。分两步走，逻辑最清晰。

```cpp
class Solution {
public:
    int getMinimumDifference(TreeNode* root) {
        vector<int> vals;
        // 第一步：中序遍历收集所有值（有序）
        inorder(root, vals);
        // 第二步：扫描相邻元素，找最小差值
        int minDiff = INT_MAX;
        for (int i = 1; i < vals.size(); i++) {
            minDiff = min(minDiff, vals[i] - vals[i - 1]);
        }
        return minDiff;
    }

private:
    void inorder(TreeNode* node, vector<int>& vals) {
        if (!node) return;
        inorder(node->left, vals);
        vals.push_back(node->val);
        inorder(node->right, vals);
    }
};
```

**关键点**: 简单易懂，但需要 O(n) 额外空间存数组。面试中这是"能写出来的第一版"，面试官会追问能否优化空间。

### 解法2: 中序递归 + prev 指针 — O(n) / O(h) ⭐ 面试首选

**从解法1优化**: 解法1存了整个数组，但其实我们只需要"当前节点"和"前一个节点"。把数组优化为一个 prev 指针，边遍历边比较。

```cpp
class Solution {
public:
    int getMinimumDifference(TreeNode* root) {
        int minDiff = INT_MAX;
        TreeNode* prev = nullptr;  // 记录中序遍历中的前一个节点
        inorder(root, prev, minDiff);
        return minDiff;
    }

private:
    void inorder(TreeNode* node, TreeNode*& prev, int& minDiff) {
        if (!node) return;

        // 左子树（中序：先左）
        inorder(node->left, prev, minDiff);

        // 处理当前节点（中序：再根）
        if (prev) {
            // prev 非空才计算差值（第一个节点没有前驱）
            minDiff = min(minDiff, node->val - prev->val);
            // 因为中序遍历有序，node->val >= prev->val，所以不用取绝对值
        }
        prev = node;  // 更新 prev 为当前节点

        // 右子树（中序：后右）
        inorder(node->right, prev, minDiff);
    }
};
```

**执行过程可视化**:
```
//       4
//      / \
//     2   6
//    / \
//   1   3
//
// 中序遍历顺序: 1 → 2 → 3 → 4 → 6
//
// Step1: 访问1, prev=null      → 不比较, prev=1
// Step2: 访问2, prev=1         → diff=2-1=1, minDiff=1, prev=2
// Step3: 访问3, prev=2         → diff=3-2=1, minDiff=1, prev=3
// Step4: 访问4, prev=3         → diff=4-3=1, minDiff=1, prev=4
// Step5: 访问6, prev=4         → diff=6-4=2, minDiff=1, prev=6
// 返回 1
```

**关键点**: prev 必须是引用传递（`TreeNode*&`），否则递归回溯时 prev 的更新会丢失。

### 解法3: 中序迭代（栈） — O(n) / O(h)

**迭代模拟递归**: 用显式栈模拟中序遍历的递归过程。面试中展示你对栈与递归关系的理解。

```cpp
class Solution {
public:
    int getMinimumDifference(TreeNode* root) {
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int minDiff = INT_MAX;

        // 中序遍历迭代模板：一路向左入栈，弹出处理，转右子树
        while (curr || !stk.empty()) {
            // 一路向左，把左链全部入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            // 弹出栈顶 = 当前最小的未处理节点
            curr = stk.top();
            stk.pop();

            // 处理当前节点：和 prev 比较
            if (prev) {
                minDiff = min(minDiff, curr->val - prev->val);
            }
            prev = curr;

            // 转向右子树
            curr = curr->right;
        }
        return minDiff;
    }
};
```

**迭代过程可视化**:
```
//       4
//      / \
//     2   6
//    / \
//   1   3
//
// Step1: curr=4, 入栈4→2→1, 栈=[4,2,1]
// Step2: 弹出1, prev=null→不比较, prev=1, curr=null(1无右子)
// Step3: 弹出2, prev=1→diff=1, minDiff=1, prev=2, curr=3
// Step4: 入栈3, 弹出3, prev=2→diff=1, minDiff=1, prev=3, curr=null
// Step5: 弹出4, prev=3→diff=1, minDiff=1, prev=4, curr=6
// Step6: 入栈6, 弹出6, prev=4→diff=2, minDiff=1, prev=6, curr=null
// 栈空且curr=null → 结束, 返回1
```

## 解法对比

| | 解法1: 存数组 | 解法2: 递归+prev | 解法3: 迭代+prev |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) 数组+栈 | O(h) 递归栈 | O(h) 显式栈 |
| 代码量 | 最简单 | 适中 | 稍多 |
| 面试首选 | ✗ 浪费空间 | ✓ 简洁高效 | ✓ 展示功底 |

- **h** 是树高，平衡树 h=O(log n)，最坏（退化链表）h=O(n)
- 解法2 和解法3 逻辑完全一致，只是递归 vs 迭代的区别
- 面试建议：先写解法2，被追问"能否不用递归"时给出解法3

## 易错点

1. **prev 没用引用传递**:
   - ✗ `void inorder(TreeNode* node, TreeNode* prev, int& minDiff)` — prev 的更新在递归返回后丢失
   - ✓ `void inorder(TreeNode* node, TreeNode*& prev, int& minDiff)` — 引用传递确保更新全局可见
   - 也可以用类的成员变量代替引用传递

2. **忘记 BST 中序有序，多此一举取绝对值**:
   - 不算错误，但 `node->val - prev->val` 在中序遍历中一定 ≥ 0，取绝对值是多余操作

3. **第一个节点没有前驱时直接计算差值**:
   - ✗ 不判断 `prev != nullptr` 就访问 `prev->val` → 空指针崩溃
   - ✓ 先检查 `if (prev)` 再计算差值

4. **用 `int prev = -1` 初始化代替指针**:
   - ✗ 如果节点值也可能是 0 或负数（本题 val ≥ 0），用 -1 作哨兵值可能导致第一次比较结果错误
   - ✓ 用 `TreeNode* prev = nullptr` 更安全通用

## 面试追问

**Q1: 为什么不暴力枚举所有节点对？**
> 暴力是 O(n²)，而 BST 的中序有序性让我们只需比较相邻节点，O(n) 就够了。有序序列中最小差值一定在相邻元素间。

**Q2: 如果不是 BST 而是普通二叉树呢？**
> 失去了中序有序的性质。可以先遍历收集所有值到数组，排序后比较相邻元素。时间 O(n log n)，空间 O(n)。

**Q3: 能否用 O(1) 额外空间（不算递归栈）？**
> 解法2 已经只用了一个 prev 指针和一个 minDiff 变量，额外空间 O(1)。递归栈空间 O(h) 无法避免（除非用 Morris 遍历可以做到真正 O(1) 空间）。

**Q4: 说一下 Morris 中序遍历怎么做？**
> Morris 遍历通过临时修改树的结构（线索化），利用叶子节点的空指针回指前驱，实现不用栈的中序遍历。遍历完后树结构恢复。时间 O(n)，空间 O(1)。

## 相关题型

- **783. 二叉搜索树节点最小距离** — 和本题**完全相同**，代码可以直接复用。
- **94. 二叉树的中序遍历** — 复用中序遍历的递归和迭代模板，本题在中序遍历基础上加了 prev 比较逻辑。
- **98. 验证二叉搜索树** — 同样利用"BST 中序有序"的性质，用 prev 指针判断是否严格递增，和本题框架几乎一样，区别：98 是判断 `node->val > prev->val`，本题是算 `node->val - prev->val` 的最小值。
- **99. 恢复二叉搜索树** — 也是中序遍历 + prev 比较，找两个被交换的节点。prev 模式的进阶应用。
- **230. 二叉搜索树中第K小的元素** — 中序遍历 + 计数，遍历到第 K 个就返回。