# 971. Flip Binary Tree To Match Preorder Traversal

[file://solution.cpp](./solution.cpp)

## 核心思路

给定一棵二叉树和目标前序遍历序列 voyage，通过翻转（交换左右子树）最少的节点，使得前序遍历匹配 voyage。用 DFS 模拟前序遍历，在每个节点检查是否需要翻转：如果左子节点的值不等于 voyage 中下一个期望的值，就翻转当前节点。

## 思维链

1. **读完题第一反应** --> 前序遍历是 根-左-右。翻转一个节点意味着变成 根-右-左。我们需要让某种"可能翻转"后的前序遍历匹配 voyage。

2. **关键观察** --> 在前序遍历中，访问完当前节点后，下一个应该访问的是左子节点。如果左子节点存在但其值 != voyage 的下一个值，说明我们必须翻转当前节点（先访问右子树）。如果翻转后右子节点的值也 != voyage 的下一个值，则 impossible，返回 [-1]。

3. **DFS 设计** --> 维护一个全局索引 idx，表示当前在 voyage 中期望匹配的位置。DFS 过程：
   - 如果当前节点为空，直接返回（base case）
   - 如果当前节点值 != voyage[idx]，说明不可能匹配，标记失败
   - idx++（消费当前节点）
   - 检查左子节点：如果左子节点存在且 left->val != voyage[idx]，则需要翻转（记录当前节点值），先递归右子树再递归左子树
   - 否则正常递归：先左后右

4. **失败检测** --> 如果在任何节点发现 node->val != voyage[idx]，说明无论怎么翻转都无法匹配（翻转只是交换左右，不能改变节点值），标记为不可能。

5. **返回结果** --> 如果没有标记失败，返回收集的翻转节点列表；否则返回 [-1]。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 模拟 | 前序遍历 + 按需翻转 | O(n) | O(n) | 唯一合理解法 ⭐ |
| 迭代前序遍历 | 用栈模拟前序遍历 | O(n) | O(n) | 变体写法 |

## 关键提示

1. **为什么翻转只需要看左子节点？** 前序遍历的顺序是 根-左-右。访问完根后，下一个必须是左子节点（如果存在）。如果左子节点的值不等于 voyage 的下一个期望值，唯一的补救措施就是翻转（让右子节点先被访问）。如果右子节点也不匹配，那就彻底不可能了。

2. **全局索引 idx 的角色**: idx 始终指向 voyage 中"下一个应该被匹配的位置"。每个节点被访问时消费一个 idx。DFS 结束后 idx 应该等于 n（所有位置都匹配完毕）。

3. **翻转 vs. 重排**: 翻转只能交换左右子树，不能移动节点到其他位置。所以如果当前节点的值就不等于 voyage[idx]，无论怎么翻转都救不了。

4. **DFS 遍历过程可视化**:
```
Tree:      1           voyage = [1, 3, 2]
          / \
         2   3

idx=0: node=1, voyage[0]=1 ✓, idx→1
  左子=2, voyage[1]=3, 2≠3 → 需要翻转节点1!
  记录 flipped=[1]
  先递归右子树(3)：
    idx=1: node=3, voyage[1]=3 ✓, idx→2
  再递归左子树(2)：
    idx=2: node=2, voyage[2]=2 ✓, idx→3

结果: [1]
```

5. **不可能的情况示例**:
```
Tree:      1           voyage = [2, 1]
          /
         2

idx=0: node=1, voyage[0]=2, 1≠2 → 不可能!
无论怎么翻转，根节点的值不会改变。

结果: [-1]
```

## 解法详解

### 解法1: DFS 递归 -- O(n) / O(n) ⭐ 面试首选

**思考过程**: 模拟前序遍历，用全局指针 idx 跟踪 voyage 中的位置。在每个节点决定是否翻转。

```cpp
class Solution {
public:
    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        vector<int> flipped;
        int idx = 0;
        bool possible = true;
        
        dfs(root, voyage, idx, flipped, possible);
        
        if (!possible) return {-1};
        return flipped;
    }
    
private:
    void dfs(TreeNode* node, vector<int>& voyage, int& idx,
             vector<int>& flipped, bool& possible) {
        if (!node || !possible) return;
        
        // 当前节点值必须匹配 voyage[idx]
        if (node->val != voyage[idx]) {
            possible = false;
            return;
        }
        idx++; // 消费当前位置
        
        // 如果左子节点存在但不匹配 voyage 的下一个位置 → 需要翻转
        if (node->left && node->left->val != voyage[idx]) {
            flipped.push_back(node->val);
            // 翻转：先递归右子树，再递归左子树
            dfs(node->right, voyage, idx, flipped, possible);
            dfs(node->left, voyage, idx, flipped, possible);
        } else {
            // 正常顺序：先左后右
            dfs(node->left, voyage, idx, flipped, possible);
            dfs(node->right, voyage, idx, flipped, possible);
        }
    }
};
```

**关键点**:
- idx 用引用传递，确保全局推进
- 翻转的判断只需要看左子节点是否匹配
- 翻转的实现就是改变递归顺序（先右后左）

---

### 解法2: 迭代前序遍历（栈） -- O(n) / O(n)

**思路**: 用栈模拟前序遍历，遇到需要翻转的节点时交换入栈顺序。

```cpp
class Solution {
public:
    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        vector<int> flipped;
        stack<TreeNode*> stk;
        stk.push(root);
        int idx = 0;
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            
            if (!node) continue;
            if (node->val != voyage[idx++]) return {-1};
            
            // 前序遍历用栈：先压右再压左（出栈时先左后右）
            // 但如果需要翻转，则先压左再压右（出栈时先右后左）
            if (node->right && node->right->val == voyage[idx]) {
                // 右子节点先匹配 → 需要翻转
                if (node->left) flipped.push_back(node->val);
                stk.push(node->left);  // 后访问
                stk.push(node->right); // 先访问
            } else {
                // 正常顺序
                stk.push(node->right); // 后访问
                stk.push(node->left);  // 先访问
            }
        }
        
        return flipped;
    }
};
```

## 解法对比

| | DFS 递归 | 迭代栈 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(n) 递归栈 | O(n) 显式栈 |
| 代码量 | 中等 | 稍多 |
| 可读性 | 好（递归自然表达前序） | 一般 |
| 面试推荐 | ⭐ 首选 | 面试官要求迭代时 |

**DFS 递归更直观**：前序遍历本身就是递归定义的，递归写法最自然。迭代写法需要处理栈的入栈顺序（先右后左 / 先左后右），容易搞混。

## 易错点

1. **idx 没有用引用传递**
   - x `dfs(node, voyage, idx, ...)` 其中 idx 是值传递 --> 左子树消费的 idx 不会传递给右子树
   - o `void dfs(..., int& idx, ...)` 用引用

2. **翻转判断条件写反**
   - x `if (node->left && node->left->val == voyage[idx])` 相等时翻转 --> 这恰好反了
   - o `if (node->left && node->left->val != voyage[idx])` 不相等时才需要翻转

3. **迭代写法中翻转判断逻辑**
   - 在迭代写法中，判断是否翻转要看右子节点是否匹配下一个 voyage 值（因为如果左子不匹配，那右子必须匹配）
   - 特别注意：只有当节点有两个子节点时，翻转才有意义

4. **忘记处理左子节点为空的情况**
   - 当 left 为空时，不需要检查翻转，直接递归右子树即可
   - `if (node->left && node->left->val != voyage[idx])` 中的 `node->left &&` 就处理了这个情况

5. **possible 标记后继续递归**
   - 一旦发现不可能，应该立即返回，不要继续递归浪费时间
   - `if (!node || !possible) return;` 这个检查很重要

## 面试追问

**Q1: 为什么题目保证最多有一种有效的翻转方案？**
> 因为在前序遍历中，每个节点的子树访问顺序只有两种（先左后右 / 先右后左）。给定 voyage 后，每个节点的选择是确定的：如果左子匹配就不翻转，不匹配就必须翻转。没有"两种翻转方案都合法"的情况。

**Q2: 时间复杂度为什么是 O(n)？**
> 每个节点恰好被访问一次（DFS 中每个节点 enter 一次 exit 一次），idx 严格递增不会回退。总共 n 个节点，O(n)。

**Q3: 如果允许翻转任意子树（不只是交换左右），问题会怎样变化？**
> 如果可以任意重排子树，那只要树中节点的值的集合等于 voyage 的集合就行。但因为二叉树的结构限制（每个节点最多两个子节点），即使允许任意重排，可能性仍然有限。这是一个不同的问题。

**Q4: 能否返回翻转次数最少的方案？**
> 本算法已经保证了最少翻转：只在必须翻转时才翻转（左子不匹配 voyage 时）。贪心策略保证了最优性。

## 相关题型

- **LC 144: Binary Tree Preorder Traversal** -- 基础前序遍历，本题的核心操作就是在前序遍历中做决策
- **LC 114: Flatten Binary Tree to Linked List** -- 同样需要改变二叉树结构来满足特定遍历顺序
- **LC 105: Construct Binary Tree from Preorder and Inorder Traversal** -- 从遍历序列重建树，需要理解前序遍历的结构
- **LC 889: Construct Binary Tree from Preorder and Postorder Traversal** -- 类似地利用遍历序列的性质来确定树的结构
