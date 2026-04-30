// 【题目本质】
// 找第三大的不同数，考察 top-K 追踪和边界处理（INT_MIN 是合法输入）

// 【解法总览】
// Solution1: 三变量追踪 — O(n) / O(1) — 面试首选
// Solution2: Set 维护 Top-3 — O(n log 3) ≈ O(n) / O(1) — 简洁

// ===================== Solution 1: 三变量追踪 =====================
// 用 optional<long> 避免 INT_MIN 边界问题
class Solution1 {
public:
    int thirdMax(vector<int>& nums) {
        optional<long> first, second, third;
        for (int num : nums) {
            // 跳过已存在的值（去重）
            if ((first && num == *first) ||
                (second && num == *second) ||
                (third && num == *third)) continue;

            if (!first || num > *first) {
                third = second;
                second = first;
                first = num;
            } else if (!second || num > *second) {
                third = second;
                second = num;
            } else if (!third || num > *third) {
                third = num;
            }
        }
        return third ? *third : *first;
    }
};

// ===================== Solution 2: Set 维护 Top-3 =====================
// set 自动排序去重，维护大小 <= 3，淘汰最小值
class Solution2 {
public:
    int thirdMax(vector<int>& nums) {
        set<int> top3;
        for (int num : nums) {
            top3.insert(num);
            if (top3.size() > 3) {
                top3.erase(top3.begin()); // 淘汰最小的
            }
        }
        // size==3 说明有3个不同值，取最小（即第三大）
        // 否则取最大值
        return top3.size() == 3 ? *top3.begin() : *top3.rbegin();
    }
};

// 【解法对比】
// Solution1: 纯 O(1) 空间，但逻辑分支多，容易写错边界
// Solution2: 代码简洁优雅，set 自动处理排序和去重，推荐面试快速写出

// 【易错点】
// 1. INT_MIN 是合法输入！不能用它做"未初始化"标记
//    → 用 optional 或 long long 初始化为 LLONG_MIN
// 2. 必须去重：[2,2,3,1] 第三大是 1 不是 2
// 3. 不满3个不同值时返回最大值，不是返回 -1 或 0

// 【面试追问】
// Q1: 推广到第K大? → set<int> 维护大小K，或 quickselect O(n)
// Q2: 数据流? → 大小为K的最小堆 min-heap
// Q3: 为什么 set.begin() 是第三大? → set 升序，淘汰最小后剩3个，begin是最小=第三大
