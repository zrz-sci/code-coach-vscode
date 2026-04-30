# 260. Single Number III (只出现一次的数字 III)

**难度**: Medium | **标签**: Bit Manipulation, Array

## 核心思路

数组中恰好有两个数只出现一次，其余出现两次。要求 O(n) 时间 + O(1) 空间。核心技巧：**全体异或得到 a^b，根据某一位不同将数组分成两组，各组独立异或**。因为 a != b，所以 a^b 至少有一位为 1，这一位上 a 和 b 不同，可以用来分组。

## 思维链

1. **回顾 Single Number I**: 全体异或可以找出唯一出现一次的数（成对消除）
2. **全体异或**: 对所有数异或，结果是 `a ^ b`（成对的数消除了）
3. **找不同位**: `a ^ b` 中为 1 的某一位表示 a 和 b 在该位上不同
4. **提取最低位 1**: 用 `diff & (-diff)` 或 `diff & ~(diff-1)` 得到最低有效位
5. **分组异或**: 根据该位是否为 1 将所有数分成两组，每组恰好包含一个目标数
6. **各组异或**: 成对的数在同一组中消除，剩下的就是目标数

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 位操作分组异或 | O(n) | O(1) | ⭐⭐⭐⭐⭐ |
| 哈希表计数 | O(n) | O(n) | ⭐⭐⭐ |

## 关键提示

- `x & (-x)` 提取最低位的 1（`-x` 是补码取反加一）
- 分组依据：该位为 1 的一组，为 0 的一组
- 成对出现的数一定在同一组（相同的数每一位都相同）
- 两个目标数一定在不同组（在该选定位上不同）
- 注意 INT_MIN 的处理：`diff & (-diff)` 在 diff == INT_MIN 时可能溢出

## 解法详解

### 解法1: 位操作分组异或 (最优解)

**思路**: 全体异或 → 提取区分位 → 分组异或。

```cpp
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        // Step 1: 全体异或，得到 a ^ b
        unsigned int xorAll = 0;
        for (int num : nums) {
            xorAll ^= num;
        }
        
        // Step 2: 提取最低位的 1（a 和 b 在此位不同）
        unsigned int diffBit = xorAll & (-xorAll);
        
        // Step 3: 按 diffBit 分组异或
        int a = 0, b = 0;
        for (int num : nums) {
            if (num & diffBit) {
                a ^= num;  // 该位为 1 的组
            } else {
                b ^= num;  // 该位为 0 的组
            }
        }
        
        return {a, b};
    }
};
```

**复杂度**: 时间 O(n)，空间 O(1)

### 解法2: 哈希表计数

**思路**: 统计每个数的出现次数，返回只出现一次的两个数。

```cpp
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }
        
        vector<int> res;
        for (auto& [num, cnt] : count) {
            if (cnt == 1) {
                res.push_back(num);
            }
        }
        return res;
    }
};
```

**复杂度**: 时间 O(n)，空间 O(n)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `int diff = xorAll & (-xorAll)` | ✓ `unsigned int diff = ...` | INT_MIN 取负溢出，用 unsigned 安全 |
| ✗ 全体异或后直接返回 | ✓ 需要分组再异或 | 全体异或得到 a^b 不是答案 |
| ✗ 随意选一位来分组 | ✓ 用 `x & (-x)` 取最低位 1 | 高效且保证该位 a 和 b 不同 |
| ✗ 只异或一组 | ✓ 两组都要异或 | 两组分别产出 a 和 b |

## 面试追问

**Q1**: 为什么用 `x & (-x)` 提取最低位？
→ **A**: `-x` 是 `~x + 1`，与 x 做 AND 后只保留最低的 1。例如 `x=0110` → `-x=1010` → `x&(-x)=0010`。

**Q2**: 为什么成对的数一定在同一组？
→ **A**: 相同的数每一位都相同，用任何一位分组，它们一定在同一组。

**Q3**: 如果有三个数各出现一次怎么办？
→ **A**: 不能直接用此方法。需要更复杂的位运算技巧或数学方法（如 LC 137 的推广）。

## 相关题型

- [136. Single Number](https://leetcode.com/problems/single-number/) - 一个数出现一次（直接异或）
- [137. Single Number II](https://leetcode.com/problems/single-number-ii/) - 一个数出现一次，其余三次
- [268. Missing Number](https://leetcode.com/problems/missing-number/) - 缺失的数字（异或应用）
