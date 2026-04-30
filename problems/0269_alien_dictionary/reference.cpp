/*
 * LeetCode 269: Alien Dictionary (火星词典)
 * 
 * 【题目本质】
 * 从已排序的外星语单词列表中，逆向推导字母的顺序。
 * 本质：从偏序关系构建全序 → 拓扑排序。
 *
 * 【解法总览】
 * 解法1: BFS 拓扑排序 (Kahn 算法) — O(C) / O(U+E) — 面试首选 ⭐
 * 解法2: DFS 拓扑排序 (后序反转)  — O(C) / O(U+E) — 递归思维
 * 
 * C = 所有单词总字符数, U = 不同字母数(≤26), E = 边数(≤U²)
 */

// ============================================================
// 解法1: BFS 拓扑排序 (Kahn 算法)
// 时间: O(C)  空间: O(U + E)
//
// 【思路】
// 1. 比较每对相邻单词，找第一个不同字符 → 有向边
//    为什么只看相邻的？因为传递关系由拓扑排序自动处理
//    为什么只看第一个不同字符？因为字典序比较就到此为止
//
// 2. 用 Kahn 算法做拓扑排序：
//    - 维护入度表，从入度为 0 的节点开始
//    - 每处理一个节点，减少其后继的入度
//    - 最终如果处理的节点数 < 总数 → 有环 → 无解
//
// 【建图过程图解】
// words = ["wrt","wrf","er","ett","rftt"]
//
// "wrt" vs "wrf" → 前缀 "wr" 相同, t≠f → 边 t→f
// "wrf" vs "er"  → 第一个字符不同, w≠e → 边 w→e
// "er"  vs "ett" → 前缀 "e" 相同, r≠t  → 边 r→t
// "ett" vs "rftt"→ 第一个字符不同, e≠r → 边 e→r
//
// 有向图:  w → e → r → t → f
//
// 入度:  w:0  e:1  r:1  t:1  f:1
//
// BFS 过程:
// 队列: [w] → 弹出 w, e 入度→0 → 队列: [e]
// 队列: [e] → 弹出 e, r 入度→0 → 队列: [r]
// 队列: [r] → 弹出 r, t 入度→0 → 队列: [t]
// 队列: [t] → 弹出 t, f 入度→0 → 队列: [f]
// 队列: [f] → 弹出 f, 无后继
//
// 结果: "wertf", 5个字母全部处理 → 合法!
// ============================================================
class Solution1 {
public:
    string alienOrder(vector<string>& words) {
        // Step 1: 收集所有出现的字母并初始化入度为 0
        // 为什么要这步？因为有些字母可能不参与任何比较，
        // 但它们确实存在于字典中，结果必须包含它们
        unordered_map<char, unordered_set<char>> graph;
        unordered_map<char, int> indegree;
        
        for (const string& w : words) {
            for (char c : w) {
                if (indegree.find(c) == indegree.end()) {
                    indegree[c] = 0;
                }
            }
        }
        
        // Step 2: 建图 — 比较每对相邻单词
        for (int i = 0; i < (int)words.size() - 1; i++) {
            const string& w1 = words[i];
            const string& w2 = words[i + 1];
            
            // 特判非法前缀情况: ["abc", "ab"] 不合法
            // 因为字典序中短的前缀一定排在长的前面
            if (w1.size() > w2.size() && w1.substr(0, w2.size()) == w2) {
                return "";
            }
            
            int minLen = min(w1.size(), w2.size());
            for (int j = 0; j < minLen; j++) {
                if (w1[j] != w2[j]) {
                    // 第一个不同字符：w1[j] 排在 w2[j] 前面
                    // 用 set 去重防止同一条边被加多次导致入度多算
                    if (graph[w1[j]].find(w2[j]) == graph[w1[j]].end()) {
                        graph[w1[j]].insert(w2[j]);
                        indegree[w2[j]]++;
                    }
                    // 关键！只有第一个不同字符有信息量
                    // 后续字符的顺序无法从这对单词推导
                    break;
                }
            }
        }
        
        // Step 3: BFS (Kahn 算法) — 不断取出入度为 0 的节点
        queue<char> q;
        for (auto& [ch, deg] : indegree) {
            if (deg == 0) {
                q.push(ch);
            }
        }
        
        string result;
        while (!q.empty()) {
            char curr = q.front();
            q.pop();
            result += curr;
            
            // 删除 curr 的所有出边，减少后继的入度
            for (char next : graph[curr]) {
                indegree[next]--;
                if (indegree[next] == 0) {
                    q.push(next);
                }
            }
        }
        
        // Step 4: 环检测
        // 如果有环，环上的节点入度永远不会变为 0，不会被处理
        // 因此处理的节点数 < 总节点数 → 有环 → 无解
        if (result.size() != indegree.size()) {
            return "";
        }
        
        return result;
    }
};

