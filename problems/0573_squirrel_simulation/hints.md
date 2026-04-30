# 573. 松鼠模拟 (Squirrel Simulation)

## 核心思路
除了第一个坚果外，松鼠每捡一个坚果都需要从树出发去坚果再返回（来回 = 2 * dist(tree, nut)）。关键在于选择哪个坚果作为第一个，使总距离最小。

## 思维链
1. 基准距离: 如果松鼠从树出发，所有坚果的总距离 = sum(2 * dist(tree, nut_i))
2. 实际上松鼠从起始位置出发，第一个坚果的距离 = dist(squirrel, nut_first) + dist(nut_first, tree)
3. 而非第一个的基准是 2 * dist(tree, nut_first)
4. 差值 = dist(squirrel, nut_first) - dist(tree, nut_first)
5. 要总距离最小 → 选差值最小的（即节省最多的）坚果作为第一个
6. 答案 = 基准总距离 + min(dist(squirrel, nut_i) - dist(tree, nut_i))

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| 贪心（一次遍历）⭐ | 计算每个坚果的节省量 | O(n) | O(1) | 首选 |

## 关键提示
1. 曼哈顿距离: |r1-r2| + |c1-c2|
2. 关键公式: ans = totalDist + min_i(dist(squirrel, nut_i) - dist(tree, nut_i))
3. 只有第一个坚果的路径不同，其余都是从树出发的来回

## 解法详解

### 解法一：贪心（推荐）⭐
```cpp
class Solution {
public:
    int minDistance(int height, int width, vector<int>& tree, vector<int>& squirrel, vector<vector<int>>& nuts) {
        int totalDist = 0;
        int maxSave = INT_MIN;  // 最大节省量
        
        for (auto& nut : nuts) {
            int d_tree = abs(nut[0] - tree[0]) + abs(nut[1] - tree[1]);
            int d_sq = abs(nut[0] - squirrel[0]) + abs(nut[1] - squirrel[1]);
            totalDist += 2 * d_tree;
            maxSave = max(maxSave, d_tree - d_sq);  // 选节省最多的
        }
        
        return totalDist - maxSave;
    }
};
```

## 易错点
- ✗ 贪心选离松鼠最近的坚果 → ✓ 应选"节省量 d_tree - d_sq 最大"的坚果
- ✗ 忘记基准是 2 * dist(tree, nut) → ✓ 每个坚果除第一个外都是来回
- ✗ maxSave 初始化为 0 → ✓ 应为 INT_MIN，因为节省量可能为负

## 面试追问
**Q1: 为什么不是选离松鼠最近的坚果？**
> 因为第一个坚果的总代价是 dist(squirrel, nut) + dist(nut, tree)，而基准是 2*dist(tree, nut)。节省量 = 2*dist(tree,nut) - (dist(squirrel,nut) + dist(nut,tree)) = dist(tree,nut) - dist(squirrel,nut)。选节省量最大的而非距离最近的。

**Q2: 如果松鼠能同时携带 k 个坚果怎么办？**
> 这变成了一个更复杂的路径规划问题，可能需要 TSP 近似算法或动态规划。

**Q3: height 和 width 参数有什么用？**
> 实际上这两个参数在求解中不需要用到，它们只是描述花园大小。曼哈顿距离计算只依赖坐标。

## 相关题型
- [1066. Campus Bikes II](../1066_campus_bikes_ii/) - 分配优化
- [317. Shortest Distance from All Buildings](../0317_shortest_distance_from_all_buildings/) - 曼哈顿距离
