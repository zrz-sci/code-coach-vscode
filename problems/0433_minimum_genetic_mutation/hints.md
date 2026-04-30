# 433. Minimum Genetic Mutation

## 核心思路
将每个基因序列看作图中的一个节点，两个序列之间如果仅差一个字符就连一条边。问题转化为 **从 startGene 到 endGene 的最短路径**，使用 **BFS** 求解。

## 思维链
1. 基因序列长度固定为 8，字符集为 {A, C, G, T}
2. 一次突变 = 改变一个字符 = 图中走一步
3. 合法突变必须在 bank 中 → bank 就是合法节点集合
4. 求最少突变次数 = 最短路径 → BFS
5. 优化：bank 很小（≤10），可以直接枚举 bank 中的邻居

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ BFS + 逐字符替换 | O(B * 8 * 4) | O(B) | 经典写法 |
| BFS + 枚举 bank 邻居 | O(B^2 * 8) | O(B) | 利用 bank 很小的特点 |

> B = bank.length (≤ 10)

## 关键提示
1. **bank 放入 set**：快速判断某个序列是否合法，同时当作 visited（访问后删除）
2. **两种邻居生成方式**：(a) 枚举每个位置 × 4 种字符；(b) 直接遍历 bank 找只差 1 个字符的
3. **endGene 必须在 bank 中**：否则直接返回 -1
4. **startGene 不一定在 bank 中**：但它是合法起点

## 解法详解

### 解法一：BFS + 逐字符替换 ⭐

**思路**：从 startGene 出发 BFS，每一步尝试将 8 个位置分别替换为 A/C/G/T，如果替换后的序列在 bank 中则入队。

```cpp
int minMutation(string startGene, string endGene, vector<string>& bank) {
    unordered_set<string> bankSet(bank.begin(), bank.end());
    if (!bankSet.count(endGene)) return -1;
    
    queue<string> q;
    q.push(startGene);
    unordered_set<string> visited;
    visited.insert(startGene);
    int steps = 0;
    string chars = "ACGT";
    
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            string cur = q.front(); q.pop();
            if (cur == endGene) return steps;
            for (int j = 0; j < 8; j++) {
                char old = cur[j];
                for (char c : chars) {
                    cur[j] = c;
                    if (bankSet.count(cur) && !visited.count(cur)) {
                        visited.insert(cur);
                        q.push(cur);
                    }
                }
                cur[j] = old;
            }
        }
        steps++;
    }
    return -1;
}
```

**复杂度**：时间 O(B * 8 * 4)，空间 O(B)

### 解法二：BFS + 枚举 bank 邻居

**思路**：不逐字符替换，直接遍历 bank 找与当前序列只差 1 个字符的邻居。

```cpp
int minMutation(string startGene, string endGene, vector<string>& bank) {
    unordered_set<string> bankSet(bank.begin(), bank.end());
    if (!bankSet.count(endGene)) return -1;
    
    queue<string> q;
    q.push(startGene);
    unordered_set<string> visited;
    visited.insert(startGene);
    int steps = 0;
    
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            string cur = q.front(); q.pop();
            if (cur == endGene) return steps;
            for (const string& gene : bank) {
                if (!visited.count(gene) && isDiffOne(cur, gene)) {
                    visited.insert(gene);
                    q.push(gene);
                }
            }
        }
        steps++;
    }
    return -1;
}
// isDiffOne: 比较两个字符串是否恰好差 1 个字符
```

## 易错点
- ✗ 忘记检查 endGene 是否在 bank 中 → ✓ 开头判断 `!bankSet.count(endGene)` 返回 -1
- ✗ 用 bank 当 visited，但 startGene 可能不在 bank 中导致重复访问 → ✓ 用独立的 visited set
- ✗ 替换字符后忘记还原 → ✓ 每次替换后 `cur[j] = old` 恢复原字符

## 面试追问

**Q1: 这题和 Word Ladder (127) 有什么异同？**
本质相同，都是 BFS 求最短变换路径。区别：基因序列固定 8 位、字符集只有 4 种（Word Ladder 是 26 种），bank 极小（≤10）而 wordList 可能很大。

**Q2: 如果 bank 非常大（10^5 级别），应该用哪种邻居生成方式？**
用逐字符替换（解法一），因为每个节点只生成 8*4=32 个候选。枚举 bank 邻居（解法二）会变成 O(B) 每步，太慢。

**Q3: 能否用双向 BFS 优化？**
可以，但 bank ≤ 10 时没必要。如果 bank 很大，双向 BFS 从 start 和 end 同时搜索，相遇时返回，能显著减少搜索空间。

## 相关题型
- [127. Word Ladder](https://leetcode.com/problems/word-ladder/) - 单词接龙（BFS 最短路径）
- [126. Word Ladder II](https://leetcode.com/problems/word-ladder-ii/) - 所有最短变换序列
- [752. Open the Lock](https://leetcode.com/problems/open-the-lock/) - BFS 状态搜索
