/*
 * LeetCode 67: Add Binary (二进制求和)
 * 
 * 【题目本质】
 * 模拟竖式加法，只不过是二进制（逢二进一）。
 * 和十进制加法唯一的区别：% 2 和 / 2 代替 % 10 和 / 10。
 *
 * 【解法总览】
 * 解法1: 模拟竖式加法 — O(max(m,n)) / O(max(m,n)) — 面试首选 ⭐
 * 解法2: 位运算模拟   — O(max(m,n)²) / O(max(m,n)) — 加分项（不用加法运算符）
 */

// ============================================================
// 解法1: 模拟竖式加法 — 从最低位逐位相加
// 时间: O(max(m,n))  空间: O(max(m,n)) 用于存储结果
//
// 【思路】
// 和我们手算竖式加法一模一样：
//   1. 从最低位（字符串末尾）开始
//   2. 每位做: digitA + digitB + carry
//   3. 当前位 = sum % 2, 新进位 = sum / 2
//   4. 两个指针各自向前移动，越界的当 0
//   5. 最后如果 carry 还有，再补一位
//
// 竖式加法过程演示:
//
//     a:    1 0 1 0
//     b:    1 0 1 1
//          --------
//  第1轮:  0+1+carry(0) = 1 → 写'1', carry=0
//  第2轮:  1+1+carry(0) = 2 → 写'0', carry=1
//  第3轮:  0+0+carry(1) = 1 → 写'1', carry=0
//  第4轮:  1+1+carry(0) = 2 → 写'0', carry=1
//  剩余carry=1           → 写'1'
//          --------
//  result(构建中): "10101"（已经是倒序追加后翻转的结果）
//  最终: "10101"
// ============================================================
class Solution1 {
public:
    string addBinary(string a, string b) {
        string result;
        int i = a.size() - 1;  // a 的指针，从末尾（最低位）开始
        int j = b.size() - 1;  // b 的指针，从末尾（最低位）开始
        int carry = 0;         // 进位
        
        // 三个条件任一满足就继续：a还有位 / b还有位 / 还有进位
        // 为什么 carry 也要作为条件？
        // 因为 "11" + "1" 最后 carry=1，不处理就丢失最高位
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            
            // 取 a 的当前位（越界则视为 0）
            if (i >= 0) {
                sum += a[i] - '0';  // 字符 '0'/'1' 转整数 0/1
                i--;
            }
            
            // 取 b 的当前位（越界则视为 0）
            if (j >= 0) {
                sum += b[j] - '0';
                j--;
            }
            
            // sum 的范围是 [0, 3]（最多 1+1+1）
            result += (sum % 2) + '0';  // 当前位：对 2 取余，再转回字符
            carry = sum / 2;            // 新进位：对 2 整除
        }
        
        // 我们从低位往高位追加，结果是反的，需要翻转
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 解法2: 位运算模拟（不使用加法运算符）
// 时间: O(max(m,n)²) 最坏  空间: O(max(m,n))
//
// 【思路】
// 面试追问"不用 + 怎么做？"时使用。
// 加法可以分解为两步：
//   1. 无进位和 = a XOR b（相同为0，不同为1）
//   2. 进位 = (a AND b) 左移一位（都是1时产生进位）
// 然后把"无进位和"和"进位"再加（重复上述过程），直到进位为 0。
//
// 这就是计算机硬件中全加器的工作原理。
//
// 示例: a="11", b="1" (补齐: a="11", b="01")
//   Round1: xor="10", and="01", carry=and<<1="010"
//           对齐: a="010", b="010"
//   Round2: xor="000", and="010", carry="0100"
//           对齐: a="0000", b="0100"
//   Round3: xor="0100", and="0000" → 进位全0，结束
//           结果: "100"
// ============================================================
class Solution2 {
public:
    string addBinary(string a, string b) {
        // 对齐长度
        while (a.size() < b.size()) a = "0" + a;
        while (b.size() < a.size()) b = "0" + b;
        
        // 循环直到 b（进位）全为 0
        while (hasOne(b)) {
            string xorResult = bitwiseXor(a, b);    // 无进位和
            string andResult = bitwiseAnd(a, b);     // 两位都为1的位置
            string carryResult = leftShift(andResult); // 进位左移一位
            
            a = xorResult;
            b = carryResult;
            
            // 重新对齐长度（左移可能让 b 变长）
            while (a.size() < b.size()) a = "0" + a;
            while (b.size() < a.size()) b = "0" + b;
        }
        
        // 去除前导零（但至少保留一个 '0'）
        int start = 0;
        while (start < (int)a.size() - 1 && a[start] == '0') start++;
        return a.substr(start);
    }
    
private:
    // 检查字符串中是否有 '1'
    bool hasOne(const string& s) {
        for (char c : s) {
            if (c == '1') return true;
        }
        return false;
    }
    
