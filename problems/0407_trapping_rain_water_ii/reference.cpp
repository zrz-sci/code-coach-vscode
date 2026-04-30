/*
 * LeetCode 407: 接雨水 II (Trapping Rain Water II)
 * 
 * 【题目本质】
 * 二维高度图中，计算"围住"的凹陷能存多少水。
 * 本质是一维接雨水(LeetCode 42)的二维推广：
 * 每个格子能存的水取决于围住它的一圈"围墙"中最矮的那块——木桶效应。
 *
 * 【解法总览】
 * 解法1: 最小堆 BFS (由外向内灌水) — O(mn·log(mn)) / O(mn) — 面试标准解法
 * 解法2: 并查集 (由低到高填充)     — O(mn·α(mn))    / O(mn) — 加分项/进阶理解
 *
 * 【从 1D 到 2D 的推广思路】
 * 1D 接雨水: 左右两堵墙，双指针从两端向中间夹逼，每次移动较矮的那端
 * 2D 接雨水: 一圈边界是围墙，用最小堆代替双指针，每次弹出最矮的围墙向内扩展
 * "双指针"是"最小堆"在一维上的特例！
 */

// ============================================================
// 解法1: 最小堆 BFS — 从边界向内灌水（木桶效应）
// 时间: O(mn·log(mn))  空间: O(mn)
//
// 【思路】
// 水一定从边界溢出，边界就是天然围墙。
// 木桶效应：水位取决于最矮的围墙。
// 所以我们用最小堆管理围墙，每次弹出最矮的围墙向内灌水：
//   - 如果邻居比水位矮 → 能蓄水（差值就是蓄水量）
//   - 邻居成为新围墙，高度 = max(自身高度, 当前水位)
//
// 【可视化 — 示例2】
//
//  原始高度:            最终水位(每格水面):     蓄水:
//  3 3 3 3 3           3 3 3 3 3              0 0 0 0 0
//  3 2 2 2 3           3 3 3 3 3              0 1 1 1 0
//  3 2 1 2 3    →      3 3 3 3 3       →      0 1 2 1 0
//  3 2 2 2 3           3 3 3 3 3              0 1 1 1 0
//  3 3 3 3 3           3 3 3 3 3              0 0 0 0 0
//                                          总计 = 10
//
// 堆处理过程示意 (简化):
//   初始堆: 所有边界(高度3), 全部相同
//   弹出任一边界(h=3), 看邻居(2,2,2,2) → 蓄水1+1+1+1
//   它们以 max(2,3)=3 入堆
//   继续弹出(h=3), 邻居(1) → 蓄水3-1=2
//   总蓄水 = 4×1 + 4×1 + 1×2 = 10 ✓
// ============================================================
class Solution1 {
public:
    int trapRainWater(vector<vector<int>>& heightMap) {
        int m = heightMap.size(), n = heightMap[0].size();
        // 至少3x3才可能在内部蓄水
        if (m <= 2 || n <= 2) return 0;
        
        // 最小堆: {高度, 行, 列}
        // 为什么用最小堆: 木桶效应，水从最矮的围墙溢出
        // 必须先处理最矮的，这样内部格子被处理时水位已确定
        priority_queue<tuple<int,int,int>, 
                       vector<tuple<int,int,int>>, 
                       greater<tuple<int,int,int>>> pq;
        
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        
        // 将四条边界全部入堆 — 边界是天然围墙，不可能蓄水
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 || i == m-1 || j == 0 || j == n-1) {
                    pq.push({heightMap[i][j], i, j});
                    // 入堆时就标记visited，防止同一格子重复入堆
                    visited[i][j] = true;
                }
            }
        }
        
        int water = 0;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        while (!pq.empty()) {
            auto [h, x, y] = pq.top();  // h = 当前围墙的有效高度（即水位线）
            pq.pop();
            
            // 检查四个邻居
            for (auto& d : dirs) {
                int nx = x + d[0], ny = y + d[1];
                
                // 越界或已处理 → 跳过
                if (nx < 0 || nx >= m || ny < 0 || ny >= n || visited[nx][ny])
                    continue;
                
                visited[nx][ny] = true;
                
                // 关键: 如果邻居比围墙矮，水能灌进去
                if (heightMap[nx][ny] < h) {
                    water += h - heightMap[nx][ny];
                }
                
                // 邻居成为新围墙
                // 为什么取 max: 如果邻居被水淹了(自身<h)，它对更内部格子的
                // "围墙高度"应该是水面高度h，而不是自身高度
                // 如果邻居本身就很高(自身>=h)，那围墙高度就是自身高度
                pq.push({max(h, heightMap[nx][ny]), nx, ny});
            }
        }
        
        return water;
    }
};


