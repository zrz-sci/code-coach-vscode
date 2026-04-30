# 441. 排列硬币 (Arranging Coins)

## 核心思路
求最大的 k 使得 `k*(k+1)/2 <= n`。可以用**二分搜索**或**数学公式**直接求解。

## 思维链
1. 第 1 行放 1 枚，第 2 行放 2 枚...第 k 行放 k 枚
2. 前 k 行总共需要 k*(k+1)/2 枚硬币
3. 求最大 k 使得 k*(k+1)/2 <= n
4. 二分法：在 [1, n] 上二分 k，检查 k*(k+1)/2 <= n
5. 数学法：解一元二次方程 k = (-1 + sqrt(1+8n)) / 2

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| ⭐ 解法1: 二分搜索 | 二分 k 判断 k*(k+1)/2 <= n | O(logn) | O(1) | 面试首选 |
| 解法2: 数学公式 | 解二次方程 | O(1) | O(1) | 一行解法 |

## 关键提示
1. 二分时用 `long` 计算 `mid*(mid+1)/2` 防溢出
2. 数学公式中 `8.0*n` 要用浮点数避免溢出
3. 二分右边界可以用 n（不需要更大），因为 k <= n

## 解法详解

### 解法1: 二分搜索 ⭐
```cpp
class Solution {
public:
    int arrangeCoins(int n) {
        long lo = 1, hi = n;
        while (lo <= hi) {
            long mid = lo + (hi - lo) / 2;
            long coins = mid * (mid + 1) / 2;
            if (coins == n) return mid;
            else if (coins < n) lo = mid + 1;
            else hi = mid - 1;
        }
        return hi;
    }
};
```

### 解法2: 数学公式
```cpp
class Solution {
public:
    int arrangeCoins(int n) {
        return (int)((-1.0 + sqrt(1.0 + 8.0 * n)) / 2.0);
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `int coins = mid*(mid+1)/2` | `long coins = mid*(mid+1)/2` | mid 接近 2^31 时溢出 |
| `sqrt(1 + 8*n)` | `sqrt(1.0 + 8.0*n)` | 8*n 可能溢出 int |
| 返回 `lo` | 返回 `hi` | 循环结束后 hi 是最后一个满足条件的值 |

## 面试追问

**Q1: 二分搜索和数学公式哪个更好？**
> 面试中二分搜索更好，展示二分模板能力。数学公式有浮点精度风险，面试官可能追问精度问题。

**Q2: 为什么返回 hi 而不是 lo？**
> 循环结束时 lo > hi。hi 是最后一个使得 `mid*(mid+1)/2 <= n` 的值，即最大完整行数。

**Q3: 如果要求不完整的最后一行有多少枚硬币？**
> 计算 `n - hi*(hi+1)/2` 即为最后一行的硬币数。

## 相关题型
- [69. x 的平方根](https://leetcode.com/problems/sqrtx/) - 同类二分搜索
- [367. 有效的完全平方数](https://leetcode.com/problems/valid-perfect-square/) - 二分判定
