/*
 * LeetCode 2096: 从二叉树一个节点到另一个节点每一步的方向
 *
 * 【题目本质】
 * 树中两点之间的路径唯一，一定经过 LCA。
 * 从 start 到 LCA 全部是 'U'，从 LCA 到 dest 保持 L/R 方向。
 *
 * 【解法总览】
 * 解法1: LCA + 两次DFS        — O(n) / O(n) — 经典面试写法
 * 解法2: 根路径 + 去公共前缀    — O(n) / O(n) — 代码最简洁
 * 解法3: BFS建无向图            — O(n) / O(n) — 最直觉但空间较大
 */

// ============================================================
// 解法1: LCA + 两次 DFS — 经典思路
// 时间: O(n)  空间: O(n) (递归栈 + 路径字符串)
//
// 【思路】
// 树中两点路径一定经过 LCA。
// Step1: 用后序遍历找 LCA
// Step2: 从 LCA 出发 DFS 找到 start 的路径 → 长度即 'U' 的数量
// Step3: 从 LCA 出发 DFS 找到 dest 的路径 → 保持 L/R
// 拼接: "UUU..." + pathToDest
//
// 树结构示意 (示例1):
//         5          ← LCA (start=3, dest=6)
//        / \
//       1   2
//      /   / \
//     3   6   4
//
// LCA→start 路径: "LL" (5→1→3), 长度=2 → "UU"
// LCA→dest 路径:  "RL" (5→2→6)
// 结果: "UU" + "RL" = "UURL"
// ============================================================
class Solution1 {
public:
    // 经典 LCA: 后序遍历
    // 当 left 和 right 都非空时，说明 s/d 分居两侧，当前节点就是 LCA
    TreeNode* findLCA(TreeNode* node, int s, int d) {
        if (!node || node->val == s || node->val == d) return node;
        TreeNode* left = findLCA(node->left, s, d);
        TreeNode* right = findLCA(node->right, s, d);
        if (left && right) return node;
        return left ? left : right;
    }
    
    // DFS + 回溯 找从 node 到 target 的路径
    // 回溯过程示意 (找3):
    //   5 → push 'L' → 1 → push 'L' → 3 (找到! path="LL")
    //   如果走错: push → 递归失败 → pop_back 撤销
    bool findPath(TreeNode* node, int target, string& path) {
        if (!node) return false;
        if (node->val == target) return true;
        
        // 尝试左子树
        path.push_back('L');
        if (findPath(node->left, target, path)) return true;
        path.pop_back();  // 回溯：左边没找到，撤销 'L'
        
        // 尝试右子树
        path.push_back('R');
        if (findPath(node->right, target, path)) return true;
        path.pop_back();  // 回溯：右边也没找到，撤销 'R'
        
        return false;
    }
    
    string getDirections(TreeNode* root, int startValue, int destValue) {
        TreeNode* lca = findLCA(root, startValue, destValue);
        
        string pathToStart, pathToDest;
        findPath(lca, startValue, pathToStart);
        findPath(lca, destValue, pathToDest);
        
        // start→LCA 全是向上走
        string ups(pathToStart.size(), 'U');
        return ups + pathToDest;
    }
};

// ============================================================
// 解法2: 两条根路径 + 去公共前缀 — 代码最简洁
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 不显式求 LCA，而是利用一个巧妙观察：
// 从根到 start 和从根到 dest 的两条路径，它们的公共前缀
// 就是"根到 LCA"的路径。去掉公共前缀后：
//   - start 的剩余路径长度 = 需要走的 'U' 数
//   - dest 的剩余路径 = 需要走的 L/R 方向
//
// 示意:
//   root→start: "L L"      root→dest: "R L"
//   公共前缀:   "" (长度0, 第一个字符就不同)
//   start剩余长度=2 → "UU"
//   dest剩余="RL"
//   结果 = "UU" + "RL" = "UURL"
//
// 另一个例子:
//   root→start: "L L R"    root→dest: "L L L"
//   公共前缀:   "L L" (长度2)
//   start剩余长度=1 → "U"
//   dest剩余="L"
//   结果 = "U" + "L" = "UL"
// ============================================================
class Solution2 {
public:
    bool findPath(TreeNode* node, int target, string& path) {
        if (!node) return false;
        if (node->val == target) return true;
        
        path.push_back('L');
        if (findPath(node->left, target, path)) return true;
        path.pop_back();
        
        path.push_back('R');
        if (findPath(node->right, target, path)) return true;
        path.pop_back();
        
        return false;
    }
    
    string getDirections(TreeNode* root, int startValue, int destValue) {
        string pathToStart, pathToDest;
        findPath(root, startValue, pathToStart);
        findPath(root, destValue, pathToDest);
        
        // 去公共前缀 — 公共前缀的终点就是 LCA
        int i = 0;
        while (i < (int)pathToStart.size() && i < (int)pathToDest.size()
               && pathToStart[i] == pathToDest[i]) {
            i++;
        }
        
        // start 到 LCA: 剩余长度个 'U'
        // LCA 到 dest: 剩余的 L/R 方向
        string ups(pathToStart.size() - i, 'U');
        return ups + pathToDest.substr(i);
    }
};

