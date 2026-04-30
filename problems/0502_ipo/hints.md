# 502. IPO

[LeetCode 链接](https://leetcode.com/problems/ipo/)

## 核心思路

**贪心 + 排序 + 大顶堆**: 每一轮从「当前资本可启动」的所有项目中，选利润最大的执行。按 capital 升序排列项目后，用一个递增指针逐步解锁项目、推入大顶堆，堆顶即为最优选择。

核心正确性来自 **交换论证**: profit >= 0 保证资本单调递增，选利润最大的项目后，当前能做的项目以后**也一定能做**，但反过来不成立。因此贪心不差于任何其他策略。

---

## 思维链

```
输入: k (最多做 k 个项目), w (初始资本), profits[], capital[]

Step 1  按 capital 升序排序 (索引排序保持 profits 对应关系)
          → 排好后低成本项目在前, 高成本项目在后

Step 2  初始化: 大顶堆 maxHeap (空), 指针 ptr = 0

Step 3  循环 k 轮:
        ┌─ (a) 推进 ptr: 只要 capital[idx[ptr]] <= w, push profits 入堆
        │      → 因为 w 只增不减, ptr 只往右走 (摊还 O(n))
        ├─ (b) 堆为空? → break (当前资本无法启动任何项目, 后续也不会解锁新项目)
        └─ (c) 取堆顶 profit, w += profit (资本增长, 可能解锁更多项目)

Step 4  返回 w

时间: O(n log n) 排序 + O(n log n) 堆操作
空间: O(n) 索引数组 + 堆
```

**为什么 ptr 只向右移?** 资本只增不减，已解锁的项目不会变回"未解锁"，新解锁的项目一定排在 ptr 右边。

---

## 解法概览

| 解法 | 时间 | 空间 | 推荐 | 说明 |
|------|------|------|------|------|
| 排序 + 大顶堆 (贪心) | O(n log n) | O(n) | ⭐ | 最优解, 堆维护可选集合 |
| 暴力线性扫描 | O(nk) | O(1) | | 每轮遍历找最大可行项目, k 大时 TLE |
| 枚举所有子集 | O(2^n) | O(n) | | 指数级, 完全不可行 |

---

## 关键提示

1. **贪心策略**: 每轮选当前可启动项目中利润最大的。利润 >= 0 保证贪心正确性。
2. **排序 + 指针**: 按 capital 排序后, 指针从左到右单向扫描, 每个项目最多入堆一次。
3. **提前终止**: 堆为空时 break -- profit >= 0, w 不再增长, 无法解锁新项目。
4. **索引排序**: 用 `iota + sort` 排索引, 不需要复制数据。或打包成 `pair<int,int>` 排序。

---

## 解法详解

### 解法一: 排序 + 大顶堆 (推荐)

```cpp
class Solution {
public:
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        int n = profits.size();

        // 按 capital 升序排列项目索引
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return capital[a] < capital[b];
        });

        // 大顶堆: 存当前可执行项目的利润
        priority_queue<int> maxHeap;
        int ptr = 0;

        for (int i = 0; i < k; i++) {
            // 解锁所有 capital <= w 的项目
            while (ptr < n && capital[idx[ptr]] <= w) {
                maxHeap.push(profits[idx[ptr]]);
                ptr++;
            }

            // 堆为空: 无可做项目, 提前终止
            if (maxHeap.empty()) break;

            // 选利润最大的项目执行
            w += maxHeap.top();
            maxHeap.pop();
        }

        return w;
    }
};
```

### 解法二: pair 排序写法

```cpp
class Solution {
public:
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        int n = profits.size();
        vector<pair<int, int>> projects(n);
        for (int i = 0; i < n; i++) {
            projects[i] = {capital[i], profits[i]};
        }
        sort(projects.begin(), projects.end());

        priority_queue<int> maxHeap;
        int ptr = 0;

        for (int i = 0; i < k; i++) {
            while (ptr < n && projects[ptr].first <= w) {
                maxHeap.push(projects[ptr].second);
                ptr++;
            }
            if (maxHeap.empty()) break;
            w += maxHeap.top();
            maxHeap.pop();
        }
        return w;
    }
};
```

**干跑示例**: k=2, w=0, profits=[1,2,3], capital=[0,1,1]

```
排序后: idx[0]=0(cap=0,profit=1), idx[1]=1(cap=1,profit=2), idx[2]=2(cap=1,profit=3)

第 1 轮 (w=0):
  解锁 cap=0<=0 → push(1), ptr=1 | cap=1>0 → 停
  堆={1}, 取堆顶 1, w = 0+1 = 1

第 2 轮 (w=1):
  解锁 cap=1<=1 → push(2), ptr=2 | cap=1<=1 → push(3), ptr=3
  堆={3,2}, 取堆顶 3, w = 1+3 = 4

返回 4 ✓
```

---

## 易错点

| 错误写法 (✗) | 正确写法 (✓) | 说明 |
|-------------|-------------|------|
| ✗ `priority_queue<int, vector<int>, greater<int>>` | ✓ `priority_queue<int>` | 需要大顶堆取最大利润, 不是小顶堆 |
| ✗ 排序 capital 但没同步 profits | ✓ 用索引排序或 pair 排序 | 两个数组必须保持对应关系 |
| ✗ ptr 每轮从 0 开始 | ✓ ptr 是全局递增的, 跨轮次保持 | 资本只增不减, 不需要回头扫描 |
| ✗ 堆为空时继续 `maxHeap.top()` | ✓ `if (maxHeap.empty()) break` | 空堆调用 top() 是 UB, 导致 crash |
| ✗ 忘记排序直接用指针 | ✓ 必须先按 capital 排序 | 否则指针技巧失效, 退化为 O(nk) |

---

## 面试追问

**Q1: 为什么贪心正确?**

交换论证: 假设最优解某轮选了利润较小的 p, 而贪心选了利润最大的 q。由于 profit >= 0, 资本单调递增, 选 q 后资本 >= 选 p 后的资本, 仍然能选 p。因此贪心结果 >= 最优解 (矛盾), 所以贪心就是最优。

**Q2: 如果利润可以为负, 怎么处理?**

贪心失效。资本可能减少导致后续项目无法启动, 需要 DP: `dp[i][j]` 表示前 i 个项目选 j 个的最大资本。或者用 0-1 背包变种, 但复杂度更高。

**Q3: 如果项目有依赖关系 (先做 A 才能做 B), 怎么改?**

拓扑排序 + 贪心。维护入度为 0 的项目集合, 从中用堆选利润最大的执行, 执行后更新依赖项的入度, 新入度为 0 的项目入堆。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 871 | Minimum Number of Refueling Stops | 贪心 + 大顶堆, 非常类似 |
| 1353 | Maximum Number of Events That Can Be Attended | 排序 + 贪心 + 堆 |
| 630 | Course Schedule III | 贪心 + 堆维护最优集合 |
| 253 | Meeting Rooms II | 排序 + 堆经典模式 |
| 857 | Minimum Cost to Hire K Workers | 排序 + 堆 + 贪心 |
