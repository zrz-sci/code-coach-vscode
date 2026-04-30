// LeetCode 758: Bold Words in String
// 解法：暴力匹配 + 布尔标记
// 时间复杂度：O(n * m * L)  空间复杂度：O(n)
// 其中 n = s.length, m = words.length, L = 最大单词长度

// ===================== 解法一：暴力匹配 + 布尔标记（推荐） =====================

class Solution {
public:
    string boldWords(vector<string>& words, string s) {
        int n = s.size();
        vector<bool> bold(n, false);

        // 步骤1：标记所有需要加粗的字符位置
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                // 检查从位置 i 开始是否匹配该单词
                if (i + len <= n && s.substr(i, len) == word) {
                    // 将 [i, i+len) 范围全部标记为加粗
                    for (int j = i; j < i + len; j++) {
                        bold[j] = true;
                    }
                }
            }
        }

        // 步骤2：根据标记构建结果字符串
        string result;
        for (int i = 0; i < n; i++) {
            // 加粗区域的起始：当前位置是 bold 且前一个不是 bold
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                result += "<b>";
            }

            result += s[i];

            // 加粗区域的结尾：当前位置是 bold 且下一个不是 bold
            if (bold[i] && (i == n - 1 || !bold[i + 1])) {
                result += "</b>";
            }
        }

        return result;
    }
};


// ===================== 解法二：优化标记（用 end 变量避免重复标记） =====================

class Solution2 {
public:
    string boldWords(vector<string>& words, string s) {
        int n = s.size();
        vector<bool> bold(n, false);

        // 使用 end 变量追踪当前覆盖到的最远位置
        int coverEnd = 0;
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    // 更新覆盖范围的最远位置
                    coverEnd = max(coverEnd, i + len);
                }
            }
            // 如果当前位置在覆盖范围内，标记为加粗
            if (i < coverEnd) {
                bold[i] = true;
            }
        }

        // 构建结果
        string result;
        for (int i = 0; i < n; i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                result += "<b>";
            }
            result += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) {
                result += "</b>";
            }
        }

        return result;
    }
};


// ===================== 解法三：Trie 树优化匹配 =====================
// 适合关键词数量较多的场景

class Solution3 {
public:
    // Trie 节点定义
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;
        TrieNode() : isEnd(false) {
            memset(children, 0, sizeof(children));
        }
    };

    string boldWords(vector<string>& words, string s) {
        // 步骤1：构建 Trie 树
        TrieNode* root = new TrieNode();
        for (const string& word : words) {
            TrieNode* node = root;
            for (char c : word) {
                int idx = c - 'a';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
            }
            node->isEnd = true;
        }

        // 步骤2：用 Trie 树匹配并标记
        int n = s.size();
        vector<bool> bold(n, false);

        for (int i = 0; i < n; i++) {
            TrieNode* node = root;
            for (int j = i; j < n; j++) {
                int idx = s[j] - 'a';
                if (!node->children[idx]) break;  // Trie 中无此路径
                node = node->children[idx];
                if (node->isEnd) {
                    // 匹配到一个完整单词，标记 [i, j] 为加粗
                    for (int k = i; k <= j; k++) {
                        bold[k] = true;
                    }
                }
            }
        }

        // 步骤3：构建结果字符串
        string result;
        for (int i = 0; i < n; i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                result += "<b>";
            }
            result += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) {
                result += "</b>";
            }
        }

        // 清理 Trie 内存（面试中可省略）
        deleteTrie(root);

        return result;
    }

    void deleteTrie(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) {
            deleteTrie(node->children[i]);
        }
        delete node;
    }
};


// ===================== 关键细节说明 =====================
//
// 1. 布尔数组的妙用：
//    - 天然处理了重叠匹配的合并（多次设为 true 等于一次）
//    - 天然处理了相邻匹配的合并（连续 true 只需一对标签）
//    - 避免了显式的区间合并逻辑
//
// 2. 标签插入的判定条件：
//    - 开始标签：bold[i] == true 且 (i == 0 || bold[i-1] == false)
//    - 结束标签：bold[i] == true 且 (i == n-1 || bold[i+1] == false)
//    这保证了使用最少数量的标签
//
// 3. s.compare(i, len, word) 比 s.substr(i, len) == word 更高效
//    因为 compare 不创建临时子串对象
//
// 4. 此题与 LC 616 (Add Bold Tag in String) 完全等价
//    唯一区别是参数顺序：616 是 (s, words)，758 是 (words, s)
//
// 5. Trie 解法的优势：
//    - 对于每个起始位置 i，一次 Trie 遍历就能找到所有从 i 开始的匹配
//    - 当关键词有公共前缀时效率提升明显
//    - 但对本题约束（words <= 50, 每词 <= 10），暴力法已足够
