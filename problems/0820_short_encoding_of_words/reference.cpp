/*
 * LeetCode 820: Short Encoding of Words (单词的压缩编码)
 *
 * 【题目本质】
 * 找到最短的编码字符串 s，使得每个 word 都是 s 中某个以 '#' 结尾的子串。
 * 如果一个单词是另一个的后缀，它可以被包含在较长的编码中。
 * 核心：去除所有是其他单词后缀的短词，计算剩余词的编码长度之和。
 *
 * 【解法总览】
 * 解法1: 哈希集合 + 后缀删除     — O(sum(w²)) / O(sum(w)) — 代码最简洁
 * 解法2: Trie(反转插入, 按长度排序) — O(sum(w)) / O(sum(w)) — 面试展示Trie
 * 解法3: Trie(反转插入, 叶子统计)  — O(sum(w)) / O(sum(w)) — 最通用
 */

// ============================================================
// 解法1: 哈希集合 + 后缀删除
// 时间: O(sum(w_i²))  空间: O(sum(w_i))
//
// 【思路】
// 把所有单词放入集合。对每个单词，枚举它的所有真后缀，
// 从集合中删掉。剩下的单词都不是任何其他单词的后缀。
//
// 示例: words = ["time", "me", "bell"]
// 集合 = {"time", "me", "bell"}
// "time" 的后缀: "ime", "me", "e" → 删掉 "me"
// "bell" 的后缀: "ell", "ll", "l" → 无匹配
// 剩余: {"time", "bell"} → (4+1) + (4+1) = 10
// ============================================================
class Solution1 {
public:
    int minimumLengthEncoding(vector<string>& words) {
        // 去重: 放入集合
        unordered_set<string> wordSet(words.begin(), words.end());

        // 对每个单词，删掉它的所有真后缀
        // 注意: 遍历的是原始 words (可能有重复, 无所谓, 多删几次不影响)
        for (const string& word : words) {
            // 从 i=1 开始, 不删除单词自身
            for (int i = 1; i < (int)word.size(); i++) {
                // word.substr(i) = word[i..end], 即一个后缀
                wordSet.erase(word.substr(i));
            }
        }

        // 剩余单词: 每个贡献 长度+1 ('#' 分隔符)
        int result = 0;
        for (const string& word : wordSet) {
            result += word.size() + 1;
        }
        return result;
    }
};

// ============================================================
// 解法2: Trie (反转插入, 按长度降序)
// 时间: O(sum(w_i) + n*log(n))  空间: O(sum(w_i))
//
// 【思路】
// 后缀关系 → 反转后变前缀关系 → 用 Trie 处理
//
// 关键: 按单词长度降序排列后插入 Trie。
// 长词先插入, 短词如果是长词的后缀(反转后是前缀),
// 插入时不会创建新节点 → 不贡献编码长度。
//
// 示例: words = ["time", "me", "bell"]
// 反转: ["emit", "em", "lleb"]
// 按长度排序: ["emit", "lleb", "em"]
//
// 插入 "emit": 创建 e→m→i→t, isNew=true, 贡献 4+1=5
// 插入 "lleb": 创建 l→l→e→b, isNew=true, 贡献 4+1=5
// 插入 "em":   e→m 已存在, isNew=false, 不贡献
//
// 结果: 5 + 5 = 10
// ============================================================
class Solution2 {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
    };

    int minimumLengthEncoding(vector<string>& words) {
        // 去重
        unordered_set<string> seen;

        // 按长度降序排列 (先插长词)
        sort(words.begin(), words.end(),
             [](const string& a, const string& b) {
                 return a.size() > b.size();
             });

        TrieNode* root = new TrieNode();
        int result = 0;

        for (const string& word : words) {
            if (seen.count(word)) continue;  // 跳过重复
            seen.insert(word);

            TrieNode* node = root;
            bool isNew = false;  // 是否创建了新节点

            // 反转方向插入 (从后往前遍历)
            for (int i = (int)word.size() - 1; i >= 0; i--) {
                int c = word[i] - 'a';
                if (!node->children[c]) {
                    node->children[c] = new TrieNode();
                    isNew = true;  // 创建了新节点 = 不被已有词包含
                }
                node = node->children[c];
            }

            // 只有创建了新节点的词才贡献编码长度
            if (isNew) {
                result += word.size() + 1;
            }
        }

        return result;
    }
};

