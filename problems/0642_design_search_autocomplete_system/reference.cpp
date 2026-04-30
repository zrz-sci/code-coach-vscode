/*
 * LeetCode 642: 设计搜索自动补全系统 (Design Search Autocomplete System)
 *
 * 【题目本质】
 * 流式输入场景下的"前缀匹配 + Top-K 排序"问题。
 * 用户逐字符输入，每输入一个字符就要返回与当前前缀匹配的 Top-3 热门句子。
 * 输入 '#' 表示句子结束，需要将完整句子存入系统。
 *
 * 【解法总览】
 * 解法1: HashMap 暴力         — 每次 input 遍历全部句子做前缀匹配 — O(N·L) / O(N·L)
 * 解法2: Trie + 节点缓存counts — 路径上每个节点存经过的所有句子及频次 — O(K log K) / O(总字符·N)
 * 解法3: Trie + DFS 收集       — 只在叶子存频次，查询时 DFS 子树 — O(子树+K log K) / O(总字符)
 * 解法4: Trie + 节点缓存Top-3  — 每个节点维护 Top-3 列表，查询 O(1) — O(1) / O(总字符·3)
 *
 * Trie 结构示意:
 *
 *   插入 "i love you"(5), "island"(3), "iroman"(2), "i love leetcode"(2):
 *
 *   root
 *    └─ 'i' ─┬─ ' ' ─ 'l' ─ 'o' ─ 'v' ─ 'e' ─ ' ' ─┬─ 'y' ─ 'o' ─ 'u' [freq=5]
 *             │                                         └─ 'l' ─ ... ─ 'e' [freq=2]
 *             ├─ 's' ─ 'l' ─ 'a' ─ 'n' ─ 'd' [freq=3]
 *             └─ 'r' ─ 'o' ─ 'm' ─ 'a' ─ 'n' [freq=2]
 *
 *   input("i") → 走到 'i' 节点 → 子树有4个句子 → 排序取Top3
 *   input(" ") → 走到 ' ' 节点 → 子树有2个句子 → 返回两个
 *   input("a") → ' ' 节点无 'a' 孩子 → dead=true → 返回 []
 *   input("#") → 存 "i a"(freq=1), 重置 curNode 到 root
 */

// ============================================================
// 解法1: HashMap 暴力 — 每次遍历全部句子做前缀匹配
// 时间: O(N·L) 每次 input（N=句子数, L=当前前缀长度）
// 空间: O(N·L) 存所有句子
//
// 【思路】
// 最朴素的思路：用 map 存 <句子, 频次>。每次 input 追加字符到
// 当前前缀，然后遍历 map 中所有句子，用 substr 检查前缀是否匹配，
// 收集匹配的排序取前3。
// 瓶颈：每次 input 都扫全集，句子多时慢。
// ============================================================
class AutocompleteSystem1 {
    unordered_map<string, int> freq;
    string curInput;

public:
    AutocompleteSystem1(vector<string>& sentences, vector<int>& times) {
        for (int i = 0; i < (int)sentences.size(); i++) {
            freq[sentences[i]] = times[i];
        }
        curInput = "";
    }
    
    vector<string> input(char c) {
        if (c == '#') {
            freq[curInput]++;   // 累加，不是覆盖！
            curInput = "";
            return {};
        }
        
        curInput += c;
        
        // 暴力遍历所有句子，检查前缀匹配
        vector<pair<string, int>> matches;
        for (auto& [sentence, count] : freq) {
            if (sentence.size() >= curInput.size() &&
                sentence.compare(0, curInput.size(), curInput) == 0) {
                // compare 比 substr 高效，避免创建临时字符串
                matches.push_back({sentence, count});
            }
        }
        
        // 排序规则：频次降序，频次相同则字典序升序
        sort(matches.begin(), matches.end(), [](auto& a, auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
        
        vector<string> result;
        for (int i = 0; i < min((int)matches.size(), 3); i++) {
            result.push_back(matches[i].first);
        }
        return result;
    }
};


// ============================================================
// 解法2: Trie + 节点缓存 counts — 路径上每个节点存经过的所有句子及频次
// 时间: input O(K log K)，K=当前前缀匹配的句子数；insert O(L·1)
// 空间: O(总Trie节点 × 平均经过句子数) — 冗余存储较多
//
// 【思路】
// 解法1的瓶颈：每次 input 都扫全集。用 Trie 组织句子，沿前缀
// 走到对应节点后，该节点的 counts map 包含所有以此前缀开头的句子。
//
// 关键设计：
// 1. 每个 Trie 节点有 unordered_map<string, int> counts，
//    记录所有经过此节点的完整句子及其频次。
// 2. insert 时路径上每个节点都更新 counts。
// 3. 维护 curNode 指针，每次 input 往下走一步，避免从根重查。
// 4. dead 标志：一旦走不下去，后续字符直到 '#' 都返回空。
//
// 为什么在每个节点存 counts 而不是只在叶子存？
// → 避免 DFS 遍历子树的开销，直接在当前节点取所有匹配句子。
// ============================================================
struct TrieNode2 {
    unordered_map<char, TrieNode2*> children;
    unordered_map<string, int> counts;  // 经过此节点的句子 → 频次
};

class AutocompleteSystem2 {
    TrieNode2* root;
    TrieNode2* curNode;
    string curInput;
    bool dead;

    void insert(const string& s, int count) {
        TrieNode2* node = root;
        for (char c : s) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode2();
            }
            node = node->children[c];
            node->counts[s] += count;
        }
    }

public:
    AutocompleteSystem2(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode2();
        curNode = root;
        curInput = "";
        dead = false;
        for (int i = 0; i < (int)sentences.size(); i++) {
            insert(sentences[i], times[i]);
        }
    }
    
