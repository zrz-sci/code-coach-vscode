# 164. 最大间距

## 核心思路
本质是利用桶排序/基数排序的思想在 O(n) 时间内找排序后相邻元素的最大差值，关键洞察是最大间距一定 >= ceil((max-min)/(n-1))，所以桶内的间距不用考虑。

## 思维链
1. **暴力思考**：排序后遍历相邻差值取最大值。O(n log n) 但题目要求 O(n)。
2. **线性排序**：O(n) 的排序只有桶排序、基数排序、计数排序。计数排序不适合（值域 0~10^9 太大）。
3. **桶排序思路**：将 n 个数分到 n-1 个桶中，每个桶的大小为 gap = ceil((max-min)/(n-1))。
4. **鸽巢原理**：n 个数放入 n-1 个等宽桶，至少有一个桶是空的。最大间距一定跨越至少一个空桶。
5. **桶内不用排序**：只需要记录每个桶的 min 和 max，最大间距 = max(后一个非空桶的min - 前一个非空桶的max)。
6. **基数排序替代**：也可以用基数排序 O(d*n) 排序后直接扫描，d 是位数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 桶排序 | 鸽巢原理 + 桶间最大差值 | O(n) | O(n) | ⭐ 必知 |
| 基数排序 | 按位排序后扫描相邻差 | O(d*n) | O(n) | 了解 |

## 关键提示
1. **鸽巢原理**：n 个数放 n-1 个等宽桶，必有空桶。最大间距 >= 桶宽，所以只在桶间产生
2. **桶编号**：`bucketIdx = (num - minVal) / bucketSize`，其中 `bucketSize = max(1, (maxVal - minVal) / (n - 1))`
3. **桶数量**：需要 `(maxVal - minVal) / bucketSize + 1` 个桶
4. **空桶跳过**：遍历桶时跳过空桶，用前一个非空桶的 max 和后一个非空桶的 min 计算差值
5. **特殊情况**：n < 2 返回 0，所有元素相同返回 0

```
示例: nums = [3, 6, 9, 1]

min=1, max=9, n=4
bucketSize = (9-1)/(4-1) = 2 (向上取整为3也行)
桶数 = (9-1)/2 + 1 = 5

桶分配:
  桶0 [1,2]: {1}     min=1, max=1
  桶1 [3,4]: {3}     min=3, max=3
  桶2 [5,6]: {6}     min=6, max=6
  桶3 [7,8]: {}      空桶!
  桶4 [9,10]: {9}    min=9, max=9

桶间差值:
  桶0→桶1: 3-1 = 2
  桶1→桶2: 6-3 = 3  *** 最大
  桶2→桶4: 9-6 = 3  *** 最大（跳过空桶3）

答案 = 3
```

## 解法详解

### 解法1: 桶排序 (面试最佳)

**思路过程**：
- 核心思想：最大间距不可能出现在桶内部（因为桶宽 <= 平均间距）
- 所以只需要比较"相邻非空桶"的 max→min 差值
- 每个桶只记录 min 和 max，不用存所有元素

```cpp
class Solution1 {
public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;
        
        int minVal = *min_element(nums.begin(), nums.end());
        int maxVal = *max_element(nums.begin(), nums.end());
        
        // 所有元素相同
        if (minVal == maxVal) return 0;
        
        // 桶大小：保证最大间距一定跨桶
        // 为什么用 max(1, ...)：防止除零
        int bucketSize = max(1, (maxVal - minVal) / (n - 1));
        int bucketCount = (maxVal - minVal) / bucketSize + 1;
        
        // 每个桶只记录 min 和 max
        vector<int> bucketMin(bucketCount, INT_MAX);
        vector<int> bucketMax(bucketCount, INT_MIN);
        vector<bool> used(bucketCount, false);
        
        // 将元素放入桶
        for (int num : nums) {
            int idx = (num - minVal) / bucketSize;
            used[idx] = true;
            bucketMin[idx] = min(bucketMin[idx], num);
            bucketMax[idx] = max(bucketMax[idx], num);
        }
        
        // 扫描桶间最大差值
        int ans = 0;
        int prevMax = bucketMax[0];  // 第0个桶一定非空（包含minVal）
        
        for (int i = 1; i < bucketCount; ++i) {
            if (!used[i]) continue;  // 跳过空桶
            ans = max(ans, bucketMin[i] - prevMax);
            prevMax = bucketMax[i];
        }
        
        return ans;
    }
};
```

