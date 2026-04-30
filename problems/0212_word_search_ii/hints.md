# 212. 单词搜索 II (Word Search II)

## 难度: Hard | 标签: Trie, Array, String, Backtracking, Matrix

---

## 核心思路

在 m x n 的字符网格上搜索多个单词，单词必须由相邻格子（上下左右）的字符组成，同一格子不可重复使用。如果对每个单词单独做一次 DFS（即 Word Search I 的做法），W 个单词需要 W 次全网格 DFS，太慢。

关键优化：将所有待搜索单词构建成一棵 **Trie（前缀树）**，然后用**一次 DFS** 同时搜索所有单词。DFS 的每一步沿着 Trie 向下走，如果当前路径不是任何单词的前缀，直接剪枝。

## 思维链

1. **读完题第一反应**：对每个单词做一次 DFS（LC 79 Word Search I 的做法）。W 个单词 x M*N 起点 x 4^L 搜索 = O(W * M * N * 4^L)，当 W = 3*10^4 时太慢。

2. **瓶颈分析**：多个单词可能共享前缀（如 "oath", "oat"），单独搜索会重复探索相同路径。

3. **优化思路 -> Trie**：把所有单词建成 Trie，DFS 时沿 Trie 走。多个单词共享前缀只需走一次。Trie 还天然提供了剪枝：如果当前路径不在 Trie 中，说明不是任何单词的前缀，直接返回。

4. **实现细节**：
   - Trie 的叶子节点存完整单词（而不是 bool flag），这样到达叶子时直接取出单词，不需要回溯拼接。
   - 找到一个单词后，清空该节点的 word 标记，避免重复添加。
   - 进阶优化：找到单词后可以从 Trie 中删除它（逆向剪枝叶子节点），减少后续搜索空间。

5. **回溯标记**：DFS 时将 `board[i][j]` 改为 `'#'` 标记已访问，回溯时恢复，比维护 visited 数组更省空间。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力多次 DFS | 每个单词单独搜 | O(W*M*N*4^L) | O(W*L) | 太慢 |
| Trie + 回溯 DFS | 一次搜索所有 | O(M*N*4^L) | O(sum of word lengths) | ⭐⭐⭐ 必须掌握 |

## 关键提示

1. **Trie 节点存完整单词**：到达叶子直接取出 `node->word`，不需要传递当前路径字符串。取出后清空 `node->word = ""` 防止重复。

2. **剪枝：Trie 中无对应子节点即停**：`if (!node->children[c - 'a']) return;` 这一行是核心剪枝，任何不是单词前缀的路径直接截断。

3. **回溯标记用 board 本身**：`board[i][j] = '#'`（DFS 前）、`board[i][j] = c`（DFS 后），避免额外的 visited 数组。

4. **进阶优化：Trie 节点删除**：找到单词后，如果 Trie 节点没有其他子节点，可以从父节点中删除它。这样随着找到越来越多的单词，Trie 越来越小，DFS 越来越快。

5. **时间复杂度分析**：DFS 最坏情况每个格子出发扩展 4^L 步（L = 最大单词长度），共 M*N 个起点，所以 O(M * N * 4^L)。Trie 的构建 O(sum of word lengths)。

## Trie 数据结构详解

```
Trie 构建过程 (words = ["oath", "pea", "eat", "rain"]):

插入 "oath":
      root
      /
     o
      \
       a
        \
         t
          \
           h → word="oath"

插入 "pea":
      root
      / \
     o   p
      \   \
       a   e
        \   \
         t   a → word="pea"
          \
           h → word="oath"

插入 "eat":
        root
       / | \
      o  p  e
       \  \  \
        a  e  a
         \  \  \
          t  a  t → word="eat"
           \   (pea)
            h → word="oath"

插入 "rain":
          root
        / | | \
       o  p  e  r
        \ \  \   \
         a e  a   a
          \\  \    \
           t a  t    i
            \(pea)(eat)\
             h           n → word="rain"
        (oath)

完整 Trie 树结构:
         (root)
        / |  |  \
       o  p  e   r
       |  |  |   |
       a  e  a   a
       |  |  |   |
       t  a* t*  i
       |         |
       h*        n*

  * = 该节点存储了完整单词
  h*: word="oath", a*: word="pea", t*: word="eat", n*: word="rain"
```

## 解法详解

### Trie + 回溯 DFS — O(M*N*4^L) ⭐ 面试必须掌握

