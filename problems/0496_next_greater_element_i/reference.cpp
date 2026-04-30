/*
 * 【题目本质】
 * nums1 是 nums2 的子集，对 nums1 中每个元素，找它在 nums2 中右侧第一个更大的元素。
 * 经典"单调栈 + 哈希表"模板题。
 *
 * 【解法总览】
 * 1. 单调栈 + 哈希表 — O(m+n), 预处理 nums2 所有元素的 next greater
 * 2. 暴力查找 — O(m*n), 对每个元素线性扫描
 */

// ===================== Solution 1: 单调栈 + 哈希表 =====================
// 时间: O(m + n)  空间: O(n)
// 核心: 单调递减栈遍历 nums2, 遇到更大的就弹出并记录映射
class Solution1 {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> nextGreater;
        stack<int> stk; // 单调递减栈 (栈底到栈顶递减)

        for (int num : nums2) {
            // 当前元素比栈顶大 -> 栈顶找到了 next greater
            while (!stk.empty() && stk.top() < num) {
                nextGreater[stk.top()] = num;
                stk.pop();
            }
            stk.push(num);
        }
        // 栈中剩余元素没有 next greater, 默认 -1

        vector<int> res;
        for (int num : nums1) {
            res.push_back(nextGreater.count(num) ? nextGreater[num] : -1);
        }
        return res;
    }
};

// ===================== Solution 2: 暴力查找 =====================
// 时间: O(m * n)  空间: O(1) (不计输出)
// 核心: 对 nums1 每个元素, 在 nums2 中定位后向右扫描
class Solution2 {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        for (int x : nums1) {
            bool found = false;
            bool foundX = false;
            for (int y : nums2) {
                if (y == x) foundX = true;
                if (foundX && y > x) {
                    res.push_back(y);
                    found = true;
                    break;
                }
            }
            if (!found) res.push_back(-1);
        }
        return res;
    }
};

/*
 * 【解法对比】
 * Solution1: O(m+n) 最优, 单调栈经典模板, 面试必会
 * Solution2: O(m*n) 暴力, 仅适合 n <= 1000 的小数据
 *
 * 【易错点】
 * 1. 单调栈方向: 维护递减栈, 遇到更大元素弹出记录
 * 2. 不在 map 中的元素 -> 返回 -1 (没有 next greater)
 * 3. 元素唯一, 可直接用值做 key; 若有重复需改用下标
 *
 * 【面试追问】
 * Q1: 循环数组 (LC503)? — 遍历 2n 个元素, idx % n
 * Q2: 找"下一个更小"? — 改用单调递增栈
 * Q3: 时间复杂度 O(n) 证明? — 每个元素最多入栈出栈各一次, 均摊 O(n)
 */
