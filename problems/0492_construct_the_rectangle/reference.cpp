/*
 * 【题目本质】
 * 给定面积 area，找因子对 (L, W) 使得 L*W = area, L >= W, 且 L-W 最小。
 *
 * 【解法总览】
 * 1. 从 sqrt 向下枚举 — O(sqrt(n)), 找到第一个整除的 W
 * 2. 从 1 向上枚举因子 — O(sqrt(n)), 记录最大的 W <= sqrt(area)
 */

// ===================== Solution 1: 从 sqrt 向下枚举 =====================
// 时间: O(sqrt(n))  空间: O(1)
// 思路: W 从 sqrt(area) 递减，第一个能整除的就是最优解
class Solution1 {
public:
    vector<int> constructRectangle(int area) {
        int w = sqrt(area);
        while (area % w != 0) {
            w--;
        }
        return {area / w, w};
    }
};

// ===================== Solution 2: 从 1 向上枚举因子 =====================
// 时间: O(sqrt(n))  空间: O(1)
// 思路: 枚举 [1, sqrt(area)] 的因子，最后一个就是最大 W
class Solution2 {
public:
    vector<int> constructRectangle(int area) {
        int w = 1;
        for (int i = 1; (long long)i * i <= area; i++) {
            if (area % i == 0) {
                w = i;
            }
        }
        return {area / w, w};
    }
};

/*
 * 【解法对比】
 * Solution1: 从 sqrt 向下，找到即停，常数更优
 * Solution2: 从 1 向上，需遍历到 sqrt，但逻辑同样清晰
 *
 * 【易错点】
 * 1. 返回顺序是 {L, W} 不是 {W, L}，L >= W
 * 2. i*i 可能溢出 int，用 (long long)i * i 防溢出
 *
 * 【面试追问】
 * Q1: 扩展到三维体积分解？— 枚举最小维度 H，再对 area/H 做二维分解
 * Q2: area 极大（10^18）？— 需要高效因式分解算法（Pollard's rho）
 * Q3: 如何保证所有因子都是质数？— 先筛质数，再枚举质因子对
 */
