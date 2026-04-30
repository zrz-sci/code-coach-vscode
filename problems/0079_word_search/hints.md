# 79. 单词搜索

## 核心思路

本题本质上是在一个二维网格上做**路径搜索**：从任意格子出发，沿上下左右四个方向走，看能否走出一条路径使得路径上的字符依次拼成目标单词。这是经典的**DFS + 回溯**问题。

## 思维链

1. **读完题第一反应**：单词要从网格中沿相邻格子拼出来，那我需要尝试从每个格子出发，看能否匹配整个单词。最朴素的做法就是"枚举起点 + 深度优先搜索"。

2. **怎么搜索？** 从某个起点出发，如果当前格子匹配 word[0]，就尝试四个方向去匹配 word[1]，以此类推。这就是 DFS。关键约束是"同一个单元格不允许被重复使用"，所以需要**标记已访问**。

3. **回溯在哪？** 当某条路走不通时（字符不匹配或越界），需要**撤销标记**，让这个格子在其他路径中还能被使用。这就是回溯的精髓：做选择 → 递归 → 撤销选择。

4. **能不能优化？** 数据规模很小（6×6 网格，单词长度 ≤ 15），基础 DFS+回溯就能通过。但题目 Follow-up 问了剪枝优化，我们可以在搜索前做**字符频次检查**：如果网格中某个字符的出现次数少于单词中需要的次数，直接返回 false。还可以决定**从头还是从尾**开始匹配以减少搜索分支。

5. **标记的技巧**：可以用额外的 visited 数组，也可以**原地修改** board（把当前格子改成特殊字符如 `'#'`），回溯时再改回来，省掉 O(mn) 的空间。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS + 回溯（visited 数组） | 枚举起点，DFS 四方向匹配，visited 数组防重 | O(m·n·3^L) | O(m·n + L) | 能说出即可 |
| DFS + 回溯（原地标记）⭐ | 同上，但用原地修改代替 visited 数组 | O(m·n·3^L) | O(L) | ⭐ 必须写出 |
| DFS + 回溯 + 剪枝优化 | 字符频次预检 + 选择性反转单词 | O(m·n·3^L) 但常数更小 | O(L) | 加分项 |

> L = word.length。为什么是 3^L 而不是 4^L？因为每一步不会走回上一步来的方向（已标记），所以每步最多 3 个分支。

## 关键提示

- **提示1**：从每个格子作为起点尝试 DFS。一旦某个起点找到了完整匹配，立即返回 true。
- **提示2**：DFS 中如何防止重复使用同一个格子？两种方式：额外 visited 数组 vs 原地修改 board 后回溯恢复。
- **提示3**：递归的终止条件有两个：✅ 成功匹配到 word 的最后一个字符 → 返回 true；❌ 越界 / 字符不匹配 / 已访问 → 返回 false。
- **提示4（剪枝）**：搜索前统计 board 中各字符频次，如果不够覆盖 word 的需求，直接返回 false。
- **提示5（剪枝）**：如果 word[0] 在 board 中出现次数 > word[last] 的出现次数，把 word 反转，从出现次数少的那端开始匹配，可以减少起点数量。

```
// 示例1: board 和搜索路径 "ABCCED"
//
//   A → B → C   E          搜索路径:
//   S   F   C ← S          A(0,0) → B(0,1) → C(0,2)
//   A   D ← E   E                                ↓
//                           C(1,2) → E(2,2)  ← 不对，回看...
//                           实际: A→B→C(0,2)→C(1,2)→E(2,2)→D(2,1)
//
//   起点(0,0)='A' 匹配 word[0]
//   向右(0,1)='B' 匹配 word[1]
//   向右(0,2)='C' 匹配 word[2]
//   向下(1,2)='C' 匹配 word[3]
//   向下(2,2)='E' 匹配 word[4]
//   向左(2,1)='D' 匹配 word[5] ✓ 完成!
```

## 解法详解

### 解法1: DFS + 回溯（visited 数组）— O(m·n·3^L) / O(m·n)

**思考过程**：最直觉的做法——我需要记录哪些格子在当前路径中已使用，用一个二维布尔数组 `visited[i][j]` 来标记。进入格子时标记 true，回溯时标记 false。

