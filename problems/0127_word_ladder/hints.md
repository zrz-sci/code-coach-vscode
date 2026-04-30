# 127. 单词接龙

## 核心思路

把每个单词看作图中的一个节点，只差一个字母的两个单词之间有一条边——问题就变成了在无权图中求从 `beginWord` 到 `endWord` 的**最短路径**，而无权图的最短路径天然用 **BFS** 来解决。

## 思维链

1. **读完题第一反应**：从 beginWord 出发，每次只能改一个字母，改完的词必须在 wordList 里，问最少改几次到 endWord。这不就是"最少步数"问题吗？→ 联想到 BFS。

2. **建模成图**：每个单词是一个节点，如果两个单词只差一个字母就连一条边。问题变成：求从 beginWord 到 endWord 的最短路径长度。

3. **暴力建图的瓶颈**：如果对 wordList 中所有单词两两比较是否只差一个字母，建图需要 O(N² × L) 时间（N = 单词数，L = 单词长度）。当 N=5000, L=10 时还能接受，但有更聪明的方式。

4. **优化找邻居**：对当前单词的每个位置，尝试替换成 a-z 的每个字母，检查新单词是否在 wordList 的 HashSet 中。这样找一个单词的邻居是 O(26L)，比遍历整个 wordList 的 O(NL) 在 N 较大时更优。

5. **双向 BFS 的进一步优化**：BFS 搜索空间呈指数扩展。如果从两端同时出发，在中间相遇，搜索空间从 O(b^d) 降到 O(2 × b^(d/2))，其中 b 是分支因子，d 是深度。这在实际中可以带来巨大加速。

6. **关键细节**：返回的是序列中的单词数目（包含 beginWord 和 endWord），不是变换次数。所以答案 = 路径上的边数 + 1。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 单向 BFS | 从 beginWord 出发 BFS 找最短路径 | O(N × L × 26) | O(N × L) | ⭐ 必须写出 |
| 双向 BFS | 从两端同时 BFS，在中间相遇 | O(N × L × 26) 但实际更快 | O(N × L) | 加分项 |

> 注：N = wordList 长度，L = 单词长度

## 关键提示

1. **提示1**：无权图的最短路径 = BFS。你能把"单词变换"建模成一个图吗？节点是什么？边是什么？

2. **提示2**：找一个单词的"邻居"有两种方式：(a) 遍历 wordList 逐一比较；(b) 对每个位置尝试 26 个字母。当 N >> 26L 时，方式 (b) 更快。

3. **提示3**：BFS 中已经访问过的单词不要再访问（避免环），最好直接从 set 中删除或用 visited 集合标记。

4. **提示4**：注意 endWord 不在 wordList 中的情况，应该直接返回 0。

5. **提示5（进阶）**：BFS 的搜索空间呈树状指数增长。如果从两端同时搜索，选择"当前层更小"的一端扩展，可以大幅减少搜索空间。

```
BFS 搜索空间对比：

单向 BFS:                  双向 BFS:
beginWord ──────────>      beginWord ────>
   *                          *
  ***                        ***
 *****                      *****
*******                    *****  ← 在中间相遇
*********                   ***
***********                  *
************* endWord      endWord ────>

搜索空间: b^d              搜索空间: 2 × b^(d/2)
```

## 解法详解

### 解法1: 单向 BFS — O(N × L × 26) / O(N × L) ⭐ 面试首选

**思考过程**：
- 无权图最短路径 → BFS 是标准做法
- 把 wordList 放入 HashSet，方便 O(1) 查找
- 对当前单词的每个位置，尝试替换成 a-z，如果新单词在 set 中就入队
- 访问过的单词从 set 中删除，避免重复访问（等效于 visited 标记）
- 逐层 BFS，每层代表一步变换，第一次碰到 endWord 时的层数就是答案

```
// 示例: "hit" → "cog"
// wordList = {"hot","dot","dog","lot","log","cog"}
//
// 图结构:
//   hit
//    |         (h→h, i→o, t→t → "hot")
//   hot
//   / \        (h→d: "dot", h→l: "lot")
//  dot  lot
//  |     |     (d→d→g: "dog", l→l→g: "log")
//  dog  log
//   \  /       (d→c: "cog", l→c: "cog")
//   cog
//
// BFS 逐层扩展:
// Level 1: {hit}           → depth=1
// Level 2: {hot}           → depth=2
// Level 3: {dot, lot}      → depth=3
// Level 4: {dog, log}      → depth=4
// Level 5: {cog} ← 找到!  → depth=5, 返回 5
```

