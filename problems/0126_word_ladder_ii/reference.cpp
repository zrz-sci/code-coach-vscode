/*
 * 【题目本质】
 * 在单词图上找从 beginWord 到 endWord 的所有最短路径。
 * 两个单词之间有边当且仅当它们恰好相差一个字母。
 *
 * 【解法总览】
 * 1. BFS建DAG + DFS回溯  - 标准两阶段法   O(N*26^L + P) / O(N*L)
 * 2. 双向BFS + DFS回溯   - 两端同时搜索   O(N*26^L + P) / O(N*L)
 *    N = wordList.length, L = word.length, P = 所有路径总长度
 *
 * 单词图示意（hit -> cog）：
 *
 *   hit
 *    |
 *   hot
 *   / \
 *  dot lot
 *   |   |
 *  dog log
 *   \ /
 *   cog
 *
 *   BFS 分层：
 *   Layer 0: {hit}
 *   Layer 1: {hot}
 *   Layer 2: {dot, lot}
 *   Layer 3: {dog, log}
 *   Layer 4: {cog}  ← 找到目标，停止BFS
 *
 *   DAG (children 关系)：
 *   hit -> [hot]
 *   hot -> [dot, lot]
 *   dot -> [dog]
 *   lot -> [log]
 *   dog -> [cog]
 *   log -> [cog]
 */

// ===================== 解法1：BFS 建 DAG + DFS 回溯 =====================
// 思路分两阶段：
// 阶段1 (BFS)：逐层扩展找最短路径，同时记录 children 关系形成 DAG。
//   核心陷阱：同一层的词不能互相排斥，必须整层处理完才从 wordSet 删除。
//   为什么？因为同一层的多个词可能被不同父节点引用，过早删除会丢失路径。
// 阶段2 (DFS)：在 DAG 上从 beginWord 回溯到 endWord，收集所有路径。
class Solution1 {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord,
                                        vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        vector<vector<string>> result;

        if (!wordSet.count(endWord)) return result;

        // 阶段1：BFS 建 DAG
        unordered_map<string, vector<string>> children;
        bool found = false;
        unordered_set<string> currentLevel = {beginWord};
        wordSet.erase(beginWord);

        while (!currentLevel.empty() && !found) {
            unordered_set<string> nextLevel;

            for (const string& word : currentLevel) {
                string temp = word;
                for (int i = 0; i < (int)temp.size(); i++) {
                    char original = temp[i];
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;
                        temp[i] = c;
                        if (wordSet.count(temp)) {
                            nextLevel.insert(temp);
                            children[word].push_back(temp);
                            if (temp == endWord) found = true;
                        }
                        temp[i] = original;
                    }
                }
            }

            // 关键：整层处理完才删除，保留同层多路径
            for (const string& w : nextLevel) {
                wordSet.erase(w);
            }

            currentLevel = nextLevel;
        }

        if (!found) return result;

        // 阶段2：DFS 在 DAG 上回溯
        vector<string> path = {beginWord};
        dfs(beginWord, endWord, children, path, result);

        return result;
    }

private:
    void dfs(const string& word, const string& endWord,
             unordered_map<string, vector<string>>& children,
             vector<string>& path, vector<vector<string>>& result) {
        if (word == endWord) {
            result.push_back(path);
            return;
        }
        if (children.find(word) == children.end()) return;

        for (const string& child : children[word]) {
            path.push_back(child);
            dfs(child, endWord, children, path, result);
            path.pop_back();
        }
    }
};

// ===================== 解法2：双向 BFS + DFS 回溯 =====================
// 思路：从 beginWord 和 endWord 两端同时 BFS，每次扩展较小的一端。
// 为什么更快？因为 BFS 搜索空间正比于扩展节点的扇出面积，
// 双向BFS让两端各搜一半深度，总面积远小于单向搜全程。
// 需要注意：记录 children 时要根据方向决定边的方向。
class Solution2 {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord,
                                        vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        vector<vector<string>> result;

        if (!wordSet.count(endWord)) return result;

        unordered_set<string> frontSet = {beginWord};
        unordered_set<string> backSet = {endWord};
        unordered_map<string, vector<string>> children;

        wordSet.erase(beginWord);
        wordSet.erase(endWord);

        bool found = false;
        bool reversed = false;  // 标记当前是否反转了搜索方向

        while (!frontSet.empty() && !backSet.empty() && !found) {
            if (frontSet.size() > backSet.size()) {
                swap(frontSet, backSet);
                reversed = !reversed;
            }

            unordered_set<string> nextLevel;

            for (const string& word : frontSet) {
                string temp = word;
                for (int i = 0; i < (int)temp.size(); i++) {
                    char original = temp[i];
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;
                        temp[i] = c;

                        // 与另一端相遇
                        if (backSet.count(temp)) {
                            found = true;
                            if (!reversed) children[word].push_back(temp);
                            else children[temp].push_back(word);
                        }

                        if (wordSet.count(temp)) {
                            nextLevel.insert(temp);
                            if (!reversed) children[word].push_back(temp);
                            else children[temp].push_back(word);
                        }

                        temp[i] = original;
                    }
                }
            }

            for (const string& w : nextLevel) wordSet.erase(w);
            frontSet = nextLevel;
        }

        if (!found) return result;

        vector<string> path = {beginWord};
        dfs(beginWord, endWord, children, path, result);

        return result;
    }

private:
    void dfs(const string& word, const string& endWord,
             unordered_map<string, vector<string>>& children,
             vector<string>& path, vector<vector<string>>& result) {
        if (word == endWord) {
            result.push_back(path);
            return;
        }
        if (children.find(word) == children.end()) return;

        for (const string& child : children[word]) {
            path.push_back(child);
            dfs(child, endWord, children, path, result);
            path.pop_back();
        }
    }
};

/*
 * 【解法对比】
 * | 解法          | 时间           | 空间      | 适用场景           |
 * |-------------- |---------------|----------|-------------------|
 * | BFS+DFS       | O(N*26^L + P) | O(N*L)   | 面试标准解         |
 * | 双向BFS+DFS   | O(N*26^L + P) | O(N*L)   | 实际更快，但代码复杂 |
 *
 * 【易错点】
 * 1. 同层删除时机：必须整层 BFS 结束后才从 wordSet 删除下一层的词，
 *    否则同层其他路径到该词的边会被丢失
 *    ✗ wordSet.erase(temp); // 在内层循环中立即删除
 *    ✓ nextLevel.insert(temp); // 先收集，外层统一删除
 * 2. 找到 endWord 后不能立刻停止当前层的遍历，否则丢失同层其他路径
 *    ✗ if (temp == endWord) break;
 *    ✓ if (temp == endWord) found = true; // 标记，继续处理当前层
 * 3. 双向BFS中 reversed 标记要正确维护，否则 children 方向会乱
 *
 * 【面试追问】
 * Q1: 为什么不能在BFS时直接存路径？
 *     → 同一个词可能在多条路径中出现，存完整路径会导致指数级内存开销
 * Q2: 和 LC 127 的区别？
 *     → 127 只要最短长度，一个BFS够。126 要所有最短路径，
 *       必须BFS建DAG + DFS枚举
 * Q3: 时间复杂度中的 P 是什么？
 *     → P 是所有最短路径的总长度之和，题目保证 P <= 10^5
 */
