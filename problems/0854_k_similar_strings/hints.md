# 854. K-Similar Strings (相似度为 K 的字符串)

## 难度: Hard | 标签: BFS, Hash Table, String

---

## 题目理解

给定两个字母异位词 `s1` 和 `s2`(仅含 `{a,b,c,d,e,f}`，长度 <= 20)，每次可以交换 `s1` 中两个字母的位置。求**最少交换次数 k** 使 `s1` 变成 `s2`。

```
s1 = "ab",  s2 = "ba"  → 1 (swap(0,1): "ab" → "ba")
s1 = "abc", s2 = "bca" → 2 (swap(0,1): "abc"→"bac", swap(1,2): "bac"→"bca")
```

**核心建模**: 将每个字符串状态视为图中的**节点**，每次交换产生一条**边**。问题转化为**无权图最短路径** -- 经典 BFS。

---

## 为什么是 BFS?

| 特征 | 本题的匹配 |
|------|-----------|
| 状态 | 字符串的每一种排列 |
| 转移 | 交换任意两个位置 |
| 目标 | 从 s1 到 s2 |
| 代价 | 每次交换代价为 1 (等权) |
| 求解 | **最少操作次数** → 无权图最短路 → **BFS** |

> BFS 的核心性质: 第一次到达目标状态时，当前步数就是最短路径长度。

---

## BFS 状态空间可视化

### 示例 1: s1 = "abc", s2 = "bca"

```
Layer 0 (起点):
    "abc"
      │  位置0: s1[0]='a' ≠ s2[0]='b'
      │  在 j>0 中找 s1[j]='b': j=1
      │
      └─ swap(0,1)
         │
Layer 1:
    "bac"
      │  位置0: 'b'='b' ✓ (已匹配, 跳过)
      │  位置1: s1[1]='a' ≠ s2[1]='c'
      │  在 j>1 中找 s1[j]='c': j=2
      │
      └─ swap(1,2)
         │
Layer 2:
    "bca" == s2 ✓
    return 2
```

### 示例 2: s1 = "abac", s2 = "baca"

```
Layer 0:
    "abac"
      │  位置0: 'a' ≠ 'b', 找 j 使 s1[j]='b'
      │
      └─ swap(0,1): "baac"
         │
Layer 1:
    "baac"
      │  位置0: 'b'='b' ✓
      │  位置1: 'a'='a' ✓
      │  位置2: 'a' ≠ 'c', 找 j 使 s1[j]='c'
      │
      └─ swap(2,3): "baca" == s2 ✓
         return 2
```

### 通用 BFS 状态图

```
    ┌─────────┐
    │   s1    │     Layer 0 (0次交换)
    └────┬────┘
         │ 找第一个不匹配位 i
         │ 枚举 j>i 使 s1[j]==s2[i] 且 s1[j]!=s2[j]
    ┌────┴────┬──────────┬─────┐
    ▼         ▼          ▼     ▼       Layer 1 (1次交换)
  state1   state2    state3  state4
    │         │          │     │
    ▼         ▼          ▼     ▼       Layer 2 (2次交换)
   ...    ══s2══       ...   ...
          (找到!)
          return 2

关键: BFS 保证第一次到达 s2 时的层数 = 最小交换次数
```

---

## 三个关键剪枝 (将 TLE → AC)

### 剪枝 1: 只修正第一个不匹配位

**暴力做法**: 枚举所有 C(n,2) 种交换 → 分支因子巨大，状态爆炸

**优化做法**: 找到第一个 `curr[i] != s2[i]` 的位置 `i`，只交换 `i` 和某个 `j > i`

**为什么正确?**
- 位置 `i` 迟早需要被修正(它和目标不同)
- 先修正它还是先修正别的位置，总步数不变(因为修正操作独立)
- 但固定修正顺序(总是修正第一个)大幅减少了等价搜索路径

### 剪枝 2: 只和"有用的"位置交换

```
条件: curr[j] == s2[i]   // j 位置的字符正好是 i 位置需要的
```

如果 `curr[j] != s2[i]`，交换 (i,j) 后位置 i 仍然不匹配，这次交换白费了。

### 剪枝 3: 不破坏已匹配的位置

```
条件: curr[j] != s2[j]   // j 位置本身也是错位的
```

如果 `curr[j] == s2[j]`(j 已经匹配了)，交换会破坏一个正确位置，总步数不可能更优。

**综合条件**: `curr[j] == s2[i] && curr[j] != s2[j]`

**特殊情况**: 如果 `s2[i] == s2[j]`(即 i 需要的字符恰好也是 j 位置的目标)，交换后**同时修正两个位置** -- 这是最优的交换!

---

## 去重: visited 集合

同一个字符串状态可能通过不同的交换路径到达。必须用 `visited` 集合去重，否则:
- 同一个状态反复入队 → MLE (内存溢出)
- 同一个状态反复展开 → TLE (超时)

```
例: "abcd" → swap(0,1) → "bacd" → swap(2,3) → "badc"
    "abcd" → swap(2,3) → "abdc" → swap(0,1) → "badc"
    
"badc" 被两条路径到达, 只需保留第一次 (BFS 保证第一次最短)
```

---

## 完整算法

```
BFS(s1, s2):
    if s1 == s2: return 0
    queue = [s1], visited = {s1}, steps = 0
    
    while queue not empty:
        steps++
        for each curr in current layer:
            i = 第一个 curr[i] != s2[i] 的位置
            for j = i+1 to n-1:
                if curr[j] == s2[i] AND curr[j] != s2[j]:
                    next = swap(curr, i, j)
                    if next == s2: return steps
                    if next not in visited:
                        visited.add(next)
                        queue.add(next)
    
    return -1  // 对合法输入不会到这里
```

