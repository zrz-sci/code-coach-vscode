/*
 * LeetCode 528: 按权重随机选择 (Random Pick with Weight)
 *
 * 【题目本质】
 * 将均匀随机数映射到按权重的非均匀概率分布。
 * 核心模型：权重数组构成一条数轴，每个下标占据一段区间，
 * 随机扔一个点到数轴上，找它落在哪段 → 前缀和 + 二分查找。
 *
 * 【解法总览】
 * 解法1: 展开数组         — 构造 O(sum) / 查询 O(1) / 空间 O(sum) — 最直觉但不可行
 * 解法2: 前缀和+线性扫描   — 构造 O(n) / 查询 O(n) / 空间 O(n)   — 理解过渡
 * 解法3: 前缀和+二分查找   — 构造 O(n) / 查询 O(log n) / 空间 O(n) — 面试标准解 ⭐
 */

// ============================================================
// 解法1: 展开数组 — 按权重复制下标，均匀随机选
// 构造: O(sum)  查询: O(1)  空间: O(sum)
//
// 【思路】
// 最朴素的想法：权重为3就放3份，然后均匀随机选一个。
// w = [1, 3] → expanded = [0, 1, 1, 1]
// 随机选一个：25%选到0，75%选到1，完美符合概率要求。
//
// 【瓶颈】
// 权重总和最多 10^5 × 10^4 = 10^9，展开数组会爆内存。
// 这个解法只用来理解问题本质，不适合实际提交。
// ============================================================
class Solution1 {
public:
    vector<int> expanded;
    
    Solution1(vector<int>& w) {
        for (int i = 0; i < (int)w.size(); i++) {
            for (int j = 0; j < w[i]; j++) {
                expanded.push_back(i);
            }
        }
    }
    
    int pickIndex() {
        return expanded[rand() % expanded.size()];
    }
};

// ============================================================
// 解法2: 前缀和 + 线性扫描
// 构造: O(n)  查询: O(n)  空间: O(n)
//
// 【思路】
// 不需要真的展开数组！用前缀和"虚拟"表示每个区间。
// 
// w = [1, 3, 2]
// prefix = [1, 4, 6]   total = 6
//
// 数轴模型:
//   | 1 |  2  3  4  |  5  6  |
//   idx0    idx1       idx2
//
// 生成随机数 target ∈ [1, 6]：
//   target=1 → prefix[0]=1 ≥ 1 → 返回 0
//   target=3 → prefix[0]=1 < 3, prefix[1]=4 ≥ 3 → 返回 1
//   target=5 → prefix[0]=1 < 5, prefix[1]=4 < 5, prefix[2]=6 ≥ 5 → 返回 2
//
// 瓶颈：每次查询需要线性扫描前缀和数组 O(n)。
// ============================================================
class Solution2 {
public:
    vector<int> prefix;
    int total;
    
    Solution2(vector<int>& w) {
        prefix.resize(w.size());
        prefix[0] = w[0];
        for (int i = 1; i < (int)w.size(); i++) {
            prefix[i] = prefix[i - 1] + w[i];
        }
        total = prefix.back();
    }
    
    int pickIndex() {
        int target = rand() % total + 1; // [1, total]
        for (int i = 0; i < (int)prefix.size(); i++) {
            if (prefix[i] >= target) {
                return i;
            }
        }
        return (int)prefix.size() - 1; // 理论不可达
    }
};

// ============================================================
// 解法3: 前缀和 + 二分查找 ⭐ 面试标准解
// 构造: O(n)  查询: O(log n)  空间: O(n)
//
// 【思路】
// 解法2的瓶颈：线性扫描前缀和数组找第一个 >= target 的位置。
// 关键观察：前缀和数组天然递增有序！有序数组上查找 → 二分查找！
// 
// 二分搜索空间收缩示例:
// prefix = [1, 4, 6], target = 3
//
// Round1: lo=0, hi=2, mid=1
//         prefix[1]=4 >= 3  →  hi = mid = 1
//         搜索空间: [0, 1]
//
// Round2: lo=0, hi=1, mid=0
//         prefix[0]=1 < 3   →  lo = mid+1 = 1
//         搜索空间: [1, 1]
//
// lo == hi == 1  →  返回 1 ✓
//
// 为什么判断条件是 prefix[mid] < target（严格小于）？
// 因为我们找的是"第一个 >= target 的位置"：
// - prefix[mid] < target → mid 装不下 target，答案一定在右边 → lo = mid+1
// - prefix[mid] >= target → mid 可能是答案，但左边可能还有 → hi = mid
// ============================================================
class Solution3 {
public:
    vector<int> prefix;
    int total;
    
    Solution3(vector<int>& w) {
        prefix.resize(w.size());
        prefix[0] = w[0];
        for (int i = 1; i < (int)w.size(); i++) {
            prefix[i] = prefix[i - 1] + w[i];
        }
        total = prefix.back();
    }
    
