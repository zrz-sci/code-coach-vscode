# 254. Factor Combinations (因子的组合)

**难度**: Medium | **标签**: Backtracking

## 核心思路

将整数 n 分解为因子的乘积组合 = **回溯搜索所有因式分解方式**。关键在于避免重复组合：每次递归时，因子从上一次使用的因子开始（保证非递减），且因子范围是 `[2, n-1]`。

## 思维链

1. **理解问题**: 找出 n 的所有因式分解，因子范围 [2, n-1]，不包含 n 本身
2. **递归子问题**: 如果 i 是 n 的因子，则问题分解为 `{i} + getFactors(n/i)`
3. **去重策略**: 保持因子非递减，下一个因子 >= 当前因子
4. **搜索上界**: 因子只需枚举到 `sqrt(n)`，因为较大因子会作为商出现
5. **递归终止**: 当 n == 1 时收集一个完整的因式分解结果
6. **注意**: 不能只有 n 本身（即至少两个因子的乘积）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 回溯 (因子从 start 枚举到 sqrt(n)) | O(2^(log n)) | O(log n) | ⭐⭐⭐⭐⭐ |
| 回溯 (因子枚举到 n/i) | O(2^(log n)) | O(log n) | ⭐⭐⭐⭐ |

## 关键提示

- 因子枚举的起始点 `start` 保证不产生重复组合（如 `[2,6]` 和 `[6,2]`）
- 枚举上界可以优化为 `sqrt(n)` 或 `n/i >= i` 即 `i*i <= n`
- 每找到因子 i，将 n/i 作为"最后一个因子"直接加入也是一个合法分解

## 解法详解

### 解法1: 回溯 (经典写法)

**思路**: 从小到大枚举因子 i，递归分解 n/i，保持因子非递减。

```cpp
class Solution {
public:
    vector<vector<int>> getFactors(int n) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(n, 2, path, res);
        return res;
    }
    
    void dfs(int n, int start, vector<int>& path, vector<vector<int>>& res) {
        // 从 start 到 sqrt(n) 枚举因子
        for (int i = start; (long long)i * i <= n; i++) {
            if (n % i == 0) {
                // 选择因子 i，加上商 n/i 作为一个完整组合
                path.push_back(i);
                path.push_back(n / i);
                res.push_back(path);
                path.pop_back();
                
                // 继续分解 n/i
                dfs(n / i, i, path, res);
                path.pop_back();
            }
        }
    }
};
```

**复杂度**: 时间 O(2^(log n))，空间 O(log n) 递归栈深度

### 解法2: 回溯 (枚举到 n 的写法)

**思路**: 因子枚举到 n-1，当 n == 1 且路径长度 > 1 时收集结果。

```cpp
class Solution {
public:
    vector<vector<int>> getFactors(int n) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(n, 2, path, res);
        return res;
    }
    
    void dfs(int n, int start, vector<int>& path, vector<vector<int>>& res) {
        if (n == 1) {
            if (path.size() > 1) {
                res.push_back(path);
            }
            return;
        }
        for (int i = start; i <= n; i++) {
            if (n % i == 0) {
                path.push_back(i);
                dfs(n / i, i, path, res);
                path.pop_back();
            }
        }
    }
};
```

**复杂度**: 时间 O(2^(log n))，空间 O(log n)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 因子从 1 开始 | ✓ 因子从 2 开始 | 因子范围要求 [2, n-1] |
| ✗ 不检查 path.size() > 1 | ✓ n 本身不算合法分解 | [n] 不是合法输出 |
| ✗ 每次从 2 开始枚举 | ✓ 从 start 开始枚举 | 否则产生重复如 [2,6] 和 [6,2] |
| ✗ `i <= n` 且不过滤 | ✓ `i * i <= n` 更高效 | 减少不必要的枚举 |

## 面试追问

**Q1**: 如何保证结果中因子是非递减的？
→ **A**: 通过 start 参数，每次递归中因子从 start 开始枚举。

**Q2**: 时间复杂度怎么分析？
→ **A**: n 的因式分解数量约为 O(2^(log n))，因为每次至少减半。

**Q3**: 如何只返回质因数分解？
→ **A**: 在枚举因子时跳过非素数，或在递归中只使用素数因子。

## 相关题型

- [39. Combination Sum](https://leetcode.com/problems/combination-sum/) - 组合总和（加法版）
- [77. Combinations](https://leetcode.com/problems/combinations/) - 组合
- [40. Combination Sum II](https://leetcode.com/problems/combination-sum-ii/) - 组合总和 II
