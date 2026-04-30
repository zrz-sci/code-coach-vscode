# 575. Distribute Candies - 分糖果

## 核心思路

Alice 有 n 颗糖，只能吃 n/2 颗，想吃尽可能多的种类。答案就是 **min(糖果种类数, n/2)**。种类数用 HashSet 去重即可。这是一道贪心 + 哈希集合的入门题。

## 思维链

1. **理解约束**：Alice 最多吃 n/2 颗糖
2. **贪心直觉**：要最大化种类数，每种只吃一颗是最优策略
3. **两种限制因素**：
   - 如果种类数 <= n/2：每种吃一颗，答案 = 种类数
   - 如果种类数 > n/2：即使种类再多，也只能吃 n/2 颗，答案 = n/2
4. **综合**：答案 = min(种类数, n/2)
5. **实现**：用 HashSet/unordered_set 统计种类数

## 解法概览

| 解法 | 时间 | 空间 | 适用场景 | 推荐 |
|------|------|------|----------|------|
| HashSet 去重 | O(n) | O(n) | 最直观高效 | ⭐ |
| 排序 + 计数 | O(n log n) | O(1) | 不允许额外空间时 | |
| bitset | O(n) | O(1)* | 值域有限时 | |

## 关键提示

- 本题的关键洞察：**答案只取决于种类数和 n/2 的较小值**，与每种糖的具体数量无关
- n 保证为偶数，不用担心奇偶性
- candyType[i] 范围 [-10^5, 10^5]，值域约 2*10^5，HashSet 完全承受得住
- 排序解法虽然不需要额外空间，但时间复杂度更高，面试中不如 HashSet 简洁

## 解法详解

### 解法一：HashSet 去重（推荐）

```cpp
class Solution {
public:
    int distributeCandies(vector<int>& candyType) {
        unordered_set<int> types(candyType.begin(), candyType.end());
        return min(types.size(), candyType.size() / 2);
    }
};
```

**复杂度分析**：
- 时间 O(n)：遍历一次构建 HashSet
- 空间 O(n)：HashSet 最多存 n 个元素（全不重复时）

### 解法二：排序 + 计数

```cpp
class Solution {
public:
    int distributeCandies(vector<int>& candyType) {
        sort(candyType.begin(), candyType.end());
        int types = 1;
        for (int i = 1; i < candyType.size(); ++i) {
            if (candyType[i] != candyType[i - 1]) {
                ++types;
            }
        }
        return min(types, (int)candyType.size() / 2);
    }
};
```

**复杂度分析**：
- 时间 O(n log n)：排序主导
- 空间 O(1)：原地排序（忽略排序内部栈空间）

### 解法三：bitset（利用值域有限）

```cpp
class Solution {
public:
    int distributeCandies(vector<int>& candyType) {
        // 值域 [-100000, 100000]，偏移后 [0, 200000]
        bitset<200001> seen;
        int types = 0;
        for (int c : candyType) {
            if (!seen[c + 100000]) {
                seen.set(c + 100000);
                ++types;
            }
        }
        return min(types, (int)candyType.size() / 2);
    }
};
```

**复杂度分析**：
- 时间 O(n)：遍历一次
- 空间 O(1)：bitset 大小固定，约 25KB

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ `return types.size()` | ✓ `return min(types.size(), n/2)` | 忘记 n/2 的上界限制 |
| ✗ `candyType.size() / 2` 与 `size_t` 比较 | ✓ 注意 `min` 的两个参数类型一致 | size() 返回 size_t，可能需要类型转换 |
| ✗ 用 map 统计每种数量 | ✓ 用 set 即可 | 不需要知道每种有几颗，只需种类数 |
| ✗ 复杂的贪心模拟分配过程 | ✓ 一行 min 公式 | 过度设计，题目本质极简 |

## 面试追问

**Q1：如果 Alice 必须至少吃 k 种不同的糖，最少需要吃多少颗？如果无法满足返回 -1。**

如果种类数 < k，返回 -1。否则答案就是 k（每种吃一颗）。但如果 k > n/2（超出限制），也返回 -1。

**Q2：如果把问题改成"Alice 和 Bob 各分 n/2 颗，最大化两人种类数之和"，怎么做？**

贪心策略：
- 种类数 types <= n/2 时，一人拿所有种类各一颗，剩余随意给另一人
- types > n/2 时，两人可以分摊不同种类
- 答案 = min(2 * min(types, n/2), n)，需要更细致的分析

**Q3：这道题的时间复杂度能否优化到 O(n) 且空间 O(1)？**

严格 O(1) 额外空间的 O(n) 解法不太可行（除非修改原数组）。排序是 O(n log n)，HashSet 是 O(n) 空间。bitset 虽然大小固定但依赖值域，算常数空间。实际面试中 HashSet O(n)/O(n) 是标准答案。

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 136 | Single Number | 用 HashSet 做去重/统计 |
| 217 | Contains Duplicate | HashSet 判重的基础应用 |
| 349 | Intersection of Two Arrays | HashSet 去重 + 集合操作 |
| 771 | Jewels and Stones | HashSet 统计 + 简单计数 |
| 1431 | Kids With the Greatest Number of Candies | 同为糖果分配类简单题 |
