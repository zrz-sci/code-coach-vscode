/*
 * LeetCode 212: 单词搜索 II (Word Search II)
 *
 * 【题目本质】
 * 在 m x n 字符网格中搜索多个单词。每个单词由相邻格子（上下左右）组成，
 * 同一格子不可重复使用。核心是用 Trie 将多次单词搜索合并为一次前缀树搜索。
 *
 * 【解法总览】
 * 解法1: Trie + 回溯 DFS      — O(M*N*4^L) — 标准版 ⭐面试必须掌握
 * 解法2: Trie + 回溯 + 叶子删除 — O(M*N*4^L) — 进阶优化版
 */

// ============================================================
// Trie 数据结构
//
// 【为什么用 Trie?】
// 如果对每个单词单独做一次 DFS (LC 79 做法):
//   W 个单词 × M*N 起点 × 4^L 搜索 = O(W * M*N * 4^L)
//   当 W = 30000 时太慢
//
// Trie 的优势:
//   1. 多个单词共享前缀 → 只走一次公共路径
//   2. 天然提供前缀剪枝 → 不在 Trie 中的路径立即停止
//   3. 叶子存完整单词 → 到达时直接取出，无需回溯拼接
//
// Trie 构建示意 (words = ["oath", "pea", "eat", "rain"]):
//
//              (root)
//             / |  | \
//            o  p  e  r
//            |  |  |  |
//            a  e  a  a
//            |  |  |  |
//            t  a* t* i
//            |        |
//            h*       n*
//
//   * = 该节点 word 字段存储了完整单词
//   h* → word="oath"
//   a* → word="pea"  (p->e->a)
//   t* → word="eat"  (e->a->t)
//   n* → word="rain" (r->a->i->n)
//
// ============================================================

struct TrieNode {
    TrieNode* children[26] = {};  // 26 个字母的子节点
    string word = "";             // 到达此节点时的完整单词（非叶子为空）
};

// ============================================================
// 解法1: Trie + 回溯 DFS — 标准版 ⭐面试必须掌握
// 时间: O(M * N * 4^L)  空间: O(sum of word lengths)
//
// 【思路】
// Step 1: 将所有待搜索单词构建成 Trie
// Step 2: 对 board 的每个格子为起点做 DFS
//         DFS 每一步沿着 Trie 向下走:
//           - 当前字符不在 Trie 子节点中 → 剪枝返回
//           - 到达带 word 标记的节点 → 找到一个单词
//           - 标记 board[i][j]='#' 防止重复访问，回溯时恢复
//
// DFS 搜索过程 (board 和 Trie 如上):
//
//   board:     o  a  a  n
//              e  t  a  e
//              i  h  k  r
//              i  f  l  v
//
//   从 (0,0)='o' 出发:
//     root→o → 邻居中找 Trie 'o' 的子节点 'a'
//     → (0,1)='a' → root→o→a → 找 'a' 的子节点 't'
//       → (1,1)='t' → root→o→a→t → 找 't' 的子节点 'h'
//         → (2,1)='h' → root→o→a→t→h → word="oath" → 找到!
//
//   从 (1,3)='e' 出发:
//     root→e → 找 'e' 的子节点 'a'
//     → (1,2)='a' → root→e→a → 找 'a' 的子节点 't'
//       → (1,1)='t' → root→e→a→t → word="eat" → 找到!
//
//   "pea" 和 "rain" 在 board 中没有对应路径 → 不在结果中
//
//   最终: result = ["oath", "eat"]
//
// ============================================================
class Solution1 {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        // ===== Step 1: 构建 Trie =====
        TrieNode* root = new TrieNode();
        for (const string& w : words) {
            TrieNode* node = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
            }
            node->word = w;  // 在叶子存完整单词
        }

        // ===== Step 2: 对每个格子做 DFS =====
        vector<string> result;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                dfs(board, i, j, root, result);
            }
        }
        return result;
    }

private:
    void dfs(vector<vector<char>>& board, int i, int j,
             TrieNode* node, vector<string>& result) {
        int m = board.size(), n = board[0].size();

        // 边界检查 + 已访问检查
        if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] == '#')
            return;

        char c = board[i][j];
        TrieNode* next = node->children[c - 'a'];

        // 核心剪枝: 当前字符不在 Trie 中
        // → 不是任何待搜索单词的前缀 → 直接返回
        if (!next) return;

        // 如果到达一个完整单词的终点
        if (!next->word.empty()) {
            result.push_back(next->word);
            next->word = "";  // 清空防止重复添加
            // 注意: 不要 return! 可能存在更长的单词共享这个前缀
            // 例: "oath" 和 "oathkeeper"，找到 "oath" 后还要继续
        }

        // 标记当前格子已访问（比 visited 数组更省空间）
        board[i][j] = '#';

        // 向四个方向递归 DFS
        dfs(board, i + 1, j, next, result);  // 下
        dfs(board, i - 1, j, next, result);  // 上
        dfs(board, i, j + 1, next, result);  // 右
        dfs(board, i, j - 1, next, result);  // 左

        // 回溯: 恢复格子原始值
        board[i][j] = c;
    }
};

