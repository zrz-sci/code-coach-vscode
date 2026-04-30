// [file://solution.cpp](./solution.cpp)
//
// LeetCode 995: Minimum Number of K Consecutive Bit Flips
// 解法一: 差分数组法 (推荐)
// 解法二: 原地标记法
// 解法三: 队列法

// ============================================================
// 解法一: 差分数组法
// 时间 O(n), 空间 O(n)
// 核心思想: 用差分数组追踪翻转效果，避免实际翻转操作
// ============================================================
class Solution {
public:
    int minKBitFlips(vector<int>& nums, int k) {
        int n = nums.size();
        // isFlipped[i] = 1 表示位置 i 是某次翻转的起始点
        vector<int> isFlipped(n, 0);
        // flipCount 追踪当前位置被翻转的累计次数
        int flipCount = 0;
        int result = 0;

        for (int i = 0; i < n; i++) {
            // 当窗口滑出时，减去已过期的翻转效果
            // 位置 i-k 处发起的翻转只影响 [i-k, i-1]，到位置 i 不再有效
            if (i >= k) {
                flipCount -= isFlipped[i - k];
            }

            // 判断当前位置的实际值
            // nums[i] 被翻转 flipCount 次后的值:
            //   如果 flipCount 为偶数，实际值 = nums[i]
            //   如果 flipCount 为奇数，实际值 = 1 - nums[i]
            // 当实际值为 0 时需要翻转，即 (nums[i] + flipCount) % 2 == 0
            if ((nums[i] + flipCount) % 2 == 0) {
                // 如果剩余长度不足 k，无法翻转
                if (i + k > n) {
                    return -1;
                }
                // 在此位置发起翻转
                isFlipped[i] = 1;
                flipCount++;
                result++;
            }
        }

        return result;
    }
};

// ============================================================
// 解法二: 原地标记法
// 时间 O(n), 空间 O(1)
// 核心思想: 利用 nums 数组本身标记翻转起始位置
//           将 nums[i] += 2 表示此处发起过翻转 (原值 0/1, 标记后 2/3)
// ============================================================
class SolutionInPlace {
public:
    int minKBitFlips(vector<int>& nums, int k) {
        int n = nums.size();
        int flipCount = 0;
        int result = 0;

        for (int i = 0; i < n; i++) {
            // 窗口滑出: 检查 i-k 处是否发起过翻转
            if (i >= k && nums[i - k] >= 2) {
                flipCount--;
                nums[i - k] -= 2;  // 恢复原值 (可选)
            }

            // 当前实际值为 0 时需要翻转
            if ((nums[i] + flipCount) % 2 == 0) {
                if (i + k > n) {
                    return -1;
                }
                // 标记翻转: 原值 + 2 (0->2, 1->3)
                nums[i] += 2;
                flipCount++;
                result++;
            }
        }

        // 恢复数组 (如果需要保持原数组不变)
        for (int i = 0; i < n; i++) {
            if (nums[i] >= 2) {
                nums[i] -= 2;
            }
        }

        return result;
    }
};

// ============================================================
// 解法三: 队列法
// 时间 O(n), 空间 O(n)
// 核心思想: 用队列维护当前有效的翻转起始位置
// ============================================================
class SolutionQueue {
public:
    int minKBitFlips(vector<int>& nums, int k) {
        int n = nums.size();
        // 队列存储所有翻转的起始位置
        queue<int> flipStarts;
        int result = 0;

        for (int i = 0; i < n; i++) {
            // 弹出已过期的翻转 (窗口左端点 + k <= i)
            while (!flipStarts.empty() && flipStarts.front() + k <= i) {
                flipStarts.pop();
            }

            // 当前有效翻转次数 = 队列大小
            int flipCount = flipStarts.size();

            // 判断当前位的实际值
            if ((nums[i] + flipCount) % 2 == 0) {
                if (i + k > n) {
                    return -1;
                }
                flipStarts.push(i);
                result++;
            }
        }

        return result;
    }
};