// ============================================================
// 解法3: BFS 建无向图 — 最直觉
// 时间: O(n)  空间: O(n) (邻接表 + 队列)
//
// 【思路】
// 把二叉树当成无向图。每条边记录方向 (L/R/U)。
// 从 start 出发 BFS，第一次到达 dest 时的路径就是最短的。
//
// 建图示意 (示例1):
//   5 ←→ 1 (5到1是'L', 1到5是'U')
//   5 ←→ 2 (5到2是'R', 2到5是'U')
//   1 ←→ 3 (1到3是'L', 3到1是'U')
//   2 ←→ 6 (2到6是'L', 6到2是'U')
//   2 ←→ 4 (2到4是'R', 4到2是'U')
//
// BFS 从3出发:
//   3 → 1('U') → 5('U') → 2('R') → 6('L')
//   路径: "UURL"
//
// 注意: 队列中存字符串会导致大量复制，实际更好的做法是
// 用 parent 数组 + direction 数组，BFS后回溯路径。
// 这里为了展示思路用了直接存字符串的简单写法。
// ============================================================
class Solution3 {
public:
    string getDirections(TreeNode* root, int startValue, int destValue) {
        // 建邻接表
        unordered_map<int, vector<pair<int, char>>> graph;
        
        // 用 BFS/DFS 遍历树来建图
        queue<TreeNode*> buildQ;
        buildQ.push(root);
        while (!buildQ.empty()) {
            TreeNode* node = buildQ.front(); buildQ.pop();
            if (node->left) {
                graph[node->val].push_back({node->left->val, 'L'});
                graph[node->left->val].push_back({node->val, 'U'});
                buildQ.push(node->left);
            }
            if (node->right) {
                graph[node->val].push_back({node->right->val, 'R'});
                graph[node->right->val].push_back({node->val, 'U'});
                buildQ.push(node->right);
            }
        }
        
        // BFS 从 start 到 dest
        // 用 parent + direction 回溯（避免字符串复制的优化版本）
        unordered_map<int, int> parent;
        unordered_map<int, char> direction;  // direction[v] = 从parent[v]走到v的方向
        unordered_set<int> visited;
        queue<int> bfs;
        
        bfs.push(startValue);
        visited.insert(startValue);
        parent[startValue] = -1;
        
        while (!bfs.empty()) {
            int curr = bfs.front(); bfs.pop();
            if (curr == destValue) break;
            for (auto& [next, dir] : graph[curr]) {
                if (!visited.count(next)) {
                    visited.insert(next);
                    parent[next] = curr;
                    direction[next] = dir;
                    bfs.push(next);
                }
            }
        }
        
        // 从 dest 回溯到 start，构建路径
        string result;
        int curr = destValue;
        while (curr != startValue) {
            result.push_back(direction[curr]);
            curr = parent[curr];
        }
        
        // 回溯得到的是反序
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (LCA + DFS):
//   - 优点: 思路清晰，展示对 LCA 的理解
//   - 缺点: 需要写 LCA + findPath 两个辅助函数
//   - 面试推荐度: ⭐ 高（体现算法功底）
//
// 解法2 (根路径去前缀):
//   - 优点: 代码最短，不需要显式 LCA
//   - 缺点: 需要理解"公共前缀 = LCA位置"这个洞察
//   - 面试推荐度: ⭐ 高（简洁高效）
//
// 解法3 (BFS建图):
//   - 优点: 思路最直觉，"两点最短路径 → BFS"
//   - 缺点: 空间大（建邻接表），代码长
//   - 面试推荐度: 作为第一想法提出，然后优化到解法1或2
//
// 【易错点】
// 1. findPath 中忘记 pop_back() 回溯:
//    ✗ path.push_back('L'); findPath(node->left,...); // 走不通没撤销
//    ✓ path.push_back('L'); if(!findPath(...)) path.pop_back();
//
// 2. 公共前缀循环中 i 越界:
//    ✗ while (pathToStart[i] == pathToDest[i]) i++;
//    ✓ while (i < pathToStart.size() && i < pathToDest.size() && ...)
//
// 3. start→LCA 的方向搞混:
//    ✗ 保留 findPath 返回的 L/R (那是 LCA→start 的方向)
//    ✓ start→LCA 全是 'U'，长度 = pathToStart 去掉公共前缀后的长度
//
// 4. LCA 函数漏掉 node->val==s 或 node->val==d 的提前返回:
//    ✗ if (!node) return nullptr; // 只检查空节点
//    ✓ if (!node || node->val==s || node->val==d) return node;
//    如果 s 是 d 的祖先，不提前返回就会错过 LCA
//
// 【面试追问】
// Q1: 暴力方法是什么？→ BFS建图，O(n)时间O(n)空间，概念简单但实现较重
// Q2: 能否不显式求 LCA？→ 解法2，利用两条根路径的公共前缀
// Q3: 如果多次查询 (start, dest)？→ 预处理 Euler Tour + Sparse Table
//     实现 O(1) LCA 查询，每个节点到根的路径预存
// Q4: 如果树极深(~10^5)，递归栈溢出怎么办？→ 迭代DFS(手动栈)找路径
// ============================================================
