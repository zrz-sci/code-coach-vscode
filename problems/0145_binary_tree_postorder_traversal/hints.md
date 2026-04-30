# 145. 二叉树的后序遍历

## 核心思路

后序遍历的顺序是 **左 → 右 → 根**，即先处理完左右子树，最后才处理当前节点。递归天然对应这个顺序，而迭代的难点在于：**你需要在访问完左右子树后才能处理根节点，但栈弹出根节点时你不知道右子树是否已经处理过了**。

## 思维链

1. **读完题第一反应**：后序遍历 = 左→右→根，递归三行代码搞定。
2. **Follow-up 要求迭代**：递归本质是调用栈，能否用显式栈模拟？前序遍历（根→左→右）的迭代很简单，后序能否借用？
3. **关键洞察**：后序是 `左→右→根`，如果我们按 `根→右→左` 的顺序遍历（修改版前序），然后**反转结果**，就得到了后序！
4. **另一条路**：不用反转的"真正"迭代后序 — 用一个 `prev` 指针记录上一个访问的节点，判断右子树是否已经处理完毕。
5. **Morris 遍历**（加分项）：O(1) 空间的后序遍历，利用线索化临时修改树结构。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归 | 左→右→根，直接递归 | O(n) | O(h) | 能说出即可 |
| 迭代（反转前序） | 根→右→左 入栈，结果反转 | O(n) | O(n) | ⭐ 必须写出 |
| 迭代（prev 指针） | 栈 + prev 判断右子树是否已访问 | O(n) | O(h) | ⭐ 必须写出 |
| Morris 后序 | 线索化，O(1) 空间 | O(n) | O(1) | 加分项 |

> h = 树的高度，最坏 O(n)（退化链表），平衡时 O(log n)

## 关键提示

1. **三种遍历的对比**：前序 `根左右`、中序 `左根右`、后序 `左右根`。迭代版本中，前序最简单，中序次之，后序最难 — 因为根节点要**最后**被处理，但它**最先**被遇到。

2. **反转技巧的本质**：
   ```
   后序: 左→右→根
   反转: 根→右→左  ← 这不就是"修改版前序"吗？
   ```
   前序是 `根→左→右`，只要把左右入栈顺序交换，就变成 `根→右→左`。

3. **prev 指针法的关键判断**：
   ```
   //       1
   //      / \
   //     2   3
   // 
   // 当栈顶是1时，什么情况下可以处理1？
   // 情况A: 1没有右孩子 → 直接处理
   // 情况B: 右孩子3刚被处理过(prev==3) → 说明右子树搞定了，处理1
   ```

4. **和 144（前序）、94（中序）的对比**：三道题是一个系列，迭代模板有共性也有区别，值得放在一起理解。

5. **ASCII 树示意 — 示例2的后序遍历过程**：
   ```
   //         1
   //        / \
   //       2   3
   //      / \    \
   //     4   5    8
   //        / \  /
   //       6   7 9
   //
   // 后序: 先走到最深的左子树
   // 4 → 6 → 7 → 5 → 2 (左子树全部完成)
   // → 9 → 8 → 3 (右子树全部完成)
   // → 1 (根最后)
   // 结果: [4, 6, 7, 5, 2, 9, 8, 3, 1]
   ```

## 解法详解

### 解法1: 递归 — O(n) / O(h)

**思考过程**：后序遍历的定义就是递归的 — 先遍历左子树，再遍历右子树，最后访问根。直接按定义写。

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }
    
    void dfs(TreeNode* node, vector<int>& result) {
        if (!node) return;          // 终止条件：空节点
        dfs(node->left, result);    // 递归左子树
        dfs(node->right, result);   // 递归右子树
        result.push_back(node->val); // 处理当前节点（后序 = 最后处理根）
    }
};
```

**递归三要素**：
- **返回值**：void（结果通过引用参数收集）
- **终止条件**：node 为空
- **单层逻辑**：先左、再右、最后自己

**关键点**：递归简洁但面试官一定会追问迭代版本。这只是热身。

---

### 解法2: 迭代（反转前序）— O(n) / O(n) ⭐ 面试首选

**从解法1到解法2**：递归用的是系统调用栈，我们用显式栈模拟。但后序直接模拟比较复杂（根节点要最后处理），所以换个思路：

```
后序 = 左→右→根
反转后序 = 根→右→左
          = 修改版前序（把"左右"改成"右左"）
