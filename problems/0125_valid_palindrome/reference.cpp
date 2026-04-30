/*
 * LeetCode 125: 验证回文串 (Valid Palindrome)
 *
 * 【题目本质】
 * 忽略非字母数字字符、忽略大小写后，判断字符串是否左右对称。
 *
 * 【解法总览】
 * 解法1: 预处理 + 反转比较 — O(n) / O(n) — 最直觉
 * 解法2: 预处理 + 双指针   — O(n) / O(n) — 省去反转
 * 解法3: 原地双指针       — O(n) / O(1) — 最优，面试首选
 */

// ============================================================
// 解法1: 预处理 + 反转比较 — 最直觉的方式
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最自然的想法：先把字符串"清洗"干净（去掉非字母数字、统一小写），
// 得到一个纯净的字符串，然后反转它，看反转前后是否相同。
// 如果相同，就是回文。
//
// 示例: "A man, a plan, a canal: Panama"
//  清洗后: "amanaplanacanalpanama"
//  反转后: "amanaplanacanalpanama" → 相同 → true
// ============================================================
class Solution1 {
public:
    bool isPalindrome(string s) {
        string cleaned;
        for (char c : s) {
            if (isalnum(c)) {
                cleaned += tolower(c);  // 只保留字母数字，统一转小写
            }
        }
        string reversed = cleaned;
        reverse(reversed.begin(), reversed.end());
        return cleaned == reversed;     // 反转后相同 → 回文
    }
};

// ============================================================
// 解法2: 预处理 + 双指针 — 省去反转步骤
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1 反转整个字符串其实多此一举。
// 回文的定义就是"首尾对称"，只需要用两个指针从两端向中间走，
// 逐个比较即可。一旦发现不匹配就返回 false。
//
// 比较过程 (cleaned = "amanaplanacanalpanama"):
//   L=0('a')  R=20('a')  → 匹配 ✓
//   L=1('m')  R=19('m')  → 匹配 ✓
//   ...
//   L=10('c') R=10('c')  → L==R, 循环结束 → true
// ============================================================
class Solution2 {
public:
    bool isPalindrome(string s) {
        string cleaned;
        for (char c : s) {
            if (isalnum(c)) {
                cleaned += tolower(c);
            }
        }
        int left = 0, right = (int)cleaned.size() - 1;
        while (left < right) {
            if (cleaned[left] != cleaned[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};

// ============================================================
// 解法3: 原地双指针 — 面试首选 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法2 的瓶颈：需要 O(n) 空间存清洗后的字符串。
// 观察：我们其实不需要真的建一个新字符串，只需要在原字符串上
// 用双指针，遇到非字母数字字符直接跳过即可。
//
// 核心逻辑：
// 1. left 从左往右，right 从右往左
// 2. left 遇到无效字符 → left++（跳过）
// 3. right 遇到无效字符 → right--（跳过）
// 4. 两边都停在有效字符上时，比较（忽略大小写）
// 5. 不匹配 → false，匹配 → 继续移动
//
// 示例: "A man, a plan, a canal: Panama"
//        L                            R
// Step1: L='A'(有效), R='a'(有效) → tolower 都是 'a' → 匹配 ✓
//         L                          R
// Step2: L=' '(无效) → 跳过 → L='m'
//          L                        R
// Step3: L='m', R='m' → 匹配 ✓
//           ...
// 最终: left >= right → 全部匹配 → return true
//
// 特殊情况: "  " (全空格)
//            L  R
// Step1: L跳过空格 → left=2, right=1 → left > right → 循环不执行
// → return true (空串是回文)
// ============================================================
class Solution3 {
public:
    bool isPalindrome(string s) {
        int left = 0, right = (int)s.size() - 1;
        while (left < right) {
            // 左指针跳过非字母数字字符
            // 为什么内层也要检查 left < right？
            // → 防止字符串全是无效字符时 left 越过 right 导致越界
            while (left < right && !isalnum(s[left])) left++;
            // 右指针跳过非字母数字字符
            while (left < right && !isalnum(s[right])) right--;

            // 此时 left 和 right 都指向有效字符（或 left >= right）
            // 忽略大小写比较
            if (tolower(s[left]) != tolower(s[right])) {
                return false;  // 不对称 → 不是回文
            }
            left++;
            right--;
        }
        return true;  // 所有有效字符都对称匹配
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 核心操作 |
// |------|------|------|----------|
// | 预处理+反转 | O(n) | O(n) | 建新串 + reverse + 比较 |
// | 预处理+双指针 | O(n) | O(n) | 建新串 + 双指针比较 |
// | 原地双指针 | O(n) | O(1) | 原地跳过 + 双指针比较 |
//
// 解法1→2: 省去了反转步骤，用双指针代替（逻辑更清晰）
// 解法2→3: 省去了建新串的 O(n) 空间，直接在原串上操作
//
// 面试时：直接写解法3，简洁且最优。
//         如果想展示思路，可以先口述解法1的想法再说"我们可以优化空间"。
//
// ============================================================
// 【易错点】
//
// 1. 内层 while 漏掉 left < right 检查:
//    ✗ while (!isalnum(s[left])) left++;
//      → 输入 ".,," 时 left 会越界访问
//    ✓ while (left < right && !isalnum(s[left])) left++;
//
// 2. 忘记 tolower 就直接比较:
//    ✗ if (s[left] != s[right]) return false;
//      → "Aa" 会被判为非回文（'A' != 'a'）
//    ✓ if (tolower(s[left]) != tolower(s[right])) return false;
//
// 3. 用 isalpha 代替 isalnum:
//    ✗ isalpha(s[left])  → 只检查字母，会跳过数字
//    ✓ isalnum(s[left])  → 检查字母和数字
//    → 输入 "0P" 时，如果用 isalpha 会跳过 '0'，得到错误结果
//
// 4. right 初始化为 s.size() 而不是 s.size()-1:
//    ✗ int right = s.size();  → 越界访问 s[s.size()]
//    ✓ int right = (int)s.size() - 1;
//
// ============================================================
// 【面试追问】
//
// Q1（基础）: 暴力解怎么做？时间空间？
// → 预处理清洗字符串 + 反转比较，O(n) 时间 O(n) 空间。
//
// Q2（优化）: 能否 O(1) 空间？
// → 原地双指针，左右指针跳过无效字符直接比较，不需要建新串。
//
// Q3（变体）: 如果允许最多删除一个字符呢？(LeetCode 680)
// → 复用本题双指针框架。当 s[left] != s[right] 时，
//   尝试跳过左边一个 (left+1, right) 或右边一个 (left, right-1)，
//   分别检查子串是否回文。只要有一个是回文就返回 true。
//
// Q4（扩展）: 如果输入含 Unicode（如中文）？
// → isalnum/tolower 只适用 ASCII，需要 locale-aware 函数
//   或自定义有效字符判断规则。
// ============================================================
