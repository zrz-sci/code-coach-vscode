# 287. 寻找重复数

## 核心思路

n+1 个整数放在 [1, n] 的范围内，根据鸽巢原理必有重复。这道题的关键约束是：**不能修改数组 + O(1) 空间**。如果把数组看成一个从索引到值的映射函数 `f(i) = nums[i]`，重复数就意味着有两个不同的索引指向同一个值——这恰好构成了一个带环链表，环的入口就是重复数。

## 思维链

1. **读完题第一反应**：找重复数？排序后相邻比较就行，O(n log n)。但题目说不能修改数组。用哈希表？但题目要求 O(1) 空间。好，两个常规方法都被堵死了。

2. **先放宽约束想暴力**：如果不限空间，哈希表 O(n) 时间 O(n) 空间能搞定。如果能修改数组，原地标记也行。但这两个都不满足约束，还有什么办法？

3. **数值范围的特殊性**：值域是 [1, n]，数组长度是 n+1。对于某个数 mid，如果统计数组中 ≤ mid 的元素个数，正常情况应该恰好是 mid 个。如果超过 mid 个，说明重复数在 [1, mid] 里。这就能二分了！

4. **更进一步——O(n) 能做到吗？** 把 `nums[i]` 看作第 i 个节点指向的下一个节点。因为有重复数，必然有两个不同位置指向同一个节点——这就是链表中的"环"！环的入口就是重复数，用 Floyd 快慢指针即可。

5. **本质映射**：索引 i → nums[i] 形成一条链。从索引 0 出发（0 不在值域 [1,n] 内，所以 0 只能作为起点，不会被指向），沿着这条链走一定能走到环。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希表 | 记录出现过的数 | O(n) | O(n) | 能说出即可 |
| 二分答案 | 对值域二分，统计 ≤mid 个数 | O(n log n) | O(1) | 加分项 |
| 位运算 | 逐位比较 nums 和 [1,n] 的每一位 | O(n log n) | O(1) | 加分项 |
| Floyd 环检测 | 快慢指针找环入口 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **鸽巢原理**：n+1 个数放入 n 个位置，至少一个位置放了两个——这证明了重复数必然存在。

2. **二分的搜索空间不是数组索引，而是值域 [1, n]**：对于猜测值 mid，数一数数组中 ≤ mid 的数有多少个。如果超过 mid 个，重复数一定 ≤ mid。

3. **链表类比**：把数组想象成邻接表，`i → nums[i]` 构成一个函数图。从节点 0 出发：
   ```
   nums = [1, 3, 4, 2, 2]
   
   0 → nums[0]=1 → nums[1]=3 → nums[3]=2 → nums[2]=4 → nums[4]=2 → nums[2]=4 → ...
                                                          ↑_______________|
                                                          环！入口 = 2 = 重复数
   ```

4. **为什么从索引 0 出发？** 因为值域是 [1, n]，没有任何 nums[i] 会等于 0，所以索引 0 不在环上，它一定是链的"头部"，类似于链表的 head。

5. **Floyd 算法分两阶段**：Phase 1 快慢指针相遇（证明有环），Phase 2 一个从起点一个从相遇点同步走（找环入口）。

## 解法详解

### 解法1: 哈希表 — O(n) / O(n)

**思考过程**: 最直觉的方法——遍历数组，记录见过的数，第一次遇到重复的就返回。但不满足 O(1) 空间约束，作为理解题意的起点。

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        unordered_set<int> seen;
        for (int num : nums) {
            if (seen.count(num)) return num;
            seen.insert(num);
        }
        return -1; // 不会到这里
    }
};
```

**关键点**: 简单直接，但空间 O(n)，不满足题目 Follow-up 的 O(1) 空间要求。

---

### 解法2: 二分答案 — O(n log n) / O(1)

**从解法1优化**: 不用额外空间记录，能不能"猜"重复数是谁？值域是 [1, n]，对它二分！

**核心逻辑**: 对于猜测值 mid，统计数组中 ≤ mid 的元素个数 count：
- 如果没有重复，[1, mid] 范围内恰好有 mid 个数，count 应该 ≤ mid
- 如果重复数在 [1, mid] 范围内，count 一定 > mid
- 所以 count > mid → 答案在 [left, mid]，否则 → 答案在 [mid+1, right]

```
// 二分搜索空间收缩示例: nums = [1,3,4,2,2], n=4
//
// Round1: left=1, right=4, mid=2
//   count(≤2) = 3 个 (1,2,2) > mid=2  → 答案在 [1,2]
//
// Round2: left=1, right=2, mid=1
//   count(≤1) = 1 个 (1) ≤ mid=1  → 答案在 [2,2]
//
// Round3: left=2, right=2 → 答案 = 2 ✓
```

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int left = 1, right = nums.size() - 1; // 值域 [1, n]
        while (left < right) {
            int mid = left + (right - left) / 2;
            // 统计数组中 ≤ mid 的元素个数
            int count = 0;
            for (int num : nums) {
                if (num <= mid) count++;
            }
            // count > mid 说明重复数在 [left, mid]
            if (count > mid) {
                right = mid;      // mid 自身可能就是答案，不能跳过
            } else {
                left = mid + 1;   // [left, mid] 没有多余的数，答案在右半
            }
        }
        return left;
    }
};
```

