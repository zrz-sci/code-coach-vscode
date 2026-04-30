// ============================================================
// 720. Longest Word in Dictionary
// ============================================================
// 标签: Array, Hash Table, String, Trie, Sorting
// 难度: Medium
//
// 核心思路:
//   单词可逐步构建 ⟺ 每个前缀都在词典中
//   解法1: 排序后用 HashSet 递推验证
//   解法2: Trie + BFS 只走 isEnd=true 路径
// ============================================================


// ============================================================
// 解法1: 排序 + HashSet (推荐, 实现简洁)
//
// 思路:
//   1. 按长度升序排序 (同长度按字典序)
//   2. HashSet 初始放入空串 ""
//   3. 遍历每个单词 w:
//      - 若 w 去掉最后一个字符的前缀在 HashSet 中 → w 可构建
//      - 将 w 加入 HashSet, 更新 result
//
// 为什么正确:
//   排序保证处理 w 时, 比 w 短的单词都已处理
//   前缀检查只需看 w[0..len-2], 因为更短的前缀在处理
//   更短单词时已经验证过了 (递推)
//
// 时间: O(n * L * log n) 排序 + O(n * L) 遍历
// 空间: O(n * L) HashSet
// ============================================================
class Solution {
public:
    string longestWord(vector<string>& words) {
        // 按长度升序, 同长度按字典序
        sort(words.begin(), words.end());

        unordered_set<string> built;
        built.insert("");  // 空串是所有长度1单词的"前缀"
        string result;

        for (const string& w : words) {
            // 检查 w 去掉最后一个字符的前缀是否可构建
            string prefix = w.substr(0, w.size() - 1);
            if (built.count(prefix)) {
                built.insert(w);
                // 只在严格更长时更新, 保证字典序最小
                // (排序保证同长度中字典序小的先处理)
                if (w.size() > result.size()) {
                    result = w;
                }
            }
        }

        return result;
    }
};


// ============================================================
// 解法2: Trie + BFS (展示数据结构功底)
//
// 思路:
//   1. 构建 Trie, 插入所有单词
//   2. BFS 遍历 Trie, 只走 isEnd=true 的子节点
//   3. 按 a-z 顺序遍历子节点, 天然满足字典序
//   4. BFS 最后更新的 result 就是最长可构建单词
//
// 为什么只走 isEnd=true 的节点?
//   如果某个中间节点不是单词结尾, 说明对应前缀不在词典中
//   无法"逐步构建"到达更深的节点
//
// 时间: O(n * L) 插入 + O(n * L) 遍历
// 空间: O(n * L) Trie 节点
// ============================================================
class Solution_Trie {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
        string word;  // 存储到达该节点的完整单词
    };

public:
    string longestWord(vector<string>& words) {
        // Step 1: 构建 Trie
        TrieNode* root = new TrieNode();

        for (const string& w : words) {
            TrieNode* cur = root;
            for (char c : w) {
                int idx = c - 'a';
                if (!cur->children[idx]) {
                    cur->children[idx] = new TrieNode();
                }
                cur = cur->children[idx];
            }
            cur->isEnd = true;
            cur->word = w;
        }

        // Step 2: BFS, 只沿 isEnd=true 的节点前进
        string result;
        queue<TrieNode*> q;
        q.push(root);

        while (!q.empty()) {
            TrieNode* node = q.front();
            q.pop();

            // 按 a-z 顺序遍历子节点
            for (int i = 0; i < 26; i++) {
                TrieNode* child = node->children[i];
                if (child && child->isEnd) {
                    // 更新结果: 更长的单词 or 同长度字典序更小
                    if (child->word.size() > result.size()) {
                        result = child->word;
                    }
                    q.push(child);
                }
            }
        }

        // 清理内存 (面试中可省略, 但生产代码应有)
        deleteTrie(root);

        return result;
    }

private:
    void deleteTrie(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) {
            deleteTrie(node->children[i]);
        }
        delete node;
    }
};


// ============================================================
// 解法3: HashSet + 逐前缀验证 (朴素但清晰)
//
// 直接把所有 words 放入 set
// 对每个单词, 逐个检查长度 1,2,...,len-1 的前缀是否都在 set 中
//
// 时间: O(n * L^2) — 每个单词检查 O(L) 个前缀, 每个前缀 hash O(L)
// 空间: O(n * L)
// ============================================================
class Solution_Naive {
public:
    string longestWord(vector<string>& words) {
        unordered_set<string> wordSet(words.begin(), words.end());
        string result;

        for (const string& w : words) {
            // 检查每个前缀是否存在
            bool valid = true;
            for (int len = 1; len < (int)w.size(); len++) {
                if (!wordSet.count(w.substr(0, len))) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                // 更长, 或同长度字典序更小
                if (w.size() > result.size() ||
                    (w.size() == result.size() && w < result)) {
                    result = w;
                }
            }
        }

        return result;
    }
};


// ============================================================
// 总结
// ============================================================
/*
 * 【解法对比】
 *
 * | 解法              | 时间            | 空间    | 优点              |
 * |-------------------|-----------------|---------|-------------------|
 * | 排序+HashSet(推荐)| O(nL logn)      | O(nL)   | 简洁, 面试易写    |
 * | Trie+BFS          | O(nL)           | O(nL)   | 展示Trie, 无排序  |
 * | HashSet+逐前缀    | O(nL^2)         | O(nL)   | 最直观, 但较慢    |
 *
 * 【关键洞察】
 * 1. 可构建 ⟺ 每个前缀都在词典中 (递推判定)
 * 2. 排序法: 短单词先处理, 只需检查"去掉最后一个字符"的前缀
 * 3. Trie法: 只沿 isEnd=true 路径 BFS, 等价于验证所有前缀存在
 *
 * 【易错点】
 * 1. HashSet 必须初始包含空串 "", 否则长度1的单词无法判定
 * 2. 更新 result 用 > 不是 >=, 保证字典序最小
 * 3. Trie 中内存泄漏问题 (面试时可提一句, 说明知道需要清理)
 * 4. 排序规则: 先按长度升序, 不是降序
 */
