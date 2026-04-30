# 2. 两数相加

## 核心思路

这道题本质上就是**模拟竖式加法**——从最低位开始逐位相加、处理进位，而链表恰好是逆序存储（个位在头），所以可以从头到尾直接模拟，无需反转。

## 思维链

1. **读完题第一反应**：两个链表表示两个数，要返回它们的和。能不能把链表转成整数，加完再转回链表？→ 可以，但数字可能有 100 位，远超 `long long` 范围，所以不能用整数直接算。

2. **退一步想**：小学竖式加法是怎么做的？从个位开始，逐位相加，大于等于 10 就进 1。关键是要从**最低位**开始。

3. **和链表结构的契合**：链表是逆序存储的！个位就是头节点，十位是第二个节点...所以我们从两个链表头同时开始遍历，就天然是从最低位开始，完美对应竖式加法。

4. **核心逻辑**：同时遍历两个链表，每步取出当前位的值（如果某个链表已走完则取 0），加上上一步的进位，算出当前位的数字和新的进位，创建新节点存结果。

5. **收尾**：两个链表都走完后，如果还有进位（carry = 1），要再加一个节点。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 迭代模拟 | dummy head + 逐位加 + 进位 | O(max(m,n)) | O(max(m,n)) | ⭐ 必须写出 |
| 递归 | 递归处理每一位，carry 作参数传递 | O(max(m,n)) | O(max(m,n)) 栈 | 加分项 |

## 关键提示

1. **dummy head 技巧**：结果链表的头节点在开始时还不知道值，用一个虚拟头节点 `dummy` 简化"第一个节点的特殊处理"。最后返回 `dummy->next`。

2. **统一处理不等长**：不要分别处理"l1 比 l2 长"和"l2 比 l1 长"，而是在循环中：如果某个链表已经走完，就当这一位是 0。

3. **别忘了最后的进位**：`99 + 1 = 100`，两个链表都走完了但 carry = 1，需要额外创建一个值为 1 的节点。

4. **指针变化图**：
```
l1:  2 → 4 → 3 → NULL       (表示 342)
l2:  5 → 6 → 4 → NULL       (表示 465)

逐位相加过程:
Step1: 2+5+0=7, carry=0  → 创建节点 7
Step2: 4+6+0=10, carry=1 → 创建节点 0
Step3: 3+4+1=8, carry=0  → 创建节点 8
两链表都空, carry=0, 结束

结果: 7 → 0 → 8 → NULL  (表示 807)
```

5. **不等长 + 进位的例子**：
```
l1:  9 → 9 → 9 → 9 → 9 → 9 → 9 → NULL  (9999999)
l2:  9 → 9 → 9 → 9 → NULL                (9999)

Step1: 9+9+0=18, carry=1 → 8
Step2: 9+9+1=19, carry=1 → 9
Step3: 9+9+1=19, carry=1 → 9
Step4: 9+9+1=19, carry=1 → 9
Step5: 9+0+1=10, carry=1 → 0   ← l2 已空，取 0
Step6: 9+0+1=10, carry=1 → 0
Step7: 9+0+1=10, carry=1 → 0
Step8: carry=1           → 1   ← 别忘了！

结果: 8→9→9→9→0→0→0→1  (10009998)
```

## 解法详解

### 解法1: 迭代模拟（dummy head） — O(max(m,n)) / O(max(m,n)) ⭐ 面试首选

**思考过程**：这道题就是模拟竖式加法。链表逆序存储意味着头节点就是最低位，所以从头开始遍历就是从个位开始加——完美匹配。我们用一个 `carry` 变量追踪进位，用 dummy head 简化新链表的构建。

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);        // 虚拟头节点，避免对第一个节点的特殊处理
        ListNode* curr = &dummy;  // curr 指向结果链表的当前尾部
        int carry = 0;
        
        // 只要有一个链表没遍历完，或者还有进位，就继续
        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }
            carry = sum / 10;             // 进位：sum>=10 则 carry=1
            curr->next = new ListNode(sum % 10);  // 当前位：取个位
            curr = curr->next;
        }
        
        return dummy.next;  // dummy 的下一个节点才是真正的头
    }
};
```

**关键点**：
- 循环条件 `while (l1 || l2 || carry)` 把"两个链表不等长"和"最后有进位"统一处理了，非常简洁。
- `sum / 10` 得到进位（0 或 1），`sum % 10` 得到当前位的值。

### 解法2: 递归 — O(max(m,n)) / O(max(m,n))

**思考过程**：链表天然适合递归。每一层递归处理一位的加法，carry 作为参数传递给下一层。基本情况是两个链表都为空且 carry 为 0。

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        return helper(l1, l2, 0);
    }
    
private:
    ListNode* helper(ListNode* l1, ListNode* l2, int carry) {
        // 递归终止：两个链表都空且无进位
        if (!l1 && !l2 && carry == 0) return nullptr;
        
        int sum = carry;
        if (l1) sum += l1->val;
        if (l2) sum += l2->val;
        
        // 创建当前位的节点
        ListNode* node = new ListNode(sum % 10);
        // 递归处理下一位
        node->next = helper(
            l1 ? l1->next : nullptr,
            l2 ? l2->next : nullptr,
            sum / 10
        );
        
        return node;
    }
};
```

