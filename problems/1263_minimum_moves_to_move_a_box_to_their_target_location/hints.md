# 1263. 推箱子

## 核心思路

经典的 **Sokoban（推箱子）问题**。求最少推动次数将箱子 'B' 推到目标 'T'。关键难点：**玩家需要先走到箱子的对面才能推动它**，而"走到对面"这个过程不算推动次数。因此本题需要 **双层搜索**——外层搜索箱子的最短推动次数，内层判断玩家能否走到正确位置来执行推动。

最优解法是 **0-1 BFS（deque BFS）** 或 **BFS + 内层 BFS**，状态定义为 `(箱子位置, 玩家位置)`。

## 思维链

1. **读完题第一反应**：最少推动次数 → BFS。但不是普通的网格 BFS——玩家每次要先走到箱子旁边的特定位置才能推。

2. **状态定义怎么想？** 仅记录箱子位置不够——同样的箱子位置，玩家在不同位置，后续可达状态不同。所以状态必须包含 **(箱子位置, 玩家位置)**。状态空间 O(m^2 * n^2)。

3. **如何优化状态？** 观察：推动箱子后，玩家一定在箱子的前一个位置。所以可以将状态压缩为 **(箱子位置, 推动方向)**，因为推完后玩家位置 = 箱子的旧位置。这将状态空间减小到 O(m*n*4)。

4. **搜索策略**：外层 BFS 搜索箱子移动。对于当前状态，尝试 4 个推动方向。每个方向需要检查：(a) 玩家能否走到箱子的对面（内层 BFS），(b) 推动后箱子的新位置是否合法。

5. **0-1 BFS 替代方案**：将状态扩展为 `(boxR, boxC, playerR, playerC)`。玩家移动 = cost 0（deque 前端），推箱子 = cost 1（deque 后端）。这样一次 BFS 就搞定，无需嵌套。

6. **为什么不用 Dijkstra？** 边权只有 0 和 1，0-1 BFS 就足够了（deque 代替优先队列），时间复杂度 O(V+E) 比 Dijkstra 的 O(V log V + E) 更好。

## 推箱子状态转移图示

```
示例 1 的网格:
  # # # # # #
  # T # # # #
  # . . B . #
  # . # # . #
  # . . . S #
  # # # # # #

箱子在 (2,3), 玩家在 (4,4), 目标在 (1,1)

尝试推箱子向左 (方向: 0,-1):
  1. 玩家需要站到箱子右边 (2,4) → 能否从 (4,4) 走到 (2,4)?
     内层 BFS: (4,4)→(3,4)→(2,4) ✓ (不经过箱子位置(2,3))
  2. 推动: 箱子 (2,3)→(2,2), 玩家移到 (2,3)
  3. cost = 1

继续推箱子向左:
  玩家在 (2,3), 箱子在 (2,2)
  玩家需要站到箱子右边 (2,3) → 已经在! ✓
  推动: 箱子 (2,2)→(2,1), 玩家移到 (2,2)
  cost = 2

推箱子向上:
  玩家在 (2,2), 箱子在 (2,1)
  玩家需要站到箱子下方 (3,1) → 内层 BFS: (2,2)→(3,1)? ✓
  推动: 箱子 (2,1)→(1,1) = 目标! 
  cost = 3 ← 答案
```

## BFS 状态搜索可视化

