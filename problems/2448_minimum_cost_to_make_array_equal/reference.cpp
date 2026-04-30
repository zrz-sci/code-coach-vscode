/*
 * LeetCode 2448: Minimum Cost to Make Array Equal
 *
 * 【题目本质】
 * 带权绝对值偏差最小化：f(t) = Σ cost[i] × |nums[i] - t|，求使 f(t) 最小的 t。
 * 这就是经典的"加权中位数"问题。
 *
 * 【解法总览】
 * 解法1: 暴力枚举       — O(nV) / O(1)   — 枚举所有可能目标值
 * 解法2: 三分搜索       — O(n log V) / O(1) — 利用凸函数性质
 * 解法3: 排序+加权中位数 — O(n log n) / O(n) — 找加权中位数 ⭐
 * 解法4: 排序+前缀和递推 — O(n log n) / O(n) — 增量计算最优点 ⭐
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举每个目标值，O(n) 算开销
// 时间: O(nV)  空间: O(1)    (V = max(nums) - min(nums))
//
// 【思路】
// 目标值只可能在 [min(nums), max(nums)] 范围内。
// 超出这个范围一定不如取端点值。
// 对每个 t，遍历数组算 Σ cost[i] * |nums[i] - t|。
// 瓶颈：V 可达 10^6, n 可达 10^5, 总计 10^11 次操作。
// ============================================================
class Solution1 {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int lo = *min_element(nums.begin(), nums.end());
        int hi = *max_element(nums.begin(), nums.end());
        long long ans = LLONG_MAX;
        for (int t = lo; t <= hi; t++) {
            long long total = 0;
            for (int i = 0; i < (int)nums.size(); i++) {
                total += (long long)cost[i] * abs(nums[i] - t);
            }
            ans = min(ans, total);
        }
        return ans;
    }
};

// ============================================================
// 解法2: 三分搜索 — 利用 f(t) 是凸函数
// 时间: O(n log V)  空间: O(1)
//
// 【思路】
// f(t) = Σ cost[i] * |nums[i] - t| 是若干加权绝对值函数之和。
// 每个 cost[i] * |nums[i] - t| 关于 t 是凸函数（V字形），
// 凸函数之和仍然是凸函数。
// 凸函数的极小值可以用三分搜索在 O(log V) 次内定位。
//
// 三分搜索原理:
//   cost
//    |  \         /
//    |   \       /
//    |    \_____/     ← f(t) 是凸的
//    +------------- t
//    lo  m1  m2  hi
//
//    若 f(m1) < f(m2)，极小值在 [lo, m2) 中 → hi = m2-1
//    若 f(m1) ≥ f(m2)，极小值在 (m1, hi] 中 → lo = m1+1
// ============================================================
class Solution2 {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int n = nums.size();
        
        // 计算把所有数移到 t 的总开销
        auto calc = [&](long long t) -> long long {
            long long total = 0;
            for (int i = 0; i < n; i++) {
                total += (long long)cost[i] * abs(nums[i] - t);
            }
            return total;
        };
        
        long long lo = *min_element(nums.begin(), nums.end());
        long long hi = *max_element(nums.begin(), nums.end());
        
        while (lo < hi) {
            long long m1 = lo + (hi - lo) / 3;
            long long m2 = hi - (hi - lo) / 3;
            if (calc(m1) < calc(m2)) {
                hi = m2 - 1;  // 最小值不可能在 m2 右侧
            } else {
                lo = m1 + 1;  // 最小值不可能在 m1 左侧
            }
        }
        return calc(lo);
    }
};

// ============================================================
// 解法3: 排序 + 加权中位数 — 数学最优解 ⭐
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 经典结论：Σ w_i * |x_i - t| 在加权中位数处取最小值。
//
// 加权中位数定义：按 x 排序后，累计权重首次达到总权重一半的位置。
//
// 直觉理解：当 t 从左往右移一单位时，
//   - 左侧所有点距离减1 → 开销减少 leftWeight
//   - 右侧所有点距离加1 → 开销增加 rightWeight
//   只要 leftWeight < rightWeight，t 右移就是有利的。
//   当 leftWeight ≥ rightWeight，就不应该再右移了。
//   这个临界点就是加权中位数。
//
// 示例: nums=[1,3,5,2], cost=[2,3,1,14]
// 排序后: (1,2), (2,14), (3,3), (5,1)
// 总权重 = 20, 半值 = 10
//
// 累计: 2 → 16(≥10) → 加权中位数 = 2
// 开销 = 2×1 + 14×0 + 3×1 + 1×3 = 8  ✓
// ============================================================
class Solution3 {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int n = nums.size();
        
        // 用索引数组间接排序，保持 nums 和 cost 的对应关系
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums[a] < nums[b];
        });
        
        // 计算总权重
        long long totalWeight = 0;
        for (int c : cost) totalWeight += c;
        
        // 找加权中位数：累计权重 * 2 >= 总权重的第一个位置
        long long cumWeight = 0;
        int target = -1;
        for (int i : idx) {
            cumWeight += cost[i];
            // 用乘法避免整数除法截断: cumWeight >= totalWeight / 2
            if (cumWeight * 2 >= totalWeight) {
                target = nums[i];
                break;
            }
        }
        
        // 计算总开销
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            ans += (long long)cost[i] * abs(nums[i] - target);
        }
        return ans;
    }
};

// ============================================================
// 解法4: 排序 + 前缀和递推 — 不需要知道加权中位数结论 ⭐
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 排序后，只需考虑目标值 = nums[idx[0]], nums[idx[1]], ..., nums[idx[n-1]]。
// 因为最优解一定在某个 nums 值上取到（凸函数的整数最优点在折点上）。
//
// 先算出 f(nums[idx[0]])——所有数移到最小值的开销。
// 然后递推：当目标从 nums[idx[k-1]] 变到 nums[idx[k]] 时，差值 d：
//   左侧（已过目标的）每个多走 d → 开销增加 d * leftWeight
//   右侧（还没到目标的）每个少走 d → 开销减少 d * rightWeight
//
// 递推示意 (排序后):
// 位置:    1    2    3    5
// 权重:    2   14    3    1
//
// f(1) = 2×0 + 14×1 + 3×2 + 1×4 = 24
// f(2) = 24 + 1×2  - 1×18 = 8     (左权=2, 右权=18, d=1)
// f(3) = 8  + 1×16 - 1×4  = 20    (左权=16, 右权=4, d=1)
// f(5) = 20 + 2×19 - 2×1  = 56    (左权=19, 右权=1, d=2)
// 最小 = 8 ✓
// ============================================================
class Solution4 {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int n = nums.size();
        
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums[a] < nums[b];
        });
        
        // 先计算以 nums[idx[0]] 为目标的总开销
        long long totalCost = 0;
        long long rightWeight = 0;
        for (int i : idx) {
            // 注意: (long long) 强转防溢出
            totalCost += (long long)cost[i] * (nums[i] - nums[idx[0]]);
            rightWeight += cost[i];
        }
        
        long long ans = totalCost;
        long long leftWeight = 0;
        
        for (int k = 1; k < n; k++) {
            long long d = nums[idx[k]] - nums[idx[k - 1]];
            
            // idx[k-1] 从"右侧"归入"左侧"
            // 因为目标值已经越过 nums[idx[k-1]] 了
            leftWeight += cost[idx[k - 1]];
            rightWeight -= cost[idx[k - 1]];
            
            // 目标值右移 d: 左侧点距离增加 d, 右侧点距离减少 d
            totalCost += d * leftWeight - d * rightWeight;
            ans = min(ans, totalCost);
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间       | 空间  | 核心思想     | 适用场景           |
// |----------|-----------|-------|-------------|-------------------|
// | 暴力     | O(nV)     | O(1)  | 穷举         | 理解题意           |
// | 三分     | O(n logV) | O(1)  | 凸函数性质   | 通用凸优化         |
// | 加权中位数| O(n logn) | O(n)  | 统计学结论   | 面试首选(有数学功底)|
// | 前缀和   | O(n logn) | O(n)  | 增量计算     | 面试首选(编程技巧) |
//
// 三分搜索 vs 加权中位数：
//   三分是通用方法，不需要知道"加权中位数"这个概念
//   加权中位数更精确，直接 O(1) 定位最优点（排序后一次遍历）
//   面试中两者都可以，但说出"加权中位数"会加分
//
// 【易错点】
// 1. 溢出: cost[i] * |nums[i] - t| 可达 10^12，必须 long long
//    ✗ int total = cost[i] * abs(nums[i] - t);
//    ✓ long long total = (long long)cost[i] * abs(nums[i] - t);
//
// 2. 加权中位数判断条件:
//    ✗ if (cumWeight >= totalWeight / 2) — 整数除法截断
//    ✓ if (cumWeight * 2 >= totalWeight) — 用乘法避免截断
//
// 3. 前缀和递推中 leftWeight 更新时机:
//    ✗ 先算 totalCost 再更新 leftWeight
//       → idx[k-1] 还在右侧，开销算错
//    ✓ 先把 idx[k-1] 归入 leftWeight，再算增量
//       → 因为目标已越过 nums[idx[k-1]]
//
// 4. 排序对象:
//    ✗ 直接 sort(nums.begin(), nums.end()) 
//       → nums 和 cost 的对应关系丢失
//    ✓ 用索引数组排序 或 pair<int,int> 打包排序
//
// 【面试追问】
// Q1: 如果所有 cost 相同，最优目标值是什么？
//     → 中位数。f(t)=Σ|x_i-t| 在中位数处取最小，经典结论。
//
// Q2: 加了权重后为什么是加权中位数？
//     → 等价于把 nums[i] 复制 cost[i] 份的无权问题。
//       或者分析 t 移动一个单位时，开销变化 = leftWeight - rightWeight，
//       临界点就是累计权重过半的位置。
//
// Q3: 如果开销是平方代价 cost[i] * (nums[i] - t)²？
//     → 最优解变成加权平均数。对 t 求导=0 得 t = Σ(cost[i]*nums[i]) / Σcost[i]。
//       注意 t 可能不是整数，需要检查 floor(t) 和 ceil(t) 两个值。
//
// Q4: 能否不排序，O(n) 解决？
//     → 理论可以：加权中位数等价于带权第 k 大，可用快速选择 O(n) 期望。
//       但面试中 O(n log n) 通常足够，且更容易写对。
// ============================================================
