# 501. 二叉搜索树中的众数

## 核心思路

BST 的中序遍历是有序的，相同的值一定相邻出现。所以"找众数"本质上就是在一个有序序列中找出现次数最多的元素——只需要边遍历边计数即可。

## 思维链

1. **读完题第一反应**：遍历整棵树，用哈希表统计每个值的出现次数，然后找最大频率。这对任何二叉树都能用，但完全没利用 BST 性质。
2. **BST 有什么特殊的？**：BST 的中序遍历产生有序序列！有序意味着相同的值一定连续出现。
3. **有序序列找众数不需要哈希表**：只要维护一个 `prev` 指针记录上一个访问的节点，当前值和前一个值相同就 `count++`，不同就重置 `count=1`。
4. **怎么收集众数？**：维护一个 `maxCount`，当 `count > maxCount` 时清空结果重新收集，当 `count == maxCount` 时追加到结果中。这样一次遍历就能搞定。
5. **进阶：不用额外空间？**：Morris 遍历可以 O(1) 空间完成中序遍历，把递归栈的开销也省掉。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希表统计 | 遍历全树统计频次 | O(n) | O(n) | 能说出即可 |
| 中序遍历(递归) | 利用BST有序性，边遍历边计数 | O(n) | O(h) | ⭐ 必须写出 |
| 中序遍历(迭代) | 栈模拟中序遍历 | O(n) | O(h) | ⭐ 必须写出 |
| Morris 中序遍历 | O(1)空间的中序遍历 | O(n) | O(1) | 加分项 |

## 关键提示

- **提示1**：BST + 中序遍历 = 有序序列，这是 BST 题最重要的性质。

```
//  示例 BST:          中序遍历结果:
//       1             [1, 2, 2]
//        \            相同值连续出现!
//         2
//        /
//       2
```

- **提示2**：在有序序列 `[1, 2, 2, 3, 3, 3]` 中找众数，你会怎么做？只需要一个计数器和一个"前一个值"就够了，不需要哈希表。

- **提示3**：众数可能有多个（频率并列最高）。怎么一次遍历就收集齐？关键是：`count > maxCount` 时清空结果，`count == maxCount` 时追加。

- **提示4**：Follow-up 要求 O(1) 额外空间。Morris 遍历通过临时修改树结构（利用空闲右指针）来代替栈，遍历结束后树会恢复原样。

## 解法详解

### 解法1: 哈希表统计 — O(n) / O(n)

**思考过程**：不管什么树，最暴力的做法就是遍历所有节点，用 map 记录每个值出现几次，最后找最大频率对应的值。这个方法对普通二叉树也成立，完全没利用 BST 性质。

```cpp
class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        unordered_map<int, int> freq;
        // 任意遍历方式都行，这里用前序
        function<void(TreeNode*)> dfs = [&](TreeNode* node) {
            if (!node) return;
            freq[node->val]++;
            dfs(node->left);
            dfs(node->right);
        };
        dfs(root);
        
        // 找最大频率
        int maxFreq = 0;
        for (auto& [val, cnt] : freq)
            maxFreq = max(maxFreq, cnt);
        
        // 收集所有频率等于最大值的
        vector<int> result;
        for (auto& [val, cnt] : freq)
            if (cnt == maxFreq) result.push_back(val);
        
        return result;
    }
};
```

**关键点**：这个解法虽然正确，但面试官一定会追问"能否利用 BST 性质优化空间？"

### 解法2: 中序遍历递归 — O(n) / O(h) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是 O(n) 的哈希表空间。BST 中序遍历是有序的，相同值连续出现，只需要一个计数器就能统计频次，不需要哈希表。

```
// 核心思想：BST 中序遍历 = 有序数组
//
//       6
//      / \
//     2   8
//    / \ / \
//   0  4 7  9
//    \
//     2
//
// 中序遍历: [0, 2, 2, 4, 6, 7, 8, 9]
//               ^^^^
//            相同值连续 → count=2
//
// 遍历过程模拟:
// val=0: prev=NULL, count=1, maxCount=1, result=[0]
// val=2: prev=0≠2, count=1, maxCount=1, result=[0,2]
// val=2: prev=2==2, count=2, maxCount=2>1, result=[2]  ← 清空重填!
// val=4: prev=2≠4, count=1, maxCount=2
// val=6: prev=4≠6, count=1, maxCount=2
// ...
// 最终 result=[2]
```

