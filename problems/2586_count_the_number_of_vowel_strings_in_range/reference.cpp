/*
 * LeetCode 2586: Count the Number of Vowel Strings in Range
 * (统计范围内的元音字符串数)
 *
 * 【题目本质】
 * 在 words[left..right] 范围内，统计以元音开头且以元音结尾的字符串数量。
 * 元音: a, e, i, o, u
 *
 * 【解法总览】
 * 解法1:  直接遍历(lambda)     — O(n)/O(1)   — 面试首写
 * 解法1b: 直接遍历(string.find) — O(n)/O(1)  — 更简洁
 * 解法1c: 直接遍历(set)        — O(n)/O(1)   — 最清晰
 * 解法2:  前缀和              — O(n)/O(n)    — 多次查询优化
 */


// ============================================================
// 解法1: 直接遍历 + Lambda — 面试首写 ⭐
// 时间: O(right - left + 1)  空间: O(1)
//
// 【思路】
// 遍历 words[left..right]，对每个字符串检查：
//   1. 首字符是否是元音 (a, e, i, o, u)
//   2. 尾字符是否是元音
// 两个条件都满足则计数+1。
//
// 示例1: words = ["are", "amy", "u"], left=0, right=2
//   i=0: "are" → front='a'(✓) back='e'(✓) → count=1
//   i=1: "amy" → front='a'(✓) back='y'(✗) → 不算
//   i=2: "u"   → front='u'(✓) back='u'(✓) → count=2
//   返回 2
//
// 示例2: words = ["hey","aeo","mu","ooo","artro"], left=1, right=4
//   i=1: "aeo"   → 'a'(✓) 'o'(✓) → count=1
//   i=2: "mu"    → 'm'(✗)         → 不算 (短路求值,不检查尾)
//   i=3: "ooo"   → 'o'(✓) 'o'(✓) → count=2
//   i=4: "artro" → 'a'(✓) 'o'(✓) → count=3
//   返回 3
//
// 为什么用 lambda 封装 isVowel?
//   避免在 if 条件中写两遍长长的 || 链，提高可读性。
//   编译器会内联 lambda，无性能损失。
// ============================================================
class Solution1 {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        // lambda 封装元音判断：清晰、可复用
        auto isVowel = [](char c) {
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };

