# 126. 单词接龙 II

## 核心思路
找所有最短转换序列，本质是在单词图上做 BFS 找最短路径 + DFS/回溯枚举所有最短路径。关键是用 BFS 建 DAG（有向无环图），再在 DAG 上 DFS 回溯。

## 思维链
1. **建模**：每个单词是图中一个节点，相差一个字母的单词之间有边
2. **暴力 BFS + 记录路径**：BFS 时每个节点记录完整路径，内存爆炸
3. **优化思路**：先 BFS 确定最短距离，再 DFS 回溯找所有路径
4. **BFS 建 DAG**：BFS 过程中记录每个节点的"父节点集合"，构建一个 DAG
5. **DAG 上 DFS**：从 endWord 沿 DAG 反向 DFS 回溯到 beginWord，收集所有路径
6. **剪枝优化**：BFS 逐层处理，同一层的词不互相排斥（同层可以被多个父节点引用）

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS建DAG + DFS回溯 | 先BFS分层建图，再DFS枚举路径 | O(N * 26^L + P) | O(N * L) | ⭐ 必知 |
| 双向BFS + DFS | 从两端BFS建DAG，再DFS | O(N * 26^L + P) | O(N * L) | 进阶优化 |

其中 N = wordList.length, L = word.length, P = 所有最短路径的总长度

## 关键提示
1. **BFS 逐层删除**：每层 BFS 结束后，才从 wordSet 中删除该层访问的所有词（而非遇到就删），否则会丢失同层的并行路径
2. **建图方向**：记录 `children[word]` 或 `parents[word]`，后者更方便从 endWord 回溯
3. **换字母枚举邻居**：对每个位置尝试 26 个字母，比遍历整个 wordList 快（L*26 vs N）
4. **提前终止**：BFS 发现 endWord 后，处理完当前层就停止

```
示例: "hit" -> "cog"
wordList: ["hot","dot","dog","lot","log","cog"]

BFS 分层图（DAG）：
Layer 0: hit
Layer 1: hot
Layer 2: dot, lot
Layer 3: dog, log
Layer 4: cog

DAG 边（parent -> child）：
hit -> hot
hot -> dot, lot
dot -> dog
lot -> log
dog -> cog
log -> cog

DFS 从 hit 到 cog 的所有路径：
hit -> hot -> dot -> dog -> cog
hit -> hot -> lot -> log -> cog
```

## 解法详解

### 解法1：BFS 建 DAG + DFS 回溯

**思考过程**：
- 第一步 BFS：逐层扩展，记录每个词的子节点（下一层能到达的词），形成 DAG
- 关键：同一层内的词不能互相排除——必须整层处理完再从 wordSet 中删除
- 第二步 DFS：在 DAG 上从 beginWord 递归到 endWord，收集所有路径

```cpp
class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, 
                                        vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        vector<vector<string>> result;
        
        if (wordSet.find(endWord) == wordSet.end()) return result;
        
        // BFS 建 DAG：记录每个词的子节点
        unordered_map<string, vector<string>> children;
        
        bool found = false;
        unordered_set<string> currentLevel = {beginWord};
        wordSet.erase(beginWord);
        
        while (!currentLevel.empty() && !found) {
            // 先从 wordSet 中删除当前层所有词，防止同层回环
            // 但不能在遍历时删除，需要先收集下一层
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
            
            // 整层处理完后才删除，保证同层词可以被多个父节点引用
            for (const string& w : nextLevel) {
                wordSet.erase(w);
            }
            
            currentLevel = nextLevel;
        }
        
        if (!found) return result;
        
        // DFS 在 DAG 上回溯所有路径
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
```

**关键点**：
- BFS 使用 set 存当前层而非 queue，便于整层处理
- 必须等当前层完全处理后再从 wordSet 删除下一层的词
- DFS 部分是标准回溯模板

### 解法2：双向 BFS + DFS（优化版）

**思考过程**：
- 从 beginWord 和 endWord 两端同时 BFS，哪边小就扩展哪边
- 两端相遇时建好 DAG，再 DFS 回溯
- 减少搜索空间，类似双向 BFS 的剪枝效果

```cpp
class Solution {
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
        bool reversed = false;
        
        while (!frontSet.empty() && !backSet.empty() && !found) {
            // 总是扩展较小的一端
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
                        
                        // 检查是否与另一端相遇
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
            
            for (const string& w : nextLevel) {
                wordSet.erase(w);
            }
            
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
```

## 易错点

1. **BFS 中过早删除导致丢失路径**
```cpp
// ✗ 错误：遇到一个词就立即从 wordSet 删除
if (wordSet.count(temp)) {
    wordSet.erase(temp);  // 同层其他父节点就无法连到 temp 了！
    children[word].push_back(temp);
}

// ✓ 正确：收集到 nextLevel，整层结束后统一删除
if (wordSet.count(temp)) {
    nextLevel.insert(temp);
    children[word].push_back(temp);
}
// 循环结束后：
for (const string& w : nextLevel) wordSet.erase(w);
```

2. **找到 endWord 后没有处理完当前层就停止**
```cpp
// ✗ 错误：找到 endWord 立即 break
if (temp == endWord) {
    found = true;
    break;  // 当前层其他路径被丢弃！
}

// ✓ 正确：设置 found 标记，但继续处理当前层
if (temp == endWord) found = true;
children[word].push_back(temp);
// 当前层全部处理完后，下一轮 while 检查 found 终止
```

3. **BFS 用 queue 代替 set 导致重复访问**
```cpp
// ✗ 错误：用 queue 会导致同一个词被多次加入
queue<string> q;

// ✓ 正确：用 unordered_set 存当前层，自动去重
unordered_set<string> currentLevel;
```

## 面试追问

**Q1: 为什么不能在 BFS 中直接记录完整路径？**
→ 每个节点可能出现在多条最短路径中，记录完整路径会导致指数级内存爆炸。正确做法是只记录 DAG 的边（父子关系），最后 DFS 枚举。

**Q2: 这道题和 LC 127 (Word Ladder I) 的区别？**
→ LC 127 只需要最短长度，一个 BFS 就够了。本题需要所有最短路径，必须 BFS 建 DAG + DFS 回溯两步走。

**Q3: 如果 wordList 非常大（10^5），如何优化邻居查找？**
→ 当前做法每个词尝试 26*L 个变体是 O(26L) 的；如果 wordList 很大但 L 很小（本题 L<=5），这个方法比遍历 wordList O(N) 更快。如果 L 也很大，可以用通配符预处理：`h*t → [hat, hit, hot]`，将邻居查找降为 O(L)。

## 相关题型
- [127. Word Ladder](https://leetcode.com/problems/word-ladder/) - 只求最短长度，标准 BFS
- [752. Open the Lock](https://leetcode.com/problems/open-the-lock/) - 类似的 BFS 图搜索
- [301. Remove Invalid Parentheses](https://leetcode.com/problems/remove-invalid-parentheses/) - BFS 找所有最短结果
