// ============================================================
// 724. Find Pivot Index
// ============================================================
// 标签: Array, Prefix Sum
// 难度: Easy
//
// 核心思路:
//   total = sum(nums)
//   遍历时维护 leftSum, 检查 2*leftSum + nums[i] == total
//   第一个满足条件的 i 就是 pivot index
// ============================================================


// ============================================================
// 解法1: 前缀和 + 一次遍历 (最优, 面试首选)
//
// 数学推导:
//   leftSum + nums[i] + rightSum = total
//   pivot 条件: leftSum == rightSum
//   代入: leftSum == total - leftSum - nums[i]
//   化简: 2 * leftSum + nums[i] == total
//
// 边界:
//   i=0 时 leftSum=0, 检查 nums[0] == total
//   i=n-1 时检查 2*leftSum + nums[n-1] == total (即 rightSum=0)
//   这些都在通用公式中自然成立
//
// 时间: O(n)
// 空间: O(1)
// ============================================================
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        // Step 1: 计算总和
        int total = 0;
        for (int x : nums) {
            total += x;
        }

        // Step 2: 从左到右遍历, 维护 leftSum
        int leftSum = 0;
        for (int i = 0; i < (int)nums.size(); i++) {
            // 中心下标条件: leftSum == rightSum
            // 即: 2 * leftSum + nums[i] == total
            if (2 * leftSum + nums[i] == total) {
                return i;
            }
            // 先检查再更新, 确保 nums[i] 不被算入 leftSum
            leftSum += nums[i];
        }

        return -1;  // 不存在中心下标
    }
};


// ============================================================
// 解法2: 前缀和数组 (更直观, 空间 O(n))
//
// 构建前缀和数组 prefix[]
//   prefix[0] = 0
//   prefix[i+1] = prefix[i] + nums[i]
//
// 对于位置 i:
//   leftSum  = prefix[i]
//   rightSum = prefix[n] - prefix[i+1]
//
// 时间: O(n)
// 空间: O(n)
// ============================================================
class Solution_PrefixArray {
public:
    int pivotIndex(vector<int>& nums) {
        int n = nums.size();
        vector<int> prefix(n + 1, 0);

        // 构建前缀和
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // 查找中心下标
        for (int i = 0; i < n; i++) {
            int leftSum = prefix[i];
            int rightSum = prefix[n] - prefix[i + 1];
            if (leftSum == rightSum) {
                return i;
            }
        }

        return -1;
    }
};


// ============================================================
// 解法3: 右侧和递减 (另一种视角)
//
// 初始: leftSum = 0, rightSum = total - nums[0]
// 每步: 检查 leftSum == rightSum
//       然后 leftSum += nums[i], rightSum -= nums[i+1]
//
// 本质和解法1相同, 但显式维护 rightSum
// 代码略冗长 (需要处理 i+1 越界)
//
// 时间: O(n)
// 空间: O(1)
// ============================================================
class Solution_TwoSum {
public:
    int pivotIndex(vector<int>& nums) {
        int n = nums.size();

        int total = 0;
        for (int x : nums) total += x;

        int leftSum = 0;
        int rightSum = total - nums[0];

        // 检查 i=0
        if (leftSum == rightSum) return 0;

        for (int i = 1; i < n; i++) {
            leftSum += nums[i - 1];
            rightSum -= nums[i];
            if (leftSum == rightSum) {
                return i;
            }
        }

        return -1;
    }
};


// ============================================================
// 总结
// ============================================================
/*
 * 【解法对比】
 *
 * | 解法              | 时间  | 空间  | 推荐度       |
 * |-------------------|-------|-------|--------------|
 * | 一次遍历 (推荐)   | O(n)  | O(1)  | 面试首选     |
 * | 前缀和数组        | O(n)  | O(n)  | 更直观       |
 * | 双变量递减        | O(n)  | O(1)  | 本质相同     |
 *
 * 【关键公式】
 * 2 * leftSum + nums[i] == total  ⟺  leftSum == rightSum
 *
 * 【易错点】
 * 1. 先检查条件再更新 leftSum (否则 nums[i] 被算入左侧)
 * 2. 负数情况: leftSum 可能为负, 公式依然正确
 * 3. 返回最左的: 从左到右遍历, 第一个满足条件的直接 return
 * 4. 单元素数组: [x] → leftSum=0, rightSum=0, 返回 0
 * 5. 全零数组: 每个位置都满足, 返回 0
 *
 * 【延伸】
 * - LeetCode 1991 完全相同的题目
 * - 二维版本: 行前缀和找"中心行"
 * - 循环数组: 需重新定义左右侧的范围
 *
 * 【面试追问】
 * Q: 能否二分搜索?
 * A: 不能。leftSum 在有负数时不单调, 无法二分。
 *
 * Q: 如果要找所有 pivot index?
 * A: 不在第一个满足时 return, 改为收集到数组中。
 */
