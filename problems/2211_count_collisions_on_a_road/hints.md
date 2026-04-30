# 2211. 统计道路上的碰撞次数 (Count Collisions on a Road)

## 核心思路

**关键洞察 + 双指针**：最左端连续的 `L` 向左逃逸永不碰撞，最右端连续的 `R` 向右逃逸永不碰撞。去掉这两端的"逃逸车"后，中间部分的每个 `R` 和 `L` 最终都会参与碰撞（因为中间必有阻挡），每个贡献 1 次碰撞。答案 = 中间区间内 `R` 和 `L` 的总数。

## 思维链

1. **读题审题**：n 辆车在一条无限长公路上，每辆车状态为 `L`（向左）、`R`（向右）或 `S`（静止）。所有运动车速度相同。碰撞规则：
   - R 碰 L（相向）→ +2
   - R 碰 S 或 S 碰 L（运动撞静止）→ +1
   - 碰撞后车辆变为静止（新的障碍物，可能引发连锁碰撞）

2. **直觉：碰撞后变静止会连锁**：

```
"RLRSLL" 的碰撞过程:

初始:   R  L  R  S  L  L
        →  ←  →  .  ←  ←
                   
时刻1: 车0(R)碰车1(L) → +2, 两者变 S
        S  S  R  S  L  L
                   
时刻2: 车2(R)碰车3(S) → +1, 车2变 S
        S  S  S  S  L  L
              
时刻3: 车4(L)碰车3(S) → +1, 车4变 S
        S  S  S  S  S  L
                         
时刻4: 车5(L)碰车4(S) → +1, 车5变 S
        S  S  S  S  S  S
                         
总碰撞: 2+1+1+1 = 5 ✓
```

3. **哪些车永远不会碰撞？**

```
物理直觉:
- 最左边连续向左的车 → 一直向左飞走, 左边没有任何东西能挡住它们
- 最右边连续向右的车 → 一直向右飞走, 右边没有任何东西能挡住它们

例: "LLRRSLL"
     ^^        ← 这两辆 L 向左逃逸
     这里没有连续的尾部 R

例: "LLRRSSRR"
     ^^          ← 左端 LL 逃逸
              ^^ ← 右端 RR 逃逸
     中间 "RRSS" 里的 R 会撞上 S
```

4. **核心观察——中间部分的 R 和 L 一定会碰撞**：

```
去掉两端逃逸车后, 中间部分满足:
  - 最左边不是 L (否则它已被归入"左端逃逸")
  - 最右边不是 R (否则它已被归入"右端逃逸")

所以中间的每辆 R 向右走时:
  它右边一定有 S 或 L (因为最右端没有纯 R 段)
  → 一定会碰撞, 贡献 1 次碰撞

中间的每辆 L 向左走时:
  它左边一定有 S 或 R (因为最左端没有纯 L 段)
  → 一定会碰撞, 贡献 1 次碰撞

中间的 S 本身不产生碰撞 (它只是被撞的目标)
```

5. **碰撞次数的优雅数学公式**：

```
每个运动中的车, 要么逃逸, 要么碰撞停下。
停下 = 贡献 1 次碰撞。

碰撞次数 = (中间区间的 R 数量) + (中间区间的 L 数量)
         = (中间区间长度) - (中间区间的 S 数量)

等价公式:
  碰撞次数 = n - (左端连续L数) - (右端连续R数) - (总S数)

验证 "RLRSLL":
  左端连续L: 0 (第一个字符是R)
  右端连续R: 0 (最后字符是L)
  总S: 1
  碰撞 = 6 - 0 - 0 - 1 = 5 ✓

验证 "LLRR":
  左端连续L: 2
  右端连续R: 2
  总S: 0
  碰撞 = 4 - 2 - 2 - 0 = 0 ✓
```

6. **双指针实现**：

```
1. left 指针从左端向右跳过所有 'L'
2. right 指针从右端向左跳过所有 'R'
3. 遍历 [left, right], 统计非 'S' 字符数量

"RLRSLL":
 left=0 (第一个不是L, 不跳)
 right=5 (最后不是R, 不跳)
 中间 [0,5] = "RLRSLL"
 非S: R,L,R,L,L = 5  → 答案 5 ✓

"LLRR":
 left: L,L → left=2
 right: R,R → right=1
 left > right → 循环不执行 → 答案 0 ✓
```

7. **为什么不需要模拟？**

```
模拟的困难:
- 碰撞有连锁效应 (车A撞停 → 变S → 车B再撞上)
- 需要追踪每辆车的状态变化
- 时间复杂度可能达到 O(n^2)

数学方法的优雅:
- 跳过两端逃逸车 → O(n)
- 统计中间的 R 和 L → O(n)
- 不需要关心碰撞的具体顺序和连锁反应!
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 双指针计数 | 去掉两端逃逸车, 统计中间非S | O(n) | O(1) | 最优 |
| 公式法 | n - 左端L数 - 右端R数 - 总S数 | O(n) | O(1) | 最简洁 |
| 栈模拟 | 用栈模拟碰撞过程 | O(n) | O(n) | 直观但复杂 |

## 关键提示

- **提示1**: 不要尝试逐步模拟碰撞！连锁反应很难正确处理。直接用数学观察：只有两端的车能逃逸。
- **提示2**: 去掉两端逃逸车后，中间的每个 `R` 和 `L` 都一定会碰撞，每个贡献恰好 1 次碰撞。这是因为中间不可能有"通道"让它们逃走。
- **提示3**: `S` 本身不贡献碰撞次数——它只是被撞的目标，撞它的运动车贡献了碰撞。
- **提示4**: 公式法更简洁：`碰撞次数 = n - leftL - rightR - totalS`。一次遍历即可算出所有变量。
- **提示5**: 栈模拟法也能做（类似 LC 735 行星碰撞），但代码更长且有更多 edge case。面试时优先用双指针/公式法。

```
栈模拟核心逻辑:
  R → 入栈
  S → 弹出所有栈中R(每个+1), 入栈S
  L → 若栈顶R: +2, 弹R, 继续弹剩余R(每个+1), 入栈S
       若栈顶S: +1
       若空/栈顶L: 逃逸

