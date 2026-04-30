/*
 * LeetCode 1722: 执行交换操作后的最小汉明距离
 * 
 * 【题目本质】
 * 交换操作具有传递性，同一连通分量内的元素可以任意排列。
 * 问题转化为：对每个连通分量，source 和 target 的多重集能匹配多少个值。
 *
 * 【解法总览】
 * 解法1: Union-Find + 哈希计数 — O(n·α(n)) / O(n)   — 面试首选 ⭐
 * 解法2: DFS + 哈希计数       — O(n + E) / O(n + E) — 图遍历思路
 */

/*
 * 可视化示例:
 *
 * source = [1, 2, 3, 4]
 * target = [2, 1, 4, 5]
 * allowedSwaps = [[0,1], [2,3]]
 *
 * 图结构（下标为节点）:
 *
 *   0 --- 1     2 --- 3
 *
 * 连通分量:
 *   分量A: {0, 1}
 *     source 值池: {1, 2}
 *     target 值池: {2, 1}
 *     匹配: 1↔1, 2↔2 → 全部匹配 → 贡献 0
 *
 *   分量B: {2, 3}
 *     source 值池: {3, 4}
 *     target 值池: {4, 5}
 *     匹配: 4↔4 → 匹配1个, 3 和 5 无法匹配 → 贡献 1
 *
 * 汉明距离 = 0 + 1 = 1
 *
 * 示例3 可视化:
 *   source = [5, 1, 2, 4, 3]
 *   target = [1, 5, 4, 2, 3]
 *   allowedSwaps = [[0,4], [4,2], [1,3], [1,4]]
 *
 *   图:
 *       0
 *       |
 *   1 - 4 - 2
 *   |
 *   3
 *
 *   所有节点在同一连通分量 {0,1,2,3,4}
 *   source 值池: {5,1,2,4,3} = {1,2,3,4,5}
 *   target 值池: {1,5,4,2,3} = {1,2,3,4,5}
 *   完全匹配 → 汉明距离 = 0
 */

// ============================================================
// 解法1: Union-Find + 哈希计数 — 面试首选
// 时间: O(n·α(n)) ≈ O(n)   空间: O(n)
//
// 【思路】
// 1. 交换操作的传递性 → 连通分量内元素可任意排列
//    为什么？同一连通分量内的任意两个位置之间存在交换路径，
//    通过多次"冒泡"可以实现任意排列（和冒泡排序原理一样）。
//
// 2. 用并查集 O(α(n)) 合并所有 allowedSwap 对，找出连通分量。
//
// 3. 对每个连通分量，统计 source 值的频率（多重集），
//    然后用 target 值去"消减"。能消减的次数就是匹配数。
//    未匹配的位置贡献汉明距离。
//
// 4. 答案 = n - 总匹配数
// ============================================================
class Solution1 {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        // 路径压缩：让 x 直接指向根，下次查找 O(1)
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        // 按秩合并：矮树接到高树下面，保持平衡
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
    }
    
    int minimumHammingDistance(vector<int>& source, vector<int>& target,
                               vector<vector<int>>& allowedSwaps) {
        int n = source.size();
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // 第一步：合并所有允许交换的下标对
        for (auto& sw : allowedSwaps) {
            unite(sw[0], sw[1]);
        }
        
        // 第二步：按连通分量分组，统计每个分量中 source 值的出现次数
        // key = 分量代表(root), value = {值 → 出现次数}
        unordered_map<int, unordered_map<int, int>> groupCount;
        for (int i = 0; i < n; i++) {
            // 用 find(i) 而非 i 做 key——这是同一分量节点的共同标识
            groupCount[find(i)][source[i]]++;
        }
        
        // 第三步：遍历 target，在对应分量的 source 值池中尝试匹配
        int match = 0;
        for (int i = 0; i < n; i++) {
            int root = find(i);
            if (groupCount[root].count(target[i]) && 
                groupCount[root][target[i]] > 0) {
                // 成功匹配一个：source 值池消减一个 target[i]
                groupCount[root][target[i]]--;
                match++;
            }
        }
        
        // 汉明距离 = 总位置数 - 成功匹配数
        return n - match;
    }
};


