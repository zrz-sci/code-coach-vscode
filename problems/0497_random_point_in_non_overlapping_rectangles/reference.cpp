/*
 * 【题目本质】
 * 在多个不重叠矩形覆盖的整数点中等概率随机选一个点。
 * 核心: 按面积(整数点数)加权随机选矩形 + 矩形内均匀随机。
 *
 * 【解法总览】
 * 1. 前缀和 + 二分查找 — 构造 O(n), pick O(log n)
 * 2. 展平索引法 — 同样的复杂度, 用编号反推坐标
 */

// ===================== Solution 1: 前缀和 + 二分查找 =====================
// 构造: O(n)  pick: O(log n)  空间: O(n)
// 核心: 前缀和记录累计点数, 二分定位矩形, 矩形内随机
class Solution1 {
public:
    vector<vector<int>> rects;
    vector<int> prefixSum;
    int total;

    Solution1(vector<vector<int>>& rects) : rects(rects) {
        total = 0;
        for (auto& r : rects) {
            // 整数点数 = (x2-x1+1) * (y2-y1+1)
            total += (r[2] - r[0] + 1) * (r[3] - r[1] + 1);
            prefixSum.push_back(total);
        }
    }

    vector<int> pick() {
        // 随机 [0, total-1], 用 upper_bound 找目标矩形
        int target = rand() % total;
        int idx = upper_bound(prefixSum.begin(), prefixSum.end(), target) - prefixSum.begin();
        auto& r = rects[idx];
        // 在矩形内均匀随机整数坐标
        int x = r[0] + rand() % (r[2] - r[0] + 1);
        int y = r[1] + rand() % (r[3] - r[1] + 1);
        return {x, y};
    }
};

// ===================== Solution 2: 展平索引法 =====================
// 构造: O(n)  pick: O(log n)  空间: O(n)
// 核心: 把所有点编号 0~total-1, 随机编号后反推坐标
class Solution2 {
public:
    vector<vector<int>> rects;
    vector<int> prefixSum;
    int total;

    Solution2(vector<vector<int>>& rects) : rects(rects) {
        total = 0;
        for (auto& r : rects) {
            total += (r[2] - r[0] + 1) * (r[3] - r[1] + 1);
            prefixSum.push_back(total);
        }
    }

    vector<int> pick() {
        int target = rand() % total;
        int idx = upper_bound(prefixSum.begin(), prefixSum.end(), target) - prefixSum.begin();
        auto& r = rects[idx];
        // 计算在当前矩形内的偏移量
        int offset = target - (idx > 0 ? prefixSum[idx - 1] : 0);
        int width = r[2] - r[0] + 1;
        int x = r[0] + offset % width;
        int y = r[1] + offset / width;
        return {x, y};
    }
};

/*
 * 【解法对比】
 * Solution1: 矩形内独立随机 x, y, 代码更简洁
 * Solution2: 用编号反推坐标, 严格均匀(避免 rand()%n 的微小偏差叠加)
 *
 * 【易错点】
 * 1. 整数点数是 (x2-x1+1)*(y2-y1+1), 不是 (x2-x1)*(y2-y1)
 * 2. 二分: target 在 [0,total-1], 用 upper_bound 找第一个 > target 的
 * 3. rand() 的随机性可能不够好, 生产环境用 mt19937
 *
 * 【面试追问】
 * Q1: 矩形重叠? — 需要容斥原理或坐标压缩去重
 * Q2: 如何验证等概率? — 大量采样 + 卡方检验
 * Q3: 动态增删矩形? — 线段树/BIT 维护前缀和
 */
