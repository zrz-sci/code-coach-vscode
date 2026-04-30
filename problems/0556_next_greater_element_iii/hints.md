# 556. Next Greater Element III (下一个更大元素 III)

## 难度: Medium | 标签: Math, Two Pointers, String

---

## 题目理解

给定一个正整数 `n`，找到由 `n` 的各位数字**重新排列**组成的、**比 n 大的最小整数**。如果不存在，返回 -1。结果必须在 32 位整数范围内，否则也返回 -1。

**关键洞察**: 这道题本质上就是经典的 **Next Permutation (LC 31)** 问题，只不过操作对象从数组变成了整数的各位数字。

---

## 思路推演: 为什么是 Next Permutation?

### 直觉建立

想象你有一个密码锁，每位可以是 0-9。你当前的密码是 `12443`，你想找到"下一个更大的密码"。

- `12443` 的数字集合是 `{1, 2, 3, 4, 4}`
- 这些数字能组成的所有数按大小排列: ..., 12344, 12434, **12443**, **13244**, 13424, ..., 44321
- 答案就是排列序列中紧接在 `12443` 后面的那个: `13244`

**核心问题**: 如何高效地找到"下一个排列"，而不用枚举所有排列?

### 手工模拟: n = 12443

```
数字串:  1  2  4  4  3
索引:    0  1  2  3  4
```

**观察规律**: 从右往左看 `3, 4, 4` 是非递增序列(降序或相等)。这意味着这个后缀已经是它能组成的**最大排列**了 -- 在后缀内部无论怎么调整，都只会变小，不可能变大。

所以关键在于: **打破这个非递增后缀**。

---

## 算法: Next Permutation 四步法

### Step 1: 从右往左找第一个"下降点"

从右往左扫描，找到第一个满足 `digits[i] < digits[i+1]` 的位置 `i`。

```
1  2  4  4  3
      ↑
   i=1 (digits[1]=2 < digits[2]=4)

digits[2..4] = [4,4,3] 是非递增 ✓
```

**如果找不到** (整个数字是降序如 `54321`)，说明已是最大排列，返回 -1。

### Step 2: 从右往左找第一个比 digits[i] 大的位置

在后缀 `[i+1, end]` 中，从右往左找第一个 `digits[j] > digits[i]` 的位置 `j`。

```
digits = [1, 2, 4, 4, 3]
              ↑           ↑
              i=1         j=4 (digits[4]=3 > digits[1]=2)
```

**为什么从右找?** 因为后缀是降序的，从右找到的第一个大于 `digits[i]` 的元素，就是**刚好比 `digits[i]` 大的最小元素**。这保证了交换后增量最小。

### Step 3: 交换 digits[i] 和 digits[j]

```
交换前: 1  2  4  4  3
           ↑        ↑  swap(2, 3)
交换后: 1  3  4  4  2
```

**关键性质**: 交换后，`digits[i+1..end]` 仍然保持非递增!
- 原来 `[4, 4, 3]` → 交换 2 和 3 → `[4, 4, 2]` 仍然非递增

### Step 4: 反转 digits[i+1..end]

将非递增的后缀反转为非递减(升序)，使后缀尽可能小。

```
反转前: 1  3  [4  4  2]   (非递增)
反转后: 1  3  [2  4  4]   (非递减, 最小)
→ 结果: 13244
```

---

## 完整图解: 三个示例

### 示例 1: n = 12443

```
原数字:      1   2   4   4   3
             ↑
Step 1: 从右扫: 3←4(ok) 4←4(ok) 2<4 停!
        i = 1, digits[i] = '2'

Step 2: 在[4,4,3]中从右找第一个 > '2' 的
        j = 4, digits[j] = '3'

Step 3: swap(s[1], s[4])
        "12443" → "13442"

Step 4: reverse s[2..4]
        "13|442" → "13|244"

答案: 13244 ✓ (在32位范围内)
```

### 示例 2: n = 230241