```cpp
class Solution {
public:
    int m, n;
    // 四方向偏移: 上、下、左、右
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    bool exist(vector<vector<char>>& board, string word) {
        m = board.size();
        n = board[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        
        // 枚举每个格子作为起点
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (dfs(board, word, 0, i, j, visited)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    // k: 当前要匹配 word[k]
    bool dfs(vector<vector<char>>& board, const string& word, 
             int k, int i, int j, vector<vector<bool>>& visited) {
        // 成功匹配完最后一个字符
        if (k == word.size()) return true;
        
        // 越界 / 已访问 / 字符不匹配 → 剪掉这个分支
        if (i < 0 || i >= m || j < 0 || j >= n) return false;
        if (visited[i][j]) return false;
        if (board[i][j] != word[k]) return false;
        
        // 做选择：标记当前格子为已访问
        visited[i][j] = true;
        
        // 向四个方向递归
        for (auto& d : dirs) {
            if (dfs(board, word, k + 1, i + d[0], j + d[1], visited)) {
                return true;  // 找到了就立即返回，不用继续搜
            }
        }
        
        // 撤销选择：回溯
        visited[i][j] = false;
        return false;
    }
};
```

**关键点**：
- `visited[i][j] = false` 是回溯的核心——如果忘了撤销，后续从其他路径搜索时会错误地跳过这个格子。
- 先检查 `k == word.size()` 再检查越界，顺序很重要。

---

### 解法2: DFS + 回溯（原地标记）— O(m·n·3^L) / O(L) ⭐ 面试首选

**从解法1优化**：解法1 用了 O(m·n) 的 visited 数组。观察到 board 中都是英文字母，我们可以把当前格子临时改成一个不可能出现的字符（如 `'#'`），回溯时再改回来。这样省掉了 visited 数组，空间只剩递归栈的 O(L)。

```cpp
class Solution {
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
        if (k == word.size()) return true;
        if (i < 0 || i >= m || j < 0 || j >= n) return false;
        if (board[i][j] != word[k]) return false;
        
        // 做选择：临时替换为特殊字符，防止重复使用
        char saved = board[i][j];
        board[i][j] = '#';
        
        for (auto& d : dirs) {
            if (dfs(board, word, k + 1, i + d[0], j + d[1])) {
                // 注意：找到答案后也要恢复（如果题目要求不修改输入）
                // 但如果只需返回 true/false 且不在乎 board 状态，可以不恢复
                board[i][j] = saved;  // 良好习惯：恢复
                return true;
            }
        }
        
        // 撤销选择：恢复原字符
        board[i][j] = saved;
        return false;
    }
};
```

**关键点**：
- 原地修改是面试中最常见的写法，面试官期望看到这个技巧。
- 注意 `board[i][j] = '#'` 后如果 board 中本身可能包含 `'#'`，就要换一个真正不会出现的字符。题目保证只有英文字母，所以 `'#'` 是安全的。

---

### 解法3: DFS + 回溯 + 剪枝优化 — 常数优化

**进一步优化**：解法2 的时间复杂度上界不变，但可以通过剪枝显著减少实际搜索量。

**剪枝策略1 — 字符频次检查**：搜索前统计 board 中每个字符出现的次数，如果 word 中某个字符需要的次数超过 board 中的次数，直接返回 false。

**剪枝策略2 — 选择性反转 word**：如果 word[0] 在 board 中出现的次数比 word[last] 多，那么从 word 尾部开始匹配（反转 word），可以减少有效起点数量，大幅减少搜索。

```
// 剪枝示例：
// board 中 'A' 出现 5 次，'Z' 出现 1 次
// word = "A....Z"
// 正向搜索：5 个起点都要试
// 反向搜索（word 变 "Z....A"）：只有 1 个起点要试！
```

```cpp
class Solution {
public:
    int m, n;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    bool exist(vector<vector<char>>& board, string word) {
        m = board.size();
        n = board[0].size();
        
        // 剪枝1：字符频次检查
        unordered_map<char, int> boardCount;
        for (auto& row : board)
            for (char c : row)
                boardCount[c]++;
        
        unordered_map<char, int> wordCount;
        for (char c : word) wordCount[c]++;
        
        for (auto& [ch, cnt] : wordCount) {
            if (boardCount[ch] < cnt) return false;  // board 中字符不够
        }
        
        // 剪枝2：如果 word 首字符在 board 中出现次数 > 尾字符，反转 word
        if (boardCount[word.front()] > boardCount[word.back()]) {
            reverse(word.begin(), word.end());
        }
        
        // 标准 DFS + 回溯
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
        if (k == word.size()) return true;
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
```

```
// 决策树示例（搜索 "SEE"）:
//
// board:                尝试起点 (1,3)='S'
//   A B C E
//   S F C S  ←          从 S(1,3) 开始匹配 word[0]='S' ✓
//   A D E E
//
//                    S(1,3)
//               /    |    \        （不会向上走因为 E≠'E'...等等会走）
//           ↑E(0,3) ←C(1,2) ↓E(2,3)
//           匹配E?    匹配E?  匹配E?
//           word[1]   word[1] word[1]
//           E=='E'✓   C≠'E'✗  E=='E'✓
//            |                   |
//         继续匹配             继续匹配
//         word[2]='E'          word[2]='E'
//         四方向中             四方向中
//         无可用E → ✗          E(2,2)='E' ✓
//                              → 匹配完成! 返回 true
```

