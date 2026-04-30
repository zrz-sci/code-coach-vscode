# 327. 区间和的个数 (Hard)

**Tags:** Binary Indexed Tree, Segment Tree, Array, Binary Search, Divide and Conquer, Ordered Set, Merge Sort

## 核心思路

将区间和问题转化为前缀和之差的范围查询：对于每对 (i, j)，`lower <= prefix[j] - prefix[i] <= upper`。利用**归并排序**在合并阶段高效统计满足条件的前缀和对，或使用**有序集合/BIT** 进行在线查询。

## 思维链

1. 区间和 S(i,j) = prefix[j+1] - prefix[i]，需要统计所有满足 lower <= prefix[j] - prefix[i] <= upper 且 i <= j 的对数
2. 暴力 O(n^2) 枚举所有对 → 超时
3. 观察：对于固定的 j，需要在 prefix[0..j] 中找满足 prefix[j]-upper <= prefix[i] <= prefix[j]-lower 的 i 的个数
4. 归并排序：合并两个已排序的半段时，左段的元素作为 prefix[i]，右段作为 prefix[j]，可以用双指针在 O(n) 时间统计
5. 或者用有序数据结构（BIT/线段树/平衡BST）维护已访问的前缀和，每次查询范围内的个数

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| 归并排序 | O(n log n) | O(n) | ⭐ |
| BIT/线段树 + 离散化 | O(n log n) | O(n) | ⭐ |
| 暴力枚举 | O(n^2) | O(n) | - |

## 关键提示

1. **前缀和必须用 long long**：nums[i] 范围 [-2^31, 2^31-1]，累加会溢出 int
2. **归并排序中的双指针**：对于右半段每个 prefix[j]，在左半段中找范围 [prefix[j]-upper, prefix[j]-lower]，左半段已排序故用双指针扫描
3. **初始前缀和**：prefix 数组长度为 n+1，prefix[0] = 0
4. **归并中统计与排序分离**：先统计配对数，再执行标准归并排序
5. **BIT 需要离散化**：前缀和值域大，需先排序去重建立映射

## 解法详解

### 解法一：归并排序 (推荐)

```cpp
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        return mergeSort(prefix, 0, n + 1, lower, upper);
    }
    
private:
    int mergeSort(vector<long long>& prefix, int left, int right, int lower, int upper) {
        if (right - left <= 1) return 0;
        
        int mid = left + (right - left) / 2;
        int count = mergeSort(prefix, left, mid, lower, upper)
                  + mergeSort(prefix, mid, right, lower, upper);
        
        // 统计跨越 mid 的满足条件的对
        // 对于右半段每个 j，在左半段找 prefix[j]-upper <= prefix[i] <= prefix[j]-lower
        int lo = left, hi = left;
        for (int j = mid; j < right; j++) {
            while (lo < mid && prefix[lo] < prefix[j] - upper) lo++;
            while (hi < mid && prefix[hi] <= prefix[j] - lower) hi++;
            count += hi - lo;
        }
        
        // 标准归并排序
        vector<long long> temp(right - left);
        int p1 = left, p2 = mid, idx = 0;
        while (p1 < mid && p2 < right) {
            if (prefix[p1] <= prefix[p2]) temp[idx++] = prefix[p1++];
            else temp[idx++] = prefix[p2++];
        }
        while (p1 < mid) temp[idx++] = prefix[p1++];
        while (p2 < right) temp[idx++] = prefix[p2++];
        copy(temp.begin(), temp.end(), prefix.begin() + left);
        
        return count;
    }
};
```

### 解法二：BIT + 离散化

```cpp
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        // 收集所有需要查询的值进行离散化
        set<long long> allVals(prefix.begin(), prefix.end());
        for (auto& p : prefix) {
            allVals.insert(p - lower);
            allVals.insert(p - upper);
        }
        
        unordered_map<long long, int> ranks;
        int rank = 1;
        for (long long v : allVals) ranks[v] = rank++;
        
        vector<int> bit(rank + 1, 0);
        int count = 0;
        
        // 从左到右遍历前缀和
        for (int i = 0; i <= n; i++) {
            // 查询范围 [prefix[i]-upper, prefix[i]-lower] 内的前缀和个数
            int lo = ranks[prefix[i] - upper];
            int hi = ranks[prefix[i] - lower];
            count += query(bit, hi) - query(bit, lo - 1);
            // 将当前前缀和加入 BIT
            update(bit, ranks[prefix[i]], 1);
        }
        
        return count;
    }
    
private:
    void update(vector<int>& bit, int i, int val) {
        for (; i < (int)bit.size(); i += i & (-i))
            bit[i] += val;
    }
    
    int query(vector<int>& bit, int i) {
        int sum = 0;
        for (; i > 0; i -= i & (-i))
            sum += bit[i];
        return sum;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 原因 |
|----------|----------|------|
| ✗ 前缀和用 `int` | ✓ 用 `long long` | nums[i] 可达 2^31-1，累加严重溢出 |
| ✗ 归并时先排序再统计 | ✓ 先统计再排序 | 排序后打乱了原有顺序，无法正确统计 |
| ✗ prefix 长度为 n | ✓ 长度为 n+1，prefix[0]=0 | 漏掉从头开始的区间 |
| ✗ 双指针边界 `<=` vs `<` 搞混 | ✓ lo 用 `<`，hi 用 `<=` | 左闭右开区间计数 |

## 面试追问

**Q1: 归并排序中为什么双指针不会回退？**
→ 因为右半段 j 递增时，prefix[j] 在排序后的右半段中递增，所以 prefix[j]-upper 和 prefix[j]-lower 都在递增，lo 和 hi 只会向右移动。

**Q2: 能否用平衡 BST (如 C++ 的 `multiset`) 代替 BIT？**
→ 可以，用 `order_of_key` 或手动用 `lower_bound`/`upper_bound` + `distance`，但 `distance` 是 O(n) 的。需要用 policy-based tree (`__gnu_pbds::tree`) 才能 O(log n)。

**Q3: 空间能否优化到 O(1)？**
→ 不能。归并排序至少需要 O(n) 辅助数组。但可以用原地归并(复杂度退化)或迭代归并减少栈深。

## 相关题型

- [315. 计算右侧小于当前元素的个数](https://leetcode.com/problems/count-of-smaller-numbers-after-self/) - 归并排序
- [493. 翻转对](https://leetcode.com/problems/reverse-pairs/) - 归并排序统计
- [53. 最大子数组和](https://leetcode.com/problems/maximum-subarray/) - 前缀和 / 分治
