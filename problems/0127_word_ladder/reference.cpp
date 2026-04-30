/*
 * LeetCode 127: 单词接龙 (Word Ladder)
 *
 * 【题目本质】
 * 把单词看作图的节点，只差一个字母的单词之间连边，
 * 求从 beginWord 到 endWord 的最短路径长度（无权图最短路径 = BFS）。
 *
 * 【解法总览】
 * 解法1: 单向 BFS   — O(N×L×26) / O(N×L) — 面试首选，标准模板
 * 解法2: 双向 BFS   — O(N×L×26) / O(N×L) — 实际更快，加分项
 *
 * 其中 N = wordList.size(), L = 单词长度
 */

// ============================================================
// 解法1: 单向 BFS — 无权图最短路径的标准做法
// 时间: O(N × L × 26)  空间: O(N × L)
//
// 【思路】
// 问"最少变换几步"→ 无权图最短路径 → BFS。
//
// 关键问题：如何找一个单词的所有邻居？
// 方案A: 遍历 wordList 逐一比较 → O(N×L) 每个节点
// 方案B: 对每个位置尝试 a-z 替换，查 HashSet → O(26×L) 每个节点
// 当 N > 26 时方案B更优，实际中 N 通常远大于 26。
//
// 已访问的单词直接从 set 中删除，等效于 visited 标记，
// 这样既避免重复访问，又省去额外的 visited 集合。
//
// 图结构示意 (示例1):
//
//   hit
//    |
//   hot
//   / \
//  dot  lot
//  |     |
//  dog  log
//   \  /
//   cog
//
// BFS 逐层扩展:
// Level 1: {hit}           depth=1
// Level 2: {hot}           depth=2
// Level 3: {dot, lot}      depth=3
// Level 4: {dog, log}      depth=4
// Level 5: {cog} ← 找到!  depth=5  → 返回 5
// ============================================================
class Solution1 {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());

        // 提前判断：endWord 不在字典中则无解
        if (!wordSet.count(endWord)) return 0;

        queue<string> q;
        q.push(beginWord);
        wordSet.erase(beginWord); // 防止回到起点

        int depth = 1; // beginWord 自身算第 1 个单词

        while (!q.empty()) {
            int size = q.size(); // 当前层的节点数

            for (int i = 0; i < size; i++) {
                string word = q.front();
                q.pop();

                // 对每个位置尝试替换成 a-z
                for (int j = 0; j < (int)word.size(); j++) {
                    char original = word[j];

                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;

                        word[j] = c;

                        // 找到终点 → 当前 depth 再 +1
                        if (word == endWord) return depth + 1;

                        // 新单词在字典中 → 入队并删除（标记已访问）
                        if (wordSet.count(word)) {
                            q.push(word);
                            wordSet.erase(word);
                        }
                    }

                    word[j] = original; // 恢复原字母，不能忘！
                }
            }

            depth++; // 一整层处理完，进入下一层
        }

        return 0; // BFS 结束仍未到达 endWord
    }
};

// ============================================================
// 解法2: 双向 BFS — 从两端同时搜索，在中间相遇
// 时间: O(N × L × 26) 理论相同，实际搜索空间大幅缩小
// 空间: O(N × L)
//
// 【思路】
// 单向 BFS 的搜索空间呈指数增长: O(b^d)
// 双向 BFS 从 beginWord 和 endWord 两端同时搜索:
//   搜索空间降为 O(2 × b^(d/2))，指数减半。
//
// 核心优化：每次扩展"当前较小"的一端（贪心减少搜索量）。
// 当扩展出的新单词出现在另一端的集合中时，说明相遇，
// 此时的总深度就是最短路径长度。
//
// 双向 BFS 过程示意:
//
//  front={hit}       back={cog}
//       |                |
//  front={hot}       back={cog}
//       |                |
//  front={dot,lot}   back={cog}
//       |                |
//  front={dot,lot}   back={dog,log}   ← back 更小，扩展 back
//       |                |
//  扩展 front: dot→dog   dog 在 back 中! → 相遇!
//  depth = 5
// ============================================================
class Solution2 {
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
            // 核心: 总是扩展较小的一端，减少搜索空间
            if (frontSet.size() > backSet.size()) {
                swap(frontSet, backSet);
            }

