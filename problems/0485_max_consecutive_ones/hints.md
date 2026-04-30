# 485. Max Consecutive Ones - 提示与详解

## 难度：Easy | 标签：Array

---

## 一、核心思路

本题是一道经典的 **单次遍历计数** 问题。

遍历数组，维护两个变量：
- `count`：当前连续 1 的长度
- `maxCount`：历史最大连续 1 的长度

遇到 1 就 count++，遇到 0 就重置 count = 0，每步更新 maxCount。

**一句话总结**：一次遍历，遇 1 递增计数，遇 0 归零重置，全局取最大。

---

## 二、思维链：从题目到解法

### Step 1：理解题意

```
输入：[1, 1, 0, 1, 1, 1]
       ├──┤  │  ├──────┤
       2个1  0  3个1

最大连续 1 的个数 = 3
```

### Step 2：设计状态变量

我们只需要知道"当前连续 1 有多长"和"历史上最长的连续 1 有多长"。

```
index:    0    1    2    3    4    5
nums:     1    1    0    1    1    1
count:    1    2    0    1    2    3
maxCount: 1    2    2    2    2    3
                                  ↑ 最终答案
```

### Step 3：状态转移

```
如果 nums[i] == 1:
    count = count + 1        // 连续段延长
    maxCount = max(maxCount, count)  // 更新全局最大
    
如果 nums[i] == 0:
    count = 0                // 连续段断裂，重置
```

### Step 4：边界情况

- 全部是 1：`[1,1,1,1]` → maxCount = 4
- 全部是 0：`[0,0,0]` → maxCount = 0（count 从未增长）
- 单元素：`[1]` → 1，`[0]` → 0
- 末尾是 1：`[0,1,1]` → maxCount 在循环内更新，不需要循环后再更新

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 计数法 | 遍历+计数+重置 | O(n) | O(1) | 最优解 |
| 分组法 | 按0分割，求最长段 | O(n) | O(k) | 变体思路 |
| 滑动窗口 | 维护全1窗口 | O(n) | O(1) | 为 follow-up 铺垫 |
| STL 一行流 | transform+max_element | O(n) | O(n) | 炫技 |

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：需要几个变量？</summary>

只需要两个整数变量：`count`（当前连续 1 的长度）和 `maxCount`（历史最长）。不需要数组、不需要哈希表。

</details>

<details>
<summary>提示 2：遇到 0 时该做什么？</summary>

遇到 0 时，当前的连续段已经结束，将 count 重置为 0。不需要显式"保存"之前的 count —— maxCount 已经记录了历史最大值。

</details>

<details>
<summary>提示 3：maxCount 应该在哪里更新？</summary>

在 count++ 之后立即更新 maxCount = max(maxCount, count)。这样可以确保即使数组最后一个元素是 1，maxCount 也是正确的。

另一种写法：每次循环都更新 maxCount（不管是不是 1），逻辑更简洁。

</details>

<details>
<summary>提示 4：全 0 数组怎么办？</summary>

如果数组全是 0，count 始终为 0，maxCount 也始终为 0。返回 0 是正确的（最大连续 1 的个数确实是 0）。

</details>

<details>
<summary>提示 5：这道题和"最大连续子数组和"有什么联系？</summary>

本题是 Kadane's Algorithm 的简化版。Kadane 维护"当前子数组和"，遇到负数可能重置；本题维护"当前连续1长度"，遇到0一定重置。本题更简单是因为条件更明确（不需要决策是否重置）。

</details>

---

## 五、解法详解

### 解法一：计数法（最优解）

```cpp
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxCount = 0;  // 全局最大连续 1 的长度
        int count = 0;     // 当前连续 1 的长度

        for (int num : nums) {
            if (num == 1) {
                count++;
                maxCount = max(maxCount, count);
            } else {
                count = 0;  // 遇到 0，重置
            }
        }

        return maxCount;
    }
};
```

**逐步执行**：

