/*
 * LeetCode 1202: Smallest String With Swaps
 *
 * 【题目本质】
 * 给定可交换的索引对，同一连通分量内的位置可任意重排。
 * 对每个连通分量内的字符排序，最小字符放最小位置，即得字典序最小结果。
 *
 * 【解法总览】
 * 解法1: DFS 找连通分量 — O(n log n + m) / O(n + m) — 建图遍历，直观
 * 解法2: 并查集 (Union-Find) — O(n log n + m·α(n)) / O(n) — 面试首选，代码简洁
 */

// ============================================================
// 解法1: DFS 找连通分量 + 组内排序
// 时间: O(n log n + m)  空间: O(n + m)
//
// 【思路】
// 把索引看作节点，pairs 看作无向边，建邻接表。
// 用 DFS 找到每个连通分量，收集该分量内的所有索引和字符。
// 组内字符排序后，按位置从小到大填回，就是字典序最小。
//
// 为什么？因为连通分量内的位置可以任意重排（传递性），
// 要字典序最小，显然最小字符放最小位置。
//
// 示例: s = "dcab", pairs = [[0,3],[1,2],[0,2]]
//
//   邻接表:
//   0: [3, 2]
//   1: [2]
//   2: [1, 0]
//   3: [0]
//
//   DFS从0出发: 0→3→2→1, 连通分量 = {0,1,2,3}
//   字符: d,c,a,b → 排序 → a,b,c,d
//   索引: 0,1,2,3 → 排序 → 0,1,2,3
//   填回: result = "abcd"
// ============================================================
class Solution1 {
public:
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        int n = s.size();
        
        // 建邻接表
        vector<vector<int>> adj(n);
        for (auto& p : pairs) {
            adj[p[0]].push_back(p[1]);
            adj[p[1]].push_back(p[0]);
        }
        
        vector<bool> visited(n, false);
        string result = s;
        
        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            
            // 用栈实现 DFS（避免递归栈溢出，n 可达 10^5）
            vector<int> indices;
            stack<int> stk;
            stk.push(i);
            visited[i] = true;
            
            while (!stk.empty()) {
                int node = stk.top(); stk.pop();
                indices.push_back(node);
                for (int nei : adj[node]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        stk.push(nei);
                    }
                }
            }
            
            // 收集字符
            vector<char> chars;
            for (int idx : indices) chars.push_back(s[idx]);
            
            // 关键：索引和字符都排序，最小字符填最小位置
            sort(indices.begin(), indices.end());
            sort(chars.begin(), chars.end());
            
            for (int k = 0; k < (int)indices.size(); k++) {
                result[indices[k]] = chars[k];
            }
        }
        
        return result;
    }
};

// ============================================================
// 解法2: 并查集 (Union-Find) — 面试首选
// 时间: O(n log n + m·α(n))  空间: O(n)
//
// 【思路】
// 并查集天然解决"合并+查询连通性"问题，无需建邻接表。
// 
// 步骤:
// 1. 初始化：每个索引是自己的根
// 2. 合并：遍历 pairs，union(a, b)
// 3. 分组：遍历所有索引，按 find(i) 的根分组
// 4. 组内排序填回
//
// 并查集过程（示例3: s="cba", pairs=[[0,1],[1,2]]）:
//
//   初始:     [0] [1] [2]     每个是独立集合
//
//   union(0,1):
//              0               0 是根
//             /
//            1       [2]
//
//   union(1,2): find(1)=0, find(2)=2 → union(0,2)
//              0               全部连通
//             / \
//            1   2
//
//   分组: root=0 → {0,1,2}
//   字符: c,b,a → 排序 → a,b,c
//   填回: result = "abc"
// ============================================================
class Solution2 {
public:
    vector<int> parent, rank_;
    
    // 路径压缩：让 x 直接指向根，后续查询接近 O(1)
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    // 按秩合并：矮树挂到高树下，避免退化成链
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;  // 已在同一集合
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
    }
    
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        int n = s.size();
        parent.resize(n);
        rank_.resize(n, 0);
        // 初始化：每个节点是自己的根（不能全部初始化为0！）
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // 合并所有 pair 对应的索引
        for (auto& p : pairs) {
            unite(p[0], p[1]);
        }
        
        // 按根节点分组
        unordered_map<int, vector<int>> groups;
        for (int i = 0; i < n; i++) {
            groups[find(i)].push_back(i);
        }
        
        // 每组内：收集字符 → 排序 → 最小字符放最小位置
        string result = s;
        for (auto& [root, indices] : groups) {
            // indices 是按 i=0..n-1 顺序加入的，所以天然有序
            // 但显式排序更安全（防止 unordered_map 实现差异）
            sort(indices.begin(), indices.end());
            
            vector<char> chars;
            for (int idx : indices) {
                chars.push_back(s[idx]);
            }
            sort(chars.begin(), chars.end());
            
            for (int k = 0; k < (int)indices.size(); k++) {
                result[indices[k]] = chars[k];
            }
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// DFS vs 并查集:
// - DFS: 需要建邻接表 O(m) 额外空间，代码略长
// - 并查集: 只需 O(n) 的 parent 数组，代码更简洁
// - 两者时间复杂度本质相同，瓶颈都在排序 O(n log n)
// - 面试优先写并查集：体现对 Union-Find 的掌握
//
// 【易错点】
//
// 1. parent 初始化:
//    ✗ parent.resize(n, 0);  — 全部指向0，一开始所有节点就在同一集合
//    ✓ for(int i=0;i<n;i++) parent[i]=i;  — 每个节点初始是自己的根
//
// 2. find 没有路径压缩:
//    ✗ int find(int x) { while(parent[x]!=x) x=parent[x]; return x; }
//      最坏 O(n) 每次，总体 O(n*m) 可能 TLE
//    ✓ if(parent[x]!=x) parent[x]=find(parent[x]); return parent[x];
//      路径压缩后近 O(1)
//
// 3. 分组后只排序字符不排序索引:
//    ✗ 把排序后的字符按 DFS 发现顺序填回（DFS 顺序不是从小到大）
//    ✓ 索引和字符都排序，一一配对：最小字符 → 最小索引
//
// 4. 递归 DFS 栈溢出:
//    ✗ 对 n=10^5 的图直接递归，可能爆栈
//    ✓ 用显式栈的迭代 DFS，或用并查集避免遍历
//
// 【面试追问】
//
// Q1(基础理解): 为什么连通分量内的字符可以任意重排？
// → 类比冒泡排序：任意两个相邻位置可交换 = 可以排成任意排列。
//   连通分量保证任意两个位置之间有交换路径，字符可以"传递"到任意位置。
//
// Q2(优化): DFS 和并查集哪个更好？各自的优缺点？
// → 并查集更轻量（无需邻接表），代码更短，且天然支持增量式合并。
//   DFS 更直观，适合需要遍历路径的场景。
//   时间复杂度本质相同，瓶颈在排序 O(n log n)。
//
// Q3(变体): 如果 pairs 是动态增加的，每加一条边就要返回当前最小字符串？
// → 并查集天然支持增量合并。维护每组字符的有序结构（如 multiset），
//   合并两组时用归并合并两个有序序列 O(k)，避免全量重排。
//   总体 O(n log n) 预处理 + O(k) 每次合并（k 为较小组大小）。
// ============================================================
