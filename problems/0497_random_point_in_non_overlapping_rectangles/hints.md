# 497. Random Point in Non-overlapping Rectangles - 非重叠矩形中的随机点

## 核心思路
要等概率选取所有矩形覆盖的整数点中的一个。先按每个矩形包含的整数点数量做前缀和加权，用二分查找随机选中哪个矩形，再在该矩形内均匀随机一个整数点。

## 思维链
1. 每个矩形包含的整数点数 = (x-a+1) * (y-b+1)
2. 要等概率选点 -> 先按点数加权随机选矩形
3. 前缀和 + 二分查找 -> 高效定位哪个矩形
4. 在选中的矩形内，x 方向和 y 方向各均匀随机

## 解法概览表

| 解法 | 构造复杂度 | pick 复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|-----------|------|
| ⭐ 前缀和 + 二分 | O(n) | O(log n) | O(n) | 最优解 |
| 展平索引 | O(n) | O(log n) | O(n) | 思路不同但等价 |

## 关键提示
1. 整数点数 = (x2 - x1 + 1) * (y2 - y1 + 1)，注意 +1
2. 前缀和数组存累计点数，二分找到目标矩形
3. 在矩形内随机：`x = a + rand() % (x2-x1+1)`
4. 使用 `rand()` 或 C++ `<random>` 生成随机数

## 解法详解

### 解法一：前缀和 + 二分查找 ⭐
**思路**：构造时计算每个矩形的整数点数前缀和。pick 时随机一个 [1, total] 的数，二分定位矩形，再在矩形内随机坐标。

```cpp
class Solution {
public:
    vector<vector<int>> rects;
    vector<int> prefixSum;
    int total;
    
    Solution(vector<vector<int>>& rects) : rects(rects) {
        total = 0;
        for (auto& r : rects) {
            total += (r[2] - r[0] + 1) * (r[3] - r[1] + 1);
            prefixSum.push_back(total);
        }
    }
    
    vector<int> pick() {
        int target = rand() % total;
        int idx = upper_bound(prefixSum.begin(), prefixSum.end(), target) - prefixSum.begin();
        auto& r = rects[idx];
        int x = r[0] + rand() % (r[2] - r[0] + 1);
        int y = r[1] + rand() % (r[3] - r[1] + 1);
        return {x, y};
    }
};
```

### 解法二：展平索引法
**思路**：将所有点编号 0 到 total-1，随机一个编号，反推出它属于哪个矩形的哪个坐标。

```cpp
class Solution {
public:
    vector<vector<int>> rects;
    vector<int> prefixSum;
    int total;
    
    Solution(vector<vector<int>>& rects) : rects(rects) {
        total = 0;
        for (auto& r : rects) {
            total += (r[2] - r[0] + 1) * (r[3] - r[1] + 1);
            prefixSum.push_back(total);
        }
    }
    
    vector<int> pick() {
        int target = rand() % total;
        int idx = upper_bound(prefixSum.begin(), prefixSum.end(), target) - prefixSum.begin();
        auto& r = rects[idx];
        int offset = target - (idx > 0 ? prefixSum[idx - 1] : 0);
        int width = r[2] - r[0] + 1;
        int x = r[0] + offset % width;
        int y = r[1] + offset / width;
        return {x, y};
    }
};
```

## 易错点
- ✗ 整数点数算成 (x2-x1) * (y2-y1)，少了边界点
- ✓ 正确公式: (x2 - x1 + 1) * (y2 - y1 + 1)

- ✗ 用 `lower_bound` 而非 `upper_bound` 导致偏移
- ✓ target 在 [0, total-1]，用 `upper_bound` 找第一个 > target 的前缀和

- ✗ 忘记处理 total 溢出（大矩形多时可能超 int）
- ✓ 题目约束 xi-ai <= 2000, yi-bi <= 2000, rects.length <= 100，不会溢出

## 面试追问

**Q1: 如果矩形可能重叠怎么办？**
> 需要容斥原理或坐标压缩来计算不重叠的总面积。重叠区域的点不应被多次计算。

**Q2: 如何验证 pick 的等概率性？**
> 大量调用 pick，统计每个整数点被选中的频率，应趋近于 1/total。可用卡方检验。

**Q3: 如果要支持动态添加/删除矩形？**
> 用线段树或 BIT 维护前缀和，支持动态更新。添加矩形时更新对应区间的权重。

## 相关题型
- [LeetCode 528] Random Pick with Weight（加权随机 + 二分）
- [LeetCode 710] Random Pick with Blacklist（随机 + 映射）
- [LeetCode 478] Generate Random Point in a Circle（几何随机）