## 解法对比

| | 解法1 (visited 数组) | 解法2 (原地标记) ⭐ | 解法3 (剪枝优化) |
|---|---|---|---|
| 时间 | O(m·n·3^L) | O(m·n·3^L) | O(m·n·3^L) 但常数小 |
| 空间 | O(m·n + L) | O(L) | O(L) |
| 代码量 | 多一个 visited 数组 | 最简洁 | 多预处理代码 |
| 适用 | board 不能修改时 | 一般面试场景 | board 较大/需要卡常 |

面试中推荐直接写**解法2**，如果面试官追问优化，再补充**解法3**的剪枝策略。

## 易错点

1. **回溯时忘记撤销标记**
   - ✗ 只在失败时撤销：`board[i][j] = saved;` 只写在 return false 前
   - ✓ 成功路径也要撤销（或者确保函数返回前都恢复）：在 return true 前也要 `board[i][j] = saved;`
   - 为什么：如果不恢复，虽然当前 exist() 调用能得到正确结果，但 board 会被永久修改。面试中面试官可能追问"如果 board 不能修改呢"。

2. **终止条件顺序错误**
   - ✗ 先检查越界，再检查 `k == word.size()`：当 k 已经等于 word.size() 时，i/j 可能是无效坐标（因为是从上一层递归传入的邻居坐标），先越界检查会错误返回 false
   - ✓ 先检查 `if (k == word.size()) return true;`
   - 或者更稳妥的写法：在递归调用前就检查 k+1 == word.size()，匹配成功直接返回 true，就不会有这个问题。

3. **方向数组写错导致对角线移动**
   - ✗ `dirs = {{-1,-1},{-1,1},{1,-1},{1,1}}` — 这是对角线方向
   - ✓ `dirs = {{-1,0},{1,0},{0,-1},{0,1}}` — 上下左右四方向

4. **起点匹配忘了检查**
   - ✗ 枚举起点时不检查 `board[i][j] == word[0]` 就进入 DFS
   - ✓ 虽然 DFS 内部第一步会检查，但在外层提前判断可以避免不必要的函数调用开销（优化，非必须但推荐）

## 面试追问

**Q1（基础理解）**：这道题为什么用 DFS 而不是 BFS？
> **A**：BFS 适合"最短路径"问题，它按层扩展，不适合维护"一条路径"上的状态。本题需要沿着一条路径匹配字符，DFS 天然维护了从起点到当前点的路径，配合回溯可以高效地尝试和撤销选择。

**Q2（优化追问）**：时间复杂度 O(m·n·3^L) 中的 3 是怎么来的？能否进一步优化？
> **A**：每个格子最多有 4 个邻居，但来时的方向已经被标记为已访问，所以实际只有 3 个方向可走。进一步优化可以做：(1) 字符频次预检查，(2) 选择性反转 word 从出现少的字符端开始搜索，(3) 如果某个格子上次搜索失败过且状态一样可以缓存（但回溯场景下状态难以复用）。

**Q3（变体追问）**：如果要同时搜索多个单词怎么办？（→ LeetCode 212）
> **A**：如果逐个搜索，每个单词都要遍历整个 board，效率低。可以把所有单词构建成 **Trie（前缀树）**，然后在 board 上做一次 DFS，同时在 Trie 中匹配。这样一次遍历就能找到所有存在的单词。

**Q4（边界追问）**：如果 board 不允许修改，又不想用 O(mn) 的 visited 数组，有办法吗？
> **A**：可以用**位掩码**。board 最大 6×6=36 个格子，用一个 `long long` 的 36 个 bit 来记录哪些格子已访问。空间 O(1)（除递归栈外），但操作位运算比数组访问稍慢。

## 相关题型

- **212. 单词搜索 II** — 本题的多单词版本。复用本题的 DFS+回溯框架，区别：把多个 word 构建成 Trie，DFS 时同时在 Trie 上走，一次搜索找出所有存在的单词。
- **130. 被围绕的区域** — 同样是矩阵上 DFS/BFS + 标记的思路，复用方向数组和边界检查模板。
- **200. 岛屿数量** — 矩阵上 DFS 遍历的经典题，复用 DFS + 原地标记防重的技巧。
- **37. 解数独** — 同样是矩阵上的回溯搜索，但状态空间更复杂，复用"做选择→递归→撤销"的回溯框架。