```cpp
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // 把 wordList 放入 set，支持 O(1) 查找和删除
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        
        // endWord 不在字典中，直接无解
        if (wordSet.find(endWord) == wordSet.end()) return 0;
        
        queue<string> q;
        q.push(beginWord);
        wordSet.erase(beginWord); // 标记已访问
        
        int depth = 1; // beginWord 本身算第 1 个单词
        
        while (!q.empty()) {
            int size = q.size(); // 当前层的节点数
            
            for (int i = 0; i < size; i++) {
                string word = q.front();
                q.pop();
                
                // 尝试改变每个位置的字母
                for (int j = 0; j < word.size(); j++) {
                    char original = word[j];
                    
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue; // 跳过相同字母
                        
                        word[j] = c;
                        
                        if (word == endWord) return depth + 1; // 找到了！
                        
                        if (wordSet.count(word)) {
                            q.push(word);
                            wordSet.erase(word); // 从 set 删除 = 标记已访问
                        }
                    }
                    
                    word[j] = original; // 恢复原字母
                }
            }
            
            depth++; // 进入下一层
        }
        
        return 0; // 无法到达 endWord
    }
};
```

**关键点**：
- 用 `wordSet.erase(word)` 代替额外的 visited 集合——更省空间，且能保证同一个词不会被重复入队
- 必须先检查 `endWord` 是否在 wordList 中
- `depth` 从 1 开始（题目要求返回的是单词数而非变换次数）

---

### 解法2: 双向 BFS — O(N × L × 26) / O(N × L) 进阶加分

**从解法1优化**：单向 BFS 从 beginWord 出发，搜索空间随层数指数增长。如果我们同时从 beginWord 和 endWord 两端出发，每次扩展"当前较小"的一端，两个搜索前沿在中间相遇时就是最短路径。实际搜索的节点数大幅减少。

**核心思想**：
- 维护两个集合 `frontSet` 和 `backSet`，分别从两端扩展
- 每次选择较小的集合进行扩展（贪心地减少搜索空间）
- 当扩展出的新单词在另一端的集合中出现时，说明两端相遇，找到了最短路径

```
// 双向 BFS 过程示意:
//
// Step1: front={hit}, back={cog}
//        front 更小(相等选front), 扩展 front
//        hit → hot  → front={hot}
//
// Step2: front={hot}, back={cog}
//        扩展 front
//        hot → dot, lot  → front={dot, lot}
//
// Step3: front={dot,lot}, back={cog}
//        back 更小, 扩展 back
//        cog → dog, log  → back={dog, log}
//
// Step4: front={dot,lot}, back={dog,log}
//        相等, 扩展 front
//        dot → dog (dog 在 back 中!) → 相遇! 返回 depth=5
```

```cpp
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) return 0;
        
        // 两个搜索前沿
        unordered_set<string> frontSet, backSet;
        frontSet.insert(beginWord);
        backSet.insert(endWord);
        
        int depth = 1;
        
        while (!frontSet.empty() && !backSet.empty()) {
            // 总是扩展较小的一端 —— 这是双向 BFS 的核心优化
            if (frontSet.size() > backSet.size()) {
                swap(frontSet, backSet);
            }
            
            unordered_set<string> nextSet; // 下一层的节点
            
            for (const string& word : frontSet) {
                string temp = word;
                
                for (int j = 0; j < temp.size(); j++) {
                    char original = temp[j];
                    
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;
                        
                        temp[j] = c;
                        
                        // 如果在另一端的集合中找到 → 两端相遇
                        if (backSet.count(temp)) return depth + 1;
                        
                        if (wordSet.count(temp)) {
                            nextSet.insert(temp);
                            wordSet.erase(temp); // 标记已访问
                        }
                    }
                    
                    temp[j] = original;
                }
            }
            
            depth++;
            frontSet = nextSet; // 用新一层替换旧的前沿
        }
        
        return 0;
    }
};
```

**关键点**：
- `swap(frontSet, backSet)` 保证总是扩展较小的一端
- `wordSet.erase(temp)` 确保一个单词不会被两端重复处理
- 渐近时间复杂度不变，但实际搜索空间大幅缩小

