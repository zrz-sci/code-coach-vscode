# 430. 扁平化多级双向链表

## 核心思路

这道题本质上是一个**多叉树的深度优先遍历（前序遍历）**：每个节点的 `child` 相当于"第一个孩子"，`next` 相当于"兄弟"。我们需要把这棵"多叉树"按 DFS 前序的顺序串成一条双向链表。

## 思维链

1. **读完题第一反应**：遇到有 `child` 的节点时，需要把子链表"插入"到当前节点和 `next` 节点之间。子链表本身也可能有 child，所以这是一个递归/嵌套的结构。

2. **画图理解插入操作**：
   ```
   原始:  1 ↔ 2 ↔ 3 ↔ 4
                |
                7 ↔ 8
   
   插入后: 1 ↔ 2 ↔ 7 ↔ 8 ↔ 3 ↔ 4
   
   关键操作: 把 child 链表的"头"接到 curr 后面，
            把 child 链表的"尾"接到 curr.next 前面
   ```

3. **递归结构**：child 链表内部也可能有 child，所以需要先递归扁平化 child 链表，再做拼接。这就是 DFS！

4. **两种实现方式**：
   - **递归 DFS**：递归处理 child，找到子链表的尾节点，然后拼接到 next 前面
   - **迭代（用栈）**：遇到有 child 的节点，把 next 压栈保存，先处理 child。child 链走完后，从栈中弹出 next 继续处理

5. **不用额外空间的迭代**：不用栈，直接在遇到 child 时找到子链表末尾，做指针拼接，然后继续往 next 走。因为子链表已经被拼进主链了。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归 DFS | 递归处理 child，返回尾节点用于拼接 | O(n) | O(d) 递归栈，d 为嵌套深度 | ⭐ 必须写出 |
| 迭代 + 栈 | 遇到 child 就把 next 压栈，先走 child | O(n) | O(d) 栈空间 | ⭐ 必须写出 |
| 迭代原地拼接 | 找子链表尾，直接拼接到 next 前 | O(n) | O(1) | 加分项 |

## 关键提示

1. **把结构想象成树**：`next` 是右兄弟，`child` 是第一个孩子。扁平化就是树的前序遍历：先访问自己，再访问 child（左子树），最后访问 next（右兄弟）。

2. **拼接操作的四步**（最容易出错的地方）：
   ```
   // curr 有 child，curr.next 是 nextNode
   // 子链表扁平化后，尾节点是 childTail
   
   Step1: curr.next = child         (curr 连向子链表头)
   Step2: child.prev = curr         (子链表头的 prev 指回 curr)
   Step3: childTail.next = nextNode (子链表尾连向原来的 next)
   Step4: if(nextNode) nextNode.prev = childTail  (别忘了 nextNode 可能是 null!)
   Step5: curr.child = null         (清除 child 指针!)
   ```

3. **递归的返回值设计**：让递归函数返回扁平化后的**尾节点**，这样上层就能方便地做拼接。

4. **栈的方法**：本质上和递归完全一样，只是用显式栈替代了调用栈。遇到 child 和 next 都有的节点，先压 next（后处理），再压 child（先处理）。

5. **别忘了 `prev` 指针**：这是双向链表，每次修改 `next` 的同时必须修改对应的 `prev`。

## 解法详解

### 解法1: 递归 DFS — O(n) / O(d)

**思考过程**：

多级链表的结构天然是递归的——child 链表内部还可能有 child。所以最自然的想法是递归：

- 遍历当前层链表，遇到有 child 的节点，先递归扁平化 child 链表
- 递归返回后，我们拿到了子链表的尾节点，就可以把子链表插入到 curr 和 curr.next 之间
- 继续遍历剩余节点

**关键设计**：递归函数返回扁平化后的尾节点，因为拼接时需要知道子链表的末尾在哪。

```
// 递归过程示意（示例1的部分）:
//
// flatten(1→2→3→4→5→6)
//               |
//               3 有 child → flatten(7→8→9→10)
//                                     |
//                                     8 有 child → flatten(11→12)
//                                                  返回尾节点 12
//                                拼接: 8→11→12→9→10
//                                返回尾节点 10
//               拼接: 3→7→8→11→12→9→10→4→5→6
//               返回尾节点 6
```

