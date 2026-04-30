// 【题目本质】找一个目标值使所有元素到它的绝对差之和最小 -> 中位数
// 【解法总览】解法1: 排序+中位数 O(nlogn)/O(1) | 解法2: 快速选择 O(n)/O(1)

// ===================== Solution 1: 排序 + 中位数 =====================
// 思路：排序后取中间元素作为目标，累加绝对差
// 时间 O(n log n)，空间 O(1)
class Solution1 {
public:
    int minMoves2(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int median = nums[nums.size() / 2];
        int moves = 0;
        for (int num : nums) {
            moves += abs(num - median);
        }
        return moves;
    }
};

// ===================== Solution 2: 快速选择 + 双指针 =====================
// 思路：nth_element 找中位数 O(n)，排序后双指针配对累加差值
// 时间 O(n) 均摊，空间 O(1)
class Solution2 {
public:
    int minMoves2(vector<int>& nums) {
        int n = nums.size();
        nth_element(nums.begin(), nums.begin() + n / 2, nums.end());
        int median = nums[n / 2];
        int moves = 0;
        for (int num : nums) {
            moves += abs(num - median);
        }
        return moves;
    }
};

// 【解法对比】
// Solution1: O(n log n) 排序，简洁直观，面试首选
// Solution2: O(n) 快速选择，最优时间，但最坏 O(n^2)

// 【易错点】
// 1. 用平均值代替中位数 -> 平均值最小化平方和，中位数最小化绝对值和
// 2. 偶数个元素时纠结选哪个中位数 -> 选哪个结果都一样
// 3. 双指针法忘记排序 -> 必须先排序才能配对

// 【面试追问】
// Q1: 为什么中位数最优？
//     -> 目标从中位数移开，多数一侧增加的 > 少数一侧减少的
// Q2: 二维版本怎么做(LC296)？
//     -> x/y 坐标独立，分别取中位数
// Q3: 如果步长不是1而是k？
//     -> 先检查所有元素 mod k 是否相同，再取中位数计算
