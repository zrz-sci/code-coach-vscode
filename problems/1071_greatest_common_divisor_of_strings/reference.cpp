/*
 * LeetCode 1071: 字符串的最大公因子 (Greatest Common Divisor of Strings)
 *
 * 【题目本质】
 * 把整数 GCD 的概念迁移到字符串上：找最长的字符串 x，使得 x 重复若干次
 * 能分别构成 str1 和 str2。
 *
 * 【解法总览】
 * 解法1: 暴力枚举    — O(n·(n+m)) / O(n+m) — 最直觉，穷举所有候选长度
 * 解法2: 辗转相减    — O(n+m) / O(n+m)     — 模拟整数 GCD 的辗转相减过程
 * 解法3: GCD+拼接验证 — O(n+m) / O(n+m)     — 最优雅，面试首选
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有可能的公因子长度，逐个验证
// 时间: O(n·(n+m))  空间: O(n+m)
//
// 【思路】
// 公因子字符串 x 一定是 str1 的前缀，且 len(x) 同时整除 len1 和 len2。
// 从最长的候选开始枚举（min(len1,len2) 到 1），第一个满足条件的就是答案。
// 对每个候选，验证它重复后能否拼出 str1 和 str2。
//
// 为什么从大到小？因为题目要求最长的公因子，第一个满足的就能直接返回。
// ============================================================
class Solution1 {
public:
    string gcdOfStrings(string str1, string str2) {
        int len1 = str1.size(), len2 = str2.size();
        
        for (int i = min(len1, len2); i >= 1; i--) {
            // 候选长度必须同时整除两个字符串长度，否则不可能重复整数次拼成
            if (len1 % i != 0 || len2 % i != 0) continue;
            
            string candidate = str1.substr(0, i);
            if (isDivisor(str1, candidate) && isDivisor(str2, candidate)) {
                return candidate;
            }
        }
        return "";
    }

private:
    // 验证 s 是否由 t 重复若干次构成
    // 不需要真的拼接，逐字符用取模比较即可
    bool isDivisor(const string& s, const string& t) {
        int n = s.size(), m = t.size();
        for (int i = 0; i < n; i++) {
            if (s[i] != t[i % m]) return false;
        }
        return true;
    }
};

// ============================================================
// 解法2: 辗转相减（递归） — 模拟 gcd(a,b) = gcd(a-b, b)
// 时间: O(n+m)  空间: O(n+m) 递归栈+子串创建
//
// 【思路】
// 整数的辗转相减法: gcd(12, 8) = gcd(4, 8) = gcd(4, 4) = 4
// 字符串同理: 如果 str1 以 str2 开头，就"减去"这个前缀，继续递归。
//
// 示意:
//   gcd("ABABAB", "ABAB")
//     → "ABABAB" 以 "ABAB" 开头 → 去掉前缀 → gcd("AB", "ABAB")
//     → "ABAB" 以 "AB" 开头 → 去掉前缀 → gcd("AB", "AB")
//     → 相等 → 返回 "AB"
//
// 如果某一步较长的字符串不以较短的开头，说明不存在公因子，返回 ""。
// ============================================================
class Solution2 {
public:
    string gcdOfStrings(string str1, string str2) {
        // 两者相等 → 公因子就是自身
        if (str1 == str2) return str1;
        
        // 保证 str1 >= str2（长度）
        if (str1.size() < str2.size()) swap(str1, str2);
        
        // str1 必须以 str2 开头，否则不存在公因子
        // 这一步对应整数辗转相减中 "a-b < 0 则无解" 的判断
        if (str1.substr(0, str2.size()) != str2) return "";
        
        // "减去" str2 前缀，继续递归
        return gcdOfStrings(str1.substr(str2.size()), str2);
    }
};

// ============================================================
// 解法3: GCD + 拼接验证 — 最优雅，面试首选 ⭐
// 时间: O(n+m)  空间: O(n+m) 用于拼接比较
//
// 【思路】
// 核心定理: str1 和 str2 存在公因子字符串
//           ⟺ str1 + str2 == str2 + str1
//
// 证明直觉:
//   设 str1 = x*a, str2 = x*b (x重复a次/b次)
//   str1 + str2 = x*(a+b)
//   str2 + str1 = x*(b+a) = x*(a+b)  → 一定相等
//
//   反过来, 如果拼接相等, 可以证明 str1[0..gcd-1] 就是那个 x,
//   因为拼接相等意味着字符具有以 gcd(len1,len2) 为周期的循环性质。
//
// 确认存在公因子后, 最大的长度就是 gcd(len1, len2),
// 对应的字符串就是 str1 的前 gcd(len1,len2) 个字符。
//
// 示意:
//   str1 = "ABCABC" (len=6), str2 = "ABC" (len=3)
//   "ABCABCABC" == "ABCABCABC" ✓
//   gcd(6,3) = 3 → 返回 "ABC"
//
//   str1 = "AAAAAB" (len=6), str2 = "AAA" (len=3)
//   "AAAAABAA A" vs "AAAAAAAAB"  → 不相等 ✗ → 返回 ""
// ============================================================
class Solution3 {
public:
    string gcdOfStrings(string str1, string str2) {
        // 拼接验证: 不相等 → 不存在公因子字符串
        if (str1 + str2 != str2 + str1) return "";
        
        // 存在公因子, 最大长度是 gcd(len1, len2)
        int g = gcd((int)str1.size(), (int)str2.size());
        return str1.substr(0, g);
    }
    
    // 如果不用 std::gcd, 手写辗转相除:
    // int gcd(int a, int b) {
    //     return b == 0 ? a : gcd(b, a % b);
    // }
};

// ============================================================
// 解法3 变体: 不创建拼接字符串，O(1) 额外空间验证
// 时间: O(n+m)  空间: O(1)
//
// 【思路】
// 面试追问: "能否避免创建 str1+str2 这两个新字符串?"
// 答: 用双指针逐字符比较, 模拟拼接后的对比。
// 比较 (str1+str2)[i] 和 (str2+str1)[i], 其中:
//   (str1+str2)[i] = i < len1 ? str1[i] : str2[i-len1]
//   (str2+str1)[i] = i < len2 ? str2[i] : str1[i-len2]
// ============================================================
class Solution3_SpaceOpt {
public:
    string gcdOfStrings(string str1, string str2) {
        int len1 = str1.size(), len2 = str2.size();
        int total = len1 + len2;
        
        // 逐字符比较 (str1+str2) 和 (str2+str1), 不真正拼接
        for (int i = 0; i < total; i++) {
            char c1 = (i < len1) ? str1[i] : str2[i - len1];
            char c2 = (i < len2) ? str2[i] : str1[i - len2];
            if (c1 != c2) return "";
        }
        
        int g = gcd(len1, len2);
        return str1.substr(0, g);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间         | 空间    | 核心思路           | 适用场景     |
// |-----------|-------------|---------|-------------------|------------|
// | 暴力枚举   | O(n·(n+m))  | O(n+m)  | 穷举候选长度+验证   | 理解题意     |
// | 辗转相减   | O(n+m)      | O(n+m)  | 递归"减去"前缀      | 加分项       |
// | GCD+拼接   | O(n+m)      | O(n+m)  | 数学性质一步到位     | 面试首选 ⭐  |
// | 拼接优化   | O(n+m)      | O(1)*   | 不创建拼接字符串     | 追问优化     |
//
// * 注: substr 返回答案本身需要 O(g) 空间, 但验证过程是 O(1)
//
// 【易错点】
// 1. 忘记拼接验证就直接取 gcd 长度:
//    ✗ return str1.substr(0, gcd(len1, len2));  
//      → 对 "AAAAAB","AAA" 返回 "AAA"（错！"AAAAAB" 不能由 "AAA" 重复构成）
//    ✓ 必须先检查 str1+str2 == str2+str1
//
// 2. 暴力解中用 substr 拼接验证而不是逐字符比较:
//    ✗ string repeated = ""; for(int j=0;j<len1/i;j++) repeated += candidate;
//      if(repeated == str1) ...   → 每次创建长字符串，浪费内存
//    ✓ 用 i % m 逐字符比较，不创建新字符串
//
// 3. 辗转相减忘记检查前缀匹配:
//    ✗ return gcdOfStrings(str1.substr(str2.size()), str2); // 不检查前缀
//      → 对 "LEET","CODE"，str1 不以 str2 开头，直接截取会得到错误结果
//    ✓ 必须先检查 str1.substr(0, str2.size()) == str2
//
// 4. gcd 函数参数类型问题:
//    ✗ gcd(str1.size(), str2.size())  → size() 返回 size_t (无符号), 某些编译器报警告
//    ✓ gcd((int)str1.size(), (int)str2.size())
//
// 【面试追问】
// Q1: "为什么 str1+str2 == str2+str1 就能保证有公因子字符串？"
//     → 因为等式意味着 str1[i] 具有以 gcd(len1,len2) 为周期的循环性质，
//       str1[(i+len2) % (len1+len2)] 的推导可以证明。
//
// Q2: "能否 O(1) 空间完成验证？"
//     → 见 Solution3_SpaceOpt，用虚拟索引逐字符比较，不真正创建拼接字符串。
//
// Q3: "如果输入是 n 个字符串（而不是2个），找所有字符串的 GCD？"
//     → 依次求两两的 GCD: gcd(gcd(gcd(s1,s2), s3), s4)...
//       因为 GCD 满足结合律。先用拼接验证排除不可能，再取所有长度的 gcd。
//
// Q4: "辗转相减法和辗转相除法的关系？"
//     → 辗转相减每次减一个 str2，辗转相除等于一次性减掉 len1/len2 个 str2。
//       对字符串来说，相除版本就是 str1.substr((len1/len2)*len2) 与 str2 继续递归，
//       但实现上直接取余更简洁: gcd(str2, str1[0..len1%len2-1])。
// ============================================================
