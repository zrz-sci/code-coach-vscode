/*
 * LeetCode 303: Range Sum Query - Immutable (区域和检索 - 数组不可变)
 *
 * 【题目本质】
 * 多次查询同一个不可变数组的区间和。
 * 核心问题：如何将每次查询从 O(n) 降到 O(1)？
 *
 * 【解法总览】
 * 解法1: 暴力求和    — 构造O(1), 查询O(n)   — 基线对比
 * 解法2: 前缀和      — 构造O(n), 查询O(1)   — 面试首选
 * 解法3: 线段树      — 构造O(n), 查询O(logn) — 可修改版本(LC307)用
 */

// ============================================================
// 解法1: 暴力求和
// 构造: O(1)  查询: O(n)
//
// 【思路】
// 最直接: 存下原数组, 每次查询遍历 [left, right] 累加。
// q 次查询总 O(n*q), n=q=10^4 时为 10^8, 可能超时。
// 价值: 明确瓶颈在"每次都要重新遍历"。
// ============================================================
class NumArray1 {
public:
    vector<int> data;

    NumArray1(vector<int>& nums) : data(nums) {}

    int sumRange(int left, int right) {
        int sum = 0;
        for (int i = left; i <= right; i++) {
            sum += data[i];
        }
        return sum;
    }
};

// ============================================================
// 解法2: 前缀和 — 面试首选 ⭐
// 构造: O(n)  查询: O(1)
//
// 【思路】
// 预处理前缀和数组: prefix[i] = nums[0] + ... + nums[i-1]
// 区间和公式: sumRange(l, r) = prefix[r+1] - prefix[l]
//
// 为什么 prefix 长度 n+1, 且 prefix[0] = 0？
//   → 哨兵设计。left=0 时 sumRange(0,r) = prefix[r+1] - prefix[0]
//     = prefix[r+1], 不需要特殊处理。
//
// 示例: nums = [-2, 0, 3, -5, 2, -1]
//
// 构建:
//   prefix[0] = 0
//   prefix[1] = 0 + (-2) = -2
//   prefix[2] = -2 + 0   = -2
//   prefix[3] = -2 + 3   = 1
//   prefix[4] = 1 + (-5) = -4
//   prefix[5] = -4 + 2   = -2
//   prefix[6] = -2 + (-1) = -3
//
//   prefix = [0, -2, -2, 1, -4, -2, -3]
//
// 查询:
//   sumRange(0,2) = prefix[3] - prefix[0] = 1 - 0 = 1 ✓
//   sumRange(2,5) = prefix[6] - prefix[2] = -3 - (-2) = -1 ✓
//   sumRange(0,5) = prefix[6] - prefix[0] = -3 - 0 = -3 ✓
// ============================================================
class NumArray2 {
public:
    vector<int> prefix;

    NumArray2(vector<int>& nums) {
        int n = nums.size();
        prefix.resize(n + 1, 0);
        // prefix[0] = 0 (哨兵, 已被 resize 初始化)
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    int sumRange(int left, int right) {
        // 区间 [left, right] 的和 = 前(right+1)个元素之和 - 前left个元素之和
        return prefix[right + 1] - prefix[left];
    }
};

// ============================================================
// 解法3: 线段树 — 为 LC307 (可修改版本) 做准备
// 构造: O(n)  查询: O(log n)  修改: O(log n)
//
// 【思路】
// 线段树可以同时支持区间查询和单点修改。
// 本题数组不可变, 线段树查询 O(logn) 反而比前缀和的 O(1) 慢。
// 但如果数组可修改(LC307), 前缀和修改需 O(n) 重建,
// 而线段树修改只需 O(logn)。
//
// 线段树结构:
//   seg[1] = 整个数组的和
//   seg[2*node] = 左半区间的和
//   seg[2*node+1] = 右半区间的和
//   4*n 大小足够存储所有节点
//
// 本题可以不写, 提到"如果数组可变就需要线段树"是加分项。
// ============================================================
class NumArray3 {
public:
    vector<int> seg;
    int n;

