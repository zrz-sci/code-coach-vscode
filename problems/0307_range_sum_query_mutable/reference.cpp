/*
 * 【题目本质】
 * 单点更新 + 区间求和 → 经典 BIT / 线段树应用
 * 朴素前缀和 update O(n)，暴力 sumRange O(n) → 均不够高效
 *
 * 【解法总览】
 * Solution1: 树状数组 BIT  ⭐⭐⭐⭐⭐  init O(n log n) / update O(log n) / query O(log n)
 * Solution2: 线段树         ⭐⭐⭐⭐    init O(n) / update O(log n) / query O(log n)
 */

// ===================== Solution1: 树状数组 BIT（推荐） =====================
// BIT 核心：lowbit(x) = x & (-x)，索引从 1 开始
class NumArray1 {
    vector<int> tree; // BIT 数组，1-indexed
    vector<int> nums; // 保存原始值（用于计算 update 差值）
    int n;

    int lowbit(int x) { return x & (-x); }

    // 单点加：将位置 i 增加 delta
    void add(int i, int delta) {
        for (; i <= n; i += lowbit(i))
            tree[i] += delta;
    }

    // 前缀和查询：返回 [1..i] 的和
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= lowbit(i))
            sum += tree[i];
        return sum;
    }

public:
    NumArray1(vector<int>& nums) : nums(nums), n(nums.size()), tree(nums.size() + 1, 0) {
        for (int i = 0; i < n; i++)
            add(i + 1, nums[i]); // 0-indexed → 1-indexed
    }

    void update(int index, int val) {
        add(index + 1, val - nums[index]); // 传差值，不是新值
        nums[index] = val;                 // 同步更新原数组
    }

    int sumRange(int left, int right) {
        return query(right + 1) - query(left); // 前缀和差分
    }
};

// ===================== Solution2: 线段树 =====================
// 完整的递归线段树，支持单点更新 + 区间查询
class NumArray2 {
    vector<int> tree; // 线段树数组，4 * n 大小
    int n;

    void build(vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            tree[node] = nums[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(nums, 2 * node, start, mid);
        build(nums, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1]; // pushUp
    }

    void updateTree(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val; // 叶子节点直接赋值
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid)
            updateTree(2 * node, start, mid, idx, val);
        else
            updateTree(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1]; // pushUp
    }

    int queryTree(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;           // 完全不交
        if (l <= start && end <= r) return tree[node]; // 完全包含
        int mid = start + (end - start) / 2;
        return queryTree(2 * node, start, mid, l, r) +
               queryTree(2 * node + 1, mid + 1, end, l, r);
    }

public:
    NumArray2(vector<int>& nums) : n(nums.size()), tree(4 * nums.size(), 0) {
        if (n > 0) build(nums, 1, 0, n - 1);
    }

    void update(int index, int val) {
        updateTree(1, 0, n - 1, index, val);
    }

    int sumRange(int left, int right) {
        return queryTree(1, 0, n - 1, left, right);
    }
};

/*
 * 【解法对比】
 * ┌────────────────┬──────────────┬──────────┬──────────────────────────┐
 * │ 解法           │ 单次操作     │ 空间     │ 特点                     │
 * ├────────────────┼──────────────┼──────────┼──────────────────────────┤
 * │ Solution1 BIT  │ O(log n)     │ O(n)     │ 代码简洁，常数小         │
 * │ Solution2 线段树│ O(log n)     │ O(4n)    │ 通用性强，可扩展懒标记   │
 * └────────────────┴──────────────┴──────────┴──────────────────────────┘
 *
 * 【易错点】
 * 1. BIT 索引从 1 开始，lowbit(0) = 0 会死循环
 * 2. BIT update 传的是差值 (val - nums[index])，不是新值
 * 3. 更新 BIT 后必须同步更新 nums[index]，否则下次差值计算错误
 * 4. 线段树数组大小必须是 4n，不是 2n（满二叉树最坏情况）
 * 5. sumRange(l, r) = query(r+1) - query(l)，注意 0-indexed 转 1-indexed
 *
 * 【面试追问】
 * Q1: BIT vs 线段树？→ BIT 仅限可减性操作（和/异或），线段树可处理最值/懒标记
 * Q2: O(n) 初始化 BIT？→ 从左到右，tree[i] += nums[i]，再传给 tree[i+lowbit(i)]
 * Q3: 区间更新+区间查询？→ 两个 BIT 维护差分 / 线段树+懒标记
 */