// ============================================================
// 解法2: DFS 拓扑排序 (后序反转 + 三色标记)
// 时间: O(C)  空间: O(U + E)
//
// 【思路】
// DFS 拓扑排序的核心：
// - 对一个节点做 DFS，先递归处理所有后继
// - 后继全部处理完后，把自己加入结果（后序）
// - 最终反转结果就是拓扑序
//
// 为什么后序反转是拓扑序？
// - 后序意味着：一个节点在其所有后继之后被记录
// - 反转后：一个节点在其所有后继之前 → 拓扑序的定义！
//
// 三色标记检测环：
// - 白色(0): 未访问
// - 灰色(1): 正在递归栈中（正在处理其子树）
// - 黑色(2): 已完成
// - 如果 DFS 中遇到灰色节点 → 存在从它到它自己的路径 → 有环！
//
// 【DFS 过程图解】
// 图:  w → e → r → t → f
//
// 从 w 开始 DFS:
//   dfs(w): 灰色
//     dfs(e): 灰色
//       dfs(r): 灰色
//         dfs(t): 灰色
//           dfs(f): 灰色 → 无后继 → 黑色, 记录 f
//         t: 后继完成 → 黑色, 记录 t
//       r: 后继完成 → 黑色, 记录 r
//     e: 后继完成 → 黑色, 记录 e
//   w: 后继完成 → 黑色, 记录 w
//
// 后序记录: [f, t, r, e, w]
// 反转: [w, e, r, t, f] → "wertf" ✓
// ============================================================
class Solution2 {
public:
    string alienOrder(vector<string>& words) {
        // Step 1: 建图（与 BFS 版完全相同）
        unordered_map<char, unordered_set<char>> graph;
        unordered_set<char> allChars;
        
        for (const string& w : words) {
            for (char c : w) {
                allChars.insert(c);
            }
        }
        
        for (int i = 0; i < (int)words.size() - 1; i++) {
            const string& w1 = words[i];
            const string& w2 = words[i + 1];
            
            if (w1.size() > w2.size() && w1.substr(0, w2.size()) == w2) {
                return "";
            }
            
            int minLen = min(w1.size(), w2.size());
            for (int j = 0; j < minLen; j++) {
                if (w1[j] != w2[j]) {
                    graph[w1[j]].insert(w2[j]);
                    break;
                }
            }
        }
        
        // Step 2: DFS 拓扑排序
        // 三色标记: 0=白(未访问), 1=灰(在递归栈中), 2=黑(完成)
        unordered_map<char, int> color;
        for (char c : allChars) color[c] = 0;
        
        string result;
        bool hasCycle = false;
        
        function<void(char)> dfs = [&](char u) {
            if (hasCycle) return; // 剪枝：已经发现环，无需继续
            
            color[u] = 1; // 标记灰色：进入递归栈
            
            for (char v : graph[u]) {
                if (color[v] == 1) {
                    // 遇到灰色节点 → v 在当前递归栈中 → 有环
                    hasCycle = true;
                    return;
                }
                if (color[v] == 0) {
                    dfs(v);
                    if (hasCycle) return;
                }
                // color[v] == 2: 已经处理完，跳过
            }
            
            color[u] = 2; // 标记黑色：处理完成
            // 后序位置：所有后继都已处理完，才记录自己
            result += u;
        };
        
        // 对所有未访问的节点发起 DFS
        for (char c : allChars) {
            if (color[c] == 0) {
                dfs(c);
                if (hasCycle) return "";
            }
        }
        
        // 后序的反转 = 拓扑序
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// BFS (Kahn) vs DFS (后序反转):
//
// | 维度         | BFS (Kahn)              | DFS 后序反转          |
// |-------------|--------------------------|----------------------|
// | 核心操作     | 入度表 + 队列            | 三色标记 + 递归栈     |
// | 环检测方式   | 结果长度 < 总节点数      | 遇到灰色节点          |
// | 直觉性       | ⭐ 更直观                | 需理解后序反转的含义  |
// | 字典序最小   | 换 priority_queue 即可   | 不容易控制            |
// | 面试推荐     | ⭐ 首选                  | 备选                  |
//
// 时间空间完全相同: O(C) / O(U+E)
//
// ============================================================
// 【易错点】
//
// 1. 忘记 break:
//    ✗ 找到不同字符后继续循环，提取出错误的顺序关系
//    ✓ 找到第一个不同字符就 break，后续字符无法提供信息
//
// 2. 漏掉前缀非法判断:
//    ✗ ["abc", "ab"] 时 for 循环正常结束，不加任何边也不报错
//    ✓ 在比较前检查 w1.size() > w2.size() 且 w2 是 w1 的前缀 → return ""
//
// 3. 重复边导致入度多算 (BFS版):
//    ✗ graph 用 vector 存邻接表，"t→f" 如果出现两次则入度多加
//    ✓ 用 unordered_set 存邻接表，自动去重
//
// 4. 遗漏孤立字母:
//    ✗ 只在建边时注册字母 → 没参与任何比较的字母缺失
//    ✓ 初始化时遍历所有单词所有字符，全部加入入度表
//
// 5. DFS 版混淆灰色和黑色:
//    ✗ 只用 visited 布尔值，无法区分"在当前路径上"和"已处理完"
//       → 把跨分支的已处理节点误判为环
//    ✓ 三色标记: 灰=当前路径, 黑=已完成, 只有遇到灰色才是环
//
// ============================================================
// 【面试追问】
//
// Q1: 如何从单词列表提取字母顺序关系？
//     → 比较相邻单词的第一个不同字符，产生有向边。
//       为什么只看相邻的？传递关系由拓扑排序自动处理。
//
// Q2: 什么情况下无解？
//     → (1) 顺序矛盾形成环（拓扑排序不完整）
//       (2) 前缀矛盾（长词排在短前缀前面，如 ["abc","ab"]）
//
// Q3: 如何保证返回字典序最小的合法答案？
//     → BFS 版：把 queue 换成 priority_queue<char, vector<char>, greater<char>>
//       每次从入度为 0 的节点中选最小的字母。时间增加到 O(C + U log U)。
//
// Q4: 这道题和课程表 (207/210) 有什么关系？
//     → 完全相同的拓扑排序框架。区别是课程表直接给边，
//       本题需要自己从单词列表中提取边（建图过程不同，排序过程相同）。
// ============================================================
