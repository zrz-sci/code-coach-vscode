# 850. 矩形面积 II

## 核心思路

本质是一个**扫描线 + 坐标压缩**问题：将所有矩形的边作为事件，沿 x 轴扫描，在每个 x 区间内计算 y 方向上被覆盖的总长度，面积 = 覆盖长度 * x 区间宽度。需要对重叠区域**去重计算**。

## 思维链

1. **读完题第一反应** → 多个矩形求并集面积。直接加减会有重叠区域的加减容斥问题，矩形多了容斥指数爆炸。
2. **几何问题的经典套路？** → 扫描线。把二维面积问题降维成一维长度问题。
3. **扫描线怎么做？** → 把每个矩形拆成两条竖直边（左边：进入事件 +1，右边：离开事件 -1），按 x 排序扫描。在两个相邻 x 坐标之间，y 方向上被覆盖的长度是固定的。
4. **y 方向覆盖长度怎么求？** → 方法1: 坐标压缩后暴力合并区间 O(n)。方法2: 线段树维护覆盖长度 O(log n)。由于 n <= 200，暴力就够了。
5. **坐标压缩是什么？** → 把所有出现过的 y 坐标去重排序，只关心这些关键 y 坐标之间的区间。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 坐标压缩 + 扫描线 | 离散化 y 坐标，逐 x 区间累计覆盖长度 | O(n^2 log n) | O(n^2) | ⭐ 推荐 |
| 线段树 + 扫描线 | 用线段树维护 y 方向覆盖长度 | O(n log n) | O(n) | 加分项 |
| 容斥原理 | 对所有矩形子集做加减容斥 | O(2^n * n) | O(2^n) | 不实用 |

## 关键提示

1. **扫描线事件定义**：每个矩形 `[x1, y1, x2, y2]` 产生两个事件：`(x1, y1, y2, +1)` 表示进入，`(x2, y1, y2, -1)` 表示离开。
2. **坐标压缩的目的**：y 坐标范围可达 10^9，不能用数组直接表示。将所有出现过的 y 坐标去重排序后，只需关心 O(n) 个离散区间。
3. **取模注意事项**：面积可能很大（10^18 级别），需要 `long long` 计算，最后对 10^9 + 7 取模。中间乘法需要防溢出。
4. **暴力覆盖计算**：由于 n <= 200，在每个 x 区间里遍历所有活跃矩形，收集 y 区间后合并，O(n) 即可。总复杂度 O(n^2 log n)。
5. **替代做法——逐行扫描**：坐标压缩后把平面分成 O(n^2) 个小矩形，逐个检查是否被任何输入矩形覆盖，O(n^3) 但 n<=200 可过。

## 解法详解

### 解法1: 坐标压缩 + 扫描线 — O(n^2 log n) ⭐ 面试首选

**思考过程**: 将 x/y 坐标分别离散化，沿 x 轴扫描。在每个 x 区间内，统计 y 方向被覆盖的总长度。

```
示例: rectangles = [[0,0,2,2],[1,0,2,3],[1,0,3,1]]

坐标压缩后:
  x 坐标: [0, 1, 2, 3]
  y 坐标: [0, 1, 2, 3]

逐 x 区间:
  [0,1): 活跃矩形: rect0 → y覆盖[0,2), 长度=2, 面积=1*2=2
  [1,2): 活跃矩形: rect0,rect1,rect2 → y覆盖[0,3), 长度=3, 面积=1*3=3
  [2,3): 活跃矩形: rect2 → y覆盖[0,1), 长度=1, 面积=1*1=1
  总面积 = 2+3+1 = 6 ✓
```

