/*
 * LeetCode 443: 压缩字符串 (String Compression)
 *
 * 【题目本质】
 * 原地游程编码(Run-Length Encoding)：用双指针在原数组上读取连续字符并写入压缩结果。
 * 核心保证：压缩后长度 ≤ 原始长度，所以 write 指针永远不会超过 read 指针。
 *
 * 【解法总览】
 * 解法1: 额外空间法     — O(n) / O(n)   — 辅助理解逻辑
 * 解法2: 双指针原地修改  — O(n) / O(1)   — 面试首选 ⭐
 * 解法3: 双指针(不用to_string) — O(n) / O(1) — 面试追问变体
 */

// ============================================================
// 解法1: 额外空间法 — 用临时字符串收集压缩结果
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最直觉的做法：遍历数组，对每组连续相同字符统计个数，
// 把字符+计数追加到临时字符串，最后拷贝回原数组。
// 不满足 O(1) 空间要求，但逻辑清晰，帮助理解问题本身。
// ============================================================
class Solution1 {
public:
    int compress(vector<char>& chars) {
        int n = chars.size();
        string result;
        
        int i = 0;
        while (i < n) {
            char ch = chars[i];
            int count = 0;
            // 内层循环：统计当前字符连续出现的次数
            while (i < n && chars[i] == ch) {
                i++;
                count++;
            }
            result += ch;
            // 题目规定：count=1 时不写数字
            if (count > 1) {
                result += to_string(count);
            }
        }
        
        // 拷贝回原数组
        for (int j = 0; j < (int)result.size(); j++) {
            chars[j] = result[j];
        }
        return result.size();
    }
};

// ============================================================
// 解法2: 双指针原地修改 — 面试首选 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：用了O(n)额外空间。
// 核心观察：压缩后总长度 ≤ 原始长度（因为 1+digits(count) ≤ count 对所有 count≥1 成立）
// 所以可以直接在原数组上用 write 指针写入，不会覆盖 read 还没扫到的数据。
//
// 【过程可视化】
// 输入: ['a','a','b','b','c','c','c']
//        r                             (read 扫描)
//        w                             (write 写入)
//
// 第1组 'a' x2:
//   read 扫完2个'a' → read=2
//   write: chars[0]='a', chars[1]='2' → write=2
//   数组: ['a','2','b','b','c','c','c']
//                   r
//                   w
//
// 第2组 'b' x2:
//   read 扫完2个'b' → read=4
//   write: chars[2]='b', chars[3]='2' → write=4
//   数组: ['a','2','b','2','c','c','c']
//                           r
//                           w
//
// 第3组 'c' x3:
//   read 扫完3个'c' → read=7
//   write: chars[4]='c', chars[5]='3' → write=6
//   数组: ['a','2','b','2','c','3','c']
//                                       r=7 结束
//                               w=6 → 返回6
// ============================================================
class Solution2 {
public:
    int compress(vector<char>& chars) {
        int n = chars.size();
        int write = 0;  // 写指针
        int read = 0;   // 读指针
        
        while (read < n) {
            char ch = chars[read];
            int count = 0;
            
            // 统计连续相同字符的个数
            // 注意：必须先检查 read < n 防止越界
            while (read < n && chars[read] == ch) {
                read++;
                count++;
            }
            
            // 写入字符本身
            chars[write++] = ch;
            
            // count > 1 才写入数字（题目规定 count=1 不写）
            if (count > 1) {
                // to_string 自动处理多位数（如 12 → "12"）
                string countStr = to_string(count);
                for (char c : countStr) {
                    chars[write++] = c;
                }
            }
        }
        
        // write 就是压缩后数组的长度
        return write;
    }
};