    NumArray3(vector<int>& nums) {
        n = nums.size();
        if (n == 0) return;
        seg.resize(4 * n, 0);
        build(nums, 1, 0, n - 1);
    }

    int sumRange(int left, int right) {
        return query(1, 0, n - 1, left, right);
    }

private:
    void build(vector<int>& nums, int node, int lo, int hi) {
        if (lo == hi) {
            seg[node] = nums[lo];
            return;
        }
        int mid = (lo + hi) / 2;
        build(nums, 2 * node, lo, mid);
        build(nums, 2 * node + 1, mid + 1, hi);
        seg[node] = seg[2 * node] + seg[2 * node + 1];
    }

    int query(int node, int lo, int hi, int l, int r) {
        if (l <= lo && hi <= r) return seg[node]; // 完全包含
        if (lo > r || hi < l) return 0;            // 完全不交
        int mid = (lo + hi) / 2;
        return query(2 * node, lo, mid, l, r) +
               query(2 * node + 1, mid + 1, hi, l, r);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法    | 构造  | 查询    | 修改     | 适用场景              |
// |---------|-------|---------|----------|-----------------------|
// | 暴力    | O(1)  | O(n)   | O(1)     | 极少次查询            |
// | 前缀和  | O(n)  | O(1)   | O(n)重建 | 不可变数组 + 多次查询 |
// | 线段树  | O(n)  | O(logn)| O(logn)  | 可变数组 + 多次查询   |
//
// 本题: 前缀和是最优解, 代码最短, O(1) 查询。
//
// ============================================================
// 【易错点】
//
// 1. 前缀和数组大小:
//    ✗ prefix.resize(n) → prefix[n] 越界
//    ✓ prefix.resize(n + 1) → 多一个哨兵位
//
// 2. 区间和公式下标:
//    ✗ prefix[right] - prefix[left] → 漏了 nums[right]
//    ✓ prefix[right + 1] - prefix[left]
//
// 3. 忘记 prefix[0] = 0:
//    ✗ prefix[0] = nums[0] → 公式在 left=0 时多算了 nums[0]
//    ✓ prefix[0] = 0, prefix[i+1] = prefix[i] + nums[i]
//
// 4. 整数溢出(大数组):
//    ✗ int prefix → 如果 nums[i] 很大且 n 很大可能溢出
//    ✓ 本题 n<=10^4, nums[i]<=10^5, 最大和 10^9, int 够用
//       但如果扩展到更大范围, 考虑用 long long
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 如果数组会被修改呢？(LC307)
//     前缀和修改需 O(n) 重建。
//     用 BIT (树状数组) 或线段树, 修改和查询都是 O(logn)。
//
// Q2: 如何扩展到二维？(LC304)
//     二维前缀和 prefix[i][j] = 左上角(0,0)到(i-1,j-1)的总和。
//     区间和用容斥: sum = P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1]
//
// Q3: 前缀和还有哪些应用？
//     - LC560: 前缀和 + 哈希表 → 和为K的子数组
//     - 差分数组: 前缀和的逆操作, 用于"区间加"(LC370)
//     - 前缀积: LC238 除自身以外的积
//
// ============================================================
// 【相关题型】
//
// - 304. 二维区域和检索:
//   前缀和从一维扩展到二维, 用容斥原理。
//
// - 307. 区域和检索 - 数组可修改:
//   数组可变, 需要 BIT 或线段树。直接复用解法3。
//
// - 560. 和为 K 的子数组:
//   前缀和 + 哈希表。"子数组和=k" → "两个前缀和之差=k"。
//
// - 53. 最大子数组和:
//   前缀和视角: max(prefix[j] - prefix[i]), 但 Kadane 更优。
// ============================================================
