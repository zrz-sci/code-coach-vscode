# 365. 水壶问题 (Water and Jug Problem)

## 难度: Medium
## 标签: Depth-First Search, Breadth-First Search, Math

---

## 题目理解

有两个水壶容量分别为 x 升和 y 升，水源无限。可以进行三种操作：
1. **装满**任意一个水壶
2. **清空**任意一个水壶
3. **从一个壶倒入另一个壶**，直到接水壶满或倒水壶空

判断能否通过这些操作使两个壶中的水的**总量**恰好等于 target。

关键观察：
- target 是两个壶中水的**总量**，不是某一个壶的水量
- 每个壶只能装满或清空，不能精确倒出某个量
- x=3, y=5, target=4: 通过反复倒水可以实现 (见题目步骤)
- x=2, y=6, target=5: 无法实现，因为 5 不是 gcd(2,6)=2 的倍数
- x=1, y=2, target=3: 两个壶都装满即可

---

## 核心思路

这道题有两种截然不同的思路：

1. **数学法 (裴蜀定理/Bezout's Identity)**: 能量出的水量都是 gcd(x, y) 的整数倍。所以 target 可以实现当且仅当 `target % gcd(x, y) == 0` 且 `target <= x + y`。

2. **BFS 搜索法**: 将 (壶1水量, 壶2水量) 作为状态，枚举所有可能的操作进行 BFS，检查是否能达到总量为 target 的状态。

数学法是 O(log(min(x,y))) 时间，BFS 法是 O(x*y) 时间。面试中应优先给出数学法，但要能解释 BFS 法作为备选。

---

## 思维链 (从观察到解法)

### 第一步: 理解操作的本质

每次操作的效果：
- 装满壶 x: 总水量增加若干(最多增加 x)
- 装满壶 y: 总水量增加若干(最多增加 y)
- 清空壶 x: 总水量减少若干(最多减少 x)
- 清空壶 y: 总水量减少若干(最多减少 y)
- 倒水: 总水量不变，只是重新分配

关键洞察: 从数学角度看，通过反复操作，能达到的水量是 ax + by 的形式(a, b 为任意整数)。

### 第二步: 裴蜀定理 (Bezout's Identity)

**定理**: 对于整数 a, b，方程 ax + by = c 有整数解当且仅当 gcd(a, b) | c (即 c 是 gcd(a,b) 的倍数)。

应用到本题：
- 能量出的水量集合 = {ax + by | a, b 为整数} = gcd(x, y) 的所有整数倍
- 但还有物理约束: 总水量不能超过 x + y

### 第三步: 完整判断条件

```
target 可以量出 ⟺ target <= x + y  且  target % gcd(x, y) == 0
```

特殊情况:
- target == 0: 总是可以(两个壶都空)
- x == 0 或 y == 0: target 必须等于另一个壶的容量或 0

### 第四步 (BFS 法): 状态空间搜索

将 (a, b) 表示两个壶的当前水量，起始状态 (0, 0)。
每个状态可以转移到最多 6 个新状态（装满x、装满y、清空x、清空y、x倒入y、y倒入x）。
用 visited 集合避免重复访问。

---

## 解法表
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| Solution1: 裴蜀定理 | O(log(min(x,y))) | O(1) | 数学最优解 |
| Solution2: BFS 状态搜索 | O(x*y) | O(x*y) | 直觉理解更容易 |

---

## 解法详解

### Solution1: 裴蜀定理 (数学法)

**裴蜀定理回顾**:
对于任意整数 a, b，存在整数 x, y 使得 ax + by = gcd(a, b)。
推论: ax + by 能表示的正整数恰好是 gcd(a, b) 的所有正整数倍。

**直觉解释**:
- gcd(3, 5) = 1，所以用 3 升和 5 升壶能量出任何 1 的倍数 = 任何整数
- gcd(2, 6) = 2，所以用 2 升和 6 升壶只能量出 2 的倍数 = {0, 2, 4, 6, 8}
  - 但不能超过 2 + 6 = 8，所以实际是 {0, 2, 4, 6, 8}
  - 5 不在其中，所以 x=2, y=6, target=5 返回 false

**手动验证 x=3, y=5, target=4**:
- gcd(3, 5) = 1
- 4 % 1 == 0 ✓
- 4 <= 3 + 5 = 8 ✓
- 返回 true

**手动验证 x=2, y=6, target=5**:
- gcd(2, 6) = 2
- 5 % 2 == 1 ≠ 0 ✗
- 返回 false

```cpp
class Solution1 {
public:
    bool canMeasureWater(int x, int y, int target) {
        if (target > x + y) return false;
        if (target == 0) return true;
        return target % gcd(x, y) == 0;
    }

    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }
};
```

---

### Solution2: BFS 状态搜索

**状态**: (壶1的水量, 壶2的水量)
**初始状态**: (0, 0)
**目标**: 存在状态 (a, b) 使得 a + b == target
**转移**: 6 种操作

```
操作1: 装满壶1 -> (x, b)
操作2: 装满壶2 -> (a, y)
操作3: 清空壶1 -> (0, b)
操作4: 清空壶2 -> (a, 0)
操作5: 壶1倒入壶2 -> (a - pour, b + pour), pour = min(a, y - b)
操作6: 壶2倒入壶1 -> (a + pour, b - pour), pour = min(b, x - a)
```

```cpp
class Solution2 {
public:
    bool canMeasureWater(int x, int y, int target) {
        if (target > x + y) return false;

        set<pair<int,int>> visited;
        queue<pair<int,int>> q;
        q.push({0, 0});
        visited.insert({0, 0});

        while (!q.empty()) {
            auto [a, b] = q.front(); q.pop();
            if (a + b == target) return true;

            // 生成所有可能的下一状态
            vector<pair<int,int>> nexts = {
                {x, b}, {a, y},           // 装满
                {0, b}, {a, 0},           // 清空
                {a - min(a, y - b), b + min(a, y - b)}, // 1->2
                {a + min(b, x - a), b - min(b, x - a)}  // 2->1
            };

            for (auto& next : nexts) {
                if (!visited.count(next)) {
                    visited.insert(next);
                    q.push(next);
                }
            }
        }
        return false;
    }
};
```

---

## 裴蜀定理的直觉理解

为什么壶的操作等价于 ax + by？

考虑一系列操作：
```
(0, 0) -> 装满y -> (0, 5)
         -> y倒x -> (3, 2)
         -> 清空x -> (0, 2)
         -> y倒x -> (2, 0)
         -> 装满y -> (2, 5)
         -> y倒x -> (3, 4)    <- 此时总量 = 5 + 5 - 3 - 3 = 4 = 2*5 - 2*3
```

每次"装满"等于 +x 或 +y，每次"清空"等于 -x 或 -y。
倒水只是重新分配，不改变总量。
所以最终总量 = (装满y的次数)*y + (装满x的次数)*x - (清空y的次数)*y - (清空x的次数)*x
= (某个整数)*x + (某个整数)*y
= ax + by 的形式

---

## GCD 算法回顾 (辗转相除法)

```
gcd(3, 5):
  gcd(5, 3) -> gcd(3, 2) -> gcd(2, 1) -> gcd(1, 0) -> 1

gcd(2, 6):
  gcd(6, 2) -> gcd(2, 0) -> 2
```

时间复杂度: O(log(min(a, b)))，因为每次至少减半。

---

## 易错点

1. **target > x + y 必须先判**: 两个壶加起来装不下 target 量的水
2. **target == 0 返回 true**: 两个壶都空就行
3. **x 或 y 为 0**: gcd(0, y) = y，仍然成立
4. **BFS 法容易超时**: 状态空间 O(x*y)，当 x, y 较大时很慢
5. **别忘了 C++ 的 __gcd 或手写 gcd**: C++17 有 `std::gcd`

---

## 面试追问递进链

1. **"不用数学定理，怎么做？"**
   -> BFS 搜索所有可能的 (壶1水量, 壶2水量) 状态

2. **"裴蜀定理为什么适用？"**
   -> 每次操作本质上是对总水量做 +x, -x, +y, -y 的变化
   -> 最终总水量 = ax + by (a, b 为整数)

3. **"如果有 3 个壶怎么办？"**
   -> gcd(x, gcd(y, z))，target 是这个值的倍数且不超过 x+y+z

4. **"如何输出具体操作步骤？"**
   -> 用 BFS 记录 parent 路径，回溯得到操作序列

5. **"GCD 的时间复杂度怎么证明是 O(log n)？"**
   -> 每两步至少让较大数减半(Fibonacci 数是最坏情况)

---

## 相关题型

- [914. X of a Kind in a Deck of Cards](../0914_x_of_a_kind_in_a_deck_of_cards/) - GCD 应用
- [1979. Find Greatest Common Divisor of Array](../1979_find_greatest_common_divisor_of_array/) - GCD 基础
- [752. Open the Lock](../0752_open_the_lock/) - BFS 状态搜索