```
外层 BFS 队列（每一层 = 一次推动）:

=== Level 0 (pushes=0) ===
  状态: box=(2,3), player=(4,4)
  箱子不在目标，尝试推动...

=== Level 1 (pushes=1) ===
  尝试4个方向:
    向左推: 玩家需在(2,4)
      canReach((4,4)→(2,4), 绕过box(2,3))? 
      路线: (4,4)→(3,4)→(2,4) ✓
      → 入队: box=(2,2), player=(2,3), pushes=1
    
    向右推: 箱子到(2,4), 玩家需在(2,2)
      canReach((4,4)→(2,2), 绕过box(2,3))?
      路线: (4,4)→(4,3)→(4,2)→(4,1)→(3,1)→(2,1)→(2,2) ✓
      → 入队: box=(2,4), player=(2,3), pushes=1
    
    向上推: 玩家需在(3,3), grid[3][3]='#' → 不可达
    向下推: 玩家需在(1,3), grid[1][3]='#' → 不可达

=== Level 2 (pushes=2) ===
  取出 box=(2,2), player=(2,3):
    向左推: 玩家需在(2,3) → 已在! ✓
      → 入队: box=(2,1), player=(2,2), pushes=2
    ...

=== Level 3 (pushes=3) ===
  取出 box=(2,1), player=(2,2):
    向上推: 玩家需在(3,1)
      canReach((2,2)→(3,1), 绕过box(2,1))?
      路线: (2,2)→(3,2)→(4,2)→(4,1)→(3,1) ✓
      → box=(1,1) == target!  返回 3
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS + 内层BFS | 外层推箱子, 内层检查玩家可达 | O(m^2*n^2) | O(m*n) | 首选 |
| 0-1 BFS (deque) | 统一状态, 0-1 权重 BFS | O(m^2*n^2) | O(m^2*n^2) | 进阶 |
| A* 搜索 | 曼哈顿距离做启发式 | O(m^2*n^2) 最坏 | O(m^2*n^2) | 加分项 |

> m, n 为网格行列数。

## 关键提示

1. **状态是 (箱子位置, 推动方向)，不仅仅是箱子位置**：同一个箱子位置，玩家从不同方向来，可达的后续状态不同。用 `visited[boxR][boxC][dir]` 去重（4 个方向）。

2. **内层 BFS 中箱子是障碍**：玩家绕到箱子对面时，不能穿过箱子当前位置。所以内层 BFS 的 visited 要将箱子标记为不可通过。

3. **推动方向的玩家需求位置**：要向方向 (dr, dc) 推箱子，玩家必须站在 `(boxR - dr, boxC - dc)`（箱子的反方向），推后箱子到 `(boxR + dr, boxC + dc)`，玩家到 `(boxR, boxC)`（箱子的旧位置）。

4. **边界检查**：推动后的箱子新位置和玩家需求位置都要在网格内且不是墙。

5. **BFS 保证最优**：外层 BFS 每一层 = 推动一次，先找到的目标状态就是最短推动次数。

6. **0-1 BFS 版本的关键**：状态 `(boxR, boxC, playerR, playerC)`，玩家移动 cost=0 加到 deque 前端，推箱子 cost=1 加到后端。这样 deque 天然按 cost 排序。

## 解法详解

### 解法1: BFS + 内层 BFS — O(m^2*n^2) / O(m*n) 面试首选

**思路**：
- 外层 BFS：搜索箱子的最短推动路径。状态 = (boxR, boxC, playerR, playerC, pushes)。
- 每次从队列取出一个状态，尝试 4 个方向推箱子。
- 对每个方向，用内层 BFS 检查玩家能否从当前位置走到"推动需求位置"（箱子的对面）。
- 用 `visited[boxR][boxC][dir]` 去重，减少状态空间。

```
// 状态转移:
// 当前: 箱子(br,bc), 玩家(pr,pc)
// 推向方向 d=(dr,dc):
//   1. 玩家需求位 = (br-dr, bc-dc)  [箱子的反方向]
//   2. 检查: canReach(pr,pc → br-dr,bc-dc, 绕过箱子位置)
//   3. 新箱子位 = (br+dr, bc+dc), 新玩家位 = (br, bc)
//   4. pushes + 1
```

```cpp
class Solution {
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
    
    // 内层 BFS: 玩家能否从 (sr,sc) 走到 (tr,tc)?
    // 箱子位置 (boxR,boxC) 视为障碍
    bool canReach(vector<vector<char>>& grid, 
                  int sr, int sc, int tr, int tc, 
                  int boxR, int boxC) {
        if (sr == tr && sc == tc) return true;
        int m = grid.size(), n = grid[0].size();
        if (tr < 0 || tr >= m || tc < 0 || tc >= n) return false;
        if (grid[tr][tc] == '#') return false;
        
        vector<vector<bool>> vis(m, vector<bool>(n, false));
        queue<pair<int,int>> q;
        q.push({sr, sc});
        vis[sr][sc] = true;
        vis[boxR][boxC] = true; // 箱子视为障碍!
        
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n 
                    && !vis[nr][nc] && grid[nr][nc] != '#') {
                    if (nr == tr && nc == tc) return true;
                    vis[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
        return false;
    }
    
public:
    int minPushBox(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int boxR, boxC, playerR, playerC, targetR, targetC;
        
        // 找到 B, S, T 的位置
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 'B') { boxR = i; boxC = j; }
                if (grid[i][j] == 'S') { playerR = i; playerC = j; }
                if (grid[i][j] == 'T') { targetR = i; targetC = j; }
            }
        
