/*
 * LeetCode 79: 单词搜索 (Word Search)
 *
 * 【题目本质】
 * 在 m×n 字符网格上，从任意起点出发，沿上下左右相邻格子走出一条路径，
 * 使路径上的字符依次拼成目标单词。同一格子不能重复使用。
 * 本质：矩阵上的 DFS + 回溯。
 *
 * 【解法总览】
 * 解法1: DFS + 回溯（visited 数组）— O(m·n·3^L) / O(m·n + L)
 * 解法2: DFS + 回溯（原地标记） — O(m·n·3^L) / O(L) ⭐ 面试首选
 * 解法3: DFS + 回溯 + 剪枝优化  — O(m·n·3^L) / O(L)  常数级优化
 *
 * 其中 L = word.length()
 * 为什么是 3^L：每步最多 4 个方向，但来时的方向已标记，实际最多 3 个分支。
 */

// ============================================================
// 解法1: DFS + 回溯（visited 数组）
// 时间: O(m·n·3^L)  空间: O(m·n + L)
//
// 【思路】
// 最直觉的做法：用一个 visited 二维数组记录当前路径中已使用的格子。
// 枚举每个格子作为起点，DFS 尝试匹配 word 的每个字符。
// 匹配失败或搜索完毕时，撤销 visited 标记（回溯）。
//
// 搜索过程示例（word = "ABCCED"）:
//
//   board:               搜索路径:
//   A  B  C  E           A(0,0) → B(0,1) → C(0,2)
//   S  F  C  S                                ↓
//   A  D  E  E           D(2,1) ← E(2,2) ← C(1,2)
//
//   word[0]='A' 匹配 (0,0)
//   word[1]='B' 匹配 (0,1)  — 向右
//   word[2]='C' 匹配 (0,2)  — 向右
//   word[3]='C' 匹配 (1,2)  — 向下
//   word[4]='E' 匹配 (2,2)  — 向下
//   word[5]='D' 匹配 (2,1)  — 向左 ✓ 完成!
//
// 决策树（以 "SEE" 从 (1,3) 出发为例）:
//
//                     S(1,3)
//                  /     |      \
//            E(0,3)   C(1,2)   E(2,3)
//            word[1]  word[1]  word[1]
//            E='E'✓   C≠'E'✗  E='E'✓
//           /  |  \              |
//       C(0,2) B  S(1,3)     E(2,2)
//       C≠E ✗  ✗  已访问✗    word[2]
//       回溯                 E='E'✓ → 返回true!
// ============================================================
class Solution1 {
public:
    int m, n;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    bool exist(vector<vector<char>>& board, string word) {
        m = board.size();
        n = board[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (dfs(board, word, 0, i, j, visited)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool dfs(vector<vector<char>>& board, const string& word,
             int k, int i, int j, vector<vector<bool>>& visited) {
        // 终止条件：所有字符都已匹配成功
        // 为什么先检查这个？因为调用时 i,j 可能越界，但 k==size 说明上一步已经匹配完了
        if (k == (int)word.size()) return true;
        
        // 越界 / 已访问 / 字符不匹配 → 剪枝
        if (i < 0 || i >= m || j < 0 || j >= n) return false;
        if (visited[i][j]) return false;
        if (board[i][j] != word[k]) return false;
        
        // 做选择
        visited[i][j] = true;
        
        // 四方向递归
        for (auto& d : dirs) {
            if (dfs(board, word, k + 1, i + d[0], j + d[1], visited)) {
                visited[i][j] = false;  // 恢复（良好习惯）
                return true;
            }
        }
        
        // 撤销选择（回溯）
        visited[i][j] = false;
        return false;
    }
};

// ============================================================
// 解法2: DFS + 回溯（原地标记）⭐ 面试首选
// 时间: O(m·n·3^L)  空间: O(L) — 只有递归栈的空间
//
// 【思路】
// 解法1 的 visited 数组占 O(m·n) 空间。
// 优化：因为 board 只含英文字母，可以临时把当前格子改成 '#'（一个不可能
// 出现的字符），这样后续搜索到这个格子时 board[i][j] != word[k] 必然不匹配。
// 回溯时再恢复原字符。
//
// 这是面试中最常见的写法：简洁、空间省、且展示了"修改-恢复"的回溯技巧。
// ============================================================
class Solution2 {
public:
    int m, n;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    bool exist(vector<vector<char>>& board, string word) {
        m = board.size();
        n = board[0].size();
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (dfs(board, word, 0, i, j)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool dfs(vector<vector<char>>& board, const string& word,
             int k, int i, int j) {
        if (k == (int)word.size()) return true;
        if (i < 0 || i >= m || j < 0 || j >= n) return false;
        if (board[i][j] != word[k]) return false;
        
        // 做选择：保存原字符，替换为不可能出现的标记
        char saved = board[i][j];
        board[i][j] = '#';
        
        for (auto& d : dirs) {
            if (dfs(board, word, k + 1, i + d[0], j + d[1])) {
                board[i][j] = saved;  // 成功也要恢复
                return true;
            }
        }
        
        // 撤销选择：恢复原字符
        board[i][j] = saved;
        return false;
    }
};

// ============================================================
// 解法3: DFS + 回溯 + 剪枝优化（Follow-up）
// 时间: O(m·n·3^L) 最坏情况不变，但平均大幅优化
// 空间: O(L)
//
// 【思路】
// 在解法2 基础上增加两个预处理剪枝：
//
// 剪枝1 — 字符频次检查：
//   如果 word 中某字符需要 x 次，但 board 中只有 < x 次，直接 false。
//   例如 word 中需要 3 个 'A'，但 board 中只有 2 个 → 不可能匹配。
//
// 剪枝2 — 选择性反转 word：
//   如果 word[0] 在 board 中出现次数 > word[last] 出现次数，
//   反转 word，从出现少的字符端开始搜索，减少有效起点数量。
//
//   例如：word = "AAAB"，board 中 A 出现 10 次，B 出现 1 次
//   正向搜索：10 个起点
//   反向搜索 "BAAA"：1 个起点 → 快很多！
// ============================================================
class Solution3 {
public:
    int m, n;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    bool exist(vector<vector<char>>& board, string word) {
        m = board.size();
        n = board[0].size();
        
        // === 剪枝1：字符频次检查 ===
        // 统计 board 中每个字符的出现次数
        unordered_map<char, int> boardCnt;
        for (auto& row : board)
            for (char c : row)
                boardCnt[c]++;
        
        // word 中每个字符的需求量不能超过 board 中的供给量
        unordered_map<char, int> wordCnt;
        for (char c : word) wordCnt[c]++;
        
        for (auto& [ch, cnt] : wordCnt) {
            if (boardCnt[ch] < cnt) return false;
        }
        
        // === 剪枝2：选择性反转 ===
        // 从出现次数少的端开始搜索，减少起点数量
        if (boardCnt[word.front()] > boardCnt[word.back()]) {
            reverse(word.begin(), word.end());
        }
        
        // === 标准 DFS + 回溯 ===
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (dfs(board, word, 0, i, j)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool dfs(vector<vector<char>>& board, const string& word,
             int k, int i, int j) {
        if (k == (int)word.size()) return true;
        if (i < 0 || i >= m || j < 0 || j >= n) return false;
        if (board[i][j] != word[k]) return false;
        
        char saved = board[i][j];
        board[i][j] = '#';
        
        for (auto& d : dirs) {
            if (dfs(board, word, k + 1, i + d[0], j + d[1])) {
                board[i][j] = saved;
                return true;
            }
        }
        
        board[i][j] = saved;
        return false;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 空间     | 是否修改 board | 额外优化 | 推荐场景     |
// |-------------|----------|----------------|----------|--------------|
// | 解法1       | O(m·n+L) | 否             | 无       | board不可修改 |
// | 解法2 ⭐    | O(L)     | 临时修改后恢复 | 无       | 面试首选     |
// | 解法3       | O(L)     | 临时修改后恢复 | 频次+反转| 大数据/卡常  |
//
// 三种解法的 DFS 核心逻辑完全相同，区别只在"如何标记已访问"和"是否有预处理剪枝"。
//
// ============================================================
// 【易错点】
//
// 1. 回溯恢复遗漏：
//    ✗ 只在 return false 前写 board[i][j] = saved
//    ✓ 在 return true 前也要写 board[i][j] = saved
//    原因：如果提前返回 true 时不恢复，board 会被永久修改。
//
// 2. 终止条件顺序错误：
//    ✗ 先检查 i<0 || i>=m ... 再检查 k==word.size()
//       → 当 k 已等于 size 时 i,j 可能是邻居的无效坐标，越界会错误返回 false
//    ✓ 先检查 k == word.size() 再检查边界
//    或者：在调用 dfs 前就检查 k+1 == size 来避免这个问题
//
// 3. 方向数组写成对角线：
//    ✗ dirs = {{-1,-1},{-1,1},{1,-1},{1,1}} — 对角线方向
//    ✓ dirs = {{-1,0},{1,0},{0,-1},{0,1}}  — 上下左右
//
// 4. 原地标记字符选择不当：
//    ✗ board[i][j] = ' ' — 如果 board 可能包含空格就出 bug
//    ✓ board[i][j] = '#' — 题目保证只有英文字母，'#' 安全
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1（基础）：为什么用 DFS 而不是 BFS？
//    A: 本题需要维护"一条路径"上的标记状态，DFS 天然沿一条路径深入，
//       回溯时撤销标记。BFS 按层扩展，很难维护单条路径的访问状态。
//
// Q2（优化）：3^L 的 3 怎么来的？还有其他剪枝方式吗？
//    A: 每步 4 个方向但来时方向已标记，只剩 3 个。
//       额外剪枝：(1) 字符频次预检，(2) word 反转选择性搜索，
//       (3) 在搜索过程中如果剩余 board 字符不足以匹配 word 剩余部分可提前终止。
//
// Q3（变体）：如果有多个 word 要同时搜索？（→ LeetCode 212）
//    A: 把所有 word 构建成 Trie，在 board 上做一次 DFS，
//       同时沿 Trie 走。匹配到 Trie 中的 isEnd 节点就记录一个结果。
//       复杂度从 k * O(m·n·3^L) 降到一次 DFS 搞定。
//
// Q4（边界）：board 不能修改也不想用 O(mn) visited，有办法吗？
//    A: 用位掩码。board 最大 6×6=36 格，用一个 long long 的 36 bit
//       表示访问状态，按位操作进行标记和撤销，空间 O(1)。
//
// ============================================================
// 【相关题型】
//
// - 212. 单词搜索 II — 本题的多单词版。复用 DFS+回溯框架，
//   区别：多个 word 构建 Trie，DFS 时同步在 Trie 上匹配。
// - 200. 岛屿数量 — 矩阵上 DFS + 标记防重，复用方向数组和边界检查模板。
// - 130. 被围绕的区域 — 矩阵 DFS/BFS，从边界出发标记，复用相同的遍历框架。
// - 37. 解数独 — 矩阵上的回溯搜索，状态更复杂，复用"选择→递归→撤销"框架。
// ============================================================
