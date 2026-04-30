# 356. Line Reflection - 直线镜像

## 核心思路
若存在平行于 y 轴的对称线 x=d，则对于每个点 (x, y)，必须存在对称点 (2d-x, y)。对称线位置 d = (minX + maxX) / 2。用 **HashSet** 存所有点，遍历验证每个点的对称点是否存在。

## 思维链
1. 对称线一定是 x = (minX + maxX) / 2 → 先找 minX 和 maxX
2. 对于点 (x, y)，其关于 x=d 的对称点为 (2d - x, y) = (minX + maxX - x, y)
3. 去重后将所有点存入 set → 逐一检查对称点是否存在
4. 使用整数运算避免浮点精度问题：用 sum = minX + maxX 代替 d = sum/2

## 解法概览表⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| HashSet 对称验证 | O(N) | O(N) | 最优解，整数运算无精度问题 |
| 排序 + 双指针 | O(N logN) | O(N) | 无需哈希，排序后首尾配对 |

## 关键提示
1. **避免浮点数**：对称线 x = (minX+maxX)/2 可能是小数，用 sum = minX+maxX 做整数运算
2. **去重**：有重复点时必须去重，否则同一点会被检查两次
3. **y 坐标必须匹配**：对称点不仅 x 关于 d 对称，y 也必须相同
4. **单点情况**：只有一个点（或所有点 x 相同）时，对称线过这些点，返回 true

## 解法详解

### 解法一：HashSet 对称验证（推荐）

**思路**：找到 minX 和 maxX 确定对称轴，然后用 set 验证每个点都有对称点。

```cpp
class Solution {
public:
    bool isReflected(vector<vector<int>>& points) {
        int minX = INT_MAX, maxX = INT_MIN;
        set<pair<int,int>> pointSet;
        
        for (auto& p : points) {
            minX = min(minX, p[0]);
            maxX = max(maxX, p[0]);
            pointSet.insert({p[0], p[1]});
        }
        
        int sum = minX + maxX;  // 2 * d，避免浮点
        for (auto& [x, y] : pointSet) {
            if (!pointSet.count({sum - x, y})) return false;
        }
        return true;
    }
};
```

**复杂度**：时间 O(N logN)（set 操作），空间 O(N)

### 解法二：unordered_set + 字符串编码

**思路**：用字符串 "x,y" 作为哈希 key，实现 O(1) 查找。

```cpp
class Solution {
public:
    bool isReflected(vector<vector<int>>& points) {
        int minX = INT_MAX, maxX = INT_MIN;
        unordered_set<string> st;
        
        for (auto& p : points) {
            minX = min(minX, p[0]);
            maxX = max(maxX, p[0]);
            st.insert(to_string(p[0]) + "," + to_string(p[1]));
        }
        
        int sum = minX + maxX;
        for (auto& p : points) {
            string sym = to_string(sum - p[0]) + "," + to_string(p[1]);
            if (!st.count(sym)) return false;
        }
        return true;
    }
};
```

**复杂度**：时间 O(N)，空间 O(N)

### 解法三：排序 + 双指针

**思路**：按 x 升序、y 升序排序。首尾配对检查对称性。

```cpp
class Solution {
public:
    bool isReflected(vector<vector<int>>& points) {
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());
        
        int sum = points.front()[0] + points.back()[0];
        int l = 0, r = points.size() - 1;
        
        while (l <= r) {
            if (points[l][0] + points[r][0] != sum) return false;
            if (points[l][0] == points[r][0]) {
                // 同一x坐标的点，y值需要对称（自对称）
                l++; r--;
            } else {
                // 不同x坐标的点集，需要y值集合完全相同
                // 收集左侧和右侧同x的所有y值
                set<int> leftY, rightY;
                int lx = points[l][0], rx = points[r][0];
                while (l <= r && points[l][0] == lx) leftY.insert(points[l++][1]);
                while (r >= l && points[r][0] == rx) rightY.insert(points[r--][1]);
                if (leftY != rightY) return false;
            }
        }
        return true;
    }
};
```

**复杂度**：时间 O(N logN)，空间 O(N)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `double d = (minX + maxX) / 2.0` 然后比较浮点 | 用 `int sum = minX + maxX` 做整数运算 | 浮点精度陷阱 |
| 不去重直接验证 | 用 set 自动去重 | 重复点会导致逻辑混乱 |
| 只检查 x 对称，忽略 y | 对称点 y 坐标必须相同 | (1,1) 和 (-1,2) 不是对称关系 |
| minX 和 maxX 初始值为 0 | 初始化为 INT_MAX 和 INT_MIN | 坐标可能为负数 |

## 面试追问

**Q1: 如果要找平行于 x 轴的对称线呢？**

A: 完全对称的处理：交换 x 和 y 的角色。找 minY 和 maxY，对称线 y = (minY+maxY)/2，检查每个 (x,y) 是否存在 (x, sumY-y)。

**Q2: 如果坐标是浮点数怎么办？**

A: 不能直接用 sum 整数运算。需要：1) 用 epsilon 比较浮点相等，或 2) 将浮点乘以足够大的系数转为整数，或 3) 用 `long long` 存放乘以 10^k 后的值避免精度损失。

**Q3: 如何扩展到 3D 空间找对称平面？**

A: 寻找与某个坐标轴垂直的对称平面（如 x=d 平面）。对于点 (x,y,z)，检查 (2d-x, y, z) 是否存在。需要用 3D 坐标做 key 的 set。如果对称平面可以是任意方向，则需要更复杂的几何分析（PCA 找主方向等）。

## 相关题型
- [149. Max Points on a Line](../0149_max_points_on_a_line/) - 几何 + 哈希
- [447. Number of Boomerangs](../0447_number_of_boomerangs/) - 点对距离 + 哈希
- [593. Valid Square](../0593_valid_square/) - 几何验证
