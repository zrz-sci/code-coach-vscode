# 825. Friends Of Appropriate Ages

## 核心思路

利用年龄范围有限（1~120），用**计数数组**统计每个年龄的人数，然后枚举所有年龄对 `(ageA, ageB)` 判断是否满足发送条件。本质是**桶计数 + 条件枚举**。

## 思维链

1. **分析发送条件**：x 向 y 发请求需同时满足三个否定条件的反面
2. **简化条件**：x 向 y 发请求当且仅当 `0.5 * age[x] + 7 < age[y] <= age[x]`（第三个条件被第二个蕴含）
3. **构建计数数组**：`count[age]` 表示年龄为 age 的人数
4. **枚举年龄对**：对每对 `(ageA, ageB)` 其中 `ageA` 是发送者年龄，`ageB` 是接收者年龄
5. **计算请求数**：若 `ageA == ageB`，请求数为 `count[ageA] * (count[ageA] - 1)`（排除自己）；否则为 `count[ageA] * count[ageB]`
6. **注意最小年龄**：当 `0.5 * age + 7 >= age` 即 `age <= 14` 时，该年龄的人不会发送任何请求

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 桶计数 + 枚举 ⭐ | O(N + 120^2) | O(1) | 最优解，利用年龄有限范围 |
| 排序 + 双指针/二分 | O(NlogN) | O(1) | 通用方法，不依赖值域限制 |

## 关键提示

- **条件简化**：第三个条件 `age[y] > 100 && age[x] < 100` 被第二个条件 `age[y] > age[x]` 蕴含（若 y>100 且 x<100，则 y>x），所以只需检查两个条件
- **最终条件**：`0.5 * ageX + 7 < ageY <= ageX`
- **自己不能给自己发**：当 ageA == ageB 时，同年龄人互发，需排除自身（乘 count-1）
- 年龄 <= 14 的人永远不会发请求（`0.5 * 14 + 7 = 14 >= 14`）

## 解法详解

### 解法一：桶计数 + 年龄对枚举（推荐）

```cpp
class Solution {
public:
    int numFriendRequests(vector<int>& ages) {
        vector<int> count(121, 0);
        for (int age : ages) count[age]++;
        
        int result = 0;
        for (int ageA = 15; ageA <= 120; ageA++) {  // 发送者年龄
            if (count[ageA] == 0) continue;
            for (int ageB = 15; ageB <= 120; ageB++) {  // 接收者年龄
                if (count[ageB] == 0) continue;
                // 条件：0.5 * ageA + 7 < ageB <= ageA
                if (ageB <= ageA && ageB > ageA / 2 + 7) {
                    if (ageA == ageB) {
                        // 同年龄：每人可向其余同龄人发送
                        result += count[ageA] * (count[ageA] - 1);
                    } else {
                        result += count[ageA] * count[ageB];
                    }
                }
            }
        }
        return result;
    }
};
```

### 解法二：排序 + 二分查找

```cpp
class Solution {
public:
    int numFriendRequests(vector<int>& ages) {
        sort(ages.begin(), ages.end());
        int n = ages.size();
        int result = 0;
        
        for (int i = 0; i < n; i++) {
            int ageX = ages[i];
            if (ageX <= 14) continue; // 无法发送请求
            
            // 找满足 0.5*ageX + 7 < ageY <= ageX 的范围
            int low = ageX / 2 + 7; // ageY 必须严格大于 low
            // 用 upper_bound 找第一个 > low 的位置
            int left = upper_bound(ages.begin(), ages.end(), low) - ages.begin();
            // 用 upper_bound 找第一个 > ageX 的位置
            int right = upper_bound(ages.begin(), ages.end(), ageX) - ages.begin();
            
            // [left, right) 中的人都可以接收 i 的请求，但排除自己
            int cnt = right - left - 1; // 减去自己
            if (cnt > 0) result += cnt;
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 浮点比较 `0.5 * ageA + 7` | ✓ 整数比较 `ageA / 2 + 7` | 使用整数除法避免浮点精度问题 |
| ✗ 同年龄时 `count[a] * count[a]` | ✓ `count[a] * (count[a] - 1)` | 不能给自己发请求 |
| ✗ 从 ageA=1 开始枚举 | ✓ 从 ageA=15 开始 | 年龄 <= 14 无法发送请求 |
| ✗ 条件写成 `ageB >= ageA/2 + 7` | ✓ `ageB > ageA/2 + 7` | 是严格大于，不是大于等于 |
| ✗ 忘记跳过 count==0 | ✓ `if (count[ageA] == 0) continue` | 优化常数，避免无效计算 |

## 面试追问

**Q1: 为什么第三个条件 `age[y] > 100 && age[x] < 100` 可以忽略？**
> 如果 age[y] > 100 且 age[x] < 100，则 age[y] > age[x]，已经被第二个条件 `age[y] > age[x]` 阻止了。所以第三个条件是冗余的。

**Q2: 桶计数方法的时间复杂度为什么是 O(N + 120^2) 而非 O(N^2)？**
> 统计计数数组需要 O(N)，枚举所有年龄对需要 O(120*120) = O(14400)，这是常数级别。总复杂度为 O(N + C^2)，C=120 为年龄上限。

**Q3: 如果年龄范围扩大到 1~10^9，如何优化？**
> 不能用桶计数。排序后用双指针或二分查找：对每个人，二分找出可接收请求的范围。时间复杂度 O(NlogN)。

## 相关题型

- [LC 532] K-diff Pairs in an Array（计数 + 配对）
- [LC 454] 4Sum II（桶计数加速配对）
- [LC 1099] Two Sum Less Than K（排序 + 双指针条件配对）
- [LC 611] Valid Triangle Number（排序 + 双指针计数）
