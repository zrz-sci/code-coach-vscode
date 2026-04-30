# 475. 供暖器

## 核心思路

本质上在问：**对每栋房屋，找到离它最近的供暖器的距离，然后在所有这些"最近距离"中取最大值**。这个最大值就是能覆盖所有房屋的最小半径。

## 思维链

1. **读完题第一反应**：每栋房屋需要被至少一个供暖器覆盖。所有供暖器半径相同，所以半径取决于"最难覆盖"的那栋房屋——即离最近供暖器最远的那栋。
2. **暴力做法**：对每栋房屋，遍历所有供暖器，算出离它最近的供暖器距离。对所有房屋取 max。时间 O(m × n)。
3. **瓶颈在哪**：对每栋房屋，"在所有供暖器中找最近的"这一步是 O(n)。如果供暖器是有序的，这个查找可以用二分搜索优化到 O(log n)。
4. **二分优化**：先排序供暖器，对每栋房屋用 `lower_bound` 找到右侧最近供暖器，再看左侧邻居，取较小距离。
5. **双指针替代**：如果同时排序房屋和供暖器，可以用双指针从左到右扫描，不需要二分。
6. **换个角度——二分答案**：也可以直接二分半径 r，检验"半径为 r 时能否覆盖所有房屋"，这是一个经典的"二分答案 + 判定"模式。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 | 每栋房屋遍历所有供暖器找最近 | O(m×n) | O(1) | 能说出即可 |
| 排序+二分查找 | 排序供暖器，对每栋房屋二分找最近 | O(m log m + n log n) | O(log n) | ⭐ 必须写出 |
| 排序+双指针 | 同时排序，双指针匹配 | O(m log m + n log n) | O(log n) | ⭐ 必须写出 |
| 二分答案 | 二分半径，贪心检验可行性 | O((m+n) log(max_val)) | O(log n) | 加分项 |

> m = houses.length, n = heaters.length

## 关键提示

1. **核心等价转换**：问题等价于 `max over all houses of (min distance to any heater)`。理解了这个，暴力解就出来了。
2. **排序是关键**：供暖器排序后，"找最近供暖器"从线性扫描变成二分查找。
3. **二分查找的细节**：`lower_bound` 找到的是"≥ house 的第一个供暖器"，但最近的可能是它左边那个，两者都要比较。
4. **双指针的正确性**：房屋和供暖器都排序后，对当前房屋，只需比较当前供暖器和下一个供暖器哪个更近。如果下一个更近，指针右移。
5. **二分答案的判定函数**：给定半径 r，从左到右扫描，每个供暖器能覆盖 `[heater-r, heater+r]`，贪心检查是否所有房屋都被覆盖。

```
示例: houses=[1,2,3,4], heaters=[1,4]

供暖器位置（数轴）:
  1       4
  H-------H
  |<--1-->|   半径=1时:
  覆盖[0,2]   覆盖[3,5]
  房屋1✓ 2✓   3✓ 4✓  → 全部覆盖 → 答案=1

每栋房屋到最近供暖器的距离:
  house=1: min(|1-1|, |1-4|) = 0
  house=2: min(|2-1|, |2-4|) = 1
  house=3: min(|3-1|, |3-4|) = 1
  house=4: min(|4-1|, |4-4|) = 0
  max = 1 → 答案=1
```

## 解法详解

### 解法1: 暴力 — O(m×n) / O(1)

**思考过程**: 最直觉的想法——对每栋房屋，逐一比较所有供暖器，找到最近的那个。然后在所有"最近距离"中取最大值。

```cpp
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        int ans = 0;
        for (int house : houses) {
            int minDist = INT_MAX;
            for (int heater : heaters) {
                minDist = min(minDist, abs(house - heater));
            }
            // 最难覆盖的房屋决定了半径
            ans = max(ans, minDist);
        }
        return ans;
    }
};
```

**关键点**: 逻辑简单，但 m、n 都到 3×10⁴ 时，最坏 9×10⁸ 次运算，可能超时。

---

### 解法2: 排序 + 二分查找 — O(m log m + n log n) / O(log n) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是"对每栋房屋线性查找最近供暖器"。如果供暖器排序了，用二分查找可以 O(log n) 找到最近的。

