/*
 * 【题目本质】
 * 图的最短路径：基因序列为节点，差一个字符的合法序列间连边，BFS 求最短步数
 *
 * 【解法总览】
 * 解法1: BFS + 逐字符替换 ⭐ — O(B*32) / O(B) — 经典写法
 * 解法2: BFS + 枚举 bank 邻居 — O(B^2*8) / O(B) — 利用 bank 极小
 */

// ============================================================
// 解法1: BFS + 逐字符替换 ⭐
// 思路：每步将 8 个位置分别替换为 A/C/G/T，检查是否在 bank 中
// 时间 O(B * 32)，空间 O(B)，B = bank.length
// ============================================================
class Solution1 {
public:
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
};

// ============================================================
// 解法2: BFS + 枚举 bank 邻居
// 思路：直接遍历 bank 找与当前序列只差 1 字符的邻居
// 时间 O(B^2 * 8)，空间 O(B)
// ============================================================
class Solution2 {
public:
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
                    if (!visited.count(gene)) {
                        int diff = 0;
                        for (int j = 0; j < 8 && diff <= 1; j++) {
                            if (cur[j] != gene[j]) diff++;
                        }
                        if (diff == 1) {
                            visited.insert(gene);
                            q.push(gene);
                        }
                    }
                }
            }
            steps++;
        }
        return -1;
    }
};

/*
 * 【解法对比】
 * 解法1: 通用模板，适用于 bank 很大的场景（每步 O(32) 候选）
 * 解法2: 利用 bank ≤ 10 的约束，直接枚举更直观
 * 两者在本题约束下性能无差别
 *
 * 【易错点】
 * 1. 必须先检查 endGene 是否在 bank 中
 * 2. startGene 可能不在 bank 中但仍是合法起点
 * 3. 逐字符替换后必须恢复原字符
 *
 * 【面试追问】
 * Q: 与 Word Ladder 的区别？ A: 字符集 4 vs 26，bank ≤ 10 vs wordList 很大
 * Q: bank 很大时哪种更优？ A: 解法1，每步 O(8*4) vs O(B*8)
 * Q: 能否双向 BFS？ A: 可以，但 bank ≤ 10 时无必要
 */