```
输入: [1, 1, 0, 1, 1, 1]

i=0: num=1, count=1, maxCount=max(0,1)=1
i=1: num=1, count=2, maxCount=max(1,2)=2
i=2: num=0, count=0
i=3: num=1, count=1, maxCount=max(2,1)=2
i=4: num=1, count=2, maxCount=max(2,2)=2
i=5: num=1, count=3, maxCount=max(2,3)=3

return 3
```

### 解法二：简化写法（每步都更新 maxCount）

```cpp
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxCount = 0, count = 0;
        for (int num : nums) {
            count = (num == 1) ? count + 1 : 0;
            maxCount = max(maxCount, count);
        }
        return maxCount;
    }
};
```

**分析**：每步都更新 maxCount，即使 count 被重置为 0。这不会影响 maxCount 的正确性（max(x, 0) >= 0），但代码更简洁。

### 解法三：滑动窗口（为 follow-up 铺垫）

```cpp
/**
 * 滑动窗口视角：
 * 维护一个窗口 [left, right]，窗口内全是 1。
 * 当 nums[right] == 0 时，窗口收缩到 right+1。
 *
 * 本题中滑动窗口等价于计数法，但为 LC 487/1004 铺垫：
 * - LC 487: 最多翻转一个 0 -> 窗口内最多一个 0
 * - LC 1004: 最多翻转 k 个 0 -> 窗口内最多 k 个 0
 */
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxLen = 0;
        int left = 0;

        for (int right = 0; right < nums.size(); right++) {
            if (nums[right] == 0) {
                left = right + 1;  // 窗口收缩
            }
            maxLen = max(maxLen, right - left + 1);
        }

        return maxLen;
    }
};
```

### 解法四：分组统计

```cpp
/**
 * 把数组按 0 分割成若干段连续的 1，求最长段的长度
 * 
 * [1,1,0,1,1,1,0,1] → 段: {2, 3, 1} → max = 3
 */
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxLen = 0;
        int i = 0, n = nums.size();

        while (i < n) {
            if (nums[i] == 1) {
                int start = i;
                while (i < n && nums[i] == 1) i++;
                maxLen = max(maxLen, i - start);
            } else {
                i++;
            }
        }

        return maxLen;
    }
};
```

### 解法五：STL 一行流（趣味）

```cpp
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxCount = 0, count = 0;
        for (auto& x : nums)
            maxCount = max(maxCount, count = x ? count + 1 : 0);
        return maxCount;
    }
};
```

---

## 六、可视化：计数法的状态变化

```
数组:     [1,  1,  0,  1,  1,  1]
count:     1   2   0   1   2   3
maxCount:  1   2   2   2   2   3
           ↑   ↑               ↑
          new  new             new max!
          max  max

数组:     [1,  0,  1,  1,  0,  1]
count:     1   0   1   2   0   1
maxCount:  1   1   1   2   2   2
                       ↑
                      最终答案=2
```

---

## 七、解法对比

| 维度 | 计数法 | 滑动窗口 | 分组统计 | STL一行 |
|------|--------|---------|---------|---------|
| 时间复杂度 | O(n) | O(n) | O(n) | O(n) |
| 空间复杂度 | O(1) | O(1) | O(1) | O(1) |
| 代码简洁度 | 简洁 | 中等 | 较长 | 极简 |
| 可扩展性 | 低 | 高（适配follow-up） | 低 | 低 |
| 面试推荐 | ⭐⭐⭐ | ⭐⭐ | ⭐ | ⭐ |

---

## 八、易错点

### Bug 1：maxCount 只在循环结束后更新

```cpp
// 错误：只在遇到 0 时更新 maxCount
for (int num : nums) {
    if (num == 1) {
        count++;
    } else {
        maxCount = max(maxCount, count);  // 只在这里更新
        count = 0;
    }
}
return maxCount;
// 如果数组以 1 结尾 [0,1,1,1]，最后一段 count=3 没有被记录！
```

**修正方案 A**：在循环内 num==1 时就更新 maxCount。
**修正方案 B**：循环结束后再 `maxCount = max(maxCount, count)` 一次。

### Bug 2：count 初始化为 1

```cpp
int count = 1;  // 错！如果第一个元素是 0，count 就不对了
int count = 0;  // 正确
```

### Bug 3：忘记处理空数组

