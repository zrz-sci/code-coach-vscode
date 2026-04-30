# 218. 天际线问题 (The Skyline Problem)

## 核心思路

本题是经典的**扫描线(Sweep Line)**问题：给定一组矩形建筑物的 [left, right, height]，求从远处观察时形成的天际线轮廓的关键点。

核心观察：天际线的"关键点"只可能出现在**某栋建筑的左边缘或右边缘处**。在这些边缘位置，当前最高建筑的高度如果发生了变化，就产生一个关键点。

所以算法分为两步：
1. **提取所有边缘事件**并排序
2. 用一个**最大堆/有序集**动态维护"当前活跃建筑的最大高度"，在每个边缘位置检查高度是否变化

## 思维链

1. **读完题第一反应**：建筑物是矩形，天际线就是所有矩形叠放后的上边界轮廓。关键点出现在高度变化处。每个关键点就是"某段水平线的左端点"。

2. **天际线何时变化**：只有在某栋建筑"开始"（左边缘）或"结束"（右边缘）时，最大高度才可能变化。所以我们只需要关注所有建筑的左右边缘 —— 这就是**扫描线**的思想。

3. **怎么处理**：
   - 把每栋建筑拆成两个事件：(left, -height) 表示进入，(right, height) 表示离开
   - 为什么左边缘用负高度？ → 排序时保证同一 x 坐标下，进入事件先于离开事件处理；且同一 x 多个进入事件时，更高的先处理
   - 按 x 坐标排序所有事件

4. **扫描过程**：
   - 维护一个 multiset（或最大堆），记录"当前未结束的所有建筑的高度"
   - 初始放入高度 0（地面）
   - 遍历每个事件：进入 → 加入高度，离开 → 删除高度
   - 每次操作后检查当前最大高度是否与之前不同 → 不同则记录关键点

5. **为什么用 multiset 而不是 priority_queue**：priority_queue 不支持删除任意元素。当建筑结束时，我们需要精确地移除该建筑的高度。multiset 的 `erase(find(h))` 可以做到 O(log n) 删除。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 扫描线 + multiset | 事件排序 + 有序集维护最大高度 | O(n log n) | O(n) | ⭐ 面试首选 |
| 扫描线 + 懒删除堆 | 事件排序 + 堆 + 延迟删除 | O(n log n) | O(n) | 变体写法 |
| 分治 + 合并天际线 | 类似归并排序，递归合并 | O(n log n) | O(n) | 理解分治 |

## 事件排序的核心技巧

这是本题**最容易出错**的部分。所有边缘事件需要排序，排序规则决定了关键点是否正确。

**技巧：左边缘高度取负值**

```
建筑 [2, 9, 10] → 事件 (2, -10) 和 (9, 10)
建筑 [3, 7, 15] → 事件 (3, -15) 和 (7, 15)
```

对事件按 (x, h) 排序后，自然满足：

| 场景 | 排序效果 | 为什么正确 |
|------|---------|-----------|
| 同 x，都是左边缘 | 更高的先处理（-15 < -10） | 先加入高建筑，不会产生多余中间关键点 |
| 同 x，都是右边缘 | 更矮的先处理（10 < 15） | 先移除矮建筑，最大高度不变，不会产生虚假关键点 |
| 同 x，一左一右 | 左边缘先处理（-h < +h） | 先加入再移除，避免出现"先降为 0 再升回来"的虚假低谷 |

```
可视化: 建筑 [2,5,10] 和 [5,8,10]  (相邻且同高)
正确: x=5 先加入新建筑(左边缘)再移除旧建筑(右边缘) → 高度始终10 → 无关键点
错误: 先移除再加入 → 高度短暂降为0 → 产生虚假关键点 [5,0] 和 [5,10]
```

## 扫描过程详解

以 buildings = [[2,9,10],[3,7,15],[5,12,12]] 为例：

```
事件排序后: (2,-10), (3,-15), (5,-12), (7,15), (9,10), (12,12)

步骤     事件       操作           multiset         当前最大  之前最大  输出
─────────────────────────────────────────────────────────────────────
初始      -          -            {0}                0        -        -
1       (2,-10)    加入10        {0,10}             10       0        [2,10] ✓
2       (3,-15)    加入15        {0,10,15}          15       10       [3,15] ✓
3       (5,-12)    加入12        {0,10,12,15}       15       15       (不变)
4       (7, 15)    移除15        {0,10,12}          12       15       [7,12] ✓
5       (9, 10)    移除10        {0,12}             12       12       (不变)
6       (12, 12)   移除12        {0}                0        12       [12,0] ✓

最终输出: [[2,10],[3,15],[7,12],[12,0]]
```

## 关键提示

1. **multiset 初始放入 0**：代表地面高度。当所有建筑都结束后，最大高度回到 0，自然产生终止关键点。

