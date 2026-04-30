/*
 * LCR 007: 三数之和（同主站 15. 3Sum）
 *
 * 【题目本质】
 * 找出数组中所有不重复的三元组 (a, b, c) 使得 a + b + c = 0。
 * 核心挑战：如何高效枚举 + 正确去重。
 *
 * 【解法总览】
 * 解法1: 排序 + 双指针      — O(n^2) / O(1) — 面试首选 ⭐
 * 解法2: 排序 + 哈希表      — O(n^2) / O(n) — 备选方案
 * 解法3: 通用 kSum 递归框架 — O(n^{k-1})    — 面试加分
 */

// ============================================================
// 解法1: 排序 + 双指针 — 面试首选 ⭐
// 时间: O(n^2)  空间: O(1)（不计输出）
//
// 【思路】
// 1. 排序数组
// 2. 枚举第一个数 nums[i]，在 [i+1, n-1] 用双指针找两数之和 = -nums[i]
// 3. 三处去重:
//    (a) 外层: nums[i] == nums[i-1] 时跳过（向前看！不是向后看！）
//    (b) 内层: 找到解后 left 跳过连续相同值
//    (c) 内层: 找到解后 right 跳过连续相同值
//
// 【为什么外层去重用向前看？】
// 向后看 nums[i]==nums[i+1] 会跳过 [-1,-1,2] 这样的合法解
// 向前看 nums[i]==nums[i-1] 表示"以这个值开头的搜索已完成"
//
// 【关键剪枝】
// nums[i] > 0 时直接 break: 排序后三个正数之和不可能为 0
//
// 手动演算:
//   输入: [-1, 0, 1, 2, -1, -4]
//   排序: [-4, -1, -1, 0, 1, 2]
//
//   i=0, nums[0]=-4, target=4:
//     left=1(-1), right=5(2): sum=1 < 4 → left++
//     left=2(-1), right=5(2): sum=1 < 4 → left++
//     left=3(0),  right=5(2): sum=2 < 4 → left++
//     left=4(1),  right=5(2): sum=3 < 4 → left++
//     left=5 >= right=5 → 结束
//
//   i=1, nums[1]=-1, target=1:
//     left=2(-1), right=5(2): sum=1 == 1 ✓ → [-1,-1,2]
//       left跳重复→3, right跳重复→4, left=3, right=4
//     left=3(0), right=4(1): sum=1 == 1 ✓ → [-1,0,1]
//       left=4, right=3 → left >= right → 结束
//
//   i=2, nums[2]=-1 == nums[1]=-1 → 跳过（外层去重）
//   i=3, nums[3]=0 > 0 → break
//
//   输出: [[-1,-1,2], [-1,0,1]]
// ============================================================

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> result;

        // 特判：少于3个元素不可能有三元组
        if (n < 3) return result;

        // Step 1: 排序，为双指针和去重做准备
        sort(nums.begin(), nums.end());

        // Step 2: 枚举第一个数 nums[i]
        for (int i = 0; i < n - 2; i++) {
            // 剪枝: 排序后第一个数 > 0，三正数之和不可能为 0
            if (nums[i] > 0) break;

            // 外层去重: 跳过与前一个相同的值（向前看！）
            // 向后看 nums[i]==nums[i+1] 会跳过 [-1,-1,2] 合法解
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            // Step 3: 双指针在 [i+1, n-1] 范围内找两数之和 = -nums[i]
            int left = i + 1, right = n - 1;
            int target = -nums[i];

            while (left < right) {
                int sum = nums[left] + nums[right];

                if (sum == target) {
                    // 找到一组解
                    result.push_back({nums[i], nums[left], nums[right]});

                    // 内层去重: left 跳过连续相同值
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    // 内层去重: right 跳过连续相同值
                    while (left < right && nums[right] == nums[right - 1]) right--;

                    // 跳过当前值，继续找下一组
                    left++;
                    right--;
                } else if (sum < target) {
                    left++;   // 和太小，需要更大的数 → left 右移
                } else {
                    right--;  // 和太大，需要更小的数 → right 左移
                }
            }
        }

        return result;
    }
};


// ============================================================
// 解法2: 排序 + 哈希表 — 备选方案
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 固定前两个数 nums[i] 和 nums[j]，
// 用 unordered_set 存储 [i+1, j-1] 中的值，
// 查找 -nums[i]-nums[j] 是否在 set 中。
//
// 去重比双指针复杂，需要在 j 循环内跳过重复值。
// 面试中不推荐（双指针更优雅），但作为思路拓展。
// ============================================================

class Solution_Hash {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> result;
        if (n < 3) return result;

        sort(nums.begin(), nums.end());