## 解法对比

| | 单向 BFS | 双向 BFS |
|---|---|---|
| **实现复杂度** | 简单，标准 BFS 模板 | 中等，需要维护两个集合 |
| **时间复杂度** | O(N × L × 26) | O(N × L × 26) 但实际更快 |
| **空间复杂度** | O(N × L) | O(N × L) |
| **适用场景** | 面试首选，容易写对 | 搜索空间大时显著加速 |
| **面试表现** | 写出即可通过 | 写出是加分项 |

**什么时候选哪个？**
- 面试中先写单向 BFS，如果面试官追问优化再给出双向 BFS
- 如果单词长度短（L ≤ 5）且字典大（N ≥ 1000），双向 BFS 效果显著

## 易错点

1. **✗ 忘记检查 endWord 是否在 wordList 中**
   ```cpp
   // 错误: 直接开始 BFS，最后才发现 endWord 不在字典中
   // 正确: 一开始就检查
   if (!wordSet.count(endWord)) return 0;
   ```

2. **✗ 用 visited 集合而不从 wordSet 中删除，导致多次入队**
   ```cpp
   // 错误: 只标记 visited 但在检查时用 wordSet
   if (wordSet.count(word) && !visited.count(word)) { ... }
   // 虽然正确但浪费空间。更好的做法：
   if (wordSet.count(word)) {
       q.push(word);
       wordSet.erase(word); // 直接删除 = 标记已访问
   }
   ```

3. **✗ 返回变换次数而不是单词数目**
   ```cpp
   // 错误: return depth;      // 这是变换次数
   // 正确: return depth + 1;  // 题目要求的是序列中的单词数目
   // 或者从 depth=1 开始计数
   ```

4. **✗ 在内层循环中忘记恢复原字母**
   ```cpp
   word[j] = c;
   // ... 检查 ...
   // 错误: 忘记 word[j] = original; 
   // 导致后续位置的替换基于已修改的字符串
   ```

5. **✗ 双向 BFS 中在扩展时没有从 wordSet 中删除**
   ```cpp
   // 错误: 只加入 nextSet 但不从 wordSet 删除
   // 导致同一个词可能被两端都处理，甚至死循环
   ```

## 面试追问

**Q1: 为什么用 BFS 而不是 DFS？**
→ 这是无权图最短路径问题。BFS 逐层扩展，第一次到达终点时就是最短路径。DFS 会找到一条路径，但不保证最短，需要遍历所有路径取最小值，效率极低。

**Q2: 找邻居时，为什么选择"逐位替换 26 个字母"而不是"遍历 wordList 逐一比较"？**
→ 逐位替换的代价是 O(26L)，遍历 wordList 的代价是 O(NL)。当 N > 26 时（通常如此），逐位替换更快。但如果单词极长（L 很大）而字典很小（N 很小），遍历 wordList 可能更好。

**Q3: 能否进一步优化？双向 BFS 为什么更快？**
→ 单向 BFS 搜索空间约为 O(b^d)（b 为分支因子，d 为深度）。双向 BFS 从两端出发，搜索空间约为 O(2 × b^(d/2))，指数减半。关键技巧是"总是扩展较小的一端"。

**Q4: 如果要求输出所有最短路径（LeetCode 126），该怎么改？**
→ 需要在 BFS 过程中记录每个单词的所有前驱节点（而不是只记录是否访问过），然后 BFS 结束后用 DFS/回溯从 endWord 往回还原所有路径。注意删除时机——不能在同一层就删除，而要等这一层全部扩展完再删。

## 相关题型

- **126. 单词接龙 II** — 复用本题的 BFS 框架，但要记录所有前驱节点，BFS 结束后回溯还原路径。区别：本题只求长度，126 要求所有最短路径。
- **752. 打开转盘锁** — 同样是无权图 BFS 求最短路径，每个状态是 4 位数字串，每次拨动一位。复用"逐位枚举邻居 + BFS"的模式。
- **433. 最小基因变化** — 几乎完全相同的套路，只是字符集从 26 个字母变成 {A, C, G, T}，单词长度固定为 8。可以直接套用本题代码。
- **279. 完全平方数** — 也是 BFS 求最短路径，但图的定义不同：每个数字是节点，从 n 到 n-k² 有一条边。