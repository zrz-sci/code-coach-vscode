# 99. 恢复二叉搜索树

## 核心思路

BST 的中序遍历是有序的。如果恰好两个节点被交换，中序序列中会出现**一处或两处逆序对**，找到这两个被交换的节点，交换回它们的值即可。

## 思维链

1. **读完题第一反应**：BST 中有两个节点的值被交换了，需要找出是哪两个并换回来。最朴素的想法？中序遍历 BST 得到一个"应该有序"的数组，找出哪两个元素位置不对。

2. **暴力做法**：中序遍历 → 得到数组 → 排序后与原数组对比 → 找到不同的两个位置 → 交换对应树节点的值。但这需要 O(n) 额外空间存数组。

3. **核心观察 — 逆序对**：有序数组中交换两个元素，会产生什么？
   - 如果交换的是**相邻元素**（如 `1 3 2 4`），只会出现**1处逆序**：`(3,2)`
   - 如果交换的是**不相邻元素**（如 `1 4 3 2`），会出现**2处逆序**：`(4,3)` 和 `(3,2)`
   - 规律：**第一个错误节点是第一个逆序对的前者，第二个错误节点是最后一个逆序对的后者**。

4. **优化空间到 O(1)**：不需要先存数组再分析，中序遍历时"边走边比"就行——只需记住前一个访问的节点 `prev`，比较 `prev->val` 和 `curr->val`。这样递归/迭代栈的空间是 O(h)，还不是 O(1)。

5. **终极优化 — Morris 遍历**：利用线索化（把空右指针临时指向后继节点），实现 O(1) 空间的中序遍历，同时完成逆序对检测。这是 Follow-up 要求的 O(1) 空间解法。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 中序存数组 | 遍历→数组→找不对的两个 | O(n) | O(n) | 能说出即可 |
| 递归中序+逆序检测 | 递归遍历，边走边比较prev | O(n) | O(h) | ⭐ 必须写出 |
| 迭代中序+逆序检测 | 用栈模拟中序，边走边比较 | O(n) | O(h) | ⭐ 必须写出 |
| Morris中序遍历 | 线索二叉树，O(1)空间遍历 | O(n) | O(1) | 加分项 |

## 关键提示

- **提示1**：BST 中序遍历是严格递增的。交换两个值后，递增序列会出现"逆序"。
- **提示2**：交换相邻 vs 不相邻元素，逆序对数量不同（1个 vs 2个），但找法统一：**first = 第一个逆序对的大值，second = 最后一个逆序对的小值**。
- **提示3**：不需要真的存出数组，只要在中序遍历时维护一个 `prev` 指针，每次比较 `prev` 和当前节点即可。
- **提示4**：Follow-up 要求 O(1) 空间，关键词是 **Morris 遍历**——利用叶节点的空右指针建立临时线索。

```
// 示例2的中序遍历分析:
//
//  原树:         3
//              / \
//             1   4
//                /
//               2
//
//  中序遍历: [1, 3, 2, 4]  ← 应该是 [1, 2, 3, 4]
//                ↑  ↑
//            逆序对(3,2): first=3, second=2
//
//  交换 3 和 2 的值 → 恢复正确
//
// 示例1:
//  原树:    1          中序: [3, 2, 1]  ← 应该是 [1, 2, 3]
//          /                  ↑  ↑  ↑
//         3                逆序1(3,2): first=3
//          \               逆序2(2,1): second=1
//           2
//  交换 3 和 1 的值 → 恢复正确
```

## 解法详解

### 解法1: 中序存数组 — O(n) / O(n)

**思考过程**：最直觉的想法——BST 的中序遍历应该是有序数组。把中序结果存下来，排序后对比，找出不同的两个位置对应的节点，交换值。

