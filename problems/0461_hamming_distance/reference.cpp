// 【题目本质】统计两整数 XOR 结果中 1 的个数（popcount）
// 【解法总览】解法1: XOR+逐位统计 O(32)/O(1) | 解法2: XOR+Kernighan O(k)/O(1)

// ===================== Solution 1: XOR + 逐位统计 =====================
// 思路：XOR 标记不同位，逐位检查最低位是否为 1
// 时间 O(32)，空间 O(1)
class Solution1 {
public:
    int hammingDistance(int x, int y) {
        int xorVal = x ^ y;
        int count = 0;
        while (xorVal) {
            count += xorVal & 1;
            xorVal >>= 1;
        }
        return count;
    }
};

// ===================== Solution 2: XOR + Brian Kernighan =====================
// 思路：n &= (n-1) 每次消除最低位的 1，循环次数即为 1 的个数
// 时间 O(k)，k 为 1 的个数，空间 O(1)
class Solution2 {
public:
    int hammingDistance(int x, int y) {
        int xorVal = x ^ y;
        int count = 0;
        while (xorVal) {
            xorVal &= (xorVal - 1);
            count++;
        }
        return count;
    }
};

// 【解法对比】
// Solution1: 固定循环32次（最坏），实现最简单
// Solution2: 循环次数 = 1的个数，稀疏时更快；面试更出彩

// 【易错点】
// 1. 用 AND 而非 XOR 找不同位 -> XOR 才是找差异的运算
// 2. Kernighan 写成 n &= (n+1) -> 必须是 n-1

// 【面试追问】
// Q1: Kernighan 为什么能消除最低位的1？
//     -> n-1 翻转最低位1及其右边所有位，AND 后只消去该位
// Q2: 两两汉明距离总和(LC477)？
//     -> 按位统计，第k位有c个1则贡献 c*(n-c)
// Q3: __builtin_popcount 底层实现？
//     -> 通常用查表或 POPCNT 指令，O(1)