    vector<string> input(char c) {
        if (c == '#') {
            // 句子结束：插入 Trie 并重置状态
            insert(curInput, 1);
            curInput = "";
            curNode = root;
            dead = false;          // 不重置这个会导致下一轮全部返回空！
            return {};
        }
        
        curInput += c;
        
        if (dead) return {};       // 之前已走不下去，短路返回
        
        if (!curNode->children.count(c)) {
            dead = true;           // 当前字符无匹配，后续都不可能匹配
            return {};
        }
        curNode = curNode->children[c];
        
        // 当前节点的 counts 就是所有以 curInput 为前缀的句子
        vector<pair<string, int>> matches(curNode->counts.begin(), 
                                           curNode->counts.end());
        
        // 排序：频次降序，字典序升序
        sort(matches.begin(), matches.end(), [](auto& a, auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
        
        vector<string> result;
        for (int i = 0; i < min((int)matches.size(), 3); i++) {
            result.push_back(matches[i].first);
        }
        return result;
    }
};


// ============================================================
// 解法3: Trie + DFS 收集 — 不在节点缓存，查询时 DFS 子树
// 时间: input O(DFS子树大小 + K log K)
// 空间: O(总Trie节点数) — 比解法2省空间
//
// 【思路】
// 解法2在每个节点冗余存了完整句子列表，空间大。
// 这个版本只在句子结尾节点标记频次和完整句子。
// 查询时从 curNode 做 DFS，收集子树中所有结尾节点，排序取Top3。
//
// 权衡：空间省了，但查询时间取决于子树大小。
// 如果某个前缀下有大量句子（如前缀 "i"），DFS 开销大。
// 用 partial_sort 而非 sort 可以稍微优化（只需前3）。
// ============================================================
struct TrieNode3 {
    unordered_map<char, TrieNode3*> children;
    int freq = 0;         // >0 表示有句子在此结尾
    string sentence;       // 完整句子（仅结尾节点有效）
};

class AutocompleteSystem3 {
    TrieNode3* root;
    TrieNode3* curNode;
    string curInput;
    bool dead;

    void insert(const string& s, int count) {
        TrieNode3* node = root;
        for (char ch : s) {
            if (!node->children.count(ch)) {
                node->children[ch] = new TrieNode3();
            }
            node = node->children[ch];
        }
        node->freq += count;
        node->sentence = s;
    }

    // DFS 遍历子树，收集所有句子结尾节点
    void dfs(TrieNode3* node, vector<pair<string, int>>& matches) {
        if (!node) return;
        if (node->freq > 0) {
            matches.push_back({node->sentence, node->freq});
        }
        for (auto& [ch, child] : node->children) {
            dfs(child, matches);
        }
    }

public:
    AutocompleteSystem3(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode3();
        curNode = root;
        curInput = "";
        dead = false;
        for (int i = 0; i < (int)sentences.size(); i++) {
            insert(sentences[i], times[i]);
        }
    }

    vector<string> input(char c) {
        if (c == '#') {
            insert(curInput, 1);
            curInput = "";
            curNode = root;
            dead = false;
            return {};
        }

        curInput += c;
        if (dead) return {};

        if (!curNode->children.count(c)) {
            dead = true;
            return {};
        }
        curNode = curNode->children[c];

        vector<pair<string, int>> matches;
        dfs(curNode, matches);

        // partial_sort 只排前3个，比 sort 高效
        int k = min((int)matches.size(), 3);
        partial_sort(matches.begin(), matches.begin() + k, matches.end(),
            [](auto& a, auto& b) {
                if (a.second != b.second) return a.second > b.second;
                return a.first < b.first;
            });

        vector<string> result;
        for (int i = 0; i < k; i++) {
            result.push_back(matches[i].first);
        }
        return result;
    }
};


// ============================================================
// 解法4: Trie + 每节点缓存 Top-3 — 查询 O(1)
// 时间: 查询 O(1)，插入 O(L)（L=句子长度，每节点排序常数个元素）
// 空间: O(总Trie节点 × 3)
//
// 【思路】
// 既然每次只需要 Top-3，为什么不在每个 Trie 节点直接维护一个
// Top-3 列表？查询时走到节点直接返回，O(1)。
//
// 关键设计：
// 1. 每个节点存 vector<pair<int, string>> top3，用 {-freq, sentence}
//    方便排序（pair 默认按第一元素升序 → -freq 升序 = freq 降序）。
// 2. insert 时路径上每个节点调用 updateTop3。
// 3. updateTop3：先查是否已存在该句子→更新频次→重新排序→截断为3。
// 4. 需要 freqMap 全局追踪每个句子的最新频次，因为遇到 '#' 时
//    要传入累计频次而非增量。
// ============================================================
struct TrieNode4 {
    unordered_map<char, TrieNode4*> children;
    // {-freq, sentence} → pair 默认排序 = 按freq降序, 同freq按字典序升序
    vector<pair<int, string>> top3;
    
    void updateTop3(const string& s, int totalFreq) {
        // 检查是否已存在
        for (auto& p : top3) {
            if (p.second == s) {
                p.first = -totalFreq;  // 更新为最新频次
                sort(top3.begin(), top3.end());
                return;
            }
        }
        // 不存在，添加
        top3.push_back({-totalFreq, s});
        sort(top3.begin(), top3.end());
        // 截断：最多保留3个
        if (top3.size() > 3) top3.resize(3);
    }
};

class AutocompleteSystem4 {
    TrieNode4* root;
    TrieNode4* curNode;
    string curInput;
    bool dead;
    unordered_map<string, int> freqMap;  // 全局频次记录

    void insert(const string& s, int totalFreq) {
        TrieNode4* node = root;
        for (char ch : s) {
            if (!node->children.count(ch)) {
                node->children[ch] = new TrieNode4();
            }
            node = node->children[ch];
            // 路径上每个节点都更新 Top-3
            node->updateTop3(s, totalFreq);
        }
    }

public:
    AutocompleteSystem4(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode4();
        curNode = root;
        curInput = "";
        dead = false;
        for (int i = 0; i < (int)sentences.size(); i++) {
            freqMap[sentences[i]] = times[i];
            insert(sentences[i], times[i]);
        }
    }

    vector<string> input(char c) {
        if (c == '#') {
            freqMap[curInput]++;
            // 传累计总频次，不是增量1！否则 top3 中频次不正确
            insert(curInput, freqMap[curInput]);
            curInput = "";
            curNode = root;
            dead = false;
            return {};
        }

        curInput += c;
        if (dead) return {};

        if (!curNode->children.count(c)) {
            dead = true;
            return {};
        }
        curNode = curNode->children[c];

        // 直接返回节点缓存的 Top-3
        vector<string> result;
        for (auto& [negFreq, s] : curNode->top3) {
            result.push_back(s);
        }
        return result;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度       | 解法1 HashMap | 解法2 Trie+counts | 解法3 Trie+DFS | 解法4 Trie+Top3 |
// |-----------|-------------|------------------|---------------|----------------|
// | 查询时间   | O(N·L)      | O(K log K)       | O(子树+K log K)| O(1)           |
// | 插入时间   | O(1)        | O(L)             | O(L)          | O(L)           |
// | 空间       | O(N·L)      | O(节点数·N) 冗余  | O(节点数) 最省 | O(节点数·3)    |
// | 面试推荐   | 暖场        | ⭐ 首选           | 经典变体       | 追问加分        |
//
// 选择建议：面试中先说 Solution1 暴力，再优化到 Solution2 Trie+排序。
//
// 【面试追问 Interview Follow-ups】
// Q1: 如果热度分数会动态变化（不只是 +1），怎么优化？
// A1: Trie 节点维护一个小顶堆（size=3），每次 insert 时更新堆。
//     或使用 TreeMap 按 (-count, sentence) 排序，支持动态删除和插入。
//
// Q2: 如何支持模糊匹配（允许1个字符错误）？
// A2: 在 Trie 搜索时，对每个节点允许一次"跳过"（走任意子节点），
//     相当于 BFS/DFS 带一个"错误预算"参数。复杂度从 O(L) 变为 O(26·L)。
//
// Q3: 系统需要支持百万级句子，内存怎么优化？
// A3: 压缩 Trie（Radix Tree），将只有单个子节点的路径合并为一条边。
//     或者用外部存储（Redis Sorted Set），只在内存中缓存热门前缀。
// ============================================================

// 【易错点】
// 1. input('#') 时忘记清空当前输入前缀 → 下次查询带上了旧前缀
//    ✗ input('#') 只存句子不清空 curInput → 后续 input('a') 查到 "#a" 开头的
//    ✓ input('#') 后必须 curInput = ""
// 2. 热度排序时忘记处理相同热度按字典序
//    ✗ 只按 count 降序 → "abc"(3次) 和 "abd"(3次) 顺序不确定
//    ✓ 排序 key = (-count, sentence)
// 3. Trie 节点的 children 用 map 还是 array[128] 影响性能
//    map: 省空间但查询 O(log k); array: 快但每节点占 128 指针
