// 解法1: 标志位扫描 - seenDigit/seenDot/seenE 三标志追踪
// 时间 O(n), 空间 O(1)
// 核心: 逐字符判断，遇到 e 时重置 seenDigit 确保 e 后有数字
class Solution1 {
public:
    bool isNumber(string s) {
        bool seenDigit = false, seenDot = false, seenE = false;

        for (int i = 0; i < (int)s.size(); i++) {
            char c = s[i];

            if (isdigit(c)) {
                // 数字总是合法的，标记已见数字
                seenDigit = true;
            } else if (c == '+' || c == '-') {
                // 符号只能在开头 或 e/E 后面
                // 例: "+3.14" 合法, "3e+7" 合法, "3+7" 非法
                if (i > 0 && s[i-1] != 'e' && s[i-1] != 'E') return false;
            } else if (c == '.') {
                // 小数点不能出现两次，不能在 e 后面
                // 例: "3.14" 合法, "3.1.4" 非法, "1e2.5" 非法
                if (seenDot || seenE) return false;
                seenDot = true;
            } else if (c == 'e' || c == 'E') {
                // e 前面必须有数字，e 不能出现两次
                // 例: "2e10" 合法, "e3" 非法(e前无数字), "1ee2" 非法
                if (seenE || !seenDigit) return false;
                seenE = true;
                // 关键: 重置 seenDigit，确保 e 后面还有数字
                // 如果不重置, "1e" 会被误判为合法
                seenDigit = false;
            } else {
                // 非法字符: 字母(非e/E)等
                return false;
            }
        }

        // 最终必须看到过数字(注意 e 后已重置，所以这也确保了 e 后有数字)
        return seenDigit;
    }
};

// 解法2: DFA 确定性有限状态自动机 - 9个状态精确转移
// 时间 O(n), 空间 O(1)
// 核心: 明确定义每个状态和转移规则
//
// 状态定义:
//   0 = 起始状态
//   1 = 已读符号(+/-)
//   2 = 已读整数部分的数字 (如 "123" 中读完数字)
//   3 = 整数后接点 (如 "4." -> 合法终态)
//   4 = 符号后或起始后直接接点 (如 "+." -> 还需后续数字)
//   5 = 小数部分有数字 (如 "3.14", ".9" -> 合法终态)
//   6 = 已读 e/E (如 "2e" -> 需要后续数字)
//   7 = e/E 后接符号 (如 "2e+" -> 需要后续数字)
//   8 = e/E 后接数字 (如 "2e10" -> 合法终态)
//
// 状态转移表:
//           digit   +/-    .     e/E   other
// State 0:   2      1      4      -      -
// State 1:   2      -      4      -      -
// State 2:   2      -      3      6      -
// State 3:   5      -      -      6      -
// State 4:   5      -      -      -      -
// State 5:   5      -      -      6      -
// State 6:   8      7      -      -      -
// State 7:   8      -      -      -      -
// State 8:   8      -      -      -      -
//
// 合法终态: {2, 3, 5, 8}
class Solution2 {
public:
    bool isNumber(string s) {
        int state = 0;

        for (char c : s) {
            if (isdigit(c)) {
                // 数字输入: 进入对应数字状态
                if (state == 0 || state == 1 || state == 2) state = 2;      // -> 整数数字
                else if (state == 3 || state == 4 || state == 5) state = 5;  // -> 小数数字
                else if (state == 6 || state == 7 || state == 8) state = 8;  // -> 指数数字
                else return false;
            } else if (c == '+' || c == '-') {
                // 符号输入: 只能在起始或 e 后
                if (state == 0) state = 1;       // 数字前的符号
                else if (state == 6) state = 7;   // e 后的符号
                else return false;
            } else if (c == '.') {
                // 小数点输入: 只能在起始/符号后(state 0,1) 或整数后(state 2)
                if (state == 0 || state == 1) state = 4;  // 点前无数字
                else if (state == 2) state = 3;             // 点前有数字
                else return false;
            } else if (c == 'e' || c == 'E') {
                // 指数符号: 只能在有数字/小数之后
                if (state == 2 || state == 3 || state == 5) state = 6;
                else return false;
            } else {
                // 非法字符
                return false;
            }
        }

        // 合法终态检查
        // State 2: 纯整数如 "123"
        // State 3: 整数后接点如 "4."
        // State 5: 完整小数如 ".9" 或 "3.14"
        // State 8: 指数完成如 "2e10"
        return state == 2 || state == 3 || state == 5 || state == 8;
    }
};

