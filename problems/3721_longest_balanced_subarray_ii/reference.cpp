/*
 * LeetCode 3721: Longest Balanced Subarray II (最长平衡子数组 II)
 *
 * 【题目本质】
 * 找最长子数组，使得其中 distinct 偶数个数 == distinct 奇数个数。
 * 关键: 是 distinct count（去重后的），不是总出现次数！
 * [2,2,3] → distinct even=1, distinct odd=1 → 平衡
 *
 * 【为什么经典前缀差不能用？】
 * 普通计数可减: count([l,r]) = prefix[r] - prefix[l-1]
 * distinct 不可减: distinct([l,r]) ≠ prefix_distinct[r] - prefix_distinct[l-1]
 * 反例: [2,3,2], distinct_even([1,2]) = 1, 但 prefix[3]-prefix[1] = 0
 *
 * 【解法总览】
 * 解法1:  O(n^2) 枚举 + unordered_set  — 面试保底 ⭐
 * 解法1b: O(n^2) 枚举 + 频次 map       — 等价变体
 * 解法2:  O(n log n) 线段树             — 竞赛级最优
 */



// ============================================================
// 解法1: O(n^2) 枚举 + unordered_set — 面试保底 ⭐
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 固定左端点 l，右端点 r 从 l 向右扩展。
// 用两个 unordered_set 分别维护窗口内的 distinct 偶数和奇数。
// 每加入 nums[r]，insert 到对应 set（自动去重）。
// 比较两个 set 的 size，相等则更新答案。
//
// 【为什么 set 能自动处理 distinct？】
// unordered_set::insert(v) 如果 v 已存在，size 不变。
// 所以 set.size() 就是 distinct count。
//
// 【演算: nums = [3, 2, 2, 5, 4]】
// l=0:
//   r=0: 3(奇) → e={}, o={3}           → 0≠1
//   r=1: 2(偶) → e={2}, o={3}          → 1==1 ✓ → ans=2
//   r=2: 2(偶,重复) → e={2}, o={3}     → 1==1 ✓ → ans=3
//     ↑ 重复的 2 insert 到 set 后 size 不变！
//   r=3: 5(奇) → e={2}, o={3,5}        → 1≠2
//   r=4: 4(偶) → e={2,4}, o={3,5}      → 2==2 ✓ → ans=5
// 答案: 5
// ============================================================

class Solution {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;

        // 枚举左端点
        for (int l = 0; l < n; l++) {
            unordered_set<int> evens;  // 窗口内的 distinct 偶数
            unordered_set<int> odds;   // 窗口内的 distinct 奇数

            // 右端点向右扩展
            for (int r = l; r < n; r++) {
                // 根据奇偶性插入对应 set
                if (nums[r] % 2 == 0) {
                    evens.insert(nums[r]);  // 重复值不影响 size
                } else {
                    odds.insert(nums[r]);
                }

                // 检查是否平衡: distinct 偶数个数 == distinct 奇数个数
                if (evens.size() == odds.size()) {
                    ans = max(ans, r - l + 1);
                }
            }
        }

        return ans;
    }
};


// ============================================================
// 解法1b: O(n^2) 用频次 map — 如果需要收缩窗口更灵活
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 和解法1相同，但用 map 记录每个值的出现频次。
// 当 freq[v] 从 0→1 时 distinct++，从 1→0 时 distinct--。
// 这种方式支持删除操作（收缩左端点），虽然本题不需要。
//
// 【关键写法】
// if (freq[v]++ == 0) distinct++;  ← 后缀++，先比较再加
//   等价于: if (freq[v] == 0) { distinct++; freq[v]++; }
// ============================================================

class Solution_FreqMap {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;

