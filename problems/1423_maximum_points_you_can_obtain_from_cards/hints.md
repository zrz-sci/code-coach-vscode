# 1423. 可获得的最大点数 (Maximum Points You Can Obtain from Cards)

## 难度: Medium | 标签: Array, Prefix Sum, Sliding Window

---

## 题目理解

一排卡牌，每张有点数。每次只能从**头部或尾部**拿一张，共拿 **k** 张。求拿到的点数之和的最大值。

关键约束：只能从两端拿，不能从中间拿。

---

## 等价转换（核心洞察）

```
原始数组:  [1, 2, 3, 4, 5, 6, 1]    k = 3

从两端共拿 k=3 张，等价于中间留下 n-k=4 张连续子数组:

方案1: 拿左3张 [1,2,3]   中间留 [4,5,6,1]   得分 = 6
方案2: 拿左2+右1 [1,2]+[1]   中间留 [3,4,5,6]   得分 = 4
方案3: 拿左1+右2 [1]+[6,1]   中间留 [2,3,4,5]   得分 = 8
方案4: 拿右3张 [5,6,1]   中间留 [1,2,3,4]   得分 = 12  <-- 最大!

等价问题: 找长度为 n-k 的连续子数组，使其和最小
答案 = 总和 - 最小窗口和
```

---

## 滑动窗口图解

```
cardPoints = [1, 2, 3, 4, 5, 6, 1]    k = 3, n = 7
windowSize = n - k = 4
total = 1+2+3+4+5+6+1 = 22

窗口滑动过程:

i=0: [1]                    windowSum = 1
i=1: [1, 2]                 windowSum = 3
i=2: [1, 2, 3]              windowSum = 6
i=3: [1, 2, 3, 4]           windowSum = 10   minWindow = 10
      ^^^^^^^^^^^
      窗口大小=4, 第一次比较

i=4:    [2, 3, 4, 5]        windowSum = 10-1+5 = 14   minWindow = 10
         ^^^^^^^^^^^
         滑出1, 滑入5

i=5:       [3, 4, 5, 6]     windowSum = 14-2+6 = 18   minWindow = 10
            ^^^^^^^^^^^

i=6:          [4, 5, 6, 1]  windowSum = 18-3+1 = 16   minWindow = 10
               ^^^^^^^^^^^

最小窗口和 = 10 (对应 [1,2,3,4])
答案 = 22 - 10 = 12 ✓ (拿走 [5,6,1])
```

---

## 思维链

```
1. 读完题 --> 从两端各拿若干张，总共 k 张，最大化得分
2. 暴力思考 --> 枚举从左拿 i 张、右拿 k-i 张，O(k) 种方案
   每种方案需要 O(k) 计算前缀和... 其实可以预处理到 O(1)
3. 更优雅的思路 --> 等价转换!
   拿走两端 k 张 = 中间留下 n-k 张连续子数组
   max(两端之和) = total - min(中间窗口之和)
4. 滑动窗口 --> 维护长度恰好为 n-k 的窗口，找最小和
   时间 O(n)，空间 O(1)
5. 特判 --> k == n 时窗口大小为 0，直接返回 total
```

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 特点 |
|------|------|------|------|------|
| **滑动窗口（反向）** | total - min(n-k 窗口) | O(n) | O(1) | 推荐，最优雅 |
| 前缀和枚举 | prefix[i] + suffix[k-i] | O(n) | O(n) 或 O(k) | 直观但空间略多 |
| 反向滑窗（直接版） | 窗口在两端"环形"滑动 | O(k) | O(1) | 也可以 |

---

## 解法一: 滑动窗口（反向思维） -- O(n) / O(1) 【推荐】

### 算法步骤

```
Step 1: 计算数组总和 total
Step 2: 特判 k == n，直接返回 total
Step 3: 滑动窗口，维护长度为 windowSize = n-k 的窗口
        - 右端进入: windowSum += cardPoints[i]
        - 左端移出: windowSum -= cardPoints[i - windowSize] (当 i >= windowSize)
        - 窗口填满后开始比较: minWindow = min(minWindow, windowSum)
Step 4: 答案 = total - minWindow
```

### 代码

```cpp
class Solution {
public:
    int maxScore(vector<int>& cardPoints, int k) {
        int n = cardPoints.size(), windowSize = n - k;
        int total = 0;
        for (int x : cardPoints) total += x;
        if (windowSize == 0) return total;

        int windowSum = 0, minWindow = INT_MAX;
        for (int i = 0; i < n; i++) {
            windowSum += cardPoints[i];
            if (i >= windowSize)
                windowSum -= cardPoints[i - windowSize];
            if (i >= windowSize - 1)
                minWindow = min(minWindow, windowSum);
        }
        return total - minWindow;
    }
};
```

---

## 解法二: 前缀和枚举 -- O(n) / O(1)

### 思路

直接枚举从左拿 i 张（0 <= i <= k），从右拿 k-i 张。

