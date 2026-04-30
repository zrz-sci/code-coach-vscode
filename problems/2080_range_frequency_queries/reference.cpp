/*
 * LeetCode 2080: 区间内查询数字的频率
 * Range Frequency Queries
 *
 * 难度: Medium
 * 标签: Design, Segment Tree, Array, Hash Table, Binary Search
 *
 * =============================================
 * 解法一 (推荐): 哈希表 + 二分查找
 * =============================================
 *
 * 核心思路:
 *   倒排索引 —— 对每个值记录其出现的所有下标位置。
 *   下标天然递增, 查询 [left, right] 中 value 的频率
 *   等价于: value 的下标列表中, 落在 [left, right] 范围的元素个数。
 *   用 lower_bound + upper_bound 二分即可。
 *
 * 构造: O(n)
 * 查询: O(log n)
 * 空间: O(n)
 *
 * 示例推演:
 *   arr = [12, 33, 4, 56, 22, 2, 34, 33, 22, 12, 34, 56]
 *
 *   倒排索引:
 *     12 → [0, 9]
 *     33 → [1, 7]
 *     4  → [2]
 *     56 → [3, 11]
 *     22 → [4, 8]
 *     2  → [5]
 *     34 → [6, 10]
 *
 *   query(1, 2, 4):
 *     4 的下标: [2]
 *     lower_bound([2], 1) → 指向 2 (位置0)
 *     upper_bound([2], 2) → 指向 end (位置1)
 *     答案 = 1 - 0 = 1 ✓
 *
 *   query(0, 11, 33):
 *     33 的下标: [1, 7]
 *     lower_bound([1,7], 0) → 指向 1 (位置0)
 *     upper_bound([1,7], 11) → 指向 end (位置2)
 *     答案 = 2 - 0 = 2 ✓
 */
class RangeFreqQuery {
    // 倒排索引: value → 该值出现的所有下标 (有序)
    unordered_map<int, vector<int>> idx;

public:
    RangeFreqQuery(vector<int>& arr) {
        // O(n): 遍历数组, 每个值追加当前下标
        for (int i = 0; i < (int)arr.size(); i++) {
            idx[arr[i]].push_back(i);
            // push_back 保证每个列表按下标递增
        }
    }

    int query(int left, int right, int value) {
        // 先检查 value 是否存在于数组中
        // 用 find() 而非 [] 避免自动插入空 vector
        auto it = idx.find(value);
        if (it == idx.end()) return 0;

        auto& v = it->second; // 引用, 避免拷贝

        // lower_bound: 第一个 >= left 的迭代器 (包含 left)
        auto lo = lower_bound(v.begin(), v.end(), left);
        // upper_bound: 第一个 > right 的迭代器 (排除 right 之后)
        auto hi = upper_bound(v.begin(), v.end(), right);

        // [lo, hi) 区间内的元素数量即为频率
        return (int)(hi - lo);
    }
};


/*
 * =============================================
 * 解法一变体: 用定长数组代替 unordered_map
 * =============================================
 *
 * 由于 arr[i] <= 10^4, 可以用 vector<vector<int>> 大小 10001
 * 代替 unordered_map, 避免哈希冲突的最坏情况。
 *
 * 构造: O(n)
 * 查询: O(log n) (无哈希开销)
 * 空间: O(n + V), V = 10001
 */
class RangeFreqQuery_Array {
    // 定长数组, 下标即为 value
    static const int MAXV = 10001;
    vector<vector<int>> idx;

public:
    RangeFreqQuery_Array(vector<int>& arr) : idx(MAXV) {
        for (int i = 0; i < (int)arr.size(); i++) {
            idx[arr[i]].push_back(i);
        }
    }

    int query(int left, int right, int value) {
        if (value >= MAXV || idx[value].empty()) return 0;

        auto& v = idx[value];
        auto lo = lower_bound(v.begin(), v.end(), left);
        auto hi = upper_bound(v.begin(), v.end(), right);
        return (int)(hi - lo);
    }
};