        for (int l = 0; l < n; l++) {
            unordered_map<int, int> even_freq;  // 偶数值 → 出现次数
            unordered_map<int, int> odd_freq;   // 奇数值 → 出现次数
            int dist_even = 0, dist_odd = 0;    // distinct 计数

            for (int r = l; r < n; r++) {
                if (nums[r] % 2 == 0) {
                    // 偶数: 频次从 0→1 表示新增一个 distinct 值
                    if (even_freq[nums[r]]++ == 0) dist_even++;
                } else {
                    // 奇数: 同理
                    if (odd_freq[nums[r]]++ == 0) dist_odd++;
                }

                if (dist_even == dist_odd) {
                    ans = max(ans, r - l + 1);
                }
            }
        }

        return ans;
    }
};


// ============================================================
// 解法2: O(n log n) 线段树 — 竞赛级最优
// 时间: O(n log n)  空间: O(n)
//
// 【核心思路】
// 枚举右端点 j，维护 diff[i] = distinct_even([i,j]) - distinct_odd([i,j])
// 对于每个 i in [0, j]。
//
// 当 j 扩展到新位置:
//   v = nums[j], prev = last[v] (v 上次出现位置, -1 if 首次)
//
//   if v 是偶数: diff[prev+1 .. j] 全部 += 1
//     含义: 左端点在 [prev+1, j] 范围的子数组，v 首次出现
//     左端点在 [0, prev] 的子数组，[i,j-1] 已包含 v，不增加
//
//   if v 是奇数: diff[prev+1 .. j] 全部 -= 1
//
//   更新 last[v] = j
//
//   查询: diff[i] == 0 的最小 i → ans = max(ans, j - i + 1)
//
// 【线段树维护】
// - 区间加/减 (lazy propagation)
// - 查询区间最小值 + 最左索引
// - 当 minVal == 0 时，minIdx 就是 diff=0 的最小左端点
//
// 【演算: nums = [2, 5, 4, 3]】
// 初始: diff = [0, 0, 0, 0]
//
// j=0: v=2(偶), prev=-1 → diff[0..0] += 1
//      diff = [1, 0, 0, 0], min([0,0])=1 ≠ 0
//
// j=1: v=5(奇), prev=-1 → diff[0..1] -= 1
//      diff = [0, -1, 0, 0], min([0,1])=-1
//      但 diff[0]=0 → minIdx=0 → ans = 1-0+1 = 2
//
// j=2: v=4(偶), prev=-1 → diff[0..2] += 1
//      diff = [1, 0, 1, 0], min=0 at idx 1
//      ans = max(2, 2-1+1) = 2
//
// j=3: v=3(奇), prev=-1 → diff[0..3] -= 1
//      diff = [0, -1, 0, -1], min=-1
//      但 diff[0]=0 → minIdx=0 → ans = max(2, 3-0+1) = 4
//
// 答案: 4 ✓
// ============================================================

class Solution_SegTree {
    // 线段树节点
    struct Node {
        int minVal;      // 区间最小值
        int minIdx;      // 最小值的最左索引
        int lazy;        // 懒标记: 待加的值
    };

    vector<Node> tree;

    void build(int node, int lo, int hi) {
        tree[node].lazy = 0;
        if (lo == hi) {
            tree[node] = {0, lo, 0};
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * node, lo, mid);
        build(2 * node + 1, mid + 1, hi);
        pushUp(node);
    }

    void pushUp(int node) {
        auto& l = tree[2 * node];
        auto& r = tree[2 * node + 1];
        if (l.minVal <= r.minVal) {
            tree[node].minVal = l.minVal;
            tree[node].minIdx = l.minIdx;
        } else {
            tree[node].minVal = r.minVal;
            tree[node].minIdx = r.minIdx;
        }
    }

    void pushDown(int node) {
        if (tree[node].lazy != 0) {
            for (int child : {2 * node, 2 * node + 1}) {
                tree[child].minVal += tree[node].lazy;
                tree[child].lazy += tree[node].lazy;
            }
            tree[node].lazy = 0;
        }
    }

    // 区间 [ql, qr] 加 val
    void rangeAdd(int node, int lo, int hi, int ql, int qr, int val) {
        if (ql > hi || qr < lo) return;
        if (ql <= lo && hi <= qr) {
            tree[node].minVal += val;
            tree[node].lazy += val;
            return;
        }
        pushDown(node);
        int mid = (lo + hi) / 2;
        rangeAdd(2 * node, lo, mid, ql, qr, val);
        rangeAdd(2 * node + 1, mid + 1, hi, ql, qr, val);
        pushUp(node);
    }