// 解法3: 分段解析法 - 按 e 分割后分别验证整数/小数
// 时间 O(n), 空间 O(n) (子串)
// 核心: 将问题拆分为验证 "整数或小数" 和 "整数" 两个子问题
class Solution3 {
public:
    bool isNumber(string s) {
        // 第一步: 找到 e/E 的位置
        int ePos = -1;
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == 'e' || s[i] == 'E') {
                if (ePos != -1) return false; // 多个 e，直接非法
                ePos = i;
            }
        }

        // 第二步: 分段验证
        if (ePos == -1) {
            // 没有 e，整体必须是合法的整数或小数
            return isDecimalOrInteger(s);
        } else {
            // 有 e，前半部分是整数/小数，后半部分必须是整数
            string before = s.substr(0, ePos);
            string after = s.substr(ePos + 1);
            return !before.empty() && !after.empty()
                   && isDecimalOrInteger(before) && isInteger(after);
        }
    }

private:
    // 验证是否为合法整数: [+/-] digit+
    bool isInteger(const string& s) {
        if (s.empty()) return false;
        int start = 0;
        if (s[0] == '+' || s[0] == '-') start = 1;
        if (start == (int)s.size()) return false; // 只有符号没有数字

        for (int i = start; i < (int)s.size(); i++) {
            if (!isdigit(s[i])) return false;
        }
        return true;
    }

    // 验证是否为合法小数或整数: [+/-] (digit+[.digit*] | .digit+)
    bool isDecimalOrInteger(const string& s) {
        if (s.empty()) return false;
        int start = 0;
        if (s[0] == '+' || s[0] == '-') start = 1;

        bool hasDot = false, hasDigit = false;
        for (int i = start; i < (int)s.size(); i++) {
            if (isdigit(s[i])) {
                hasDigit = true;
            } else if (s[i] == '.') {
                if (hasDot) return false; // 两个小数点
                hasDot = true;
            } else {
                return false; // 非法字符
            }
        }
        return hasDigit; // 至少有一个数字
    }
};

/*
 * 解法对比:
 * ┌────────────────────┬────────┬────────┬──────────────────────┐
 * │ 解法               │ 时间   │ 空间   │ 特点                 │
 * ├────────────────────┼────────┼────────┼──────────────────────┤
 * │ Solution1 标志位   │ O(n)   │ O(1)   │ 简洁，面试首选       │
 * │ Solution2 DFA      │ O(n)   │ O(1)   │ 系统化，CS功底展示   │
 * │ Solution3 分段解析 │ O(n)   │ O(n)   │ 逻辑清晰，易理解     │
 * └────────────────────┴────────┴────────┴──────────────────────┘
 *
 * 详细测试用例验证:
 *   "0"          -> true  (纯整数)
 *   "+3.14"      -> true  (带符号小数)
 *   "4."         -> true  (整数后接点)
 *   ".9"         -> true  (点后接数字)
 *   "2e10"       -> true  (整数+指数)
 *   "53.5e93"    -> true  (小数+指数)
 *   "-123.456e789" -> true (完整形式)
 *   "."          -> false (只有点)
 *   "e"          -> false (只有e)
 *   "1e"         -> false (e后无数字)
 *   "e3"         -> false (e前无数字)
 *   "99e2.5"     -> false (e后不能是小数)
 *   "--6"        -> false (双符号)
 *   "1a"         -> false (非法字符)
 *
 * 易错点:
 * 1. "4." 合法 -> 小数点后可以没有数字
 * 2. ".9" 合法 -> 小数点前可以没有数字
 * 3. "." 不合法 -> 小数点前后至少一边要有数字
 * 4. e 后面必须是整数(不能是小数)
 * 5. seenDigit 遇到 e 后必须重置为 false
 * 6. 符号只能在 index 0 或 e/E 后面
 *
 * 面试追问:
 * Q: 用正则?
 *    -> ^[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$
 * Q: DFA 多少状态?
 *    -> 9 个状态 (0-8)，4 个合法终态 {2,3,5,8}
 * Q: 如何扩展支持十六进制?
 *    -> 检测 "0x" 前缀，切换到十六进制模式
 * Q: 如何做错误提示?
 *    -> 在每个 return false 处记录位置和原因
 */
