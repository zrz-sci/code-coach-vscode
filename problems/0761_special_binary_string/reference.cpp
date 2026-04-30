// LeetCode 761: Special Binary String
// 解法：递归分解 + 排序
// 时间复杂度：O(n^2)  空间复杂度：O(n)

// ===================== 解法一：递归分解 + 降序排序（推荐） =====================

class Solution {
public:
    string makeLargestSpecial(string s) {
        // 如果字符串长度 <= 2，无法进一步优化
        if (s.size() <= 2) return s;

        // 收集当前层的所有最小特殊子串
        vector<string> subs;
        int count = 0;   // 计数器：遇到 '1' 加1，遇到 '0' 减1
        int start = 0;   // 当前最小特殊子串的起始位置

        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == '1') {
                count++;
            } else {
                count--;
            }

            // 当 count 回到 0 时，找到了一个最小特殊子串
            if (count == 0) {
                // s[start..i] 是一个最小特殊子串
                // 形式为 "1" + inner + "0"
                // 其中 inner = s[start+1..i-1] 也是特殊二进制字符串

                // 递归处理内部
                string inner = makeLargestSpecial(s.substr(start + 1, i - start - 1));

                // 组装：外包 1...0
                subs.push_back("1" + inner + "0");

                // 更新下一个子串的起始位置
                start = i + 1;
            }
        }

        // 降序排序所有子串，使字典序最大
        sort(subs.begin(), subs.end(), greater<string>());

        // 拼接所有子串
        string result;
        for (const string& sub : subs) {
            result += sub;
        }

        return result;
    }
};


// ===================== 解法二：带详细注释的递归版 =====================
// 更详细地展示递归过程

class Solution2 {
public:
    string makeLargestSpecial(string s) {
        // 基本情况：空串或最短特殊串 "10"
        if (s.empty()) return "";
        if (s.size() == 2) return s;  // "10" 是最短的特殊二进制字符串

        vector<string> parts;
        int balance = 0;  // 类比括号深度
        int segStart = 0;

        // 第一步：在最外层找到所有「顶层括号对」
        // 类比：对于 "(())()(()())"，顶层括号对是 "(())", "()", "(()())"
        for (int i = 0; i < (int)s.size(); i++) {
            // '1' 相当于 '('，'0' 相当于 ')'
            balance += (s[i] == '1') ? 1 : -1;

            if (balance == 0) {
                // 找到一个完整的顶层括号对 s[segStart..i]
                // 例如 "1...0"

                // 第二步：递归处理内部
                // 内部 = s[segStart+1..i-1]，去掉外层的 1 和 0
                string innerResult = makeLargestSpecial(
                    s.substr(segStart + 1, i - segStart - 1)
                );

                // 第三步：重新包上外层的 1 和 0
                parts.push_back("1" + innerResult + "0");

                segStart = i + 1;
            }
        }

        // 第四步：降序排列所有顶层部分
        // 交换相邻特殊子串 = 对顶层部分任意排列
        // 字典序最大 = 降序排列
        sort(parts.begin(), parts.end(), [](const string& a, const string& b) {
            return a > b;
        });

        // 第五步：拼接结果
        string ans;
        for (const string& p : parts) {
            ans += p;
        }
        return ans;
    }
};


// ===================== 示例演算 =====================
//
// 输入：s = "11011000"
//
// 第一层递归 makeLargestSpecial("11011000"):
//   扫描 count 变化：1,2,1,2,1,0  （整个串是一个顶层括号对）
//   内部 = "101100"
//   递归 makeLargestSpecial("101100"):
//
//     第二层递归 makeLargestSpecial("101100"):
//       扫描 count 变化：
//         i=0: '1', count=1
//         i=1: '0', count=0  -> 子串 "10", 内部=""
//         i=2: '1', count=1
//         i=3: '1', count=2
//         i=4: '0', count=1
//         i=5: '0', count=0  -> 子串 "1100", 内部="10"
//
//       递归 makeLargestSpecial("") -> ""
//       子串1: "1" + "" + "0" = "10"
//
//       递归 makeLargestSpecial("10") -> "10" (长度2，直接返回)
//       子串2: "1" + "10" + "0" = "1100"
//
//       parts = ["10", "1100"]
//       降序排序: ["1100", "10"]
//       拼接: "110010"
//       返回 "110010"
//
//   回到第一层：
//   parts = ["1" + "110010" + "0"] = ["11100100"]
//   只有一个部分，无需排序
//   返回 "11100100"
//
// 输出: "11100100"  (正确!)


// ===================== 关键细节说明 =====================
//
// 1. 特殊二进制字符串 vs 合法括号序列：
//    - '1' = '('  开始
//    - '0' = ')'  结束
//    - 0和1数量相等 = 左右括号数量相等
//    - 每个前缀1 >= 0 = 每个前缀左括号 >= 右括号
//
// 2. 递归结构：
//    - 最小特殊子串 = 不可分割的顶层括号对
//    - 去掉首尾后的内部 = 嵌套的括号内容
//    - 递归处理内部，再包上外层
//
// 3. 降序排序的正确性：
//    - 同一层的特殊子串可以任意排列（通过相邻交换）
//    - 字典序最大 = 降序排列
//    - 例如 ["1100", "10"] 降序 -> "110010" > "101100"
//
// 4. 复杂度在约束内完全够用：
//    - s.length <= 50
//    - O(n^2) 最坏情况约 2500 次操作
//    - 实际递归深度通常远小于 n
