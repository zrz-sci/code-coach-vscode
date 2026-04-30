/*
 * ============================================================
 * LeetCode 201: Bitwise AND of Numbers Range (数字范围按位与)
 * 难度: Medium | 标签: Bit Manipulation
 *
 * 【题目本质】
 * [left, right] 范围内所有数按位与的结果
 * 等价于找 left 和 right 的二进制公共前缀，低位补 0
 *
 * 【解法总览】
 * 解法1: 右移找公共前缀 — 最直观，同时右移直到相等
 * 解法2: Brian Kernighan — 不断清除 right 最低位 1
 * 解法3: 位掩码         — 从高位用掩码找公共前缀
 * ============================================================
 */

// ==================== 解法1: 右移找公共前缀（推荐） ====================
// 思路：left 和 right 同时右移，直到相等（即公共前缀）
//       记录移动次数，最后左移回来补零
// 时间 O(log n)，空间 O(1)
class Solution1 {
public:
    int rangeBitwiseAnd(int left, int right) {
        int shift = 0;
        // 不断右移直到 left == right
        while (left < right) {
            left >>= 1;
            right >>= 1;
            shift++;
        }
        // 公共前缀左移回原位
        return left << shift;
    }
};

// ==================== 解法2: Brian Kernighan ====================
// 思路：每次用 right & (right - 1) 清除 right 的最低位 1
//       当 right <= left 时，right 就是公共前缀
// 原理：right-1 将最低位 1 及其右边全部翻转，AND 后清除最低 1
// 时间 O(log n)，空间 O(1)
class Solution2 {
public:
    int rangeBitwiseAnd(int left, int right) {
        while (right > left) {
            // 清除 right 的最低位 1
            right &= (right - 1);
        }
        return right;
    }
};

// ==================== 解法3: 位掩码 ====================
// 思路：用全1掩码从高位开始，逐步左移忽略低位
//       直到 left 和 right 在掩码范围内相等
// 时间 O(32)，空间 O(1)
class Solution3 {
public:
    int rangeBitwiseAnd(int left, int right) {
        unsigned int mask = ~0;  // 0xFFFFFFFF，全1
        while ((left & mask) != (right & mask)) {
            mask <<= 1;  // 左移掩码，屏蔽更多低位
        }
        return left & mask;
    }
};

/*
 * ============================================================
 * 【解法对比】
 * | 解法            | 时间     | 空间 | 循环次数        | 推荐 |
 * |----------------|----------|------|----------------|------|
 * | 右移公共前缀     | O(log n) | O(1) | 最多 31 次      | ★★★  |
 * | Brian Kernighan | O(log n) | O(1) | right 的 1 个数  | ★★★  |
 * | 位掩码          | O(32)    | O(1) | 最多 32 次      | ★★   |
 *
 * 【易错点】
 * 1. 暴力循环 O(n) 会超时，n 最大 2^31
 * 2. 位移时注意 int 溢出问题
 * 3. left == right == 0 时要正确返回 0（所有解法自动处理）
 * 4. Brian Kernighan 的条件是 right > left，不是 right != left
 *
 * 【面试追问】
 * Q1: 为什么低位一定为 0？
 *     → 公共前缀以下的位在 [left,right] 范围内必然出现 0 和 1
 * Q2: n & (n-1) 的经典用途？
 *     → 清除最低位 1、判断 2 的幂、计数 1 的个数
 * Q3: 如果改成按位或（OR），结果是什么？
 *     → 需要不同策略，OR 会保留所有出现过的 1
 * ============================================================
 */