```
数字串:  2  3  0  2  4  1
索引:    0  1  2  3  4  5

Step 1: 从右扫描
  - s[4]=4 > s[5]=1 → 非递增 ok
  - s[3]=2 < s[4]=4 → 找到! i=3

Step 2: 从右找 > s[3]=2 的
  - s[5]=1 <= 2 → 跳过
  - s[4]=4 > 2 → 找到! j=4

Step 3: swap s[3] 和 s[4]
  "230241" → "230421"

Step 4: reverse s[4..5]
  "2304|21" → "2304|12"

答案: 230412 ✓
```

### 示例 3: n = 21 (无解)

```
数字串: 2  1
Step 1: s[0]=2 >= s[1]=1 → 全程非递增, i < 0
答案: -1 (已是最大排列, 不存在更大的重排)
```

---

## 为什么这个算法是正确的?

```
排列字典序:  ... → 12344 → 12434 → 12443 → 13244 → 13424 → ...
                                     ↑          ↑
                                    当前        下一个

算法做的事:
1. 找到当前排列中 "可以增大的最低位" (i=1, 数字2)
2. 用后缀中 "最小的更大数字" 替换它 (3替换2)
3. 让替换后的后缀尽可能小 (反转为升序)
→ 保证增量最小 → 就是下一个排列
```

**严格证明**:
1. **Step 1** 找到的 `i` 确定了"必须改变"的最高位 -- 在 `i` 右边的后缀已是最大排列，无法仅靠后缀内部调整变大
2. **Step 2** 从右找确保选到的 `digits[j]` 是后缀中比 `digits[i]` 大的**最小值**，使第 `i` 位的增量最小
3. **Step 3** 交换后，第 `i` 位变大了(保证结果 > 原数)
4. **Step 4** 反转使后缀变为升序(最小排列)，整体增量最小化

### 为什么 Step 4 不用排序而是反转?

因为交换操作**不会破坏** i+1 到末尾的降序性质:
- 交换前 `[i+1, end)` 是降序
- `digits[j]` 被替换为 `digits[i]`(更小的值)，不影响降序
- 所以反转即可得到升序，无需 O(n log n) 排序

---

## 边界情况与易错点

| 陷阱 | 说明 | 示例 |
|------|------|------|
| 32位溢出 | 结果可能超 INT_MAX | `n=1999999999` → 下一排列 9199999999 超限 |
| 全同数字 | 无下降点 | `n=1111` → 返回 -1 |
| 已是最大排列 | 完全降序 | `n=54321` → 返回 -1 |
| 单位数 | 无法重排 | `n=5` → 返回 -1 |
| `>=` vs `>` | 等号不能漏 | `[4,4,3]` 中 4=4 不算下降 |
| Step 2 方向 | 必须从右往左 | 从左找到的不是最小的大于 `digits[i]` 的元素 |

### 溢出处理 (高频面试考点)

```
n = 1999999999
Step 1-4: "1999999999" → "9199999999"
转为 long: 9199999999 > INT_MAX (2147483647)
答案: -1

n = 2147483476
→ 下一排列 = 2147483647 = INT_MAX, 合法
答案: 2147483647
```

---

## 复杂度分析

| 方面 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | O(d) | d 为数字位数，最多 10 位 (n <= 2^31-1) |
| 空间 | O(d) | 字符串存储数字 |

由于 n <= 2^31 - 1，最多 10 位，实际上是常数时间/空间。

---

## 完整代码

### C++ (推荐解法)

```cpp
class Solution {
public:
    int nextGreaterElement(int n) {
        string s = to_string(n);
        int len = s.size();

        // Step 1: 从右往左找第一个下降位 i
        int i = len - 2;
        while (i >= 0 && s[i] >= s[i + 1]) {
            i--;
        }
        if (i < 0) return -1;  // 全降序, 无更大排列

        // Step 2: 从右往左找第一个比 s[i] 大的位置 j
        int j = len - 1;
        while (s[j] <= s[i]) {
            j--;
        }

        // Step 3: 交换
        swap(s[i], s[j]);

        // Step 4: 反转后缀
        reverse(s.begin() + i + 1, s.end());

        // Step 5: 溢出检查
        long result = stol(s);
        return result > INT_MAX ? -1 : (int)result;
    }
};
```