2. **erase 只删一个**：`ms.erase(ms.find(h))` 只删除一个 h。如果用 `ms.erase(h)` 会删除所有等于 h 的元素，导致错误（多栋同高建筑时出 bug）。

3. **记录之前的最大高度**：每次操作后，比较 `*ms.rbegin()` 与操作前的最大高度。只有在发生变化时才输出关键点。

4. **坐标范围**：题目说 left, right 可到 2^31-1，但建筑数量只到 10^4，所以事件数最多 2 * 10^4，排序不会超时。

5. **分治法理解**：将建筑集合一分为二，递归得到左半和右半的天际线，然后像归并排序一样合并两个天际线。合并时用两个指针分别扫描，取 max(左高, 右高)。

## 决策树可视化

```
           getSkyline([[2,9,10],[3,7,15],[5,12,12]])
                      ↓ 提取事件并排序
        (2,-10) (3,-15) (5,-12) (7,15) (9,10) (12,12)
                      ↓ 扫描
     x=2: +10 → heights={0,10}     → max=10≠0  → 输出[2,10]
     x=3: +15 → heights={0,10,15}  → max=15≠10 → 输出[3,15]
     x=5: +12 → heights={0,10,12,15} → max=15=15 → 跳过
     x=7: -15 → heights={0,10,12}  → max=12≠15 → 输出[7,12]
     x=9: -10 → heights={0,12}     → max=12=12 → 跳过
     x=12:-12 → heights={0}        → max=0≠12  → 输出[12,0]
```

## 解法详解

### 解法1: 扫描线 + multiset — O(n log n) / O(n) ⭐ 面试首选

**思考过程**：把所有建筑的左右边缘提取为事件，排序后依次处理。用 multiset 动态维护当前活跃建筑的高度集合，在每个事件位置判断最大高度是否变化。

```cpp
class Solution {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        // 第一步: 提取事件，左边缘高度取负
        vector<pair<int,int>> events;
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]}); // 进入: (left, -height)
            events.push_back({b[1],  b[2]}); // 离开: (right, height)
        }
        sort(events.begin(), events.end());
        
        // 第二步: 扫描线
        multiset<int> heights = {0}; // 地面高度
        int prevMax = 0;
        vector<vector<int>> result;
        
        for (auto& [x, h] : events) {
            if (h < 0) {
                heights.insert(-h);     // 进入: 加入高度
            } else {
                heights.erase(heights.find(h)); // 离开: 移除高度(只删一个)
            }
            int curMax = *heights.rbegin();
            if (curMax != prevMax) {
                result.push_back({x, curMax});
                prevMax = curMax;
            }
        }
        return result;
    }
};
```

**关键点**：
- 事件排序技巧（负高度）是本题的核心，保证了同 x 坐标下的正确处理顺序
- multiset 的 `erase(find(h))` 精确删除一个元素
- `heights = {0}` 的初始化简化了终止关键点的判断

### 解法2: 扫描线 + 懒删除堆 — O(n log n) / O(n)

**从解法1变化**：用优先队列（最大堆）代替 multiset。因为堆不支持删除任意元素，采用"懒删除"策略：标记要删除的元素，在堆顶遇到被标记的元素时才真正弹出。

```cpp
class Solution {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int,int>> events;
        for (auto& b : buildings) {
            events.push_back({b[0], -b[2]});
            events.push_back({b[1],  b[2]});
        }
        sort(events.begin(), events.end());
        
        priority_queue<int> maxHeap;
        unordered_map<int, int> toRemove; // 待删除: 高度 → 剩余删除次数
        maxHeap.push(0);
        int prevMax = 0;
        vector<vector<int>> result;
        
        for (auto& [x, h] : events) {
            if (h < 0) {
                maxHeap.push(-h);
            } else {
                toRemove[h]++; // 标记删除，不立即从堆中移除
            }
            // 懒删除：堆顶如果是待删除元素，弹出
            while (!maxHeap.empty() && toRemove.count(maxHeap.top()) 
                   && toRemove[maxHeap.top()] > 0) {
                toRemove[maxHeap.top()]--;
                if (toRemove[maxHeap.top()] == 0)
                    toRemove.erase(maxHeap.top());
                maxHeap.pop();
            }
            int curMax = maxHeap.top();
            if (curMax != prevMax) {
                result.push_back({x, curMax});
                prevMax = curMax;
            }
        }
        return result;
    }
};
```

**关键点**：
- `toRemove` 记录每个高度待删除的次数（处理多栋同高建筑）
- 每次取堆顶之前，先清理已标记删除的元素
- 时间复杂度仍然是 O(n log n)，因为每个元素最多被 push 和 pop 各一次

### 解法3: 分治 + 合并天际线 — O(n log n) / O(n)

**思路**：类似归并排序。将建筑群一分为二，递归求解左右两半的天际线，然后合并两个天际线。合并过程：用两个指针分别扫描左右天际线，取当前 x 最小的点，高度取 max(左侧当前高度, 右侧当前高度)。

