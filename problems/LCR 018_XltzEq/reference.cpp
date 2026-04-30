/*
 * LCR 018: 验证回文串 (同主站 125. Valid Palindrome)
 *
 * 题意: 给定字符串, 只考虑字母和数字, 忽略大小写, 判断是否回文
 * 空字符串定义为有效回文串
 *
 * 解法: 双指针对撞
 * 时间: O(n)  空间: O(1)
 *
 * 两个指针从字符串两端向中间移动:
 * - 跳过非字母数字字符
 * - 转小写后比较
 * - 全部匹配 → 回文; 发现不匹配 → 非回文
 */

// ============================================================
// 解法一: 双指针对撞 (面试首选, O(1) 空间)
// ============================================================
class Solution {
public:
    bool isPalindrome(string s) {
        int left = 0, right = (int)s.size() - 1;

        while (left < right) {
            // 跳过左侧非字母数字字符
            // 注意: 必须检查 left < right, 防止越界
            while (left < right && !isalnum(s[left])) {
                left++;
            }
            // 跳过右侧非字母数字字符
            while (left < right && !isalnum(s[right])) {
                right--;
            }

            // 忽略大小写比较
            if (tolower(s[left]) != tolower(s[right])) {
                return false;
            }

            left++;
            right--;
        }

        // 所有有效字符对都匹配（或字符串有效长度 <= 1）
        return true;
    }
};

// ============================================================
// 解法二: 预处理 + 翻转比较 (O(n) 空间, 最简洁)
// ============================================================
class Solution_PreProcess {
public:
    bool isPalindrome(string s) {
        // Step 1: 提取所有字母/数字字符, 转小写
        string cleaned;
        for (char ch : s) {
            if (isalnum(ch)) {
                cleaned += tolower(ch);
            }
        }

        // Step 2: 翻转后比较
        string reversed = cleaned;
        reverse(reversed.begin(), reversed.end());
        return cleaned == reversed;
    }
};

// ============================================================
// 解法三: 预处理 + 双指针 (O(n) 空间)
// ============================================================
class Solution_PreProcessTP {
public:
    bool isPalindrome(string s) {
        // Step 1: 提取有效字符并转小写
        string cleaned;
        cleaned.reserve(s.size()); // 预分配避免多次扩容
        for (char ch : s) {
            if (isalnum(ch)) {
                cleaned += tolower(ch);
            }
        }

        // Step 2: 双指针判断 cleaned 是否回文
        int left = 0, right = (int)cleaned.size() - 1;
        while (left < right) {
            if (cleaned[left] != cleaned[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
};

// ============================================================
// 解法四: 不用库函数 (纯手动实现)
// ============================================================
class Solution_NoLib {
public:
    bool isPalindrome(string s) {
        int left = 0, right = (int)s.size() - 1;

        while (left < right) {
            while (left < right && !myIsAlnum(s[left])) left++;
            while (left < right && !myIsAlnum(s[right])) right--;

            if (myToLower(s[left]) != myToLower(s[right])) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }

private:
    // 手动判断是否为字母或数字
    bool myIsAlnum(char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               (c >= '0' && c <= '9');
    }

    // 手动转小写
    char myToLower(char c) {
        if (c >= 'A' && c <= 'Z') {
            return c + ('a' - 'A'); // 'A' + 32 = 'a'
        }
        return c;
    }
};

/*
 * 复杂度分析:
 *
 * 解法一 (双指针):
 *   时间: O(n) - 每个字符最多被访问一次
 *   空间: O(1) - 只用常数个变量
 *
 * 解法二 (预处理+翻转):
 *   时间: O(n) - 遍历 + 翻转 + 比较各 O(n)
 *   空间: O(n) - cleaned 和 reversed 各 O(n)
 *
 * 解法三 (预处理+双指针):
 *   时间: O(n)
 *   空间: O(n) - cleaned 字符串
 *
 * 解法四 (无库函数):
 *   时间: O(n)
 *   空间: O(1)
 *
 * 关键测试用例:
 *   "A man, a plan, a canal: Panama" → true
 *   "race a car"                     → false
 *   ""                               → true  (空串是回文)
 *   " "                              → true  (无有效字符)
 *   "0P"                             → false (数字0和字母P)
 *   "a"                              → true  (单字符)
 *   "aA"                             → true  (忽略大小写)
 *
 * 易错点:
 *   1. 内层 while 跳过无效字符时必须检查 left < right
 *   2. 比较时必须 tolower, 否则大小写不同会误判
 *   3. right 初始化: (int)s.size()-1, 注意 size_t 转 int
 *   4. isalnum 包括数字, 不能只用 isalpha
 *
 * 面试技巧:
 *   1. 先说思路: "双指针从两端向中间, 跳过无效字符"
 *   2. 提及边界: 空串、全符号串
 *   3. 提及 O(1) 空间优势
 *   4. Follow-up: 删一个字符(LC680), 最长回文子串(LC5)
 */

// ============================================================
// 【解法对比】
//
// | 解法               | 时间  | 空间  | 优势                         |
// |-------------------|------|------|----------------------------|
// | 双指针对撞 (解法一)  | O(n) | O(1) | 面试首选, 原地判断             |
// | 预处理+翻转 (解法二) | O(n) | O(n) | 代码最短最好写                |
// | 预处理+双指针 (解法三)| O(n) | O(n) | 两步分离, 逻辑清晰            |
// | 无库函数 (解法四)    | O(n) | O(1) | 展示底层实现能力              |
//
// 面试策略: 默认写解法一 (O(1) 空间)。
// 如果面试官要求快速实现, 解法二一行 reverse + 比较搞定。
//
// ============================================================
// 【易错点】
//
// 1. 内层 while 缺少 left < right 检查:
//    ✗ while (!isalnum(s[left])) left++;  → 全符号串越界
//    ✓ while (left < right && !isalnum(s[left])) left++;
//
// 2. 忘记 tolower:
//    ✗ s[left] != s[right]  → "Aa" 误判为 false
//    ✓ tolower(s[left]) != tolower(s[right])
//
// 3. isalnum vs isalpha:
//    ✗ 用 isalpha → 漏掉数字 '0'-'9'
//    ✓ 用 isalnum → 字母+数字都保留
//
// 4. size_t 隐式转换:
//    ✗ int right = s.size() - 1;  → 空串时 size()=0, 0-1 溢出成巨大正数
//    ✓ int right = (int)s.size() - 1;
//
// ============================================================
// 【面试追问】
//
// Q1: 如果允许最多删除一个字符, 怎么判断回文?
// → LC 680 Valid Palindrome II
//   当 s[left] != s[right] 时, 尝试跳过 left 或跳过 right,
//   检查剩余子串是否回文。两种跳法有一种成功即可。
//   时间 O(n), 空间 O(1)。
//
// Q2: 如何找到字符串中的最长回文子串?
// → LC 5 Longest Palindromic Substring
//   方法1: 中心扩展法 — 枚举每个(或每对)中心, 向两侧扩展,
//   时间 O(n^2), 空间 O(1)。
//   方法2: Manacher 算法 — O(n) 但面试极少考。
//
// Q3: 如果输入是 Unicode 字符串 (中文/emoji), 代码需要改什么?
// → isalnum/tolower 只处理 ASCII, 需要替换为 locale-aware 版本
//   或使用 ICU 库。C++ 中可用 <locale> + wstring/u32string,
//   注意 UTF-8 多字节编码不能直接按 char 索引。
// ============================================================
