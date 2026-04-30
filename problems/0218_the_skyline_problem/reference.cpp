/*
 * LeetCode 218: 天际线问题 (The Skyline Problem)
 *
 * 【题目本质】
 * 给定 n 栋建筑物 [left, right, height]，求从远处观察时的天际线轮廓关键点。
 * 关键点就是天际线高度发生变化的 (x, height) 位置。
 *
 * 【核心观察】
 * 天际线高度只可能在"某栋建筑开始"或"某栋建筑结束"时变化。
 * 所以只需处理所有建筑的左右边缘（事件），用数据结构维护当前最大高度。
 *
 * 【解法总览】
 * 解法1: 扫描线 + multiset          — O(n log n) / O(n) — 面试首选 ⭐
 * 解法2: 扫描线 + 懒删除堆          — O(n log n) / O(n) — 变体写法
 * 解法3: 分治 + 合并天际线          — O(n log n) / O(n) — 理解分治
 */

/*
 * 事件排序技巧可视化:
 *
 * 建筑 [2,9,10] → 事件 (2,-10) 和 (9,10)
 * 建筑 [3,7,15] → 事件 (3,-15) 和 (7,15)
 * 建筑 [5,12,12] → 事件 (5,-12) 和 (12,12)
 *
 * 排序后: (2,-10) (3,-15) (5,-12) (7,15) (9,10) (12,12)
 *
 * 左边缘取负的妙处:
 * - 同x两个进入事件: (-15 < -10) → 高的先处理 ✓
 * - 同x一进一出: (-h < +h) → 先进入再离开 ✓
 * - 同x两个离开事件: (10 < 15) → 矮的先处理 ✓
 *
 * 扫描过程:
 *
 *   heights     操作         multiset           curMax  prevMax  输出
 *   ─────────────────────────────────────────────────────────────
 *   初始         -           {0}                 0       -        -
 *   (2,-10)     +10         {0,10}              10      0        [2,10]
 *   (3,-15)     +15         {0,10,15}           15      10       [3,15]
 *   (5,-12)     +12         {0,10,12,15}        15      15       (跳过)
 *   (7, 15)     -15         {0,10,12}           12      15       [7,12]
 *   (9, 10)     -10         {0,12}              12      12       (跳过)
 *   (12, 12)    -12         {0}                 0       12       [12,0]
 */

// ============================================================
// 解法1: 扫描线 + multiset ⭐ 面试首选
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 1. 每栋建筑拆为两个事件: 左边缘(x, -h) + 右边缘(x, h)
// 2. 所有事件按 (x, h) 排序 (负高度技巧保证同x正确顺序)
// 3. multiset 维护当前活跃建筑高度, 初始含 {0} (地面)
// 4. 遍历事件: 进入→加入, 离开→删除(只删一个!)
// 5. 每次操作后检查最大高度是否变化 → 变化则输出关键点
//
// 【为什么用 multiset】
// priority_queue 不支持删除任意元素。
// 建筑结束时需要精确移除该高度, multiset.erase(find(h)) O(log n)。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        // 第一步: 提取事件
        // 左边缘: (x, -height) → 负号是排序技巧
        // 右边缘: (x,  height) → 正号
        vector<pair<int, int>> events;
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]}); // 进入事件
            events.push_back({b[1], b[2]});   // 离开事件
        }

        // pair 默认按 first 升序, first 相同按 second 升序
        // 这正好满足: 同x时 (-15 < -10 < 10 < 15)
        sort(events.begin(), events.end());

        // 第二步: 扫描线
        multiset<int> heights;
        heights.insert(0); // 地面高度, 保证 rbegin() 始终有效
        int prevMax = 0;
        vector<vector<int>> result;

        for (auto& [x, h] : events) {
            if (h < 0) {
                // 进入事件: 加入高度 (-h 还原为正)
                heights.insert(-h);
            } else {
                // 离开事件: 移除该高度 (只删一个!)
                // 注意: heights.erase(h) 会删除所有等于h的元素!
                // 必须用 heights.erase(heights.find(h)) 只删一个
                heights.erase(heights.find(h));
            }

            // 当前最大高度 (multiset 有序, rbegin 是最大值)
            int curMax = *heights.rbegin();
            if (curMax != prevMax) {
                result.push_back({x, curMax});
                prevMax = curMax;
            }
        }

        return result;
    }
};

// ============================================================
// 解法2: 扫描线 + 懒删除堆
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 和解法1框架相同, 区别在于用 priority_queue 替代 multiset。
// 因为堆不支持删除任意元素, 采用"懒删除":
// - 离开事件时不立即从堆中移除, 而是记在 toRemove 计数器中
// - 每次取堆顶前, 检查堆顶是否在待删除列表中, 是则弹出
//
// 【和解法1的权衡】
// - multiset: 代码更简洁, erase(find(h)) 一行搞定
// - 懒删除堆: 常数更小(堆比红黑树快), 但逻辑稍复杂
// ============================================================
class Solution2 {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int, int>> events;
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]});
            events.push_back({b[1], b[2]});
        }
        sort(events.begin(), events.end());

        priority_queue<int> maxHeap;       // 最大堆
        unordered_map<int, int> toRemove;  // 高度 → 待删除次数
        maxHeap.push(0);
        int prevMax = 0;
        vector<vector<int>> result;

        for (auto& [x, h] : events) {
            if (h < 0) {
                maxHeap.push(-h);
            } else {
                // 标记删除, 不立即移除
                toRemove[h]++;
            }

            // 懒删除: 堆顶若在待删除列表中, 弹出
            while (!maxHeap.empty() && toRemove.count(maxHeap.top())
                   && toRemove[maxHeap.top()] > 0) {
                toRemove[maxHeap.top()]--;
                if (toRemove[maxHeap.top()] == 0)
                    toRemove.erase(maxHeap.top());
                maxHeap.pop();
            }

            int curMax = maxHeap.top();
            if (curMax != prevMax) {
                result.push_back({x, curMax});
                prevMax = curMax;
            }
        }

        return result;
    }
};