        // visited[br][bc][dir]: 是否已从方向 dir 推箱子到 (br,bc)
        vector<vector<vector<bool>>> visited(
            m, vector<vector<bool>>(n, vector<bool>(4, false)));
        
        struct State {
            int br, bc, pr, pc, pushes;
        };
        queue<State> q;
        q.push({boxR, boxC, playerR, playerC, 0});
        
        while (!q.empty()) {
            auto [br, bc, pr, pc, pushes] = q.front(); q.pop();
            
            if (br == targetR && bc == targetC) return pushes;
            
            for (int d = 0; d < 4; d++) {
                // 玩家需要站的位置 (箱子的反方向)
                int needR = br - dirs[d][0], needC = bc - dirs[d][1];
                // 箱子推动后的新位置
                int newBR = br + dirs[d][0], newBC = bc + dirs[d][1];
                
                // 边界和障碍检查
                if (newBR < 0 || newBR >= m || newBC < 0 || newBC >= n) continue;
                if (grid[newBR][newBC] == '#') continue;
                if (visited[newBR][newBC][d]) continue;
                
                // 玩家能否走到需求位置? (不穿过箱子)
                if (canReach(grid, pr, pc, needR, needC, br, bc)) {
                    visited[newBR][newBC][d] = true;
                    // 推动后: 箱子到新位置, 玩家到箱子旧位置
                    q.push({newBR, newBC, br, bc, pushes + 1});
                }
            }
        }
        
