/*
 * LeetCode 905: Sort Array By Parity
 *
 * 解法1: 双指针对撞 (面试首选)
 * 时间: O(n)  空间: O(1)
 *
 * 左指针找奇数，右指针找偶数，交换。
 * 循环不变量: [0,l) 全偶数, (r,n-1] 全奇数
 */

// ==================== 解法1: 双指针对撞 ====================

class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        int l = 0, r = (int)nums.size() - 1;

        while (l < r) {
            // 左指针: 跳过偶数(它们已经在正确位置)
            // 注意: 必须先检查 l < r 防止越界
            while (l < r && (nums[l] & 1) == 0) {
                l++;
            }
            // 右指针: 跳过奇数(它们已经在正确位置)
            while (l < r && (nums[r] & 1) == 1) {
                r--;
            }

            // 此时 l < r, nums[l] 为奇数, nums[r] 为偶数
            // 交换它们到各自应在的一侧
            if (l < r) {
                swap(nums[l], nums[r]);
                l++;
                r--;
            }
        }

        return nums;
    }
};

// ==================== 解法2: 快慢指针(单指针) ====================

/*
 * 时间: O(n)  空间: O(1)
 * 维护偶数区域边界 slow，遇到偶数就交换到 slow 位置
 */

class Solution_SlowFast {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        // slow: 偶数区域的右边界(下一个偶数应放的位置)
        // 循环不变量: nums[0..slow-1] 全是偶数
        int slow = 0;

        for (int fast = 0; fast < (int)nums.size(); fast++) {
            // 遇到偶数 → 放到偶数区域尾部
            if ((nums[fast] & 1) == 0) {
                swap(nums[slow], nums[fast]);
                slow++;
            }
            // 遇到奇数 → 跳过，它会自然留在偶数区域后面
        }

        return nums;
    }
};

// ==================== 解法3: STL partition ====================

/*
 * 时间: O(n)  空间: O(1)
 * 用 STL 一行搞定。展示 C++ 标准库熟练度。
 */

class Solution_STL {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        // partition: 将满足谓词的元素移到前面
        // 返回第一个不满足谓词的迭代器
        partition(nums.begin(), nums.end(), [](int x) {
            return (x & 1) == 0; // 偶数为 true → 移到前面
        });
        return nums;
    }
};

// ==================== 解法4: 额外数组(两遍扫描) ====================

/*
 * 时间: O(n)  空间: O(n)
 * 最简单直观，但不是原地操作
 */

class Solution_Extra {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        vector<int> result;
        result.reserve(nums.size());

        // 第一遍: 收集所有偶数
        for (int num : nums) {
            if ((num & 1) == 0) {
                result.push_back(num);
            }
        }

        // 第二遍: 收集所有奇数
        for (int num : nums) {
            if ((num & 1) == 1) {
                result.push_back(num);
            }
        }

        return result;
    }
};

// ==================== 解法5: 自定义排序 ====================

/*
 * 时间: O(n log n)  空间: O(1) [原地排序]
 * 利用排序的比较函数：偶数"更小"排前面
 * 不如双指针高效，但代码极简
 */

class Solution_Sort {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        sort(nums.begin(), nums.end(), [](int a, int b) {
            // 偶数排前面: 偶数的"权重"为0, 奇数为1
            return (a & 1) < (b & 1);
        });
        return nums;
    }
};

// ==================== 解法6: 稳定分区(保持相对顺序) ====================

/*
 * 时间: O(n)  空间: O(n)
 * 如果面试追问"能否保持偶数/奇数各自的相对顺序"
 * 用 stable_partition 或额外数组
 */

class Solution_Stable {
public:
    vector<int> sortArrayByParity(vector<int>& nums) {
        // stable_partition 保持相对顺序
        stable_partition(nums.begin(), nums.end(), [](int x) {
            return (x & 1) == 0;
        });
        return nums;
    }
};

/*
 * ==================== 各解法对比 ====================
 *
 * | 解法           | 时间      | 空间  | 稳定 | 代码量 |
 * |---------------|-----------|-------|------|--------|
 * | 双指针对撞     | O(n)      | O(1)  | 否   | 中等   |
 * | 快慢指针       | O(n)      | O(1)  | 否   | 最少   |
 * | STL partition  | O(n)      | O(1)  | 否   | 一行   |
 * | 额外数组       | O(n)      | O(n)  | 是   | 简单   |
 * | 自定义排序     | O(n logn) | O(1)  | 否   | 一行   |
 * | stable_part.  | O(n)      | O(n)  | 是   | 一行   |
 *
 * 面试策略:
 * 1. 先说暴力(额外数组)
 * 2. 优化到双指针 O(1) 空间
 * 3. 被追问稳定性时切换到 stable_partition
 * 4. 知道 partition 和快排的关系
 *
 * ==================== 延伸: 荷兰国旗问题 ====================
 *
 * 如果有三类元素需要分区(如 LC 75 Sort Colors):
 * - 维护三个指针: low, mid, high
 * - [0, low): 第一类
 * - [low, mid): 第二类
 * - [mid, high]: 未处理
 * - (high, n-1]: 第三类
 *
 * 本题是荷兰国旗的简化版(只有两类):
 * - [0, slow): 偶数
 * - [slow, fast): 奇数
 * - [fast, n): 未处理
 */
