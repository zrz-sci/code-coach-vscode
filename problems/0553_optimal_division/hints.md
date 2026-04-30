# 553. Optimal Division

[LeetCode 链接](https://leetcode.com/problems/optimal-division/)

## 核心思路

**数学贪心**: 对于所有正整数 >= 2 的数组, 最优解固定为 `nums[0] / (nums[1] / nums[2] / ... / nums[n-1])`。

数学本质: `a / (b / c / d) = a * c * d / b`。这把 nums[0] 放在分子, nums[1] 放在唯一的分母, 其余所有数 (nums[2..n-1]) 都变成分子的乘法因子。分子最大化、分母最小化, 结果必然最大。

看似需要区间 DP 枚举所有括号方案, 但数学分析后发现答案固定, O(n) 构造即可。区间 DP 是通用解法 (适用于数可 < 1 的情况), 面试中展示两种方法可加分。

---

## 思维链

```
输入: 正整数数组 nums (所有数 >= 2)
输出: 加括号使连除结果最大的表达式字符串

Step 1  数学分析:
        连除的本质: a/b/c/d = a * (1/b) * (1/c) * (1/d)
        要最大化: 分子尽可能大, 分母尽可能小

Step 2  关键洞察:
        a / (b / c / d / ...) = a * c * d * ... / b
        → 把 nums[0] 留在最外层分子
        → 把 nums[1] 放在分母
        → nums[2..n-1] 全部通过"除以除法"变成乘法因子

Step 3  构造答案:
        ┌─ n == 1: 直接返回 nums[0]
        ├─ n == 2: 返回 "a/b" (不加括号, 避免冗余)
        └─ n >= 3: 返回 "a/(b/c/d/...)" (括号包裹 nums[1..n-1])

Step 4  验证: nums = [1000,100,10,2]
        1000/(100/10/2) = 1000/5 = 200  ← 最大!
        vs 1000/100/10/2 = 0.5
        vs 1000/(100/10)/2 = 50
        vs 1000/(100/(10/2)) = 50

时间: O(n) — 一次遍历构造字符串
空间: O(n) — 输出字符串
```

---

## 解法概览

| 解法 | 时间 | 空间 | 推荐 | 说明 |
|------|------|------|------|------|
| 数学贪心 | O(n) | O(n) | ⭐ | 直接构造答案, 本题最优解 |
| 区间 DP | O(n^3) | O(n^2) | | 通用解法, 追踪最大值+最小值+表达式 |

---

## 关键提示

1. **特判 n==1**: 只有一个数, 没有除法操作, 直接返回数字字符串。
2. **特判 n==2**: 两个数 `"a/b"`, 不需要括号 (题目要求不含冗余括号)。
3. **n>=3 的通用公式**: `"a/(b/c/d/...)"`, 把 nums[1..n-1] 用一对括号包起来。
4. **区间 DP 必须同时追踪 max 和 min**: 因为 `max / min` 才能产生最大值, `min / max` 才能产生最小值。

---

## 解法详解

### 解法一: 数学贪心 (推荐)

```cpp
class Solution {
public:
    string optimalDivision(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return to_string(nums[0]);
        if (n == 2) return to_string(nums[0]) + "/" + to_string(nums[1]);

        // n >= 3: "a/(b/c/d/...)"
        string result = to_string(nums[0]) + "/(";
        for (int i = 1; i < n; i++) {
            result += to_string(nums[i]);
            if (i < n - 1) result += "/";
        }
        result += ")";
        return result;
    }
};
```

### 解法二: 区间 DP (通用解法, 面试加分项)

```cpp
class Solution {
public:
    string optimalDivision(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return to_string(nums[0]);

        // dp_max[i][j] / dp_min[i][j]: nums[i..j] 能得到的最大/最小值
        // expr_max[i][j] / expr_min[i][j]: 对应的表达式
        vector<vector<double>> dpMax(n, vector<double>(n, 0));
        vector<vector<double>> dpMin(n, vector<double>(n, 1e18));
        vector<vector<string>> exprMax(n, vector<string>(n));
        vector<vector<string>> exprMin(n, vector<string>(n));

        // 初始化: 单个元素
        for (int i = 0; i < n; i++) {
            dpMax[i][i] = dpMin[i][i] = nums[i];
            exprMax[i][i] = exprMin[i][i] = to_string(nums[i]);
        }

        // 枚举区间长度 (从 2 到 n)
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                for (int k = i; k < j; k++) {
                    // 最大值 = 左边最大 / 右边最小
                    double val = dpMax[i][k] / dpMin[k + 1][j];
                    if (val > dpMax[i][j]) {
                        dpMax[i][j] = val;
                        string right = (k + 1 == j)
                            ? exprMin[k + 1][j]
                            : "(" + exprMin[k + 1][j] + ")";
                        exprMax[i][j] = exprMax[i][k] + "/" + right;
                    }
                    // 最小值 = 左边最小 / 右边最大
                    val = dpMin[i][k] / dpMax[k + 1][j];
                    if (val < dpMin[i][j]) {
                        dpMin[i][j] = val;
                        string right = (k + 1 == j)
                            ? exprMax[k + 1][j]
                            : "(" + exprMax[k + 1][j] + ")";
                        exprMin[i][j] = exprMin[i][k] + "/" + right;
                    }
                }
            }
        }
        return exprMax[0][n - 1];
    }
};
```

**干跑示例**: nums = [1000, 100, 10, 2]

```
贪心解法:
n = 4 >= 3
result = "1000/" + "(" + "100/10/2" + ")" = "1000/(100/10/2)"
验证: 1000/(100/10/2) = 1000/(10/2) = 1000/5 = 200 ✓

区间 DP 同样会得到 "1000/(100/10/2)" ← max[0][3]
```

---

## 易错点

| 错误写法 (✗) | 正确写法 (✓) | 说明 |
|-------------|-------------|------|
| ✗ n==2 时返回 `"a/(b)"` | ✓ n==2 返回 `"a/b"` | 题目明确要求不含冗余括号 |
| ✗ 忘记 n==1 特判 | ✓ n==1 直接返回数字字符串 | 单个数没有除法操作 |
| ✗ 区间 DP 只追踪 max | ✓ 同时追踪 max 和 min | max/min 才能得到极值, 只有 max 会漏掉最优分割 |
| ✗ 以为需要复杂 DP | ✓ 数学分析后贪心 O(n) 搞定 | 所有数 >= 2 时答案固定, 不需要枚举 |
| ✗ 区间 DP 括号添加错误 | ✓ 只有 k+1 != j 时右半部分才加括号 | 单个数不需要括号 |

---

## 面试追问

**Q1: 如果数字可以是 0 或小数 (< 1), 贪心还成立吗?**

不成立。如果某个数 < 1 (如 0.5), 放在分母反而会增大结果 (`a / 0.5 = 2a`)。此时需要区间 DP, 枚举所有分割点, 同时追踪最大值和最小值。

**Q2: 如果运算符不只有除法, 还有加减乘, 怎么做?**

需要区间 DP, 类似"矩阵链乘法"或 LC 241 的思路。对每个区间枚举所有分割点和运算符, 追踪最大值和最小值。时间 O(n^3)。

**Q3: 这题和 241. Different Ways to Add Parentheses 的关系?**

241 是分治/记忆化搜索枚举所有加括号方式, 返回所有可能的结果值。本题可以用类似方法 (区间 DP), 但由于所有数 >= 2 的约束, 贪心直接构造是最优解。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 241 | Different Ways to Add Parentheses | 加括号枚举所有结果 |
| 312 | Burst Balloons | 经典区间 DP |
| 282 | Expression Add Operators | 表达式类问题 |
| 1039 | Minimum Score Triangulation of Polygon | 区间 DP |
| 1246 | Palindrome Removal | 区间 DP |
