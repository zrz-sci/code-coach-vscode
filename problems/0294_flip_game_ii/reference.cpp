/*
 * 294. Flip Game II
 *
 * 【题目本质】
 * 组合博弈：先手是否存在必胜策略。
 * 核心：存在一步使对手必败 => 先手必胜。
 *
 * 【解法总览】
 * Solution1: 回溯 + 记忆化     O(2^n) / O(2^n)  ⭐推荐
 * Solution2: Sprague-Grundy    O(n^2) / O(n)    数学最优
 */

// ===================== Solution1: 回溯 + 记忆化 =====================
// 思路：枚举每步操作，如果对手在新状态下必败，则当前必胜。
//       用 HashMap 缓存已计算的状态。
class Solution1 {
public:
    bool canWin(string currentState) {
        unordered_map<string, bool> memo;
        return dfs(currentState, memo);
    }

private:
    bool dfs(string& s, unordered_map<string, bool>& memo) {
        if (memo.count(s)) return memo[s];

        for (int i = 0; i + 1 < (int)s.size(); i++) {
            if (s[i] == '+' && s[i + 1] == '+') {
                s[i] = s[i + 1] = '-';

                bool opponentWins = dfs(s, memo);

                s[i] = s[i + 1] = '+';

                if (!opponentWins) {
                    // 找到一步让对手必败，当前必胜
                    memo[s] = true;
                    return true;
                }
            }
        }

        // 所有操作对手都赢，或无操作可做 => 必败
        memo[s] = false;
        return false;
    }
};

// ===================== Solution2: Sprague-Grundy 定理 =====================
// 思路：按 '-' 分割字符串为独立子游戏（连续+段），
//       各段 SG 值 XOR 起来，非零则先手必胜。
// 关键公式：
//   翻转位置(i, i+1)后，长度 n 的段分裂为 i 和 n-i-2 两段
//   SG(n) = mex{ SG(i) ^ SG(n-i-2) : 0 <= i <= n-2 }
class Solution2 {
public:
    bool canWin(string currentState) {
        int grundy = 0;
        int count = 0;

        for (char c : currentState) {
            if (c == '+') {
                count++;
            } else {
                if (count >= 2) grundy ^= calcSG(count);
                count = 0;
            }
        }
        if (count >= 2) grundy ^= calcSG(count);

        return grundy != 0;
    }

private:
    unordered_map<int, int> cache;

    int calcSG(int n) {
        if (n <= 1) return 0;
        if (cache.count(n)) return cache[n];

        unordered_set<int> reachable;
        for (int i = 0; i + 1 < n; i++) {
            int left = i, right = n - i - 2;
            int sg = 0;
            if (left >= 2) sg ^= calcSG(left);
            if (right >= 2) sg ^= calcSG(right);
            reachable.insert(sg);
        }

        // mex: minimum excludant
        int mex = 0;
        while (reachable.count(mex)) mex++;

        return cache[n] = mex;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 回溯+memo | O(2^n) | O(2^n) | 直觉好懂，面试首选 |
 * | Solution2 SG定理 | O(n^2) | O(n) | 数学最优，适合follow-up |
 *
 * 【易错点】
 * 1. 逻辑反转：对手输了(!canWin)才是自己赢，初学者常搞反
 * 2. 忘记记忆化：同一状态可从多条路径到达，不缓存必超时
 * 3. 原地修改后 memo key：存入memo时必须是操作前的状态
 * 4. SG分段：循环结束后别忘处理最后一段连续'+'
 *
 * 【面试追问】
 * Q1: 不同操作顺序能否到达相同状态？-> 能，所以需要memo
 * Q2: SG定理的mex是什么？-> 不在后继SG值集合中的最小非负整数
 * Q3: 如果规则改为翻转3个连续+？-> 修改操作和SG递推公式即可
 */