**关键点**:
- 二分的是**值域**而不是数组索引
- 每次二分需要遍历整个数组计数，所以总时间 O(n log n)
- 判断条件是 `count > mid` 而不是 `count >= mid`

---

### 解法3: 位运算 — O(n log n) / O(1)

**思考过程**: 对于二进制的每一位，分别统计 nums 中所有数在该位上为 1 的个数，以及 [1, n] 中所有数在该位上为 1 的个数。如果 nums 多出来了，说明重复数在该位上为 1。逐位还原即可。

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int n = nums.size() - 1;
        int ans = 0;
        // 遍历每一个二进制位
        for (int bit = 0; bit < 32; bit++) {
            int mask = 1 << bit;
            int numsCount = 0, baseCount = 0;
            for (int i = 0; i <= n; i++) {
                if (nums[i] & mask) numsCount++;  // nums 中该位为1的个数
                if (i & mask) baseCount++;          // [1,n] 中该位为1的个数
            }
            // 如果 nums 在该位上的 1 更多，说明重复数该位为 1
            if (numsCount > baseCount) {
                ans |= mask;
            }
        }
        return ans;
    }
};
```

**关键点**: 这个方法不依赖"链表环"的抽象，纯粹从位运算角度解决，适合作为备选思路。但注意它对于"重复数出现多次"的情况也能正确工作，因为多出来的计数全部来自同一个数。

---

### 解法4: Floyd 环检测（快慢指针） — O(n) / O(1) ⭐ 面试首选

**核心洞察**: 把数组看成链表！索引 i 的"下一个节点"是 nums[i]。

```
// nums = [1, 3, 4, 2, 2]，索引 0~4
//
// 构建映射 i → nums[i]:
//   0 → 1
//   1 → 3
//   2 → 4
//   3 → 2
//   4 → 2
//
// 从索引 0 出发的链:
//   0 → 1 → 3 → 2 → 4 → 2 → 4 → 2 → ...
//                    ↑_________↓
//                      环！
//   环的入口 = 2 = 重复数
//
// 为什么入口是重复数？
//   索引 3 和索引 4 都指向 2 (nums[3]=nums[4]=2)
//   → 节点 2 有两个"入边" → 它就是环的入口
```

**算法分两个阶段**:

**Phase 1**: 快慢指针找相遇点（证明有环）
- slow 每次走 1 步：`slow = nums[slow]`
- fast 每次走 2 步：`fast = nums[nums[fast]]`
- 在环内某处相遇

**Phase 2**: 找环入口（= 重复数）
- 一个指针从起点 (0) 出发，另一个从相遇点出发
- 都每次走 1 步，再次相遇的地方就是环入口

```
// Phase 1 详细过程: nums = [1,3,4,2,2]
//   Step  slow  fast
//   0     0     0
//   1     1     3    (slow=nums[0]=1, fast=nums[nums[0]]=nums[1]=3)
//   2     3     2    (slow=nums[1]=3, fast=nums[nums[3]]=nums[2]=4→nums[4]=2)
//   3     2     4    (slow=nums[3]=2, fast=nums[nums[2]]=nums[4]=2→nums[2]=4)
//   4     4     2    (slow=nums[2]=4, fast=nums[nums[4]]=nums[2]=4→nums[4]=2)
//   → 等等，让我重新精确计算:
//
//   slow=0, fast=0
//   slow=nums[0]=1,       fast=nums[nums[0]]=nums[1]=3
//   slow=nums[1]=3,       fast=nums[nums[3]]=nums[2]=4
//   slow=nums[3]=2,       fast=nums[nums[4]]=nums[2]=4
//   slow=nums[2]=4,       fast=nums[nums[4]]=nums[2]=4
//   → slow==fast==4? 不对，再看:
//   slow=nums[2]=4,       fast已经在4，fast=nums[nums[4]]=nums[2]=4
//   → 实际上 slow=4, fast=4, 相遇!
//
// Phase 2: ptr1=0, ptr2=4 (相遇点)
//   ptr1=nums[0]=1, ptr2=nums[4]=2
//   ptr1=nums[1]=3, ptr2=nums[2]=4
//   ptr1=nums[3]=2, ptr2=nums[4]=2
//   → ptr1==ptr2==2, 环入口 = 2 ✓
```

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        // Phase 1: 快慢指针找环内相遇点
        int slow = nums[0];
        int fast = nums[nums[0]];
        while (slow != fast) {
            slow = nums[slow];           // 走 1 步
            fast = nums[nums[fast]];     // 走 2 步
        }
        
        // Phase 2: 从起点和相遇点同步走，找环入口
        int ptr = 0;  // 从起点出发
        while (ptr != slow) {
            ptr = nums[ptr];     // 走 1 步
            slow = nums[slow];   // 走 1 步
        }
        
        return ptr;  // 环入口 = 重复数
    }
};
```

