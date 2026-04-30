# 492. Construct the Rectangle - 构造矩形

## 核心思路
给定面积 area，找两个因子 L >= W 使得 L * W = area 且 L - W 最小。最优解就是从 sqrt(area) 向下找第一个能整除 area 的数作为 W。

## 思维链
1. L * W = area，要求 L >= W 且差最小
2. 当 L = W = sqrt(area) 时差最小，但 area 不一定是完全平方数
3. 从 sqrt(area) 向下递减，找到第一个能整除 area 的 W
4. L = area / W

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 从sqrt向下枚举 | O(sqrt(n)) | O(1) | 最优解，简洁直观 |
| 枚举所有因子 | O(sqrt(n)) | O(1) | 思路相同，方向不同 |

## 关键提示
1. 从 sqrt(area) 开始向下找，保证找到的第一个因子就是最接近的
2. W 是较小的那个，L = area / W 是较大的那个
3. 注意 sqrt 的精度问题，用 int(sqrt(area)) 可能需要向上取整检查

## 解法详解

### 解法一：从 sqrt(area) 向下枚举 ⭐
**思路**：W 从 sqrt(area) 开始递减，找到第一个 area % W == 0 的值。

```cpp
class Solution {
public:
    vector<int> constructRectangle(int area) {
        int w = sqrt(area);
        while (area % w != 0) {
            w--;
        }
        return {area / w, w};
    }
};
```

### 解法二：从1向上枚举因子
**思路**：遍历 1 到 sqrt(area) 的所有因子，记录最大的 W。

```cpp
class Solution {
public:
    vector<int> constructRectangle(int area) {
        int w = 1;
        for (int i = 1; i * i <= area; i++) {
            if (area % i == 0) w = i;
        }
        return {area / w, w};
    }
};
```

## 易错点
- ✗ 忘记 L >= W 的约束，返回了 {W, L}
- ✓ W 是小的，L = area/W 是大的，返回 {L, W}

- ✗ 用 float sqrt 导致精度丢失
- ✓ 使用 int 强转后向下搜索即可

## 面试追问

**Q1: 如果要求 L 和 W 都是质数怎么办？**
> 需要先筛质数，然后枚举所有 <= sqrt(area) 的质数作为 W，检查 area/W 是否也是质数。

**Q2: 如果 area 非常大（10^18），如何优化？**
> sqrt 向下枚举的复杂度是 O(sqrt(n))，对于 10^18 约为 10^9，可能需要 Pollard's rho 等因式分解算法。

**Q3: 如何扩展到三维——给定体积找 L >= W >= H 且差最小的长方体？**
> 三重嵌套：先枚举 H（从 cbrt 向下），再对 area/H 用二维方法找 L 和 W。

## 相关题型
- [LeetCode 507] Perfect Number（因子枚举）
- [LeetCode 204] Count Primes（质数筛选）
