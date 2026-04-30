# 401. Binary Watch - 二进制手表

## 核心思路
二进制手表共 10 个 LED（4 小时 + 6 分钟），给定亮灯数，枚举所有合法时间。本质是**枚举所有组合**，关键洞察：直接遍历所有可能的时间 (0-11 小时, 0-59 分钟)，统计二进制 1 的个数是否等于 turnedOn。

## 思维链
1. 暴力回溯：从 10 个 LED 中选 turnedOn 个，生成时间 => 需要去重、验证合法性
2. **逆向思维**：直接枚举 12*60=720 种时间，用 `__builtin_popcount` 检查 => 代码极简
3. 分组枚举：将 turnedOn 分配给小时(h个) 和分钟(turnedOn-h个)，分别枚举

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 枚举所有时间 | O(12*60) | O(1) | 最简洁，面试推荐 |
| ⭐ 回溯/组合枚举 | O(2^10) | O(10) | 练习回溯思想 |

## 关键提示
1. **位运算技巧**：将小时和分钟拼成一个 10-bit 数 `(h << 6) | m`，统计 1 的个数
2. **合法范围**：小时 0-11，分钟 0-59，超范围的跳过
3. **格式要求**：小时无前导零，分钟必须两位（需 `%02d` 格式化）

## 解法详解

### 解法一：枚举所有时间（推荐）
```cpp
class Solution {
public:
    vector<string> readBinaryWatch(int turnedOn) {
        vector<string> res;
        for (int h = 0; h < 12; h++) {
            for (int m = 0; m < 60; m++) {
                if (__builtin_popcount(h) + __builtin_popcount(m) == turnedOn) {
                    res.push_back(to_string(h) + ":" + (m < 10 ? "0" : "") + to_string(m));
                }
            }
        }
        return res;
    }
};
```

### 解法二：回溯法
```cpp
class Solution {
    vector<string> res;
    int hours[4] = {1, 2, 4, 8};
    int mins[6] = {1, 2, 4, 8, 16, 32};
public:
    void dfs(int num, int idx, int h, int m) {
        if (h > 11 || m > 59) return;
        if (num == 0) {
            res.push_back(to_string(h) + ":" + (m < 10 ? "0" : "") + to_string(m));
            return;
        }
        for (int i = idx; i < 10; i++) {
            if (i < 4) dfs(num - 1, i + 1, h + hours[i], m);
            else dfs(num - 1, i + 1, h, m + mins[i - 4]);
        }
    }
    vector<string> readBinaryWatch(int turnedOn) {
        dfs(turnedOn, 0, 0, 0);
        return res;
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `to_string(m)` 当 m<10 | `"0" + to_string(m)` | 分钟必须两位，如 "0:02" 不是 "0:2" |
| 小时范围 0-23 | 小时范围 0-11 | 二进制手表只显示 12 小时制 |
| 分钟范围 0-63 | 分钟范围 0-59 | 6位二进制最大 63，但分钟最多 59 |

## 面试追问

**Q1: 为什么直接枚举比回溯更优？**
> 搜索空间只有 720 种，直接枚举 O(720) 比回溯 O(2^10)=1024 还少，而且代码更简洁不易出错。

**Q2: 如果手表显示 24 小时制怎么改？**
> 只需将小时范围改为 0-23，需要 5 个 LED 表示小时（最大 23 = 10111），总共 11 个 LED。

**Q3: `__builtin_popcount` 的原理？**
> 编译器内置函数，利用 CPU 指令（如 x86 的 POPCNT）在 O(1) 时间计算二进制中 1 的个数。手写实现可用 `n & (n-1)` 循环消除最低位 1。

## 相关题型
- [191. Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/) - popcount 基础
- [78. Subsets](https://leetcode.com/problems/subsets/) - 组合枚举
- [77. Combinations](https://leetcode.com/problems/combinations/) - C(n,k) 回溯