// ============================================================
// 解法2: 并查集 — 由低到高填充
// 时间: O(mn·α(mn)) ≈ O(mn)  空间: O(mn)
//
// 【思路】
// 换个角度：把所有格子按高度从小到大排序，依次"填入"地形。
// 每填入一个格子时：
//   - 它和已填入的相邻格子合并(Union)
//   - 如果合并后的连通分量接触到边界 → 水会从边界流出，不能蓄水
//   - 如果连通分量未接触边界 → 这个分量的所有格子都可以蓄水到当前高度
//
// 实际实现中，我们先填入所有格子(从低到高)，对每个连通分量
// 维护：是否接触边界(isBorder)、分量中格子数(size)。
// 当高度从 h 升到 h+1 时，所有高度≤h且不接触边界的分量中的
// 格子都多蓄了1单位水 → 但这样逐层计算很慢。
//
// 更高效的做法：对每个格子按高度排序后处理，计算每次合并时
// 新增的蓄水量。详见代码注释。
//
// 这个解法比解法1更快(没有log因子)，但实现复杂，面试中不推荐
// 作为首选，但能说出来是加分项。
// ============================================================
class Solution2 {
public:
    // 并查集
    vector<int> parent, rank_;
    vector<int> sz;         // 连通分量的格子数
    vector<bool> isBorder;  // 连通分量是否接触边界
    
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // 路径压缩
            x = parent[x];
        }
        return x;
    }
    
    // 合并，返回新根
    int unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return ra;
        // 按秩合并
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
        sz[ra] += sz[rb];
        isBorder[ra] = isBorder[ra] || isBorder[rb];
        return ra;
    }
    
    int trapRainWater(vector<vector<int>>& heightMap) {
        int m = heightMap.size(), n = heightMap[0].size();
        if (m <= 2 || n <= 2) return 0;
        
        int total = m * n;
        parent.resize(total);
        rank_.resize(total, 0);
        sz.resize(total, 1);
        isBorder.resize(total, false);
        
        for (int i = 0; i < total; i++) parent[i] = i;
        
        // 标记边界格子
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (i == 0 || i == m-1 || j == 0 || j == n-1)
                    isBorder[i * n + j] = true;
        
        // 按高度排序所有格子
        vector<pair<int,int>> cells; // (高度, 一维索引)
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                cells.push_back({heightMap[i][j], i * n + j});
        sort(cells.begin(), cells.end());
        
        vector<bool> added(total, false);
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        int water = 0;
        
        // 分组处理同一高度的格子
        int idx = 0;
        while (idx < total) {
            int curH = cells[idx].first;
            int start = idx;
            
            // 先把同一高度的格子全部标记为已添加
            // 再统一做合并（避免顺序问题）
            while (idx < total && cells[idx].first == curH) {
                added[cells[idx].second] = true;
                idx++;
            }
            
            // 对这批格子做合并
            for (int k = start; k < idx; k++) {
                int id = cells[k].second;
                int x = id / n, y = id % n;
                for (auto& d : dirs) {
                    int nx = x + d[0], ny = y + d[1];
                    if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                    int nid = nx * n + ny;
                    if (!added[nid]) continue; // 邻居还没被添加（更高）
                    unite(id, nid);
                }
            }
            
            // 计算蓄水：对这批格子中，所属连通分量不接触边界的
            // 在下一个高度到来之前，它们能蓄水到"下一个高度"
            // 但更简单的方式：最终统一计算
            // 这里我们先不在循环中累加，最后统一算
        }
        
        // 最终计算：对每个格子，如果它所在分量不接触边界是不可能的
        // （最终所有分量都会连在一起接触边界）
        // 所以需要在过程中计算——换一种实现方式：
        //
        // 实际上并查集做法的正确实现较复杂，需要在每一层记录
        // 非边界分量的面积并乘以高度差。这里给出简化的正确实现：
        
        // --- 重新实现：正确的并查集做法 ---
        // 重置
        for (int i = 0; i < total; i++) {
            parent[i] = i; rank_[i] = 0; sz[i] = 1;
            isBorder[i] = (i/n == 0 || i/n == m-1 || i%n == 0 || i%n == n-1);
        }
        fill(added.begin(), added.end(), false);
        
        water = 0;
        idx = 0;
        while (idx < total) {
            int curH = cells[idx].first;
            // 下一个不同高度
            int nextIdx = idx;
            while (nextIdx < total && cells[nextIdx].first == curH) nextIdx++;
            int nextH = (nextIdx < total) ? cells[nextIdx].first : curH;
            int heightDiff = nextH - curH;
            
            // 添加当前高度的所有格子
            for (int k = idx; k < nextIdx; k++) {
                added[cells[k].second] = true;
            }
            
            // 合并
            for (int k = idx; k < nextIdx; k++) {
                int id = cells[k].second;
                int x = id / n, y = id % n;
                for (auto& d : dirs) {
                    int nx = x + d[0], ny = y + d[1];
                    if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                    int nid = nx * n + ny;
                    if (!added[nid]) continue;
                    unite(id, nid);
                }
            }
            
            // 统计所有"不接触边界"的连通分量的总面积
            // 这些分量中的格子在 curH → nextH 之间能蓄 heightDiff 的水
            if (heightDiff > 0) {
                // 用 set 收集所有当前已添加格子的根
                unordered_map<int,int> rootSize;
                for (int k = 0; k < nextIdx; k++) {
                    int r = find(cells[k].second);
                    if (!isBorder[r]) {
                        rootSize[r] = sz[r];
                    }
                }
                for (auto& [r, s] : rootSize) {
                    water += s * heightDiff;
                }
            }
            
            idx = nextIdx;
        }
        
        return water;
    }
};


