# 551. Student Attendance Record I

[LeetCode 链接](https://leetcode.com/problems/student-attendance-record-i/)

## 核心思路

**一次遍历 + 双计数器**: 遍历字符串, 同时维护 `absentCount`(累计缺勤次数) 和 `lateStreak`(当前连续迟到次数)。一旦缺勤 >= 2 或连续迟到 >= 3, 立即返回 false。

本质上是一个简单的条件检查: 两个独立的约束条件, 一个是全局累计计数, 一个是局部连续计数。遇到非 'L' 字符时重置 lateStreak, 遇到 'A' 时累加 absentCount。

---

## 思维链

```
输入: 字符串 s, 由 'A'(缺勤) / 'L'(迟到) / 'P'(到场) 组成

Step 1  初始化: absentCount = 0, lateStreak = 0

Step 2  遍历 s 的每个字符 c:
        ┌─ c == 'A':
        │    absentCount++ → 若 >= 2, return false
        │    lateStreak = 0 (非 L, 重置连续迟到)
        ├─ c == 'L':
        │    lateStreak++ → 若 >= 3, return false
        └─ c == 'P':
             lateStreak = 0 (非 L, 重置连续迟到)

Step 3  遍历完成, 两个条件都未违反 → return true

时间: O(n) — 一次遍历
空间: O(1) — 两个计数器
```

**手工模拟**: s = "PPALLP"
```
P → absent=0, streak=0
P → absent=0, streak=0
A → absent=1(<2), streak=0
L → absent=1, streak=1
L → absent=1, streak=2(<3)
P → absent=1, streak=0
→ return true ✓
```

**手工模拟**: s = "PPALLL"
```
P → absent=0, streak=0
P → absent=0, streak=0
A → absent=1, streak=0
L → absent=1, streak=1
L → absent=1, streak=2
L → absent=1, streak=3 >= 3 → return false ✓
```

---

## 解法概览

| 解法 | 时间 | 空间 | 推荐 | 说明 |
|------|------|------|------|------|
| 一次遍历 (双计数器) | O(n) | O(1) | ⭐ | 最直观高效, 面试首选 |
| STL count + find | O(n) | O(1) | | 简洁但遍历两次 |
| 正则表达式 | O(n) | O(1) | | 一行代码, 但面试不推荐 |

---

## 关键提示

1. **lateStreak 重置时机**: 遇到 `'A'` 或 `'P'` (即任何非 `'L'` 字符) 都要重置为 0, 不只是 `'P'`。
2. **absentCount 是全局累计的**: 不需要重置, 因为统计的是总缺勤天数。
3. **两个条件是 AND 关系**: 任意一个违反就返回 false, 都不违反才返回 true。
4. **边界条件**: "少于 2 天" 意味着 >= 2 就不行; "连续 3 天或以上" 意味着 >= 3 就不行。

---

## 解法详解

### 解法一: 一次遍历 (推荐)

```cpp
class Solution {
public:
    bool checkRecord(string s) {
        int absentCount = 0;   // 累计缺勤次数
        int lateStreak = 0;    // 当前连续迟到次数

        for (char c : s) {
            if (c == 'A') {
                absentCount++;
                if (absentCount >= 2) return false;
                lateStreak = 0;  // 非 L, 重置连续迟到
            } else if (c == 'L') {
                lateStreak++;
                if (lateStreak >= 3) return false;
            } else {  // 'P'
                lateStreak = 0;  // 非 L, 重置连续迟到
            }
        }
        return true;
    }
};
```

### 解法二: STL count + find (简洁写法)

```cpp
class Solution {
public:
    bool checkRecord(string s) {
        // count('A') < 2: 缺勤少于 2 次
        // find("LLL") == npos: 不含连续 3 个 L
        return count(s.begin(), s.end(), 'A') < 2
            && s.find("LLL") == string::npos;
    }
};
```

### 解法三: 提前终止优化版

```cpp
class Solution {
public:
    bool checkRecord(string s) {
        int aCount = 0, lStreak = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == 'L') {
                lStreak++;
                if (lStreak >= 3) return false;
            } else {
                lStreak = 0;
                if (s[i] == 'A') {
                    aCount++;
                    if (aCount >= 2) return false;
                }
            }
        }
        return true;
    }
};
```

---

## 易错点

| 错误写法 (✗) | 正确写法 (✓) | 说明 |
|-------------|-------------|------|
| ✗ `lateStreak > 3` | ✓ `lateStreak >= 3` | "连续 3 天或以上"迟到就不行, >= 3 是正确阈值 |
| ✗ 遇到 'A' 时不重置 lateStreak | ✓ 遇到 'A' 时 `lateStreak = 0` | A 也不是 L, 打断了连续迟到 |
| ✗ `absentCount > 2` | ✓ `absentCount >= 2` | "严格少于 2 天"即 >= 2 就不合格 |
| ✗ 只在 c=='P' 时重置 lateStreak | ✓ 只要 c != 'L' 都重置 | A 和 P 都会打断连续 L |
| ✗ 遍历完才检查条件 | ✓ 发现违规立即 return false | 提前终止更高效 (虽然渐近复杂度相同) |

---

## 面试追问

**Q1: 如果把条件泛化为"缺勤少于 k 天, 连续迟到少于 m 天", 怎么改?**

只需将硬编码的 2 和 3 替换为参数 k 和 m 即可, 算法结构完全不变。`absentCount >= k` 返回 false, `lateStreak >= m` 返回 false。

**Q2: 这题和 552 (Student Attendance Record II) 的关系是什么?**

551 是"检查一个给定记录是否合法" (判断问题), 552 是"计算长度为 n 的所有合法记录总数" (计数问题)。552 需要 DP, 状态为 `dp[已用A次数][末尾连续L次数]`, 共 2*3=6 个状态。

**Q3: 如果字符串非常长 (10^6+), 有没有更快的方法?**

一次遍历 O(n) 已经是理论下界 (每个字符至少要看一次), 无法更快。可以做的优化是提前终止: 发现两个 A 后不再继续扫描。如果要并行化, 可以分段统计 A 个数和每段最大连续 L, 但面试中 O(n) 足够。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 552 | Student Attendance Record II | 本题的 DP 计数版 |
| 485 | Max Consecutive Ones | 连续 1 的最大长度, 类似连续计数 |
| 1446 | Consecutive Characters | 最长连续相同字符 |
| 1004 | Max Consecutive Ones III | 滑动窗口 + 连续计数 |
| 674 | Longest Continuous Increasing Subsequence | 连续递增计数 |
