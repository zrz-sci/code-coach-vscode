// ============================================================
// 719. Find K-th Smallest Pair Distance
// ============================================================
// 标签: Array, Two Pointers, Binary Search, Sorting
// 难度: Hard
//
// 核心思路:
//   在距离值域 [0, max-min] 上二分搜索
//   对每个候选距离 mid，用排序后的双指针 O(n) 统计距离 <= mid 的数对个数
//   找最小的 mid 使得 count >= k
// ============================================================


// ============================================================
// 解法1: 二分答案 + 双指针 (最优解, 面试首选)
//
// countPairs(d) = "排序后距离 <= d 的数对个数"
// 该函数关于 d 单调非递减 → 可二分找第一个 >= k 的 d
//
// 双指针计数原理 (排序后):
//   固定 right，收缩 left 使 nums[right]-nums[left] <= mid
//   [left, right) 中每个 i 与 right 都构成合法数对
//   贡献 right - left 个
//   left 随 right 单调不减 → 总时间 O(n)
//
// 时间: O(n log n + n log W), W = max - min
// 空间: O(log n) 排序栈
// ============================================================
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();

        // 二分搜索距离值域
        int lo = 0;
        int hi = nums[n - 1] - nums[0];

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int cnt = countPairsWithinDist(nums, mid);

            if (cnt >= k) {
                // mid 距离有 >= k 对, 答案可能更小或就是 mid
                hi = mid;
            } else {
                // mid 距离不够 k 对, 答案必须 > mid
                lo = mid + 1;
            }
        }

        return lo;  // lo == hi 就是答案
    }

private:
    // 统计排序数组中距离 <= maxDist 的数对个数
    // 双指针: O(n)
    int countPairsWithinDist(const vector<int>& nums, int maxDist) {
        int count = 0;
        int left = 0;

        for (int right = 1; right < (int)nums.size(); right++) {
            while (nums[right] - nums[left] > maxDist) {
                left++;
            }
            count += right - left;
        }

        return count;
    }
};


// ============================================================
// 解法2: 二分 + upper_bound (替代双指针的写法)
//
// 对每个 left，用 upper_bound 在 [left+1, n) 中定位右边界
// upper_bound 找第一个 > nums[left]+mid 的位置
// 距离 <= mid 的右端点个数 = upper_bound位置 - (left+1)
//
// 时间: O(n log n * log W) — 每轮计数 O(n log n)
// 空间: O(log n)
// ============================================================
class Solution_UpperBound {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();

        int lo = 0, hi = nums[n - 1] - nums[0];

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;

            long long cnt = 0;
            for (int left = 0; left < n; left++) {
                auto it = upper_bound(nums.begin() + left + 1,
                                      nums.end(),
                                      nums[left] + mid);
                cnt += (it - nums.begin() - left - 1);
            }

            if (cnt >= k) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }

        return lo;
    }
};


// ============================================================
// 解法3: 暴力枚举 + nth_element (仅用于理解和验证)
//
// 枚举所有 C(n,2) 数对, 用 nth_element 获取第 k 小
// nth_element 平均 O(n), 比完全排序快
//
// 时间: O(n^2) 枚举 + O(n^2) nth_element
// 空间: O(n^2)
// n=10^4 时 n^2=10^8, 会 TLE
// ============================================================
class Solution_Brute {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> distances;
        distances.reserve(n * (n - 1) / 2);

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                distances.push_back(abs(nums[i] - nums[j]));
            }
        }

        nth_element(distances.begin(),
                    distances.begin() + k - 1,
                    distances.end());

        return distances[k - 1];
    }
};


// ============================================================
// 总结
// ============================================================
/*
 * 【解法对比】
 *
 * | 解法              | 时间              | 空间     | 适用场景       |
 * |-------------------|-------------------|----------|----------------|
 * | 二分+双指针(推荐) | O(n logn + n logW)| O(logn)  | 最优, 面试首选 |
 * | 二分+upper_bound  | O(n logn * logW)  | O(logn)  | 代码更直观     |
 * | 暴力+nth_element  | O(n^2)            | O(n^2)   | 仅用于验证     |
 *
 * 【关键洞察】
 * 1. "第 k 小" → 二分答案: 将 "找第 k 小" 转化为 "最小的 d 使得 count(<=d) >= k"
 * 2. 排序 + 双指针: 排序后双指针 O(n) 统计满足距离约束的数对数
 * 3. 值域二分 vs 下标二分: lo/hi 是距离值域 [0, max-min], 不是数组下标
 *
 * 【易错点】
 * 1. 二分的是"距离值"而非"下标"
 * 2. cnt >= k → hi = mid, 不是 hi = mid - 1 (mid 本身可能是答案)
 * 3. left 不要在 right 循环体内重置为 0
 * 4. 极端情况下 count 可能很大, 注意 int 范围
 * 5. 别忘了先排序, 双指针依赖有序性
 *
 * 【面试追问】
 * Q: 二分找到的值一定是某个实际数对的距离吗?
 * A: 是的。count(d) 是阶梯函数, 只在实际距离值处跳增。
 *    若 d 不是实际距离则 count(d)==count(d-1), 两者都>=k,
 *    与 "第一个>=k" 的二分语义矛盾。
 *
 * Q: 重复元素怎么处理?
 * A: 无需特殊处理。排序后相同元素相邻, 距离0被正确计入。
 *    m 个相同元素贡献 C(m,2) 对, 双指针自然累加正确。
 */
