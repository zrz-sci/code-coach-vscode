/*
 * LeetCode 468: 验证IP地址 (Validate IP Address)
 *
 * 【题目本质】
 * 字符串解析 + 规则校验：按分隔符拆分，逐段检查是否满足 IPv4/IPv6 的格式规则。
 * 没有复杂算法，关键是规则理清楚、边界处理干净。
 *
 * 【解法总览】
 * 解法1: split + 逐段验证 — O(n) / O(n) — 面试首选，逻辑清晰
 * 解法2: 双指针逐字符验证 — O(n) / O(1) — 空间优化，追问时用
 *
 * 【IPv4 规则】
 * - 恰好 4 段，由 '.' 分割
 * - 每段: 纯数字, 长度 1~3, 数值 0~255, 无前导零(除了"0"本身)
 *
 * 【IPv6 规则】
 * - 恰好 8 段，由 ':' 分割
 * - 每段: 长度 1~4, 只含 0-9/a-f/A-F, 允许前导零
 */

// ============================================================
// 解法1: split + 逐段验证
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最自然的做法：先看字符串含 '.' 还是 ':'，决定走 IPv4 还是 IPv6 路径。
// 然后按分隔符 split，逐段检验规则。
// 为什么选这个方法？因为逻辑层次清晰：split 一步，validate 一步，不容易出错。
//
// 【关键细节】
// - split 必须保留空段（"1.1.1.1." 应产生 5 段，最后一段为空 → 非法）
// - IPv4 的 stoi 之前必须先逐字符检查是否全是数字
//   （因为 stoi("1e1") 不会报错，会返回 1，造成误判）
// - IPv4 前导零：size>1 && 首字符=='0' 才非法，"0" 本身是合法的
// ============================================================
class Solution1 {
public:
    string validIPAddress(string queryIP) {
        if (queryIP.find('.') != string::npos) {
            return isValidIPv4(queryIP) ? "IPv4" : "Neither";
        }
        if (queryIP.find(':') != string::npos) {
            return isValidIPv6(queryIP) ? "IPv6" : "Neither";
        }
        return "Neither";
    }

private:
    // 按分隔符拆分，保留空段（这是和 Python split 的关键区别）
    // 例如 "1.1.1.1." → ["1","1","1","1",""]，空尾段不能丢
    vector<string> split(const string& s, char delim) {
        vector<string> parts;
        string cur;
        for (char c : s) {
            if (c == delim) {
                parts.push_back(cur);
                cur.clear();
            } else {
                cur += c;
            }
        }
        parts.push_back(cur); // 别忘了最后一段！
        return parts;
    }

    bool isValidIPv4(const string& ip) {
        vector<string> parts = split(ip, '.');
        // 规则1: 必须恰好 4 段
        if (parts.size() != 4) return false;

        for (const string& part : parts) {
            // 规则2: 不能为空，长度不超过 3
            if (part.empty() || part.size() > 3) return false;

            // 规则3: 每个字符必须是数字（先检查再 stoi，避免 "1e1" 的坑）
            for (char c : part) {
                if (!isdigit(c)) return false;
            }

            // 规则4: 不能有前导零（"0" 合法，"00"/"01" 非法）
            if (part.size() > 1 && part[0] == '0') return false;

            // 规则5: 数值范围 [0, 255]
            int val = stoi(part);
            if (val > 255) return false;
        }
        return true;
    }

    bool isValidIPv6(const string& ip) {
        vector<string> parts = split(ip, ':');
        // 规则1: 必须恰好 8 段
        if (parts.size() != 8) return false;

        string hexChars = "0123456789abcdefABCDEF";
        for (const string& part : parts) {
            // 规则2: 每段长度 1~4
            if (part.empty() || part.size() > 4) return false;

            // 规则3: 每个字符必须是合法十六进制（大小写都算）
            for (char c : part) {
                if (hexChars.find(c) == string::npos) return false;
            }
            // IPv6 允许前导零，不需要额外检查
            // IPv6 不需要检查数值范围（4位hex最大=FFFF=65535，题目只要求字符合法）
        }
        return true;
    }
};


