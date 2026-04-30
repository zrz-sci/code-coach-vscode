/*
 * LeetCode 409: 最长回文串 (Longest Palindrome)
 *
 * 标签: Greedy, Hash Table, String
 * 难度: Easy
 *
 * 四种解法:
 * 1. 频次数组 + 贪心   — O(n)/O(1) 标准解法
 * 2. HashSet 配对法    — O(n)/O(1) 直观简洁
 * 3. 统计奇数字符数    — O(n)/O(1) 代码最短
 * 4. 位运算变体        — O(n)/O(1) 技巧展示
 * 附加: 实际构造最长回文串
 */



// ============================================================
// 解法1: 频次数组 + 贪心 — O(n) / O(1)  ⭐ 标准解法
//
// 核心思想:
//   回文串 = 偶数配对 + 最多 1 个中心字符
//   对每个字符取 count/2*2 (最大偶数部分)
//   如果有剩余(result < n), 可以放 1 个在中间
//
// 示例: s = "abccccdd"
//   a=1 → 0,  b=1 → 0,  c=4 → 4,  d=2 → 2
//   result = 0+0+4+2 = 6, 有剩余 → 6+1 = 7
//
// 为什么 freq[128]？
//   题目: "lowercase and/or uppercase English letters"
//   'A'=65...'Z'=90, 'a'=97...'z'=122
//   用 128 覆盖所有 ASCII, 直接用字符值做索引
// ============================================================
class Solution_v1 {
public:
    int longestPalindrome(string s) {
        // 频次数组: 128 覆盖所有 ASCII 字符
        // 包括 'A'-'Z' (65-90) 和 'a'-'z' (97-122)
        int freq[128] = {};

        // Step 1: 统计每个字符的出现次数
        for (char c : s) {
            freq[c]++;
        }

        // Step 2: 贪心取偶数部分
        int result = 0;
        bool hasOdd = false;

        for (int i = 0; i < 128; i++) {
            // count / 2 * 2 = 向下取整到最近偶数
            // 5 → 4, 6 → 6, 1 → 0, 0 → 0
            result += freq[i] / 2 * 2;

            // 记录是否存在奇数次字符
            if (freq[i] % 2 == 1) {
                hasOdd = true;
            }
        }

        // Step 3: 如果有奇数次字符, 可以放 1 个在中间
        if (hasOdd) {
            result++;
        }

        return result;
    }
};


// ============================================================
// 解法2: HashSet 配对法 — O(n) / O(1)  直观简洁
//
// 思路: 维护一个"等待配对"的 set
//   遇到 set 中已有的字符 → 配对成功! 移除, result += 2
//   遇到 set 中没有的    → 加入等待
//   最终 set 非空 → 有落单字符可放中间, result += 1
//
// 过程追踪 (s = "abccccdd"):
//   'a' → set={a}
//   'b' → set={a,b}
//   'c' → set={a,b,c}
//   'c' → 配对! set={a,b}, result=2
//   'c' → set={a,b,c}
//   'c' → 配对! set={a,b}, result=4
//   'd' → set={a,b,d}
//   'd' → 配对! set={a,b}, result=6
//   set 非空 → +1 → result=7
// ============================================================
class Solution_v2 {
public:
    int longestPalindrome(string s) {
        unordered_set<char> unpaired;
        int result = 0;

        for (char c : s) {
            if (unpaired.count(c)) {
                // 找到配对: 移除并计数
                unpaired.erase(c);
                result += 2;
            } else {
                // 尚未配对: 加入等待集合
                unpaired.insert(c);
            }
        }

        // 有落单字符可以放中心
        if (!unpaired.empty()) {
            result++;
        }

        return result;
    }
};


// ============================================================
// 解法3: 统计奇数字符数 — O(n) / O(1)  代码最短
//
// 数学推导:
//   设 odd = 出现奇数次的字符数量
//   每个奇数次字符"浪费" 1 个 (无法配对)
//   但可以"挽救" 1 个放中心
//   总浪费 = max(odd - 1, 0)
//   答案 = n - max(odd - 1, 0) = n - odd + min(odd, 1)
//
// 示例: s = "abccccdd" (n=8)
//   a=1(奇), b=1(奇), c=4(偶), d=2(偶)
//   odd = 2
//   答案 = 8 - 2 + 1 = 7 ✓
//
// 示例: s = "aabb" (n=4)
//   a=2(偶), b=2(偶)
//   odd = 0
//   答案 = 4 - 0 + 0 = 4 ✓
// ============================================================
class Solution_v3 {
public:
    int longestPalindrome(string s) {
        int freq[128] = {};
        for (char c : s) freq[c]++;

        int oddCount = 0;
        for (int i = 0; i < 128; i++) {
            if (freq[i] & 1) { // freq[i] % 2 == 1
                oddCount++;
            }
        }

        int n = s.length();
        return n - oddCount + (oddCount > 0 ? 1 : 0);
    }
};