### C++ 变体: 使用 STL next_permutation (简洁写法)

```cpp
class Solution {
public:
    int nextGreaterElement(int n) {
        string s = to_string(n);
        // STL next_permutation 返回 false 表示已是最大排列
        if (!next_permutation(s.begin(), s.end())) {
            return -1;
        }
        long result = stol(s);
        return result > INT_MAX ? -1 : (int)result;
    }
};
```

> 面试中不建议直接用 STL，因为考的就是手写这个算法。

### Python

```python
class Solution:
    def nextGreaterElement(self, n: int) -> int:
        digits = list(str(n))
        length = len(digits)

        # Step 1: 找下降点
        i = length - 2
        while i >= 0 and digits[i] >= digits[i + 1]:
            i -= 1
        if i < 0:
            return -1

        # Step 2: 找交换目标
        j = length - 1
        while digits[j] <= digits[i]:
            j -= 1

        # Step 3: 交换
        digits[i], digits[j] = digits[j], digits[i]

        # Step 4: 反转后缀
        digits[i + 1:] = reversed(digits[i + 1:])

        # Step 5: 溢出检查
        result = int(''.join(digits))
        return result if result <= 2**31 - 1 else -1
```

### C++ 变体: 纯数组操作 (不用 string/stol)

```cpp
class Solution {
public:
    int nextGreaterElement(int n) {
        // 提取每一位数字
        vector<int> digits;
        int temp = n;
        while (temp > 0) {
            digits.push_back(temp % 10);
            temp /= 10;
        }
        reverse(digits.begin(), digits.end());
        int len = digits.size();

        // Step 1-4 同上
        int i = len - 2;
        while (i >= 0 && digits[i] >= digits[i + 1]) i--;
        if (i < 0) return -1;

        int j = len - 1;
        while (digits[j] <= digits[i]) j--;

        swap(digits[i], digits[j]);
        reverse(digits.begin() + i + 1, digits.end());

        // 重建数字并检查溢出
        long result = 0;
        for (int d : digits) {
            result = result * 10 + d;
            if (result > INT_MAX) return -1;
        }
        return (int)result;
    }
};
```

---

## "Next Greater Element" 系列题对比

| 题号 | 题目 | 核心方法 | 难度 |
|------|------|---------|------|
| 496 | Next Greater Element I | 单调栈 + 哈希表 | Easy |
| 503 | Next Greater Element II | 单调栈 + 循环数组 | Medium |
| **556** | **Next Greater Element III** | **下一个排列** | **Medium** |
| 31  | Next Permutation | 下一个排列(数组版) | Medium |
| 60  | Permutation Sequence | 阶乘数系统 | Hard |

> 虽然 496/503/556 名字相似，但 556 和前两题的方法完全不同! 496/503 用单调栈找数组中下一个更大的元素，556 是排列问题。

---

## 面试沟通建议

1. **立即识别模式**: 看到"同样的数字、更大、最小"就应该想到 Next Permutation
2. **和面试官确认**: 问清楚溢出处理 -- 如果结果超出 int 范围怎么办?
3. **画图演示**: 用 3 位数的小例子(如 132 → 213)在白板上走一遍四步法
4. **提及关联**: 说"这和 LeetCode 31 Next Permutation 本质相同"，展示知识面

---

## 面试追问

**Q1: 如果要找下一个更小的排列? (Previous Permutation)**
- 反过来: 从右找第一个"上升点" (digits[i] > digits[i+1])，从右找第一个 < digits[i] 的交换，然后反转后缀为降序。

**Q2: 如果数字可以有前导零?**
- 这道题不会有前导零(正整数重排，最高位不可能是 0)。

**Q3: 如果要求第 k 个排列?**
- 那就是 LeetCode 60 "Permutation Sequence"，用阶乘数系统解决。

**Q4: 如果 n 可以非常大(超出 long)?**
- 使用字符串操作全程，不转数值。比较时逐位比较字符串。