        for (int i = 0; i < n - 2; i++) {
            if (nums[i] > 0) break;
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            unordered_set<int> seen;
            for (int j = i + 1; j < n; j++) {
                int complement = -nums[i] - nums[j];

                if (seen.count(complement)) {
                    result.push_back({nums[i], complement, nums[j]});
                    // 跳过重复的 nums[j]
                    while (j + 1 < n && nums[j] == nums[j + 1]) j++;
                }
                seen.insert(nums[j]);
            }
        }

        return result;
    }
};


// ============================================================
// 解法3: 通用 kSum 递归框架 — 面试加分
// 时间: O(n^{k-1})  空间: O(k)（递归深度）
//
// 【思路】
// kSum 固定一个数后降维为 (k-1)Sum，递归到 2Sum 时用双指针。
// 可以轻松扩展到 4Sum, 5Sum 等。
//
// 【剪枝优化】
// - nums[i] * k > target → 当前最小的 k 个数之和都 > target
// - nums[i] + nums[n-1] * (k-1) < target → 当前数太小
// ============================================================

class Solution_kSum {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        return kSum(nums, 0, 0, 3);
    }

private:
    vector<vector<int>> kSum(vector<int>& nums, long long target, int start, int k) {
        vector<vector<int>> result;
        int n = nums.size();

        // 基本情况: 2Sum 用双指针
        if (k == 2) {
            int left = start, right = n - 1;
            while (left < right) {
                long long sum = (long long)nums[left] + nums[right];
                if (sum == target) {
                    result.push_back({nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < target) {
                    left++;
                } else {
                    right--;
                }
            }
            return result;
        }

        // 递归情况: 固定一个数，降维到 (k-1)Sum
        for (int i = start; i < n - k + 1; i++) {
            // 去重
            if (i > start && nums[i] == nums[i - 1]) continue;

            // 剪枝1: 当前最小 k 个数之和 > target
            if ((long long)nums[i] * k > target) break;
            // 剪枝2: 当前数 + 最大 (k-1) 个数之和 < target
            if ((long long)nums[i] + (long long)nums[n - 1] * (k - 1) < target) continue;

            auto sub = kSum(nums, target - nums[i], i + 1, k - 1);
            for (auto& triplet : sub) {
                triplet.insert(triplet.begin(), nums[i]);
                result.push_back(triplet);
            }
        }

        return result;
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法          | 时间     | 空间  | 去重难度 | 推荐 |
 * |---------------|---------|-------|---------|------|
 * | 暴力三重循环  | O(n^3)  | O(n)  | 极难    | ✗    |
 * | 排序+双指针   | O(n^2)  | O(1)  | 简单    | ⭐   |
 * | 排序+哈希表   | O(n^2)  | O(n)  | 中等    | 备选 |
 * | kSum 框架     | O(n^2)  | O(k)  | 简单    | 扩展 |
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 外层去重方向:
 *    ✗ if (nums[i] == nums[i+1]) continue
 *      → [-1,-1,2] 中 i=0 就被跳过，错过合法解
 *    ✓ if (i > 0 && nums[i] == nums[i-1]) continue
 *
 * 2. 内层去重遗漏:
 *    ✗ 找到解后直接 left++; right-- 不跳重复
 *      → [-2,0,0,2,2] 中 [-2,0,2] 被重复收录
 *    ✓ while 跳过重复值后再移动
 *
 * 3. 忘记特判:
 *    ✗ 不检查 n < 3 → 空数组时越界
 *    ✓ if (n < 3) return {};
 *
 * 4. 外层循环范围:
 *    ✗ for (int i = 0; i < n; i++)
 *    ✓ for (int i = 0; i < n - 2; i++)
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 4Sum / kSum 怎么办？
 *     递归降维: kSum 固定一个数 → (k-1)Sum → ... → 2Sum 双指针
 *     时间 O(n^{k-1})
 *
 * Q2: 为什么去重用向前看？
 *     向后看 nums[i]==nums[i+1] 会跳过 [-1,-1,2] 合法解。
 *     向前看表示"以这个值开头的搜索已做过"。
 *
 * Q3: 能否不排序？
 *     可以用哈希表 + set<vector<int>> 去重，但去重极复杂且常数巨大。
 *     排序是 O(n log n)，不影响总体 O(n^2)，收益远大于成本。
 *
 * Q4: 双指针为什么不会漏解？
 *     有序数组中，sum < target 时 left++ 只会让 sum 增大，
 *     sum > target 时 right-- 只会让 sum 减小。
 *     单调性保证了每个可能的 (left, right) 组合都被覆盖。
 * ============================================================
 */
