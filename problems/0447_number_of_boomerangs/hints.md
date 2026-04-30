# 447. 回旋镖的数量 (Number of Boomerangs)

## 核心思路
对每个点 i，统计到其他所有点的距离，距离相同的点可以任选两个作为 j 和 k（有序），排列数为 n*(n-1)。

## 思维链
1. 暴力三重循环 O(n³) → 能否优化？
2. 固定 i，统计距离出现次数 → 哈希表
3. 距离相同的 cnt 个点中选 2 个有序排列 → cnt*(cnt-1)
4. 不需要开根号，用距离平方即可避免浮点误差

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| Solution1 | 哈希表统计距离 ⭐ | O(n²) | O(n) | 最优解 |
| Solution2 | 暴力枚举三元组 | O(n³) | O(1) | 超时 |

## 关键提示
1. **用距离平方代替实际距离**：避免浮点运算，整数比较更安全
2. **排列而非组合**：(i,j,k) 有序，cnt 个同距点贡献 cnt*(cnt-1)
3. **每次固定 i 清空哈希表**：哈希表只统计到当前 i 的距离

## 解法详解

### Solution 1: 哈希表统计距离 ⭐
**思路**：枚举每个点作为回旋镖的中心点 i，用哈希表统计到其他所有点的距离平方，对每个距离值 cnt 累加 cnt*(cnt-1)。

```cpp
class Solution {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        int res = 0;
        for (auto& p : points) {
            unordered_map<int, int> cnt;
            for (auto& q : points) {
                int dx = p[0] - q[0], dy = p[1] - q[1];
                cnt[dx * dx + dy * dy]++;
            }
            for (auto& [d, c] : cnt) {
                res += c * (c - 1);
            }
        }
        return res;
    }
};
```

### Solution 2: 暴力枚举（会超时）
**思路**：三重循环枚举 (i, j, k)，检查 dist(i,j) == dist(i,k)。

```cpp
class Solution {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        int res = 0, n = points.size();
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++) {
                    if (i == j || i == k || j == k) continue;
                    int d1 = (points[i][0]-points[j][0])*(points[i][0]-points[j][0])
                           + (points[i][1]-points[j][1])*(points[i][1]-points[j][1]);
                    int d2 = (points[i][0]-points[k][0])*(points[i][0]-points[k][0])
                           + (points[i][1]-points[k][1])*(points[i][1]-points[k][1]);
                    if (d1 == d2) res++;
                }
        return res;
    }
};
```

## 易错点
- ✗ 使用 `sqrt` 计算实际距离 → ✓ 使用距离平方，避免浮点误差
- ✗ 用组合数 C(cnt,2) 计算 → ✓ 有序排列 cnt*(cnt-1)
- ✗ 忘记每次换 i 时清空哈希表 → ✓ 在内层循环前重新构建哈希表

## 面试追问
**Q1: 为什么用距离平方而不是实际距离？**
A: 避免浮点精度问题。两个 int 坐标差的平方和仍是 int，比较精确。

**Q2: 为什么是 cnt*(cnt-1) 而不是 cnt*(cnt-1)/2？**
A: 因为题目要求元组有序 (i,j,k)，j 和 k 的顺序不同算不同回旋镖，所以是排列 P(cnt,2)。

**Q3: 能否进一步优化时间复杂度？**
A: O(n²) 已经是下界，因为必须计算每对点之间的距离。除非数据有特殊分布，否则无法突破。

## 相关题型
- [149. 直线上最多的点数](https://leetcode.com/problems/max-points-on-a-line/) - 哈希表+几何
- [356. 直线镜像](https://leetcode.com/problems/line-reflection/) - 哈希表+坐标
