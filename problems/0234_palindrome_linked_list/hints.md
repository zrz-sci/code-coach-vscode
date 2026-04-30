# 234. 回文链表

## 核心思路

本质上就是判断一个序列正着读和反着读是否一样。难点在于**单链表只能从前往后遍历，无法从后往前**，所以我们需要想办法获取"反向信息"。

## 思维链

1. **读完题第一反应**：回文判断最简单的方法是用数组——把值全部复制到数组里，然后双指针从两端向中间比较。但这需要 O(n) 额外空间。

2. **暴力解的瓶颈**：额外空间 O(n)。题目 Follow-up 要求 O(1) 空间。

3. **链表的核心困难**：单链表不能从后往前走。如果我们能让后半段链表"倒过来"，就能从中间出发同时向两边比较了。

4. **怎么找到中点？** 快慢指针！快指针走两步，慢指针走一步，快指针到末尾时慢指针刚好在中间。

5. **怎么反转后半段？** 经典的链表反转：三指针迭代翻转。

6. **最终方案**：快慢指针找中点 → 反转后半段 → 逐一比较前后两半 → (可选) 恢复链表。时间 O(n)，空间 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 复制到数组 | 把链表值存入数组，双指针比较 | O(n) | O(n) | 能说出即可 |
| 用栈 | 遍历一遍入栈，再遍历一遍逐个对比栈顶 | O(n) | O(n) | 能说出即可 |
| 递归 | 利用递归回溯时的"逆序"特性 | O(n) | O(n) 栈空间 | 加分项 |
| 快慢指针+反转后半段 | 找中点→反转→比较 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

- **提示1**：回文的定义是"前后对称"。如果能同时从链表头和链表尾向中间走，就能判断。问题是单链表怎么从尾部往回走？

- **提示2**：快慢指针可以找到链表中点。快指针每次走 2 步，慢指针每次走 1 步。

- **提示3**：反转链表是一个经典操作（LeetCode 206）。反转后半段后，你就有两个短链表，从各自的头部开始比较即可。

- **提示4**：注意奇偶长度的区别！

```
偶数长度: 1 → 2 → 2 → 1 → NULL
               ↑slow      ↑fast(NULL)
反转后半段: 1 → 2   1 → 2
            前半     后半(已反转)

奇数长度: 1 → 2 → 3 → 2 → 1 → NULL
                   ↑slow          ↑fast(最后节点)
中间的 3 不需要参与比较
反转后半段: 1 → 2   1 → 2
            前半     后半(已反转, 跳过中间节点3)
```

- **提示5**：递归解法利用了"递归归来时"天然的逆序特性——递归到链表尾部，回溯时一个一个比较，同时用一个外部指针从头部推进。

## 解法详解

### 解法1: 复制到数组 + 双指针 — O(n) / O(n)

**思考过程**：单链表不能随机访问也不能反向遍历，但数组可以。最直觉的方式就是"把链表抄到数组里"，然后问题退化为"判断数组是否回文"——双指针从两端向中间走，逐个比较。

```cpp
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        // 第一步：把链表的值全部复制到数组中
        vector<int> vals;
        while (head) {
            vals.push_back(head->val);
            head = head->next;
        }
        // 第二步：双指针判断数组是否回文
        int left = 0, right = vals.size() - 1;
        while (left < right) {
            if (vals[left] != vals[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};
```

**关键点**：这个解法零思维门槛，面试时如果紧张可以先说这个保底，然后再优化。

---

### 解法2: 用栈（半栈优化）— O(n) / O(n)

**思考过程**：栈的特性是"先进后出"，正好把链表的正向序列变成逆序。我们可以只把前半部分入栈，然后让后半部分逐个和栈顶比较。

```
链表: 1 → 2 → 2 → 1
栈(前半): [1, 2]  ← 栈顶是2
后半从第3个节点开始: 2 → 1

比较过程:
  栈顶 2 == 节点 2 ✓, pop
  栈顶 1 == 节点 1 ✓, pop
  栈空 → 回文!
```

```cpp
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        // 快慢指针找中点
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // 前半部分入栈
        stack<int> stk;
        ListNode* curr = head;
        while (curr != slow) {
            stk.push(curr->val);
            curr = curr->next;
        }
        // 奇数长度时跳过中间节点
        if (fast) slow = slow->next;  // fast非空说明是奇数长度
        
        // 后半部分逐个和栈顶比较
        while (slow) {
            if (stk.top() != slow->val) return false;
            stk.pop();
            slow = slow->next;
        }
        return true;
    }
};
```

