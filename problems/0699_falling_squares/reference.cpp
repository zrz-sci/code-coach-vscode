// 699. Falling Squares
// 难度: Hard
// 标签: Segment Tree, Array, Ordered Set

/*
 * 题目描述：
 * 方块从高处掉落到 X 轴上。每个方块 [left, sideLength] 表示左边对齐 X 坐标 left、
 * 边长为 sideLength 的正方形。方块落到已有方块顶部或 X 轴上（擦边不算）。
 * 每次掉落后返回当前最高的方块堆叠高度。
 *
 * 核心思路：
 * 暴力 O(N^2)：维护已落方块列表，每次查询重叠区间最大高度。
 * 优化 O(N log N)：坐标压缩 + 线段树区间查询/赋值。
 */

// ==================== 解法一：暴力模拟 O(N^2)（推荐面试首选）====================
// 时间 O(N^2)，空间 O(N)
// 思路：维护已落方块列表，每次遍历检查区间重叠
class Solution {
public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        int n = positions.size();
        vector<int> ans;

        // 每个已落方块: {left, right, height}
        vector<vector<int>> intervals;
        int globalMax = 0;

        for (auto& pos : positions) {
            int left = pos[0];
            int size = pos[1];
            int right = left + size;

            // 查找与 [left, right) 重叠的所有方块的最大高度
            int maxH = 0;
            for (auto& inv : intervals) {
                int l = inv[0], r = inv[1], h = inv[2];
                // 重叠条件：left < r && l < right（严格小于，擦边不算）
                if (left < r && l < right) {
                    maxH = max(maxH, h);
                }
            }

            int curH = maxH + size;
            intervals.push_back({left, right, curH});

            globalMax = max(globalMax, curH);
            ans.push_back(globalMax);
        }

        return ans;
    }
};

// ==================== 解法二：坐标压缩 + 线段树 ====================
// 时间 O(N log N)，空间 O(N)
// 思路：离散化坐标后用线段树维护区间最大高度

class Solution_SegTree {
private:
    // 线段树：支持区间赋值和区间最大值查询
    vector<int> tree;
    vector<int> lazy;
    int sz;

    void build(int n) {
        sz = n;
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }

    void pushDown(int node) {
        if (lazy[node] != 0) {
            // 区间赋值（取 max）下推
            tree[2 * node] = max(tree[2 * node], lazy[node]);
            tree[2 * node + 1] = max(tree[2 * node + 1], lazy[node]);
            lazy[2 * node] = max(lazy[2 * node], lazy[node]);
            lazy[2 * node + 1] = max(lazy[2 * node + 1], lazy[node]);
            lazy[node] = 0;
        }
    }

    // 区间查询最大值 [ql, qr]
    int query(int node, int lo, int hi, int ql, int qr) {
        if (qr < lo || hi < ql) return 0;
        if (ql <= lo && hi <= qr) return tree[node];
        pushDown(node);
        int mid = (lo + hi) / 2;
        return max(query(2 * node, lo, mid, ql, qr),
                   query(2 * node + 1, mid + 1, hi, ql, qr));
    }

    // 区间赋值（设置为 val，取 max 语义）
    void update(int node, int lo, int hi, int ql, int qr, int val) {
        if (qr < lo || hi < ql) return;
        if (ql <= lo && hi <= qr) {
            tree[node] = max(tree[node], val);
            lazy[node] = max(lazy[node], val);
            return;
        }
        pushDown(node);
        int mid = (lo + hi) / 2;
        update(2 * node, lo, mid, ql, qr, val);
        update(2 * node + 1, mid + 1, hi, ql, qr, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        int n = positions.size();

        // Step 1: 坐标压缩
        // 收集所有左端点和右端点
        vector<int> coords;
        for (auto& p : positions) {
            coords.push_back(p[0]);
            coords.push_back(p[0] + p[1]);
        }
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());

        // 坐标 -> 索引 映射
        unordered_map<int, int> coordIdx;
        for (int i = 0; i < (int)coords.size(); i++) {
            coordIdx[coords[i]] = i;
        }

        // Step 2: 建立线段树
        int m = coords.size();
        build(m);

        // Step 3: 逐个处理方块
        vector<int> ans;
        int globalMax = 0;

        for (auto& p : positions) {
            int left = p[0], size = p[1];
            int right = left + size;

            int li = coordIdx[left];
            int ri = coordIdx[right] - 1;  // 左闭右开 -> 右端点索引-1

            // 查询 [li, ri] 区间的最大高度
            int maxH = 0;
            if (li <= ri) {
                maxH = query(1, 0, m - 1, li, ri);
            }

            int curH = maxH + size;

            // 将 [li, ri] 区间更新为 curH
            if (li <= ri) {
                update(1, 0, m - 1, li, ri, curH);
            }

            globalMax = max(globalMax, curH);
            ans.push_back(globalMax);
        }

        return ans;
    }
};

// ==================== 解法三：有序 Map 区间管理 ====================
// 时间 O(N log N)，空间 O(N)
// 思路：用 std::map 维护不重叠的高度区间
class Solution_Map {
public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        // heightMap[x] 表示从坐标 x 开始的高度
        // 每个条目代表一个连续区间的开始
        map<int, int> heightMap;
        heightMap[0] = 0;  // 初始：整条 X 轴高度为 0

        vector<int> ans;
        int globalMax = 0;

        for (auto& pos : positions) {
            int left = pos[0];
            int size = pos[1];
            int right = left + size;

            // 找出 [left, right) 区间内的最大高度
            int maxH = 0;

            // 找到第一个 >= left 的迭代器
            auto it = heightMap.upper_bound(left);
            if (it != heightMap.begin()) {
                --it;  // 可能覆盖到的前一个区间
            }

            // 遍历所有可能重叠的区间
            while (it != heightMap.end() && it->first < right) {
                maxH = max(maxH, it->second);
                ++it;
            }

            int curH = maxH + size;

            // 记录 right 处的原始高度（如果还没有记录）
            auto rightIt = heightMap.upper_bound(right);
            if (rightIt != heightMap.begin()) {
                --rightIt;
                if (rightIt->first < right) {
                    heightMap[right] = rightIt->second;
                }
            }

            // 删除 (left, right) 范围内的所有条目
            auto eraseStart = heightMap.lower_bound(left);
            auto eraseEnd = heightMap.lower_bound(right);
            heightMap.erase(eraseStart, eraseEnd);

            // 设置新区间 [left, right) 的高度
            heightMap[left] = curH;

            globalMax = max(globalMax, curH);
            ans.push_back(globalMax);
        }

        return ans;
    }
};

/*
 * 复杂度分析：
 * - 解法一: O(N^2) 时间，O(N) 空间 -- N<=1000 完全够用
 * - 解法二: O(N log N) 时间，O(N) 空间 -- 线段树最优
 * - 解法三: O(N log N) 时间，O(N) 空间 -- map 区间管理
 *
 * 关键要点：
 * 1. 区间重叠判断: l1 < r2 && l2 < r1（严格小于，擦边不算）
 * 2. 高度累加: 新方块高度 = 重叠区间最大高度 + sideLength
 * 3. 输出全局最大值，不是当前方块高度
 * 4. 坐标压缩: 坐标范围 10^8 但只有 2N 个不同值
 * 5. 线段树需要 lazy propagation 支持区间赋值
 * 6. 面试中暴力解法优先，时间允许再优化到线段树
 */
