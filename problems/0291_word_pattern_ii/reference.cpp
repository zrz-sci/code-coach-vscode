/*
 * 291. Word Pattern II
 *
 * 【题目本质】
 * 建立 pattern 字符到子串的双射映射，回溯搜索所有可能的分割方案。
 * 核心难点：分割点未知 + 双射约束 + 回溯。
 *
 * 【解法总览】
 * Solution1: 回溯 + 双HashMap双射  O(n^m) / O(m+n) ⭐推荐
 * Solution2: 回溯 + HashMap+Set     O(n^m) / O(m+n)
 */

// ===================== Solution1: 回溯 + 双HashMap双射 =====================
// 思路：维护 char->string 和 string->char 两个映射，保证双射。
//       递归处理每个 pattern 字符，枚举对应子串长度。
class Solution1 {
public:
    bool wordPatternMatch(string pattern, string s) {
        unordered_map<char, string> charToStr;
        unordered_map<string, char> strToChar;
        return backtrack(pattern, 0, s, 0, charToStr, strToChar);
    }

private:
    bool backtrack(const string& pattern, int pi,
                   const string& s, int si,
                   unordered_map<char, string>& charToStr,
                   unordered_map<string, char>& strToChar) {
        if (pi == pattern.size() && si == s.size()) return true;
        if (pi == pattern.size() || si == s.size()) return false;

        char c = pattern[pi];

        // 已有映射 -> 直接验证
        if (charToStr.count(c)) {
            const string& mapped = charToStr[c];
            if (s.compare(si, mapped.size(), mapped) != 0) return false;
            return backtrack(pattern, pi + 1, s, si + mapped.size(),
                             charToStr, strToChar);
        }

        // 无映射 -> 枚举子串长度
        int remainPattern = pattern.size() - pi - 1;
        for (int len = 1; si + len <= (int)s.size() - remainPattern; len++) {
            string sub = s.substr(si, len);

            // 双射检查：该子串不能已被其他字符占用
            if (strToChar.count(sub)) continue;

            charToStr[c] = sub;
            strToChar[sub] = c;

            if (backtrack(pattern, pi + 1, s, si + len,
                          charToStr, strToChar)) {
                return true;
            }

            charToStr.erase(c);
            strToChar.erase(sub);
        }

        return false;
    }
};

// ===================== Solution2: 回溯 + HashMap + Set =====================
// 思路：用 HashMap 记录正向映射，用 Set 记录已使用的子串防止重复。
class Solution2 {
public:
    bool wordPatternMatch(string pattern, string s) {
        unordered_map<char, string> map;
        unordered_set<string> used;
        return dfs(pattern, 0, s, 0, map, used);
    }

private:
    bool dfs(const string& p, int pi, const string& s, int si,
             unordered_map<char, string>& map,
             unordered_set<string>& used) {
        if (pi == p.size()) return si == s.size();
        if (si == s.size()) return false;

        char c = p[pi];

        // 已映射 -> 验证
        if (map.count(c)) {
            const string& w = map[c];
            if (s.compare(si, w.size(), w) != 0) return false;
            return dfs(p, pi + 1, s, si + w.size(), map, used);
        }

        // 未映射 -> 枚举
        for (int len = 1; si + len <= (int)s.size(); len++) {
            string sub = s.substr(si, len);
            if (used.count(sub)) continue;

            map[c] = sub;
            used.insert(sub);

            if (dfs(p, pi + 1, s, si + len, map, used)) return true;

            map.erase(c);
            used.erase(sub);
        }
        return false;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 双HashMap | O(n^m) | O(m+n) | 双射语义清晰，推荐 |
 * | Solution2 HashMap+Set | O(n^m) | O(m+n) | 用Set代替反向map |
 *
 * 【易错点】
 * 1. 忘记双射约束：只建正向映射会导致不同字符映射同一子串
 * 2. 不剪枝：剩余s长度不够分配给剩余pattern字符时应提前返回
 * 3. 回溯时忘记撤销映射
 * 4. 边界条件：pi和si必须同时到达末尾才算成功
 *
 * 【面试追问】
 * Q1: 与 Word Pattern (290) 区别？-> 290单词已分隔，O(n)；291需搜索分割点
 * Q2: 最坏时间复杂度？-> O(n^m)，每个pattern字符有n种选择
 * Q3: 如何进一步剪枝？-> 统计字符频次，约束子串长度上下界
 */