        int count = 0;
        for (int i = left; i <= right; i++) { // 注意: <= right (闭区间)
            // front() = 首字符, back() = 尾字符
            // 单字符串如 "u": front() == back() == 'u'
            if (isVowel(words[i].front()) && isVowel(words[i].back())) {
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法1b: 用 string.find 判断元音 — 更简洁的写法
// 时间: O(right - left + 1)  空间: O(1)
//
// 【思路】
// 用 "aeiou".find(c) 判断字符是否是元音。
// find 在长度5的字符串中查找，实际是 O(1)。
// ============================================================
class Solution1b {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        string vowels = "aeiou";
        int count = 0;
        for (int i = left; i <= right; i++) {
            // find 返回位置，找不到返回 string::npos
            if (vowels.find(words[i].front()) != string::npos &&
                vowels.find(words[i].back()) != string::npos) {
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法1c: 用 unordered_set 判断元音 — 语义最清晰
// 时间: O(right - left + 1)  空间: O(1) (集合大小固定为5)
//
// 【思路】
// 预定义元音集合，用 count() 方法判断。
// 语义: "元音集合中是否包含这个字符？"
// ============================================================
class Solution1c {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
        int count = 0;
        for (int i = left; i <= right; i++) {
            // count() 返回 0 或 1，可直接用作布尔值
            if (vowels.count(words[i].front()) &&
                vowels.count(words[i].back())) {
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 解法2: 前缀和 — 多次查询优化
// 时间: O(n) 预处理 + O(1) 每次查询  空间: O(n)
//
// 【思路】
// 如果 vowelStrings 被多次调用（不同的 left, right），
// 每次遍历一遍太慢。预处理前缀和后每次 O(1)。
//
// prefix[i] = words[0..i-1] 中元音字符串的数量
// 查询 [left, right] = prefix[right+1] - prefix[left]
//
// 示例: words = ["are", "amy", "u", "hey", "aeo"]
//
//   判断: [1, 0, 1, 0, 1]  (are=是, amy=否, u=是, hey=否, aeo=是)
//   prefix: [0, 1, 1, 2, 2, 3]
//            ↑  ↑  ↑  ↑  ↑  ↑
//           空  are amy u  hey aeo
//
//   查询 [0,2]: prefix[3]-prefix[0] = 2-0 = 2 ✓ (are + u)
//   查询 [1,4]: prefix[5]-prefix[1] = 3-1 = 2 ✓ (u + aeo)
//   查询 [2,2]: prefix[3]-prefix[2] = 2-1 = 1 ✓ (u)
//
// 本题只有一次查询，前缀和是 overkill。
// 但如果面试官追问多次查询场景，这是标准答案。
// (参考: LeetCode 2559 就是本题的多查询版)
// ============================================================
class Solution2 {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        int n = words.size();
        auto isVowel = [](char c) {
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };

        // 预处理: 构建前缀和
        // prefix[0] = 0 (空前缀)
        // prefix[i] = words[0..i-1] 中元音字符串数量
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            bool isVowelStr = isVowel(words[i].front()) &&
                              isVowel(words[i].back());
            prefix[i + 1] = prefix[i] + (isVowelStr ? 1 : 0);
        }

        // O(1) 查询: [left, right] 范围内的元音字符串数
        return prefix[right + 1] - prefix[left];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间(单次) | 时间(k次) | 空间  | 场景          |
// |-------------|-----------|----------|-------|--------------|
// | 直接遍历     | O(n)      | O(k*n)   | O(1)  | 单次查询      |
// | 前缀和       | O(n)+O(1) | O(n+k)   | O(n)  | 多次查询      |
//
// n = right - left + 1 (查询范围大小)
//
// ============================================================
// 【元音判断方法对比】
//
// 方法A: || 链
//   c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'
//   优点: 零额外空间，编译器优化好
//   缺点: 代码长，写两遍(首尾)更长
//
// 方法B: string.find
//   string("aeiou").find(c) != string::npos
//   优点: 简洁
//   缺点: 可能创建临时 string 对象(用常量避免)
//
// 方法C: unordered_set
//   unordered_set<char>{'a','e','i','o','u'}.count(c)
//   优点: 语义最清晰
//   缺点: 哈希表开销(5个元素其实不值得)
//
// 方法D: 查找表 (bool array)
//   bool v[128] = {}; v['a']=v['e']=v['i']=v['o']=v['u']=true;
//   if (v[c]) ...
//   优点: O(1) 查找，无分支
//   缺点: 占128字节空间(可忽略)
//
// 面试推荐: 方法A(lambda封装) 或 方法C(语义清晰)
//
// ============================================================
// 【易错点】
//
// 1. 只检查首字符忘记检查尾字符:
//    ✗ if (isVowel(front()))  → "amy" 被错误计入
//    ✓ if (isVowel(front()) && isVowel(back()))
//
// 2. 循环范围写错:
//    ✗ for (i = left; i < right; ...)  → 少检查 words[right]
//    ✓ for (i = left; i <= right; ...) → 闭区间
//
// 3. 元音集合不完整:
//    ✗ 忘记 'u' 或 'o'
//    ✓ {a, e, i, o, u} 恰好5个
//
// 4. 前缀和下标偏移:
//    ✗ prefix[right] - prefix[left]  → 漏了 words[right]
//    ✓ prefix[right+1] - prefix[left] → 正确的闭区间查询
//
// ============================================================
// 【面试追问】
//
// Q1: 多次查询怎么优化?
//     前缀和。预处理 O(n)，每次查询 O(1)。
//     参考 LeetCode 2559 (本题的多查询版)。
//
// Q2: 如何让元音判断无分支(branch-free)?
//     用 bool 查找表: bool v[128]; v['a']=...=true;
//     if(v[c]) 是一次数组访问，无条件分支。
//     但对 5 个元素，编译器通常已经优化得很好。
//
// Q3: 如果 words 可能包含大写字母?
//     先 tolower(c) 再判断，或者元音集合加上大写:
//     'A','E','I','O','U'
//
// ============================================================
// 【相关题型】
//
// - 2559. 统计范围内的元音字符串数 II:
//   本题的多查询版，必须用前缀和。
//
// - 1456. 定长子串中元音的最大数目:
//   滑动窗口 + 元音计数。
//
// - 345. 反转字符串中的元音字母:
//   双指针 + 元音判断。
//
// - 1839. 所有元音按顺序排列的最长子字符串:
//   滑动窗口 + 元音顺序检查。
// ============================================================