// ============================================================
// 实际面试推荐：直接用解法1（最小堆 BFS），简洁且正确。
// 解法2（并查集）实现细节多、容易出错，作为进阶理解即可。
// 下面给出解法1的精简版，面试时直接写这个：
// ============================================================
class Solution {
public:
    int trapRainWater(vector<vector<int>>& heightMap) {
        int m = heightMap.size(), n = heightMap[0].size();
        if (m <= 2 || n <= 2) return 0;
        
        // 最小堆: {高度, 行, 列}
        priority_queue<tuple<int,int,int>, 
                       vector<tuple<int,int,int>>, 
                       greater<>> pq;
        
        vector<vector<bool>> vis(m, vector<bool>(n, false));
        
        // 边界入堆
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (i == 0 || i == m-1 || j == 0 || j == n-1) {
                    pq.push({heightMap[i][j], i, j});
                    vis[i][j] = true;
                }
        
        int ans = 0;
        int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
        
        while (!pq.empty()) {
            auto [h, x, y] = pq.top(); pq.pop();
            for (int d = 0; d < 4; d++) {
                int nx = x+dx[d], ny = y+dy[d];
                if (nx < 0 || nx >= m || ny < 0 || ny >= n || vis[nx][ny]) continue;
                vis[nx][ny] = true;
                ans += max(0, h - heightMap[nx][ny]);
                pq.push({max(h, heightMap[nx][ny]), nx, ny});
            }
        }
        return ans;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 最小堆 BFS    | 解法2: 并查集          |
// |-------------|---------------------|----------------------|
// | 时间         | O(mn·log(mn))       | O(mn·α(mn)) ≈ O(mn) |
// | 空间         | O(mn)               | O(mn)                |
// | 思路方向      | 从外向内灌水          | 从低到高填充           |
// | 实现难度      | 中等                 | 高                    |
// | 面试推荐      | ⭐ 首选              | 加分项                |
// | 正确性保证    | 最小堆保证先处理最矮围墙 | 按高度排序保证逐层处理  |
//
// 【易错点】
// 1. 邻居入堆高度错误:
//    ✗ pq.push({heightMap[nx][ny], nx, ny})
//    ✓ pq.push({max(h, heightMap[nx][ny]), nx, ny})
//    原因: 如果邻居被水淹(自身<h), 它对更内部格子的围墙高度应是水面h
//
// 2. 边界入堆时未标记 visited:
//    ✗ 只在 pop 时标记 → 同一格子可能被多个邻居推入堆
//    ✓ push 时立即标记，保证每个格子只入堆一次
//
// 3. 忘记判断 m<=2 || n<=2:
//    ✗ 直接处理，边界就是全部格子，循环逻辑可能出问题
//    ✓ 提前返回 0
//
// 4. 蓄水量计算:
//    ✗ water += h - heightMap[nx][ny]  (可能为负)
//    ✓ water += max(0, h - heightMap[nx][ny])
//    或者用 if (heightMap[nx][ny] < h) 判断后再加
//
// 【面试追问链】
//
// Q1(基础理解): 和1D接雨水(LeetCode 42)有什么联系？
// A: 1D用双指针从两端向中间夹逼，每次移动较矮端。
//    2D的边界是一圈而非两个端点，用最小堆替代双指针，
//    但核心思想相同：从最矮处向内灌水（木桶效应）。
//
// Q2(正确性): 为什么最小堆能保证正确？能否用BFS(不用堆)?
// A: 普通BFS不行，因为水位不是按层均匀增长的。
//    必须用最小堆保证"永远先处理最矮围墙"，这样当内部格子
//    被处理时，它的水位已经由最短板决定了，不会再有更矮的出口。
//    这本质上是 Dijkstra 算法的变体（贪心+优先队列）。
//
// Q3(优化): 能否做到比 O(mn·log(mn)) 更快？
// A: 并查集做法可以达到近似 O(mn)。思路是按高度从低到高填充，
//    用并查集维护连通分量和边界信息。但实现复杂度远高于堆解法，
//    面试中堆解法已经足够。
//
// Q4(变体): 如果高度图是动态变化的（某些格子高度会增减），
//    如何高效重新计算蓄水量？
// A: 这是一个开放问题。可以考虑增量式更新：
//    - 格子变高：只影响局部，重新从该格子附近做堆BFS
//    - 格子变矮：更复杂，可能需要重新计算受影响区域的水位
//    完全动态的高效解法是研究级别的问题。
// ============================================================