```cpp
class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> result;
        TreeNode* prev = nullptr;
        int count = 0, maxCount = 0;
        
        function<void(TreeNode*)> inorder = [&](TreeNode* node) {
            if (!node) return;
            inorder(node->left);
            
            // ---- 处理当前节点（中序位置）----
            if (prev && node->val == prev->val) {
                count++;  // 和前一个值相同，计数+1
            } else {
                count = 1;  // 新值出现，重置计数
            }
            
            if (count > maxCount) {
                maxCount = count;
                result.clear();       // 发现更高频率，清空之前的结果
                result.push_back(node->val);
            } else if (count == maxCount) {
                result.push_back(node->val);  // 频率并列，追加
            }
            
            prev = node;  // 更新 prev
            // ---- 处理结束 ----
            
            inorder(node->right);
        };
        
        inorder(root);
        return result;
    }
};
```

**关键点**：`prev` 初始为 nullptr，第一个节点访问时 `prev` 为空，所以走 `else` 分支，`count` 被设为 1，这是正确的。

### 解法3: 中序遍历迭代（栈） — O(n) / O(h)

**思路**：用栈模拟中序遍历的递归过程。处理逻辑和解法2完全一样，只是遍历方式不同。

```
// 栈模拟中序遍历:
// 1. 一路向左，把路径上的节点全部入栈
// 2. 弹出栈顶（当前最小的未处理节点），处理它
// 3. 转向右子树，重复步骤1
```

```cpp
class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int count = 0, maxCount = 0;
        
        while (curr || !stk.empty()) {
            // 一路向左入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            // 弹出栈顶处理
            curr = stk.top(); stk.pop();
            
            // ---- 和递归版完全相同的处理逻辑 ----
            if (prev && curr->val == prev->val) {
                count++;
            } else {
                count = 1;
            }
            
            if (count > maxCount) {
                maxCount = count;
                result.clear();
                result.push_back(curr->val);
            } else if (count == maxCount) {
                result.push_back(curr->val);
            }
            
            prev = curr;
            // ---- 处理结束 ----
            
            curr = curr->right;  // 转向右子树
        }
        
        return result;
    }
};
```

### 解法4: Morris 中序遍历 — O(n) / O(1)（Follow-up 进阶）

**为什么需要 Morris**：递归和迭代都需要 O(h) 的栈空间。Follow-up 要求不使用额外空间（递归栈不算），Morris 遍历通过利用叶子节点空闲的右指针来记录"回去的路"，实现 O(1) 空间的中序遍历。

```
// Morris 遍历核心思想:
// 对于当前节点 curr:
// - 如果没有左子树: 直接访问 curr，然后走右
// - 如果有左子树: 找到左子树的最右节点(predecessor)
//   - 如果 predecessor->right == NULL: 建线索，predecessor->right = curr，向左走
//   - 如果 predecessor->right == curr: 已建过线索，说明左子树遍历完了
//     删除线索，访问 curr，向右走
//
// 示例: 树 [1, null, 2, 2]
//     1
//      \
//       2       ← curr=1 无左子树，访问1，走右
//      /        ← curr=2 有左子树(2)，predecessor=2
//     2             predecessor->right==NULL → 建线索2->right=2，走左
//                ← curr=2(左子节点)，无左子树，访问2，走右(线索回到2)
//                ← curr=2，有左子树(2)，predecessor=2
//                   predecessor->right==curr → 删线索，访问2，走右(NULL)
//                结束: 访问顺序 [1, 2, 2]
```

