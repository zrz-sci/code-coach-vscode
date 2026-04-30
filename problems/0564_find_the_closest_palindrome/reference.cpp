// 【题目本质】
// 给定数字字符串 n，找到与 n 最近的回文数（不含自身），平局取较小值。
// 关键洞察：最近回文一定来自有限候选集（前半部分 +-1/不变 的镜像 + 位数边界）。

// 【解法总览】
// Solution1: 前半部分枚举 + 5候选比较  O(L) / O(L)
// Solution2: 同思路的简洁写法           O(L) / O(L)

// ===================== Solution 1 =====================
// 前半部分枚举法
// 构造5个候选回文，选最近且非自身的
class Solution1 {
    // 根据前半部分构造完整回文
    long buildPalindrome(long half, bool oddLen) {
        long res = half;
        if (oddLen) half /= 10;  // 奇数长度跳过中间位
        while (half > 0) {
            res = res * 10 + half % 10;
            half /= 10;
        }
        return res;
    }
public:
    string nearestPalindromic(string n) {
        int len = n.size();
        long num = stol(n);
        long half = stol(n.substr(0, (len + 1) / 2));
        bool oddLen = (len % 2 == 1);

        // 5个候选：前半-1/不变/+1的镜像 + 两个位数边界
        vector<long> candidates = {
            buildPalindrome(half, oddLen),
            buildPalindrome(half - 1, oddLen),
            buildPalindrome(half + 1, oddLen),
            (long)pow(10, len - 1) - 1,   // 99...9 (少一位)
            (long)pow(10, len) + 1         // 100...01 (多一位)
        };

        long closest = -1;
        for (long cand : candidates) {
            if (cand == num) continue;  // 排除自身
            if (closest == -1 ||
                abs(cand - num) < abs(closest - num) ||
                (abs(cand - num) == abs(closest - num) && cand < closest)) {
                closest = cand;
            }
        }
        return to_string(closest);
    }
};

// ===================== Solution 2 =====================
// 字符串镜像法（不转数字构造）
// 直接在字符串层面操作前半部分
class Solution2 {
    string mirror(string half, bool oddLen) {
        string res = half;
        int start = oddLen ? half.size() - 2 : half.size() - 1;
        for (int i = start; i >= 0; --i)
            res += half[i];
        return res;
    }

    // 前半部分 +1 或 -1（字符串加减法）
    string addToHalf(string half, int delta) {
        int carry = delta;
        for (int i = half.size() - 1; i >= 0 && carry != 0; --i) {
            int d = (half[i] - '0') + carry;
            if (d >= 10) { half[i] = '0'; carry = 1; }
            else if (d < 0) { half[i] = '9'; carry = -1; }
            else { half[i] = '0' + d; carry = 0; }
        }
        if (carry == 1) half = "1" + half;
        // 前导零处理：如果变成 "0"，保留
        if (half.size() > 1 && half[0] == '0') half = half.substr(1);
        return half;
    }
public:
    string nearestPalindromic(string n) {
        int len = n.size();
        long num = stol(n);
        string half = n.substr(0, (len + 1) / 2);
        bool oddLen = (len % 2 == 1);

        vector<string> candidates = {
            mirror(half, oddLen),
            mirror(addToHalf(half, -1), oddLen),
            mirror(addToHalf(half, 1), oddLen),
            string(len - 1, '9'),                             // 99...9
            "1" + string(len - 1, '0') + "1"                  // 100...01
        };

        long closest = -1;
        for (auto& s : candidates) {
            if (s.empty() || s == "0" && len > 1) continue;
            long cand = stol(s);
            if (cand == num) continue;
            if (closest == -1 ||
                abs(cand - num) < abs(closest - num) ||
                (abs(cand - num) == abs(closest - num) && cand < closest)) {
                closest = cand;
            }
        }
        return to_string(closest);
    }
};

// 【解法对比】
// Solution1: 数字层面操作，简洁直观，面试首选
// Solution2: 字符串层面操作，避免溢出风险但代码更长

// 【易错点】
// 1. 必须处理位数边界（99...9 和 100...01）
// 2. 数值范围 [1, 10^18-1]，必须用 long long
// 3. n 本身是回文时必须排除自身
// 4. 前半 -1 可能导致位数减少（如 100 -> 前半 10 -> 9 -> 回文 9）
// 5. 平局时返回较小的回文数

// 【面试追问】
// Q1: 为什么候选只有5个？位数相同时前半+-1涵盖了最近的，位数不同只可能差1
// Q2: pow(10, len) 会溢出吗？len<=18, 10^18 在 long long 范围内
// Q3: 如何处理 n="1" 的特殊情况？候选包含 10^0-1=0，自然覆盖
