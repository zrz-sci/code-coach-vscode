/*
 * LeetCode 929: Unique Email Addresses (独特的电子邮件地址)
 *
 * 【题目本质】
 * 根据邮件规则规范化每个地址（本地名去点、截断+后内容，域名保留），
 * 统计规范化后不同地址的数量。
 *
 * 【解法总览】
 * 解法1: 字符串分割 + 规范化 + 哈希集合 — O(n*L) / O(n*L) — 面试首选
 * 解法2: 逐字符遍历状态机             — O(n*L) / O(n*L) — 一遍扫描
 */

// ============================================================
// 解法1: 字符串分割 + 规范化
// 时间: O(n * L)  空间: O(n * L)
//   n = 邮件数量, L = 平均邮件长度
//
// 【思路】
// 对每个邮件：
//   1. 用 '@' 分成本地名和域名
//   2. 本地名：截断 '+' 后的部分，然后删除所有 '.'
//   3. 域名：原样保留
//   4. 拼接后放入 set 去重
//
// 示例: emails = ["test.email+alex@leetcode.com",
//                 "test.e.mail+bob.cathy@leetcode.com",
//                 "testemail+david@lee.tcode.com"]
//
//   "test.email+alex@leetcode.com"
//     → 本地名 "test.email+alex" → 截断+ "test.email" → 去. "testemail"
//     → "testemail@leetcode.com"
//
//   "test.e.mail+bob.cathy@leetcode.com"
//     → 本地名 "test.e.mail+bob.cathy" → 截断+ "test.e.mail" → 去. "testemail"
//     → "testemail@leetcode.com"   (重复!)
//
//   "testemail+david@lee.tcode.com"
//     → 本地名 "testemail+david" → 截断+ "testemail" → 去. "testemail"
//     → "testemail@lee.tcode.com"  (域名不同，不重复)
//
//   set = {"testemail@leetcode.com", "testemail@lee.tcode.com"}
//   返回 2
// ============================================================
class Solution1 {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_set<string> uniqueEmails;

        for (const string& email : emails) {
            // 第一步：找到 '@' 分割本地名和域名
            int atPos = email.find('@');
            string local = email.substr(0, atPos);
            string domain = email.substr(atPos); // 包含 '@'

            // 第二步：处理本地名
            // (a) 截断 '+' 及之后的内容
            int plusPos = local.find('+');
            if (plusPos != string::npos) {
                local = local.substr(0, plusPos);
            }

            // (b) 删除所有 '.'
            string cleaned;
            for (char c : local) {
                if (c != '.') {
                    cleaned += c;
                }
            }

            // 第三步：拼接规范化地址
            uniqueEmails.insert(cleaned + domain);
        }

        return uniqueEmails.size();
    }
};

// ============================================================
// 解法2: 逐字符遍历状态机
// 时间: O(n * L)  空间: O(n * L)
//
// 【思路】
// 不用 find/substr，一次遍历邮件完成规范化。
// 用两个布尔状态标志：
//   - ignoreLocal: 遇到 '+' 后为 true，跳过后续本地名字符
//   - inDomain:    遇到 '@' 后为 true，进入域名原样复制模式
//
// 状态转换：
//   初始: ignoreLocal=false, inDomain=false
//   遇到 '+' 且不在域名中: ignoreLocal = true
//   遇到 '@': inDomain = true, 开始原样复制
//   在本地名中、未遇到+: 跳过'.'，其他字符追加
//   在域名中: 所有字符原样追加
//
// 优势：只遍历一次字符串，不创建中间子串。
// ============================================================
class Solution2 {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_set<string> uniqueEmails;

        for (const string& email : emails) {
            string normalized;
            bool ignoreLocal = false;
            bool inDomain = false;

            for (char c : email) {
                if (c == '@') {
                    // 进入域名模式
                    inDomain = true;
                    normalized += c;
                } else if (inDomain) {
                    // 域名部分：原样保留所有字符
                    normalized += c;
                } else if (c == '+') {
                    // 本地名遇到 '+': 开始忽略后续字符
                    ignoreLocal = true;
                } else if (!ignoreLocal && c != '.') {
                    // 本地名有效字符（非'.'且未被+截断）
                    normalized += c;
                }
                // else: ignoreLocal=true 时的本地名字符 → 跳过
            }

            uniqueEmails.insert(normalized);
        }

        return uniqueEmails.size();
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间    | 空间    | 字符串操作 | 代码清晰度 |
// |-------------|---------|---------|-----------|-----------|
// | 字符串分割   | O(n*L)  | O(n*L)  | 多次      | 高         |
// | 逐字符状态机 | O(n*L)  | O(n*L)  | 一次      | 高         |
//
// 面试推荐解法1（更快写完），性能差异微乎其微。
//
// ============================================================
// 【易错点】
//
// 1. 域名中的点也被删除:
//    ✗ 对整个邮件字符串删除 '.' → "a@lee.tcode.com" 变成 "a@leetcodecom"
//    ✓ 必须先分离出域名，只对本地名做 '.' 删除
//
// 2. 没有判断 find 返回 npos:
//    ✗ local.substr(0, local.find('+')) 当没有 '+' 时
//       实际上 substr(0, npos) 返回整个字符串，碰巧正确
//    ✓ 最好显式判断，代码意图更清晰
//
// 3. '@' 分割位置搞错:
//    ✗ domain = email.substr(atPos + 1) 但拼接时忘加 '@'
//    ✓ 要么 domain 包含 '@'，要么拼接时手动加
//
// 4. 误以为域名也要处理:
//    题目明确说规则"不适用于域名"。
//    域名中的 '.' 和 '+' (如果有) 都保持原样。
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 时间复杂度还能更低吗？
//     不能。每封邮件至少读一遍 O(L)，n 封 O(n*L) 是理论下界。
//
// Q2: 如果邮件数量极大，怎么工程优化？
//     (1) string_view 减少拷贝
//     (2) 预分配 unordered_set 的 bucket 数量
//     (3) 多线程分片处理后合并 set
//
// Q3: 如果域名也有规范化规则怎么办？
//     将规范化封装为两个函数 normalizeLocal() 和 normalizeDomain()
//     分别处理后拼接。核心框架不变。
//
// ============================================================
// 【相关题型】
//
// - 49. 字母异位词分组:
//   同样的"规范化 + 哈希表分组"模式。
//   规范化方法: 将字符串排序后作为键。
//
// - 771. 宝石与石头:
//   哈希集合的简单应用。
//
// - 811. 子域名访问计数:
//   字符串解析 + 哈希表统计。同样需要按分隔符拆分。
// ============================================================