```cpp
class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> result;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int count = 0, maxCount = 0;
        
        while (curr) {
            if (!curr->left) {
                // 没有左子树：访问当前节点，向右走
                // ---- 处理逻辑 ----
                update(curr, prev, count, maxCount, result);
                prev = curr;
                // ---- 处理结束 ----
                curr = curr->right;
            } else {
                // 找左子树的最右节点 (predecessor)
                TreeNode* predecessor = curr->left;
                while (predecessor->right && predecessor->right != curr)
                    predecessor = predecessor->right;
                
                if (!predecessor->right) {
                    // 第一次到达：建立线索
                    predecessor->right = curr;
                    curr = curr->left;
                } else {
                    // 第二次到达：删除线索，访问当前节点
                    predecessor->right = nullptr;
                    // ---- 处理逻辑 ----
                    update(curr, prev, count, maxCount, result);
                    prev = curr;
                    // ---- 处理结束 ----
                    curr = curr->right;
                }
            }
        }
        
        return result;
    }
    
private:
    void update(TreeNode* curr, TreeNode* prev, int& count, int& maxCount, vector<int>& result) {
        if (prev && curr->val == prev->val) {
            count++;
        } else {
            count = 1;
        }
        if (count > maxCount) {
            maxCount = count;
            result.clear();
            result.push_back(curr->val);
        } else if (count == maxCount) {
            result.push_back(curr->val);
        }
    }
};
```

**关键点**：Morris 遍历会临时修改树结构，但遍历结束后树完全恢复原样。面试中能讲清 Morris 的原理是很大的加分项。

## 解法对比

| | 哈希表 | 中序递归 | 中序迭代 | Morris |
|---|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(h) | O(h) | O(1) |
| 利用BST性质 | ✗ | ✓ | ✓ | ✓ |
| 代码难度 | 简单 | 简单 | 中等 | 较难 |
| 面试推荐 | 作为暴力解提及 | 首选 | 能写更好 | Follow-up加分 |

**什么时候选哪个**：面试中先说哈希表展示理解，然后主动提出"BST 中序有序可以优化空间"，写递归版本。如果面试官追问 O(1) 空间，再写 Morris。

## 易错点

1. **清空结果的时机搞错**
   - ✗ 只在 `count == maxCount` 时 push，遍历结束后取最大 → 需要两次遍历
   - ✓ `count > maxCount` 时立即 `clear()` + `push_back()`，一次遍历搞定

2. **`prev` 初始化忘记判空**
   - ✗ `if (node->val == prev->val)` → 第一个节点时 `prev` 为空，空指针崩溃
   - ✓ `if (prev && node->val == prev->val)` → 先判空

3. **count 重置的位置**
   - ✗ 在 `count > maxCount` 之后才重置 count → 逻辑错位
   - ✓ 先更新 count（相同+1 / 不同=1），再和 maxCount 比较

4. **Morris 遍历中处理节点的位置**
   - ✗ 在"建立线索"时就访问节点 → 节点会被访问两次
   - ✓ 只在"无左子树"和"删除线索"两个时机访问节点

## 面试追问

**Q1: 如果这不是 BST 而是普通二叉树，怎么找众数？**
→ 只能用哈希表统计，O(n) 时间 + O(n) 空间。BST 的价值就是中序有序，让我们省掉哈希表。

**Q2: 你的解法为什么能一次遍历就找到所有众数？不需要先求 maxCount 再扫一遍吗？**
→ 关键在于 `count > maxCount` 时清空结果。如果后面出现更高频率的值，之前收集的都会被清掉。这保证结束时 result 里只有频率最高的值。

**Q3: Follow-up — 能否 O(1) 额外空间？**
→ Morris 中序遍历。原理是利用叶子节点空闲的右指针建立临时线索（指回父节点），从而不需要栈。遍历结束后线索会被删除，树结构恢复。

**Q4: 如果树非常不平衡（退化成链表），递归版有什么风险？**
→ 栈溢出。树高 h 可达 n，递归深度 O(n)。这时候迭代版或 Morris 版更安全。

## 相关题型

- **98. 验证二叉搜索树** — 同样利用 BST 中序有序性质，中序遍历时检查是否严格递增。复用同样的 `prev` 指针技巧。
- **230. 二叉搜索树中第K小的元素** — 中序遍历，计数到第 K 个停止。复用中序遍历框架。
- **169. 多数元素（数组版）** — 在无序数组中找众数，Boyer-Moore 投票法 O(1) 空间。区别：数组无序不能用相邻比较，需要不同的算法。
- **94. 二叉树的中序遍历** — 本题的基础：中序遍历的递归和迭代实现。