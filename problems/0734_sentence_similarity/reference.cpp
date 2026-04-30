// 734. Sentence Similarity
// 难度: Easy
// 标签: Array, Hash Table, String

// ===================== 解法一：HashSet 存储词对（推荐） =====================
// 时间 O(n + p)  空间 O(p)
// n 为句子长度，p 为 similarPairs 数量
// 思路：将所有相似词对的双向编码存入 HashSet，逐位置 O(1) 查询
class Solution {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        // 长度不等直接返回 false
        if (sentence1.size() != sentence2.size()) return false;

        // 构建词对的 HashSet，使用 "#" 分隔符拼接
        unordered_set<string> pairSet;
        for (auto& pair : similarPairs) {
            pairSet.insert(pair[0] + "#" + pair[1]);
            pairSet.insert(pair[1] + "#" + pair[0]); // 双向存储
        }

        // 逐位置比较
        for (int i = 0; i < (int)sentence1.size(); i++) {
            const string& w1 = sentence1[i];
            const string& w2 = sentence2[i];

            // 相同单词直接通过
            if (w1 == w2) continue;

            // 查找是否存在相似关系
            if (pairSet.count(w1 + "#" + w2)) continue;

            // 既不相同也不相似
            return false;
        }
        return true;
    }
};

// ===================== 解法二：HashMap 邻接表 =====================
// 时间 O(n + p)  空间 O(p)
// 思路：用 HashMap<string, set<string>> 表达相似关系图
//       对每个位置检查 w2 是否在 w1 的邻居集合中
class Solution2 {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;

        // 构建邻接表：每个单词映射到它的相似单词集合
        unordered_map<string, unordered_set<string>> adj;
        for (auto& pair : similarPairs) {
            adj[pair[0]].insert(pair[1]);
            adj[pair[1]].insert(pair[0]);
        }

        for (int i = 0; i < (int)sentence1.size(); i++) {
            const string& w1 = sentence1[i];
            const string& w2 = sentence2[i];

            if (w1 == w2) continue;

            // 检查 w1 的邻居中是否有 w2
            if (adj.count(w1) && adj[w1].count(w2)) continue;

            return false;
        }
        return true;
    }
};

// ===================== 解法三：暴力遍历 similarPairs =====================
// 时间 O(n * p)  空间 O(1)
// 思路：不做预处理，每个位置都遍历 similarPairs 查找匹配
//       适合 p 很小的场景，面试中作为暴力基线提及
class Solution3 {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;

        for (int i = 0; i < (int)sentence1.size(); i++) {
            const string& w1 = sentence1[i];
            const string& w2 = sentence2[i];

            if (w1 == w2) continue;

            bool found = false;
            for (auto& pair : similarPairs) {
                if ((pair[0] == w1 && pair[1] == w2) ||
                    (pair[0] == w2 && pair[1] == w1)) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
        return true;
    }
};

// ===================== 解法四：set<pair<string,string>> =====================
// 时间 O(n * log(p) + p * log(p))  空间 O(p)
// 思路：使用 set<pair<string,string>> 有序存储词对
//       查找时 O(log p)，不如 unordered_set 的 O(1)
//       但在面试中展示了 STL 容器的灵活运用
class Solution4 {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;

        // 使用 set<pair> 存储，利用 pair 的字典序比较
        set<pair<string, string>> pairSet;
        for (auto& p : similarPairs) {
            pairSet.insert({p[0], p[1]});
            pairSet.insert({p[1], p[0]});
        }

        for (int i = 0; i < (int)sentence1.size(); i++) {
            if (sentence1[i] == sentence2[i]) continue;
            if (pairSet.count({sentence1[i], sentence2[i]})) continue;
            return false;
        }
        return true;
    }
};

// ===================== 解法五：函数式风格 =====================
// 时间 O(n + p)  空间 O(p)
// 思路：使用 STL 算法和 lambda 实现更声明式的代码风格
class Solution5 {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;

        // 构建查找表
        unordered_set<string> lookup;
        for (auto& p : similarPairs) {
            lookup.insert(p[0] + "#" + p[1]);
            lookup.insert(p[1] + "#" + p[0]);
        }

        // 使用 lambda 判断单个位置
        auto isSimilar = [&](int i) -> bool {
            if (sentence1[i] == sentence2[i]) return true;
            return lookup.count(sentence1[i] + "#" + sentence2[i]) > 0;
        };

        // 检查所有位置
        for (int i = 0; i < (int)sentence1.size(); i++) {
            if (!isSimilar(i)) return false;
        }
        return true;
    }
};

/*
 * 总结：
 * - 解法一（HashSet）是面试首选：简洁高效，O(1) 查找
 * - 解法二（邻接表）更直观地表达图结构，是 LC 737 的前置思路
 * - 解法三（暴力）适合作为面试中的暴力基线
 * - 解法四（ordered set）展示 STL pair 的自然排序
 * - 解法五（函数式）展示现代 C++ 编程风格
 *
 * 关键要点：
 * 1. 首先检查句子长度是否相同
 * 2. 相似关系是双向的 -- 存储时需要存两个方向
 * 3. 相似关系不具有传递性 -- 不需要并查集
 * 4. 单词与自身总是相似的 -- w1 == w2 时直接 continue
 * 5. 使用分隔符拼接避免字符串哈希碰撞
 *
 * 与 LC 737 的对比：
 * - LC 734：不可传递 => HashSet 查边即可
 * - LC 737：可传递 => 并查集（Union-Find）判连通性
 */
