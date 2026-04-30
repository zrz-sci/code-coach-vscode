/*
 * LeetCode 16: 3Sum Closest (最接近的三数之和)
 *
 * 【题目本质】
 * 在数组中找三个不同位置的数，使和与 target 的绝对差最小。
 * 是 LeetCode 15 "三数之和" 的变体：从找等于变成找最接近。
 *
 * 【解法总览】
 * 解法1: 暴力枚举        — O(n³) / O(1) — 最直觉，三重循环
 * 解法2: 排序 + 双指针    — O(n²) / O(1) — 面试首选
 * 解法3: 排序 + 双指针 + 剪枝 — O(n²) / O(1) — 常数优化，追问加分
 */

// ============================================================
// 解法1: 暴力枚举 — 遍历所有三元组
// 时间: O(n³)  空间: O(1)
//
// 【思路】
// 最自然的想法：把所有 C(n,3) 种三元组都试一遍，
// 每次计算和与 target 的绝对差，维护最小差对应的和。
//
// 这个解法的价值在于：让我们看出"固定第一个数后，
// 问题退化成在剩余数组中找两个数和最接近某个值"，
// 从而为双指针优化做铺垫。
// ============================================================
class Solution1 {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int n = nums.size();
        // 用第一个合法组合初始化，避免 INT_MAX 导致的溢出风险
        int closest = nums[0] + nums[1] + nums[2];

        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 1; j < n - 1; j++) {
                for (int k = j + 1; k < n; k++) {
                    int sum = nums[i] + nums[j] + nums[k];
                    // 绝对差更小，说明更接近
                    if (abs(sum - target) < abs(closest - target)) {
                        closest = sum;
                    }
                }
            }
        }
        return closest;
    }
};

// ============================================================
// 解法2: 排序 + 双指针 — 面试首选 ⭐
// 时间: O(n²)  空间: O(1)（不计排序的栈空间）
//
// 【思路】
// 暴力法的瓶颈：固定 nums[i] 后，在剩余元素中找两个数使和
// 最接近 target - nums[i]，暴力需要 O(n²)。
//
// 关键观察：如果数组有序，两个数的和可以通过双指针在 O(n)
// 内逼近目标——和太小就左指针右移（增大），和太大就右指针
// 左移（减小）。有序性保证了不会遗漏最优解。
//
// 双指针逼近过程示意 (nums=[-4,-1,1,2], target=1):
//
// 排序后: [-4, -1, 1, 2]
//
// i=0, nums[i]=-4:
//   L=1, R=3: sum=-4+(-1)+2=-3  diff=|(-3)-1|=4  sum<target → L++
//   L=2, R=3: sum=-4+1+2=-1     diff=|(-1)-1|=2  sum<target → L++
//   L==R, 结束. closest=-1
//
// i=1, nums[i]=-1:
//   L=2, R=3: sum=-1+1+2=2      diff=|2-1|=1  更优! closest=2
//                                sum>target → R--
//   L==R, 结束.
//
// i=2, 剩余不足2个, 结束. 返回 closest=2 ✓
// ============================================================
class Solution2 {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end()); // 排序是双指针的前提
        int n = nums.size();
        int closest = nums[0] + nums[1] + nums[2];

        for (int i = 0; i < n - 2; i++) {
            // 跳过重复的 i：不影响正确性，减少无意义计算
            // 注意 i > 0 防止越界
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];

                // 绝对差更小 → 更新 closest
                if (abs(sum - target) < abs(closest - target)) {
                    closest = sum;
                }

                if (sum == target) {
                    // 差值为0，不可能更近了，直接返回
                    return target;
                } else if (sum < target) {
                    // 和太小，需要更大的数 → 左指针右移
                    left++;
                } else {
                    // 和太大，需要更小的数 → 右指针左移
                    right--;
                }
            }
        }
        return closest;
    }
};

// ============================================================
// 解法3: 排序 + 双指针 + 剪枝优化 — 追问加分项
// 时间: O(n²) 最坏，实际常数更小  空间: O(1)
//
// 【思路】
// 在解法2的基础上加两个剪枝:
//
// 剪枝1 (最小值剪枝):
//   对于当前 i，能取到的最小和 = nums[i] + nums[i+1] + nums[i+2]
//   如果这个最小和已经 > target，那后面的 i 更大，最小和只会更大。
//   当前的 minSum 可能是一个候选答案（更新 closest），但之后直接 break。
//
// 剪枝2 (最大值剪枝):
//   对于当前 i，能取到的最大和 = nums[i] + nums[n-2] + nums[n-1]
//   如果这个最大和还 < target，说明当前 i 的所有组合都小于 target，
//   maxSum 是当前 i 下最接近的（更新 closest），然后 continue 到下一个 i。
// ============================================================
class Solution3 {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int closest = nums[0] + nums[1] + nums[2];

