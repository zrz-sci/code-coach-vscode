// 【题目本质】
// 打乱的数字英文单词字母中，还原原始数字并升序输出。
// 核心：利用各数字英文单词中的独特字母，按层级推断每个数字的出现次数。

// 【解法总览】
// Solution1: 独特字母推断法 O(n) / O(1) ⭐推荐
//   - 第1层(独特字母): z→0, w→2, u→4, x→6, g→8
//   - 第2层(扣除后独特): o→1, h→3, f→5, s→7
//   - 第3层(最后剩余): i→9

// ===================== Solution1: 独特字母推断法 =====================
// 数字单词与独特字母映射：
// zero(z), one(o*), two(w), three(h*), four(u),
// five(f*), six(x), seven(s*), eight(g), nine(i*)
// 带*表示需要在前一轮扣除后才独特
class Solution1 {
public:
    string originalDigits(string s) {
        // 统计字母频率
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;

        vector<int> count(10, 0);

        // 第1层：这些字母只出现在一个数字中
        count[0] = freq['z' - 'a'];            // z 只在 zero
        count[2] = freq['w' - 'a'];            // w 只在 two
        count[4] = freq['u' - 'a'];            // u 只在 four
        count[6] = freq['x' - 'a'];            // x 只在 six
        count[8] = freq['g' - 'a'];            // g 只在 eight

        // 第2层：扣除已确定数字后变为独特
        count[1] = freq['o' - 'a'] - count[0] - count[2] - count[4];
        count[3] = freq['h' - 'a'] - count[8];
        count[5] = freq['f' - 'a'] - count[4];
        count[7] = freq['s' - 'a'] - count[6];

        // 第3层：最后一个
        count[9] = freq['i' - 'a'] - count[5] - count[6] - count[8];

        // 按升序构建结果
        string result;
        for (int i = 0; i <= 9; i++) {
            result += string(count[i], '0' + i);
        }
        return result;
    }
};

// ===================== Solution2: 显式扣除法（更清晰但等价） =====================
// 思路：每确定一个数字，就从 freq 中扣除该数字单词的所有字母
class Solution2 {
public:
    string originalDigits(string s) {
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;

        // 数字单词
        string words[] = {"zero","two","four","six","eight",
                          "one","three","five","seven","nine"};
        int digits[] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};
        char unique[] = {'z','w','u','x','g','o','h','f','s','i'};

        vector<int> count(10, 0);
        for (int k = 0; k < 10; k++) {
            int d = digits[k];
            int times = freq[unique[k] - 'a'];
            count[d] = times;
            // 从 freq 中扣除
            for (char c : words[k]) {
                freq[c - 'a'] -= times;
            }
        }

        string result;
        for (int i = 0; i <= 9; i++)
            result += string(count[i], '0' + i);
        return result;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | Solution1 | O(n) | O(1) | 数学公式直接推，最简洁 |
// | Solution2 | O(n) | O(1) | 显式扣除，逻辑更清晰易理解 |

// 【易错点】
// 1. 扣除顺序错误：必须先处理有独特字母的偶数(0,2,4,6,8)，再处理奇数(1,3,5,7)，最后9
// 2. 忘记 o 同时出现在 zero/one/two/four 中 → 必须扣除 count[0]+count[2]+count[4]
// 3. 忘记 i 同时出现在 five/six/eight/nine 中 → 必须扣除 count[5]+count[6]+count[8]
// 4. 结果需要按数字升序排列，不是按发现顺序

// 【面试追问】
// Q1: 如何确定哪些字母是"独特"的？
// A1: 列出 0-9 英文单词，看哪些字母只出现在一个单词中。
//     z→zero, w→two, u→four, x→six, g→eight 是第一层独特字母。
//     扣除这5个数字后，o/h/f/s 变成新的独特字母。最后 i 对应 nine。
//
// Q2: 这个方法的正确性如何证明？
// A2: 每一层的独特字母只可能来自对应的数字单词，所以 freq[unique] 就是该数字的次数。
//     扣除后，下一层的字母也变成独特的，依此类推直到所有数字确定。
//
// Q3: 如果输入不保证合法，如何检测？
// A3: 最终检查 freq 数组是否全为 0，以及 count 数组是否全非负。
