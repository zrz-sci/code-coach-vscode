/*
 * LeetCode 816: Ambiguous Coordinates
 *
 * 解法一：枚举逗号 + 枚举小数点 + 合法性判断（推荐）
 * 时间复杂度: O(n^3)
 * 空间复杂度: O(n^3) 用于存储结果
 *
 * 思路：
 * 1. 去括号得到数字串
 * 2. 枚举逗号位置，分成左右两部分
 * 3. 对每部分枚举小数点位置，生成所有合法数字表示
 * 4. 左右做笛卡尔积，格式化输出
 */

class Solution {
public:
    vector<string> ambiguousCoordinates(string s) {
        // 去掉括号
        string digits = s.substr(1, s.size() - 2);
        int n = digits.size();

        vector<string> result;

        // 枚举逗号位置：将 digits 分为 digits[0..i-1] 和 digits[i..n-1]
        for (int i = 1; i < n; ++i) {
            string leftStr = digits.substr(0, i);
            string rightStr = digits.substr(i);

            // 生成左右两侧的合法数字表示
            vector<string> leftCandidates = generate(leftStr);
            vector<string> rightCandidates = generate(rightStr);

            // 笛卡尔积
            for (const string& l : leftCandidates) {
                for (const string& r : rightCandidates) {
                    result.push_back("(" + l + ", " + r + ")");
                }
            }
        }

        return result;
    }

private:
    // 生成字符串 s 的所有合法数字表示
    // 合法规则：
    // 1. 整数不能有前导零（除了 "0" 本身）
    // 2. 小数的整数部分不能有前导零（除了 "0"）
    // 3. 小数的小数部分不能以零结尾
    vector<string> generate(const string& s) {
        int n = s.size();
        vector<string> candidates;

        // 情况1：不加小数点，作为纯整数
        // 合法条件：没有前导零，或者字符串就是 "0"
        if (isValidInteger(s)) {
            candidates.push_back(s);
        }

        // 情况2：在位置 dot 处插入小数点
        // s[0..dot-1] 为整数部分, s[dot..n-1] 为小数部分
        for (int dot = 1; dot < n; ++dot) {
            string intPart = s.substr(0, dot);
            string decPart = s.substr(dot);

            // 整数部分合法（无前导零或为 "0"）且小数部分不以零结尾
            if (isValidInteger(intPart) && isValidDecimal(decPart)) {
                candidates.push_back(intPart + "." + decPart);
            }
        }

        return candidates;
    }

    // 判断整数部分是否合法：不能有前导零（"0" 除外）
    bool isValidInteger(const string& s) {
        // "0" 合法，"01", "00" 不合法
        return s == "0" || s[0] != '0';
    }

    // 判断小数部分是否合法：不能以零结尾
    bool isValidDecimal(const string& s) {
        return s.back() != '0';
    }
};


/*
 * 解法二：更紧凑的写法（减少函数调用）
 * 时间复杂度: O(n^3)
 * 空间复杂度: O(n^3)
 *
 * 思路相同，将判断逻辑内联，减少代码量。
 * 面试时可以从这个版本开始，如果面试官要求拆分再重构。
 */

class SolutionCompact {
public:
    vector<string> ambiguousCoordinates(string s) {
        string d = s.substr(1, s.size() - 2);
        int n = d.size();
        vector<string> res;

        for (int i = 1; i < n; ++i) {
            // 枚举逗号位置
            auto left = make(d.substr(0, i));
            auto right = make(d.substr(i));

            for (auto& l : left)
                for (auto& r : right)
                    res.push_back("(" + l + ", " + r + ")");
        }
        return res;
    }

private:
    vector<string> make(string s) {
        int n = s.size();
        vector<string> res;

        for (int dot = 1; dot <= n; ++dot) {
            // dot == n 表示没有小数点（纯整数）
            string intP = s.substr(0, dot);
            string decP = s.substr(dot); // dot==n 时为空

            // 整数部分：长度 >1 时首字符不能是 '0'
            if (intP.size() > 1 && intP[0] == '0') continue;
            // 小数部分：不能以 '0' 结尾（空串视为合法，即纯整数情况）
            if (!decP.empty() && decP.back() == '0') continue;

            if (decP.empty()) {
                res.push_back(intP);
            } else {
                res.push_back(intP + "." + decP);
            }
        }
        return res;
    }
};


