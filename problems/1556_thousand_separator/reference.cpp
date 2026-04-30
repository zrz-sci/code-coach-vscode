// 解法1: 从左到右遍历 - 最简洁的写法
// 时间 O(d), 空间 O(d)，其中 d 为数字位数
//
// 核心: 转字符串后，从左到右扫描
// 当 (s.size() - i) % 3 == 0 且 i > 0 时，在当前字符前插入分隔符
//
// 为什么 (s.size() - i) % 3 == 0 有效？
//   s.size() - i = 从位置 i 到末尾的字符数
//   当它是 3 的倍数时，说明后面是完整的若干组(每组3位)
//   此时应在位置 i 前插入分隔符
//
// 例: "1234567" (len=7)
//   i=0: (7-0)%3=1, 不插入 -> "1"
//   i=1: (7-1)%3=0, 插入!  -> "1.2"
//   i=4: (7-4)%3=0, 插入!  -> "1.234.5"
class Solution1 {
public:
    string thousandSeparator(int n) {
        string s = to_string(n);
        string res;
        for (int i = 0; i < (int)s.size(); i++) {
            // 非开头位置 + 剩余字符数是 3 的倍数 -> 插入分隔符
            if (i > 0 && (s.size() - i) % 3 == 0)
                res += '.';
            res += s[i];
        }
        return res;
    }
};

// 解法2: 从右到左构建 + 反转 - 更直觉的实现
// 时间 O(d), 空间 O(d)
//
// 核心: 从字符串末尾开始取字符，每 3 个字符后插入一个分隔符
// 最后反转字符串得到正确顺序
//
// 例: "1234567"
//   从右往左: 7, 6, 5, '.', 4, 3, 2, '.', 1
//   构建: "765.432.1"
//   反转: "1.234.567"
class Solution2 {
public:
    string thousandSeparator(int n) {
        string s = to_string(n);
        string res;
        int cnt = 0; // 已处理的字符计数

        // 从右往左遍历
        for (int i = (int)s.size() - 1; i >= 0; i--) {
            // 每 3 个字符后插入分隔符
            if (cnt > 0 && cnt % 3 == 0)
                res += '.';
            res += s[i];
            cnt++;
        }

        // 反转得到正确顺序
        reverse(res.begin(), res.end());
        return res;
    }
};

// 解法3: 数学取模法 - 不依赖 to_string (除了最终拼接)
// 时间 O(d), 空间 O(d)
//
// 核心: 每次 n % 1000 提取末三位, n /= 1000 移除末三位
// 非最高位的组需要补前导零(如 1000007 -> "1.000.007")
//
// 例: n = 1234567
//   第一轮: part=567, n=1234 -> 拼上 ".567"
//   第二轮: part=234, n=1    -> 拼上 ".234"
//   第三轮: part=1,   n=0    -> 拼上 "1" (最高位不补零)
//   结果: "1.234.567"
class Solution3 {
public:
    string thousandSeparator(int n) {
        // 特判: n = 0
        if (n == 0) return "0";

        string res;
        bool first = true; // 标记是否为第一组(最末尾的三位)

        while (n > 0) {
            int part = n % 1000;
            n /= 1000;

            string partStr = to_string(part);

            // 非最高位组需要补前导零到 3 位
            // 例: 1000007 中的 "0" 需要变成 "000", "7" 需要变成 "007"
            if (n > 0) {
                while ((int)partStr.size() < 3)
                    partStr = "0" + partStr;
            }

            // 拼接: 非第一组前面加分隔符
            if (first) {
                res = partStr;
                first = false;
            } else {
                res = partStr + "." + res;
            }
        }

        return res;
    }
};

// 解法4: 利用 insert 直接原地插入
// 时间 O(d^2) 最坏(因为 insert), 空间 O(d)
// 代码最短但效率稍低
class Solution4 {
public:
    string thousandSeparator(int n) {
        string s = to_string(n);
        // 从右往左每 3 位插入一个点
        for (int i = (int)s.size() - 3; i > 0; i -= 3) {
            s.insert(i, ".");
        }
        return s;
    }
};

/*
 * 解法对比:
 * ┌────────────────────────┬────────┬────────┬──────────────────────┐
 * │ 解法                   │ 时间   │ 空间   │ 特点                 │
 * ├────────────────────────┼────────┼────────┼──────────────────────┤
 * │ Solution1 从左到右     │ O(d)   │ O(d)   │ 最简洁，推荐         │
 * │ Solution2 从右到左     │ O(d)   │ O(d)   │ 直觉清晰             │
 * │ Solution3 数学取模     │ O(d)   │ O(d)   │ 不依赖 to_string     │
 * │ Solution4 insert       │ O(d^2) │ O(d)   │ 代码最短             │
 * └────────────────────────┴────────┴────────┴──────────────────────┘
 *
 * 详细测试验证:
 *   n = 0          -> "0"
 *   n = 987        -> "987"           (3位，无分隔符)
 *   n = 1234       -> "1.234"         (4位，1个分隔符)
 *   n = 123456789  -> "123.456.789"   (9位，2个分隔符)
 *   n = 1000000    -> "1.000.000"     (前导零要保留!)
 *   n = 2147483647 -> "2.147.483.647" (10位，3个分隔符)
 *
 * 易错点:
 * 1. n = 0 时 to_string 返回 "0"，Solution1/2 自动处理
 *    Solution3 的 while(n>0) 不执行，需要特判
 * 2. 前导零: n = 1000007 -> "1.000.007" 不是 "1.0.7"
 *    Solution3 需要补齐到 3 位
 * 3. 分隔符是点 "." 不是逗号 ","
 * 4. 不要在字符串开头插入分隔符 (i > 0 条件)
 * 5. int 范围: n 最大 2^31-1 = 2147483647 (10位)
 *
 * 面试追问:
 * Q: 不用 to_string?
 *    -> Solution3 数学取模法
 * Q: 处理负数?
 *    -> 检查符号, abs(n) 处理后加回负号
 * Q: 处理小数?
 *    -> 小数点分割, 只对整数部分加分隔符
 * Q: locale 感知?
 *    -> C++ std::locale + std::numpunct
 */