```cpp
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(heaters.begin(), heaters.end());
        int ans = 0;
        
        for (int house : houses) {
            // lower_bound: 找到第一个 >= house 的供暖器
            auto it = lower_bound(heaters.begin(), heaters.end(), house);
            
            int distRight = INT_MAX, distLeft = INT_MAX;
            
            // 右侧最近供暖器（>= house）
            if (it != heaters.end()) {
                distRight = *it - house;
            }
            // 左侧最近供暖器（< house）
            if (it != heaters.begin()) {
                --it;
                distLeft = house - *it;
            }
            
            // 这栋房屋到最近供暖器的距离
            int minDist = min(distLeft, distRight);
            // 所有房屋中取最大
            ans = max(ans, minDist);
        }
        return ans;
    }
};
```

```
// 二分查找过程示例:
// heaters = [1, 4], house = 3
//
// lower_bound(3) → 指向 4（第一个 >= 3）
// distRight = 4 - 3 = 1
// --it → 指向 1
// distLeft = 3 - 1 = 2
// minDist = min(2, 1) = 1
//
// heaters = [1, 4], house = 1
// lower_bound(1) → 指向 1（第一个 >= 1）
// distRight = 1 - 1 = 0
// --it? → begin, 不存在
// 但 distRight 已经是 0 了
// minDist = 0
```

**关键点**: `lower_bound` 找的是"≥"，最近的可能在它左边一个位置，两边都要看！

---

### 解法3: 排序 + 双指针 — O(m log m + n log n) / O(log n)

**不同角度**: 如果房屋也排序了，房屋从左到右扫，供暖器指针也只需单调右移。

核心贪心逻辑：对于当前房屋，比较当前供暖器 `heaters[j]` 和下一个供暖器 `heaters[j+1]` 谁更近。如果下一个更近（或一样近），就移动指针。

```cpp
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        
        int ans = 0;
        int j = 0; // 供暖器指针
        
        for (int i = 0; i < (int)houses.size(); i++) {
            // 如果下一个供暖器离当前房屋更近（或一样近），右移指针
            while (j + 1 < (int)heaters.size() &&
                   abs(heaters[j + 1] - houses[i]) <= abs(heaters[j] - houses[i])) {
                j++;
            }
            // 当前供暖器就是离 houses[i] 最近的
            ans = max(ans, abs(heaters[j] - houses[i]));
        }
        return ans;
    }
};
```

```
// 双指针过程:
// houses = [1,2,3,4], heaters = [1,4]
//
// i=0, house=1: j=0, heater[0]=1, |1-1|=0
//   j+1=1, heater[1]=4, |4-1|=3 > 0 → 不移动
//   dist=0, ans=0
//
// i=1, house=2: j=0, heater[0]=1, |1-2|=1
//   j+1=1, heater[1]=4, |4-2|=2 > 1 → 不移动
//   dist=1, ans=1
//
// i=2, house=3: j=0, heater[0]=1, |1-3|=2
//   j+1=1, heater[1]=4, |4-3|=1 <= 2 → j=1
//   dist=1, ans=1
//
// i=3, house=4: j=1, heater[1]=4, |4-4|=0
//   j+1=2, 越界 → 不移动
//   dist=0, ans=1
//
// 最终 ans=1 ✓
```

**关键点**: 双指针 `j` 不会回退——因为房屋是有序的，后面的房屋不可能需要更左边的供暖器。这保证了总共只移动 O(n) 次。

---

### 解法4: 二分答案 — O((m+n) log(max_val)) / O(log n)

**换个角度**: 不直接计算半径，而是猜一个半径 r，检验它是否能覆盖所有房屋。半径越大越容易覆盖，满足单调性，可以二分。

```cpp
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        
        // 二分答案：半径 r 的范围是 [0, max_possible]
        int lo = 0, hi = max(
            abs(houses.back() - heaters.front()),
            abs(houses.front() - heaters.back())
        );
        
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (canCover(houses, heaters, mid)) {
                hi = mid;     // mid 可行，尝试更小的半径
            } else {
                lo = mid + 1; // mid 不够，需要更大的半径
            }
        }
        return lo;
    }
    
private:
    // 判定：半径为 r 时，能否覆盖所有房屋
    bool canCover(vector<int>& houses, vector<int>& heaters, int r) {
        int j = 0; // 供暖器指针
        for (int i = 0; i < (int)houses.size(); i++) {
            // 跳过覆盖不到 houses[i] 的供暖器
            while (j < (int)heaters.size() && heaters[j] + r < houses[i]) {
                j++;
            }
            // 检查当前供暖器能否覆盖 houses[i]
            if (j >= (int)heaters.size() || heaters[j] - r > houses[i]) {
                return false;
            }
        }
        return true;
    }
};
```

