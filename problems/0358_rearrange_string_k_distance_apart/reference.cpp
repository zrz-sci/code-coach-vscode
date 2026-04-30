/*
 * 【题目本质】
 * 重排字符串使相同字符之间距离至少为 k
 * 贪心策略：每次优先放置剩余频率最高的字符 + 冷却队列控制间距
 *
 * 【解法总览】
 * 解法1: 最大堆 + 冷却队列 — 经典贪心，O(N log26) 时间
 * 解法2: 贪心 + 每轮填充 k 个 — 无需堆，O(N * 26) 时间
 */

// ===================== Solution 1: 最大堆 + 冷却队列（推荐）=====================
// 思路：
// 1. 统计频率，所有字符入最大堆（按频率排序）
// 2. 每步从堆顶取频率最高的字符放入结果
// 3. 取出的字符进入冷却队列，等待 k 步后重新入堆
// 4. 当冷却队列长度 >= k 时，队头出队：若频率>0 则重新入堆
// 5. 堆空时若结果长度 != s长度 → 无法完成
//
// 贪心正确性：
// 优先消耗高频字符，避免高频字符在后期"堆积"导致无法间隔
//
// 时间：O(N log 26) ≈ O(N)   空间：O(26) = O(1)
class Solution {
public:
    string rearrangeString(string s, int k) {
        if (k <= 1) return s;  // 无间距约束

        // 1. 统计频率
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;

        // 2. 所有字符入最大堆
        priority_queue<pair<int,char>> pq;
        for (auto& [c, f] : freq) {
            pq.push({f, c});
        }

        string result;
        queue<pair<int,char>> cooldown;  // 冷却队列 (remainFreq, char)

        while (!pq.empty()) {
            // 取频率最高的字符
            auto [f, c] = pq.top();
            pq.pop();
            result += c;

            // 该字符进入冷却队列（频率-1）
            cooldown.push({f - 1, c});

            // 冷却期满（队列长度 = k），队头字符可以重新使用
            if ((int)cooldown.size() >= k) {
                auto [rf, rc] = cooldown.front();
                cooldown.pop();
                if (rf > 0) pq.push({rf, rc});
            }
        }

        // 验证是否完成
        return (int)result.size() == (int)s.size() ? result : "";
    }
};

// ===================== Solution 2: 贪心 + 每轮填充 k 个 =====================
// 思路：
// 每轮从频率最高的字符中取 k 个不同字符依次放入结果
// 如果可用字符种类 < k 且还有剩余字符需要放 → 无法完成
// 最后一轮可能不满 k 个（此时剩余字符数 < k，无间距约束冲突）
//
// 时间：O(N * 26)   空间：O(26) = O(1)
class Solution2 {
public:
    string rearrangeString(string s, int k) {
        if (k <= 1) return s;

        int n = s.size();
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;

        string result;

        while ((int)result.size() < n) {
            // 找当前频率最高的字符们
            vector<pair<int,int>> cands;  // (freq, charIdx)
            for (int i = 0; i < 26; i++) {
                if (freq[i] > 0) cands.push_back({freq[i], i});
            }
            // 按频率降序排序
            sort(cands.rbegin(), cands.rend());

            // 本轮需要填充 min(k, remaining) 个字符
            int fill = min(k, n - (int)result.size());

            // 可用字符种类不够 fill 个 → 无法满足间距约束
            if ((int)cands.size() < fill) return "";

            // 依次填入频率最高的 fill 个字符
            for (int i = 0; i < fill; i++) {
                int idx = cands[i].second;
                result += (char)('a' + idx);
                freq[idx]--;
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 堆+冷却队列 | O(N log26) | O(26) | 经典，面试首选 |
 * | 每轮填充k个 | O(N*26) | O(26) | 直观，无需堆 |
 *
 * 【易错点】
 * 1. k<=1 直接返回 s：k=0 无约束，k=1 允许相邻
 * 2. 冷却队列 size >= k 才弹出：不是 > k
 * 3. 最后一轮允许 fill < k：只要剩余字符数 < k 就行
 * 4. 验证结果长度：result.size() == s.size() 才算成功
 *
 * 【面试追问】
 * Q1: 如何在 O(N) 内预判是否可行？
 *     → 设最大频率 maxF，出现 maxF 次的字符数为 cnt
 *     → 可行条件：(maxF-1)*k + cnt <= n
 * Q2: 和 621 Task Scheduler 的关系？
 *     → 621 求最短时间（可以插 idle），本题求具体排列（不能插 idle）
 *     → 621 的答案 = max(n, (maxF-1)*(k+1) + cnt)
 * Q3: 如果要求输出所有可行排列？
 *     → 回溯法 + 剪枝，但指数级复杂度，面试中不常考
 */