```cpp
class Solution {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        const int MOD = 1e9 + 7;
        // 收集所有 x 和 y 坐标
        vector<int> xs, ys;
        for (auto& r : rectangles) {
            xs.push_back(r[0]); xs.push_back(r[2]);
            ys.push_back(r[1]); ys.push_back(r[3]);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        long long ans = 0;
        // 遍历每个离散化的小格子
        for (int i = 0; i + 1 < xs.size(); i++) {
            for (int j = 0; j + 1 < ys.size(); j++) {
                // 小格子的实际坐标范围
                int x1 = xs[i], x2 = xs[i+1];
                int y1 = ys[j], y2 = ys[j+1];
                // 检查是否被任意矩形覆盖
                for (auto& r : rectangles) {
                    if (r[0] <= x1 && x2 <= r[2] && r[1] <= y1 && y2 <= r[3]) {
                        ans += (long long)(x2 - x1) * (y2 - y1);
                        ans %= MOD;
                        break;  // 只计一次
                    }
                }
            }
        }
        return (int)ans;
    }
};
```

---

### 解法2: 扫描线 + 区间合并 — O(n^2 log n)

**不同视角**: 沿 x 轴扫描，在每个 x 区间内收集活跃的 y 区间并合并。

```cpp
class Solution {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        const int MOD = 1e9 + 7;
        vector<int> xs;
        for (auto& r : rectangles) {
            xs.push_back(r[0]);
            xs.push_back(r[2]);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());

        long long ans = 0;
        for (int i = 0; i + 1 < (int)xs.size(); i++) {
            int xMid = xs[i]; // 当前 x 区间的左端点
            // 收集在此 x 区间内活跃的 y 区间
            vector<pair<int,int>> yIntervals;
            for (auto& r : rectangles) {
                if (r[0] <= xMid && xMid < r[2]) {
                    yIntervals.push_back({r[1], r[3]});
                }
            }
            // 合并 y 区间
            sort(yIntervals.begin(), yIntervals.end());
            long long yLen = 0;
            int curL = -1, curR = -1;
            for (auto& [l, r] : yIntervals) {
                if (l > curR) {
                    yLen += curR - curL;
                    curL = l; curR = r;
                } else {
                    curR = max(curR, r);
                }
            }
            yLen += curR - curL;
            ans = (ans + (long long)(xs[i+1] - xs[i]) % MOD * (yLen % MOD)) % MOD;
        }
        return (int)ans;
    }
};
```

## 易错点

1. **整数溢出**：
   - 错: `(x2-x1) * (y2-y1)` 用 int → 两个 10^9 相乘溢出
   - 对: 用 `(long long)(x2-x1) * (y2-y1)` 或全程 long long

2. **取模位置不对**：
   - 错: 只在最后取模 → 中间累加就溢出了
   - 对: 每次加法后都取模，乘法用 long long 防溢出

3. **坐标压缩后忘记用原始坐标算面积**：
   - 错: 用离散化索引 `i * j` 当面积 → 离散化只是索引映射
   - 对: 用 `xs[i+1]-xs[i]` 和 `ys[j+1]-ys[j]` 算实际长度

4. **区间合并时初始值问题**：
   - 错: `curL = 0, curR = 0` → 多算了 [0, 第一个区间左端] 的长度
   - 对: `curL = -1, curR = -1` 或用第一个区间初始化

## 面试追问

**Q1: 为什么不用容斥原理？**
> 容斥需要枚举所有矩形子集，复杂度 O(2^n)，n=200 时不可行。扫描线是 O(n^2 log n)。

**Q2: 如果矩形数量很大（n=10^5），怎么优化？**
> 用线段树维护 y 方向的覆盖长度。每个事件更新线段树 O(log n)，总 O(n log n)。

**Q3: 扫描线为什么沿 x 轴而不是 y 轴？**
> 都可以，沿哪个轴扫描都行。选哪个轴不影响正确性和复杂度。

**Q4: 坐标值可以为负怎么办？**
> 坐标压缩不受影响。离散化只关心相对顺序和实际差值，负数完全没问题。

## 相关题型

- **218. The Skyline Problem** — 经典扫描线问题，沿 x 轴扫描建筑物，用堆或 multiset 维护当前最大高度。
- **391. Perfect Rectangle** — 判断矩形是否恰好拼成一个完美矩形，用面积检查 + 角点检查。
- **223. Rectangle Area** — 两个矩形的并集面积，是本题 n=2 的特例，用简单的重叠面积公式即可。
- **84. Largest Rectangle in Histogram** — 不同的矩形面积问题，但同样用到了扫描和单调栈的思想。
