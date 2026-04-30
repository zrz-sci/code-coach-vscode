# LCR 155. 将二叉搜索树转化为排序的双向链表

## 核心思路

将 BST **就地**转为有序的双向循环链表。核心观察：**BST 的中序遍历是有序的**。所以只要在中序遍历的过程中，把每个节点的 `left` 改为指向前驱、`right` 改为指向后继，就能原地串成双向链表。最后把首尾相连形成循环。

关键词：**中序遍历** + **就地指针修改** + **循环链表**

## 思维链

1. **读完题第一反应**：BST 的中序遍历是有序序列。如果能在遍历过程中把节点串起来，就不需要额外空间存节点列表。

2. **"就地转换"怎么理解？** 不创建新节点。把树节点的 `left` 指针当作双向链表的"前驱"(prev)，`right` 指针当作"后继"(next)。

3. **遍历过程中怎么串？** 维护一个 `prev` 指针，指向中序遍历中"上一个访问的节点"。当访问当前节点 `cur` 时：
   - `cur->left = prev`（cur 的前驱是 prev）
   - `prev->right = cur`（prev 的后继是 cur）
   - 然后更新 `prev = cur`

4. **头节点怎么找？** 中序遍历的第一个节点就是头节点（BST 最左的叶子）。当 `prev == nullptr` 时，说明当前是第一个节点，记为 `head`。

5. **循环链表怎么做？** 中序遍历结束后，`prev` 指向最后一个节点（最大值）。把 `head->left = prev`，`prev->right = head`，首尾相连。

6. **空树怎么处理？** `root == nullptr` 时直接返回 `nullptr`。

## 中序遍历 + 双向链表转换过程

```
原始 BST:
        4
       / \
      2   5
     / \
    1   3

中序遍历顺序: 1 -> 2 -> 3 -> 4 -> 5

转换过程 (维护 prev 和 head):

步骤1: 访问节点 1 (BST最左叶子)
  prev == nullptr → head = 1 (记录链表头)
  prev = 1
  
  链表状态: head -> [1]

步骤2: 访问节点 2
  cur->left = prev → 2->left = 1
  prev->right = cur → 1->right = 2
  prev = 2
  
  链表状态: head -> [1] <-> [2]

步骤3: 访问节点 3
  cur->left = prev → 3->left = 2
  prev->right = cur → 2->right = 3
  prev = 3
  
  链表状态: head -> [1] <-> [2] <-> [3]

步骤4: 访问节点 4
  cur->left = prev → 4->left = 3
  prev->right = cur → 3->right = 4
  prev = 4
  
  链表状态: head -> [1] <-> [2] <-> [3] <-> [4]

步骤5: 访问节点 5
  cur->left = prev → 5->left = 4
  prev->right = cur → 4->right = 5
  prev = 5
  
  链表状态: head -> [1] <-> [2] <-> [3] <-> [4] <-> [5]

步骤6: 首尾相连 (形成循环)
  head->left = prev → 1->left = 5
  prev->right = head → 5->right = 1
  
  最终循环双向链表:
  
    +------------------------------------------+
    |                                          |
    +->[1] <-> [2] <-> [3] <-> [4] <-> [5]<---+
       head                             tail
       
  任意节点 X:
    X->left = 前驱节点 (比X小的最大值)
    X->right = 后继节点 (比X大的最小值)
  特殊:
    head->left = tail (最小节点的前驱是最大节点)
    tail->right = head (最大节点的后继是最小节点)
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归中序遍历 | DFS 中序遍历 + 原地修改指针 | O(n) | O(h) 栈深 | ⭐ 必须写出 |
| 迭代中序遍历 | 用显式栈模拟中序遍历 | O(n) | O(h) | 变体考法 |
| Morris 遍历 | 线索化遍历，无需栈 | O(n) | O(1) | 加分项 |

## 关键提示

1. **中序遍历 = 有序序列**：BST 的核心性质。左子树的所有值 < 根 < 右子树的所有值。中序遍历(左-根-右)自然产生递增序列。

2. **prev 指针的初始化**：`prev` 初始为 `nullptr`。当 `prev == nullptr` 时，说明正在访问第一个节点（最小值），此时记录为 `head`。

3. **递归中的指针修改**：在递归函数的"访问根节点"这一步修改指针。不要在"访问左子树"或"访问右子树"时做修改。

4. **head 和 prev 必须是成员变量（或全局变量）**：因为递归函数需要跨调用保持状态。如果用局部变量，每层递归都有自己的副本，无法传递信息。或者用引用参数。

5. **循环链表的意义**：题目要求"双向循环"链表，即首尾相连。这在实际中很有用 -- 可以从任意节点双向遍历整个有序序列。

6. **与原始 BST 的关系**：转换后，原始的树结构被破坏（left/right 不再是父子关系而是前驱后继关系）。这是"就地转换"的代价。

## 解法详解

### 解法1: 递归中序遍历 -- O(n) / O(h) ⭐ 面试首选

**思考过程**：最自然的做法。BST 中序遍历得到有序序列，在遍历过程中修改每个节点的 left 和 right 指针，将它们串成双向链表。

```
递归栈的展开过程 (root = [4,2,5,1,3]):