**关键点**：奇数长度时 `if (fast)` 这个判断很关键——fast 非空说明链表长度是奇数，中间节点不参与比较。

---

### 解法3: 递归（利用回溯的逆序性）— O(n) / O(n) 栈空间

**思考过程**：递归天然有"先深入到底，再回溯"的特性。当递归到链表尾部开始回溯时，回溯的顺序恰好是链表的逆序。如果我们同时维护一个从头部开始的指针，就能实现"一个从前往后，一个从后往前"的对比。

```
递归过程 (链表: 1 → 2 → 2 → 1):

调用栈展开:
  recurse(1) → recurse(2) → recurse(2) → recurse(1) → recurse(NULL)
  
回溯时:
  front指针: 1           递归当前节点: 1  → 匹配✓, front前进到2
  front指针: 2           递归当前节点: 2  → 匹配✓, front前进到2(第二个)
  front指针: 2(第二个)    递归当前节点: 2  → 匹配✓ (但已经过中点，不影响)
  front指针: 2→1         递归当前节点: 1  → (已经比较过了)
```

```cpp
class Solution {
    ListNode* front;  // 从头部推进的指针
    
    bool recurse(ListNode* curr) {
        if (!curr) return true;
        // 先递归到尾部
        if (!recurse(curr->next)) return false;
        // 回溯时：curr 从尾部往回走，front 从头部往前走
        if (front->val != curr->val) return false;
        front = front->next;
        return true;
    }
    
public:
    bool isPalindrome(ListNode* head) {
        front = head;
        return recurse(head);
    }
};
```

**关键点**：
- 空间复杂度虽然"看起来"没用额外数据结构，但递归调用栈深度是 O(n)，所以空间仍然是 O(n)。
- `front` 必须是成员变量（或引用参数），因为它需要在回溯过程中持续推进。

---

### 解法4: 快慢指针 + 反转后半段 — O(n) / O(1) ⭐ 面试首选

**从前面解法优化**：前面所有解法的空间都是 O(n)，瓶颈在于"需要存储逆序信息"。但如果我们直接在原链表上把后半段反转，就不需要额外空间了！

**三步走**：
1. 快慢指针找中点
2. 反转后半段链表
3. 双指针同时从两头向中间比较

```
完整示例 — 偶数长度 [1,2,2,1]:

原始链表:
  1 → 2 → 2 → 1 → NULL

Step1: 快慢指针找中点
  初始:  slow=1, fast=1
  第1轮: slow=2, fast=2(第二个)
  第2轮: slow=2(第二个), fast=NULL
  → slow 停在后半段开头

  1 → 2 → 2 → 1 → NULL
           ↑slow

Step2: 反转后半段 (从slow开始)
  反转前: 2 → 1 → NULL
  反转后: 1 → 2 → NULL

  前半: 1 → 2 → (断开)
  后半: 1 → 2 → NULL

Step3: 逐一比较
  p1=1, p2=1 → 匹配✓
  p1=2, p2=2 → 匹配✓
  → 回文!


完整示例 — 奇数长度 [1,2,3,2,1]:

原始链表:
  1 → 2 → 3 → 2 → 1 → NULL

Step1: 快慢指针
  初始:  slow=1, fast=1
  第1轮: slow=2, fast=3
  第2轮: slow=3, fast=1(最后节点)
  → slow 停在中间节点

  1 → 2 → 3 → 2 → 1 → NULL
               ↑slow

Step2: 反转从 slow->next 开始的后半段
  反转前: 2 → 1 → NULL
  反转后: 1 → 2 → NULL

  前半: 1 → 2 → (3是中间, 不参与比较)
  后半: 1 → 2 → NULL

Step3: 逐一比较
  p1=1, p2=1 → 匹配✓
  p1=2, p2=2 → 匹配✓
  → 回文!
```

```cpp
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        
        // === Step 1: 快慢指针找中点 ===
        // 循环结束时 slow 指向后半段的起点(偶数)或中间节点(奇数)
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // slow 现在是前半段的最后一个节点
        // 后半段从 slow->next 开始
        
        // === Step 2: 反转后半段 ===
        ListNode* secondHalf = reverseList(slow->next);
        
        // === Step 3: 比较前后两半 ===
        ListNode* p1 = head;
        ListNode* p2 = secondHalf;
        bool result = true;
        while (p2) {  // 以后半段为准（后半段 <= 前半段长度）
            if (p1->val != p2->val) {
                result = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        
        // === Step 4 (可选): 恢复链表 ===
        // 面试中如果面试官要求不修改原链表，需要再反转回去
        slow->next = reverseList(secondHalf);
        
        return result;
    }
    
private:
    // 经典的链表反转 (LeetCode 206)
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* next = curr->next;  // 暂存下一个
            curr->next = prev;            // 反转指向
            prev = curr;                  // prev 前进
            curr = next;                  // curr 前进
        }
        return prev;  // prev 是新的头节点
    }
};
```

