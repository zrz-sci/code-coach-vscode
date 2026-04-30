/*
 * LeetCode 811: 子域名访问计数 (Subdomain Visit Count)
 *
 * 【题目本质】
 * 字符串解析 + 哈希表计数。对每条 "count domain" 记录，
 * 拆出所有层级的域名并在哈希表中累加访问次数。
 *
 * 【解法总览】
 * 解法1: find('.') + substr 提取 — O(n*L) / O(n*L) — 面试首选
 * 解法2: 从右往左逐字符构建域名 — O(n*L) / O(n*L) — 变体写法
 *
 * 重点在于字符串解析的正确性和代码清晰度。
 */

// ============================================================
// 解法1: find('.') + substr 提取各级域名
// 时间: O(n * L)  空间: O(n * L)  n=记录数, L=平均域名长度
//
// 【思路】
// 每条记录 "count domain":
//   1. 按空格分离 count 和 domain
//   2. 完整域名 domain 本身计入哈希表
//   3. 扫描域名中的每个 '.'，取 '.' 之后的子串作为父域名
//
// 处理过程:
// "900 google.mail.com"
//   count = 900, domain = "google.mail.com"
//   → counter["google.mail.com"] += 900
//   → 找到 '.' at 6:  counter["mail.com"] += 900
//   → 找到 '.' at 11: counter["com"] += 900
//
// "50 yahoo.com"
//   count = 50, domain = "yahoo.com"
//   → counter["yahoo.com"] += 50
//   → 找到 '.' at 5: counter["com"] += 50
//
// 最终: {"google.mail.com":900, "mail.com":900, "yahoo.com":50, "com":950}
// ============================================================
class Solution1 {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        unordered_map<string, int> counter;

        for (const string& cp : cpdomains) {
            // 1. 分离 count 和 domain
            int spacePos = cp.find(' ');
            int count = stoi(cp.substr(0, spacePos));
            string domain = cp.substr(spacePos + 1);

            // 2. 完整域名自身
            counter[domain] += count;

            // 3. 逐级提取父域名: 找每个 '.'，取后面的子串
            for (int i = 0; i < (int)domain.size(); i++) {
                if (domain[i] == '.') {
                    // "google.mail.com" 中 i=6 时提取 "mail.com"
                    counter[domain.substr(i + 1)] += count;
                }
            }
        }

        // 4. 格式化输出
        vector<string> result;
        result.reserve(counter.size());
        for (auto& [domain, cnt] : counter) {
            result.push_back(to_string(cnt) + " " + domain);
        }
        return result;
    }
};

// ============================================================
// 解法2: 使用 string::find 逐次定位 '.'
// 时间: O(n * L)  空间: O(n * L)
//
// 【思路】
// 和解法1 逻辑相同，但用 string::find('.', pos) 逐次搜索 '.'，
// 代码风格不同，某些人觉得更清晰。
// ============================================================
class Solution2 {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        unordered_map<string, int> counter;

        for (const string& cp : cpdomains) {
            // 分离 count 和 domain
            size_t space = cp.find(' ');
            int count = stoi(cp.substr(0, space));
            string domain = cp.substr(space + 1);

            // 从完整域名开始，逐级去掉最左前缀
            // "google.mail.com" → "mail.com" → "com"
            string sub = domain;
            counter[sub] += count;

            size_t dot = sub.find('.');
            while (dot != string::npos) {
                sub = sub.substr(dot + 1);
                counter[sub] += count;
                dot = sub.find('.');
            }
        }

        // 格式化输出
        vector<string> result;
        for (auto& [domain, cnt] : counter) {
            result.push_back(to_string(cnt) + " " + domain);
        }
        return result;
    }
};

// ============================================================
// 解法3: 手动解析数字 (避免 stoi + substr 的额外拷贝)
// 时间: O(n * L)  空间: O(n * L)
//
// 【思路】
// 不用 stoi/substr，直接手动解析数字并用下标定位域名。
// 减少了临时字符串的分配，面试中加分写法。
// ============================================================
class Solution3 {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        unordered_map<string, int> counter;

        for (const string& cp : cpdomains) {
            // 手动解析数字
            int count = 0;
            int i = 0;
            while (cp[i] != ' ') {
                count = count * 10 + (cp[i] - '0');
                i++;
            }
            i++; // 跳过空格

            // 从位置 i 开始的整个子串是完整域名
            string domain = cp.substr(i);
            counter[domain] += count;

            // 提取父域名
            for (int j = i; j < (int)cp.size(); j++) {
                if (cp[j] == '.') {
                    counter[cp.substr(j + 1)] += count;
                }
            }
        }

        vector<string> result;
        for (auto& [domain, cnt] : counter) {
            result.push_back(to_string(cnt) + " " + domain);
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
// | 解法 | 优点 | 缺点 |
// |------|------|------|
// | Solution1 (遍历找'.') | 简洁直观 | substr 有拷贝开销 |
// | Solution2 (find循环) | 逻辑清晰 | 每次 sub.find 多一层循环 |
// | Solution3 (手动解析) | 减少临时对象 | 代码稍长 |
//
// 面试推荐 Solution1，最简洁。
//
// 【易错点】
// 1. 忘记累加完整域名:
//    ✗ 只累加了 '.' 后面的父域名
//    ✓ counter[domain] += count 先累加完整域名
//
// 2. 输出格式反了:
//    ✗ domain + " " + to_string(count)
//    ✓ to_string(count) + " " + domain
//
// 3. 空格和点混淆:
//    ✗ 用 '.' 分离 count 和 domain
//    ✓ 用 ' ' 分离 count 和 domain
//
// 4. 忘记跳过空格:
//    ✗ domain = cp.substr(spacePos) → 域名开头多了个空格
//    ✓ domain = cp.substr(spacePos + 1)
//
// 【面试追问】
// Q1: 域名很深(10层)影响性能吗？
//     (每条记录 O(L)，L 是域名长度，总计 O(n*L)，可控)
// Q2: 如何用 string_view 优化？
//     (避免 substr 拷贝，但哈希表 key 仍需 string)
// Q3: 如果要按层级排序输出？
//     (按 '.' 数量排序，或用 map + 自定义比较)
// ============================================================
