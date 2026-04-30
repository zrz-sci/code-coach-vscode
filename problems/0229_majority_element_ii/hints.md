# 229. 多数元素 II (Medium)

**Tags:** Array, Hash Table, Counting, Sorting

## 核心思路

找出数组中所有出现超过 n/3 次的元素。关键洞察：**最多只有 2 个**这样的元素（因为 3 个各超过 n/3 就超过 n 了）。最优解是 **Boyer-Moore 投票算法的扩展版**：维护两个候选人和计数器，最后验证候选人是否真的超过 n/3。

## 思维链

1. **暴力思考**：用 HashMap 统计每个元素出现次数，返回超过 n/3 的。时间 O(N)，空间 O(N)。
2. **排序方法**：排序后相同元素相邻，扫描一遍计数。时间 O(N log N)，空间 O(1)。
3. **关键观察**：超过 n/3 的元素最多有 2 个。因为如果有 3 个，各自 > n/3，总数 > n，矛盾。
4. **Boyer-Moore 扩展**：维护两个候选人 (cand1, cand2) 和对应计数器 (cnt1, cnt2)。遍历时：
   - 如果等于某个候选人，对应计数器 +1
   - 如果某个计数器为 0，替换候选人
   - 否则两个计数器都 -1（三三抵消）
5. **二次验证**：候选人不一定满足条件，需要再遍历一次确认出现次数 > n/3。
6. **最终方案**：Boyer-Moore 投票法 O(N) 时间 O(1) 空间。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Boyer-Moore 投票扩展 ⭐ | O(N) | O(1) | 最优，面试首选 |
| HashMap 计数 | O(N) | O(N) | 简单直接 |
| 排序 + 扫描 | O(N log N) | O(1) | 不需要额外空间 |

## 关键提示

### Boyer-Moore 投票原理（三三抵消）

```
类比：投票选举，n/3 以上的才能当选，最多选 2 人

数组: [1, 1, 1, 3, 3, 2, 2, 2]  n=8, n/3=2

步骤:
i=0: num=1, cand1=1,cnt1=1, cand2=_,cnt2=0
i=1: num=1, cand1=1,cnt1=2, cand2=_,cnt2=0
i=2: num=1, cand1=1,cnt1=3, cand2=_,cnt2=0
i=3: num=3, cand1=1,cnt1=3, cand2=3,cnt2=1
i=4: num=3, cand1=1,cnt1=3, cand2=3,cnt2=2
i=5: num=2, 两个都不匹配 → cnt1=2,cnt2=1  (三三抵消)
i=6: num=2, 两个都不匹配 → cnt1=1,cnt2=0  (三三抵消)
i=7: num=2, cnt2=0 → cand2=2,cnt2=1

候选人: cand1=1, cand2=2
验证: count(1)=3>2 ✓, count(2)=3>2 ✓
答案: [1, 2]
```

### 为什么需要二次验证

```
数组: [1, 2, 3, 4, 5]  n=5, n/3=1

Boyer-Moore 结束后候选人可能是 [4, 5]
但 count(4)=1 不大于 1，count(5)=1 不大于 1
所以答案为空 []

→ 候选人只是"有可能"超过 n/3 的元素，必须验证
```

## 解法详解

### 解法1: Boyer-Moore 投票扩展 (推荐)

**思路**：维护两个候选人，遇到相同的 +1，遇到不同的三三抵消，最后验证。

```cpp
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int n = nums.size();
        int cand1 = 0, cand2 = 1;  // 初始化为不同值
        int cnt1 = 0, cnt2 = 0;
        
        // 第一轮：找候选人
        for (int num : nums) {
            if (num == cand1) {
                cnt1++;
            } else if (num == cand2) {
                cnt2++;
            } else if (cnt1 == 0) {
                cand1 = num;
                cnt1 = 1;
            } else if (cnt2 == 0) {
                cand2 = num;
                cnt2 = 1;
            } else {
                cnt1--;
                cnt2--;
            }
        }
        
        // 第二轮：验证候选人
        cnt1 = cnt2 = 0;
        for (int num : nums) {
            if (num == cand1) cnt1++;
            else if (num == cand2) cnt2++;
        }
        
        vector<int> result;
        if (cnt1 > n / 3) result.push_back(cand1);
        if (cnt2 > n / 3) result.push_back(cand2);
        return result;
    }
};
```

### 解法2: HashMap 计数

**思路**：统计每个元素出现次数，返回超过 n/3 的。

```cpp
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int n = nums.size();
        unordered_map<int, int> count;
        vector<int> result;
        
        for (int num : nums) {
            count[num]++;
        }
        
        for (auto& [val, cnt] : count) {
            if (cnt > n / 3) {
                result.push_back(val);
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `cand1 = 0, cand2 = 0` 初始化相同 | ✓ `cand1 = 0, cand2 = 1` 初始化不同值 | 两个候选人必须初始不同，否则逻辑混乱 |
| ✗ 跳过第二轮验证 | ✓ 必须验证 `count > n/3` | 候选人不一定满足条件 |
| ✗ if-else 顺序错误：先检查 cnt==0 | ✓ 先检查 `num==cand`，再检查 `cnt==0` | 必须优先匹配已有候选人 |
| ✗ `cnt > n / 3` 用 `>=` | ✓ 严格大于 `>` | 题目要求"超过 n/3 次" |
| ✗ 验证时用 `if...if` 两个独立判断 | ✓ 用 `if...else if` | 避免 cand1==cand2 时重复计数 |

## 面试追问

**Q1: 如何推广到出现超过 n/k 次的元素？**
→ 维护 k-1 个候选人和计数器，遇到不同的 k 个一起抵消。最后验证所有候选人。时间 O(N*k)，空间 O(k)。

**Q2: Boyer-Moore 投票法的数学证明？**
→ 如果某元素出现 > n/3 次，它不可能在所有"三三抵消"中被完全消除（因为其他元素总数 < 2n/3，不够抵消 > n/3 的元素）。所以它一定会留在候选人中。

**Q3: 如果数组是流式的（不能二次遍历），如何处理？**
→ 第二轮验证需要重新遍历。如果不能二次遍历，Boyer-Moore 只能给出候选人，不能保证正确性。可以用概率方法或需要额外信息（如已知一定存在多数元素）。

## 相关题型

- [169. Majority Element](https://leetcode.com/problems/majority-element/) - 经典 Boyer-Moore（n/2）
- [1150. Check If a Number Is Majority Element in a Sorted Array](https://leetcode.com/problems/check-if-a-number-is-majority-element-in-a-sorted-array/) - 排序数组中的多数元素
- [1287. Element Appearing More Than 25% of the Time in a Sorted Array](https://leetcode.com/problems/element-appearing-more-than-25-of-the-time-in-a-sorted-array/) - n/4 变体