**关键点**：
- `while (fast->next && fast->next->next)` 和 `while (fast && fast->next)` 的区别：前者让 slow 停在前半段最后一个节点，后者让 slow 多走一步到后半段第一个。两种写法都行，但要保持一致。
- 比较时以 **后半段 p2** 为终止条件，因为奇数长度时后半段比前半段短一个（中间节点归前半段）。

## 解法对比

| | 数组法 | 栈 | 递归 | 快慢指针+反转 |
|---|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(n) | O(n) 栈 | **O(1)** |
| 修改原链表 | 否 | 否 | 否 | **是** |
| 代码难度 | 最简单 | 简单 | 中等 | 中等 |
| 面试优先级 | 保底 | 过渡 | 展示递归能力 | **首选** |

**选择策略**：
- 面试优先写解法4（O(1) 空间），这是 Follow-up 的标准答案
- 如果面试官说"不能修改原链表"，用解法1或解法3
- 解法3（递归）适合展示你对递归和函数调用栈的深刻理解

## 易错点

1. **快慢指针的循环条件写错导致 slow 位置不对**
   - ✗ `while (fast && fast->next)` 然后直接反转 `slow` 开始的部分 → 奇数长度时 slow 已经到了后半段第一个节点，前半段包含中间节点，对比长度不匹配
   - ✓ 要搞清楚自己用的写法下 slow 停在哪里，以此决定反转的起点

2. **比较时用错终止条件**
   - ✗ `while (p1 && p2)` → 虽然能 work，但如果恢复链表的话，p1 可能走过头
   - ✓ `while (p2)` → 以较短的后半段为准

3. **递归解法中 front 指针用局部变量**
   - ✗ 在 `isPalindrome` 中定义 `ListNode* front = head` 然后传给递归 → front 不会被回溯过程更新
   - ✓ front 必须是成员变量或引用/指针参数

4. **奇数长度忘记跳过中间节点**（栈解法中）
   - ✗ 不判断奇偶直接比较 → 中间节点和栈顶比较不上
   - ✓ `if (fast) slow = slow->next;`

5. **反转链表时忘记暂存 next**
   - ✗ `curr->next = prev; curr = curr->next;` → curr->next 已经被改了，往回走了
   - ✓ 先 `next = curr->next`，再改 `curr->next = prev`，最后 `curr = next`

## 面试追问

**Q1: 最直觉的做法是什么？时间空间各多少？**
→ 复制到数组，双指针比较。O(n) 时间，O(n) 空间。

**Q2: 能否优化到 O(1) 空间？** （Follow-up）
→ 快慢指针找中点 + 反转后半段 + 对比。核心在于：反转操作本身只需要三个指针，不需要额外空间。

**Q3: 这个 O(1) 空间的解法修改了原链表，如果面试官要求不能修改怎么办？**
→ 两种选择：
1. 比较完之后把后半段再反转回去（代码中 Step 4）
2. 如果不允许临时修改，就只能用 O(n) 空间的解法（数组或递归）

**Q4: 如果是双向链表呢？**
→ 更简单了！一个指针从头往后，一个从尾往前，O(n/2) 次比较，O(1) 空间，不需要反转。

**Q5: 如果链表有环呢？**
→ 首先用快慢指针检测环（LeetCode 141），有环则不可能是回文（回文链表必须有尾部）。

## 相关题型

- **206. 反转链表** — 解法4的核心子操作，必须先掌握。反转链表的三指针迭代法是本题的直接前置知识。
- **876. 链表的中间结点** — 解法4的第一步"快慢指针找中点"就是这道题，可以直接复用。
- **143. 重排链表** — 同样需要"找中点 + 反转后半段"，然后交替合并。复用解法4的前两步，区别在第三步是"合并"而非"比较"。
- **9. 回文数** — 数字版的回文判断，思路类似：可以反转后半部分数字来比较。
- **125. 验证回文串** — 字符串版回文，双指针法，比链表简单因为支持随机访问。