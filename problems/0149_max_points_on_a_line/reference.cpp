/*
 * 【题目本质】
 * 给定平面上的点集，求共线的最多点数。
 * 核心是"枚举基准点 + 斜率哈希分组"，关键难点是精确表示斜率。
 *
 * 【解法总览】
 * 1. 哈希表 + GCD约分斜率: O(n^2) 时间, O(n) 空间 — 最优解
 * 2. 暴力叉积验证:         O(n^3) 时间, O(1) 空间 — 简洁但慢
 *
 * 【斜率表示图示】
 *
 *   点 (1,1) 到 (3,2): dx=2, dy=1, gcd=1 → key=(2,1)
 *   点 (1,1) 到 (5,3): dx=4, dy=2, gcd=2 → key=(2,1) ← 同一斜率!
 *
 *   y
 *   4 |         * (1,4)
 *   3 |     * (2,3)     * (5,3)
 *   2 |         * (3,2)
 *   1 | * (1,1)     * (4,1)
 *   0 +---+---+---+---+---→ x
 *     0   1   2   3   4   5
 *
 *   经过 (1,1) 斜率为 (2,1) 的直线: (1,1)-(3,2)-(5,3) → 3点共线
 */

// ===================== 解法1: 哈希表 + GCD约分斜率 =====================
// 思路: 固定基准点，斜率相同的点必然共线
// 为什么用约分整数对而非double? 避免浮点精度问题
class Solution1 {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;

        int ans = 2;
        for (int i = 0; i < n; i++) {
            // 每换一个基准点就清空斜率计数
            map<pair<int, int>, int> slopeCount;
            for (int j = i + 1; j < n; j++) {
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];

                // 用GCD约分到最简
                int g = __gcd(abs(dx), abs(dy));
                dx /= g;
                dy /= g;

                // 为什么要规范化符号?
                // (-1,2) 和 (1,-2) 表示同一斜率，必须统一
                if (dx < 0) { dx = -dx; dy = -dy; }
                // 垂直线: dx=0 时，dy 统一为正
                if (dx == 0) dy = abs(dy);

                slopeCount[{dx, dy}]++;
            }

            for (auto& [slope, cnt] : slopeCount) {
                // 为什么 +1? 因为 cnt 只计了"与基准点共线的其他点"，
                // 需要加上基准点自身
                ans = max(ans, cnt + 1);
            }
        }
        return ans;
    }
};

// ===================== 解法2: 暴力叉积判共线 =====================
// 思路: 枚举两个点确定一条直线，检查所有其他点是否在该直线上
// 为什么用叉积? 三点共线 ⟺ 向量叉积为0，完全避免除法
class Solution2 {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;

        int ans = 2;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int count = 2; // i 和 j 两点
                for (int k = j + 1; k < n; k++) {
                    // 叉积: (P_j - P_i) x (P_k - P_i)
                    // 为什么用 long long? dx 最大 2*10^4, 乘积最大 4*10^8
                    // int 范围足够，但养成好习惯
                    int dx1 = points[j][0] - points[i][0];
                    int dy1 = points[j][1] - points[i][1];
                    int dx2 = points[k][0] - points[i][0];
                    int dy2 = points[k][1] - points[i][1];
                    if ((long long)dx1 * dy2 == (long long)dy1 * dx2) {
                        count++;
                    }
                }
                ans = max(ans, count);
            }
        }
        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 适用场景 |
 * |------|------|------|----------|
 * | 解法1 | O(n^2) | O(n) | n <= 300, 面试首选 |
 * | 解法2 | O(n^3) | O(1) | 简洁验证思路 |
 *
 * 【易错点】
 * 1. 用 double 做斜率 key → 精度问题
 * 2. 约分后未规范化符号 → 同斜率不同 key
 * 3. 忘记 +1 → 漏算基准点
 * 4. gcd(0, x) 需要正确处理 → __gcd(0, x) = x，OK
 *
 * 【面试追问】
 * Q1: 为什么不用 double? → 1/3 ≠ 2/6 的浮点表示可能不同
 * Q2: 点可重复怎么办? → 先统计重合点数 same，答案加 same
 * Q3: 三维空间中的共线? → 用叉积向量是否为零向量判断
 */
