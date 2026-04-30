/*
 * LeetCode 831: Masking Personal Information (隐藏个人信息)
 *
 * 【题目本质】
 * 判断输入是邮箱还是电话号码，按规则进行掩码处理。
 * 纯字符串模拟题，关键是正确解析和格式化。
 *
 * 【解法总览】
 * 解法1: 分支处理（邮箱 vs 电话）  — O(n) / O(n) — 面试首选 ⭐
 * 解法2: 提取 + 格式化（更模块化）  — O(n) / O(n) — 代码更清晰
 */

// ============================================================
// 解法1: 分支处理 — 面试首选 ⭐
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 1. 判断类型：包含 '@' → 邮箱，否则 → 电话
// 2. 邮箱处理：
//    - 全部转小写
//    - 找 '@' 分隔 name 和 domain
//    - name 保留首尾，中间替换为 "*****"
// 3. 电话处理：
//    - 提取所有数字字符
//    - 保留最后 4 位，其余掩码
//    - 根据国家码长度（总位数 - 10）选择格式
//
// 邮箱示例:
//   "LeetCode@LeetCode.com"
//   → 小写: "leetcode@leetcode.com"
//   → name="leetcode", domain="@leetcode.com"
//   → 掩码: "l" + "*****" + "e" + "@leetcode.com"
//   → 输出: "l*****e@leetcode.com"
//
// 电话示例:
//   "1(234)567-890"
//   → 提取数字: "1234567890" (10位)
//   → 国家码: 0位
//   → 输出: "***-***-7890"
//
//   "+111 (202) 555-0191"
//   → 提取数字: "11120255501​91" (13位)
//   → 国家码: 3位
//   → 输出: "+***-***-***-0191"
// ============================================================
class Solution1 {
public:
    string maskPII(string s) {
        // 判断类型：邮箱包含 '@'
        if (s.find('@') != string::npos) {
            return maskEmail(s);
        }
        return maskPhone(s);
    }

private:
    string maskEmail(const string& s) {
        // 找 '@' 位置
        int atPos = s.find('@');

        // 提取 name 和 domain，全部转小写
        string name = s.substr(0, atPos);
        string domain = s.substr(atPos); // 包含 '@'

        // 转小写
        for (char& c : name) c = tolower(c);
        for (char& c : domain) c = tolower(c);

        // name 保留首尾，中间 5 个 '*'
        string maskedName = string(1, name.front()) + "*****" + string(1, name.back());

        return maskedName + domain;
    }

    string maskPhone(const string& s) {
        // 提取所有数字
        string digits;
        for (char c : s) {
            if (c >= '0' && c <= '9') {
                digits += c;
            }
        }

        // 最后 4 位保留
        string last4 = digits.substr(digits.size() - 4);

        // 本地号码部分（固定格式）
        string localPart = "***-***-" + last4;

        // 国家码长度
        int countryLen = (int)digits.size() - 10;

        if (countryLen == 0) {
            // 没有国家码
            return localPart;
        }

        // 有国家码："+*...*-" 前缀
        return "+" + string(countryLen, '*') + "-" + localPart;
    }
};

// ============================================================
// 解法2: 提取 + 格式化（更模块化的写法）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 和解法1 逻辑完全一致，但把字符串操作拆分成更小的辅助函数，
// 让代码更易读、更易测试。
//
// 电话号码的格式化规则:
//   总位数 | 国家码 | 格式
//   10     | 0      | "***-***-XXXX"
//   11     | 1      | "+*-***-***-XXXX"
//   12     | 2      | "+**-***-***-XXXX"
//   13     | 3      | "+***-***-***-XXXX"
// ============================================================
class Solution2 {
public:
    string maskPII(string s) {
        if (isEmail(s)) {
            return processEmail(s);
        }
        return processPhone(s);
    }

private:
    // 判断是否是邮箱
    bool isEmail(const string& s) {
        for (char c : s) {
            if (c == '@') return true;
        }
        return false;
    }

