# 752. 打开转盘锁

## 核心思路

把每个4位密码看作图上的一个**节点**，每次拨动一个轮盘就是走一条**边**。从 `"0000"` 到 `target` 的最少旋转次数，就是这张图上的**最短路径**——经典 BFS 问题。

## 思维链

1. **读完题第一反应**：从 `"0000"` 出发，每次可以改变4个轮盘中的一个，每个轮盘可以+1或-1（循环），所以每个状态有 **4×2 = 8** 个邻居。我们要找到达 `target` 的最少步数。

2. **建模成图**：一共有 10^4 = 10000 个可能的状态（`"0000"` ~ `"9999"`），每个状态有8条边。deadends 是图上的"墙"，不能经过。这不就是一个**无权图上的最短路径**问题吗？

3. **无权图最短路径 → BFS**：BFS 天然保证第一次到达某个节点时就是最短路径。用队列逐层扩展，第一次碰到 `target` 时的层数就是答案。

4. **优化方向 → 双向 BFS**：普通 BFS 从起点单向搜索，搜索空间像一棵"扇形"扩展。双向 BFS 同时从起点和终点搜索，两边"相遇"时停止。搜索空间从 O(8^d) 缩小到 O(2 × 8^(d/2))，实际快很多。

5. **进一步思考**：A* 搜索可以用启发式函数（如4位数字的最小旋转距离之和）来引导搜索方向，但面试中一般不要求。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 单向 BFS | 从 `"0000"` 出发逐层扩展 | O(10^4 × 4) | O(10^4) | ⭐ 必须写出 |
| 双向 BFS | 从起点和终点同时搜索 | O(10^4 × 4) 但常数小很多 | O(10^4) | 加分项 |
| A* 搜索 | 启发式函数引导搜索 | 依赖启发函数 | O(10^4) | 了解即可 |

## 关键提示

1. **状态空间有限**：只有10000个状态，完全可以枚举。把 visited 用 `unordered_set<string>` 实现，deadends 也塞进 visited 里当"预访问"。

2. **邻居生成**：对4位中的每一位，可以 +1 或 -1（mod 10），共8个邻居。注意 `'9'+1='0'` 和 `'0'-1='9'` 的循环处理。

3. **特殊边界**：`"0000"` 本身可能就在 deadends 里！此时直接返回 -1。

4. **双向 BFS 的关键**：每轮选择集合**更小**的那一端扩展，保持两端搜索规模平衡。

5. **BFS 扩展过程示意**：
```
层0: {"0000"}
         ↓ 拨动4个轮盘各±1
层1: {"1000","9000","0100","0900","0010","0090","0001","0009"}  (8个邻居)
         ↓ 每个节点再扩展8个邻居，排除已访问和deadends
层2: {...}
         ↓
...直到找到 target
```

## 解法详解

### 解法1: 单向 BFS — O(10^4 × 4) / O(10^4) ⭐ 面试首选

**思考过程**：无权图最短路径 = BFS。把每个4位字符串当作节点，从 `"0000"` 开始，逐层扩展所有可达邻居。deadends 和已访问节点都不能再走。第一次到达 target 时的步数就是答案。

```cpp
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        // 用 visited 同时记录 deadends 和已访问状态
        unordered_set<string> visited(deadends.begin(), deadends.end());
        
        // 边界：起点就是 deadend
        if (visited.count("0000")) return -1;
        // 边界：起点就是目标
        if (target == "0000") return 0;
        
        queue<string> q;
        q.push("0000");
        visited.insert("0000");
        int steps = 0;
        
        while (!q.empty()) {
            steps++;
            int size = q.size(); // 当前层的节点数
            for (int i = 0; i < size; i++) {
                string curr = q.front();
                q.pop();
                // 生成 8 个邻居
                for (int j = 0; j < 4; j++) {
                    for (int d = -1; d <= 1; d += 2) { // -1 和 +1
                        string next = curr;
                        // 循环处理：'0'-1='9', '9'+1='0'
                        next[j] = (curr[j] - '0' + d + 10) % 10 + '0';
                        if (next == target) return steps;
                        if (!visited.count(next)) {
                            visited.insert(next);
                            q.push(next);
                        }
                    }
                }
            }
        }
        return -1; // 无法到达
    }
};
```

**关键点**：
- 把 deadends 直接塞进 visited 集合，省去了单独判断的逻辑
- `(curr[j] - '0' + d + 10) % 10 + '0'` 处理了循环拨动，`+10` 防止负数取模

---

### 解法2: 双向 BFS — O(10^4 × 4) / O(10^4)

**从解法1优化**：单向 BFS 从起点"一路往外扩"，如果答案在第 d 步，搜索空间大约是 8^d。双向 BFS 同时从起点和终点搜索，各自只需扩展 d/2 层，搜索空间约 2 × 8^(d/2)，指数减半。

```
// 单向 BFS:   起点 ──────────────────→ 终点
//             搜索空间 ≈ 8^d
//
// 双向 BFS:   起点 ──────→ ←────── 终点
//             各 ≈ 8^(d/2)，总共 ≈ 2 × 8^(d/2)
```

