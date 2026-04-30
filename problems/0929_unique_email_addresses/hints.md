# 929. 独特的电子邮件地址

[file://./solution.cpp](file://./solution.cpp)

## 核心思路

根据电子邮件的规则（本地名中的点被忽略，加号后的内容被忽略，域名保持不变），对每个邮件地址做**规范化处理**，然后用哈希集合统计不同地址的数量。

## 思维链

1. **读完题第一反应** → 需要理解两条规则：(a) 本地名中的 `.` 被忽略；(b) 本地名中第一个 `+` 之后的全部忽略。域名不受这两条规则影响。
2. **怎么处理每个邮件？** → 先用 `@` 分割成本地名和域名。对本地名依次应用规则：去掉所有 `.`，截断 `+` 后的部分。然后拼接回 `规范化本地名@域名`。
3. **怎么去重？** → 将规范化后的邮件地址放入 `set` 或 `unordered_set`，最后返回集合大小。
4. **边界情况？** → 本地名没有 `.` 也没有 `+`；域名中有 `.`（这个不影响，域名原样保留）；本地名以 `+` 开头（题目约束说不会）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 字符串处理 + 哈希集合 | 规范化每个邮件，set 去重 | O(n * L) | O(n * L) | 必须写出 |

## 关键提示

1. **先分离本地名和域名**：找到 `@` 的位置，左边是本地名，右边是域名。域名完全不做处理。
2. **处理顺序**：先处理 `+`（截断），再处理 `.`（删除），或者反过来都可以。因为两个操作互不影响。
3. **用 `find` 和 `substr`**：C++ 字符串操作中，`find('@')` 找分隔符，`find('+')` 找截断位置，`substr` 提取子串。
4. **直接遍历构建**：也可以逐字符遍历本地名部分，跳过 `.`，遇到 `+` 时停止。这种方式更高效，避免了多次字符串操作。
5. **域名中的 `.` 不受影响**：比如 `lee.tcode.com` 是完整的域名，不要误删域名中的点。

## 解法详解

### 解法1: 字符串规范化 + 哈希集合 — O(n * L) / O(n * L)

**思考过程**: 每个邮件地址需要经过两步规范化，然后用集合去重。

```
示例: "test.e.mail+bob.cathy@leetcode.com"

第一步：分离
  本地名 = "test.e.mail+bob.cathy"
  域名   = "leetcode.com"

第二步：处理本地名
  (a) 截断'+'后: "test.e.mail"
  (b) 去掉'.': "testemail"

第三步：拼接
  规范化地址 = "testemail@leetcode.com"
```

```cpp
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_set<string> uniqueEmails;

        for (const string& email : emails) {
            // 分离本地名和域名
            int atPos = email.find('@');
            string local = email.substr(0, atPos);
            string domain = email.substr(atPos); // 包含@

            // 处理'+': 截断'+'及之后的部分
            int plusPos = local.find('+');
            if (plusPos != string::npos) {
                local = local.substr(0, plusPos);
            }

            // 处理'.': 删除所有'.'
            string cleaned;
            for (char c : local) {
                if (c != '.') cleaned += c;
            }

            // 拼接并加入集合
            uniqueEmails.insert(cleaned + domain);
        }

        return uniqueEmails.size();
    }
};
```

**关键点**:
- `domain` 包含 `@` 符号，这样拼接时自然正确。也可以不包含 `@`，拼接时手动加。
- `string::npos` 表示 `find` 未找到。
- 逐字符构建 `cleaned` 比用 `erase` 反复删除字符更高效。

---

### 解法2: 逐字符遍历法 — O(n * L) / O(n * L)

**不同角度**: 不用 `find/substr`，一次遍历邮件字符串完成规范化。

```cpp
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_set<string> uniqueEmails;

        for (const string& email : emails) {
            string normalized;
            bool ignoreLocal = false;  // 遇到'+'后设为true
            bool inDomain = false;     // 遇到'@'后设为true

            for (char c : email) {
                if (c == '@') {
                    inDomain = true;
                    normalized += c;
                } else if (inDomain) {
                    // 域名部分：原样保留
                    normalized += c;
                } else if (c == '+') {
                    // 本地名遇到'+': 后续忽略（直到'@'）
                    ignoreLocal = true;
                } else if (!ignoreLocal && c != '.') {
                    // 本地名普通字符（跳过'.'）
                    normalized += c;
                }
                // 其他情况（ignoreLocal=true 且不是@）: 跳过
            }

            uniqueEmails.insert(normalized);
        }

        return uniqueEmails.size();
    }
};
```

**关键点**:
- 只遍历一次字符串，用两个布尔标志控制状态。
- 代码略长但逻辑非常清晰，面试时容易解释状态转换。

## 解法对比

|  | 字符串分割法 | 逐字符遍历法 |
|--|-------------|-------------|
| 时间 | O(n * L) | O(n * L) |
| 空间 | O(n * L) | O(n * L) |
| 代码量 | 较短 | 略长 |
| 字符串操作次数 | 多次 substr/find | 一次遍历 |
| 推荐 | 面试写法更快 | 性能微优，逻辑清晰 |

## 易错点

1. **域名中的点也被删除了**：
   - `"a@lee.tcode.com"` → 错误结果 `"a@leetcodecom"` 
   - 必须先分离出域名，只对本地名做处理。

2. **`+` 在域名中出现**：
   - 虽然题目约束保证了格式，但如果不先分离，可能会把域名中的某些字符也截断。

3. **`find` 返回 `string::npos` 未判断**：
   - 如果本地名没有 `+`，`find('+')` 返回 `string::npos`，直接用它做 `substr` 参数实际上等于取整个字符串（`substr(0, npos)` = 整个字符串），所以恰好正确。但依赖这个行为不够清晰，建议显式判断。

4. **重复邮件的处理**：
   - 如果输入中有完全相同的邮件，`set` 自动去重，无需额外处理。

## 面试追问

**Q1: 时间复杂度能否优化到更低？**
> 不能。每个邮件至少要读一遍（O(L)），n 封邮件总共 O(n * L)。这已经是理论下界。

**Q2: 如果邮件量极大（10^6+），有什么工程上的优化？**
> (1) 用 `string_view` 减少拷贝；(2) 预分配 `unordered_set` 容量减少 rehash；(3) 规范化过程中直接计算哈希值，避免存完整字符串。

**Q3: 如果规则更复杂（比如域名也有特殊规则），怎么扩展？**
> 将规范化逻辑封装为函数 `normalize(local)` 和 `normalize(domain)`，分别处理。核心框架不变，只需修改规范化规则。

**Q4: 不用 set 能做吗？**
> 可以：先将所有规范化后的邮件放入数组，排序，然后数相邻不重复的个数。时间 O(n * L * log n)，空间 O(n * L)。但不如 set 解法优雅。

## 相关题型

- **771. 宝石与石头** — 同样是字符串 + 哈希集合的简单应用。先将宝石字符存入 set，再遍历石头计数。
- **387. 字符串中的第一个唯一字符** — 哈希表统计字符频率的基础应用。
- **49. 字母异位词分组** — 字符串规范化（排序后作为键）+ 哈希表分组，与本题"规范化后去重"的模式非常类似。
- **811. 子域名访问计数** — 字符串解析 + 哈希表统计，同样需要按 `@` 或 `.` 分割字符串。
