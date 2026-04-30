# 414. Third Maximum Number (第三大的数)

## 核心思路
找到数组中**第三大的不同数**。如果不存在，返回最大值。核心是维护三个变量追踪 top-3 的**去重**最大值。

## 思维链
1. 需要找"第三大的**不同**数"，重复值只算一次
2. 方法一：用 set 去重后排序取第3个
3. 方法二：维护 first, second, third 三个变量，一次遍历 O(n)
4. 用 `optional<long long>` 或初始化为 `LLONG_MIN` 避免 INT_MIN 边界问题
5. 如果不满3个不同值，返回最大值

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 三变量追踪 | O(n) | O(1) | 最优，面试首选 |
| Set + 排序 | O(n log n) | O(n) | 简洁但非最优 |

## 关键提示
1. 题目说 `-2^31 <= nums[i] <= 2^31-1`，INT_MIN 是合法输入！
2. 不能用 `INT_MIN` 作为"未设置"标记，要用 `long long` 或 `optional`
3. 去重很重要：`[2,2,3,1]` 的第三大是 1，不是 2

## 解法详解

### 解法一：三变量追踪（推荐）
```cpp
class Solution {
public:
    int thirdMax(vector<int>& nums) {
        // 用 optional 处理"尚未设置"的状态
        optional<long> first, second, third;
        for (int num : nums) {
            if ((first && num == *first) || 
                (second && num == *second) || 
                (third && num == *third)) continue; // 跳过重复
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
```

### 解法二：Set 解法
```cpp
class Solution {
public:
    int thirdMax(vector<int>& nums) {
        set<int> top3;
        for (int num : nums) {
            top3.insert(num);
            if (top3.size() > 3) top3.erase(top3.begin()); // 淘汰最小的
        }
        return top3.size() == 3 ? *top3.begin() : *top3.rbegin();
    }
};
```

## 易错点
- ✗ 用 `INT_MIN` 初始化 → ✓ `INT_MIN` 是合法输入，应该用 `long long` 或 `optional`
- ✗ 忘记去重，重复值多次计入 → ✓ 遇到等于 first/second/third 的值要 skip
- ✗ 不满3个不同值时返回 third → ✓ 应返回**最大值** first

## 面试追问
**Q1: 能否推广到第 K 大？**
A: 用大小为 K 的 `set` 维护 top-K，每次淘汰最小值，时间 O(n log K)。或者用 quickselect O(n) 平均。

**Q2: 如果数据流持续输入怎么办？**
A: 维护大小为 K 的最小堆（min-heap），新元素大于堆顶时替换。参考 LC 703. Kth Largest Element in a Stream。

**Q3: 为什么 set 解法维护大小为 3？**
A: set 有序，始终淘汰最小的，最终 set 中存的就是最大的3个不同值。取 `begin()` 就是第三大。

## 相关题型
- [215. Kth Largest Element in an Array](https://leetcode.com/problems/kth-largest-element-in-an-array/) - 第K大
- [703. Kth Largest Element in a Stream](https://leetcode.com/problems/kth-largest-element-in-a-stream/) - 数据流第K大
- [628. Maximum Product of Three Numbers](https://leetcode.com/problems/maximum-product-of-three-numbers/) - 类似的 top-3 追踪