            unordered_set<string> nextSet; // 存放当前层扩展出的新节点

            for (const string& word : frontSet) {
                string temp = word;

                for (int j = 0; j < (int)temp.size(); j++) {
                    char original = temp[j];

                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;

                        temp[j] = c;

                        // 在另一端的集合中找到 → 两端相遇!
                        if (backSet.count(temp)) return depth + 1;

                        // 在字典中且未访问过 → 加入下一层
                        if (wordSet.count(temp)) {
                            nextSet.insert(temp);
                            wordSet.erase(temp); // 标记已访问
                        }
                    }

                    temp[j] = original; // 恢复原字母
                }
            }

            depth++;
            frontSet = nextSet; // 新一层替换当前前沿
            // 注意: backSet 保持不变（或在下一轮被选中扩展）
        }

        return 0; // 两端无法相遇
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 单向 BFS         | 双向 BFS              |
// |-------------|------------------|-----------------------|
// | 实现复杂度    | 简单(标准BFS模板) | 中等(两个集合+swap)    |
// | 渐近时间      | O(N×L×26)        | O(N×L×26)             |
// | 实际性能      | 基准              | 通常快 2-10 倍         |
// | 面试推荐      | ⭐ 先写这个       | 追问优化时给出          |
//
// 选择建议:
// - 面试先写单向 BFS，简洁不易出错
// - 如果面试官追问"能否更快"，再给出双向 BFS
// - 双向 BFS 的加速效果取决于图的分支因子和路径深度
//
// 【易错点】
//
// 1. 忘记提前检查 endWord 是否在 wordList 中
//    ✗ 直接开始 BFS
//    ✓ if (!wordSet.count(endWord)) return 0;
//
// 2. 返回值搞混：题目要"单词数目"，不是"变换次数"
//    ✗ 找到 endWord 时返回 depth（变换次数）
//    ✓ 返回 depth + 1（包含 beginWord 和 endWord）
//
// 3. 替换字母后忘记恢复原字母
//    ✗ word[j] = c; ... (循环到下一个 j 时 word 已经变了)
//    ✓ word[j] = c; ... word[j] = original; (每个位置处理完恢复)
//
// 4. 用 visited 集合但没有在入队时标记，而是出队时标记
//    ✗ 出队时才标记 visited → 同一个词可能被多次入队(浪费时间)
//    ✓ 入队时就标记(或从 wordSet 中删除)
//
// 5. 双向 BFS 中忘记从 wordSet 删除已处理节点
//    ✗ 只加入 nextSet，不删除 → 可能被另一端重复处理
//    ✓ wordSet.erase(temp) 确保全局只处理一次
//
// 【面试追问】
//
// Q1: 为什么用 BFS 而不是 DFS？
// → 无权图最短路径 = BFS。DFS 无法保证第一次找到的就是最短路径，
//   需要遍历所有路径取最小值，指数级时间。
//
// Q2: 找邻居为什么用"逐位替换26字母"而不是"遍历wordList比较"？
// → 逐位替换 O(26L)，遍历比较 O(NL)。
//   当 N > 26（通常如此）时逐位替换更优。
//   极端情况：L 极大且 N 极小时，遍历比较更好。
//
// Q3: 双向 BFS 为什么更快？为什么选择扩展较小的一端？
// → 搜索空间从 b^d 降到 2×b^(d/2)，指数减半。
//   选较小一端 = 贪心地在分支因子较小的方向扩展，
//   使两端的搜索前沿大小趋于平衡。
//
// Q4: 如果要输出所有最短路径（LeetCode 126）？
// → BFS 时记录每个单词的所有前驱（同一层的不同路径都保留），
//   注意同一层内不能提前删除（要等整层处理完再统一删除），
//   BFS 结束后从 endWord 回溯所有前驱，用 DFS 还原路径。
// ============================================================