```cpp
class Solution {
public:
    Node* flatten(Node* head) {
        flattenDFS(head);
        return head;
    }
    
    // 返回扁平化后的尾节点
    Node* flattenDFS(Node* head) {
        Node* curr = head;
        Node* tail = nullptr;
        
        while (curr) {
            Node* nextNode = curr->next;  // 先保存 next，因为后面可能被覆盖
            
            if (curr->child) {
                // 递归扁平化子链表，拿到子链表的尾节点
                Node* childTail = flattenDFS(curr->child);
                
                // 拼接：curr → child链表头
                curr->next = curr->child;
                curr->child->prev = curr;
                
                // 拼接：child链表尾 → nextNode
                childTail->next = nextNode;
                if (nextNode) nextNode->prev = childTail;
                
                // 清除 child 指针
                curr->child = nullptr;
                
                // tail 更新为 childTail（如果 nextNode 为 null，childTail 就是目前的尾）
                tail = childTail;
            } else {
                tail = curr;  // 没有 child 的普通节点，更新 tail
            }
            
            curr = nextNode;  // 继续处理原来的 next
        }
        
        return tail;
    }
};
```

**关键点**：
- `nextNode` 必须在拼接之前保存，因为 `curr->next` 会被改成 `child`
- 递归返回的是**尾节点**，不是头节点，这是整个设计的核心
- 循环结束后返回 `tail`，它是整条扁平化链表的最后一个节点

---

### 解法2: 迭代 + 栈 — O(n) / O(d)

**从递归到迭代**：递归本质上是系统帮你维护一个调用栈。我们可以自己用栈来模拟：
- 遍历链表，遇到有 child 的节点：
  - 如果有 next，把 next 压栈（先保存，后面再处理）
  - 把 child 接到 curr 后面，清除 child 指针
- 当 curr.next 为 null 时（当前层走到头了），从栈中弹出之前保存的 next，接上去

```cpp
class Solution {
public:
    Node* flatten(Node* head) {
        if (!head) return nullptr;
        
        stack<Node*> stk;
        Node* curr = head;
        
        while (curr) {
            if (curr->child) {
                // 有 next 就先压栈保存
                if (curr->next) {
                    stk.push(curr->next);
                }
                // child 接到 curr 后面
                curr->next = curr->child;
                curr->child->prev = curr;
                curr->child = nullptr;  // 清除 child
            }
            
            // 如果走到当前链的末尾，且栈中有之前保存的 next
            if (!curr->next && !stk.empty()) {
                Node* nextNode = stk.top();
                stk.pop();
                curr->next = nextNode;
                nextNode->prev = curr;
            }
            
            curr = curr->next;
        }
        
        return head;
    }
};
```

**关键点**：栈中保存的是"之前被 child 链表打断的 next 节点"。当一条 child 链走到头，从栈中恢复继续。这和递归的调用栈是同一个意思。

---

### 解法3: 迭代原地拼接（无额外空间）— O(n) / O(1) ⭐ 加分项

**优化思路**：解法2用栈保存被打断的 next。能不能不用栈？

核心观察：当我们遇到 child 时，可以**先找到子链表的尾节点**，然后直接把尾节点连到 next。这样就不需要栈了——因为拼接完后，child 链表已经变成了主链的一部分，继续往 next 走就行。

```
// 原地拼接过程:
//
// 原始:  1 ↔ 2 ↔ 3 ↔ 4 ↔ 5 ↔ 6
//                 |
//                 7 ↔ 8 ↔ 9 ↔ 10
//                     |
//                     11 ↔ 12
//
// Step1: curr=3，有 child
//   找 child 尾: 7→8→9→10，尾=10（注意这里不深入8的child，只走next）
//   拼接: 3→7...10→4，清除3.child
//   结果: 1↔2↔3↔7↔8↔9↔10↔4↔5↔6
//                  |
//                  11↔12
//
// Step2: 继续往next走，curr=7, 8(有child!)
//   找 child 尾: 11→12，尾=12
//   拼接: 8→11→12→9，清除8.child
//   结果: 1↔2↔3↔7↔8↔11↔12↔9↔10↔4↔5↔6
//
// Step3: 继续走到末尾，完成！
```