**关键点**：
- 递归三要素：**返回值**=当前位的节点指针；**终止条件**=两链表都空且 carry=0；**单层逻辑**=算 sum、建节点、递归下一位。
- 递归深度 = max(m, n) + 1（最多多一层处理最终进位），每层 O(1)，所以时间 O(max(m,n))，栈空间 O(max(m,n))。

## 解法对比

| | 迭代 | 递归 |
|---|---|---|
| 代码风格 | 循环 + dummy head | 函数调用自身 |
| 空间开销 | O(1) 额外（不算结果链表） | O(max(m,n)) 栈空间 |
| 可读性 | 更直观，像模拟竖式 | 更简洁但需要理解递归 |
| 面试推荐 | ⭐ 首选，不容易出错 | 能写出是加分项 |

**选择建议**：面试优先写迭代版。如果面试官追问"能否用递归实现？"，再给递归版。

## 易错点

1. **忘记最后的进位**：
   - ✗ `while (l1 || l2)` — 当两链表都遍历完但 carry=1 时，直接结束了，丢失了最高位。
   - ✓ `while (l1 || l2 || carry)` — carry 不为 0 时继续循环，自动处理最后进位。

2. **不等长链表的空指针访问**：
   - ✗ `sum = l1->val + l2->val + carry` — 当 l1 或 l2 为 NULL 时直接崩溃。
   - ✓ 先判断 `if (l1)` 再取值，空的链表当作 0。

3. **忘记移动指针**：
   - ✗ 取完 `l1->val` 后忘了 `l1 = l1->next`，导致死循环。
   - ✓ 取值和移动放在一起：`sum += l1->val; l1 = l1->next;`

4. **没用 dummy head 导致头节点特殊处理**：
   - ✗ 单独处理第一个节点，然后循环处理后续节点 — 代码冗余且容易出 bug。
   - ✓ 用 `dummy` 统一处理，返回 `dummy.next`。

## 面试追问

**Q1: 如果数字是正序存储（最高位在头）怎么办？**
→ 这就是 [445. 两数相加 II](https://leetcode.cn/problems/add-two-numbers-ii/)。正序存储意味着要从尾部开始加，方法有二：
1. 先反转两个链表，然后用本题解法加完再反转回去。
2. 用栈把两个链表的值分别压入，然后从栈顶（即最低位）开始加。

**Q2: 能否不创建新链表，直接在 l1 或 l2 上原地修改？**
→ 可以。选较长的链表作为结果链表，原地修改节点值。但要注意：如果最后有进位，还是需要创建一个新节点。代码更复杂但省空间。

**Q3: 如果要求不用额外空间（O(1)），且链表不可变呢？**
→ 那只能先计算两个链表的长度，然后从高位开始递归，递归返回时处理进位。本质上是用递归栈隐式实现了"从后往前"的遍历。

## 相关题型

- **[445. 两数相加 II](https://leetcode.cn/problems/add-two-numbers-ii/)** — 数字正序存储，复用本题的逐位加法逻辑，但需要先反转链表或用栈把顺序倒过来。
- **[67. 二进制求和](https://leetcode.cn/problems/add-binary/)** — 同样的逐位加法 + 进位模式，只是从链表变成了字符串，从十进制变成了二进制（carry 阈值从 10 变成 2）。
- **[66. 加一](https://leetcode.cn/problems/plus-one/)** — 简化版：只加 1，但核心的进位处理逻辑完全相同。
- **[989. 数组形式的整数加法](https://leetcode.cn/problems/add-to-array-form-of-integer/)** — 数组 + 整数的加法，同样的逐位模拟。