```cpp
class Solution {
public:
    void recoverTree(TreeNode* root) {
        // 步骤1: 中序遍历，收集所有节点（按中序顺序）
        vector<TreeNode*> nodes;
        vector<int> vals;
        inorder(root, nodes, vals);
        
        // 步骤2: 排序值数组
        sort(vals.begin(), vals.end());
        
        // 步骤3: 对比找出被交换的两个节点，恢复
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->val != vals[i]) {
                nodes[i]->val = vals[i];
            }
        }
    }
    
private:
    void inorder(TreeNode* root, vector<TreeNode*>& nodes, vector<int>& vals) {
        if (!root) return;
        inorder(root->left, nodes, vals);
        nodes.push_back(root);
        vals.push_back(root->val);
        inorder(root->right, nodes, vals);
    }
};
```

**关键点**：这个解法虽然简单但空间 O(n)，面试中只作为"我理解了题目"的证明，面试官一定会追问优化。

---

### 解法2: 递归中序 + 逆序对检测 — O(n) / O(h) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是需要存整个数组。其实我们不需要存数组，只需要在中序遍历过程中维护一个 `prev` 指针，实时检测逆序对。

**核心逻辑**：
- 维护 `prev`（上一个访问的节点）、`first`（第一个错误节点）、`second`（第二个错误节点）
- 每次发现 `prev->val > curr->val`（逆序）：
  - 如果是**第一次发现逆序**：`first = prev`，`second = curr`
  - 如果是**第二次发现逆序**：只更新 `second = curr`
- 遍历结束后交换 `first` 和 `second` 的值

```
// 为什么 first 和 second 这样赋值？
//
// 情况1: 交换不相邻元素  [1, (6), 3, 4, 5, (2), 7]
//                           ↑  ↑           ↑  ↑
//                     逆序1(6,3)      逆序2(5,2)
//        first = 6 (第一个逆序对的前者)
//        second = 2 (第二个逆序对的后者)
//
// 情况2: 交换相邻元素  [1, 2, (4), (3), 5, 6, 7]
//                              ↑    ↑
//                         只有一个逆序(4,3)
//        first = 4 (逆序对的前者)
//        second = 3 (逆序对的后者)
//
// 统一处理: 第一次逆序时同时设 first=prev, second=curr
//          第二次逆序时只更新 second=curr
//          → 无论哪种情况都正确!
```

```cpp
class Solution {
public:
    void recoverTree(TreeNode* root) {
        TreeNode* first = nullptr;   // 第一个错误节点
        TreeNode* second = nullptr;  // 第二个错误节点
        TreeNode* prev = nullptr;    // 中序遍历的前一个节点
        
        inorder(root, prev, first, second);
        
        // 交换两个错误节点的值
        swap(first->val, second->val);
    }
    
private:
    void inorder(TreeNode* curr, TreeNode*& prev, 
                 TreeNode*& first, TreeNode*& second) {
        if (!curr) return;
        
        inorder(curr->left, prev, first, second);
        
        // 检测逆序: prev 的值应该小于 curr 的值
        if (prev && prev->val > curr->val) {
            if (!first) {
                // 第一次发现逆序
                first = prev;    // first 是逆序对中较大的那个
            }
            // 无论第几次发现逆序，都更新 second
            // 如果只有一次逆序，second = 这次的 curr
            // 如果有两次逆序，second 最终 = 第二次的 curr
            second = curr;
        }
        
        prev = curr;  // 更新 prev
        
        inorder(curr->right, prev, first, second);
    }
};
```

**关键点**：
- `prev`、`first`、`second` 必须用引用传递（`TreeNode*&`），否则递归中的修改不会反映到外层。
- `second = curr` 放在 `if(!first)` 的外面，这样无论是第一次还是第二次逆序都能正确赋值。

---

### 解法3: 迭代中序 + 逆序对检测 — O(n) / O(h)

**思路**：用栈模拟递归中序遍历。逻辑完全一样，只是把递归改成了显式栈。面试时如果面试官要求迭代写法，这是标准答案。

```cpp
class Solution {
public:
    void recoverTree(TreeNode* root) {
        // 迭代中序遍历模板:
        //   curr 从 root 开始
        //   一路向左入栈 → 弹出访问 → 转右子树
        
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;
        
        while (curr || !stk.empty()) {
            // 一路向左，全部入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            
            // 弹出栈顶 = 当前中序节点
            curr = stk.top();
            stk.pop();
            
            // 检测逆序（和递归版完全相同的逻辑）
            if (prev && prev->val > curr->val) {
                if (!first) first = prev;
                second = curr;
            }
            prev = curr;
            
            // 转向右子树
            curr = curr->right;
        }
        
        swap(first->val, second->val);
    }
};
```