```
DFS 搜索过程示意 (board 如下):

  o  a  a  n
  e  t  a  e
  i  h  k  r
  i  f  l  v

从 board[0][0]='o' 出发, Trie 中 root->children['o'] 存在:
  o → 往下走 Trie 的 'o' 节点
  → board[1][0]='e': Trie 'o' 下没有 'e' → 剪枝!
  → board[0][1]='a': Trie 'o' 下有 'a' → 继续
    oa → 往下走 Trie 的 'a' 节点
    → board[0][2]='a': Trie 'oa' 下没有 'a' → 剪枝
    → board[1][1]='t': Trie 'oa' 下有 't' → 继续
      oat → 往下走 Trie 的 't' 节点
      → board[1][0]='e': Trie 'oat' 下没有 'e' → 剪枝
      → board[2][1]='h': Trie 'oat' 下有 'h' → 继续
        oath → word="oath" 非空 → 找到了! 加入结果，清空word
        (继续搜索，但 'oath' 的 'h' 没有更多子节点，自然返回)

从 board[1][1]='t' 出发, Trie 中 root 没有 't' → 跳过
...

从 board[1][2]='a' 出发, Trie 中 root 没有 'a'... 
  等等! 'e' 在 Trie 中! 所以从 board[1][0]='e' 出发:
  e → board[1][1]='t': Trie 'e' 下没有 't' → 剪枝
  e → board[0][0]='o': Trie 'e' 下没有 'o' → 剪枝
  e → board[2][0]='i': Trie 'e' 下没有 'i' → 剪枝

从 board[0][2]='a' 出发, Trie root 没有 'a' → 跳过
  (注意: 虽然 Trie 有 'e'->'a' 路径, 但 DFS 只从 root 开始)

最终从 board[2][1]='h' 出发也不会匹配任何前缀...

但从 board[1][2]='a' 出发，可以走到... 
  实际上 "eat" 的搜索: 从 board[1][0]='e' 出发, 
  但 Trie 'e'下面是 'a', board 中 'e' 的邻居没有'a'可以继续...
  
  等一下，从 board[0][2]='a' 向下到 board[1][2]='a', 再到 board[1][1]='t' 
  但这需要从 'e' 开始。'e' 在 board[0][3] 和 board[1][0]:
  
  从 board[0][3]='e': Trie root 有 'e' →
    e → board[0][2]='a': Trie 'e' 下有 'a' →
      ea → board[1][2]='a': Trie 'ea' 下没有 'a' → 剪枝
      ea → board[0][1]='a': Trie 'ea' 下没有 'a' → 剪枝  
      (hmm, 'ea' 下面只有 't')
      ea → board[0][3]已访问('#')
      ea → board[1][2]='a': Trie 没有 'a' → 剪枝
    e → board[1][3]='e': Trie 'e' 下没有 'e' → 剪枝

  从 board[1][2]='a'? root 没有 'a' → 跳过
  
  从 board[2][1]='h'? root 没有 'h' → 跳过
  
  "eat" 从 board[1][0]='e':
    e → board[0][0]='o' (no 'o' in Trie after 'e') → 剪枝
    e → board[1][1]='t' (no 't' in Trie after 'e') → 剪枝  
    e → board[2][0]='i' → 剪枝
  
  实际上 "eat" 是从 (1,2) 邻域开始... 但需要从 'e' 出发。
  board[0][3]='e' → 'a' at (0,2) → 't' at (1,2)? No, (0,2) 邻居有 (1,2).
  But Trie: e->a->t, 'eat' 结束.
  So: board[0][3]='e' → board[0][2]='a' → board[1][2]='a'? No, Trie 只有't'
  
  Hmm, board[0][2]的邻居: (0,1)='a', (0,3)='e'已访问, (1,2)='a'
  等等: board[0][2]='a' 邻居中 board[1][2]='a'，但 Trie 中 'ea' 下面只有 't'，
  不是 'a'。那 't' 在哪? board[1][1]='t'!
  
  board[0][3]='e' → board[0][2]='a' → board[1][2]='a'? Trie 没有 → 
  board[0][2]='a' 的邻居 board[1][2]='a' 但 Trie ea 下只有 t
  board[0][2]='a' 的邻居 board[0][1]='a' 但 Trie ea 下只有 t
  
  那从另一个 'e': board[1][0] 无法到达 'a' 再到 't'
  
  答案中有 "eat"，实际是: e(1,0)? 不对。让我重看board:
  o a a n          正确路径: e(0,3) → a(0,2) → t(1,2)? 
  e t a e          board[1][2]='a' 不是 't'!
  i h k r          board[1][1]='t'!
  i f l v
  
  "eat": e at (0,3), a at (1,3)? board[1][3]='e', no.
  实际: e at (1,0) or (0,3). a at (0,1), (0,2), (1,2). t at (1,1).
  
  路径: (0,3)e → (0,2)a → ... 无法到 t(1,1), 因为(0,2)不与(1,1)相邻
  路径: (1,0)e → (1,1)t? "et" 不是 "eat"
  
  其实 "eat": 官方结果确实包含 "eat"。
  路径是: (1,3)e? board[1][3]='e'! → (0,3)? 不对
  board[1][3]='e' → board[0][3]='n'? no → board[2][3]='r'? no → board[1][2]='a'!
  e(1,3) → a(1,2) → t(1,1) ← 这是正确路径! "eat" ✓

最终结果: ["oath", "eat"]
```

