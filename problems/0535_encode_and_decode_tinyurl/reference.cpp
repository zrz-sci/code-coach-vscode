/*
 * 【题目本质】
 * 设计 URL 短链系统：建立长URL与短URL之间的双向映射。
 * 核心是选择合适的编码策略，保证 encode/decode 一致性。
 *
 * 【解法总览】
 * Solution1: 自增ID映射 — 简单直接，用递增整数做短码
 * Solution2: 随机短码映射 — 62进制随机码，更安全更实用（推荐）
 */

// 解法一：自增ID映射
// 思路：每次 encode 分配一个递增ID，存入 map
// 时间 O(1)，空间 O(n)
class Solution1 {
    unordered_map<int, string> idToUrl;
    int id = 0;
public:
    string encode(string longUrl) {
        id++;
        idToUrl[id] = longUrl;
        return "http://tinyurl.com/" + to_string(id);
    }

    string decode(string shortUrl) {
        int pos = shortUrl.rfind('/');
        int id = stoi(shortUrl.substr(pos + 1));
        return idToUrl[id];
    }
};

// 解法二：随机短码映射（推荐）
// 思路：随机生成6位 Base62 编码，双向 map 保证幂等性
// 时间 O(1) 均摊，空间 O(n)
class Solution2 {
    unordered_map<string, string> codeToUrl;
    unordered_map<string, string> urlToCode;
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    string generateCode() {
        string code;
        for (int i = 0; i < 6; i++) {
            code += chars[rand() % chars.size()];
        }
        return code;
    }
public:
    string encode(string longUrl) {
        if (urlToCode.count(longUrl)) {
            return "http://tinyurl.com/" + urlToCode[longUrl];
        }
        string code = generateCode();
        while (codeToUrl.count(code)) {
            code = generateCode();
        }
        codeToUrl[code] = longUrl;
        urlToCode[longUrl] = code;
        return "http://tinyurl.com/" + code;
    }

    string decode(string shortUrl) {
        string code = shortUrl.substr(shortUrl.rfind('/') + 1);
        return codeToUrl[code];
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 自增ID | O(1) | O(n) | 简单但可被枚举 |
 * | 随机短码 | O(1)均摊 | O(n) | 安全，更贴近真实系统 |
 *
 * 【易错点】
 * 1. 同一URL多次encode应返回相同短码（幂等性）
 * 2. 随机码必须处理冲突（while循环重新生成）
 * 3. rfind('/') 提取短码时注意边界
 *
 * 【面试追问】
 * Q1: 自增ID安全隐患？→ 可枚举猜测，泄露数量和顺序
 * Q2: 分布式下如何保证不冲突？→ Snowflake/预分配ID段
 * Q3: 过期URL如何清理？→ TTL + 定期扫描清理
 */
