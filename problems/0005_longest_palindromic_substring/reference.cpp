/*
 * LeetCode 5: 最长回文子串 (Longest Palindromic Substring)
 *
 * 【题目本质】
 * 在字符串 s 中，找到最长的连续子串使其正读反读一样。
 * 核心是如何高效地搜索所有可能的回文子串。
 *
 * 【解法总览】
 * 解法1: 暴力枚举        — O(n³) / O(1)   — 枚举所有子串逐个判断
 * 解法2: 动态规划        — O(n²) / O(n²)  — 区间DP记录子串是否回文
 * 解法3: 中心扩展法      — O(n²) / O(1)   — 枚举中心向两边扩展 ⭐面试首选
 * 解法4: Manacher算法    — O(n) / O(n)    — 利用已知回文对称性跳跃
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子串，逐个判断是否回文
// 时间: O(n³)  空间: O(1)
//
// 【思路】
// 最直觉的做法：双重循环枚举所有 O(n²) 个子串 s[i..j]，
// 对每个子串用双指针判断是否回文 O(n)。
// 瓶颈：每个子串的回文判断都是独立的，没有利用任何已知信息。
// 当 n=1000 时约 10⁹ 次操作，会超时。
// ============================================================
class Solution1 {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        int start = 0, maxLen = 1;

        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                if (isPalindrome(s, i, j) && j - i + 1 > maxLen) {
                    start = i;
                    maxLen = j - i + 1;
                }
            }
        }
        return s.substr(start, maxLen);
    }

private:
    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};

// ============================================================
// 解法2: 动态规划 — 区间DP记录子串是否回文
// 时间: O(n²)  空间: O(n²)
//
// 【思路】
// 解法1的瓶颈：判断 s[i..j] 是否回文时，没有复用 s[i+1..j-1] 的结果。
// 观察：s[i..j] 是回文 ⟺ s[i]==s[j] 且 s[i+1..j-1] 是回文。
// 这就是经典的区间DP——用 dp[i][j] 记录 s[i..j] 是否回文。
//
// 状态转移:
//   dp[i][j] = (s[i] == s[j]) && (j-i < 2 || dp[i+1][j-1])
//   其中 j-i < 2 处理长度1和2的 base case
//
// 遍历顺序: 按子串长度从小到大，保证 dp[i+1][j-1] 先于 dp[i][j] 计算
//
// DP 填表过程 (s = "babad"):
//
//   dp[i][j] 表示 s[i..j] 是否回文
//
//       j→  0(b)  1(a)  2(b)  3(a)  4(d)
//   i↓
//   0(b)     T     F     T     F     F
//   1(a)           T     F     T     F
//   2(b)                 T     F     F
//   3(a)                       T     F
//   4(d)                             T
//
//   填表方向: 先填对角线(len=1, 全T)
//             → 次对角线(len=2): dp[0][1]=(b==a)=F, dp[1][2]=(a==b)=F, ...
//             → len=3: dp[0][2]=(b==b)&&dp[1][1]=T ← "bab"是回文!
//             → len=4: dp[0][3]=(b==a)=F, dp[1][4]=(a==d)=F
//             → len=5: dp[0][4]=(b==d)=F
//   最长回文: dp[0][2]=T 和 dp[1][3]=T, 长度3, "bab"或"aba"
// ============================================================
class Solution2 {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        if (n < 2) return s;

        vector<vector<bool>> dp(n, vector<bool>(n, false));
        int start = 0, maxLen = 1;

        // base case: 长度1的子串都是回文
        for (int i = 0; i < n; i++) {
            dp[i][i] = true;
        }

        // 按子串长度从小到大填表
        // 为什么按长度遍历？因为 dp[i][j] 依赖 dp[i+1][j-1]（更短的子串）
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;

                if (s[i] != s[j]) {
                    dp[i][j] = false;
                } else {
                    // s[i]==s[j] 时:
                    //   len==2: 如 "aa"，中间没有字符，直接是回文
                    //   len>=3: 还需要中间部分 s[i+1..j-1] 也是回文
                    dp[i][j] = (len == 2) || dp[i + 1][j - 1];
                }

                if (dp[i][j] && len > maxLen) {
                    start = i;
                    maxLen = len;
                }
            }
        }

        return s.substr(start, maxLen);
    }
};

// ============================================================
// 解法3: 中心扩展法 — 枚举 2n-1 个中心，向两边扩展 ⭐面试首选
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 换个角度：回文关于中心对称。与其枚举子串的两个端点 (i,j)，
// 不如枚举回文的中心，然后从中心向外扩展。
//
// 关键洞察：回文中心有两种
//   - 奇数长度回文（如 "aba"）→ 以单个字符 s[i] 为中心
//   - 偶数长度回文（如 "abba"）→ 以两个相邻字符 s[i],s[i+1] 之间的间隙为中心
//   共 n + (n-1) = 2n-1 个中心
//
// 对每个中心，向外扩展直到两边字符不等，得到以该中心的最长回文。
//
// 为什么比暴力快？
//   暴力：对每个子串从头验证整个串 → O(n) per check
//   中心扩展：增量验证，每扩一步只比较两个字符 → 总工作量不变但不需要额外空间
//
// 扩展过程示意 (s = "babad"):
//
//   中心 s[0]='b':
//     expand(0,0): b → 尝试扩展 s[-1]?越界 → 回文"b", 长度1
//   中心 s[0..1]:
//     expand(0,1): b≠a → 不回文, 长度0（这里 right-left-1 = 1-0-1 = 0，但实际是无回文）
//   中心 s[1]='a':
//     expand(1,1): a → s[0]='b'==s[2]='b' ✓ → "bab"
//                    → s[-1]?越界 → 回文"bab", 长度3 ← 最长！
//   中心 s[1..2]:
//     expand(1,2): a≠b → 不回文
//   中心 s[2]='b':
//     expand(2,2): b → s[1]='a'==s[3]='a' ✓ → "aba"
//                    → s[0]='b'≠s[4]='d' ✗ → 回文"aba", 长度3
//   中心 s[2..3]:
//     expand(2,3): b≠a → 不回文
//   中心 s[3]='a':
//     expand(3,3): a → s[2]='b'≠s[4]='d' → 回文"a", 长度1
//   中心 s[3..4]:
//     expand(3,4): a≠d → 不回文
//   中心 s[4]='d':
//     expand(4,4): d → 越界 → 回文"d", 长度1
//
//   结果: 最长 = "bab", 长度3, 起始位置0
// ============================================================
class Solution3 {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        if (n < 2) return s;

        int start = 0, maxLen = 1;

        for (int i = 0; i < n; i++) {
            // 以 s[i] 为中心（奇数长度回文，如 "aba"）
            int len1 = expandAroundCenter(s, i, i);
            // 以 s[i] 和 s[i+1] 之间为中心（偶数长度回文，如 "abba"）
            int len2 = expandAroundCenter(s, i, i + 1);

            int len = max(len1, len2);
            if (len > maxLen) {
                // 从中心位置和长度反推起始位置:
                //   奇数: 中心i, 向左(len-1)/2步 → start = i - (len-1)/2
                //   偶数: 中心在i和i+1之间, 向左len/2-1步 → start = i - len/2 + 1
                //   统一公式: start = i - (len - 1) / 2  (整数除法自动处理奇偶)
                start = i - (len - 1) / 2;
                maxLen = len;
            }
        }

        return s.substr(start, maxLen);
    }

private:
    // 从 (left, right) 开始向外扩展，返回找到的最长回文长度
    // 初始 left==right → 奇数长度; left+1==right → 偶数长度
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < (int)s.size() && s[left] == s[right]) {
            left--;   // 向左扩
            right++;  // 向右扩
        }
        // 循环结束时: s[left] != s[right] 或越界
        // 回文的实际范围是 [left+1, right-1]
        // 长度 = (right-1) - (left+1) + 1 = right - left - 1
        return right - left - 1;
    }
};

// ============================================================
// 解法4: Manacher 算法 — 利用已知回文的对称性跳过重复扩展
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 中心扩展法的瓶颈：每个中心独立扩展，没有利用之前的扩展结果。
// Manacher 的核心观察：
//   如果我们知道以 center 为中心的回文延伸到了 right，
//   那么对于 right 内的新位置 i，i 关于 center 的镜像点 mirror 
//   的回文半径已经算过了，可以直接复用！
//
// 预处理: 插入 # 使所有回文变成奇数长度
//   "babad" → "^#b#a#b#a#d#$"
//   ^ 和 $ 是哨兵字符，保证扩展时自然停止不会越界
//
// Manacher 核心逻辑:
//
//   维护: center(当前最右回文的中心), right(其右边界)
//   对于位置 i:
//     mirror = 2*center - i  (i关于center的对称点)
//     如果 i < right:
//       p[i] = min(p[mirror], right - i)  ← 利用对称性跳过已知部分
//     然后尝试继续扩展 p[i]
//     如果 i + p[i] > right: 更新 center = i, right = i + p[i]
//
//   为什么 p[i] = min(p[mirror], right - i)?
//     p[mirror] 是镜像点的半径，但不能超过 right-i（超出部分还没验证）
//     如果 p[mirror] < right-i: p[i] 就等于 p[mirror]，不需要继续扩展
//     如果 p[mirror] >= right-i: p[i] 至少是 right-i，但可能更大，需要继续扩展
//
//   预处理后索引映射:
//     预处理串 t 中的位置 i 对应原串 s 中的位置 (i-1)/2（仅当 t[i] != '#'）
//     回文半径 p[i] 就是原串中的回文长度
//     原串起始位置 = (centerIdx - maxLen) / 2
// ============================================================
class Solution4 {
public:
    string longestPalindrome(string s) {
        // 预处理: 插入 # 统一奇偶, ^ $ 作为哨兵
        // 例: "babad" → "^#b#a#b#a#d#$"
        string t = "^#";
        for (char c : s) {
            t += c;
            t += '#';
        }
        t += '$';

        int n = t.size();
        vector<int> p(n, 0);  // p[i] = 以 t[i] 为中心的回文半径

        int center = 0, right = 0;

        for (int i = 1; i < n - 1; i++) {
            int mirror = 2 * center - i;

            // 利用对称性: 如果 i 在当前最右回文范围内
            if (i < right) {
                // p[mirror] 是镜像点的半径, right-i 是到右边界的距离
                // 取较小值: 不能超出已验证的范围
                p[i] = min(p[mirror], right - i);
            }

            // 尝试继续向外扩展
            // 哨兵 ^ 和 $ 保证不会越界（它们互不相等，也不等于任何其他字符）
            while (t[i + p[i] + 1] == t[i - p[i] - 1]) {
                p[i]++;
            }

            // 如果当前回文超过了最右边界，更新 center 和 right
            if (i + p[i] > right) {
                center = i;
                right = i + p[i];
            }
        }

        // 在 p 数组中找最大值
        int maxLen = 0, centerIdx = 0;
        for (int i = 1; i < n - 1; i++) {
            if (p[i] > maxLen) {
                maxLen = p[i];
                centerIdx = i;
            }
        }

        // 映射回原字符串
        // 预处理后位置 centerIdx 的回文半径 maxLen 就是原串中的回文长度
        int start = (centerIdx - maxLen) / 2;
        return s.substr(start, maxLen);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间   | 空间   | 代码复杂度 | 面试推荐 |
// |-------------|--------|--------|-----------|---------|
// | 暴力枚举     | O(n³) | O(1)   | 简单       | ❌ 超时  |
// | 动态规划     | O(n²) | O(n²) | 中等       | ✓ 理解DP |
// | 中心扩展     | O(n²) | O(1)   | 简单       | ⭐⭐⭐   |
// | Manacher    | O(n)  | O(n)   | 复杂       | 加分项   |
//
// 暴力 → DP: 优化了"判断回文"的重复计算，从 O(n³) 到 O(n²)
// 暴力 → 中心扩展: 换了搜索角度（从枚举端点到枚举中心），同样 O(n²) 但空间 O(1)
// 中心扩展 → Manacher: 利用已知回文跳过重复扩展，从 O(n²) 到 O(n)
//
// 面试策略:
//   1. 先说暴力思路（30秒）展示理解
//   2. 直接写中心扩展（代码简洁、O(1)空间）
//   3. 面试官追问DP就口头描述状态转移
//   4. 面试官问O(n)就描述Manacher思路，一般不要求写完整代码
//
// ============================================================
// 【易错点】
//
// 1. 中心扩展忘记偶数长度回文:
//    ✗ 只调用 expand(i, i)
//    ✓ 必须同时调用 expand(i, i) 和 expand(i, i+1)
//    "abba" 的中心在 b 和 b 之间，不是任何单个字符
//
// 2. 中心扩展起始位置公式:
//    ✗ start = i - len / 2
//    ✓ start = i - (len - 1) / 2
//    验证: 奇数 len=3, i=2 → start = 2-1 = 1 ✓ ("aba"在位置1)
//          偶数 len=4, i=1 → start = 1-1 = 0 ✓ ("abba"在位置0)
//    如果用 len/2: 偶数 len=4, i=1 → start = 1-2 = -1 ✗ 越界!
//
// 3. DP 遍历顺序错误:
//    ✗ for(i=0→n) for(j=i→n) — dp[i+1][j-1] 可能还没算
//    ✓ 按子串长度 len 从 2 到 n 遍历，或 i 从 n-1 到 0 倒序遍历
//    dp[i][j] 依赖 dp[i+1][j-1]，即更短的子串必须先算
//
// 4. DP 长度为2的边界:
//    ✓ 按子串长度 len 从 2 到 n 遍历，确保 dp[i+1][j-1] 已填
//
// 【面试追问 Interview Follow-ups】
// Q1: 中心扩展法的时间复杂度怎么证明是 O(n²)？
// A1: n 个中心（奇数）+ n-1 个中心（偶数）= 2n-1，每个最多扩展 n/2 次，总计 O(n²)。
//
// Q2: Manacher 算法能做到 O(n) 的核心原理是什么？
// A2: 利用已知回文的对称性跳过重复扩展：如果 i 在某个已知回文内部，
//     其对称位置 mirror 的回文半径可以直接复用，只需扩展超出部分。
//
// Q3: 如果要找所有回文子串（而不只是最长的），怎么改？
// A3: 中心扩展法天然可以在扩展过程中收集所有回文。或用 DP 表中所有 dp[i][j]=true 的 (i,j)。
//     参考 LC 647 Palindromic Substrings。
// ============================================================