```cpp
struct TrieNode {
    TrieNode* children[26] = {};
    string word = "";  // 叶子存完整单词，省去回溯拼接
};

class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        // ========== Step 1: 构建 Trie ==========
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
            node->word = w;  // 叶子存完整单词
        }

        // ========== Step 2: 对每个格子做 DFS ==========
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
        
        // 核心剪枝: 当前字符不在 Trie 中 → 不是任何单词的前缀
        if (!next) return;

        // 如果到达一个完整单词
        if (!next->word.empty()) {
            result.push_back(next->word);
            next->word = "";  // 清空防止重复添加
        }

        // 标记当前格子已访问（回溯标记）
        board[i][j] = '#';

        // 向四个方向 DFS
        dfs(board, i + 1, j, next, result);
        dfs(board, i - 1, j, next, result);
        dfs(board, i, j + 1, next, result);
        dfs(board, i, j - 1, next, result);

        // 回溯: 恢复格子
        board[i][j] = c;
    }
};
```

### 进阶优化: 找到单词后删除 Trie 叶子节点

```cpp
// 进阶: DFS 返回是否应该删除当前 Trie 节点（逐步缩小搜索空间）
struct TrieNode2 {
    TrieNode2* children[26] = {};
    string word = "";
    int childCount = 0;  // 记录子节点数量，用于判断是否可以删除
};

class Solution2 {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode2* root = new TrieNode2();
        for (const string& w : words) {
            TrieNode2* node = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode2();
                    node->childCount++;
                }
                node = node->children[idx];
            }
            node->word = w;
        }

        vector<string> result;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                dfs(board, i, j, root, result);
        return result;
    }

private:
    void dfs(vector<vector<char>>& board, int i, int j,
             TrieNode2* node, vector<string>& result) {
        int m = board.size(), n = board[0].size();
        if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] == '#')
            return;

        char c = board[i][j];
        TrieNode2* next = node->children[c - 'a'];
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

        // 逆向剪枝: 如果子节点没有更多分支且不包含单词，删除它
        if (next->childCount == 0 && next->word.empty()) {
            node->children[c - 'a'] = nullptr;
            node->childCount--;
            delete next;
        }
    }
};
```

## 易错点

1. **Trie 节点不存完整单词，试图在 DFS 中传递路径字符串**
   - 错: `dfs(board, i, j, node, path + c, result)` — 字符串拷贝开销大
   - 对: 在 Trie 叶子存 `word = "oath"`，到达时直接取出

2. **找到单词后忘记清空标记导致重复**
   - 错: `result.push_back(next->word)` 不清空
   - 对: 取出后 `next->word = ""`，否则从不同路径到达同一 Trie 叶子会重复添加

3. **用 visited 数组而不是修改 board**
   - 不是错误，但浪费 O(M*N) 空间
   - 直接 `board[i][j] = '#'` + 回溯恢复更简洁

4. **DFS 中先检查 Trie 再检查边界**
   - 错: 先访问 `node->children[c-'a']` 再检查 i, j 合法性
   - 对: 先检查边界和 `board[i][j] != '#'`，再访问 Trie

5. **忘记回溯 board 标记**
   - 错: DFS 后不恢复 `board[i][j] = c`
   - 会导致后续从其他起点开始的 DFS 误以为该格子已使用

## 面试追问

**Q1: 为什么用 Trie 而不是 HashSet？**
→ HashSet 只能判断一个完整字符串是否存在，不能判断"当前路径是否是某个单词的前缀"。没有前缀判断就无法剪枝，DFS 必须走到底才知道是否匹配，退化成暴力。

**Q2: 时间复杂度 O(M*N*4^L) 看起来很大，实际上快吗？**
→ 实际远快于理论上界。Trie 的前缀剪枝非常强力：大量路径在 1-2 步就被截断。找到单词后的 Trie 删除进一步缩小搜索空间。实测即使 board 12x12、words 30000 个也能通过。

**Q3: 如果单词列表非常大（百万级），怎么优化？**
→ Trie 的空间可能成为瓶颈。可以考虑：(1) 压缩 Trie（合并单子节点链）(2) 只保留 board 中出现过的字符 (3) 分批处理单词。

**Q4: 这道题和 LC 79 Word Search I 的关系？**
→ LC 79 只搜一个单词，直接 DFS+回溯即可。本题搜多个单词，Trie 是将"多次单词搜索"合并为"一次前缀树搜索"的关键。如果面试只问 79，不需要 Trie。

## 相关题型

- [79. Word Search](../0079_word_search/) — 本题的简化版，单词只有一个
- [208. Implement Trie](../0208_implement_trie_prefix_tree/) — Trie 的基础实现
- [211. Design Add and Search Words Data Structure](../0211_design_add_and_search_words_data_structure/) — Trie + 通配符 DFS
- [425. Word Squares](../0425_word_squares/) — Trie + 回溯的另一个经典应用