// ============================================================
// 解法2: 进阶优化 — 找到单词后删除 Trie 叶子节点
// 时间: O(M * N * 4^L)  空间: O(sum of word lengths)
//
// 【优化思路】
// 找到一个单词后，如果对应的 Trie 叶子节点没有更多子节点，
// 从 Trie 中删除它。这样:
//   - Trie 随着找到越来越多单词而逐渐缩小
//   - DFS 的剪枝越来越早，后续搜索越来越快
//   - 当所有单词都找到后，Trie 变为空，DFS 立即剪枝
//
// DFS 函数返回当前 Trie 节点是否"可以被删除"
// ============================================================

struct TrieNode2 {
    TrieNode2* children[26] = {};
    string word = "";
};

class Solution2 {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        // 构建 Trie
        TrieNode2* root = new TrieNode2();
        for (const string& w : words) {
            TrieNode2* node = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!node->children[idx])
                    node->children[idx] = new TrieNode2();
                node = node->children[idx];
            }
            node->word = w;
        }

        vector<string> result;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                dfs(board, i, j, root, result);

        // 清理 Trie 内存（面试中可省略）
        deleteTrie(root);
        return result;
    }

private:
    void dfs(vector<vector<char>>& board, int i, int j,
             TrieNode2* node, vector<string>& result) {
        int m = board.size(), n = board[0].size();
        if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] == '#')
            return;

        char c = board[i][j];
        int idx = c - 'a';
        TrieNode2* next = node->children[idx];
        if (!next) return;

        if (!next->word.empty()) {
            result.push_back(next->word);
            next->word = "";
        }

        board[i][j] = '#';
        dfs(board, i + 1, j, next, result);
        dfs(board, i - 1, j, next, result);
        dfs(board, i, j + 1, next, result);
        dfs(board, i, j - 1, next, result);
        board[i][j] = c;

        // 逆向剪枝: 如果 next 已经是空叶子，从父节点删除
        // 判断: word 为空 且 没有任何子节点
        if (next->word.empty() && isEmpty(next)) {
            node->children[idx] = nullptr;
            delete next;
        }
    }

    bool isEmpty(TrieNode2* node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) return false;
        }
        return true;
    }

    void deleteTrie(TrieNode2* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) {
            deleteTrie(node->children[i]);
        }
        delete node;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间          | 空间            | 面试推荐 |
// |---------------|---------------|----------------|---------|
// | 暴力多次DFS    | O(W*M*N*4^L) | O(W*L)         | 太慢     |
// | Trie+回溯      | O(M*N*4^L)   | O(sum(|w|))    | ⭐⭐⭐    |
// | Trie+回溯+删除 | O(M*N*4^L)   | O(sum(|w|))    | 加分项   |
//
// 理论复杂度相同，但 Trie 剪枝和叶子删除使实际运行快得多。
// 面试中写出解法1即可通过，解法2作为优化追问的回答。
//
// ============================================================
// 【易错点】
//
// 1. 找到单词后不能立即 return:
//    ✗ if (!next->word.empty()) { result.push_back(...); return; }
//    ✓ 找到后继续 DFS，因为可能有更长的单词共享前缀
//    例: "oath" 和 "oathkeeper" 共享前缀
//
// 2. 清空 word 防止重复:
//    ✗ 找到后不清空 → 从不同路径到达同一叶子会重复添加
//    ✓ next->word = "" 或用 set 去重
//
// 3. 回溯标记:
//    ✗ 不恢复 board[i][j] → 后续 DFS 误判格子已使用
//    ✓ DFS 结束后 board[i][j] = c 恢复原值
//
// 4. DFS 参数传的是 next (子节点) 不是 node (当前节点):
//    ✗ dfs(board, i+1, j, node, result) — 没有在 Trie 中前进
//    ✓ dfs(board, i+1, j, next, result) — 沿 Trie 向下走一步
//
// ============================================================
// 【面试追问 Interview Follow-ups】
//
// Q1: 为什么 Trie 比 HashSet 好?
// A1: HashSet 只能判断完整单词是否存在，无法判断"当前路径是否是某个
//     单词的前缀"。没有前缀判断就无法剪枝，DFS 必须穷尽所有路径。
//
// Q2: 时间复杂度 O(M*N*4^L) 很大，为什么实际很快?
// A2: Trie 剪枝非常强力。4^L 是最坏情况（board 全是同一字符）。
//     实际中大部分路径 1-2 步就被剪掉。叶子删除进一步缩小搜索空间。
//
// Q3: 如何避免重复单词?
// A3: 方法1: 找到后 word="" 清空标记（本题做法）
//     方法2: 结果放入 unordered_set 最后转 vector
//     方法1 更高效，只需要修改一个指针。
//
// Q4: 这道题 vs LC 79 Word Search I?
// A4: LC 79 只搜一个单词，直接 DFS+回溯，不需要 Trie。
//     本题搜多个单词，Trie 将"多次搜索"合并为"一次前缀树搜索"。
// ============================================================