```
// 迭代中序遍历过程（示例2: [3,1,4,null,null,2]）:
//
//       3
//      / \
//     1   4
//        /
//       2
//
// Step1: curr=3, 入栈[3], 左走 curr=1
// Step2: curr=1, 入栈[3,1], 左走 curr=null
// Step3: 弹出1, 访问1, prev=null→无比较, prev=1, 右走 curr=null
// Step4: 弹出3, 访问3, prev=1, 1<3 ✓, prev=3, 右走 curr=4
// Step5: curr=4, 入栈[4], 左走 curr=2
// Step6: curr=2, 入栈[4,2], 左走 curr=null
// Step7: 弹出2, 访问2, prev=3, 3>2 ✗ → first=3, second=2, prev=2
// Step8: 弹出4, 访问4, prev=2, 2<4 ✓, prev=4
// 结束: 交换 first(3) 和 second(2) 的值
```

---

### 解法4: Morris 中序遍历 — O(n) / O(1) （Follow-up 最优解）

**从解法2/3优化**：递归和迭代的空间瓶颈是栈空间 O(h)。Morris 遍历利用叶节点的空右指针建立临时"线索"（指向中序后继），实现 O(1) 空间遍历。

**Morris 中序遍历核心思想**：
- 对当前节点 `curr`：
  - 如果没有左子树：访问 curr，移到右子树
  - 如果有左子树：找到左子树的**最右节点**（即 curr 的中序前驱 predecessor）
    - 如果 predecessor 的右指针为空：建立线索（`predecessor->right = curr`），移到左子树
    - 如果 predecessor 的右指针指向 curr：说明左子树已遍历完，断开线索，访问 curr，移到右子树

```
// Morris 遍历示意（示例2）:
//
// 初始:        3              建线索: 1的右指向3
//            / \                    3
//           1   4                 / \
//              /                 1   4
//             2                   \ /
//                                  ↗2
//                               (线索)
//
// 遍历过程:
// curr=3: 左子树存在, 前驱=1, 1->right=null → 建线索 1->right=3, curr=1
// curr=1: 无左子树 → 访问1, curr=1->right=3 (通过线索回来)
// curr=3: 左子树存在, 前驱=1, 1->right=3 → 已有线索,断开, 访问3, curr=4
// curr=4: 左子树存在, 前驱=2, 2->right=null → 建线索 2->right=4, curr=2
// curr=2: 无左子树 → 访问2 (此时prev=3, 3>2 → 发现逆序!), curr=2->right=4
// curr=4: 左子树存在, 前驱=2, 2->right=4 → 已有线索,断开, 访问4, curr=null
// 结束
```

```cpp
class Solution {
public:
    void recoverTree(TreeNode* root) {
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;
        TreeNode* prev = nullptr;
        TreeNode* curr = root;
        
        while (curr) {
            if (!curr->left) {
                // 没有左子树: 直接访问当前节点
                check(prev, curr, first, second);
                prev = curr;
                curr = curr->right;
            } else {
                // 找左子树的最右节点 (中序前驱)
                TreeNode* predecessor = curr->left;
                while (predecessor->right && predecessor->right != curr) {
                    predecessor = predecessor->right;
                }
                
                if (!predecessor->right) {
                    // 前驱的右指针为空: 建立线索, 进入左子树
                    predecessor->right = curr;
                    curr = curr->left;
                } else {
                    // 前驱的右指针指向curr: 左子树已遍历完
                    // 断开线索, 访问当前节点
                    predecessor->right = nullptr;
                    check(prev, curr, first, second);
                    prev = curr;
                    curr = curr->right;
                }
            }
        }
        
        swap(first->val, second->val);
    }
    
private:
    void check(TreeNode* prev, TreeNode* curr, 
               TreeNode*& first, TreeNode*& second) {
        if (prev && prev->val > curr->val) {
            if (!first) first = prev;
            second = curr;
        }
    }
};
```