call dfs(4)
  call dfs(2)                    // 先遍历左子树
    call dfs(1)                  // 再遍历左子树
      call dfs(nullptr) return   // 1 没有左子树
      // 访问节点 1: prev=null → head=1, prev=1
      call dfs(nullptr) return   // 1 没有右子树
    // 访问节点 2: 1.right=2, 2.left=1, prev=2
    call dfs(3)                  // 遍历右子树
      call dfs(nullptr) return
      // 访问节点 3: 2.right=3, 3.left=2, prev=3
      call dfs(nullptr) return
  // 访问节点 4: 3.right=4, 4.left=3, prev=4
  call dfs(5)
    call dfs(nullptr) return
    // 访问节点 5: 4.right=5, 5.left=4, prev=5
    call dfs(nullptr) return

遍历结束: head=1, prev=5
首尾相连: 1.left=5, 5.right=1
```

```cpp
class Solution {
public:
    Node* head = nullptr;  // 链表头节点（最小值）
    Node* prev = nullptr;  // 上一个访问的节点
    
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        
        // 中序遍历
        dfs(root);
        
        // 首尾相连，形成循环链表
        head->left = prev;
        prev->right = head;
        
        return head;
    }
    
    void dfs(Node* cur) {
        if (!cur) return;
        
        // 左子树
        dfs(cur->left);
        
        // 访问当前节点：修改指针
        if (prev) {
            // prev 存在，双向链接
            prev->right = cur;
            cur->left = prev;
        } else {
            // prev 为空，说明 cur 是第一个节点（最小值）
            head = cur;
        }
        prev = cur;
        
        // 右子树
        dfs(cur->right);
    }
};
```

**复杂度分析**：
- 时间 O(n)：每个节点访问一次
- 空间 O(h)：递归栈深度，h 为树高。平衡树 h=O(logn)，链状树 h=O(n)

---

### 解法2: 迭代中序遍历 -- O(n) / O(h)

**思考过程**：用显式栈模拟递归的中序遍历，逻辑完全相同，只是用循环代替递归。

**迭代中序遍历模板**：
1. 不断将当前节点压栈并左移（走到最左）
2. 弹栈，处理节点
3. 转向右子树

```
模拟 [4,2,5,1,3]:

初始: stack=[], cur=4

阶段1 - 一路向左: cur=4→2→1→null
  stack: [4, 2, 1]

弹出 1: 处理节点1, prev=null→head=1, prev=1
  cur=1.right=null → 继续弹栈

弹出 2: 处理节点2, 1↔2, prev=2
  cur=2.right=3

阶段2 - 从3向左: cur=3→null
  stack: [4, 3]

弹出 3: 处理节点3, 2↔3, prev=3
  cur=3.right=null → 继续弹栈

弹出 4: 处理节点4, 3↔4, prev=4
  cur=4.right=5

阶段3 - 从5向左: cur=5→null
  stack: [5]

弹出 5: 处理节点5, 4↔5, prev=5
  cur=5.right=null → 栈空，结束

