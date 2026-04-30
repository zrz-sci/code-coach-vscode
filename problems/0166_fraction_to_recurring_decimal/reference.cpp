/*
 * 【题目本质】
 * 模拟手工长除法，检测小数部分的循环节。
 * 核心：余数一旦重复，小数就开始循环。用哈希表记录余数首次出现位置。
 *
 * 【解法总览】
 * 解法1: 长除法 + 哈希表(直接构建)  O(d)/O(d) ⭐ 面试首选
 * 解法2: 长除法 + 分段构建           O(d)/O(d)  逻辑更清晰
 *
 * 长除法示例: 4 / 333
 *
 *     0 . 0  1  2  (0  1  2 ...)
 *   ___________
 *   333 | 4.000000...
 *         0       → 商0, 余4    记录 {4→pos0}
 *         40      → 商0, 余40   记录 {40→pos1}(*)
 *         400     → 商1, 余67   记录 {67→pos2}
 *         670     → 商2, 余4    余数4已出现在pos0!
 *
 *   结果: "0." + "(012)" = "0.(012)"
 *         循环节从 pos0 开始
 *
 *   (*) 注意: 商0也要写出来，不能跳过
 */

// ===================== 解法1: 长除法 + 哈希表 (直接构建) =====================
// 思路：在 result 字符串上直接构建，检测到循环时用 insert 插入括号
// 为什么用 long long：INT_MIN 取绝对值会溢出 int
class Solution1 {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";

        string result;

        // 异号为负（XOR判断）
        if ((numerator < 0) ^ (denominator < 0)) {
            result += '-';
        }

        // 转 long long 防止 abs(INT_MIN) 溢出
        long long num = llabs((long long)numerator);
        long long den = llabs((long long)denominator);

        // 整数部分
        result += to_string(num / den);
        long long rem = num % den;

        if (rem == 0) return result;  // 整除，无小数

        result += '.';

        // 哈希表: 余数 → 该余数对应商出现在 result 的位置
        unordered_map<long long, int> seen;

        while (rem != 0) {
            if (seen.count(rem)) {
                // 循环检测：在循环起始位置插入 '('
                result.insert(seen[rem], "(");
                result += ')';
                return result;
            }

            // 记录余数和其商将要写入的位置
            seen[rem] = result.size();

            // 模拟一步长除法
            rem *= 10;
            result += to_string(rem / den);
            rem %= den;
        }

        return result;  // 有限小数
    }
};

// ===================== 解法2: 分段构建 =====================
// 思路：小数部分单独构建成 string，最后拼接
// 为什么分开：避免 insert 操作（O(n)），改用 substr 拼接
class Solution2 {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";

        string ans;
        bool neg = (numerator < 0) ^ (denominator < 0);
        if (neg) ans += '-';

        long long num = llabs((long long)numerator);
        long long den = llabs((long long)denominator);

        ans += to_string(num / den);
        long long rem = num % den;
        if (rem == 0) return ans;

        ans += '.';

        // 单独构建小数部分
        string decimal;
        unordered_map<long long, int> seen;

        while (rem != 0) {
            if (seen.count(rem)) {
                int start = seen[rem];
                ans += decimal.substr(0, start);
                ans += '(' + decimal.substr(start) + ')';
                return ans;
            }
            seen[rem] = decimal.size();
            rem *= 10;
            decimal += to_string(rem / den);
            rem %= den;
        }

        ans += decimal;
        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 直接构建 | O(d) | O(d) | 一次构建，用 insert 加括号 |
 * | 分段构建 | O(d) | O(d) | 小数分开，避免 insert |
 *
 * 两种方法本质相同，选自己更容易写对的即可。
 *
 * 【易错点】
 * 1. abs(INT_MIN) 溢出 → 必须先转 long long 再取绝对值
 * 2. numerator=0 忘处理 → 可能输出 "-0"
 * 3. 符号判断用 || 而非 ^ → 同号负数误判为负
 * 4. 余数用 int → rem*10 溢出
 * 5. 忘记商为0也要写入 → 4/333 的 "0.0..." 第一个0不能跳过
 *
 * 【面试追问】
 * Q1: 循环节最长多长？ → 最长为 denominator-1（余数种类数）
 * Q2: 为什么有理数小数一定有限或循环？ → 余数有限集，鸽巢原理
 * Q3: 反过来循环小数转分数？ → 代数消元：x=0.abc(def), 列方程消循环
 */