    // 查询 [ql, qr] 中最小值以及最左索引
    pair<int, int> queryMin(int node, int lo, int hi, int ql, int qr) {
        if (ql > hi || qr < lo) return {INT_MAX, -1};
        if (ql <= lo && hi <= qr) return {tree[node].minVal, tree[node].minIdx};
        pushDown(node);
        int mid = (lo + hi) / 2;
        auto left = queryMin(2 * node, lo, mid, ql, qr);
        auto right = queryMin(2 * node + 1, mid + 1, hi, ql, qr);
        if (left.first <= right.first) return left;
        return right;
    }

public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        tree.resize(4 * n);
        build(1, 0, n - 1);

        unordered_map<int, int> last;  // 值 → 上次出现位置
        int ans = 0;

        for (int j = 0; j < n; j++) {
            int v = nums[j];
            int prev = last.count(v) ? last[v] : -1;

            // 对左端点 [prev+1, j] 做区间更新
            // 偶数: 这些左端点的子数组新增了一个 distinct 偶数
            // 奇数: 新增了一个 distinct 奇数
            int delta = (v % 2 == 0) ? 1 : -1;
            rangeAdd(1, 0, n - 1, prev + 1, j, delta);

            last[v] = j;

            // 查询 [0, j] 中 diff == 0 的最小索引
            // diff == 0 ↔ 该区间的最小值为 0 且该最小值的位置就是答案
            auto [minVal, minIdx] = queryMin(1, 0, n - 1, 0, j);
            if (minVal == 0) {
                ans = max(ans, j - minIdx + 1);
            }
        }

        return ans;
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法          | 时间       | 空间 | 实现难度 | 适用场景      |
 * |---------------|-----------|------|---------|---------------|
 * | O(n^2) + set  | O(n^2)    | O(n) | 简单    | 面试基础解    |
 * | O(nlogn) 线段树| O(nlogn) | O(n) | 高      | 竞赛/大数据量 |
 *
 * 面试策略:
 * 1. 先写 O(n^2) 枚举（表明思路清晰）
 * 2. 分析瓶颈（distinct 不可减，前缀差不可用）
 * 3. 指出线段树优化方向（不一定要完整实现）
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 混淆总出现次数和 distinct 个数:
 *    ✗ [2,2,3] 偶数 2 次 ≠ 奇数 1 次 → 不平衡？
 *    ✓ [2,2,3] distinct 偶数 1 个 = distinct 奇数 1 个 → 平衡！
 *
 * 2. 尝试用前缀差:
 *    ✗ 套用 LC 525 的前缀差方法
 *    ✓ distinct count 不可减，必须用枚举或线段树
 *
 * 3. 线段树查询方向:
 *    ✗ 查 diff=0 的最大索引（得到最短子数组）
 *    ✓ 查 diff=0 的最小索引（得到最长子数组）
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: "O(n^2) 太慢怎么优化？"
 *     线段树: 每个值只在首次出现位置贡献 distinct，
 *     用 last[v] + 区间更新定位影响范围。O(n log n)。
 *
 * Q2: "和 LC 525 的区别？"
 *     525 是简单 0/1 计数（可减），前缀差 O(n)。
 *     本题是 distinct count（不可减），前缀差不适用。
 *
 * Q3: "滑动窗口可以吗？"
 *     不行。收缩左端点时，如果被移除的值在窗口内还有副本，
 *     distinct count 不变 → 不满足单调性 → 滑窗不适用。
 *
 * Q4: "数据范围 n=10^5，O(n^2) 能过吗？"
 *     10^10 操作通常 TLE。但 unordered_set 常数小，
 *     且很多情况提前结束。实际上可能勉强通过，
 *     但面试中应该指出 O(n log n) 的优化方向。
 * ============================================================
 */
