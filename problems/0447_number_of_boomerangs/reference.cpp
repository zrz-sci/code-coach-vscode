/*
 * 【题目本质】
 * 对每个点 i，找到距离相同的点对 (j, k)，有序排列计数。
 * 核心：哈希表统计距离 + 排列计数。
 *
 * 【解法总览】
 * Solution1: 哈希表统计距离 O(n²)/O(n) ⭐ 最优解
 * Solution2: 暴力三重循环 O(n³)/O(1) 超时
 */

// ===================== Solution 1: 哈希表统计距离 =====================
// 固定中心点 i，统计到其他所有点的距离平方
// 距离相同的 cnt 个点，有序选 2 个：cnt * (cnt - 1)
class Solution1 {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        int res = 0;
        for (auto& p : points) {
            unordered_map<int, int> cnt;
            for (auto& q : points) {
                int dx = p[0] - q[0], dy = p[1] - q[1];
                cnt[dx * dx + dy * dy]++;
            }
            for (auto& [d, c] : cnt) {
                res += c * (c - 1); // 排列 P(c, 2)
            }
        }
        return res;
    }
};

// ===================== Solution 2: 暴力枚举 =====================
// 三重循环枚举 (i, j, k)，检查 dist(i,j) == dist(i,k)
// 时间 O(n³)，n=500 时约 1.25 亿次，会超时
class Solution2 {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        int res = 0, n = points.size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                for (int k = j + 1; k < n; k++) {
                    if (i == k) continue;
                    int d1 = (points[i][0] - points[j][0]) * (points[i][0] - points[j][0])
                           + (points[i][1] - points[j][1]) * (points[i][1] - points[j][1]);
                    int d2 = (points[i][0] - points[k][0]) * (points[i][0] - points[k][0])
                           + (points[i][1] - points[k][1]) * (points[i][1] - points[k][1]);
                    if (d1 == d2) res += 2; // j,k 可互换
                }
            }
        }
        return res;
    }
};

/*
 * 【解法对比】
 * Solution1: O(n²) 时间，O(n) 空间，利用哈希表一次统计
 * Solution2: O(n³) 时间，O(1) 空间，暴力但超时
 *
 * 【易错点】
 * 1. 用 sqrt 会引入浮点误差，应使用距离平方
 * 2. 排列而非组合：cnt*(cnt-1) 不是 cnt*(cnt-1)/2
 * 3. 哈希表每次固定新的 i 时需要重建
 *
 * 【面试追问】
 * Q1: 坐标范围 [-10^4, 10^4]，距离平方最大多少？会溢出吗？
 *     A: 最大 (2*10^4)^2 * 2 = 8*10^8，int 范围内安全
 * Q2: 如果点有重复怎么办？题目保证 distinct，但如果允许重复，
 *     距离为 0 的点也会被统计进来，逻辑不变
 * Q3: 如何输出所有回旋镖？在哈希表中记录具体点索引即可
 */
