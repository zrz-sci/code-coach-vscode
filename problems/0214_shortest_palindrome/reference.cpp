// ============================================================
// 214. Shortest Palindrome
// ============================================================
// 标签: String, String Matching, Hash Function, Rolling Hash
// 难度: Hard
//
// 核心思路:
//   找从索引0开始的最长回文前缀，把剩余后缀反转拼到前面。
//   解法1: KMP failure function -- O(n) / O(n)
//   解法2: Rabin-Karp Rolling Hash -- O(n) / O(1)
//   解法3: 暴力 -- O(n^2) / O(n)
// ============================================================


// ============================================================
// 解法1: KMP Failure Function (首选)
//
// 思路:
//   1. 构造 combined = s + "#" + reverse(s)
//   2. 计算 KMP failure function (next 数组)
//   3. next 最后一个值 = s 的最长回文前缀长度
//   4. 把非回文后缀反转，拼到前面
//
// 原理:
//   combined 的前缀来自 s 的前缀
//   combined 的后缀来自 reverse(s) 的后缀 = s 前缀的反转
//   两者相等 => s 的某前缀 = 它自身的反转 => 回文
//
// failure 数组构建 (s = "aacecaaa"):
//
//   combined = "aacecaaa#aaacecaa"
//
//   索引:     0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16
//   字符:     a  a  c  e  c  a  a  a  #  a   a   a   c   e   c   a   a
//   next[i]:  0  1  0  0  0  1  2  2  0  1   2   2   3   4   5   6   7
//                                                                     ^
//                                                          最后值 = 7
//
//   => 最长回文前缀 = s[0..6] = "aacecaa" (长度7)
//   => 答案 = reverse("a") + s = "aaacecaaa"
//
// 分隔符 '#' 防止 next 值超过 s 长度:
//   s="aaa": 无分隔符 "aaaaaa" next最后=5>3 (错)
//            有分隔符 "aaa#aaa" next最后=3 (对)
//
// 时间: O(n)  空间: O(n)
// ============================================================
class Solution_KMP {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;

        // Step 1: 构造 combined
        string rev = s;
        reverse(rev.begin(), rev.end());
        string combined = s + "#" + rev;

        // Step 2: 计算 failure function
        int n = combined.size();
        vector<int> next(n, 0);

        for (int i = 1; i < n; i++) {
            int j = next[i - 1];

            // 不匹配时回退
            while (j > 0 && combined[i] != combined[j]) {
                j = next[j - 1];
            }

            if (combined[i] == combined[j]) {
                j++;
            }

            next[i] = j;
        }

        // Step 3: 构造答案
        int longestPalinPrefix = next.back();
        string suffix = s.substr(longestPalinPrefix);
        reverse(suffix.begin(), suffix.end());
        return suffix + s;
    }
};


// ============================================================
// 解法2: Rabin-Karp Rolling Hash -- O(n) / O(1)
//
// 思路:
//   遍历 s，同时维护正向和反向哈希:
//     forward  = s[0]*B^i + ... + s[i]*B^0    (正向读)
//     backward = s[0]*B^0 + ... + s[i]*B^i    (反向读)
//   forward == backward => s[0..i] 是回文
//
// 更新公式:
//   forward  = forward * BASE + c
//   backward = backward + c * power
//   power    = power * BASE
//
// 示例 (s="aacecaaa", B=31, a=1):
//   i=0: fwd=1,   bwd=1    -> 相等! len=1  "a"
//   i=1: fwd=32,  bwd=32   -> 相等! len=2  "aa"
//   i=2: fwd=995, bwd=2915 -> 不等
//   ...
//   i=6: fwd=bwd=919055265  -> 相等! len=7  "aacecaa"
//   i=7: 不等
//   longest = 7
//
// 优点: 空间 O(1)，比 KMP 更省
// 缺点: 哈希冲突概率（极低，可用双哈希消除）
//
// 时间: O(n)  空间: O(1)
// ============================================================
class Solution_RollingHash {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;

        long long BASE = 31;
        long long MOD = 1e9 + 7;
        long long fwd = 0, bwd = 0, power = 1;
        int longest = 0;

        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a' + 1;  // 1-26, 避免0歧义

            fwd = (fwd * BASE + c) % MOD;
            bwd = (bwd + (long long)c * power) % MOD;
            power = (power * BASE) % MOD;

            if (fwd == bwd) {
                longest = i + 1;
            }
        }

        string suffix = s.substr(longest);
        reverse(suffix.begin(), suffix.end());
        return suffix + s;
    }
};


// ============================================================
// 解法3: 暴力 -- O(n^2) / O(n)
//
// 从长到短枚举 s[0..k] 是否回文。
// 找到最长回文前缀后，把剩余部分反转拼到前面。
//
// 时间: O(n^2)  空间: O(n)
// ============================================================
class Solution_Brute {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;

        int n = s.size();
        for (int k = n - 1; k >= 0; k--) {
            if (isPalin(s, 0, k)) {
                string suffix = s.substr(k + 1);
                reverse(suffix.begin(), suffix.end());
                return suffix + s;
            }
        }
        return s;
    }