**关键点**：
- Morris 遍历会临时修改树结构（建线索），但遍历完后会恢复原状。
- "访问节点"的逻辑（检测逆序）和递归/迭代版完全一致，只是遍历框架不同。
- 时间 O(n)（虽然有些节点会被访问两次，但总访问次数仍为 O(n)）。

---

## 解法对比

| | 解法1 存数组 | 解法2 递归 | 解法3 迭代 | 解法4 Morris |
|---|---|---|---|---|
| 时间 | O(n log n) | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(h) | O(h) | O(1) |
| 代码量 | 最简单 | 简洁 | 中等 | 较长 |
| 面试推荐 | 展示理解 | ⭐首选 | 追问时写 | 加分项 |

**关键区别**：
- 解法1→2 的优化：不需要存数组，一边遍历一边检测逆序
- 解法2→3 的转换：递归改迭代，用栈模拟调用栈，逻辑完全一样
- 解法3→4 的优化：栈空间也省掉，用 Morris 线索代替栈

## 易错点

1. **`second` 的赋值位置错误**：
   - ✗ `if (!first) { first = prev; second = curr; } else { second = curr; }` — 虽然正确但啰嗦
   - ✓ `if (!first) first = prev; second = curr;` — `second = curr` 放在 if 外面，每次逆序都更新，简洁且正确
   - 为什么：如果只有一个逆序对（相邻交换），second 在第一次逆序时就赋值了；如果有两个逆序对，second 会被第二次逆序更新为正确值。

2. **递归中参数不用引用**：
   - ✗ `void inorder(TreeNode* curr, TreeNode* prev, TreeNode* first, TreeNode* second)` — 递归中修改不影响外层
   - ✓ `void inorder(TreeNode* curr, TreeNode*& prev, TreeNode*& first, TreeNode*& second)` — 必须用 `TreeNode*&`

3. **Morris 遍历忘记断开线索**：
   - ✗ 建立了 `predecessor->right = curr` 但遍历完忘记设回 `nullptr`
   - ✓ 第二次遇到同一前驱时，执行 `predecessor->right = nullptr` 恢复树结构

4. **只检测了一个逆序对就停止**：
   - ✗ 找到第一个逆序对就 `return`
   - ✓ 必须遍历完整棵树，因为可能有第二个逆序对

## 面试追问

**Q1（基础理解）**：为什么 BST 中交换两个节点的值，中序遍历一定会出现逆序？
> BST 中序遍历是严格递增的。交换两个值相当于在有序数组中交换两个元素，必然打破局部有序性，产生 prev > curr 的逆序。

**Q2（核心优化）**：为什么可能有 1 个或 2 个逆序对？怎么统一处理？
> 交换相邻元素产生 1 个逆序对，交换不相邻元素产生 2 个。统一处理：第一次逆序时记 first=prev, second=curr；之后每次逆序只更新 second=curr。最终 first 和 second 就是被交换的两个节点。

**Q3（Follow-up）**：能否 O(1) 空间？
> Morris 遍历。利用叶节点空右指针建立临时线索指向中序后继，不需要栈。遍历完后恢复树结构。

**Q4（变体）**：如果交换的不是两个节点而是 K 个节点呢？
> 中序遍历得到实际序列和排序后的期望序列，找出所有不匹配的位置。不能只用 O(1) 个指针了，需要 O(k) 空间记录差异位置。

## 相关题型

- **94. 二叉树的中序遍历** — 本题的基础，递归/迭代/Morris 三种中序遍历模板直接复用
- **98. 验证二叉搜索树** — 同样利用"中序遍历有序"性质，区别：98 只需判断是否有逆序，99 需要找到逆序的具体节点
- **501. 二叉搜索树中的众数** — 同样在中序遍历过程中维护 prev 指针做统计
- **Morris 遍历系列**：掌握 Morris 后可以在 O(1) 空间内解决任何需要中序遍历的 BST 问题