**核心策略**：维护两个集合 `front` 和 `back`，每轮选择**更小**的集合扩展（让两端搜索规模尽量平衡）。当某个邻居出现在另一端集合中时，说明相遇了。

```cpp
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> visited(deadends.begin(), deadends.end());
        
        if (visited.count("0000")) return -1;
        if (target == "0000") return 0;
        
        // 两端的"前沿"集合
        unordered_set<string> front_set, back_set;
        front_set.insert("0000");
        back_set.insert(target);
        visited.insert("0000");
        visited.insert(target);
        int steps = 0;
        
        while (!front_set.empty() && !back_set.empty()) {
            steps++;
            
            // 总是扩展较小的集合 → 保持平衡
            if (front_set.size() > back_set.size()) {
                swap(front_set, back_set);
            }
            
            unordered_set<string> next_set; // 下一层的前沿
            for (const string& curr : front_set) {
                for (int j = 0; j < 4; j++) {
                    for (int d = -1; d <= 1; d += 2) {
                        string next = curr;
                        next[j] = (curr[j] - '0' + d + 10) % 10 + '0';
                        // 如果在对面的前沿中找到 → 两端相遇
                        if (back_set.count(next)) return steps;
                        if (!visited.count(next)) {
                            visited.insert(next);
                            next_set.insert(next);
                        }
                    }
                }
            }
            front_set = next_set; // 更新前沿
        }
        return -1;
    }
};
```

**关键点**：
- `swap(front_set, back_set)` 保证总是扩展小的一端
- 相遇判断：邻居存在于**对面集合**中
- visited 在两端共享，避免重复访问

## 解法对比

| | 单向 BFS | 双向 BFS |
|------|------|------|
| 实现难度 | 简单，标准BFS模板 | 中等，需要维护两个集合 |
| 实际性能 | 可能遍历大量无关状态 | 搜索空间指数级缩小 |
| 面试推荐 | 先写这个，保证正确 | 被追问优化时再写 |
| 数据结构 | queue + set | 两个 set（无需 queue） |

## 易错点

1. **忘记判断起点在 deadends 中**
   - ✗ 直接开始 BFS，当 `"0000"` 是 deadend 时会错误地将它入队
   - ✓ BFS 前先 `if (visited.count("0000")) return -1;`

2. **循环拨动的取模写法错误**
   - ✗ `next[j] = (curr[j] - '0' + d) % 10 + '0'`  — 当 `d=-1` 且 `curr[j]='0'` 时，`(-1) % 10` 在 C++ 中是 `-1`，不是 `9`
   - ✓ `next[j] = (curr[j] - '0' + d + 10) % 10 + '0'`  — `+10` 保证被除数非负

3. **双向 BFS 中 visited 标记时机错误**
   - ✗ 检查到邻居在 `back_set` 中后还标记为 visited 才 return → 不影响正确性但多此一举
   - ✗ 不把 target 加入 visited → 可能被错误跳过
   - ✓ 初始化时 `visited.insert("0000")` 和 `visited.insert(target)` 都要做

4. **忘记 target == "0000" 的特判**
   - ✗ 没有特判，BFS 至少走一步才检查 → 返回错误值
   - ✓ 单独处理 `if (target == "0000") return 0;`

## 面试追问

**Q1: 这道题的本质模型是什么？为什么用 BFS 而不是 DFS？**
> 无权图的最短路径问题。BFS 逐层扩展，第一次到达即最短。DFS 会找到路径但不保证最短，除非遍历所有路径取最小值，代价太大。

**Q2: 单向 BFS 最坏要访问多少个状态？能优化吗？**
> 最坏访问全部 10^4 = 10000 个状态。用双向 BFS 可以大幅减少实际访问的状态数——两端各搜索一半深度，搜索空间从 8^d 降到 2 × 8^(d/2)。

**Q3: 如果不是4位数字锁而是 n 位，每位有 k 种字符，复杂度如何？**
> 状态空间 k^n，每个状态有 2n 个邻居。BFS 时间 O(k^n × n)，空间 O(k^n)。当 n 和 k 较大时状态空间爆炸，需要 A* 或 IDA* 等启发式搜索。

**Q4: 能否用 A* 搜索进一步优化？启发式函数怎么设计？**
> 启发式函数 h(state) = 每位数字到 target 对应位的最小旋转距离之和。例如 `"1234"` 到 `"5678"`，h = min(4,6) + min(4,6) + min(4,6) + min(4,6) = 4+4+4+4=16。这个 h 是 admissible 的（不会高估），因此 A* 保证最优解。

## 相关题型

- **127. 单词接龙 (Word Ladder)** — 复用完全相同的 BFS / 双向 BFS 框架。区别：邻居不是拨轮±1，而是改变一个字母；状态空间由 wordList 决定而非固定 10^4。
- **126. 单词接龙 II** — 在 127 基础上要求输出所有最短路径，需要 BFS + 回溯。
- **773. 滑动谜题 (Sliding Puzzle)** — 同样是状态空间 BFS 求最少步数，状态表示和邻居生成方式不同（棋盘上滑块移动）。
- **854. K-Similar Strings** — 状态空间 BFS，每次操作是交换两个字符。复用 BFS 最短路模板。