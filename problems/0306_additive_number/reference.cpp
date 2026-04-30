/*
 * 【题目本质】
 * 枚举前两个数，后续序列完全确定 → 验证是否匹配原字符串
 * 核心难点：大数处理（字符串长度可达35）、前导零约束
 *
 * 【解法总览】
 * Solution1: 枚举 + 字符串加法 ⭐⭐⭐⭐⭐  O(n^3) / O(n) — 处理大数安全
 * Solution2: 枚举 + 数值回溯      ⭐⭐⭐    O(n^2) / O(n) — 简洁但有溢出风险
 */

// ===================== Solution1: 枚举 + 字符串加法（推荐） =====================
class Solution1 {
public:
    bool isAdditiveNumber(string num) {
        int n = num.size();
        // 枚举第一个数长度 i，第二个数长度 j
        for (int i = 1; i <= n / 2; i++) {
            for (int j = 1; max(i, j) <= n - i - j; j++) {
                if (check(num, i, j)) return true;
            }
        }
        return false;
    }

private:
    bool check(const string& num, int len1, int len2) {
        string s1 = num.substr(0, len1);
        string s2 = num.substr(len1, len2);
        // 前导零检查：长度 >1 的数不能以 '0' 开头
        if (s1.size() > 1 && s1[0] == '0') return false;
        if (s2.size() > 1 && s2[0] == '0') return false;

        int start = len1 + len2;
        while (start < (int)num.size()) {
            string sum = addStrings(s1, s2);
            // 检查剩余字符串是否以 sum 开头
            if (num.compare(start, sum.size(), sum) != 0) return false;
            start += sum.size();
            s1 = s2;
            s2 = sum;
        }
        return start == (int)num.size(); // 必须消耗完所有字符
    }

    // 字符串大数加法（同 LC 415）
    string addStrings(const string& a, const string& b) {
        string result;
        int carry = 0;
        int i = a.size() - 1, j = b.size() - 1;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            result.push_back('0' + sum % 10);
            carry = sum / 10;
        }
        reverse(result.begin(), result.end());
        return result;
    }
};

// ===================== Solution2: 枚举 + 数值回溯 =====================
// 注意：当 num.length > 18 时可能溢出，仅适合面试快速写出
class Solution2 {
public:
    bool isAdditiveNumber(string num) {
        int n = num.size();
        for (int i = 1; i <= n / 2; i++) {
            if (i > 1 && num[0] == '0') break; // 前导零剪枝
            for (int j = 1; max(i, j) <= n - i - j; j++) {
                if (j > 1 && num[i] == '0') break; // 前导零剪枝
                long long n1 = stoll(num.substr(0, i));
                long long n2 = stoll(num.substr(i, j));
                if (verify(num, i + j, n1, n2)) return true;
            }
        }
        return false;
    }

private:
    bool verify(const string& num, int start, long long n1, long long n2) {
        if (start == (int)num.size()) return true;
        long long sum = n1 + n2;
        string sumStr = to_string(sum);
        if (num.compare(start, sumStr.size(), sumStr) != 0) return false;
        return verify(num, start + sumStr.size(), n2, sum);
    }
};

/*
 * 【解法对比】
 * ┌─────────────────────┬──────────┬──────────┬──────────────────────┐
 * │ 解法                │ 时间     │ 空间     │ 特点                 │
 * ├─────────────────────┼──────────┼──────────┼──────────────────────┤
 * │ Solution1 字符串加法 │ O(n^3)   │ O(n)     │ 大数安全，推荐首选   │
 * │ Solution2 数值回溯   │ O(n^2)   │ O(n)递归 │ 简洁但溢出风险       │
 * └─────────────────────┴──────────┴──────────┴──────────────────────┘
 *
 * 【易错点】
 * 1. 前导零：长度 >1 的数以 '0' 开头 → 非法（但 "0" 本身合法）
 * 2. 枚举边界：max(i, j) <= n - i - j，保证第三个数至少有 max(i,j) 位
 * 3. 大数溢出：num.length 可达 35 位，超出 long long 范围
 * 4. 完整匹配：验证结束时 start 必须恰好等于 num.size()
 *
 * 【面试追问】
 * Q1: 大数如何处理？→ 字符串加法（逐位相加+进位）
 * Q2: 时间复杂度？→ O(n^2) 枚举 × O(n) 验证 = O(n^3)
 * Q3: 如何输出具体序列？→ check 中用 vector 记录，返回 true 时输出
 */
