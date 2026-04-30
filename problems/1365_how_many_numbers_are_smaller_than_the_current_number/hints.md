# 1365. 有多少小于当前数字的数字

## 核心思路

给定数组 `nums`，对每个元素 `nums[i]`，统计数组中有多少个数**严格小于**它。关键观察：**值域非常小** (`0 <= nums[i] <= 100`)，可以用**计数排序 + 前缀和**在 O(n) 时间内完成。

## 思维链

1. **读完题第一反应**：暴力——对每个 `nums[i]`，遍历整个数组数比它小的。O(n^2)，n 最大 500，可以过。但有更优解。

2. **观察值域**：`nums[i]` 范围是 [0, 100]，只有 101 个可能值。可以用计数数组 `count[v]` 统计每个值出现几次。

3. **前缀和的妙用**：`prefix[v] = count[0] + count[1] + ... + count[v-1]` 正好就是"严格小于 v 的元素个数"！对每个 `nums[i]`，答案直接是 `prefix[nums[i]]`。

4. **另一种思路——排序**：先排序，用哈希表记录每个值在排序后数组中**第一次出现的位置**，这个位置就是小于它的元素个数。O(n log n)。

5. **为什么计数排序更优**：O(n + k) vs O(n log n)，且常数更小，不需要额外排序。

## 关键提示

1. **前缀和的含义**：`prefix[v]` 是**严格小于** v 的元素个数，不是小于等于。构造时 `prefix[i] = prefix[i-1] + count[i-1]`，注意是 `count[i-1]` 不是 `count[i]`。

2. **为什么不是 `prefix[i] = prefix[i-1] + count[i]`？**：因为 `count[i]` 是等于 i 的个数，加上去就变成了"小于等于 i"。我们要的是严格小于，所以加的是 `count[i-1]`。

3. **排序法的关键细节**：排序后，相同值的元素应该共享同一个答案。所以需要记录每个值**第一次出现**的索引，而不是任意一次。

4. **示例手动推演**：
   ```
   nums = [8, 1, 2, 2, 3]
   
   Step 1: 计数数组 count[]
     count[1]=1, count[2]=2, count[3]=1, count[8]=1
     (其余为 0)
   
   Step 2: 前缀和 prefix[]
     prefix[0] = 0             (没有比0更小的)
     prefix[1] = 0 + count[0] = 0   (没有比1更小的)
     prefix[2] = 0 + count[1] = 1   (比2小的: {1}, 共1个)
     prefix[3] = 1 + count[2] = 3   (比3小的: {1,2,2}, 共3个)
     ...
     prefix[8] = prefix[7] + count[7] = ... = 4  (比8小的共4个)
   
   Step 3: 查询
     nums[0]=8 → prefix[8] = 4
     nums[1]=1 → prefix[1] = 0
     nums[2]=2 → prefix[2] = 1
     nums[3]=2 → prefix[2] = 1
     nums[4]=3 → prefix[3] = 3
   
   结果: [4, 0, 1, 1, 3] ✓
   ```

5. **全相同元素的边界**：`nums = [7,7,7,7]`，count[7]=4，prefix[7]=0，所以每个元素答案都是 0。正确！

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 | 双重循环 | O(n^2) | O(1) | 能过但不推荐 |
| 排序 + 哈希 | 排序后记录首次位置 | O(n log n) | O(n) | 可以 |
| 计数排序 + 前缀和 | 利用值域小 | O(n + k) | O(k) | 推荐 |

## 解法详解

### 解法1: 暴力 -- O(n^2) / O(1)

**思路**：对每个元素，遍历整个数组统计比它小的数。

```cpp
class Solution {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            int cnt = 0;
            for (int j = 0; j < n; j++) {
                if (nums[j] < nums[i]) cnt++;  // j != i 不需要特判，因为 nums[j] < nums[i] 自动排除等于
            }
            res[i] = cnt;
        }
        return res;
    }
};
```

**为什么不需要 `j != i` 的判断**：题目要求 `j != i` 且 `nums[j] < nums[i]`。当 `j == i` 时，`nums[j] == nums[i]`，不满足 `<`，所以自动跳过。

---

### 解法2: 排序 + 哈希 -- O(n log n) / O(n)

**思路**：排序后，每个元素第一次出现的索引就是比它小的元素个数。