// ============================================================
// 解法2: 双指针逐字符验证（无 split，O(1) 额外空间）
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1 的 split 产生 O(n) 空间。如果面试官追问"能否 O(1) 空间？"，
// 我们可以用双指针 [i, j) 直接在原字符串上定位每一段，原地验证。
//
// 核心改动：不再 split 产生子串，而是用 i 和 j 标记当前段的边界，
// 直接在 [i, j) 范围内做检查。
//
// 首尾分隔符的处理：如果首字符或尾字符是分隔符，直接返回 false，
// 这样可以避免空段检测的遗漏。
// ============================================================
class Solution2 {
public:
    string validIPAddress(string queryIP) {
        if (queryIP.find('.') != string::npos) {
            return checkIPv4(queryIP) ? "IPv4" : "Neither";
        }
        if (queryIP.find(':') != string::npos) {
            return checkIPv6(queryIP) ? "IPv6" : "Neither";
        }
        return "Neither";
    }

private:
    bool checkIPv4(const string& ip) {
        int n = ip.size();
        // 首尾不能是分隔符（快速排除 ".1.1.1" 和 "1.1.1." 这类情况）
        if (n == 0 || ip[0] == '.' || ip[n - 1] == '.') return false;

        int segCount = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            // j 前进到段末尾（下一个 '.' 或字符串结尾）
            while (j < n && ip[j] != '.') j++;

            int len = j - i;
            if (len == 0 || len > 3) return false;

            // 逐字符检查是否全是数字
            for (int k = i; k < j; k++) {
                if (!isdigit(ip[k])) return false;
            }

            // 前导零检查
            if (len > 1 && ip[i] == '0') return false;

            // 数值范围（手动计算避免 substr 的 O(k) 拷贝）
            int val = 0;
            for (int k = i; k < j; k++) {
                val = val * 10 + (ip[k] - '0');
            }
            if (val > 255) return false;

            segCount++;
            i = j + 1; // 跳过分隔符 '.'
        }
        return segCount == 4;
    }

    bool checkIPv6(const string& ip) {
        int n = ip.size();
        if (n == 0 || ip[0] == ':' || ip[n - 1] == ':') return false;

        int segCount = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && ip[j] != ':') j++;

            int len = j - i;
            if (len == 0 || len > 4) return false;

            for (int k = i; k < j; k++) {
                char c = ip[k];
                // 合法十六进制字符: 0-9, a-f, A-F
                bool valid = (c >= '0' && c <= '9') ||
                             (c >= 'a' && c <= 'f') ||
                             (c >= 'A' && c <= 'F');
                if (!valid) return false;
            }

            segCount++;
            i = j + 1;
        }
        return segCount == 8;
    }
};


/*
 * ============================================================
 * 【解法对比】
 * 
 * 解法1 (split+验证) vs 解法2 (双指针):
 * - 逻辑完全相同，区别在于是否显式创建子串
 * - 解法1 代码更清晰，split 后逐段处理，面试中推荐
 * - 解法2 省去 O(n) 空间，面试追问 "能否 O(1) 空间" 时给出
 * - 两者时间都是 O(n)，实际性能差异可忽略
 *
 * 面试中的策略：先写解法1拿到 AC，如果面试官追问空间优化再改解法2
 *
 * ============================================================
 * 【易错点】
 * 
 * 1. split 丢失尾部空段:
 *    ✗ 用 getline(ss, token, '.') 在某些实现中不会产生尾部空 token
 *    ✓ 手写 split，循环结束后 push_back(cur) 收集最后一段
 *
 * 2. IPv4 前导零误杀 "0":
 *    ✗ if (part[0] == '0') return false;  // "0" 被误判为非法
 *    ✓ if (part.size() > 1 && part[0] == '0') return false;
 *
 * 3. stoi 对非纯数字字符串不报错:
 *    ✗ 直接 stoi("1e1") 返回 1，不抛异常，导致 "1e1" 被误判合法
 *    ✓ 先逐字符 isdigit() 检查通过后，再 stoi
 *
 * 4. IPv6 大小写遗漏:
 *    ✗ 只检查 c >= 'a' && c <= 'f'，忘记 'A'-'F'
 *    ✓ 同时检查 (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')
 *
 * 5. 空输入或只有分隔符:
 *    ✗ 没处理 ""、"."、":" 这些边界输入
 *    ✓ split 后 parts.size() != 4/8 自然拦住；或首尾分隔符检查拦住
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 时间复杂度？能更快吗？
 * → O(n)，每个字符最多访问常数次。已经最优——至少要读一遍输入。
 *
 * Q2: 能否 O(1) 空间？
 * → 可以。用双指针在原字符串上逐段定位验证（解法2），不需要 split。
 *
 * Q3: 如果要支持 IPv6 缩写形式（:: 代表连续全零段），怎么改？
 * → 核心改动:
 *   - 允许最多一个 "::"（多个非法）
 *   - "::" 代表 1~7 个全零段
 *   - split 后段数 <= 8，用 8 - 实际段数 = 被 :: 省略的段数
 *   - 特殊: "::1"(开头), "fe80::"(结尾), "::"(全零)
 *
 * Q4: 如果输入量极大（每秒百万次验证），有什么工程优化？
 * → 用 string_view 避免拷贝；手动累加数值避免 stoi 开销；
 *   长度预检（IPv4 最长15字符，IPv6 最长39字符）提前短路。
 *
 * ============================================================
 * 【相关题型】
 * - 93. 复原IP地址 — 本题的逆问题，用回溯枚举所有合法IPv4分割方式，
 *   复用本题的 "段验证" 逻辑（0-255、无前导零）
 * - 8. 字符串转整数(atoi) — 同类字符串解析题，核心是逐字符+边界处理
 * - 65. 有效数字 — 更复杂的字符串验证，推荐有限状态机方法
 * ============================================================
 */
