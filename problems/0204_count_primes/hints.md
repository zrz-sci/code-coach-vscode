# 204. 计数质数 (Count Primes)

**难度**: Medium | **标签**: Array, Math, Enumeration, Number Theory

## 核心思路

埃拉托斯特尼筛法（Sieve of Eratosthenes）：从 2 开始，将每个质数的倍数标记为合数，最后统计未被标记的数。

## 思维链

1. 暴力法：对每个数判断是否为质数 → O(n*sqrt(n))，超时
2. 优化：不需要逐个判断，可以「筛」掉合数
3. 关键洞察：如果 i 是质数，那么 i*2, i*3, i*4... 都不是质数
4. 进一步优化：从 i*i 开始标记（i*2 到 i*(i-1) 已被更小的质数标记过）
5. 只需筛到 sqrt(n)

## 解法概览

| 解法 | 思路 | 时间 | 推荐 |
|------|------|------|------|
| 埃氏筛法 | 标记质数的倍数为合数 | O(n log log n) | ⭐ |
| 优化埃氏筛 | 从 i*i 开始标记 + 只筛到 sqrt(n) | O(n log log n) | ⭐ |
| 线性筛（欧拉筛） | 每个合数只被最小质因子标记一次 | O(n) | |

## 关键提示

1. 题目要求**严格小于** n 的质数个数（不包含 n）
2. 0 和 1 不是质数，从 2 开始
3. 内层循环从 `i*i` 开始而非 `2*i`（前面的已被标记）
4. 外层循环只需到 `sqrt(n)`，因为 > sqrt(n) 的合数必有 <= sqrt(n) 的因子
5. 注意 `i*i` 可能溢出 int，用 `(long)i*i < n` 或改用 `j += i`

## 解法详解

### 解法一：埃氏筛法（推荐）

```cpp
class Solution {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;
        // is_prime[i] 表示 i 是否为质数
        vector<bool> is_prime(n, true);
        is_prime[0] = is_prime[1] = false;

        // 只需筛到 sqrt(n)
        for (int i = 2; (long)i * i < n; i++) {
            if (is_prime[i]) {
                // 从 i*i 开始标记，步长为 i
                for (int j = i * i; j < n; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        return count(is_prime.begin(), is_prime.end(), true);
    }
};
```

### 解法二：线性筛（欧拉筛）

```cpp
class Solution {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;
        vector<bool> is_prime(n, true);
        vector<int> primes;  // 存储已发现的质数
        is_prime[0] = is_prime[1] = false;

        for (int i = 2; i < n; i++) {
            if (is_prime[i]) {
                primes.push_back(i);
            }
            // 用已有质数去标记 i 的倍数
            for (int j = 0; j < primes.size() && (long)i * primes[j] < n; j++) {
                is_prime[i * primes[j]] = false;
                if (i % primes[j] == 0) break;  // 保证每个合数只被最小质因子筛一次
            }
        }

        return primes.size();
    }
};
```

**原理**：`if (i % primes[j] == 0) break` 确保每个合数只被标记一次（被它的最小质因子标记），实现严格 O(n)。

## 易错点

1. ✗ `for (int j = 2*i; ...)` 从 2*i 开始标记
   ✓ `for (int j = i*i; ...)` 从 i*i 开始，避免重复标记

2. ✗ `i * i` 溢出 int（当 i > 46340 时 i*i > INT_MAX）
   ✓ 用 `(long)i * i < n` 或把外层循环条件改为 `i <= sqrt(n)`

3. ✗ 统计时包含了 n → 题目要求「严格小于 n」
   ✓ 数组大小为 n，索引 0 到 n-1

4. ✗ 忘记处理 n <= 2 的边界（n=0, n=1, n=2 都应返回 0）
   ✓ 开头加边界检查

## 面试追问

**Q1**: 埃氏筛法的时间复杂度为什么是 O(n log log n)？
→ 调和级数 sum(1/p) 对质数求和收敛于 log log n

**Q2**: 线性筛为什么 `i % primes[j] == 0` 时要 break？
→ 保证每个合数只被最小质因子筛掉，避免重复标记，实现 O(n)

**Q3**: 如何优化空间？能否不用 bool 数组？
→ 可以用 bitset 压缩到 n/8 字节；或只标记奇数（跳过偶数）减半空间

## 相关题型

- 263: Ugly Number（因子分解）
- 264: Ugly Number II（DP + 多指针）
- 279: Perfect Squares（数论 + BFS/DP）
- 507: Perfect Number（因子枚举）