```
左拿 i 张的和: leftSum (累加维护)
右拿 k-i 张的和: rightSum (从右侧预计算后递减维护)

初始: leftSum = 0, rightSum = sum(最右 k 张)
每步: leftSum += cardPoints[i], rightSum -= cardPoints[n-k+i]
```

### 窗口移动图解

```
cardPoints = [1, 2, 3, 4, 5, 6, 1]    k = 3

i=0: 左拿0张, 右拿3张 -> 0 + (5+6+1) = 12
     [_, _, _, _, 5, 6, 1]

i=1: 左拿1张, 右拿2张 -> (1) + (6+1) = 8
     [1, _, _, _, _, 6, 1]

i=2: 左拿2张, 右拿1张 -> (1+2) + (1) = 4
     [1, 2, _, _, _, _, 1]

i=3: 左拿3张, 右拿0张 -> (1+2+3) + 0 = 6
     [1, 2, 3, _, _, _, _]

最大 = 12
```

### 代码

```cpp
class Solution {
public:
    int maxScore(vector<int>& cardPoints, int k) {
        int n = cardPoints.size();
        // 初始: 全部从右边拿 k 张
        int rightSum = 0;
        for (int i = n - k; i < n; i++)
            rightSum += cardPoints[i];

        int ans = rightSum, leftSum = 0;
        for (int i = 0; i < k; i++) {
            leftSum += cardPoints[i];
            rightSum -= cardPoints[n - k + i];
            ans = max(ans, leftSum + rightSum);
        }
        return ans;
    }
};
```

---

## 关键提示序列（渐进式）

| # | 提示 | 思路方向 |
|---|------|----------|
| 1 | 从两端拿 k 张后，中间剩下什么？ | 连续的 n-k 张 |
| 2 | 最大化拿走的 = ？ | total - 最小化留下的 |
| 3 | 如何找最小的长度为 n-k 的连续子数组和？ | 定长滑动窗口 |
| 4 | k == n 时窗口大小是多少？需要特判吗？ | windowSize=0，特判返回total |

---

## 易错点

1. **k == n 时窗口大小为 0**
   - 此时 windowSize = 0，不需要滑动窗口
   - 直接返回 total
   - 忘记特判会导致 minWindow 始终为 INT_MAX

2. **窗口起始比较时机**
   - 窗口在 `i >= windowSize - 1` 时才填满，才能开始比较
   - 不是 `i >= windowSize`（差一错误）

3. **前缀和方法中右边界**
   - 右拿 k-i 张时，起始索引是 `n-k+i` 而非 `n-k`
   - 每从左边多拿一张，右边就少拿一张

4. **cardPoints[i] 始终为正**
   - 题目约束 `1 <= cardPoints[i] <= 10^4`，全为正数
   - 所以总和一定为正，minWindow 一定存在

---

## 复杂度分析

### 解法一（滑动窗口）
- **时间: O(n)** -- 一次遍历计算总和，一次遍历滑动窗口
- **空间: O(1)** -- 只用常数变量

### 解法二（前缀和枚举）
- **时间: O(n)** -- 预计算右侧和 O(k)，枚举 O(k)，总 O(n)
- **空间: O(1)** -- 只用常数变量

---

## 举一反三

| 变形 | 改动 |
|------|------|
| 只能从左端拿 | 直接求前 k 个元素的和 |
| 可以从任意位置拿 | 排序取最大 k 个 |
| 拿 k 张最小化得分 | 滑动窗口找最大窗口和 |
| 环形数组拿 k 张 | 等价于线性数组留 n-k 张连续 |

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 209 | Minimum Size Subarray Sum | 滑动窗口基础 |
| 239 | Sliding Window Maximum | 定长滑动窗口 + 极值维护 |
| 1658 | Minimum Operations to Reduce X to Zero | 同样的等价转换思路 |
| 918 | Maximum Sum Circular Subarray | 环形 + 反向思维 |

---

## 面试追问

1. **如果不是从两端取，而是可以从任意位置取 k 张（不要求连续），最大和？**
   - 排序后取最大的 k 个即可，O(n log n)
   - 或用 `nth_element` 做到 O(n) 平均

2. **如果卡牌排成环形（首尾相连），从任意位置取连续 k 张，最大和？**
   - 即 LeetCode 918 的变体: 环形子数组最大和
   - 同样用反向思维: `max(连续k张) = total - min(连续n-k张)`
   - 环形场景下把数组复制一份拼接，或者分两种情况讨论

3. **如果每张卡牌有"正面/反面"两个值，取出后可以选择翻面，怎么最大化？**
   - 先用滑窗确定取哪些位置的卡牌
   - 取出后每张独立选 `max(front, back)`
   - 但"取哪些位置"的决策会变复杂，可能需要 DP

---

## 面试技巧

1. **先说直觉**：从两端拿，每次有两个选择 -> 似乎是 DP 或贪心？
2. **再说转换**：等价转换是面试加分点 -- "拿两端 k 张 = 留中间 n-k 张"
3. **画图说明**：在白板上画出窗口滑动过程，非常清晰
4. **提到两种解法**：展示思维广度，滑窗和前缀和各有优势