        for (int i = 0; i < n - 2; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            // 剪枝1: 当前 i 的最小和已经超过 target
            int minSum = nums[i] + nums[i + 1] + nums[i + 2];
            if (minSum > target) {
                // minSum 可能比之前的 closest 更接近 target
                if (abs(minSum - target) < abs(closest - target)) {
                    closest = minSum;
                }
                break; // 后续 i 更大，minSum 只会更大，不可能更优
            }

            // 剪枝2: 当前 i 的最大和还不够 target
            int maxSum = nums[i] + nums[n - 2] + nums[n - 1];
            if (maxSum < target) {
                // maxSum 是当前 i 下最接近 target 的
                if (abs(maxSum - target) < abs(closest - target)) {
                    closest = maxSum;
                }
                continue; // 当前 i 的所有组合都 < target，下一个 i 可能更好
            }

            // 标准双指针
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];

                if (abs(sum - target) < abs(closest - target)) {
                    closest = sum;
                }

                if (sum == target) {
                    return target;
                } else if (sum < target) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        return closest;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (暴力) vs 解法2 (排序+双指针):
//   暴力法固定 i 后用 O(n²) 找另外两个数。
//   排序后利用有序性，双指针只需 O(n)，总体从 O(n³) 降到 O(n²)。
//   关键: 排序不影响答案（题目要求返回和，不是下标）。
//
// 解法2 vs 解法3 (加剪枝):
//   最坏情况时间一样 O(n²)，但剪枝在数据分布不均时能跳过大量无效 i。
//   面试中先写解法2，被追问"还能优化吗？"时说出剪枝。
//
// 【易错点】
//
// 1. closest 初始化用 INT_MAX 导致溢出:
//    ✗ int closest = INT_MAX;
//      → abs(INT_MAX - target) 可能溢出 int 范围
//    ✓ int closest = nums[0] + nums[1] + nums[2];
//      → 用合法的三数之和初始化，保证在安全范围
//
// 2. 比较条件方向写反:
//    ✗ if (abs(sum - target) > abs(closest - target))
//      → 这在找最远的，不是最近的
//    ✓ if (abs(sum - target) < abs(closest - target))
//
// 3. 跳过重复 i 时忘记边界检查:
//    ✗ if (nums[i] == nums[i - 1]) continue;  // i=0 时越界!
//    ✓ if (i > 0 && nums[i] == nums[i - 1]) continue;
//
// 4. 剪枝中 break 和 continue 搞混:
//    minSum > target → break (后面的 i 更大，不可能更优)
//    maxSum < target → continue (当前 i 不行，但后面更大的 i 有机会)
//    反过来会导致漏解或多余计算
//
// 5. 忘记 sum == target 的提前返回:
//    不是 bug，但面试官可能问"能不能更早终止？"
//    差值为0是最优，直接 return target。
//
// 【面试追问 — 递进链】
//
// Q1 (基础): 暴力解时间多少？能优化吗？
//    → O(n³)。排序后用双指针把内层从 O(n²) 降到 O(n)，总 O(n²)。
//
// Q2 (正确性): 双指针为什么不会漏掉最优解？
//    → 排序后，sum < target 时只有增大 left 才可能更接近，
//      sum > target 时只有减小 right 才可能更接近。
//      被跳过的方向只会让差距更大。
//
// Q3 (优化): 还能不能更快？
//    → 最坏 O(n²) 是下界（需要检查 Θ(n²) 对候选）。
//      但可以用最小和/最大和剪枝跳过不可能的 i，实际更快。
//
// Q4 (变体): 如果要返回下标而不是和？
//    → 排序打乱下标。用 (值, 原始下标) 的 pair 数组，
//      排序后双指针同时维护下标信息。
//
// Q5 (拓展): 推广到 K 数之和最接近？
//    → 外层 K-2 层循环 + 最内层双指针，O(n^(K-1))。
//      3Sum → O(n²), 4Sum → O(n³), 以此类推。
// ============================================================