        return -1; // 无法到达目标
    }
};
```

**关键点**：
- `visited[br][bc][dir]` 只有 m*n*4 个状态，比 m^2*n^2 小得多
- 内层 BFS 每次调用最坏 O(m*n)，外层最多 m*n*4 个状态，总时间 O(m^2*n^2)
- 一定不要忘记在内层 BFS 中将箱子标记为障碍

### 解法2: 0-1 BFS (Deque BFS) — O(m^2*n^2) / O(m^2*n^2)

**完全不同的思路**：将问题统一为一个图。状态 `(boxR, boxC, playerR, playerC)`，两种边：
- 玩家移动（不推箱子）：cost = 0，加到 deque 前端
- 玩家推箱子：cost = 1，加到 deque 后端

```
// 0-1 BFS 的边权:
// 
// 玩家自由移动 (不推箱子):
//   状态变化: (bR,bC,pR,pC) → (bR,bC,pR',pC')
//   代价: 0 → deque 前端
//
// 推箱子:
//   条件: 玩家在箱子旁边且推动方向有空位
//   状态变化: (bR,bC,pR,pC) → (bR',bC',bR,bC)
//   代价: 1 → deque 后端
```

```cpp
class Solution {
public:
    int minPushBox(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int bR, bC, pR, pC, tR, tC;
        
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 'B') { bR = i; bC = j; }
                if (grid[i][j] == 'S') { pR = i; pC = j; }
                if (grid[i][j] == 'T') { tR = i; tC = j; }
            }
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        // 状态编码
        auto encode = [&](int br, int bc, int pr, int pc) -> int {
            return ((br * n + bc) * m + pr) * n + pc;
        };
        
        unordered_map<int, int> dist;
        deque<tuple<int,int,int,int>> dq;
        
        int startState = encode(bR, bC, pR, pC);
        dist[startState] = 0;
        dq.push_back({bR, bC, pR, pC});
        
        while (!dq.empty()) {
            auto [br, bc, pr, pc] = dq.front(); dq.pop_front();
            int curDist = dist[encode(br, bc, pr, pc)];
            
            if (br == tR && bc == tC) return curDist;
            
            // 玩家自由移动 (cost = 0)
            for (auto& d : dirs) {
                int npr = pr + d[0], npc = pc + d[1];
                if (npr < 0 || npr >= m || npc < 0 || npc >= n) continue;
                if (grid[npr][npc] == '#') continue;
                
                if (npr == br && npc == bc) {
                    // 玩家走到箱子位置 = 推箱子 (cost=1)
                    int nbr = br + d[0], nbc = bc + d[1];
                    if (nbr < 0 || nbr >= m || nbc < 0 || nbc >= n) continue;
                    if (grid[nbr][nbc] == '#') continue;
                    int newState = encode(nbr, nbc, br, bc);
                    if (!dist.count(newState) || curDist + 1 < dist[newState]) {
                        dist[newState] = curDist + 1;
                        dq.push_back({nbr, nbc, br, bc}); // cost=1 → 后端
                    }
                } else {
                    // 普通移动 (cost=0)
                    int newState = encode(br, bc, npr, npc);
                    if (!dist.count(newState) || curDist < dist[newState]) {
                        dist[newState] = curDist;
                        dq.push_front({br, bc, npr, npc}); // cost=0 → 前端
                    }
                }
            }
        }
        
        return -1;
    }
};
```

## 解法对比

| | BFS + 内层BFS | 0-1 BFS |
|--|-------------|---------|
| 时间 | O(m^2*n^2) | O(m^2*n^2) |
| 空间 | O(m*n*4) | O(m^2*n^2) |
| 代码量 | 较多(两个BFS) | 中等(一个deque BFS) |
| 思路清晰度 | 高 | 中 |
| 面试建议 | 首选 | 追问优化 |

**选择建议**：
- 面试首选 BFS + 内层 BFS：思路直觉，分为"推箱子搜索"和"玩家可达性检查"两层
- 追问优化时给 0-1 BFS：展示对图论权重 BFS 的理解
- 注意解释为什么不需要 Dijkstra（只有 0 和 1 两种权重）

## 易错点

1. **内层 BFS 忘记将箱子标记为障碍**
   - X `canReach` 中不标记箱子位置 → 玩家"穿过"箱子
   - O `vis[boxR][boxC] = true;` 在 BFS 开始前

2. **推动方向和玩家需求位置搞反**
   - X 推向右 (0,1)，玩家需要在右边 (br, bc+1)
   - O 推向右 (0,1)，玩家需要在左边 (br, bc-1)（箱子的反方向）

3. **推后玩家位置设错**
   - X 推后玩家还在原位 → 状态错误
   - O 推后玩家在箱子的旧位置 `(br, bc)`

4. **visited 状态定义不对**
   - X `visited[newBR][newBC]` 只看箱子新位置 → 同一箱子位置不同方向被错误剪枝
   - O `visited[newBR][newBC][dir]` 加上推动方向

5. **初始状态箱子已在目标位置**
   - X 不检查初始状态 → 漏掉 pushes=0 的情况
   - O BFS 中取出状态时立即检查 `br==targetR && bc==targetC`

6. **0-1 BFS 中忘记跳过已处理的更优状态**
   - X 取出状态时不检查是否已有更短距离
   - O 用 dist map 记录最短距离，取出时对比

## 面试追问

**Q1: 为什么外层 BFS 能保证最短推动次数？**
→ BFS 逐层扩展。第 k 层的所有状态都恰好需要 k 次推动。首次到达目标的层数就是最少推动次数。这是 BFS 求最短路的标准性质。

**Q2: 状态空间有多大？实际会不会很慢？**
→ BFS + 内层 BFS 方案：外层状态 O(m*n*4)，m=n=20 时约 1600 个状态。每个状态内层 BFS O(m*n)=400。总计约 640000 次操作，非常快。0-1 BFS 方案：状态 O(m^2*n^2)=160000，也很快。

**Q3: 能否用 A* 搜索优化？**
→ 可以。启发式函数 h = 箱子到目标的曼哈顿距离。A* 在有良好启发式时能显著减少搜索节点数。但对于 m,n<=20 的小网格，BFS 已经足够快，A* 的优势不明显且代码更复杂。

**Q4: 如果有多个箱子多个目标呢？（通用 Sokoban）**
→ 通用 Sokoban 是 PSPACE-complete 问题。状态空间指数增长（每个箱子位置的组合）。实际求解需要 A* + 高质量启发式（如 pattern database）或 IDA*。这远超本题范围。

## 相关题型

- **864. 获取所有钥匙的最短路径** — 类似的 BFS 扩展状态（位置 + 持有钥匙状态），和本题的（箱子位置 + 玩家信息）思路一致。
- **1162. 地图分析** — 多源 BFS，网格上的 BFS 基础。
- **542. 01 矩阵** — 0-1 BFS / 多源 BFS 的经典应用。
- **1368. 使网格图至少有一条有效路径的最小代价** — 0-1 BFS 在网格上的应用，边权 0/1。
- **847. 访问所有节点的最短路径** — BFS + 状态压缩，多维状态搜索。
