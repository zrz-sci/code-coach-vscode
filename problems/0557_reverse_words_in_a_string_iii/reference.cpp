// 【题目本质】
// 保持单词顺序不变，反转每个单词内部的字符。
// 核心：定位单词边界 + 区间反转。

// 【解法总览】
// Solution1: 双指针原地反转 — O(n) 时间，O(1) 空间 ⭐
// Solution2: istringstream 分割重组 — O(n) 时间，O(n) 空间

// ===================== Solution1: 双指针原地反转 =====================
// 思路：用 i 标记单词起点，j 扫描到空格或末尾时反转 [i, j) 区间
class Solution1 {
public:
    string reverseWords(string s) {
        int n = s.size();
        int i = 0;
        for (int j = 0; j <= n; j++) {
            if (j == n || s[j] == ' ') {
                reverse(s.begin() + i, s.begin() + j);
                i = j + 1;
            }
        }
        return s;
    }
};

// ===================== Solution2: istringstream 分割 =====================
// 思路：用流提取每个单词，反转后拼接
class Solution2 {
public:
    string reverseWords(string s) {
        istringstream iss(s);
        string word, result;
        while (iss >> word) {
            reverse(word.begin(), word.end());
            if (!result.empty()) result += ' ';
            result += word;
        }
        return result;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 优势 |
// |------|------|------|------|
// | Solution1 双指针 | O(n) | O(1) | 原地操作，最优 |
// | Solution2 流分割 | O(n) | O(n) | 代码简洁易读 |

// 【易错点】
// 1. j 循环到 n（不是 n-1），否则最后一个单词不会被反转
// 2. reverse(begin+i, begin+j) 是左闭右开区间，不需要 +1
// 3. 题目保证无前后空格且单词间恰好一个空格，不需额外处理

// 【面试追问】
// Q1: 反转单词顺序而非单词内容？→ 整体reverse + 逐词reverse（LC 151）
// Q2: 有多个连续空格？→ 双指针仍然正确，i = j+1 跳过空格
// Q3: 手写 reverse？→ while(l<r) swap(s[l++], s[r--])
