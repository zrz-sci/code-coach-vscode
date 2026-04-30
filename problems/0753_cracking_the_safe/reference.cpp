// 解法一：DFS + HashSet（直观版）
// 时间 O(k^n)，空间 O(k^n)
// 从全0串开始，每次尝试追加 0~k-1 形成新密码，DFS直到覆盖所有密码
class Solution {
public:
    string crackSafe(int n, int k) {
        // 特殊情况：n=1，直接拼接所有数字
        if (n == 1) {
            string result;
            for (int i = 0; i < k; ++i) {
                result += to_string(i);
            }
            return result;
        }

        int totalPasswords = 1;
        for (int i = 0; i < n; ++i) totalPasswords *= k;

        // 起始串：n个0
        string result(n, '0');
        unordered_set<string> visited;
        visited.insert(result);

        dfs(result, visited, totalPasswords, n, k);

        return result;
    }

private:
    bool dfs(string& result, unordered_set<string>& visited,
             int total, int n, int k) {
        if ((int)visited.size() == total) {
            return true;
        }

        // 取当前串的最后 n-1 个字符作为前缀
        string prefix = result.substr(result.size() - n + 1);

        // 尝试追加 0 ~ k-1
        for (int i = 0; i < k; ++i) {
            string next = prefix + to_string(i);
            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                result += to_string(i);

                if (dfs(result, visited, total, n, k)) {
                    return true;
                }

                // 回溯
                result.pop_back();
                visited.erase(next);
            }
        }

        return false;
    }
};

// 解法二：Hierholzer 欧拉回路算法（标准版）
// 时间 O(k^n)，空间 O(k^n)
// 在 (n-1)-gram 图上找欧拉回路，每条边对应一个密码
class Solution2 {
public:
    string crackSafe(int n, int k) {
        if (n == 1) {
            string result;
            for (int i = 0; i < k; ++i) result += to_string(i);
            return result;
        }

        // 每个节点用 (n-1) 位 k 进制数表示
        // 节点 u 有 k 条出边，追加数字 d 后转移到节点 (u * k + d) % (k^(n-1))
        int numNodes = 1;
        for (int i = 0; i < n - 1; ++i) numNodes *= k;

        // degree[u] 表示节点 u 下一个要走的出边编号
        vector<int> degree(numNodes, 0);

        string result;
        // 从节点 0（全0串）开始 Hierholzer DFS
        hierholzer(0, k, numNodes, degree, result);

        // 加上起始节点对应的 n-1 个字符
        for (int i = 0; i < n - 1; ++i) {
            result += '0';
        }

        // Hierholzer 后序得到的是反序
        reverse(result.begin(), result.end());

        return result;
    }

private:
    void hierholzer(int node, int k, int mod,
                    vector<int>& degree, string& result) {
        // 尝试当前节点的所有出边
        while (degree[node] < k) {
            int d = degree[node]++;
            int nextNode = (node * k + d) % mod;
            hierholzer(nextNode, k, mod, degree, result);
            // 后序：回溯时记录边上的字符
            result += to_string(d);
        }
    }
};

// 解法三：迭代版 Hierholzer（避免栈溢出）
// 当 k^n 很大时，递归版可能栈溢出，改用显式栈
class Solution3 {
public:
    string crackSafe(int n, int k) {
        if (n == 1) {
            string result;
            for (int i = 0; i < k; ++i) result += to_string(i);
            return result;
        }

        int numNodes = 1;
        for (int i = 0; i < n - 1; ++i) numNodes *= k;

        vector<int> degree(numNodes, 0);

        // 显式栈: 存储 (节点, 到达该节点的边字符)
        stack<pair<int, int>> stk;
        stk.push({0, -1}); // 起始节点，无入边

        string result;

        while (!stk.empty()) {
            auto& [node, edgeChar] = stk.top();

            if (degree[node] < k) {
                int d = degree[node]++;
                int nextNode = (node * k + d) % numNodes;
                stk.push({nextNode, d});
            } else {
                // 当前节点的所有出边已走完，后序记录
                if (edgeChar >= 0) {
                    result += to_string(edgeChar);
                }
                stk.pop();
            }
        }

        // 加上起始节点的 n-1 个字符
        for (int i = 0; i < n - 1; ++i) result += '0';

        reverse(result.begin(), result.end());

        return result;
    }
};
