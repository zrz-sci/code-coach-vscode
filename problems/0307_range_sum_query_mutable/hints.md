# 307. 区域和检索 - 数组可修改 (Range Sum Query - Mutable)

**难度**: Medium | **标签**: Design, Binary Indexed Tree, Segment Tree, Array

## 核心思路

本题是**数据结构设计**的经典题：需要在数组上同时支持**单点更新**和**区间求和**两种操作。朴素的前缀和在更新时需要 O(n)，朴素数组在求和时需要 O(n)，都无法满足频繁操作的效率要求。

两种经典数据结构可以将两种操作都优化到 O(log n)：
- **树状数组 (Binary Indexed Tree / Fenwick Tree)**：实现简单，常数小
- **线段树 (Segment Tree)**：功能更强大，支持区间更新等扩展

## 思维链

1. **需求分析**：update 是单点修改，sumRange 是区间查询 → 经典的「点修改 + 区间查询」问题
2. **排除朴素方案**：前缀和 update O(n)，暴力 sumRange O(n) → 不够高效
3. **选择数据结构**：BIT 或线段树，两者都能做到 O(log n) 单次操作
4. **BIT 更简洁**：对于本题（单点更新 + 区间求和），BIT 代码量更小、常数更优
5. **关键操作**：BIT 的 lowbit(x) = x & (-x) 提取最低位的 1

## 解法概览

| 解法 | 初始化 | update | sumRange | 推荐度 | 说明 |
|------|--------|--------|----------|--------|------|
| 树状数组 (BIT) | O(n) | O(log n) | O(log n) | ⭐⭐⭐⭐⭐ | 代码简洁，常数小 |
| 线段树 | O(n) | O(log n) | O(log n) | ⭐⭐⭐⭐ | 功能更通用 |
| 分块 (Sqrt Decomposition) | O(n) | O(1) | O(√n) | ⭐⭐⭐ | 思路简单但效率略低 |

## 关键提示

1. **BIT 索引从 1 开始**：内部数组大小为 n+1，外部传入的 index 需要 +1
2. **lowbit(x) = x & (-x)**：这是 BIT 的核心，用来跳跃更新/查询
3. **update 的差值传递**：BIT 的 update 传递的是差值 delta，不是新值
4. **sumRange(l, r) = query(r) - query(l-1)**：利用前缀和的差分性质
5. **线段树的递归结构**：节点 i 的左子节点是 2i，右子节点是 2i+1

## 解法详解

### 解法一：树状数组 BIT（推荐）

```cpp
class NumArray {
    vector<int> tree; // BIT 数组，1-indexed
    vector<int> nums; // 原始数组
    int n;
    
    int lowbit(int x) { return x & (-x); }
    
    void add(int i, int delta) {
        for (; i <= n; i += lowbit(i))
            tree[i] += delta;
    }
    
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= lowbit(i))
            sum += tree[i];
        return sum;
    }
    
public:
    NumArray(vector<int>& nums) : nums(nums), n(nums.size()), tree(nums.size() + 1, 0) {
        for (int i = 0; i < n; i++)
            add(i + 1, nums[i]); // BIT 从 1 开始
    }
    
    void update(int index, int val) {
        add(index + 1, val - nums[index]); // 传递差值
        nums[index] = val;
    }
    
    int sumRange(int left, int right) {
        return query(right + 1) - query(left); // 前缀和差分
    }
};
```

### 解法二：线段树

```cpp
class NumArray {
    vector<int> tree;
    int n;
    
    void build(vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            tree[node] = nums[start];
            return;
        }
        int mid = (start + end) / 2;
        build(nums, 2 * node, start, mid);
        build(nums, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    void updateTree(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) updateTree(2 * node, start, mid, idx, val);
        else updateTree(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    
    int queryTree(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        return queryTree(2 * node, start, mid, l, r) +
               queryTree(2 * node + 1, mid + 1, end, l, r);
    }
    
public:
    NumArray(vector<int>& nums) : n(nums.size()), tree(4 * nums.size(), 0) {
        if (n > 0) build(nums, 1, 0, n - 1);
    }
    
    void update(int index, int val) {
        updateTree(1, 0, n - 1, index, val);
    }
    
    int sumRange(int left, int right) {
        return queryTree(1, 0, n - 1, left, right);
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ BIT 数组从 0 开始 | ✓ BIT 数组从 1 开始（大小 n+1） | lowbit(0) = 0 会死循环 |
| ✗ `add(index, val)` 传新值 | ✓ `add(index+1, val - nums[index])` 传差值 | BIT 的 add 是累加，不是赋值 |
| ✗ `query(right) - query(left)` | ✓ `query(right+1) - query(left)` | 注意 0-indexed 到 1-indexed 的转换 |
| ✗ 线段树数组大小为 2n | ✓ 线段树数组大小为 4n | 最坏情况下需要 4n 空间 |
| ✗ 忘记更新 nums 数组 | ✓ 更新 BIT 后也要更新 nums[index] | 下次 update 需要用旧值算差值 |

## 面试追问

**Q1: BIT 和线段树的区别？各自的适用场景？**
→ BIT 只能处理"满足区间减法性质"的操作（如求和、异或），代码简洁常数小。线段树更通用，可以处理区间最值、区间更新（懒标记）等。本题两者都行，推荐 BIT。

**Q2: 如何将 BIT 初始化从 O(n log n) 优化到 O(n)？**
→ 方法：先将 tree 全部置零，然后对每个 i，令 tree[i] = nums[i]，再将 tree[i] 加到 tree[i + lowbit(i)]（如果不越界）。这样每个元素只被操作一次，总计 O(n)。

**Q3: 如果要支持区间更新 + 区间查询呢？**
→ BIT 可以维护差分数组实现区间更新 + 单点查询；但区间更新 + 区间查询需要维护两个 BIT。更通用的做法是线段树 + 懒标记（Lazy Propagation）。

## 相关题型

- [303. Range Sum Query - Immutable](https://leetcode.com/problems/range-sum-query-immutable/) - 不可变版本（前缀和即可）
- [308. Range Sum Query 2D - Mutable](https://leetcode.com/problems/range-sum-query-2d-mutable/) - 二维版本
- [315. Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/) - BIT 经典应用
- [327. Count of Range Sum](https://leetcode.com/problems/count-of-range-sum/) - 线段树/归并排序