/*
 * =============================================
 * 解法二 (对比): 暴力遍历
 * =============================================
 *
 * 每次查询遍历 [left, right] 计数。
 * 用于验证正确性, 生产环境 TLE。
 *
 * 构造: O(n)
 * 查询: O(n) — 太慢!
 * 空间: O(n)
 */
class RangeFreqQuery_Brute {
    vector<int> arr;

public:
    RangeFreqQuery_Brute(vector<int>& arr) : arr(arr) {}

    int query(int left, int right, int value) {
        int count = 0;
        for (int i = left; i <= right; i++) {
            if (arr[i] == value) count++;
        }
        return count;
    }
};


/*
 * =============================================
 * 解法三 (进阶): 分块法 — 支持动态修改
 * =============================================
 *
 * 思路:
 *   将数组分成 sqrt(n) 个块, 每块大小 sqrt(n)。
 *   每块维护一个 unordered_map<int, int> 频率表。
 *
 *   查询 [left, right]:
 *   - 边界块: 暴力计数 O(sqrt(n))
 *   - 中间块: 直接查频率表 O(sqrt(n))
 *   - 总查询: O(sqrt(n))
 *
 *   修改 arr[i] = new_val:
 *   - 更新对应块的频率表 O(1)
 *
 * 适用于需要同时支持修改和查询的场景。
 * 本题不需要修改, 所以 解法一 更优。
 *
 * 构造: O(n)
 * 查询: O(sqrt(n))
 * 修改: O(1)
 * 空间: O(n)
 */
class RangeFreqQuery_Block {
    vector<int> arr;
    int block_size;
    int num_blocks;
    // blocks[b][val] = value val 在第 b 个块中的出现次数
    vector<unordered_map<int, int>> blocks;

    int getBlock(int idx) { return idx / block_size; }

public:
    RangeFreqQuery_Block(vector<int>& arr) : arr(arr) {
        int n = arr.size();
        block_size = max(1, (int)sqrt(n));
        num_blocks = (n + block_size - 1) / block_size;
        blocks.resize(num_blocks);

        for (int i = 0; i < n; i++) {
            blocks[getBlock(i)][arr[i]]++;
        }
    }

    int query(int left, int right, int value) {
        int count = 0;
        int lb = getBlock(left), rb = getBlock(right);

        if (lb == rb) {
            // 同一个块内, 暴力计数
            for (int i = left; i <= right; i++) {
                if (arr[i] == value) count++;
            }
        } else {
            // 左边界块: 暴力
            for (int i = left; i < (lb + 1) * block_size; i++) {
                if (arr[i] == value) count++;
            }
            // 中间完整块: 直接查频率表
            for (int b = lb + 1; b < rb; b++) {
                auto it = blocks[b].find(value);
                if (it != blocks[b].end()) count += it->second;
            }
            // 右边界块: 暴力
            for (int i = rb * block_size; i <= right; i++) {
                if (arr[i] == value) count++;
            }
        }
        return count;
    }
};


/*
 * =============================================
 * 二分查找细节图解
 * =============================================
 *
 *   indices of value: [1, 3, 7, 9, 12, 15]
 *   query(left=4, right=13, value):
 *
 *   [1, 3, 7, 9, 12, 15]
 *          ^         ^
 *          lo        hi
 *   lower_bound(4) → 指向 7 (第一个 >= 4)
 *   upper_bound(13) → 指向 15 (第一个 > 13)
 *   count = hi - lo = 4 - 2 = 2  (即 7, 9 两个... 不对)
 *   实际: 7, 9, 12 三个在 [4, 13] 范围内
 *   hi 指向 index=5 (值15), lo 指向 index=2 (值7)
 *   count = 5 - 2 = 3 ✓
 *
 *   边界case: query(left=0, right=0, value):
 *   如果 value 的下标列表是 [0, 5, 10]:
 *   lower_bound(0) → 指向 0 (位置0)
 *   upper_bound(0) → 指向 5 (位置1)
 *   count = 1 - 0 = 1 ✓ (值确实出现在下标0)
 *
 * =============================================
 * 测试验证
 * =============================================
 *
 * arr = [12, 33, 4, 56, 22, 2, 34, 33, 22, 12, 34, 56]
 *
 * query(1, 2, 4) → 1 ✓
 *   4 的下标: [2], lower_bound(1)→0, upper_bound(2)→1, 1-0=1
 *
 * query(0, 11, 33) → 2 ✓
 *   33 的下标: [1, 7], lower_bound(0)→0, upper_bound(11)→2, 2-0=2
 *
 * query(0, 0, 12) → 1
 *   12 的下标: [0, 9], lower_bound(0)→0, upper_bound(0)→1, 1-0=1
 *
 * query(5, 5, 99) → 0
 *   99 不在 idx 中 → 直接返回 0
 */