private:
    bool isPalin(const string& s, int l, int r) {
        while (l < r) {
            if (s[l] != s[r]) return false;
            l++; r--;
        }
        return true;
    }
};


// ============================================================
// KMP 时间复杂度 O(n) 的摊还分析
//
// 虽然 for 内有 while 循环，但:
//   - for 每次 i++，j 最多 +1 -> j 的总增量 <= n
//   - while 每次 j 至少 -1    -> j 的总减量 <= 总增量 <= n
//   - 所以 while 总执行次数 <= n
//   - 总时间 = O(n) + O(n) = O(n)
// ============================================================


// ============================================================
// 测试代码
// ============================================================
int main() {
    Solution_KMP kmp;
    Solution_RollingHash rk;
    Solution_Brute brute;

    // 测试 1: 经典案例
    cout << "=== Test 1: s = \"aacecaaa\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("aacecaaa") << endl;   // aaacecaaa
    cout << "RK:    " << rk.shortestPalindrome("aacecaaa") << endl;    // aaacecaaa
    cout << "Brute: " << brute.shortestPalindrome("aacecaaa") << endl; // aaacecaaa

    // 测试 2: 无回文前缀
    cout << "\n=== Test 2: s = \"abcd\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("abcd") << endl;       // dcbabcd

    // 测试 3: 空串
    cout << "\n=== Test 3: s = \"\" ===" << endl;
    cout << "KMP:   \"" << kmp.shortestPalindrome("") << "\"" << endl; // ""

    // 测试 4: 单字符
    cout << "\n=== Test 4: s = \"a\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("a") << endl;          // a

    // 测试 5: 已经是回文
    cout << "\n=== Test 5: s = \"aba\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("aba") << endl;        // aba
    cout << "RK:    " << rk.shortestPalindrome("aba") << endl;         // aba

    // 测试 6: 全相同 (分隔符重要性)
    cout << "\n=== Test 6: s = \"aaa\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("aaa") << endl;        // aaa
    cout << "RK:    " << rk.shortestPalindrome("aaa") << endl;         // aaa

    // 测试 7: 两字符
    cout << "\n=== Test 7: s = \"ab\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("ab") << endl;         // bab

    // 测试 8: 较长回文
    cout << "\n=== Test 8: s = \"abcbabcba\" ===" << endl;
    cout << "KMP:   " << kmp.shortestPalindrome("abcbabcba") << endl;  // abcbabcba

    cout << "\nAll tests completed!" << endl;
    return 0;
}

// ============================================================
// 复杂度总结
//
// | 解法              | 时间   | 空间   | 面试推荐 |
// |-------------------|--------|--------|----------|
// | KMP failure func  | O(n)   | O(n)   | 首选     |
// | Rabin-Karp hash   | O(n)   | O(1)   | 备选     |
// | 暴力              | O(n^2) | O(n)   | 仅分析   |
//
// KMP 优势: 确定性正确，无哈希冲突
// Rolling Hash 优势: 空间 O(1)，代码更简短
// ============================================================
//
// ============================================================
// 易错点:
//
// 1. 忘加分隔符 '#':
//    ✗ combined = s + reverse(s)
//      s="aaa" -> "aaaaaa", next最后值=5>3, 回文前缀长度错误
//    ✓ combined = s + "#" + reverse(s)
//      s="aaa" -> "aaa#aaa", next最后值=3, 正确
//
// 2. 空串未特判:
//    ✗ 对空串做 reverse/substr 可能出错
//    ✓ if (s.empty()) return s;
//
// 3. Rolling Hash 溢出:
//    ✗ 用 int 做哈希计算, forward*BASE 溢出
//    ✓ 必须用 long long 且取模
//
// 4. KMP next 数组写法不统一:
//    ✗ 写法A和写法B混用导致 off-by-one
//    ✓ 选定一种写法 (本题用 next[0]=0) 并保持一致
//
// 5. 已经是回文的情况:
//    ✗ s="aba" 时仍然在前面多加字符
//    ✓ 最长回文前缀=整个s, suffix为空, 返回原串
//
// ============================================================
// 面试追问:
//
// Q1: 分隔符为什么必须用不在字符集中出现的字符?
//     防止 next 数组匹配跨越 s 和 reverse(s) 的边界,
//     导致回文前缀长度大于 s 本身长度。
//
// Q2: KMP failure function 的 while 为什么整体是 O(n)?
//     摊还分析: j 总增量 <= n, 总减量 <= 总增量 <= n,
//     while 总执行次数 <= n, 整体 O(n)。
//
// Q3: Rolling Hash 和 KMP 各有什么优劣?
//     KMP: 确定性正确, 无冲突; Rolling Hash: O(1) 空间, 代码更短,
//     但有极小概率哈希冲突 (可双哈希消除)。
//
// Q4: 如果允许在任意位置插入字符?
//     不同问题, 需要 DP (类似最长回文子序列), 本题限定只在前面加。
// ============================================================