```

所以：**先按"根→右→左"遍历（前序变体），最后反转结果。**

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) return result;
        
        stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            result.push_back(node->val);  // 先记录根
            
            // 注意：先压左，再压右
            // 这样出栈顺序是：右先出，左后出
            // 遍历顺序就是：根→右→左
            if (node->left)  stk.push(node->left);   // 左先压，后出
            if (node->right) stk.push(node->right);  // 右后压，先出
        }
        
        // 反转 "根→右→左" 变成 "左→右→根" = 后序
        reverse(result.begin(), result.end());
        return result;
    }
};
```

**遍历过程（示例1）**：
```
//     1
//      \
//       2
//      /
//     3
//
// 栈操作:
// push(1)
// pop(1) → result=[1], push左(无), push右(2)
// pop(2) → result=[1,2], push左(3), push右(无)
// pop(3) → result=[1,2,3]
// 
// 反转: [3,2,1] ← 这就是后序！
```

**关键点**：
- 和前序遍历的区别只有两处：(1) 左右入栈顺序交换 (2) 最后反转
- 缺点是需要 O(n) 额外空间存储全部结果后才能反转，不能边遍历边输出

---

### 解法3: 迭代（prev 指针）— O(n) / O(h) ⭐ 进阶必会

**从解法2优化**：解法2 需要反转，不够优雅。能否实现"真正的"后序迭代？

**难点分析**：当栈弹出一个节点时，我们不确定它的右子树是否已经处理完。解决方案：用 `prev` 记录上一个处理过的节点。

**判断逻辑**：对于栈顶节点 `curr`：
- 如果 `curr` 没有右孩子，或者右孩子 == prev（刚处理过），说明右子树已完成 → 可以处理 `curr`
- 否则，先去处理右子树

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) return result;
        
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;  // 记录上一个被加入result的节点
        
        while (curr || !stk.empty()) {
            // 阶段1: 一路向左，把所有左节点压栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            
            curr = stk.top();
            
            // 阶段2: 判断右子树的状态
            if (!curr->right || curr->right == prev) {
                // 右子树不存在 或 右子树已处理完 → 处理当前节点
                result.push_back(curr->val);
                stk.pop();
                prev = curr;    // 标记当前节点已处理
                curr = nullptr; // 不要再向左走了，回到栈继续
            } else {
                // 右子树还没处理 → 转向右子树
                curr = curr->right;
            }
        }
        
        return result;
    }
};
```

**详细过程（示例1: [1,null,2,3]）**：
```
//     1
//      \
//       2
//      /
//     3
//
// Step1: curr=1, 无左 → 栈[1], curr=top=1
//        1有右(2), prev=null → 转右, curr=2
// Step2: curr=2, 有左(3) → 栈[1,2], curr=3
//        curr=3, 无左 → 栈[1,2,3], curr=top=3
//        3无右, → 处理3, result=[3], prev=3, pop
// Step3: curr=null, top=2
//        2有右? 右=3, prev=3, 右==prev → 处理2
//        result=[3,2], prev=2, pop
// Step4: curr=null, top=1
//        1有右? 右=2, prev=2, 右==prev → 处理1
//        result=[3,2,1], prev=1, pop
// 栈空，结束。结果: [3,2,1] ✓
```

**关键点**：
- `curr = nullptr` 这行很关键 — 处理完一个节点后不能让 curr 指向它（否则会重复向左走），必须置空让循环从栈顶继续
- `prev` 指针只需要记录最近一个被处理的节点，因为后序中右孩子一定紧挨着在父节点之前被处理

---

### 解法4: Morris 后序遍历 — O(n) / O(1)（加分项）

**思路**：Morris 遍历利用线索化（把空闲的右指针临时指向后继节点），实现 O(1) 空间遍历。后序的 Morris 比较复杂，需要在断开线索时反转输出右边界。这里给出思路框架：

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        // 创建虚拟根节点，左孩子为 root
        TreeNode dummy(0);
        dummy.left = root;
        TreeNode* curr = &dummy;
        
        while (curr) {
            if (!curr->left) {
                curr = curr->right;
            } else {
                // 找到左子树的最右节点（前驱）
                TreeNode* pred = curr->left;
                while (pred->right && pred->right != curr) {
                    pred = pred->right;
                }
                
                if (!pred->right) {
                    // 建立线索
                    pred->right = curr;
                    curr = curr->left;
                } else {
                    // 断开线索，反向输出从 curr->left 到 pred 的路径
                    addReversePath(curr->left, pred, result);
                    pred->right = nullptr;
                    curr = curr->right;
                }
            }
        }
        return result;
    }
    
private:
    // 反转输出从 from 到 to 的右边界节点值
    void addReversePath(TreeNode* from, TreeNode* to, vector<int>& result) {
        reversePath(from, to);
        TreeNode* node = to;
        while (true) {
            result.push_back(node->val);
            if (node == from) break;
            node = node->right;
        }
        reversePath(to, from); // 恢复树结构
    }
    
    void reversePath(TreeNode* from, TreeNode* to) {
        if (from == to) return;
        TreeNode* prev = from;
        TreeNode* curr = from->right;
        while (prev != to) {
            TreeNode* next = curr->right;
            curr->right = prev;
            prev = curr;
            curr = next;
        }
    }
};
```