// ============================================================
// 解法3: 分治 + 合并天际线
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 类似归并排序:
// 1. 将建筑群一分为二
// 2. 递归求左半和右半各自的天际线
// 3. 合并两个天际线 (类似 merge sort 的 merge 步骤)
//
// 合并两个天际线:
// - 用两个指针分别扫描左右天际线
// - 每次取 x 更小的那个点, 高度取 max(左侧当前高度, 右侧当前高度)
// - 去除连续相同高度的重复点
//
// 【base case】
// 单栋建筑: 天际线就是 [[left, height], [right, 0]]
// ============================================================
class Solution3 {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        if (buildings.empty()) return {};
        return solve(buildings, 0, buildings.size() - 1);
    }

private:
    vector<vector<int>> solve(vector<vector<int>>& buildings,
                              int lo, int hi) {
        // base case: 单栋建筑
        if (lo == hi) {
            return {{buildings[lo][0], buildings[lo][2]},
                    {buildings[lo][1], 0}};
        }

        int mid = lo + (hi - lo) / 2;
        auto left = solve(buildings, lo, mid);
        auto right = solve(buildings, mid + 1, hi);
        return merge(left, right);
    }

    // 合并两个天际线 (类似归并排序的 merge)
    vector<vector<int>> merge(vector<vector<int>>& left,
                              vector<vector<int>>& right) {
        vector<vector<int>> result;
        int i = 0, j = 0;
        int lh = 0, rh = 0; // 左右侧"当前高度"

        while (i < (int)left.size() && j < (int)right.size()) {
            int x, maxH;
            if (left[i][0] < right[j][0]) {
                // 左边的 x 更小 → 处理左天际线的点
                x = left[i][0];
                lh = left[i][1]; // 更新左侧当前高度
                i++;
            } else if (left[i][0] > right[j][0]) {
                // 右边的 x 更小 → 处理右天际线的点
                x = right[j][0];
                rh = right[j][1]; // 更新右侧当前高度
                j++;
            } else {
                // 同一个 x → 同时处理
                x = left[i][0];
                lh = left[i][1];
                rh = right[j][1];
                i++;
                j++;
            }

            // 合并后的高度 = max(左当前, 右当前)
            maxH = max(lh, rh);

            // 去重: 避免连续相同高度
            if (result.empty() || result.back()[1] != maxH) {
                result.push_back({x, maxH});
            }
        }

        // 收尾: 剩余的左侧/右侧点
        while (i < (int)left.size()) {
            if (result.empty() || result.back()[1] != left[i][1])
                result.push_back(left[i]);
            i++;
        }
        while (j < (int)right.size()) {
            if (result.empty() || result.back()[1] != right[j][1])
                result.push_back(right[j]);
            j++;
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 核心数据结构 | 删除操作 | 代码量 | 推荐场景 |
// |------|------------|---------|--------|---------|
// | 1.multiset ⭐ | 有序集 | erase(find(h)) | 最少 | 面试首选 |
// | 2.懒删除堆 | priority_queue | 标记+延迟 | 中等 | 理解变体 |
// | 3.分治合并 | 递归+双指针 | 不需要 | 最多 | 分治思想 |
//
// 面试推荐: 直接写解法1, 追问堆的做法再说解法2, 追问分治再说解法3。
//
// 【易错点】
// 1. erase 删除所有等值元素:
//    ✗ heights.erase(h);           // 删除所有高度为 h 的条目!
//    ✓ heights.erase(heights.find(h)); // 只删除一个
//    当多栋建筑同高时, 前者会导致高度被提前全部移除。
//
// 2. 忘记在 multiset 中放入 0:
//    heights 为空时 *rbegin() 是未定义行为。
//    0 也确保最后一栋建筑结束时能输出 [x, 0] 关键点。
//
// 3. 同 x 坐标的事件顺序:
//    不用负高度技巧 → 需要自定义排序处理三种 case。
//    例: 建筑 [2,5,10] 和 [5,8,10], x=5 处:
//    错误: 先移除10再加入10 → 高度短暂为0 → 虚假关键点
//    正确: 先加入10再移除10 → 高度不变 → 无关键点
//
// 4. 分治合并遗漏去重:
//    ✗ result.push_back({x, maxH});  // 可能连续相同高度
//    ✓ if (result.empty() || result.back()[1] != maxH)
//          result.push_back({x, maxH});
//
// 【面试追问】
// Q1: 为什么左边缘取负高度?
//    → 利用 pair 默认排序: (-15 < -10) 高的先处理;
//      (-h < +h) 进入先于离开; 一行 sort 搞定三种 case。
//
// Q2: 为什么用 multiset 不用 priority_queue?
//    → multiset 支持 erase(find(h)) 精确删除。堆只能弹堆顶,
//      需要懒删除, 代码更复杂。
//
// Q3: 建筑数量极大 (10^6) 时怎么优化?
//    → O(n log n) 已是最优。若坐标离散化后范围小,
//      可用线段树区间更新 + 扫描, 但通常没必要。
//
// Q4(变体): 求天际线下的面积?
//    → 扫描过程中累加 prevMax * (x - prevX)。
// ============================================================
