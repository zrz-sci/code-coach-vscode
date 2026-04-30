# 539. 最小时间差

## 核心思路

将所有时间转换为**分钟数**（0~1439），排序后计算相邻元素差值，同时考虑**跨午夜**的环形差值（首尾之差）。利用鸽巢原理可以提前判断：如果时间点 > 1440，必有重复。

## 思维链

1. 24小时制 = 1440分钟，将 "HH:MM" 转为 0~1439 的整数
2. 排序后，最小差值一定出现在**相邻元素**之间
3. 别忘跨午夜情况：`1440 - (last - first)` 也是候选答案
4. 优化：如果时间点数 > 1440，根据鸽巢原理必有重复，直接返回 0
5. 可以用 bucket sort（桶排序）达到 O(1440) 时间

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 排序 + 遍历 | O(n log n) | O(n) | ⭐⭐⭐ |
| 桶排序优化 | O(n + 1440) | O(1440) | ⭐⭐ |

## 关键提示

1. 时间转分钟：`h * 60 + m`
2. 一天 = 1440 分钟，跨午夜差值 = `1440 - (max - min)`
3. 鸽巢原理：n > 1440 时直接返回 0
4. 排序后只需看相邻差值 + 首尾环形差值

## 解法详解

### 解法一：排序 + 遍历（推荐）

```cpp
class Solution {
public:
    int findMinDifference(vector<string>& timePoints) {
        if (timePoints.size() > 1440) return 0; // 鸽巢原理

        vector<int> minutes;
        for (auto& t : timePoints) {
            int h = stoi(t.substr(0, 2));
            int m = stoi(t.substr(3, 2));
            minutes.push_back(h * 60 + m);
        }
        sort(minutes.begin(), minutes.end());

        int minDiff = INT_MAX;
        for (int i = 1; i < minutes.size(); i++) {
            minDiff = min(minDiff, minutes[i] - minutes[i - 1]);
        }
        // 跨午夜环形差值
        minDiff = min(minDiff, 1440 - minutes.back() + minutes.front());

        return minDiff;
    }
};
```

### 解法二：桶排序

```cpp
class Solution {
public:
    int findMinDifference(vector<string>& timePoints) {
        vector<bool> seen(1440, false);
        for (auto& t : timePoints) {
            int mins = stoi(t.substr(0, 2)) * 60 + stoi(t.substr(3, 2));
            if (seen[mins]) return 0; // 重复时间
            seen[mins] = true;
        }

        int first = -1, last = -1, prev = -1;
        int minDiff = INT_MAX;
        for (int i = 0; i < 1440; i++) {
            if (!seen[i]) continue;
            if (first == -1) {
                first = i;
            } else {
                minDiff = min(minDiff, i - prev);
            }
            prev = i;
            last = i;
        }
        minDiff = min(minDiff, 1440 - last + first); // 环形
        return minDiff;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 只比较相邻差值 | 还要比较 `1440 - back + front` | 跨午夜是环形问题 |
| 不做鸽巢原理剪枝 | `if (n > 1440) return 0` | 超过1440个时间点必有重复 |
| `t.substr(0, 2)` 解析错误 | 确保按 "HH:MM" 固定格式解析 | 格式保证是5个字符 |

## 面试追问

**Q1: 为什么排序后只需比较相邻元素？**
→ 排序后非相邻差值必然 >= 相邻差值，数学可证。

**Q2: 桶排序的优势和局限？**
→ O(n) 时间但需要 O(1440) 空间；当 n >> 1440 时优势明显（鸽巢直接返回 0）。

**Q3: 如果时间精确到秒怎么办？**
→ 一天 = 86400 秒，桶数组扩大到 86400，方法不变但空间增加。

## 相关题型

- [LeetCode 128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) - 排序/桶思想
- [LeetCode 220. Contains Duplicate III](https://leetcode.com/problems/contains-duplicate-iii/) - 桶排序应用
- [LeetCode 252. Meeting Rooms](https://leetcode.com/problems/meeting-rooms/) - 时间区间排序