**复杂度分析**：
- 时间 O(n)：遍历数组两次 + 遍历桶一次
- 空间 O(n)：桶数量与 n 同阶

### 解法2: 基数排序

**思路过程**：
- 按位从低到高排序（LSD），每一位用计数排序
- 排序完成后扫描相邻差值
- 时间 O(d*n) 其中 d 是最大数字的位数，d <= 10

```cpp
class Solution2 {
public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;
        
        int maxVal = *max_element(nums.begin(), nums.end());
        
        // 基数排序：按每一位排序（LSD）
        vector<int> buf(n);
        long exp = 1;  // 当前位的基数
        
        while (maxVal / exp > 0) {
            vector<int> count(10, 0);
            
            // 计算每个数字在当前位的出现次数
            for (int num : nums) {
                int digit = (num / exp) % 10;
                count[digit]++;
            }
            
            // 前缀和（确定每个数字的排序位置）
            for (int i = 1; i < 10; ++i) {
                count[i] += count[i - 1];
            }
            
            // 从右到左放置（保持稳定排序）
            for (int i = n - 1; i >= 0; --i) {
                int digit = (nums[i] / exp) % 10;
                buf[--count[digit]] = nums[i];
            }
            
            nums = buf;
            exp *= 10;
        }
        
        // 排序完成，扫描最大相邻差
        int ans = 0;
        for (int i = 1; i < n; ++i) {
            ans = max(ans, nums[i] - nums[i - 1]);
        }
        
        return ans;
    }
};
```

**复杂度分析**：
- 时间 O(d*n)：d 是最大值的位数，最多 10 位
- 空间 O(n)：辅助数组

## 易错点

1. **桶大小为 0 导致除零**
```
✗ 错误：int bucketSize = (maxVal - minVal) / (n - 1);
  // 当 maxVal == minVal 时为 0

✓ 正确：int bucketSize = max(1, (maxVal - minVal) / (n - 1));
```

2. **忘记处理 n < 2**
```
✗ 错误：直接计算，n=0 或 n=1 时访问越界
✓ 正确：if (n < 2) return 0;
```

3. **桶编号越界**
```
✗ 错误：int idx = (num - minVal) / bucketSize;
  // 当 num == maxVal 时 idx 可能等于 bucketCount

✓ 正确：确保 bucketCount = (maxVal - minVal) / bucketSize + 1 足够大
```

4. **prevMax 初始化错误**
```
✗ 错误：int prevMax = minVal;
  // 如果桶0有多个元素，prevMax 应该是桶0的 max

✓ 正确：int prevMax = bucketMax[0];
```

## 面试追问

**Q1: 为什么最大间距一定不在桶内部？**
A: 平均间距 = (max-min)/(n-1)，桶宽 <= 平均间距。如果最大间距在桶内，则该桶至少两个元素的差 > 平均间距，但桶宽 <= 平均间距，矛盾。

**Q2: 桶排序和基数排序哪个更好？**
A: 桶排序更优雅（不需要真正排序），基数排序更通用。面试推荐桶排序因为体现了数学思维。

**Q3: 如果数组有负数怎么办？**
A: 桶排序思路不变，只需注意 bucketIdx 的计算用 `(num - minVal)` 保证非负。基数排序需要特殊处理负数。

## 相关题型
- [75. 颜色分类](../0075_sort_colors/) - 线性排序基础
- [347. 前K个高频元素](../0347_top_k_frequent_elements/) - 桶排序应用
- [179. 最大数](../0179_largest_number/) - 排序变体
