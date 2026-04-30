// ============================================================
// LeetCode 600: 不含连续1的非负整数
// (Non-negative Integers without Consecutive Ones)
// ============================================================

// 【题目本质】
// [0, n] 中二进制表示不含连续 1 的数的个数。
// 数位 DP 经典题，利用斐波那契数列加速计算。

// 【解法总览】
// 解法一：斐波那契 + 逐位统计（推荐） - O(log n)
// 解法二：数位 DP 记忆化搜索 - O(log n)

// ============================================================
// 解法一：斐波那契 + 逐位统计（推荐）
// 思路：f[k] 表示 k 位二进制串中无连续 1 的数量
//       从高位到低位扫描 n，遇到 1 时累加 f[剩余位]
// ============================================================
class Solution {
public:
    int findIntegers(int n) {
        // f[i]: i 位二进制中无连续 1 的数量
        // f[0]=1, f[1]=2, f[2]=3, f[3]=5, ...（斐波那契）
        vector<int> f(32, 0);
        f[0] = 1;
        f[1] = 2;
        for (int i = 2; i < 32; i++) {
            f[i] = f[i - 1] + f[i - 2];
        }

        int result = 0;
        int prevBit = 0;  // 前一位是否为 1

        for (int i = 30; i >= 0; i--) {
            if (n & (1 << i)) {
                // 当前位是 1，选 0 的情况：后续 i 位自由
                result += f[i];
                if (prevBit) {
                    // 前一位也是 1，n 的这个前缀已经有连续 1
                    // n 本身不合法，直接返回
                    return result;
                }
                prevBit = 1;
            } else {
                prevBit = 0;
            }
        }

        // n 本身没有连续 1，也是合法的
        return result + 1;
    }
};

// ============================================================
// 解法二：数位 DP 记忆化搜索
// 思路：dfs(pos, prev, tight) 标准数位 DP 框架
// ============================================================
// class Solution {
// public:
//     int memo[32][2];
//     int bits[32];
//     int len;
//
//     int dfs(int pos, int prev, bool tight) {
//         if (pos < 0) return 1;
//         if (!tight && memo[pos][prev] != -1) return memo[pos][prev];
//
//         int limit = tight ? bits[pos] : 1;
//         int result = 0;
//         for (int d = 0; d <= limit; d++) {
//             if (d == 1 && prev == 1) continue;  // 跳过连续 1
//             result += dfs(pos - 1, d, tight && (d == limit));
//         }
//
//         if (!tight) memo[pos][prev] = result;
//         return result;
//     }
//
//     int findIntegers(int n) {
//         memset(memo, -1, sizeof(memo));
//         len = 0;
//         while (n > 0) {
//             bits[len++] = n & 1;
//             n >>= 1;
//         }
//         return dfs(len - 1, 0, true);
//     }
// };

// 【解法对比】
// | 解法                 | 时间     | 空间     | 优势              |
// |--------------------|---------|---------|------------------|
// | 斐波那契+逐位 ⭐     | O(log n)| O(log n)| 代码简洁，无递归   |
// | 数位 DP 记忆化搜索   | O(log n)| O(log n)| 框架通用，可扩展   |

// 【易错点】
// 1. 遍历完所有位后如果没有提前返回，需要 result + 1（n 自身合法）
// 2. f[0]=1, f[1]=2 不是标准斐波那契初值，是无连续 1 计数的初值
// 3. prevBit 检查必须在 result += f[i] 之后，先加后判
// 4. 从 bit 30 开始（int 最多 31 位），不是 bit 31（符号位）

// 【面试追问】
// Q1: 为什么与斐波那契相关 → a(k)=a(k-1)+a(k-2)，最高位分 0/1 讨论
// Q2: 十进制无连续相同数字 → 标准数位 DP，状态加 prevDigit
// Q3: n 到 10^18 → 改 long long，f 数组长度改 64