    int pickIndex() {
        // 生成 [1, total] 的随机整数
        // 为什么是 +1 而不是 +0？
        // 因为前缀和从 w[0] 开始，prefix[0] = w[0] >= 1
        // target=0 会导致永远返回下标0（错误）
        int target = rand() % total + 1;
        
        // 二分查找：在有序的 prefix 中找第一个 >= target 的位置
        int lo = 0, hi = (int)prefix.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (prefix[mid] < target) {
                lo = mid + 1;  // prefix[mid] 覆盖不到 target，排除左半
            } else {
                hi = mid;      // prefix[mid] >= target，保留 mid 作为候选
            }
        }
        return lo;
    }
};

// ============================================================
// 解法3-STL版: 使用 lower_bound 简化二分
// 构造: O(n)  查询: O(log n)  空间: O(n)
//
// 【思路】
// lower_bound 就是找"第一个 >= target 的位置"，和我们手写二分完全等价。
// 面试中如果允许用 STL，这样写更简洁不易出错。
// ============================================================
class Solution {
public:
    vector<int> prefix;
    int total;
    
    Solution(vector<int>& w) {
        prefix.resize(w.size());
        prefix[0] = w[0];
        for (int i = 1; i < (int)w.size(); i++) {
            prefix[i] = prefix[i - 1] + w[i];
        }
        total = prefix.back();
    }
    
    int pickIndex() {
        int target = rand() % total + 1;
        // lower_bound: 第一个 >= target 的迭代器
        return (int)(lower_bound(prefix.begin(), prefix.end(), target) - prefix.begin());
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 构造 | 查询 | 空间 | 适用 |
// |------|------|------|------|------|
// | 展开数组 | O(sum) | O(1) | O(sum) | 权重和极小时 |
// | 前缀和+线扫 | O(n) | O(n) | O(n) | 理解原理 |
// | 前缀和+二分 | O(n) | O(log n) | O(n) | 面试标准答案 |
//
// 面试中直接给解法3，用解法1/2说明思考过程。
//
// ============================================================
// 【易错点】
//
// 1. 随机数范围:
//    ✗ int target = rand() % total;       // [0, total-1]，target=0 时永远返回 0
//    ✓ int target = rand() % total + 1;   // [1, total]，和前缀和语义匹配
//
// 2. 二分判断条件:
//    ✗ if (prefix[mid] <= target) lo = mid + 1;
//      → 当 prefix[mid] == target 时跳过了正确答案
//      → 例如 prefix=[1,4,6], target=4, 期望返回1
//        mid=1, prefix[1]=4==4, 错误地 lo=2, 最终返回2（错误）
//    ✓ if (prefix[mid] < target) lo = mid + 1;
//      → 严格小于才排除，相等时保留（hi=mid）
//
// 3. 前缀和构造从 prefix[0]=w[0] 开始:
//    ✗ prefix[0] = 0; // 有些人习惯前缀和多一位
//       → 如果多一位则 lower_bound 的结果需要 -1，容易搞混
//    ✓ prefix[0] = w[0]; // 不多一位，直接对应下标
//
// 4. rand() 的模偏差:
//    面试中提一句即可："rand() % total 在 RAND_MAX 不是 total 整数倍时
//    不完美均匀，更好的做法是 uniform_int_distribution"
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么需要前缀和？直接用原始权重数组不行吗？
// A1: 原始权重表示"各段的长度"，但我们需要知道每段的"边界在哪"。
//     前缀和把各段长度转换成了累积边界值，这样才能用二分查找定位。
//     例如 w=[1,3,2]，你知道第二段长度是3，但不知道它从哪到哪。
//     前缀和 prefix=[1,4,6] 告诉你第二段的边界是 [2,4]。
//
// Q2: 如果权重数组会动态变化（频繁修改某个权重），怎么优化？
// A2: 用树状数组 (BIT) 或线段树维护前缀和。
//     - 修改 w[i]: O(log n) 更新树（vs 普通前缀和数组需 O(n) 重算）
//     - 查询 pickIndex: 在树上二分 O(log n)
//     总体：修改 O(log n)，查询 O(log n)，比重建前缀和快得多。
//
// Q3: 如果要求查询严格 O(1)，可以做到吗？
// A3: 可以！用 Alias Method（别名法）:
//     - 预处理 O(n): 构造一张表，每个格子对应两个下标和一个阈值
//     - 查询 O(1): 随机选一个格子，翻硬币决定用哪个下标
//     - 思路：将非均匀分布"平铺"成均匀格子，每个格子最多装两个下标
//     - 代价：预处理稍复杂，但查询完美 O(1)
//
// Q4: 如果不允许修改输入数组、也不允许额外空间，能做吗？
// A4: 可以用蓄水池抽样 (Reservoir Sampling) 的变体：
//     遍历数组，对第 i 个元素以概率 w[i]/累积权重和 选中它。
//     时间 O(n)，空间 O(1)，但每次查询都是 O(n)。
//     适合流式数据或空间极度受限的场景。
// ============================================================
