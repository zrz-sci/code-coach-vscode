# 372. Super Pow - 超级次方

## 核心思路
将超大指数 b 按位拆解，利用模运算性质 `(a*b) mod m = ((a mod m) * (b mod m)) mod m`，配合快速幂递归计算 `a^b mod 1337`。

## 思维链
1. b 是数组形式的超大数，无法直接作为 int 指数
2. 关键等式：`a^[1,5,6,4] = a^4 * (a^[1,5,6])^10`
3. 从数组末尾逐位处理，每次取出最后一位 d，计算 `a^d * (递归结果)^10`
4. 每步取模防溢出，利用 `powmod(a, k)` 计算 a^k mod 1337（k 仅 0~9）
5. 最终得到 `a^b mod 1337`

## 解法概览表 ⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 递归拆指数 | O(n) | O(n) | 最直观，按位递归 |
| 迭代拆指数 | O(n) | O(1) | 从高位到低位迭代 |
| 欧拉定理 | O(n) | O(1) | 数论优化，利用 phi(1337)=1140 |

## 关键提示
1. **模运算分配律**：`(a * b) % m = ((a % m) * (b % m)) % m`
2. **指数拆分**：`a^(10k+d) = (a^k)^10 * a^d`
3. **1337 = 7 * 191**，都是质数
4. **a 先取模**：`a = a % 1337`，避免大数溢出
5. **快速幂**中间结果也要取模

## 解法详解

### 解法一：递归拆指数（推荐）⭐
**思路**：每次取数组最后一位作为当前位指数，递归处理剩余高位。

```cpp
class Solution {
    const int MOD = 1337;
    
    long powmod(long a, int k) {
        a %= MOD;
        long result = 1;
        for (int i = 0; i < k; i++) {
            result = result * a % MOD;
        }
        return result;
    }
    
public:
    int superPow(int a, vector<int>& b) {
        if (b.empty()) return 1;
        int last = b.back();
        b.pop_back();
        // a^[...d] = a^d * (a^[...])^10
        return powmod(superPow(a, b), 10) * powmod(a, last) % MOD;
    }
};
```

### 解法二：迭代（从高位到低位）
**思路**：从最高位开始，每处理一位就将之前的结果提升 10 次方。

```cpp
class Solution {
    const int MOD = 1337;
    
    long powmod(long a, int k) {
        a %= MOD;
        long result = 1;
        for (int i = 0; i < k; i++)
            result = result * a % MOD;
        return result;
    }
    
public:
    int superPow(int a, vector<int>& b) {
        long result = 1;
        for (int i = 0; i < b.size(); i++) {
            result = powmod(result, 10) * powmod(a, b[i]) % MOD;
        }
        return result;
    }
};
```

### 解法三：欧拉定理优化
**思路**：利用欧拉定理 `a^phi(m) ≡ 1 (mod m)`，先将 b mod phi(1337) 求出真正的指数。

```cpp
class Solution {
public:
    int superPow(int a, vector<int>& b) {
        // phi(1337) = phi(7) * phi(191) = 6 * 190 = 1140
        int phi = 1140;
        int exp = 0;
        for (int d : b) {
            exp = (exp * 10 + d) % phi;
        }
        if (exp == 0) exp = phi; // 费马小定理边界
        
        long result = 1;
        long base = a % 1337;
        while (exp > 0) {
            if (exp & 1) result = result * base % 1337;
            base = base * base % 1337;
            exp >>= 1;
        }
        return result;
    }
};
```

## 易错点
- ✗ 不对 a 取模，导致中间乘法溢出 → ✓ 每步先 `a %= 1337`
- ✗ powmod 中 result 初始化为 a → ✓ 初始化为 1，循环 k 次
- ✗ 递归不处理 b 为空的 base case → ✓ `b.empty()` 返回 1
- ✗ 忘记两部分乘积也要取模 → ✓ `part1 * part2 % MOD`
- ✗ 使用 int 存中间结果溢出 → ✓ 使用 long 存中间乘积

## 面试追问

**Q1: 为什么不能直接用快速幂？**
A: b 以数组形式给出，长度可达 2000 位，远超 int/long 范围。必须将指数拆解按位处理，每步取模。

**Q2: 迭代解法和递归解法哪个更优？**
A: 迭代解法空间 O(1) 优于递归的 O(n) 栈空间。两者时间都是 O(n)。迭代从高位到低位更自然，递归从低位到高位需要 pop_back。

**Q3: 如果 MOD 不是 1337 而是任意数，欧拉定理解法有什么限制？**
A: 欧拉定理要求 gcd(a, m) = 1。当 a 是 7 或 191 的倍数时，与 1337 不互质，需要用中国剩余定理分别对 7 和 191 取模再合并。

## 相关题型
- [50. Pow(x, n)](../0050_powx_n/) - 快速幂基础
- [326. Power of Three](../0326_power_of_three/) - 幂运算判断
- [29. Divide Two Integers](../0029_divide_two_integers/) - 大数处理思想
