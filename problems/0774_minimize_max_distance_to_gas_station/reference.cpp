// 774. Minimize Max Distance to Gas Station
// 难度: Hard
// 解法: 二分答案 (Binary Search on Answer)
// 时间复杂度: O(n * log(M / eps)), M=最大间距, eps=10^-6
// 空间复杂度: O(1)

// ============================================================
// 解法一: 二分答案 (最优解法, 面试首选)
// ============================================================
// 核心: 对"最大距离"做二分搜索
// 可行性检验: 给定最大距离 d, 计算需要多少个新加油站
// 公式: 区间 gap 需要 (int)(gap / d) 个新站

class Solution {
public:
    double minmaxGasDist(vector<int>& stations, int k) {
        int n = stations.size();

        // 二分范围: [0, 最大间距]
        double left = 0, right = 0;
        for (int i = 1; i < n; i++) {
            right = max(right, (double)(stations[i] - stations[i - 1]));
        }

        // 浮点二分, 固定迭代 100 次保证精度
        // 2^100 >> 10^14, 远超 10^-6 的精度要求
        for (int iter = 0; iter < 100; iter++) {
            double mid = (left + right) / 2.0;

            // 可行性检验: 最大距离为 mid 时需要多少个新站
            int need = 0;
            for (int i = 1; i < n; i++) {
                double gap = stations[i] - stations[i - 1];
                // 该区间需要切成 ceil(gap/mid) 段
                // 需要添加 ceil(gap/mid) - 1 个新站
                // 等价于 (int)(gap / mid) 个新站
                // (当 gap 恰好整除 mid 时少一个, 但浮点精度下很少出现)
                need += (int)(gap / mid);
            }

            if (need <= k) {
                // 可行: mid 够大, 尝试更小的距离
                right = mid;
            } else {
                // 不可行: mid 太小, 需要更多的站
                left = mid;
            }
        }

        return left;
    }
};

// ============================================================
// 解法二: 优先队列贪心
// ============================================================
// 每次取出最大间距的区间, 多切一刀
// 适合 k 较小的场景, k 很大时性能不如二分

class Solution2 {
public:
    double minmaxGasDist(vector<int>& stations, int k) {
        int n = stations.size();

        // 优先队列存储: (当前最大子段长度, {原始间距, 切分段数})
        // 用最大堆: 每次取出最大子段的区间
        priority_queue<pair<double, pair<double, int>>> pq;

        for (int i = 1; i < n; i++) {
            double gap = stations[i] - stations[i - 1];
            if (gap > 0) {
                // 初始: 1段, 最大子段长度 = 原始间距
                pq.push({gap, {gap, 1}});
            }
        }

        // 贪心: 每次对最大子段的区间多切一刀
        for (int i = 0; i < k; i++) {
            auto [curMax, info] = pq.top();
            pq.pop();
            auto [origGap, cnt] = info;

            cnt++;  // 多切一刀, 段数+1
            double newMax = origGap / cnt;  // 均匀切分后的最大子段
            pq.push({newMax, {origGap, cnt}});
        }

        return pq.top().first;
    }
};

// ============================================================
// 解法一变体: 精度终止条件 + long long 防溢出
// ============================================================
class Solution3 {
public:
    double minmaxGasDist(vector<int>& stations, int k) {
        int n = stations.size();

        double left = 0, right = 0;
        for (int i = 1; i < n; i++) {
            right = max(right, (double)(stations[i] - stations[i - 1]));
        }

        // 精度终止条件
        while (right - left > 1e-6) {
            double mid = left + (right - left) / 2.0;

            // 用 long long 防止大量小区间求和时溢出
            long long need = 0;
            for (int i = 1; i < n; i++) {
                need += (long long)((stations[i] - stations[i - 1]) / mid);
            }

            if (need <= k) {
                right = mid;
            } else {
                left = mid;
            }
        }

        return left;
    }
};

// ============================================================
// 要点总结:
// 1. 经典"二分答案"模式: 对结果空间二分 + 可行性检验
// 2. 可行性检验: gap/d 向上取整减1 = 需要的新站数
// 3. 浮点二分用固定迭代次数(100次)最稳健
// 4. 优先队列解法 O((n+k)logn), k大时劣于二分 O(n*47)
// 5. 同类题: 875(整数二分), 1011, 410
// ============================================================
