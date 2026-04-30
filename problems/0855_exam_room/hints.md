# 855. 考场就座

## 核心思路

本质是一个**动态间隔管理**问题：维护一组有人的座位，每次 `seat()` 找到最大间隔的中点坐下，每次 `leave()` 移除一个座位。需要高效地找最大间隔并支持动态增删。用**有序集合 (set)** 维护有人的座位，每次遍历集合找最大间隔。

## 思维链

1. **读完题第一反应** → 和 849 题类似，但需要反复执行。每次 seat() 要找当前最大间隔的最优位置，leave() 要移除一个位置。
2. **数据结构选型？** → 需要维护有序的座位集合，支持插入、删除、遍历。`set<int>` 天然支持。
3. **seat() 怎么找最优位置？** → 遍历 set 中相邻元素，计算间隔。同时处理首尾特殊间隔。取最大间隔的中点。
4. **能否用堆优化？** → 可以用最大堆存储间隔 `(distance, left, right)`，但 leave() 时需要懒删除，实现复杂。
5. **n 可达 10^9 但调用最多 10^4 次** → 不能用大小为 n 的数组。set 只存有人的位置，空间 O(k)，k = 当前人数。

## 解法概览

| 解法 | 思路 | seat() | leave() | 面试 |
|------|------|--------|---------|------|
| 有序集合遍历 | set 存有人位置，每次遍历找最大间隔 | O(k) | O(log k) | ⭐ 推荐 |
| 堆 + 懒删除 | 最大堆存间隔，leave 时标记删除 | O(log k) 均摊 | O(log k) | 进阶 |
| 有序集合 + 区间 set | 维护间隔集合，按距离排序 | O(log k) | O(log k) | 最优但复杂 |

## 关键提示

1. **间隔距离计算**：对于间隔 `[left, right]`，坐在中点 `mid = left + (right - left) / 2`，到最近人的距离为 `mid - left` = `(right - left) / 2`。
2. **首尾特殊处理**：左端间隔 `[-1, first_person]`，如果坐在位置0，距离 = `first_person`。右端间隔 `[last_person, n]`，如果坐在位置 `n-1`，距离 = `n - 1 - last_person`。
3. **平局处理**：题目要求距离相同时选编号最小的座位。左端间隔天然选编号0（最小）；中间间隔选左中点（`left + gap/2`，整除向下取整天然偏左）；右端间隔选 `n-1`。
4. **为什么用 set 而不是 vector？** set 的插入/删除是 O(log k)，而 vector 插入/删除是 O(k)。虽然 seat() 都需要 O(k) 遍历，但 set 在 leave() 上更快。
5. **n 可以很大（10^9）**：不能用数组标记座位。只存有人的位置索引。

## 解法详解

### 解法1: 有序集合遍历 — seat() O(k) / leave() O(log k) ⭐ 面试首选

**思考过程**: 用 `set<int>` 维护所有有人的座位。seat() 时遍历 set 找最大间隔。

```
示例: n = 10
seat(): 空房间 → 坐位置0, set={0}
seat(): 间隔[0,n): 坐位置9, 距离=9. set={0,9}
seat(): 间隔[0,9]: 中点4, 距离=4. set={0,4,9}
seat(): 间隔[0,4]: 中点2, 距离=2. 间隔[4,9]: 中点6, 距离=2.
        平局选编号小的→坐2. set={0,2,4,9}
leave(4): set={0,2,9}
seat(): 间隔[2,9]: 中点5, 距离=3. 间隔[0,2]: 中点1, 距离=1.
        → 坐5. set={0,2,5,9}
```

```cpp
class ExamRoom {
    set<int> seats;
    int n;
public:
    ExamRoom(int n) : n(n) {}

    int seat() {
        if (seats.empty()) {
            seats.insert(0);
            return 0;
        }
        int maxDist = *seats.begin(); // 左端间隔
        int bestSeat = 0;
        int prev = -1;
        for (int s : seats) {
            if (prev != -1) {
                int dist = (s - prev) / 2;
                if (dist > maxDist) {
                    maxDist = dist;
                    bestSeat = prev + (s - prev) / 2;
                }
            }
            prev = s;
        }
        // 右端间隔
        if (n - 1 - *seats.rbegin() > maxDist) {
            bestSeat = n - 1;
        }
        seats.insert(bestSeat);
        return bestSeat;
    }

    void leave(int p) {
        seats.erase(p);
    }
};
```