// ============================================================
// 解法4: 位运算变体 — O(n) / O(1)  技巧展示
//
// 利用 count & ~1 等价于 count / 2 * 2
//   ~1 = 0xFFFFFFFE (二进制全1, 最低位为0)
//   偶数: 6 & ~1 = 6  (110 & 110 = 110)
//   奇数: 5 & ~1 = 4  (101 & 110 = 100)
//
// result < n 等价于"存在奇数频次字符"
// 用这个条件判断是否 +1, 无需额外 bool 变量
// ============================================================
class Solution_v4 {
public:
    int longestPalindrome(string s) {
        int freq[128] = {};
        for (char c : s) freq[c]++;

        int result = 0;
        for (int i = 0; i < 128; i++) {
            result += freq[i] & ~1; // 清除最低位 = 取偶数部分
        }

        // result < n 说明有字符被浪费, 可以挽救 1 个
        if (result < (int)s.length()) result++;

        return result;
    }
};


// ============================================================
// 附加: 实际构造最长回文串
//
// Follow-up: "不仅返回长度, 还要构造一个合法的最长回文串"
//
// 算法:
// 1. 统计频次
// 2. 左半部分: 每个字符取 count/2 个
// 3. 中心: 如果有奇数次字符, 选 1 个
// 4. 右半 = reverse(左半)
// 5. 结果 = 左半 + 中心 + 右半
//
// 示例: s = "abccccdd"
//   左半 = "ccd"  (c取2, d取1)
//   中心 = "a"    (奇数次字符中选1个)
//   右半 = "dcc"
//   结果 = "ccd" + "a" + "dcc" = "ccdadcc"  长度=7 ✓
// ============================================================
class Solution_Construct {
public:
    string constructLongestPalindrome(string s) {
        int freq[128] = {};
        for (char c : s) freq[c]++;

        string leftHalf;
        char center = '\0';
        bool hasCenter = false;

        for (int i = 0; i < 128; i++) {
            if (freq[i] == 0) continue;

            // 左半部分取 count/2 个
            int pairs = freq[i] / 2;
            leftHalf += string(pairs, (char)i);

            // 记录一个奇数次字符作为中心候选
            if (freq[i] % 2 == 1 && !hasCenter) {
                center = (char)i;
                hasCenter = true;
            }
        }

        // 右半 = reverse(左半)
        string rightHalf(leftHalf.rbegin(), leftHalf.rend());

        // 组装结果
        string result = leftHalf;
        if (hasCenter) result += center;
        result += rightHalf;

        return result;
    }
};


// ============================================================
// 主解法 (提交用) — 解法1 频次数组 + 贪心
// ============================================================
class Solution {
public:
    int longestPalindrome(string s) {
        int freq[128] = {};
        for (char c : s) freq[c]++;

        int result = 0;
        bool hasOdd = false;
        for (int i = 0; i < 128; i++) {
            result += freq[i] / 2 * 2;
            if (freq[i] % 2 == 1) hasOdd = true;
        }
        if (hasOdd) result++;
        return result;
    }
};


// ============================================================
// 【面试追问】
// ============================================================
// Q1: 如果字符集不限于英文字母(Unicode)？
//     → 用 unordered_map<char, int> 替代固定数组
// Q2: 如果要构造而非仅返回长度？
//     → 左半(每字符取count/2个) + 中心(选1个奇数次字符) + reverse(左半)
// Q3: 与 #266 Palindrome Permutation 的关系？
//     → #266 是判定版: 能否重排成回文 ⟺ 最多1个奇数频次字符
//
// 【易错点】
// 1. 忘记大小写敏感 → 把字符串转小写再处理，合并了不同字符 ✗
// 2. 无条件 +1 → 所有字符都是偶数次时(如 "aabb")答案多 1 ✗
// 3. freq 数组大小用 26 → 大小写混合时越界或冲突 ✗
// 4. 混淆"构造回文串"和"计算长度" → 本题只需返回长度 ✗
