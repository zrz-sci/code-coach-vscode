# 479. 最大回文数乘积

## 核心思路
从大到小枚举回文数的上半部分来构造完整回文数，然后验证该回文数是否能分解为两个 n 位数的乘积，找到的第一个即为答案。

## 思维链
1. **明确范围**: n 位数范围 [10^(n-1), 10^n - 1]，两个 n 位数的乘积最大为 (10^n - 1)^2，是 2n 位数
2. **暴力不可行**: 枚举所有数对再判断回文，复杂度太高
3. **逆向思维**: 从大到小构造回文数，第一个能被分解的就是答案
4. **构造方法**: 取上半部分 half（从 10^n-1 递减），镜像拼接得到完整回文数
5. **验证分解**: 对每个回文数 P，枚举因子 i 从 upper 到 sqrt(P)，检查 P % i == 0 且商也在范围内
6. **特殊处理**: n=1 时直接返回 9

## 解法概览

| 方法 | 时间 | 空间 | 适用场景 | 推荐 |
|------|------|------|----------|------|
| 枚举回文数 + 验证分解 | O(10^n) 约 | O(1) | 通用解法 | ⭐ |
| 暴力枚举乘积对 | O(10^2n) | O(1) | 仅 n <= 3 | |

## 关键提示
1. n=1 是特殊情况，直接返回 9
2. 回文数由上半部分决定：half=99 → palindrome=9999, half=98 → palindrome=9889
3. 验证时从 upper 向下枚举因子 i，当 i*i < palindrome 时可以停止（因为另一个因子会超过 upper）
4. 最终结果对 1337 取模
5. 当 n >= 2 时，最大回文数一定是 2n 位的（可以证明一定存在）

## 解法详解

### 解法1: 枚举回文数 + 验证分解 ⭐

**思路**: 回文数的上半部分从大到小枚举，构造完整回文数后检查能否分解为两个 n 位数的乘积。

**构造过程**:
- n=2: upper=99, half 从 99 递减
  - half=99 → "99"+"99" = 9999, 验证不能分解
  - ...
  - half=90 → "90"+"09" = 9009 = 99*91, 成功！

```cpp
class Solution {
public:
    int largestPalindrome(int n) {
        if (n == 1) return 9;
        
        long upper = (long)pow(10, n) - 1;     // n 位数最大值
        long lower = (long)pow(10, n - 1);      // n 位数最小值
        
        for (long half = upper; half >= lower; --half) {
            // 构造回文数：half 拼接 half 的翻转
            string s = to_string(half);
            string rev = s;
            reverse(rev.begin(), rev.end());
            long palindrome = stol(s + rev);
            
            // 验证：能否分解为两个 n 位数的乘积
            for (long i = upper; i * i >= palindrome; --i) {
                if (palindrome % i == 0) {
                    long other = palindrome / i;
                    if (other >= lower && other <= upper) {
                        return (int)(palindrome % 1337);
                    }
                }
            }
        }
        return 0;
    }
};
```

### 解法2: 数学优化构造

**思路**: 直接用数学方法拼接回文数，不使用字符串转换，提高效率。

```cpp
class Solution {
public:
    int largestPalindrome(int n) {
        if (n == 1) return 9;
        
        long upper = (long)pow(10, n) - 1;
        long lower = (long)pow(10, n - 1);
        
        for (long half = upper; half >= lower; --half) {
            // 数学构造回文数
            long palindrome = half;
            long temp = half;
            while (temp > 0) {
                palindrome = palindrome * 10 + temp % 10;
                temp /= 10;
            }
            
            // 验证分解
            for (long i = upper; i * i >= palindrome; --i) {
                if (palindrome % i == 0 && palindrome / i <= upper) {
                    return (int)(palindrome % 1337);
                }
            }
        }
        return 0;
    }
};
```

## 易错点
- ✗ 忘记 n=1 的特殊情况，导致构造回文数逻辑出错 → ✓ n=1 直接返回 9
- ✗ 使用 int 导致溢出（n=8 时回文数可达 16 位） → ✓ 使用 long/long long
- ✗ 验证分解时只检查 palindrome % i == 0 但忘记验证商是否也是 n 位数 → ✓ 需要检查 other >= lower && other <= upper
- ✗ 枚举因子时没有正确的终止条件 → ✓ 当 i*i < palindrome 时停止

## 面试追问
- **Q1**: 为什么从回文数上半部分从大到小枚举是正确的？ → 上半部分越大，完整回文数越大，第一个能分解的就是最大的
- **Q2**: 时间复杂度如何分析？ → 外层枚举 O(10^n) 个回文数，每个验证最多 O(10^n) 次除法，但实际很快找到（通常前几个回文数就能分解）
- **Q3**: n=8 时 long long 是否够用？ → 16 位数最大约 10^16，long long 可达 9.2*10^18，足够

## 相关题型
- LC 9 回文数（判断回文数）
- LC 906 超级回文数（回文数的平方也是回文数）
- LC 866 回文素数（回文且是素数）
- LC 564 寻找最近的回文数（构造技巧）