**为什么 Phase 2 能找到环入口？**
设起点到环入口距离为 a，环入口到相遇点距离为 b，相遇点继续走到环入口距离为 c。
- Phase 1 相遇时：slow 走了 a+b 步，fast 走了 a+b+k(b+c) 步（多绕了 k 圈）
- 因为 fast 速度是 slow 的 2 倍：2(a+b) = a+b+k(b+c)，即 a+b = k(b+c)
- 所以 a = k(b+c) - b = (k-1)(b+c) + c
- 这意味着从起点走 a 步 = 从相遇点走 c 步 + 绕若干整圈，两者都到达环入口！

## 解法对比

| | 哈希表 | 二分答案 | 位运算 | Floyd 环检测 |
|---|---|---|---|---|
| 时间 | O(n) | O(n log n) | O(n log n) | O(n) |
| 空间 | O(n) | O(1) | O(1) | O(1) |
| 修改数组 | 否 | 否 | 否 | 否 |
| 思路难度 | 简单 | 中等 | 中等 | 较难 |
| 满足所有约束 | ✗ 空间 | ✓ | ✓ | ✓ 最优 |
| 面试推荐 | 开场 | 中间 | 补充 | 终极 |

**怎么选？**
- 面试开场先说哈希表，展示你理解题意
- 面试官追问 O(1) 空间，给出二分答案
- 面试官追问 O(n) 时间 + O(1) 空间，给出 Floyd

## 易错点

1. **Floyd 起点选错**:
   - ✗ `slow = 0; fast = 0;` 然后用 `do-while` — 容易搞混
   - ✓ `slow = nums[0]; fast = nums[nums[0]];` 或者用 `do-while` 从 0 开始，两种都行但要统一
   - 更安全的写法是用 do-while：
     ```cpp
     int slow = 0, fast = 0;
     do {
         slow = nums[slow];
         fast = nums[nums[fast]];
     } while (slow != fast);
     ```

2. **二分答案的判断写反**:
   - ✗ `if (count > mid) left = mid + 1;` — 方向反了！count 多说明重复数在左半边
   - ✓ `if (count > mid) right = mid;` — 重复数 ≤ mid

3. **二分答案忘记包含 mid 自身**:
   - ✗ `right = mid - 1` — mid 本身可能就是答案，不能排除
   - ✓ `right = mid` — 配合 `while (left < right)` 使用

4. **Phase 2 的起点写成相遇点**:
   - ✗ `ptr = slow;` — 两个都从相遇点出发，永远在环里转圈
   - ✓ `ptr = 0;` — 一个必须从链头（索引0）出发

## 面试追问

**Q1: 如何证明至少存在一个重复数？**
鸽巢原理：n+1 个整数放入 [1, n] 共 n 个"鸽巢"中，至少有一个鸽巢里放了 ≥ 2 个整数。

**Q2: 如果允许 O(n) 空间但不能修改数组，最简单的方法是什么？**
哈希表，O(n) 时间 O(n) 空间。

**Q3: 如果要求 O(1) 空间但时间不限于 O(n)，怎么做？**
二分答案法，O(n log n) 时间 O(1) 空间。对值域 [1,n] 二分，每次统计 ≤ mid 的个数。

**Q4: 能否同时做到 O(n) 时间 + O(1) 空间？**
Floyd 环检测。将 `i → nums[i]` 看成链表，重复数 = 环入口。

**Q5: 如果重复数可能不止一个呢？（变体）**
Floyd 不再适用（多个环入口）。二分答案可以找到其中一个，但要找全部需要其他方法（如位运算变体或放宽空间约束）。

**Q6: 为什么 Floyd 算法从索引 0 出发是安全的？**
因为值域是 [1, n]，没有 nums[i] = 0，所以索引 0 不在任何环上，它只是链的起点。这保证了从 0 出发一定先走过一段"尾巴"再进入环，满足 Floyd 算法的前提。

## 相关题型

- **141. 环形链表** — 直接复用 Floyd 快慢指针的 Phase 1（判断是否有环），本题是其"数组版"变体
- **142. 环形链表 II** — 直接复用 Floyd 的 Phase 1 + Phase 2（找环入口），本题的链表抽象与它完全等价
- **268. 丢失的数字** — 同样是 [0,n] 范围内少了一个数，可用异或/数学求和解决，与本题互为"镜像"
- **442. 数组中重复的数据** — 允许修改数组时，用原地标记法（取负标记）找所有重复数，是本题放宽约束后的变体