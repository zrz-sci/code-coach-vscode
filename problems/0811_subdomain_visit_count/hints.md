# 811. 子域名访问计数

## 核心思路

本质是一个**字符串解析 + 哈希表计数**题。对每条 `"count domain"` 记录，拆出访问次数和完整域名，然后从完整域名出发逐级提取父域名（去掉最左边的部分），将每一级域名的访问次数累加到哈希表中。

## 思维链

1. **读完题第一反应**：每次访问 `"a.b.c"` 就同时访问了 `"a.b.c"`、`"b.c"`、`"c"` 三个域名。需要统计所有域名（包括父域名）的总访问次数。

2. **怎么提取父域名？** 对于 `"discuss.leetcode.com"`，找到每个 `.` 的位置，从 `.` 后面截取就是一个父域名：
   - 找到第一个 `.` 位置 7 → `"leetcode.com"` 
   - 找到第二个 `.` 位置 16 → `"com"`

3. **字符串拆分**：每条记录格式为 `"count domain"`，先按空格分成次数和域名。次数用 `stoi` 转换。

4. **哈希表累加**：用 `unordered_map<string, int>` 记录每个域名的总访问次数。

5. **输出格式化**：遍历哈希表，将每条 `(domain, count)` 格式化为 `"count domain"` 字符串。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 字符串解析 + 哈希表 | 解析每条记录，逐级提取域名累加计数 | O(n * L) | O(n * L) | ⭐ 必须写出 |

> n 是记录数，L 是域名平均长度。本题是标准的字符串处理题，重点在于代码的清晰和正确性。

## 关键提示

1. **域名层级提取技巧**：对于域名 `"a.b.c"`，可以通过不断寻找 `.` 的位置并用 `substr` 提取子串来获得所有层级的域名。也可以直接从左到右扫描，每遇到 `.` 就把从该 `.` 后面开始的子串作为一个父域名。

2. **别忘了完整域名自身**：除了父域名外，完整域名本身也要计入哈希表。

3. **字符串转整数**：可以用 `stoi(s.substr(0, space_pos))` 或手动解析数字。

4. 处理过程示意：
```
"900 google.mail.com"
→ count = 900
→ 域名: "google.mail.com", "mail.com", "com"
→ 每个 +900

"50 yahoo.com"
→ count = 50
→ 域名: "yahoo.com", "com"
→ 每个 +50

结果: {"google.mail.com": 900, "mail.com": 900, "yahoo.com": 50, "com": 950, ...}
```

## 解法详解

### 解法1: 字符串解析 + 哈希表 -- O(n * L) / O(n * L) ⭐ 面试首选

**思考过程**：

对每条记录：(1) 分离次数和域名；(2) 枚举域名的所有后缀（即各级域名）；(3) 在哈希表中累加。

```cpp
class Solution {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        unordered_map<string, int> counter;
        
        for (const string& cp : cpdomains) {
            // 找空格分隔 count 和 domain
            int spacePos = cp.find(' ');
            int count = stoi(cp.substr(0, spacePos));
            string domain = cp.substr(spacePos + 1);
            
            // 完整域名本身
            counter[domain] += count;
            
            // 逐级提取父域名
            for (int i = 0; i < (int)domain.size(); i++) {
                if (domain[i] == '.') {
                    // 从 . 后面开始的子串就是一个父域名
                    counter[domain.substr(i + 1)] += count;
                }
            }
        }
        
        // 格式化输出
        vector<string> result;
        for (auto& [domain, count] : counter) {
            result.push_back(to_string(count) + " " + domain);
        }
        return result;
    }
};
```

**关键点**：
- `find(' ')` 找到空格位置，分离次数和域名。
- 遍历域名中的每个 `.`，取 `.` 之后的子串作为父域名。
- 完整域名也要累加到哈希表中（不只是父域名）。
- 输出格式为 `"count domain"`，顺序任意。

## 易错点

1. **忘记累加完整域名本身**：
   - ✗ 只累加了父域名（通过 `.` 截取的部分）
   - ✓ 在遍历 `.` 之前先 `counter[domain] += count`

2. **空格和点的位置混淆**：
   - ✗ 用 `.` 来分隔 count 和 domain
   - ✓ 用空格 `' '` 分隔 count 和 domain，域名内部用 `.` 分隔层级

3. **字符串拼接顺序错误**：
   - ✗ 输出 `domain + " " + to_string(count)`（顺序反了）
   - ✓ 输出 `to_string(count) + " " + domain`

4. **stoi 的潜在问题**：
   - ✗ 对非数字字符串调用 stoi 会抛异常
   - ✓ 题目保证输入格式正确，stoi 是安全的

## 面试追问

**Q1: 如果域名层级非常深（比如 10 层），会影响性能吗？**

> 每条记录的处理时间与域名长度成正比（需要扫描所有 `.` 并做 `substr`）。`substr` 操作是 O(L)，总共做 O(层级数) 次。对于 10 层的域名，每条记录最多产生 10 个哈希表操作，仍然是线性的。不过可以优化 -- 使用 `string_view` 避免 `substr` 的拷贝。

**Q2: 如果需要按域名层级排序输出（顶级域名在前），怎么改？**

> 可以在输出时按域名中 `.` 的数量排序（0 个点 = 顶级域名，1 个点 = 二级域名...）。或者用 `map` 替代 `unordered_map` 并自定义比较函数。

**Q3: 如何用 string_view 优化避免 substr 的拷贝开销？**

> 将 `domain.substr(i+1)` 替换为 `string_view(domain).substr(i+1)`，但哈希表的 key 仍然需要 `string`。可以用 `unordered_map<string_view, int>` 配合原字符串的生命周期管理来实现零拷贝，但代码复杂度增加。面试中提到这个优化点即可。

## 相关题型

- **LeetCode 1166. 设计文件系统** — 同样涉及路径/域名的层级解析和哈希表存储。
- **LeetCode 1178. 猜字谜** — 哈希表计数 + 字符串处理的组合题。
- **LeetCode 609. 在系统中查找重复文件** — 需要解析特定格式的字符串并用哈希表聚合。