// ============================================================
// 解法3: 双指针原地（不用 to_string）— 面试追问变体
// 时间: O(n)  空间: O(1) (严格 O(1)，无临时字符串)
//
// 【思路】
// 有些面试官要求不用库函数。手动将 count 拆成字符：
// 用取模逐位提取（从低位到高位），写入后再反转为正确顺序。
//
// 【数字写入可视化】
// count = 12:
//   12 % 10 = 2 → 写 '2'   count = 1
//    1 % 10 = 1 → 写 '1'   count = 0
//   写入顺序: '2','1' → 反转为 '1','2' ✓
// ============================================================
class Solution3 {
public:
    int compress(vector<char>& chars) {
        int n = chars.size();
        int write = 0, read = 0;
        
        while (read < n) {
            char ch = chars[read];
            int count = 0;
            while (read < n && chars[read] == ch) {
                read++;
                count++;
            }
            
            chars[write++] = ch;
            
            if (count > 1) {
                int start = write;  // 记录数字开始写入的位置
                // 取模逐位提取（低位先写）
                while (count > 0) {
                    chars[write++] = '0' + (count % 10);
                    count /= 10;
                }
                // 反转数字部分：因为低位先写入，顺序是反的
                // 例如 count=12 → 先写'2'再写'1' → 反转得'1','2'
                reverse(chars.begin() + start, chars.begin() + write);
            }
        }
        
        return write;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | 解法1(额外空间) | O(n) | O(n) | 逻辑清晰，不满足要求 |
// | 解法2(双指针+to_string) | O(n) | O(1)* | 面试首选，简洁 |
// | 解法3(双指针手动) | O(n) | O(1) | 严格O(1)，追问变体 |
//
// *to_string 内部创建临时字符串 O(log count)，但 count≤2000 最多4字符，
//  面试中认为是 O(1)。
//
// 核心关系：解法1→解法2 的优化在于利用"压缩后≤原始"的性质原地写入。
//          解法2→解法3 的变化在于手动替代 to_string。
//
// ============================================================
// 【易错点】
//
// 1. count=1 时误写 "1":
//    ✗ if (count >= 1) 写入数字 → 单个字符'a'变成"a1"
//    ✓ if (count > 1) 写入数字 → 单个字符'a'保持"a"
//
// 2. 内层循环忘记边界检查:
//    ✗ while (chars[read] == ch) → read=n 时越界访问
//    ✓ while (read < n && chars[read] == ch) → 先判断边界
//
// 3. 手动拆数字忘记反转:
//    ✗ count=12 → 写入 '2','1' → "b21" 
//    ✓ count=12 → 写入 '2','1' → reverse → "b12"
//
// 4. 返回值用了 chars.size():
//    ✗ return chars.size(); → 原数组大小未变，还是7
//    ✓ return write; → 写指针位置才是压缩后长度6
//
// 5. write 和 read 初始化位置错误:
//    ✗ write=1 或 read=1 → 跳过了第一个字符
//    ✓ write=0, read=0 → 从头开始
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么原地修改是安全的？怎么证明 write ≤ read？
// → 对每组 count 个相同字符：
//   count=1: 写1个字符，原占1位，1≤1 ✓
//   count=2~9: 写2个(字符+1位数字)，原占2~9位，2≤count ✓  
//   count=10~99: 写3个(字符+2位数字)，原占10~99位，3≤count ✓
//   count=100~999: 写4个，原占100~999位，4≤count ✓
//   归纳：1+floor(log10(count))+1 ≤ count 对所有 count≥2 成立。
//
// Q2: 如果压缩后可能比原数组长（如要求单字符也写"a1"），怎么办？
// → write 可能超过 read，原地不安全。两种方案：
//   A) 两遍扫描：第一遍算出压缩后总长度，从后往前填写（避免覆盖）
//   B) 使用 O(n) 额外空间
//
// Q3: 如果输入是流式数据（每次只给一个字符），如何在线压缩？
// → 维护 (currentChar, count) 状态。每来一个新字符：
//   若相同：count++
//   若不同：输出 currentChar + (count>1 ? count : "")，重置状态
//   流结束时再输出最后一组。
// ============================================================