// =====================================================
// 总结
// =====================================================
/*
 * ── 解法对比 ──
 *
 * | 解法                   | 构造   | 查询      | 空间      | 支持修改 | 推荐度 |
 * |------------------------|--------|-----------|-----------|----------|--------|
 * | 哈希表+二分 (推荐)     | O(n)   | O(log n)  | O(n)      | 否       | ★★★ 面试首选 |
 * | 定长数组+二分 (变体)   | O(n)   | O(log n)  | O(n+V)    | 否       | ★★★ 值域小时更快 |
 * | 暴力遍历               | O(n)   | O(n)      | O(n)      | 是       | ★   验证正确性 |
 * | 分块法                 | O(n)   | O(sqrt n) | O(n)      | O(1)    | ★★  需要动态修改时 |
 *
 * 哈希表+二分: 本题最优解。倒排索引天然有序, 查询只需两次二分。
 * 定长数组:    值域 <= 10^4 时可用, 省去哈希开销, 常数更小。
 * 分块法:      查询慢于二分, 但唯一支持 O(1) 单点修改的方案。
 *
 * ── 易错点 (具体bug) ──
 *
 * 1. 用 idx[value] 代替 idx.find(value) 检查是否存在:
 *    BUG: operator[] 对不存在的 key 会自动插入空 vector,
 *    导致 unordered_map 膨胀, 在大量查询不存在的 value 时
 *    内存持续增长, 最终 MLE 或显著减速。
 *
 * 2. lower_bound 和 upper_bound 混淆:
 *    BUG: 两个都用 lower_bound → 漏算 right 恰好等于某个下标的情况。
 *    正确: lo = lower_bound(left), hi = upper_bound(right), 答案 = hi - lo。
 *    或: lo = lower_bound(left), hi = lower_bound(right + 1), 等价。
 *
 * 3. 拷贝 vector 而非使用引用:
 *    BUG: auto v = it->second; 每次查询拷贝整个下标列表。
 *    若某值出现 10^5 次, 每次查询拷贝 10^5 个 int → TLE。
 *    正确: auto& v = it->second; (加引用)。
 *
 * 4. 分块法中 getBlock 在边界计算错误:
 *    BUG: 左边界块暴力终点写成 (lb+1)*block_size 但未与 right 取 min,
 *    当 left 和 right 在同一块时会越界扫描到下一块。
 *
 * ── 面试追问链 ──
 *
 * Q1: 如果需要支持单点修改 arr[i] = new_val, 怎么做?
 * A1: 分块法: 更新 arr[i] 所在块的频率表, O(1) 修改 + O(sqrt n) 查询。
 *     或者用 BIT/线段树按值域维护, 但实现复杂度更高。
 *
 * Q2: 如果查询改为"区间内出现次数最多的值", 怎么做?
 * A2: 这是经典的区间众数问题 (Range Mode Query), 没有已知的
 *     O(n) 预处理 + O(1) 查询方案。常见做法:
 *     - 分块 + 预处理块间众数: O(n * sqrt(n)) 预处理, O(sqrt n) 查询
 *     - Mo's Algorithm: O((n + q) * sqrt(n))
 *
 * Q3: 如果值域很大 (arr[i] <= 10^9), 倒排索引方案需要改吗?
 * A3: 不需要改。unordered_map 本身就支持大值域, 空间仍为 O(n),
 *     因为只存储出现过的值。但定长数组方案就不适用了。
 *     如果担心哈希冲突, 可以先离散化再用定长数组。
 */
