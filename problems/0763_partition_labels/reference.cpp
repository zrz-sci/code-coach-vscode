/*
 * LeetCode 763: Partition Labels (划分字母区间)
 *
 * 【题目本质】
 * 每个字母的出现范围形成一个区间[first, last]，要求所有重叠区间合并后，
 * 片段数最多。本质是区间合并问题。
 *
 * 【解法总览】
 * 解法1: 区间合并 — O(n) / O(1) — 显式构造区间，排序合并（通用思路）
 * 解法2: 贪心扫描 — O(n) / O(1) — 只记录最后出现位置，一次扫描搞定（面试首选）
 */

// ============================================================
// 解法1: 区间合并 — 显式构造每个字母的区间后合并
// 时间: O(n + 26 log 26) ≈ O(n)   空间: O(26) ≈ O(1)
//
// 【思路】
// 把每个字母看成一个区间 [首次出现, 最后出现]。
// 如果两个字母的区间有重叠，它们必须在同一个片段中。
// 问题转化为：合并所有重叠区间，返回每个合并后区间的长度。
//
// 可视化（示例2: "eccbbbbdec"）:
// 字母 e: [0, 8]  ████████░
// 字母 c: [1, 9]  ░████████
// 字母 b: [3, 6]  ░░░████░░
// 字母 d: [7, 7]  ░░░░░░░█░
//
// 排序后按起点: e[0,8], c[1,9], b[3,6], d[7,7]
// 合并: [0,8] + [1,9] → [0,9] + [3,6] → [0,9] + [7,7] → [0,9]
// 结果: 一个区间长度 10 → [10]
// ============================================================
class Solution1 {
public:
    vector<int> partitionLabels(string s) {
        // Step 1: 记录每个字母的首次和最后出现位置
        int first[26], last[26];
        memset(first, -1, sizeof(first));
        memset(last, -1, sizeof(last));

        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            if (first[c] == -1) first[c] = i;
            last[c] = i;
        }

        // Step 2: 收集出现过的字母的区间
        vector<pair<int,int>> intervals;
        for (int i = 0; i < 26; i++) {
            if (first[i] != -1) {
                intervals.push_back({first[i], last[i]});
            }
        }

        // Step 3: 按区间起点排序
        sort(intervals.begin(), intervals.end());

        // Step 4: 经典区间合并
        vector<int> result;
        int curStart = intervals[0].first;
        int curEnd = intervals[0].second;

        for (int i = 1; i < (int)intervals.size(); i++) {
            if (intervals[i].first <= curEnd) {
                // 重叠 → 合并，扩展右边界
                curEnd = max(curEnd, intervals[i].second);
            } else {
                // 不重叠 → 当前段结束，记录长度
                result.push_back(curEnd - curStart + 1);
                curStart = intervals[i].first;
                curEnd = intervals[i].second;
            }
        }
        // 别忘了最后一段！（经典易错点）
        result.push_back(curEnd - curStart + 1);

        return result;
    }
};

// ============================================================
// 解法2: 贪心扫描 — 只记录最后出现位置，一次扫描确定切割点
// 时间: O(n)  空间: O(26) ≈ O(1)
//
// 【思路】
// 解法1需要构造区间 + 排序 + 合并，步骤多。
// 关键观察：字符串从左到右扫描时，字母天然按"首次出现"的顺序排列，
// 不需要显式排序！只需要知道每个字母最后出现在哪里。
//
// 维护 end = 当前片段的最远右边界。
// 每遇到一个字符 c，更新 end = max(end, lastIndex[c])。
// 当 i == end 时，说明 [start, end] 内所有字母都不会在 end 之后出现，
// 可以安全切割。
//
// 贪心正确性：在最早能切的地方切，保证片段数最多。
// （推迟切割只会合并更多内容，不会创造新的切割机会）
//
// 可视化（示例1: "ababcbacadefegdehijhklij"）:
//
//  索引: 0  1  2  3  4  5  6  7  8 | 9 10 11 12 13 14 15 |16 17 18 19 20 21 22 23
//  字符: a  b  a  b  c  b  a  c  a | d  e  f  e  g  d  e | h  i  j  h  k  l  i  j
//  end:  8  8  8  8  8  8  8  8  8 |14 15 15 15 15 15 15 |19 22 23 23 23 23 23 23
//                                ↑                     ↑                          ↑
//                            i==end                i==end                     i==end
//                          切! len=9             切! len=7                  切! len=8
// ============================================================
class Solution2 {
public:
    vector<int> partitionLabels(string s) {
        // Step 1: 一次遍历，记录每个字母最后出现的位置
        int lastIndex[26] = {};
        for (int i = 0; i < (int)s.size(); i++) {
            lastIndex[s[i] - 'a'] = i;
        }

        // Step 2: 从左到右扫描，贪心确定切割点
        vector<int> result;
        int start = 0, end = 0;

        for (int i = 0; i < (int)s.size(); i++) {
            // 当前字符可能把片段的右边界推更远
            end = max(end, lastIndex[s[i] - 'a']);

            // i 追上了 end → 当前片段内所有字母都已"闭合"
            if (i == end) {
                result.push_back(end - start + 1);
                start = i + 1;  // 注意：下一段从 i+1 开始，不是 i
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1: 区间合并       | 解法2: 贪心扫描       |
// |-----------|----------------------|----------------------|
// | 时间       | O(n)                 | O(n)                 |
// | 空间       | O(1)                 | O(1)                 |
// | 代码量     | ~30行                | ~15行                |
// | 思维模型   | 通用区间合并模板       | 针对此题的贪心特化     |
// | 面试推荐   | 口述思路时提及         | ⭐ 实际编码首选        |
//
// 面试策略：先说"本质是区间合并"展示洞察力，再写解法2展示简洁编码能力。
//
// 【易错点】
//
// 1. start 更新错误:
//    ✗ start = i;      → 切割点被包含到下一段，长度计算错误
//    ✓ start = i + 1;  → 下一段从切割点的下一个位置开始
//
// 2. 解法1忘记处理最后一段:
//    ✗ 循环结束后直接 return result; → 丢失最后一个合并区间
//    ✓ 循环外追加 result.push_back(curEnd - curStart + 1);
//
// 3. lastIndex 初始化:
//    ✗ 用 memset(lastIndex, -1, sizeof(lastIndex)) 然后后面不检查
//    ✓ 初始化为 0 就行，因为遍历会覆盖所有出现过的字母，
//      而未出现的字母不会在 s 中被访问到
//
// 【面试追问】
//
// Q1(基础): 为什么只需要 lastIndex 不需要 firstIndex？
// → 从左到右扫描天然按首次出现顺序遇到字母，firstIndex 隐含在扫描过程中。
//
// Q2(正确性): 如何证明"尽早切割"是最优的？
// → 反证法：如果在可切处不切而延后，[i+1, j] 段的字母都首次出现在 i+1 后
//   （否则 end 不会停在 i），所以它们可以独立成段。延后切只会减少片段数。
//
// Q3(变体): 如果要返回划分的子字符串列表而不是长度列表？
// → 把 push_back(end - start + 1) 改成 push_back(s.substr(start, end-start+1))
//
// Q4(关联): 和 LeetCode 56 合并区间的关系？
// → 本题解法1 就是"从字符串提取区间 + 56 题合并"。解法2 是利用了字符串
//   天然有序的性质，省去了排序步骤。
// ============================================================
