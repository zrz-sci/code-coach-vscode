# 396. Rotate Function - 旋转函数

## 核心思路
利用相邻F(k)的递推关系：`F(k) = F(k-1) + sum - n * nums[n-k]`，避免重复计算。

## 思维链
1. 暴力：对每个k都算一遍F(k)，O(n^2) -> n=10^5会超时
2. 观察F(k)与F(k-1)的差异：旋转一位时，所有元素的权重+1，除了原来在末尾的元素从权重n-1变为0
3. `F(k) - F(k-1) = sum(nums) - n * nums[n-k]`
4. 先算F(0)和sum，然后O(n)递推所有F(k)

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 递推公式 | O(n) | O(1) | 数学推导，最优解 |
| 暴力枚举 | O(n^2) | O(1) | 直接计算每个F(k) |

## 关键提示
1. **递推关系**：`F(k) = F(k-1) + totalSum - n * nums[n-k]`
2. **推导过程**：顺时针旋转k位后，`arr_k[i] = nums[(i-k+n)%n]`
3. 旋转一位 = 所有系数+1，最后一个元素的系数从n-1归零 = 减去n*该元素
4. 用long防溢出（n=10^5, nums[i]最大100, 最大乘积约10^7*10^5=10^12）

## 解法详解

### 解法一：递推公式（推荐）
```cpp
class Solution {
public:
    int maxRotateFunction(vector<int>& nums) {
        int n = nums.size();
        long sum = 0, f = 0;
        for (int i = 0; i < n; i++) {
            sum += nums[i];
            f += (long)i * nums[i]; // F(0)
        }
        long maxVal = f;
        for (int k = 1; k < n; k++) {
            f = f + sum - (long)n * nums[n - k];
            maxVal = max(maxVal, f);
        }
        return (int)maxVal;
    }
};
```

### 解法二：暴力（仅作对比）
```cpp
class Solution {
public:
    int maxRotateFunction(vector<int>& nums) {
        int n = nums.size(), maxVal = INT_MIN;
        for (int k = 0; k < n; k++) {
            int val = 0;
            for (int i = 0; i < n; i++) {
                val += i * nums[(i - k + n) % n];
            }
            maxVal = max(maxVal, val);
        }
        return maxVal;
    }
};
```

## 易错点
- ✗ 递推中用 `nums[n-k]` 但k从0开始 -> 越界
- ✓ k从1开始，取 `nums[n-k]`，范围正确 [n-1, ..., 1, 0]

- ✗ 所有计算用int -> 中间值溢出
- ✓ 用long存储f和sum的乘积

- ✗ 递推方向搞反 -> `F(k) = F(k-1) + sum - n*nums[n-k]` 不是减sum

## 面试追问

**Q1: 递推公式如何推导？**
> F(k) = 0*arr_k[0] + 1*arr_k[1] + ... + (n-1)*arr_k[n-1]
> 旋转k位：arr_k[i] = nums[(n-k+i)%n]
> F(k) - F(k-1) = sum(nums) - n*nums[n-k]
> 因为每个元素系数+1(贡献+sum)，但最后一个元素从系数n-1变为0(贡献-n*该元素)

**Q2: 如果要求最小值呢？**
> 完全一样的递推，把max换成min即可。

**Q3: 如果数组可以逆时针旋转呢？**
> 逆时针旋转k位 = 顺时针旋转n-k位。所以遍历所有k（0到n-1）已经覆盖了两个方向的所有情况。

## 相关题型
- [53. Maximum Subarray](https://leetcode.com/problems/maximum-subarray/) - 数组递推
- [152. Maximum Product Subarray](https://leetcode.com/problems/maximum-product-subarray/) - DP递推
- [189. Rotate Array](https://leetcode.com/problems/rotate-array/) - 数组旋转