```cpp
// 如果 nums 为空，循环不执行，maxCount=0，正确
// 但题目保证 nums.length >= 1，所以本题不需要特别处理
// 面试中可以提一下边界情况
```

### Bug 4：使用 else if 导致逻辑遗漏

```cpp
// 错误：
if (num == 1) {
    count++;
} else if (num == 0) {  // 题目保证只有0和1，else if 多余但不影响
    count = 0;
}
maxCount = max(maxCount, count);
// 实际上这里没 bug，但用 else 更简洁
```

---

## 九、面试追问（递进链）

### Q1：如果允许最多翻转一个 0 为 1，最长连续 1 是多少？（LC 487）

```cpp
// 滑动窗口，窗口内最多一个 0
int findMaxConsecutiveOnes(vector<int>& nums) {
    int maxLen = 0, zeros = 0, left = 0;
    for (int right = 0; right < nums.size(); right++) {
        if (nums[right] == 0) zeros++;
        while (zeros > 1) {
            if (nums[left] == 0) zeros--;
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
```

### Q2：如果允许最多翻转 k 个 0？（LC 1004）

```cpp
// 滑动窗口，窗口内最多 k 个 0
int longestOnes(vector<int>& nums, int k) {
    int maxLen = 0, zeros = 0, left = 0;
    for (int right = 0; right < nums.size(); right++) {
        if (nums[right] == 0) zeros++;
        while (zeros > k) {
            if (nums[left] == 0) zeros--;
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
```

### Q3：如果数据是流式的（一次只看一个元素），怎么解？

用本题的计数法就是流式处理！只需要 O(1) 空间，处理每个元素只需 O(1) 时间。

### Q4：如何返回最大连续 1 的起始和结束位置？

```cpp
int start = -1, bestStart = -1, bestEnd = -1;
int count = 0, maxCount = 0;
for (int i = 0; i < nums.size(); i++) {
    if (nums[i] == 1) {
        if (count == 0) start = i;  // 记录段的起始位置
        count++;
        if (count > maxCount) {
            maxCount = count;
            bestStart = start;
            bestEnd = i;
        }
    } else {
        count = 0;
    }
}
// bestStart ~ bestEnd 就是最长连续 1 的范围
```

### Q5：这个问题和 Kadane's Algorithm 有什么关系？

本题是 Kadane's Algorithm 的简化版：
- Kadane：维护"当前最大子数组和"，当和变负时重置
- 本题：维护"当前连续1长度"，遇到 0 时重置
- 本题更简单因为重置条件是确定的（遇到 0），不需要决策

### Q6：如果数组很大（10^9 个元素），内存放不下怎么办？

- 流式处理（本题的计数法天然支持）
- 只需要 O(1) 额外空间
- 每次读一个元素，更新 count 和 maxCount
- 不需要将整个数组加载到内存

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 487 | Max Consecutive Ones II | 允许翻转一个 0（滑动窗口） |
| 1004 | Max Consecutive Ones III | 允许翻转 k 个 0（滑动窗口） |
| 53 | Maximum Subarray | Kadane's Algorithm（更通用的框架） |
| 674 | Longest Continuous Increasing Subsequence | 同样的计数模式 |
| 1446 | Consecutive Characters | 连续相同字符最长长度 |
| 121 | Best Time to Buy and Sell Stock | 类似的"遍历+维护状态"模式 |

---

## 十一、总结

```
本题的思维模型：

  遍历数组
     │
     ├── nums[i] == 1?
     │      │
     │     YES → count++, maxCount = max(maxCount, count)
     │      │
     │     NO  → count = 0
     │
  循环结束 → return maxCount

扩展路径：
  485 (基础) → 487 (翻转1个0) → 1004 (翻转k个0)
  计数法     → 滑动窗口         → 滑动窗口+通用模板
```

**记忆要点**：
- 两个变量：count（当前连续长度）、maxCount（历史最大）
- 遇 1 递增，遇 0 归零
- maxCount 在 count 增长时立即更新（避免末尾漏更新）
- 本题是滑动窗口系列的入门铺垫（485 → 487 → 1004）