/*
 * 解法三：带剪枝优化
 * 时间复杂度: O(n^3) 最坏，实际更快
 * 空间复杂度: O(n^3)
 *
 * 优化点：
 * 1. 如果一侧没有合法表示，直接跳过
 * 2. 全零串的特殊处理
 * 3. 提前判断是否有前导零，避免不必要的枚举
 */

class SolutionOptimized {
public:
    vector<string> ambiguousCoordinates(string s) {
        string d = s.substr(1, s.size() - 2);
        int n = d.size();
        vector<string> res;

        for (int i = 1; i < n; ++i) {
            string leftStr = d.substr(0, i);
            string rightStr = d.substr(i);

            // 剪枝：快速判断是否可能有合法表示
            if (!canHaveValidRepresentation(leftStr) ||
                !canHaveValidRepresentation(rightStr)) {
                continue;
            }

            auto left = generate(leftStr);
            auto right = generate(rightStr);

            for (auto& l : left)
                for (auto& r : right)
                    res.push_back("(" + l + ", " + r + ")");
        }
        return res;
    }

private:
    // 快速判断一个字符串是否可能有至少一种合法数字表示
    bool canHaveValidRepresentation(const string& s) {
        // 全零串只有 "0" 一种表示，但必须长度为1
        // "00" -> 整数 "00" 不合法，小数 "0.0" 末尾零 -> 无合法表示
        if (s.size() > 1 && s[0] == '0' && s.back() == '0') {
            return false;
        }
        return true;
    }

    vector<string> generate(const string& s) {
        int n = s.size();
        vector<string> candidates;

        // 纯整数
        if (n == 1 || s[0] != '0') {
            candidates.push_back(s);
        }

        // 带小数点
        for (int dot = 1; dot < n; ++dot) {
            string intP = s.substr(0, dot);
            string decP = s.substr(dot);

            // 整数部分无前导零（或为 "0"）
            if (intP.size() > 1 && intP[0] == '0') continue;
            // 小数部分不以零结尾
            if (decP.back() == '0') continue;

            candidates.push_back(intP + "." + decP);
        }

        return candidates;
    }
};


/*
 * 测试用例分析：
 *
 * 输入: "(123)"  -> digits = "123"
 *   逗号位置1: "1" | "23"  -> ["1"] x ["23","2.3"] = ["(1, 23)","(1, 2.3)"]
 *   逗号位置2: "12" | "3"  -> ["12","1.2"] x ["3"]  = ["(12, 3)","(1.2, 3)"]
 *   结果: ["(1, 23)","(1, 2.3)","(12, 3)","(1.2, 3)"]
 *
 * 输入: "(0123)" -> digits = "0123"
 *   逗号位置1: "0" | "123" -> ["0"] x ["123","12.3","1.23"] = 3个
 *   逗号位置2: "01" | "23" -> ["0.1"] x ["23","2.3"]        = 2个
 *   逗号位置3: "012" | "3" -> ["0.12"] x ["3"]              = 1个
 *   结果: 6个坐标
 *
 * 输入: "(00011)" -> digits = "00011"
 *   逗号位置1: "0" | "0011"   -> ["0"] x ["0.011"] = ["(0, 0.011)"]
 *   逗号位置2: "00" | "011"   -> [] x [...]        = [] (00无合法表示)
 *   逗号位置3: "000" | "11"   -> [] x [...]        = [] (000无合法表示)
 *   逗号位置4: "0001" | "1"   -> ["0.001"] x ["1"] = ["(0.001, 1)"]
 *   结果: ["(0, 0.011)","(0.001, 1)"]
 */