验证 "RLRSLL":
  R → stk=[R]
  L → 栈顶R, +2, pop, stk=[], push S → stk=[S], 碰撞=2
  R → stk=[S,R]
  S → 弹R(+1), stk=[S], push S → stk=[S,S], 碰撞=3
  L → 栈顶S, +1, 碰撞=4
  L → 栈顶S, +1, 碰撞=5 ✓
```

## 解法详解

### 解法1: 双指针（推荐）

1. `left` 从左向右跳过连续 `'L'`
2. `right` 从右向左跳过连续 `'R'`
3. 遍历 `[left, right]` 统计非 `'S'` 字符数

**正确性**：两端的 `L`/`R` 是唯一逃逸的车。中间的每个运动车最终停下（碰到 S 或对向车），贡献 1 次碰撞。

### 解法2: 公式法

```
碰撞次数 = n - (左端连续L数) - (右端连续R数) - (总S数)
```

只需一次遍历统计三个值，然后做减法。

### 解法3: 栈模拟

从左到右遍历，用栈维护"当前活跃的车"。遇到碰撞即时结算。栈中只会有 `R` 和 `S`（`L` 要么逃逸要么立即碰撞）。

## 完整代码

```cpp
// 解法1: 双指针 (推荐)
class Solution {
public:
    int countCollisions(string directions) {
        int n = directions.size();
        int left = 0, right = n - 1;

        // 跳过左端连续 L (逃逸)
        while (left < n && directions[left] == 'L') left++;
        // 跳过右端连续 R (逃逸)
        while (right >= 0 && directions[right] == 'R') right--;

        // 中间 [left, right] 内的 R 和 L 都会碰撞
        int collisions = 0;
        for (int i = left; i <= right; i++) {
            if (directions[i] != 'S') {
                collisions++;
            }
        }
        return collisions;
    }
};
```

```cpp
// 解法2: 公式法 (最简洁)
class Solution2 {
public:
    int countCollisions(string directions) {
        int n = directions.size();
        int leftL = 0;
        while (leftL < n && directions[leftL] == 'L') leftL++;
        int rightR = 0;
        while (rightR < n && directions[n - 1 - rightR] == 'R') rightR++;
        int totalS = count(directions.begin(), directions.end(), 'S');
        return max(0, n - leftL - rightR - totalS);
    }
};
```

```python
class Solution:
    def countCollisions(self, directions: str) -> int:
        # 去掉左端逃逸的 L
        s = directions.lstrip('L')
        # 去掉右端逃逸的 R
        s = s.rstrip('R')
        # 中间的非 S 字符数 = 碰撞次数
        return len(s) - s.count('S')
```

```cpp
// 解法3: 栈模拟
class Solution3 {
public:
    int countCollisions(string directions) {
        int collisions = 0;
        vector<char> stk;

        for (char d : directions) {
            if (d == 'R') {
                stk.push_back('R');
            } else if (d == 'S') {
                while (!stk.empty() && stk.back() == 'R') {
                    collisions++;
                    stk.pop_back();
                }
                stk.push_back('S');
            } else {  // 'L'
                if (!stk.empty() && stk.back() == 'R') {
                    collisions += 2;
                    stk.pop_back();
                    while (!stk.empty() && stk.back() == 'R') {
                        collisions++;
                        stk.pop_back();
                    }
                    stk.push_back('S');
                } else if (!stk.empty() && stk.back() == 'S') {
                    collisions++;
                }
                // else: 栈空或栈顶L → 这个L逃逸
            }
        }
        return collisions;
    }
};
```

## 面试追问

1. **"为什么不能用模拟？"** → 可以但没必要。模拟连锁碰撞需要 O(n^2)，而数学方法 O(n)。面试时优先展示洞察力。
2. **"为什么中间的 R/L 一定碰撞？"** → 中间最左不是 L（否则归入逃逸段），最右不是 R（同理）。所以任何 R 向右走一定遇到 S 或 L，任何 L 向左走一定遇到 S 或 R。
3. **"Python 的 lstrip/rstrip 写法很巧"** → 是的。`directions.lstrip('L').rstrip('R')` 精确地去掉两端逃逸车。然后 `len - count('S')` 就是非 S 字符数。
4. **"跟行星碰撞 (LC 735) 有什么区别？"** → 735 需要知道碰撞后剩余哪些行星（用栈模拟）。本题只需要碰撞总次数，可以用数学方法跳过模拟。
5. **"碰撞次数为什么等于中间非 S 数？"** → 每个运动车要么逃逸（两端），要么碰撞停下（中间）。"碰撞停下"意味着它参与了一次碰撞。所以中间每个 R/L 贡献恰好 1 次。

## 相关题目

| 题目 | 关联 |
|------|------|
| 735. Asteroid Collision | 栈模拟碰撞（经典对比题） |
| 853. Car Fleet | 车辆追及问题 |
| 1503. Last Moment Before All Ants Fall | 碰撞等价转化（脑筋急转弯） |
| 2751. Robot Collisions | 更复杂的碰撞模拟 |

## 复杂度分析

- **双指针/公式法**: 时间 O(n)，空间 O(1)
- **栈模拟**: 时间 O(n)（每辆车至多入栈出栈各一次），空间 O(n)
