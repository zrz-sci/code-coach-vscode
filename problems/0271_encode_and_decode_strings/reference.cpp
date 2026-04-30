/*
 * 271. Encode and Decode Strings
 *
 * 【题目本质】
 * 设计无歧义的字符串列表序列化协议。
 * 核心挑战：字符串可包含任意 ASCII 字符，不能用简单分隔符。
 * 最佳方案：长度前缀编码 "len#string"，解码时按长度截取。
 *
 * 【解法总览】
 * Solution1: 长度前缀编码 — O(n) 时间, O(1) 额外空间 [推荐]
 * Solution2: 转义字符编码 — O(n) 时间, O(1) 额外空间
 */

// ===================== Solution 1: 长度前缀编码 =====================
// 协议: 每个字符串编码为 "length#content"
// 例如: ["Hello","World"] -> "5#Hello5#World"
// 时间 O(n), 空间 O(1) 额外
class Solution1 {
public:
    // Encodes a list of strings to a single string.
    string encode(vector<string>& strs) {
        string encoded;
        for (const string& s : strs) {
            encoded += to_string(s.size()) + "#" + s;
        }
        return encoded;
    }

    // Decodes a single string to a list of strings.
    vector<string> decode(string s) {
        vector<string> result;
        int i = 0;
        while (i < (int)s.size()) {
            // 找到 '#' 分隔符
            int j = s.find('#', i);
            // 解析长度
            int len = stoi(s.substr(i, j - i));
            // 提取字符串内容
            result.push_back(s.substr(j + 1, len));
            // 移动到下一个编码段
            i = j + 1 + len;
        }
        return result;
    }
};

// ===================== Solution 2: 转义字符编码 =====================
// 协议: 用 "/:" 做分隔符，"/" 做转义字符
// 内容中的 "/" 转义为 "//"
// 例如: ["a/b","cd"] -> "a//b/:cd/:"
// 时间 O(n), 空间 O(1) 额外
class Solution2 {
public:
    string encode(vector<string>& strs) {
        string encoded;
        for (const string& s : strs) {
            for (char c : s) {
                if (c == '/') {
                    encoded += "//";  // 转义
                } else {
                    encoded += c;
                }
            }
            encoded += "/:";  // 分隔符
        }
        return encoded;
    }

    vector<string> decode(string s) {
        vector<string> result;
        string cur;
        int i = 0;
        while (i < (int)s.size()) {
            if (i + 1 < (int)s.size() && s[i] == '/') {
                if (s[i + 1] == ':') {
                    // 遇到分隔符，结束当前字符串
                    result.push_back(cur);
                    cur.clear();
                    i += 2;
                } else if (s[i + 1] == '/') {
                    // 转义的 '/'
                    cur += '/';
                    i += 2;
                }
            } else {
                cur += s[i];
                i++;
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 长度前缀 | O(n) | O(1) | 最简洁，无歧义，工业标准 |
 * | Solution2 转义字符 | O(n) | O(1) | 更通用，但实现复杂 |
 *
 * 两种方案都是 O(n)，但长度前缀方案更简洁且不需要扫描内容。
 *
 * 【易错点】
 * 1. 用简单分隔符（逗号、换行等）
 *    ✗ join(strs, ",") 然后 split(",")
 *    ✓ 长度前缀 "len#str"，因为字符串可含任意字符
 *
 * 2. decode 时指针移动错误
 *    ✗ i = j + len (忘记跳过 '#')
 *    ✓ i = j + 1 + len
 *
 * 3. 空字符串处理
 *    ✗ 长度为 0 时 substr 返回空 → 漏掉空串
 *    ✓ "0#" 自然表示空串，substr(j+1, 0) 返回 ""
 *
 * 4. 长度数字可能多位
 *    ✗ 假设长度只有一位，直接 s[i]-'0'
 *    ✓ 用 find('#') + stoi 解析多位数字
 *
 * 【面试追问】
 * Q1: 为什么长度前缀比转义更好?
 *     --> 长度前缀 O(1) 定位内容，转义需要逐字符扫描
 *
 * Q2: 网络传输中如何处理?
 *     --> 实际协议(HTTP, Protocol Buffers)都用长度前缀
 *     --> HTTP Content-Length 就是这个思路
 *
 * Q3: 如何支持二进制数据?
 *     --> 用固定4字节表示长度(网络字节序)，而非字符串
 */