    // 转小写辅助函数
    string toLowerStr(const string& s) {
        string result = s;
        for (char& c : result) {
            if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';
            }
        }
        return result;
    }

    // 处理邮箱
    string processEmail(const string& s) {
        string lower = toLowerStr(s);
        int atPos = lower.find('@');
        // name 部分：首字符 + ***** + 尾字符
        char first = lower[0];
        char last = lower[atPos - 1];
        // domain 部分（包含 @）
        string domain = lower.substr(atPos);
        return string(1, first) + "*****" + string(1, last) + domain;
    }

    // 提取数字辅助函数
    string extractDigits(const string& s) {
        string digits;
        for (char c : s) {
            if (c >= '0' && c <= '9') {
                digits += c;
            }
        }
        return digits;
    }

    // 处理电话号码
    string processPhone(const string& s) {
        string digits = extractDigits(s);
        int totalDigits = digits.size();
        int countryCodeLen = totalDigits - 10;

        // 最后 4 位
        string last4 = digits.substr(totalDigits - 4);

        // 组装结果
        string result;

        // 国家码部分
        if (countryCodeLen > 0) {
            result += "+";
            for (int i = 0; i < countryCodeLen; i++) {
                result += '*';
            }
            result += "-";
        }

        // 本地号码掩码部分: ***-***-
        result += "***-***-";

        // 最后 4 位明文
        result += last4;

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 代码量 | 可读性 | 适用场景 |
// |------|--------|--------|----------|
// | 解法1 | 较短 | 紧凑 | 面试快速写出 |
// | 解法2 | 较长 | 清晰模块化 | 工程实践 |
//
// 两种解法复杂度完全相同，只是代码组织方式不同。
// 面试推荐解法1（写得快），工程实践推荐解法2（可维护性好）。
//
// 【易错点】
//
// 1. 邮箱 name 只有 2 字符时:
//    ✗ name="AB" → "A*B" (只放1个星号)
//    ✓ name="AB" → "a*****b" (固定5个星号)
//    题目明确说中间替换为 5 个星号，和 name 长度无关
//
// 2. 忘记 domain 也要转小写:
//    ✗ "LeetCode@LeetCode.com" → "l*****e@LeetCode.com"
//    ✓ "LeetCode@LeetCode.com" → "l*****e@leetcode.com"
//
// 3. 电话号码分隔符处理不全:
//    ✗ 只过滤 '-'，忘了 '+', '(', ')', ' '
//    ✓ 过滤所有非数字字符
//
// 4. 10 位电话不需要 '+' 前缀:
//    ✗ "+***-***-7890" (对10位号码也加了+)
//    ✓ "***-***-7890" (10位是本地号码，无国家码)
//
// 5. substr 参数理解错误:
//    ✗ digits.substr(digits.size() - 4, 4) 和 digits.substr(digits.size() - 4)
//    两者结果相同，但第一种更明确。注意不要写成 substr(4) (从位置4开始)
//
// 【面试追问】
//
// Q1: "如何判断输入是邮箱还是电话？"
//   → 检查是否包含 '@'。题目保证输入合法，不需要复杂验证。
//
// Q2: "如果输入可能不合法，怎么增加验证？"
//   → 邮箱：验证 '@' 之前有字符，之后有 '.'，'.' 不在首尾。
//     电话：验证提取的数字为 10-13 位。
//     可以用正则表达式进行更严格的验证。
//
// Q3: "如何支持国际化的邮箱格式？"
//   → 国际化域名(IDN)和 Unicode 邮箱名需要特殊处理，
//     不能简单用 ASCII tolower。需要 ICU 库或类似工具。
//
// Q4: "这题有什么工程上的启示？"
//   → PII (Personally Identifiable Information) 脱敏是实际工程中
//     非常重要的需求。生产环境中需要考虑日志脱敏、数据库加密等。
// ============================================================
