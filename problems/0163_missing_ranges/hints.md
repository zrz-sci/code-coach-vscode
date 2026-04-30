# 163. 缺失的区间

## 核心思路
本质是遍历有序数组，检查相邻元素之间（以及首尾边界）是否存在间隙，将间隙作为区间收集起来。

## 思维链
1. **理解题意**：给定排序数组和 [lower, upper] 范围，找出所有不在数组中的数字区间。
2. **观察间隙**：如果当前数字是 nums[i]，下一个数字是 nums[i+1]，那么 [nums[i]+1, nums[i+1]-1] 就是缺失区间（如果非空的话）。
3. **处理边界**：lower 到 nums[0] 之间可能有缺失，nums[n-1] 到 upper 之间也可能有缺失。
4. **统一处理**：在 nums 前面"虚拟插入" lower-1，后面"虚拟插入" upper+1，然后统一检查相邻间隙。
5. **空数组**：如果 nums 为空，直接返回 [lower, upper]。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 | 遍历数组检查间隙 | O(n) | O(1) | ⭐ 必知 |

## 关键提示
1. **间隙条件**：两个相邻"锚点"之间差 >= 2 时存在缺失区间
2. **边界虚拟化**：把 lower-1 和 upper+1 当作数组的虚拟边界，统一处理逻辑
3. **返回格式**：新版题目要求返回 `vector<vector<int>>`，每个缺失区间是 {start, end}
4. **注意整数溢出**：lower 和 upper 的范围是 [-10^9, 10^9]，但 lower-1 不会溢出 int 范围

```
示例: nums = [0, 1, 3, 50, 75], lower = 0, upper = 99

锚点:     (lower-1=-1)  0  1  3  50  75  (upper+1=100)
间隙:               无   无  [2,2] [4,49] [51,74] [76,99]

逐步检查:
prev = lower - 1 = -1
nums[0] = 0:   0 - (-1) = 1, 差1无间隙, prev = 0
nums[1] = 1:   1 - 0 = 1, 差1无间隙, prev = 1
nums[2] = 3:   3 - 1 = 2, 差2有间隙! → [2, 2], prev = 3
nums[3] = 50:  50 - 3 = 47, 有间隙! → [4, 49], prev = 50
nums[4] = 75:  75 - 50 = 25, 有间隙! → [51, 74], prev = 75
最后: upper+1 = 100, 100 - 75 = 25, 有间隙! → [76, 99]
```

## 解法详解

### 解法1: 线性扫描 (统一边界处理)

**思路过程**：
- 维护一个 prev 变量，初始为 lower - 1
- 遍历 nums，每次检查 nums[i] - prev 是否 >= 2
- 最后检查 upper + 1 - prev 是否 >= 2
- 间隙区间为 [prev + 1, nums[i] - 1]

```cpp
class Solution1 {
public:
    vector<vector<int>> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<vector<int>> result;
        int prev = lower - 1;  // 虚拟左边界
        
        // 在 nums 后面加一个虚拟的 upper+1
        // 统一处理所有间隙（包括最后一段）
        for (int i = 0; i <= (int)nums.size(); ++i) {
            int curr = (i < (int)nums.size()) ? nums[i] : upper + 1;
            
            if (curr - prev >= 2) {
                // [prev+1, curr-1] 是缺失区间
                result.push_back({prev + 1, curr - 1});
            }
            
            prev = curr;
        }
        
        return result;
    }
};
```

**复杂度分析**：
- 时间 O(n)：遍历数组一次
- 空间 O(1)：不算返回结果的空间

### 解法2: 分步处理 (逻辑更清晰)

**思路过程**：
- 先处理 lower 到 nums[0] 的间隙
- 再处理相邻元素间的间隙
- 最后处理 nums[n-1] 到 upper 的间隙

```cpp
class Solution2 {
public:
    vector<vector<int>> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<vector<int>> result;
        int n = nums.size();
        
        if (n == 0) {
            // 整个范围都是缺失的
            result.push_back({lower, upper});
            return result;
        }
        
        // 1. lower 到 nums[0] 之间的间隙
        if (nums[0] > lower) {
            result.push_back({lower, nums[0] - 1});
        }
        
        // 2. 相邻元素之间的间隙
        for (int i = 1; i < n; ++i) {
            if (nums[i] - nums[i - 1] >= 2) {
                result.push_back({nums[i - 1] + 1, nums[i] - 1});
            }
        }
        
        // 3. nums[n-1] 到 upper 之间的间隙
        if (nums[n - 1] < upper) {
            result.push_back({nums[n - 1] + 1, upper});
        }
        
        return result;
    }
};
```

**复杂度分析**：
- 时间 O(n)
- 空间 O(1)

## 易错点

1. **忘记处理空数组**
```
✗ 错误：直接访问 nums[0] 没有判空
✓ 正确：nums 为空时返回 [{lower, upper}]
```

2. **间隙判断条件写错**
```
✗ 错误：if (curr - prev > 0) → 差1也算间隙
  // nums=[0,1], lower=0, upper=1 → 不应有间隙

✓ 正确：if (curr - prev >= 2) → 差至少为2才有间隙
```

3. **缺失区间的起止写反**
```
✗ 错误：result.push_back({prev, curr});
✓ 正确：result.push_back({prev + 1, curr - 1});
```

4. **遗漏最后一段**
```
✗ 错误：只遍历 nums 内部间隙，忘了 nums[n-1] 到 upper
✓ 正确：在循环后补上 upper 的处理（或用虚拟边界统一处理）
```

## 面试追问

**Q1: 如果 nums 未排序怎么办？**
A: 先排序 O(n log n)，再用同样的线性扫描。或者用 HashSet O(n) 然后从 lower 到 upper 扫描，但 upper-lower 可能很大。

**Q2: 如果要求返回字符串格式 "a->b" 怎么办？**
A: 旧版题目格式。单个数字返回 "a"，范围返回 "a->b"。只需修改输出格式即可。

**Q3: 如果 lower 和 upper 是 long long 范围怎么办？**
A: 注意 lower - 1 和 upper + 1 不能溢出，用 long long 存储 prev 和 curr。

## 相关题型
- [228. 汇总区间](../0228_summary_ranges/) - 反过来，把连续区间合并成范围
- [56. 合并区间](../0056_merge_intervals/) - 区间合并经典题
- [352. 将数据流变为多个不相交区间](../0352_data_stream_as_disjoint_intervals/) - 动态维护区间