首尾相连: 1↔5 循环
```

```cpp
class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        
        Node* head = nullptr;
        Node* prev = nullptr;
        
        stack<Node*> stk;
        Node* cur = root;
        
        while (cur || !stk.empty()) {
            // 一路向左，全部压栈
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            
            // 弹出栈顶（中序访问）
            cur = stk.top();
            stk.pop();
            
            // 处理当前节点
            if (prev) {
                prev->right = cur;
                cur->left = prev;
            } else {
                head = cur;
            }
            prev = cur;
            
            // 转向右子树
            cur = cur->right;
        }
        
        // 首尾相连
        head->left = prev;
        prev->right = head;
        
        return head;
    }
};
```

---

### 解法3: Morris 遍历 -- O(n) / O(1)

**思考过程**：Morris 遍历通过"线索化"实现 O(1) 空间的中序遍历。核心思想是利用叶子节点的空指针临时指向后继节点，遍历完后恢复。但本题本身就要修改指针，所以不需要"恢复"。

**Morris 中序遍历规则**：
- 如果左子树为空：访问当前节点，转向右子树
- 如果左子树不空：找到左子树的最右节点（前驱）
  - 如果前驱的 right 为空：建立线索（right 指向当前节点），进入左子树
  - 如果前驱的 right 已指向当前节点：说明左子树遍历完毕，访问当前节点，断开线索，转向右子树

```cpp
class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        
        Node* head = nullptr;
        Node* prev = nullptr;
        Node* cur = root;
        
        while (cur) {
            if (!cur->left) {
                // 左子树为空，直接处理当前节点
                if (prev) {
                    prev->right = cur;
                    cur->left = prev;
                } else {
                    head = cur;
                }
                prev = cur;
                cur = cur->right;
            } else {
                // 找左子树的最右节点（中序前驱）
                Node* predecessor = cur->left;
                while (predecessor->right && predecessor->right != cur) {
                    predecessor = predecessor->right;
                }
                
                if (!predecessor->right) {
                    // 建立线索
                    predecessor->right = cur;
                    cur = cur->left;
                } else {
                    // 左子树遍历完毕，处理当前节点
                    // 注意：这里 predecessor->right == cur
                    // 需要把 cur->left 指向前驱(prev)，而不是原来的左孩子
                    if (prev) {
                        prev->right = cur;
                        cur->left = prev;
                    } else {
                        head = cur;
                    }
                    prev = cur;
                    cur = cur->right;
                }
            }
        }
        
        // 首尾相连
        head->left = prev;
        prev->right = head;
        
        return head;
    }
};
```

**注意**：Morris 遍历的实现较为复杂，面试中一般不要求。但如果面试官追问"能否 O(1) 空间"，知道这个方法是加分项。

---

## 关键图示：BST 到双向链表的映射关系

```
BST 结构:                          双向循环链表:
        4                          
       / \                         1 <-> 2 <-> 3 <-> 4 <-> 5
      2   5                        ^                       |
     / \                           |_______________________|
    1   3                          
                                   
BST 中的 left/right 含义:          链表中的 left/right 含义:
  left  = 左孩子 (值更小)            left  = 前驱 (值更小)
  right = 右孩子 (值更大)            right = 后继 (值更大)

指针变化详情:
  节点1: left: nullptr → 5(tail)   right: nullptr → 2
  节点2: left: 1(子) → 1(前驱)     right: 3(子) → 3(后继)  [恰好不变!]
  节点3: left: nullptr → 2         right: nullptr → 4
  节点4: left: 2(子) → 3           right: 5(子) → 5        [right不变!]
  节点5: left: nullptr → 4         right: nullptr → 1(head)
```

## 举一反三

| 题目 | 关联 | 说明 |
|------|------|------|
| 426. 转换BST为双向链表 | 同题 | 本题与主站 426 题相同 |
| 94. 二叉树中序遍历 | 基础 | 本题的基础操作 |
| 114. 展开为链表 | 类似 | 将二叉树展开为单链表 (前序) |
| 897. 递增顺序搜索树 | 类似 | BST 展开为只有右子树的链表 |
| 剑指 Offer 36 | 同题 | 同题的另一个编号 |

## 易错点总结

1. **忘记处理空树**：`root == nullptr` 时直接返回 `nullptr`，否则后续的首尾相连会崩溃。

2. **忘记首尾相连**：遍历完后 head 和 prev 已经串好了线性链表，但还需要 `head->left = prev`、`prev->right = head` 形成循环。

3. **prev 作为局部变量**：递归时 prev 必须跨层传递。要么用成员变量，要么用引用参数 `Node*& prev`。

4. **混淆 left/right 的含义**：在转换过程中，`left` 变成了"前驱"，`right` 变成了"后继"。写代码时要时刻明确当前是在操作树的指针还是链表的指针。

5. **只有一个节点的情况**：`root` 只有一个节点时，`head = prev = root`，首尾相连后 `root->left = root->right = root`（自环）。

## 面试追问

**Q1: 如果不要求循环链表，只要普通双向链表，代码怎么改？**
> 去掉最后两行首尾相连即可。`head->left` 保持 `nullptr`，最后一个节点的 `right` 保持 `nullptr`。

**Q2: 如果要求不修改原树结构 (non-destructive)，怎么做？**
> 中序遍历收集节点值到数组，然后新建链表节点逐个连接。时间 O(n)，空间 O(n)。

**Q3: 如果输入不是 BST 而是普通二叉树，要求按中序遍历顺序转成双向链表？**
> 代码完全一样，中序遍历对任何二叉树都适用。只是结果不一定有序（BST 保证有序，普通树不保证）。

## 复杂度总结

| 解法 | 时间 | 空间 | 说明 |
|------|------|------|------|
| 递归中序 | O(n) | O(h) | h=树高, 最坏O(n) |
| 迭代中序 | O(n) | O(h) | 显式栈 |
| Morris | O(n) | O(1) | 无需额外空间 |