// ============================================================
// 解法3: Trie (反转插入, DFS 统计叶子节点)
// 时间: O(sum(w_i))  空间: O(sum(w_i))
//
// 【思路】
// 不需要排序。全部插入 Trie 后，DFS 统计叶子节点的深度之和。
// 叶子节点 = 没有任何子节点的节点 = 不是其他更长单词的前缀。
// 每个叶子节点贡献 深度+1 (加 '#')。
//
// 这种方法更通用，不依赖插入顺序。
//
// 示例: 反转后 ["emit", "em", "lleb"]
// Trie:
//   root → e → m → i → t (叶子, depth=4, 贡献5)
//        → l → l → e → b (叶子, depth=4, 贡献5)
// "em" 的终点 m 不是叶子 (有 i 子节点), 不贡献
// 结果: 5 + 5 = 10
// ============================================================
class Solution3 {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
    };

    int minimumLengthEncoding(vector<string>& words) {
        TrieNode* root = new TrieNode();

        // 去重后全部插入 Trie (反转方向)
        unordered_set<string> uniqueWords(words.begin(), words.end());

        // 记录每个单词对应的 Trie 终点节点
        // key: Trie 终点节点指针, value: 单词长度
        unordered_map<TrieNode*, int> leaves;

        for (const string& word : uniqueWords) {
            TrieNode* node = root;
            for (int i = (int)word.size() - 1; i >= 0; i--) {
                int c = word[i] - 'a';
                if (!node->children[c]) {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }
            leaves[node] = word.size();
        }

        // 统计叶子节点: 没有任何子节点的终点
        int result = 0;
        for (auto& [node, len] : leaves) {
            bool isLeaf = true;
            for (int i = 0; i < 26; i++) {
                if (node->children[i]) {
                    isLeaf = false;
                    break;
                }
            }
            if (isLeaf) {
                result += len + 1;
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间         | 空间       | 优势                  |
// |----------------|-------------|-----------|----------------------|
// | 哈希+后缀删除   | O(sum(w²))  | O(sum(w)) | 代码最短，直觉         |
// | Trie+排序       | O(sum(w))   | O(sum(w)) | 线性时间，展示Trie     |
// | Trie+叶子统计   | O(sum(w))   | O(sum(w)) | 最通用，不依赖排序     |
//
// 面试选择: 解法1最快写出且不容易出错。
// 如果面试官考察 Trie，用解法2或3展示。
// w_i <= 7, 所以解法1的 O(sum(w²)) 实际非常快。
//
// ============================================================
// 【易错点】
//
// 1. 后缀从 i=1 开始:
//    x 从 i=0 开始 → word.substr(0) == word, 会把自己删掉
//    v 从 i=1 开始, 只枚举真后缀
//
// 2. 忘记去重:
//    x ["time", "time"] → "time" 被计算两次
//    v 用 set 去重
//
// 3. Trie 插入方向:
//    x 正向插入 → 处理的是前缀关系, 不是后缀
//    v 反向插入 (从后往前遍历), 后缀变前缀
//
// 4. 编码长度 = 单词长度 + 1:
//    x 忘记加 '#' 的长度
//    v 每个保留的单词贡献 len + 1
//
// 5. Trie 按长度排序方案中忘记排序:
//    x 不排序 → 先插入 "me", 它贡献了长度,
//      后来插入 "time" 包含了 "me" 但 "me" 已经算了
//    v 按长度降序排列后插入
//
// ============================================================
// 【相关题型】
//
// - 208. 实现Trie(前缀树):
//   Trie 基础题, 本题是 Trie 的应用(反转处理后缀)。
//
// - 648. 单词替换:
//   Trie 处理前缀关系。本题处理后缀关系(通过反转)。
//
// - 14. 最长公共前缀:
//   字符串前缀处理基础题, 可用 Trie 建模。
// ============================================================