```cpp
class Solution {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        return solve(buildings, 0, buildings.size() - 1);
    }
    
    vector<vector<int>> solve(vector<vector<int>>& buildings, int lo, int hi) {
        if (lo > hi) return {};
        if (lo == hi) {
            return {{buildings[lo][0], buildings[lo][2]},
                    {buildings[lo][1], 0}};
        }
        int mid = lo + (hi - lo) / 2;
        auto left = solve(buildings, lo, mid);
        auto right = solve(buildings, mid + 1, hi);
        return merge(left, right);
    }
    
    vector<vector<int>> merge(vector<vector<int>>& left, 
                              vector<vector<int>>& right) {
        vector<vector<int>> result;
        int i = 0, j = 0;
        int lh = 0, rh = 0; // 左右侧当前高度
        
        while (i < (int)left.size() && j < (int)right.size()) {
            int x, maxH;
            if (left[i][0] < right[j][0]) {
                x = left[i][0]; lh = left[i][1]; i++;
            } else if (left[i][0] > right[j][0]) {
                x = right[j][0]; rh = right[j][1]; j++;
            } else {
                x = left[i][0]; lh = left[i][1]; rh = right[j][1]; i++; j++;
            }
            maxH = max(lh, rh);
            if (result.empty() || result.back()[1] != maxH) {
                result.push_back({x, maxH});
            }
        }
        while (i < (int)left.size()) {
            if (result.empty() || result.back()[1] != left[i][1])
                result.push_back(left[i]);
            i++;
        }
        while (j < (int)right.size()) {
            if (result.empty() || result.back()[1] != right[j][1])
                result.push_back(right[j]);
            j++;
        }
        return result;
    }
};
```

## 解法对比

| | 扫描线 + multiset ⭐ | 懒删除堆 | 分治合并 |
|---|---|---|---|
| 核心数据结构 | multiset | priority_queue + map | 递归 + 双指针合并 |
| 删除操作 | O(log n) 精确删除 | O(1) 标记 + 延迟清理 | 不需要删除 |
| 代码简洁度 | 最简洁 | 中等 | 较复杂（merge 逻辑多） |
| 面试推荐 | **首选** | 理解变体 | 理解分治思想 |

## 易错点

1. **erase 删除所有等值元素**
   - `ms.erase(h)` 删除所有等于 h 的元素（严重错误!）
   - `ms.erase(ms.find(h))` 只删除一个（正确!）
   - 当多栋建筑高度相同时，前者会一次性全部删除

2. **同 x 坐标事件处理顺序错误**
   - 不用负高度技巧时，需要手写复杂的排序规则
   - 忘记处理"同 x 一左一右"的情况 → 产生虚假的下降后上升

3. **忘记初始化地面高度 0**
   - multiset 为空时 `rbegin()` 未定义行为
   - 没有 0 作为底，最后一栋建筑结束时不会产生 [x, 0] 关键点

4. **分治合并时遗漏去重**
   - 合并后可能出现连续相同高度的关键点
   - 每次添加前需检查 `result.back()[1] != maxH`

## 面试追问

**Q1（事件排序）**：为什么左边缘要取负高度？不取负可以吗？
> 取负是为了利用 pair 的默认排序规则一次性解决三种边界情况（两左、两右、一左一右）。不取负也可以，但需要自定义排序函数分别处理三种 case，代码更复杂且容易出错。

**Q2（数据结构选择）**：为什么用 multiset 不用 priority_queue？
> priority_queue 不支持删除任意元素。建筑结束时需要精确移除其高度，multiset 可以 O(log n) 做到。如果用堆，需要加懒删除逻辑，代码更复杂。

**Q3（优化）**：如果建筑数量级从 10^4 增加到 10^6，哪里需要优化？
> 瓶颈在排序 O(n log n) 和 multiset 操作 O(n log n)，本身已经最优。如果坐标离散化后范围不大，可以用线段树区间更新 + 扫描输出，但通常不需要。

**Q4（变体）**：如果要求返回天际线的面积（轮廓下的面积），怎么改？
> 在扫描线过程中，每次高度变化时累加 `prevMax * (x - prevX)` 即可，只需多维护一个 prevX 变量。

## 相关题型

- **253. Meeting Rooms II** — 同样是扫描线 + 堆的经典应用：会议开始/结束时间作为事件，堆维护同时进行的会议数。复用"事件提取 → 排序 → 扫描"的框架。
- **56. Merge Intervals** — 区间合并，和天际线合并有相似的"处理重叠"思想。但本题需要维护高度维度，更复杂。
- **732. My Calendar III** — 差分数组/扫描线求最大重叠数，可视为本题的简化版（只关心计数变化而非高度变化）。
- **850. Rectangle Area II** — 二维扫描线求矩形面积并，是本题从一维到二维的扩展。用扫描线 + 线段树处理 y 维。