**关键点**:
- 空集合时直接坐位置0。
- 左端间隔距离 = `*seats.begin()`（第一个有人位置到位置0的距离）。
- 右端间隔距离 = `n - 1 - *seats.rbegin()`。
- 中间间隔：`(s - prev) / 2`，座位在 `prev + (s - prev) / 2`。
- 平局时选编号小的：因为从左到右遍历，用 `>` 而非 `>=` 比较距离，天然保留编号小的。

---

### 解法2: 堆 + 有序集合 — seat() O(log k) 均摊 / leave() O(log k)

**优化思路**: 用最大堆存储所有间隔及其最优距离，seat() 时直接取堆顶。leave() 时不立即删除旧间隔，而是标记为无效（懒删除）。

```cpp
class ExamRoom {
    int n;
    set<int> seated;
    // 堆元素: {距离, 选的座位, 左端, 右端}
    // 距离取负值实现最大堆(priority_queue 默认最大堆取正距离)
    // 自定义比较: 距离大优先，距离相同座位编号小优先
    struct Interval {
        int dist, seat, left, right;
        bool operator<(const Interval& o) const {
            if (dist != o.dist) return dist < o.dist;
            return seat > o.seat; // 距离相同选编号小的
        }
    };
    priority_queue<Interval> pq;

    int calcDist(int left, int right) {
        if (left == -1) return right;
        if (right == n) return n - 1 - left;
        return (right - left) / 2;
    }
    int calcSeat(int left, int right) {
        if (left == -1) return 0;
        if (right == n) return n - 1;
        return left + (right - left) / 2;
    }
    void addInterval(int left, int right) {
        int d = calcDist(left, right);
        int s = calcSeat(left, right);
        pq.push({d, s, left, right});
    }
public:
    ExamRoom(int n) : n(n) {
        addInterval(-1, n);
    }
    int seat() {
        while (true) {
            auto top = pq.top(); pq.pop();
            // 验证间隔是否仍然有效
            int l = top.left, r = top.right;
            bool lOk = (l == -1) || seated.count(l);
            bool rOk = (r == n) || seated.count(r);
            if (lOk && rOk) {
                // 确认 l 和 r 在 seated 中相邻
                if (l != -1 && r != n) {
                    auto it = seated.find(l);
                    if (next(it) != seated.end() && *next(it) != r) continue;
                }
                int s = top.seat;
                seated.insert(s);
                addInterval(l, s);
                addInterval(s, r);
                return s;
            }
        }
    }
    void leave(int p) {
        auto it = seated.find(p);
        int l = (it == seated.begin()) ? -1 : *prev(it);
        int r = (next(it) == seated.end()) ? n : *next(it);
        seated.erase(it);
        addInterval(l, r);
    }
};
```

## 易错点

1. **平局比较方向错误**：
   - 错: 距离相同时选编号大的 → 不满足"选编号最小的座位"
   - 对: 用 `>` 严格大于比较距离，自然保留先遇到的（编号小的）

2. **首尾间隔距离算错**：
   - 错: 左端间隔也用 `gap / 2` → `[0,0,0,1,...]` 应该距离=3不是1
   - 对: 左端距离 = `first`，右端距离 = `n-1-last`，不除以2

3. **空集合时没特判**：
   - 错: 直接遍历空 set 做计算 → 未定义行为
   - 对: `if (seats.empty()) return 0;`

4. **leave 之后 set 为空时的 seat()**：
   - 确保 empty 检查在 seat() 开头

## 面试追问

**Q1: seat() 能否优化到 O(log k)？**
> 可以。用有序集合（如 `set<pair<int,int>>`）按间隔距离排序，seat() 取最大间隔 O(log k)，leave() 合并相邻间隔 O(log k)。或用堆+懒删除。

**Q2: 这题和 849 有什么关系？**
> 849 是一次性查询，本题是动态版本。849 的一次遍历解法是本题 seat() 的核心逻辑。

**Q3: 如果 seat() 和 leave() 调用频率差异很大怎么优化？**
> 如果 seat() 频繁而 leave() 罕见，用堆优化 seat()。如果 leave() 频繁，set 遍历法更简单。

**Q4: n = 10^9 但只有 10^4 次操作，内存够吗？**
> set 只存有人的位置（最多 10^4 个），空间 O(k)，完全足够。

## 相关题型

- **849. Maximize Distance to Closest Person** — 本题的静态版本（一次查询），核心间隔分析逻辑完全相同。
- **295. Find Median from Data Stream** — 同样是设计题，需要动态维护有序结构并快速查询。
- **729. My Calendar I** — 区间管理设计题，用有序集合维护不重叠区间。
- **715. Range Module** — 更复杂的区间管理，支持添加/删除/查询区间。
