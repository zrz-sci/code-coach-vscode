/*
 * 【题目本质】
 * 判断给定排列 nums 是否是 sequences 所确定的唯一拓扑排序。
 * 等价于：sequences 的约束是否足以唯一确定 nums 这个全序。
 *
 * 【解法总览】
 * 解法1: BFS拓扑排序 — O(V+E) / O(V+E) ⭐ 首选
 * 解法2: 相邻对验证 — O(V+E) / O(V)
 */

// ===================== Solution1: BFS拓扑排序 =====================
// 思路：建有向图，BFS 中每步队列恰好 1 个元素 → 拓扑序唯一
// 时间 O(V+E)  空间 O(V+E)
class Solution1 {
public:
    bool sequenceReconstruction(vector<int>& nums, vector<vector<int>>& sequences) {
        int n = nums.size();
        vector<unordered_set<int>> graph(n + 1);
        vector<int> indegree(n + 1, 0);

        // 建图：sequences 中相邻元素形成有向边
        for (auto& seq : sequences) {
            for (int i = 0; i + 1 < (int)seq.size(); i++) {
                if (graph[seq[i]].insert(seq[i + 1]).second) {
                    indegree[seq[i + 1]]++;
                }
            }
        }

        // BFS 拓扑排序
        queue<int> q;
        for (int i = 1; i <= n; i++) {
            if (indegree[i] == 0) q.push(i);
        }

        int idx = 0;
        while (!q.empty()) {
            if (q.size() > 1) return false; // 多个入度0节点 → 不唯一
            int curr = q.front(); q.pop();
            if (idx >= n || nums[idx] != curr) return false; // 与 nums 不匹配
            idx++;
            for (int next : graph[curr]) {
                if (--indegree[next] == 0) q.push(next);
            }
        }
        return idx == n; // 确保所有节点都被处理
    }
};

// ===================== Solution2: 相邻对验证 =====================
// 思路：若 nums 中每对相邻元素 (nums[i], nums[i+1]) 都是某 sequence 的直接相邻对
//       → nums 是唯一合法超序列
// 时间 O(V+E)  空间 O(V)
class Solution2 {
public:
    bool sequenceReconstruction(vector<int>& nums, vector<vector<int>>& sequences) {
        int n = nums.size();
        vector<int> pos(n + 1);
        for (int i = 0; i < n; i++) pos[nums[i]] = i;

        vector<bool> covered(n, false);
        for (auto& seq : sequences) {
            for (int i = 0; i + 1 < (int)seq.size(); i++) {
                // 验证 seq 中的顺序与 nums 一致
                if (pos[seq[i]] >= pos[seq[i + 1]]) return false;
                // 检查是否覆盖了 nums 中的直接相邻对
                if (pos[seq[i]] + 1 == pos[seq[i + 1]]) {
                    covered[pos[seq[i]]] = true;
                }
            }
        }

        // 所有 n-1 个相邻对都必须被至少一个 sequence 覆盖
        for (int i = 0; i + 1 < n; i++) {
            if (!covered[i]) return false;
        }
        return true;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 BFS拓扑排序 | O(V+E) | O(V+E) | 标准拓扑排序，通用性强 | 需要建图 |
 * | Solution2 相邻对验证 | O(V+E) | O(V) | 空间更优，逻辑简洁 | 数学证明不直观 |
 *
 * 【易错点】
 * 1. 建图时必须去重边，否则 indegree 计数错误
 * 2. 不能只检查唯一性，还要验证结果与 nums 匹配
 * 3. 解法2 中 pos[seq[i]] >= pos[seq[i+1]] 说明 seq 与 nums 矛盾
 *
 * 【面试追问】
 * Q1: 什么情况下拓扑序不唯一？
 *     某一步有两个入度为 0 的节点，说明它们的相对顺序没有被约束
 * Q2: 这道题和课程表的区别？
 *     课程表只需判断有无环/输出任一拓扑序；此题要判断拓扑序是否唯一
 * Q3: 解法2 的正确性如何证明？
 *     若每对 (nums[i], nums[i+1]) 都有直接约束，则无法在它们之间插入其他元素
 */
