// LeetCode 742. Closest Leaf in a Binary Tree
// 解法一：树转无向图 + BFS（推荐）
// Time: O(n), Space: O(n)

class Solution {
public:
    int findClosestLeaf(TreeNode* root, int k) {
        // 邻接表：节点值 -> 邻居节点值列表
        unordered_map<int, vector<int>> graph;
        // 叶节点集合
        unordered_set<int> leaves;

        // 第一步：DFS 建无向图，标记叶节点
        buildGraph(root, nullptr, graph, leaves);

        // 第二步：BFS 从目标节点 k 出发
        queue<int> q;
        unordered_set<int> visited;
        q.push(k);
        visited.insert(k);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            // 如果当前节点是叶节点，直接返回
            if (leaves.count(node)) {
                return node;
            }

            // 扩展所有邻居
            for (int neighbor : graph[node]) {
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }

        return -1; // 不会到达此处
    }

private:
    void buildGraph(TreeNode* node, TreeNode* parent,
                    unordered_map<int, vector<int>>& graph,
                    unordered_set<int>& leaves) {
        if (!node) return;

        // 标记叶节点
        if (!node->left && !node->right) {
            leaves.insert(node->val);
        }

        // 与父节点建双向边
        if (parent) {
            graph[node->val].push_back(parent->val);
            graph[parent->val].push_back(node->val);
        }

        // 递归处理左右子树
        buildGraph(node->left, node, graph, leaves);
        buildGraph(node->right, node, graph, leaves);
    }
};


// 解法二：Parent 指针 + BFS
// Time: O(n), Space: O(n)
// 不显式建图，只记录 parent 关系

class Solution2 {
public:
    int findClosestLeaf(TreeNode* root, int k) {
        // 记录每个节点的父节点
        unordered_map<TreeNode*, TreeNode*> parentMap;
        // 找到目标节点
        TreeNode* target = nullptr;

        // DFS 建立 parent 映射并找目标节点
        function<void(TreeNode*, TreeNode*)> buildParent =
            [&](TreeNode* node, TreeNode* parent) {
            if (!node) return;
            parentMap[node] = parent;
            if (node->val == k) target = node;
            buildParent(node->left, node);
            buildParent(node->right, node);
        };
        buildParent(root, nullptr);

        // BFS 从目标节点出发
        queue<TreeNode*> q;
        unordered_set<TreeNode*> visited;
        q.push(target);
        visited.insert(target);

        while (!q.empty()) {
            TreeNode* cur = q.front();
            q.pop();

            // 判断是否为叶节点
            if (!cur->left && !cur->right) {
                return cur->val;
            }

            // 三个方向：左子、右子、父节点
            TreeNode* neighbors[] = {cur->left, cur->right, parentMap[cur]};
            for (TreeNode* next : neighbors) {
                if (next && !visited.count(next)) {
                    visited.insert(next);
                    q.push(next);
                }
            }
        }

        return -1;
    }
};


// 解法三：纯 DFS 注解距离（不建图）
// Time: O(n), Space: O(n)
// 在 DFS 过程中计算目标节点到各叶节点的距离

class Solution3 {
public:
    int closestLeaf = -1;
    int minDist = INT_MAX;

    int findClosestLeaf(TreeNode* root, int k) {
        dfs(root, k);
        return closestLeaf;
    }

private:
    // 返回值：如果子树包含目标 k，返回 k 到当前节点的距离；否则返回 -1
    int dfs(TreeNode* node, int k) {
        if (!node) return -1;

        if (node->val == k) {
            // 找到目标节点，搜索其子树中最近的叶节点
            findClosestInSubtree(node, 0);
            return 0; // 距离为 0（目标节点就是当前节点）
        }

        // 在左子树中找目标
        int leftDist = dfs(node->left, k);
        if (leftDist >= 0) {
            int distFromK = leftDist + 1; // k 到当前节点的距离
            // 检查当前节点是否是叶节点
            if (!node->left && !node->right) {
                if (distFromK < minDist) {
                    minDist = distFromK;
                    closestLeaf = node->val;
                }
            }
            // 搜索右子树中是否有更近的叶节点
            findClosestInSubtree(node->right, distFromK + 1);
            return distFromK;
        }

        // 在右子树中找目标
        int rightDist = dfs(node->right, k);
        if (rightDist >= 0) {
            int distFromK = rightDist + 1;
            if (!node->left && !node->right) {
                if (distFromK < minDist) {
                    minDist = distFromK;
                    closestLeaf = node->val;
                }
            }
            // 搜索左子树中是否有更近的叶节点
            findClosestInSubtree(node->left, distFromK + 1);
            return distFromK;
        }

        return -1; // 目标不在此子树中
    }

    // 在子树中找距离 dist 最近的叶节点
    void findClosestInSubtree(TreeNode* node, int dist) {
        if (!node) return;
        if (!node->left && !node->right) {
            // 叶节点
            if (dist < minDist) {
                minDist = dist;
                closestLeaf = node->val;
            }
            return;
        }
        findClosestInSubtree(node->left, dist + 1);
        findClosestInSubtree(node->right, dist + 1);
    }
};