**关键点**：Morris 后序是三种 Morris 遍历中最复杂的，面试中一般不要求写出，但能说清楚原理是加分项。

---

## 解法对比

| | 递归 | 迭代（反转前序） | 迭代（prev 指针） | Morris |
|---|---|---|---|---|
| **时间** | O(n) | O(n) | O(n) | O(n) |
| **空间** | O(h) 调用栈 | O(n) 栈+反转 | O(h) 栈 | O(1) |
| **难度** | 最简单 | 简单 | 中等 | 困难 |
| **是否真后序** | ✓ | ✗（先收集后反转） | ✓ | ✓ |
| **面试推荐** | 热身 | 首选（快速写出） | 追问时给出 | 极少考 |

**什么时候选哪个？**
- 快速 AC：递归
- 面试第一轮：反转前序法（最容易写对）
- 面试追问"不用反转"：prev 指针法
- 面试追问"O(1) 空间"：Morris

## 易错点

1. **反转法中左右入栈顺序搞反**：
   - ✗ 先压右再压左 → 出栈顺序是左先右后 → 得到 `根→左→右`（这是前序！）
   - ✓ 先压左再压右 → 出栈顺序是右先左后 → 得到 `根→右→左`，反转后才是后序

2. **prev 指针法中忘记 `curr = nullptr`**：
   - ✗ 处理完节点后不置空 curr → 下一轮循环 `while(curr)` 又往左走，死循环
   - ✓ `prev = curr; curr = nullptr;` — 告诉循环"不要再向左走，回去看栈"

3. **prev 指针法中混淆 `stk.top()` 和 `stk.pop()` 的时机**：
   - ✗ 先 pop 再判断右子树 → pop 后节点丢了，无法再压回去
   - ✓ 先 peek（top），确认可以处理后才 pop

4. **递归中把 `push_back` 放在递归调用之前**：
   - ✗ `result.push_back(node->val)` 放在第一行 → 变成前序
   - ✓ 放在两个递归调用之后 → 后序

## 面试追问

**Q1（基础理解）**：前序、中序、后序三种遍历的区别是什么？什么场景下用后序？
> 区别在于处理根节点的时机。后序常用于：需要先知道子树信息才能处理根的场景，如计算树的高度、判断是否平衡、删除树节点（要先删子节点再删父节点）。

**Q2（迭代实现）**：你的递归版本，能改成迭代吗？为什么后序的迭代比前序难？
> 前序和中序迭代中，根节点在弹出时就可以处理。后序中根节点弹出时不能立即处理（要等右子树完成），所以需要额外机制（反转或 prev 指针）来判断时机。

**Q3（空间优化）**：能否 O(1) 空间完成后序遍历？
> Morris 遍历。利用叶子节点的空闲右指针临时建立线索指回祖先，遍历完后恢复。后序的 Morris 需要在断开线索时反向输出右边界路径。

**Q4（变体）**：如果要求后序遍历的第 K 个节点值呢？
> 不能用反转前序法了（需要遍历完才反转）。必须用 prev 指针法或递归，维护计数器，访问到第 K 个时提前返回。

## 相关题型

- **144. 二叉树的前序遍历** — 同系列。迭代版本：后序的反转法就是在前序基础上交换左右入栈顺序 + 反转结果。
- **94. 二叉树的中序遍历** — 同系列。迭代版本：也是"一路向左压栈"，但弹出时直接处理（不需要判断右子树状态），然后转右。和后序 prev 指针法的区别：中序弹出即处理，后序弹出要等右子树。
- **590. N 叉树的后序遍历** — 复用后序的递归/迭代框架，区别：二叉变成遍历 children 数组。
- **106. 从中序与后序遍历序列构造二叉树** — 后序的最后一个元素是根，利用这个性质分割中序数组递归建树。