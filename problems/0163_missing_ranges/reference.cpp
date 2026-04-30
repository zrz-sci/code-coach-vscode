/*
 * 【题目本质】
 * 在有序数组和给定范围 [lower, upper] 中找出所有不在数组中的数字区间。
 * 本质就是检查"间隙"：相邻元素之间以及边界处是否有遗漏的数字。
 *
 * 【解法总览】
 * 解法1: 虚拟边界统一处理  O(n)/O(1) ⭐ 最简洁
 * 解法2: 分步处理三段      O(n)/O(1)  逻辑更直观
 *
 * 间隙检查图示:
 *
 *   nums = [0, 1, 3, 50, 75], lower=0, upper=99
 *
 *   虚拟边界: (-1)  0  1  3  50  75  (100)
 *                  |     |  |     |     |
 *                  无   [2,2][4,49][51,74][76,99]
 *
 *   prev=-1 → curr=0: 差1, 无间隙
 *   prev=0  → curr=1: 差1, 无间隙
 *   prev=1  → curr=3: 差2, 间隙 [2,2]
 *   prev=3  → curr=50: 差47, 间隙 [4,49]
 *   prev=50 → curr=75: 差25, 间隙 [51,74]
 *   prev=75 → curr=100: 差25, 间隙 [76,99]
 */

// ===================== 解法1: 虚拟边界法 =====================
// 思路：把 lower-1 作为起始 prev，upper+1 作为结束的虚拟元素
// 为什么这样做：统一处理首段、中间段、尾段，不需要特判
class Solution1 {
public:
    vector<vector<int>> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<vector<int>> result;
        int prev = lower - 1;  // 虚拟左边界

        for (int i = 0; i <= (int)nums.size(); ++i) {
            // 最后一次迭代用 upper+1 做虚拟右边界
            int curr = (i < (int)nums.size()) ? nums[i] : upper + 1;

            // 差 >= 2 说明有间隙（差1说明紧邻，无缺失）
            if (curr - prev >= 2) {
                result.push_back({prev + 1, curr - 1});
            }

            prev = curr;
        }

        return result;
    }
};

// ===================== 解法2: 分步处理 =====================
// 思路：分三段处理更符合直觉，适合面试时快速写出
// 为什么分三段：首段（lower到nums[0]）、中间段、尾段 逻辑稍有不同
class Solution2 {
public:
    vector<vector<int>> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<vector<int>> result;
        int n = nums.size();

        // 空数组：整个范围都是缺失区间
        if (n == 0) {
            result.push_back({lower, upper});
            return result;
        }

        // 第一段：lower 到 nums[0] 之间
        if (nums[0] > lower) {
            result.push_back({lower, nums[0] - 1});
        }

        // 中间段：相邻元素之间
        for (int i = 1; i < n; ++i) {
            if (nums[i] - nums[i - 1] >= 2) {
                result.push_back({nums[i - 1] + 1, nums[i] - 1});
            }
        }

        // 最后段：nums[n-1] 到 upper 之间
        if (nums[n - 1] < upper) {
            result.push_back({nums[n - 1] + 1, upper});
        }

        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 虚拟边界 | O(n) | O(1) | 代码最短，无特判 |
 * | 分步处理 | O(n) | O(1) | 逻辑清晰，不易出错 |
 *
 * 【易错点】
 * 1. 忘处理空数组 → 直接访问 nums[0] 崩溃
 * 2. 间隙条件写成 > 0 而非 >= 2 → 紧邻的也被当成间隙
 * 3. 区间写成 {prev, curr} 而非 {prev+1, curr-1} → 包含了锚点
 * 4. 遗漏尾段 nums[n-1] 到 upper → 少一段结果
 *
 * 【面试追问】
 * Q1: nums 未排序？ → 先排序 O(n log n)，再线性扫描
 * Q2: 返回字符串格式 "a->b"？ → 旧版题目，仅修改输出格式
 * Q3: lower/upper 为 long long？ → prev 和 curr 用 long long 防溢出
 */