---

## 复杂度分析

| 方面 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | O(状态数 * n) | 最坏 O(n!)，但剪枝后远小于此 |
| 空间 | O(状态数) | visited 集合 + BFS 队列 |

字符集只有 6 个字母且长度 <= 20，重复字母多 → 实际可达状态数远小于 n!。

---

## 完整代码

### C++ (BFS + 剪枝, 推荐)

```cpp
class Solution {
public:
    int kSimilarity(string s1, string s2) {
        if (s1 == s2) return 0;

        int n = s1.size();
        unordered_set<string> visited;
        queue<string> q;

        q.push(s1);
        visited.insert(s1);
        int steps = 0;

        while (!q.empty()) {
            steps++;
            int layerSize = q.size();

            while (layerSize--) {
                string curr = q.front();
                q.pop();

                // 剪枝1: 找第一个不匹配位置
                int i = 0;
                while (i < n && curr[i] == s2[i]) i++;

                // 剪枝2+3: 只和有效且错位的位置交换
                for (int j = i + 1; j < n; j++) {
                    if (curr[j] != s2[i] || curr[j] == s2[j]) continue;

                    string next = curr;
                    swap(next[i], next[j]);

                    if (next == s2) return steps;  // 提前终止

                    if (!visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
        }

        return -1;
    }
};
```

### Python

```python
class Solution:
    def kSimilarity(self, s1: str, s2: str) -> int:
        if s1 == s2:
            return 0

        n = len(s1)
        visited = {s1}
        queue = [s1]
        steps = 0

        while queue:
            steps += 1
            next_queue = []

            for curr in queue:
                # 找第一个不匹配位置
                i = 0
                while i < n and curr[i] == s2[i]:
                    i += 1

                # 枚举有效交换
                for j in range(i + 1, n):
                    if curr[j] != s2[i] or curr[j] == s2[j]:
                        continue

                    nxt = list(curr)
                    nxt[i], nxt[j] = nxt[j], nxt[i]
                    nxt = ''.join(nxt)

                    if nxt == s2:
                        return steps

                    if nxt not in visited:
                        visited.add(nxt)
                        next_queue.append(nxt)

            queue = next_queue

        return -1
```

---

## 进阶: A* 搜索优化

### 启发函数

`h(state) = ceil(不匹配位置数 / 2)`

**为什么 /2?** 一次最优交换最多同时修正 2 个位置(当 `s2[i]` 和 `s2[j]` 互相需要对方的字符时)，所以 h 是到终点的合法下界(admissible)。

### A* 与 BFS 的区别

| 方面 | BFS | A* |
|------|-----|-----|
| 队列 | 普通队列 (FIFO) | 优先队列 (按 f=g+h 排序) |
| 展开顺序 | 按层 (均匀扩展) | 优先展开"离目标近"的状态 |
| 展开数量 | 较多 | 通常更少 |
| 最优性 | 保证 (无权图) | 保证 (h admissible) |

### A* 代码骨架

```cpp
// 优先队列: (f, g, state), f = g + h(state)
auto heuristic = [&](const string& s) -> int {
    int mismatch = 0;
    for (int i = 0; i < n; i++)
        if (s[i] != s2[i]) mismatch++;
    return (mismatch + 1) / 2;
};
// 其余与 BFS 类似, 只是用 priority_queue 替换 queue
```

---

## 常见错误

| 错误 | 后果 | 修正 |
|------|------|------|
| 忘记 `curr[j] != s2[j]` | 搜索空间膨胀, TLE | 加上此条件排除已匹配位 |
| 没有找第一个不匹配位 | 枚举所有(i,j)对, 大量等价状态 | 固定 i 为第一个不匹配位 |
| 忘记去重 (visited) | 同状态反复入队, MLE/TLE | unordered_set 记录已访问 |
| 用 DFS 而非 BFS | 不能保证最短路径 | 求最少步骤 → BFS |

---

## 思维模式: "状态空间搜索"

当问题涉及"**最少操作次数**将一个状态变成另一个状态"时:
1. **定义状态**: 字符串排列、棋盘布局、数字组合...
2. **定义转移**: 交换、翻转、增减...
3. **BFS 求最短路径**: 每步代价相同 → 无权图 → BFS

| 同类题 | 状态 | 转移 |
|--------|------|------|
| 127 Word Ladder | 单词 | 改一个字母 |
| 752 Open the Lock | 4位密码 | 转一位 |
| 773 Sliding Puzzle | 棋盘 | 滑动一块 |
| **854 K-Similar** | 字符串 | 交换两位 |

---

## 面试追问

**Q1: 为什么不能用 DFS 代替 BFS？**
> 求最少步骤等价于无权图最短路径，BFS 保证第一次到达目标即是最短。DFS 可能走更长路径，不能保证最短。

**Q2: A\* 搜索的启发函数为什么选 `ceil(不匹配数 / 2)`？**
> 一次最优交换最多同时修正 2 个位置（互相需要对方字符时），所以不匹配数 / 2 是合法下界（admissible），不会高估。

**Q3: 如果字符集更大（如 26 个小写字母），算法还可行吗？**
> 可行但状态空间增大。核心优化仍是三个剪枝（固定首个不匹配位、有效交换、不破坏已匹配）。字符串长度是瓶颈而非字符集大小。

## 面试沟通建议

1. **识别模式**: 看到"最少交换次数"→ 状态空间搜索 → BFS
2. **画状态图**: 用 "abc"→"bca" 的小例子画 2-3 层 BFS
3. **讲剪枝**: 说出三个剪枝策略(固定第一个不匹配位、有用交换、不破坏已匹配)
4. **谈进阶**: 提到 A* 优化及启发函数设计