```
// 二分答案搜索过程 (houses=[1,5], heaters=[2]):
//
// 初始: lo=0, hi=|5-2|=3
//
// Round1: mid=1, canCover(r=1)?
//   heater=2, 覆盖[1,3], house=1✓, house=5✗ → false
//   lo=2
//
// Round2: mid=2, canCover(r=2)?
//   heater=2, 覆盖[0,4], house=1✓, house=5✗ → false
//   lo=3
//
// Round3: lo=3==hi=3 → 退出, 答案=3 ✓
```

**关键点**: 判定函数是贪心的——对于每栋房屋，尽量用最左边还能覆盖到它的供暖器，这样给后面的房屋留更多选择。

---

## 解法对比

| | 解法1 暴力 | 解法2 排序+二分 | 解法3 排序+双指针 | 解法4 二分答案 |
|---|---|---|---|---|
| 时间 | O(m×n) | O(m log n + n log n) | O(m log m + n log n) | O((m+n) log V) |
| 空间 | O(1) | O(log n) 排序 | O(log n) 排序 | O(log n) 排序 |
| 核心操作 | 线性查找 | 二分查找最近 | 双指针配对 | 二分答案+判定 |
| 面试推荐 | 说思路 | ⭐首选 | ⭐替代方案 | 加分项 |

- **解法2 vs 解法3**: 解法2不需要排序房屋，解法3需要。但解法3代码更简洁。面试中写哪个都行。
- **解法4的意义**: "二分答案"是一种通用思想——当直接求答案困难，但验证某个答案是否可行很容易时，就可以二分答案。很多"最小化最大值"或"最大化最小值"的题都能用这个模式。

## 易错点

1. **二分查找只看一侧**:
   - ✗ 只看 `lower_bound` 找到的右侧供暖器
   - ✓ 必须同时看右侧和左侧（`--it`），取较近的
   - 原因：`lower_bound` 找到 ≥ house 的第一个，但最近的可能是左边那个 < house 的

2. **双指针忘记取 abs**:
   - ✗ `heaters[j+1] - houses[i] <= heaters[j] - houses[i]`
   - ✓ `abs(heaters[j+1] - houses[i]) <= abs(heaters[j] - houses[i])`
   - 原因：供暖器可能在房屋左侧，差值为负

3. **二分答案上界设太小**:
   - ✗ `hi = max(houses.back(), heaters.back())`
   - ✓ `hi = max(abs(houses.back() - heaters.front()), abs(houses.front() - heaters.back()))`
   - 原因：房屋可能在所有供暖器的同一侧极远处

4. **忘记排序**:
   - 解法2必须排序 heaters，解法3/4必须同时排序 houses 和 heaters
   - 不排序的话二分查找/双指针/贪心都不成立

## 面试追问

**Q1: 你能用最直觉的方式解释这道题的本质吗？**
→ 每栋房屋需要被"最近的供暖器"覆盖，半径由"离供暖器最远的那栋房屋"决定。数学表达：`ans = max_i(min_j |houses[i] - heaters[j]|)`。

**Q2: 暴力 O(m×n) 的瓶颈在哪？怎么优化？**
→ 瓶颈是对每栋房屋线性搜索所有供暖器。排序后用二分查找优化到 O(log n)。或者两个数组都排序后用双指针一遍扫完。

**Q3: 如果这道题变成"供暖器可以放在任意位置，给你 k 个供暖器，最小半径是多少"？**
→ 这就变成了一个更复杂的问题。可以二分答案 r，然后判定"用 k 个半径为 r 的区间能否覆盖所有点"——这是经典的区间覆盖贪心。排序房屋后，贪心地放供暖器：第一个未覆盖的房屋位置 + r 处放一个供暖器，覆盖 [house, house+2r]，看 k 个够不够。

**Q4: 如果房屋和供暖器的坐标范围很大（10^18），解法有影响吗？**
→ 解法2和3不受影响（只用到排序和比较）。解法4的二分答案上界变大了，但 log(10^18) ≈ 60，乘上 O(m+n) 也完全可行。注意用 `long long` 防溢出。

## 相关题型

- **LeetCode 69. x 的平方根** — 复用"二分答案"思想：二分猜 ans，验证 ans² ≤ x。
- **LeetCode 410. 分割数组的最大值** — 复用"最小化最大值 → 二分答案 + 贪心判定"的模式，判定函数改为"贪心分割看是否不超过 k 段"。
- **LeetCode 658. 找到 K 个最接近的元素** — 复用"排序后二分查找最近位置"的技巧。
- **LeetCode 855. 考场就座** — 类似思想：在有序位置中找"距离最远的间隔"。