```cpp
class Solution {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int n = nums.size();
        vector<int> sorted_nums = nums;  // 拷贝一份排序
        sort(sorted_nums.begin(), sorted_nums.end());
        
        // 记录每个值第一次出现的位置
        unordered_map<int, int> firstIdx;
        for (int i = 0; i < n; i++) {
            if (firstIdx.find(sorted_nums[i]) == firstIdx.end()) {
                firstIdx[sorted_nums[i]] = i;
            }
        }
        
        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            res[i] = firstIdx[nums[i]];
        }
        return res;
    }
};
```

**为什么要"第一次出现"**：排序后 `[1, 2, 2, 3, 8]`，值 2 出现在索引 1 和 2。索引 1 表示有 1 个比 2 小的元素（就是那个 1）。如果取索引 2，就变成"有 2 个比 2 小"，多算了另一个 2（等于而非小于）。

---

### 解法3: 计数排序 + 前缀和 -- O(n + k) / O(k) 推荐

**思路**：利用值域小 [0,100]，统计频率后做前缀和，直接 O(1) 查询每个值的答案。

```cpp
class Solution {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int count[101] = {};
        for (int x : nums) count[x]++;   // 统计频率
        
        // 前缀和: prefix[i] = sum(count[0..i-1]) = 严格小于 i 的元素个数
        int prefix[101] = {};
        for (int i = 1; i <= 100; i++)
            prefix[i] = prefix[i - 1] + count[i - 1];
        
        vector<int> res;
        for (int x : nums) res.push_back(prefix[x]);
        return res;
    }
};
```

**核心推导**：
- `count[v]` = 数组中等于 v 的元素个数
- `prefix[v] = count[0] + count[1] + ... + count[v-1]` = 严格小于 v 的元素总数
- 对 `nums[i]`，答案就是 `prefix[nums[i]]`

## 解法对比

| | 暴力 | 排序 + 哈希 | 计数排序 + 前缀和 |
|---|---|---|---|
| 时间 | O(n^2) | O(n log n) | O(n + k), k=101 |
| 空间 | O(1) | O(n) | O(k)=O(101) |
| 代码量 | 最短 | 中等 | 中等 |
| 限制条件 | 无 | 无 | 值域小时才适用 |
| 面试推荐 | 作为基线 | 通用解法 | 最优解 |

## 易错点

1. **前缀和方向搞反**：
   ```cpp
   // 错误：这是"小于等于 i"的个数
   prefix[i] = prefix[i-1] + count[i];
   ```
   **正确**：
   ```cpp
   // "严格小于 i"的个数
   prefix[i] = prefix[i-1] + count[i-1];
   ```

2. **排序法忘记取第一次出现的位置**：
   ```cpp
   // 错误：覆盖了第一次出现的位置
   for (int i = 0; i < n; i++) firstIdx[sorted[i]] = i;
   ```
   **正确**：只在 `firstIdx` 中不存在时才写入。

3. **数组越界**：`prefix` 数组大小要 101（index 0~100），不是 100。

## 面试追问

**Q1（基础）**：如果值域不是 [0,100] 而是很大的范围（如 10^9），还能用计数排序吗？
> 不行，计数数组太大。此时用排序 + 哈希法，O(n log n)。或者用**离散化**：先排序去重得到映射，再对映射后的值做计数。

**Q2（变体）**：如果要求的是"小于等于当前数字"的个数呢？
> 前缀和改成 `prefix[v] = count[0] + ... + count[v]`，即 `prefix[i] = prefix[i-1] + count[i]`。

**Q3（变体）**：如果要求的是"排名"（1-based, 相同值同名次）？
> 和"严格小于"的个数 +1 一样。`rank[i] = prefix[nums[i]] + 1`。

**Q4（进阶）**：能否做到 O(1) 空间（不算输出数组）？
> 暴力法 O(n^2) 时间 O(1) 空间。计数排序法的 O(101) 空间可以视为 O(1) 常数空间，因为 101 是固定的不随 n 变化。

## 相关题型

- **1051. 高度检查器** — 同样利用值域小 [1,100]，计数排序思想
- **0315. 计算右侧小于当前元素的个数** — 困难版，需要归并排序或 BIT
- **0493. 翻转对** — 类似思路的归并排序应用
- **0347. 前 K 个高频元素** — 计数 + 桶排序的典型应用
