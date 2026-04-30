/*
 * 411. Minimum Unique Word Abbreviation
 *
 * 【题目本质】
 * 找 target 的最短缩写使其不是字典中任何等长单词的缩写。
 * 核心考点：位掩码表示缩写 + 集合覆盖 + 缩写长度计算。
 *
 * 【解法总览】
 * Solution1: 位掩码暴力枚举   — O(2^m * n) 时间
 * Solution2: 按 popcount 分层 + Gosper's hack 优化
 */

// ===================== Solution 1: 位掩码暴力枚举 =====================
// 思路：枚举 target 所有 2^m 种 mask，检查是否能区分所有字典词
//       mask 中 bit=1 表示保留字符，bit=0 表示用数字替换
// 时间：O(2^m * n) | 空间：O(n)
class Solution1 {
    // 计算 mask 对应的缩写长度
    int abbrLen(int mask, int m) {
        int len = 0;
        for (int i = 0; i < m; ) {
            if (mask & (1 << i)) {
                len++;  // 保留的字符
                i++;
            } else {
                len++;  // 一个数字段
                while (i < m && !(mask & (1 << i))) i++;
            }
        }
        return len;
    }

    // 根据 mask 生成缩写字符串
    string buildAbbr(const string& target, int mask, int m) {
        string res;
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) {
                if (cnt > 0) { res += to_string(cnt); cnt = 0; }
                res += target[i];
            } else {
                cnt++;
            }
        }
        if (cnt > 0) res += to_string(cnt);
        return res;
    }

public:
    string minAbbreviation(string target, vector<string>& dictionary) {
        int m = target.size();

        // 只保留等长词，计算 diff mask
        vector<int> diffs;
        for (auto& w : dictionary) {
            if ((int)w.size() != m) continue;
            int d = 0;
            for (int i = 0; i < m; i++)
                if (target[i] != w[i]) d |= (1 << i);
            diffs.push_back(d);
        }
        if (diffs.empty()) return to_string(m);

        int bestMask = (1 << m) - 1, bestLen = m;
        for (int mask = 0; mask < (1 << m); mask++) {
            // 检查 mask 是否能区分所有字典词
            bool valid = true;
            for (int d : diffs) {
                if ((mask & d) == 0) { valid = false; break; }
            }
            if (valid) {
                int len = abbrLen(mask, m);
                if (len < bestLen) {
                    bestLen = len;
                    bestMask = mask;
                }
            }
        }
        return buildAbbr(target, bestMask, m);
    }
};

// ===================== Solution 2: Gosper's hack 优化 =====================
// 思路：按 popcount 从小到大枚举 mask，使用 Gosper's hack 高效生成
//       同 popcount 的所有 mask，找到合法的即可提前返回
// 时间：O(2^m * n) 最坏 | 空间：O(n)
class Solution2 {
    int abbrLen(int mask, int m) {
        int len = 0;
        for (int i = 0; i < m; ) {
            if (mask & (1 << i)) { len++; i++; }
            else { len++; while (i < m && !(mask & (1 << i))) i++; }
        }
        return len;
    }

    string buildAbbr(const string& target, int mask, int m) {
        string res;
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) {
                if (cnt > 0) { res += to_string(cnt); cnt = 0; }
                res += target[i];
            } else cnt++;
        }
        if (cnt > 0) res += to_string(cnt);
        return res;
    }

public:
    string minAbbreviation(string target, vector<string>& dictionary) {
        int m = target.size();
        vector<int> diffs;
        for (auto& w : dictionary) {
            if ((int)w.size() != m) continue;
            int d = 0;
            for (int i = 0; i < m; i++)
                if (target[i] != w[i]) d |= (1 << i);
            diffs.push_back(d);
        }
        if (diffs.empty()) return to_string(m);

        int bestMask = (1 << m) - 1, bestLen = m;

        for (int bits = 0; bits <= m; bits++) {
            if (bits == 0) {
                // mask = 0: 全数字
                bool valid = true;
                for (int d : diffs)
                    if (d == 0) { valid = false; break; }
                // d 不会是 0（题目保证 target 不在字典中且只看等长词）
                // 但 mask=0 只有一段数字，无法区分等长词
                // mask & d == 0 对所有 d，所以 invalid
                continue;
            }
            int mask = (1 << bits) - 1;
            while (mask < (1 << m)) {
                bool valid = true;
                for (int d : diffs)
                    if ((mask & d) == 0) { valid = false; break; }
                if (valid) {
                    int len = abbrLen(mask, m);
                    if (len < bestLen) { bestLen = len; bestMask = mask; }
                }
                // Gosper's hack
                int c = mask & -mask;
                int r = mask + c;
                mask = (((r ^ mask) >> 2) / c) | r;
            }
            if (bestLen <= bits + 1) break;
        }
        return buildAbbr(target, bestMask, m);
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 核心技巧 | 优势 |
 * |------|------|---------|------|
 * | 暴力枚举 | O(2^m * n) | 位掩码 | 实现简单 |
 * | Gosper | O(2^m * n) | 按popcount分层 | 可提前终止 |
 *
 * 【易错点】
 * 1. 只对等长词计算 diff，不等长不会冲突
 * 2. 缩写长度 != popcount，连续 0 段只算一个数字
 * 3. mask & diff != 0 才合法（至少一个保留位能区分）
 * 4. diff=0 不会出现（题目保证 target 不在字典中）
 *
 * 【面试追问】
 * Q1: 位掩码表示缩写的巧妙之处？=> 将字符串操作转为位运算
 * Q2: Gosper's hack 原理？=> c=x&-x 找最低1, r=x+c 进位，重排剩余位
 * Q3: m>21 怎么办？=> 需要启发式搜索或 ILP
 */