```cpp
class Solution {
public:
    Node* flatten(Node* head) {
        Node* curr = head;
        
        while (curr) {
            if (curr->child) {
                // 找子链表的尾节点（只沿 next 走，不递归进 child）
                Node* childTail = curr->child;
                while (childTail->next) {
                    childTail = childTail->next;
                }
                
                // 子链表尾 ↔ curr 的 next
                childTail->next = curr->next;
                if (curr->next) {
                    curr->next->prev = childTail;
                }
                
                // curr ↔ 子链表头
                curr->next = curr->child;
                curr->child->prev = curr;
                
                // 清除 child
                curr->child = nullptr;
            }
            
            curr = curr->next;
        }
        
        return head;
    }
};
```

**为什么时间还是 O(n)**：虽然找尾看起来是额外遍历，但每个节点最多被"找尾"访问一次，被"主遍历"访问一次，总共 O(2n) = O(n)。

---

## 解法对比

| | 递归 DFS | 迭代 + 栈 | 迭代原地拼接 |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(d) 递归栈 | O(d) 显式栈 | O(1) |
| 代码复杂度 | 中等 | 简单 | 最简单 |
| 面试推荐 | ✓ | ✓ | ✓✓（追问 O(1) 空间时） |

- 递归和迭代+栈本质相同，只是调用栈 vs 显式栈的区别
- 迭代原地拼接最优雅，面试中如果追问"能否不用额外空间"就用这个
- 三种方法都要能写出来，体现对问题的全面理解

## 易错点

1. **忘记清除 `child` 指针**：
   - ✗ 拼接完后 `curr->child` 仍然指向子链表头
   - ✓ 拼接完后必须 `curr->child = nullptr`
   - 题目明确要求"所有 child 指针设置为 null"

2. **忘记处理 `prev` 指针**：
   - ✗ 只修改了 `next`，没修改对应节点的 `prev`
   - ✓ 每次 `a->next = b` 的同时，必须 `b->prev = a`（前提是 b 不为 null）

3. **`nextNode` 可能为 null**：
   - ✗ `childTail->next = nextNode; nextNode->prev = childTail;` — nextNode 为 null 时崩溃
   - ✓ 必须加 `if (nextNode)` 判断

4. **解法3中找尾时不能深入 child**：
   - ✗ 找尾时遇到有 child 的节点就递归进去
   - ✓ 只沿 `next` 走，不管 `child`。因为后续主循环会处理子链表中的 child

5. **递归中 `nextNode` 保存时机**：
   - ✗ 拼接完后才取 `curr->next`（此时 `curr->next` 已经变成了 child 链表头！）
   - ✓ 在拼接之前就保存 `nextNode = curr->next`

## 面试追问

**Q1: 这道题的结构像什么数据结构？遍历顺序是什么？**

→ 像多叉树。`child` 是第一个孩子，`next` 是右兄弟。扁平化的顺序是**前序遍历**（先自己，再 child 子树，再 next 兄弟）。

**Q2: 递归解法的空间复杂度是多少？最坏情况是什么？**

→ O(d)，d 是嵌套深度。最坏情况：每个节点都有 child 且 child 只有一个节点又有 child，形成一条竖链，d = n，空间 O(n)。

**Q3: 能否用 O(1) 空间完成？**

→ 可以。用迭代原地拼接法：遇到 child 时，找到子链表末尾，直接拼入主链，不需要栈。关键是"找尾"的总代价是 O(n)。

**Q4: 如果不是双向链表而是单向链表呢？有什么区别？**

→ 区别不大。只是拼接时不需要维护 `prev` 指针。三种解法的逻辑框架完全相同，只需删掉所有 `prev` 相关的赋值语句。

## 相关题型

- **114. 二叉树展开为链表** — 几乎同构的问题！二叉树的 left/right 对应这里的 child/next。解法3的"找尾拼接"思路可以直接复用
- **116/117. 填充每个节点的下一个右侧节点指针** — 也是在树结构上做"横向连接"操作
- **206. 反转链表** — 链表指针操作的基础功，先练这题再做 430