    // 逐位 XOR：相同为 '0'，不同为 '1'
    string bitwiseXor(const string& a, const string& b) {
        string res(a.size(), '0');
        for (int i = 0; i < (int)a.size(); i++) {
            res[i] = ((a[i] - '0') ^ (b[i] - '0')) + '0';
        }
        return res;
    }
    
    // 逐位 AND：都是 '1' 才为 '1'
    string bitwiseAnd(const string& a, const string& b) {
        string res(a.size(), '0');
        for (int i = 0; i < (int)a.size(); i++) {
            res[i] = ((a[i] - '0') & (b[i] - '0')) + '0';
        }
        return res;
    }
    
    // 二进制左移一位 = 末尾追加 '0'
    string leftShift(const string& s) {
        return s + "0";
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1 (竖式加法)      | 解法2 (位运算)         |
// |------------|----------------------|----------------------|
// | 核心思想   | 逐位加 + 进位         | XOR + AND + 左移循环  |
// | 时间复杂度 | O(max(m,n))           | O(max(m,n)²) 最坏     |
// | 空间复杂度 | O(max(m,n))           | O(max(m,n))           |
// | 代码量     | ~15 行                | ~50 行                |
// | 面试推荐   | ⭐ 首选                | 追问"不用加法"时使用   |
//
// 面试中写解法1即可，解法2作为"了解硬件加法器原理"的加分项。
//
// 【易错点】
//
// 1. 循环条件漏掉 carry：
//    ✗ while (i >= 0 || j >= 0)  — "11"+"1" 得到 "00" 丢失最高位进位
//    ✓ while (i >= 0 || j >= 0 || carry)  — 正确得到 "100"
//
// 2. 字符与整数混淆：
//    ✗ result += sum % 2;         — 追加 ASCII 0 或 1（不可见字符）
//    ✓ result += (sum % 2) + '0'; — 追加字符 '0' 或 '1'
//
// 3. 忘记翻转结果：
//    我们从低位往高位构建，追加顺序是反的，最后必须 reverse。
//
// 4. 用 insert(0,...) 代替 reverse：
//    ✗ result.insert(0, 1, ch);   — 每次 O(n)，总体 O(n²)
//    ✓ result.push_back(ch) + 最后 reverse — 总体 O(n)
//
// 【面试追问】
//
// Q1: 为什么不能转成 int/long long 直接算？
//     → 字符串最长 10⁴ 位，任何内置整数类型都放不下。
//
// Q2: 如果是十进制字符串相加呢？代码改哪里？
//     → % 2 → % 10, / 2 → / 10。框架完全一样。(LeetCode 415)
//
// Q3: 能否不用 + 运算符实现加法？
//     → 用 XOR 做无进位和，AND+左移做进位，循环直到进位为 0。
//        这就是解法2，也是计算机硬件全加器的原理。
//
// Q4: 这个竖式加法模板还能用在哪些题？
//     → LeetCode 2 (链表两数相加)、415 (十进制字符串相加)、
//        66 (加一)、43 (字符串相乘中的逐位累加步骤)。
//        模板核心不变，只是载体和进制不同。
// ============================================================