// ============================================================
// 解法2: DFS + 哈希计数 — 图遍历方式
// 时间: O(n + E)   空间: O(n + E)
//
// 【思路】
// 和解法1的核心思路完全一样：找连通分量 → 分量内匹配。
// 区别在于找连通分量的方式：
//   - 解法1用并查集，不需要建邻接表
//   - 解法2建邻接表，用 DFS 遍历找连通分量
//
// DFS 的好处是更直观——遍历过程中直接收集同一分量的下标，
// 然后立即进行匹配计算，不需要全局的 groupCount 映射。
// ============================================================
class Solution2 {
public:
    int minimumHammingDistance(vector<int>& source, vector<int>& target,
                               vector<vector<int>>& allowedSwaps) {
        int n = source.size();
        
        // 第一步：建无向邻接表
        vector<vector<int>> adj(n);
        for (auto& sw : allowedSwaps) {
            adj[sw[0]].push_back(sw[1]);
            adj[sw[1]].push_back(sw[0]); // 双向！交换是双向操作
        }
        
        // 第二步：DFS 找连通分量，逐个分量计算汉明贡献
        vector<bool> visited(n, false);
        int hamming = 0;
        
        for (int i = 0; i < n; i++) {
            if (visited[i]) continue; // 已处理过的分量跳过
            
            // DFS（用迭代栈避免递归栈溢出）收集当前连通分量的所有下标
            vector<int> component;
            stack<int> stk;
            stk.push(i);
            visited[i] = true;
            
            while (!stk.empty()) {
                int node = stk.top(); stk.pop();
                component.push_back(node);
                for (int nei : adj[node]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        stk.push(nei);
                    }
                }
            }
            
            // 第三步：统计该分量中 source 值的频率
            unordered_map<int, int> srcCount;
            for (int idx : component) {
                srcCount[source[idx]]++;
            }
            
            // 第四步：用 target 值消减 source 值池
            // 消减失败的次数就是该分量的汉明贡献
            int unmatched = 0;
            for (int idx : component) {
                if (srcCount.count(target[idx]) && srcCount[target[idx]] > 0) {
                    srcCount[target[idx]]--;
                    // 匹配成功，不贡献汉明距离
                } else {
                    unmatched++; // 分量内没有这个值可用
                }
            }
            hamming += unmatched;
        }
        
        return hamming;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度       | Union-Find (解法1)      | DFS (解法2)             |
// |-----------|------------------------|------------------------|
// | 时间       | O(n·α(n)) ≈ O(n)       | O(n + E)               |
// | 空间       | O(n)                   | O(n + E) 需要邻接表      |
// | 代码复杂度  | 需要并查集模板            | 标准 DFS，无需额外模板    |
// | 面试推荐   | ⭐ 更简洁，展示数据结构功底 | 也很好，更直观            |
//
// 两种方法核心思路完全一样，只是"找连通分量"这一步的实现不同。
// 并查集不需要显式建图，空间更优；DFS 每个分量独立处理，逻辑更清晰。
//
// ============================================================
// 【易错点】
//
// 1. 用 set 而非 map 统计 source 值：
//    ✗ unordered_set<int> srcVals; 
//      → 丢失重复值信息, source={1,1,2} target={1,2,2} 会误判全匹配
//    ✓ unordered_map<int,int> srcCount;
//      → 正确记录每个值出现几次
//
// 2. 并查集没有路径压缩：
//    ✗ int find(int x) { return parent[x]==x ? x : find(parent[x]); }
//      → find 可能退化为 O(n)，总体 O(n²)
//    ✓ int find(int x) { return parent[x]==x ? x : parent[x]=find(parent[x]); }
//      → 路径压缩后几乎 O(1)
//
// 3. 分组时 key 用 i 而非 find(i)：
//    ✗ groupCount[i][source[i]]++;    → 每个节点独立一组
//    ✓ groupCount[find(i)][source[i]]++; → 同一分量的节点聚在一起
//
// 4. DFS 建图忘记双向边：
//    ✗ adj[a].push_back(b);           → 只有单向，分量会断裂
//    ✓ adj[a].push_back(b); adj[b].push_back(a); → 交换是双向的
//
// 5. 匹配计数时忘记检查 count > 0：
//    ✗ srcCount[target[idx]]--;       → 可能减到负数，多算匹配
//    ✓ if (srcCount[target[idx]] > 0) srcCount[target[idx]]--;
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: 为什么同一连通分量内的元素可以任意排列？
//   → 连通图上的相邻对换可以生成所有置换（群论：相邻对换生成对称群）。
//     直觉理解：冒泡排序只用相邻交换就能排出任何排列。
//
// Q2: 并查集和 DFS 找连通分量，什么时候选哪个？
//   → 如果边是"在线"加入的（动态连通性），并查集更合适。
//     如果需要遍历分量内所有节点做处理，DFS 更自然。
//     本题两者都可以，并查集代码更短。
//
// Q3: 如果 allowedSwaps 是动态的（可以不断加入新的交换对），
//     如何实时维护最小汉明距离？
//   → 并查集天然支持动态合并。每次合并两个分量时，
//     合并它们的 hashmap，重新计算匹配数，更新汉明距离。
//     时间复杂度取决于 hashmap 合并，可以用"小集合合并到大集合"
//     的启发式策略，总 O(n log n)。
//
// Q4: 如果值域很小（比如只有 1-26），有什么优化？
//   → 用长度 26 的数组代替 hashmap，常数更小。
//     匹配计算可以直接比较两个计数数组。
// ============================================================
