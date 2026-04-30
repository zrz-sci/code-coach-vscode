# 535. TinyURL 的加密与解密

## 核心思路

设计一个 URL 短链系统，核心是建立长URL与短URL之间的**双向映射**。可以用自增ID、随机字符串或哈希函数生成短码，关键是保证 encode/decode 的一致性。

## 思维链

1. 需要将长URL映射到短URL，并能反向解码
2. 最简单方案：用自增ID作为短码，存入 map
3. 进阶方案：随机生成6位字符串作为短码，避免被猜测
4. 哈希方案：对URL做哈希，但需处理冲突
5. 所有方案本质都是维护两个哈希表做双向查找

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 自增ID映射 | O(1) | O(n) | ⭐⭐ |
| 随机短码映射 | O(1) 均摊 | O(n) | ⭐⭐⭐ |

## 关键提示

1. encode 和 decode 必须使用**同一个对象**的状态
2. 自增ID方案简单但可被猜测/遍历
3. 随机短码需要检查冲突，但安全性更好
4. 面试重点在于讨论各方案的**trade-off**，而非代码复杂度

## 解法详解

### 解法一：自增ID映射

```cpp
class Solution {
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
```

### 解法二：随机短码映射（推荐）

```cpp
class Solution {
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
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 每次 encode 同一 URL 生成不同短码 | 检查是否已编码过，返回相同短码 | 幂等性要求 |
| 短码长度过短（如1-2位） | 至少6位，62^6 ≈ 568亿种组合 | 避免冲突 |
| 不处理随机码冲突 | while 循环检测冲突并重新生成 | 概率虽小但必须处理 |

## 面试追问

**Q1: 自增ID方案有什么安全隐患？**
→ 可被枚举和猜测，泄露总URL数量和注册顺序。

**Q2: 如何保证分布式环境下短码不冲突？**
→ 可用分布式ID生成器（如Snowflake）、数据库自增、或预分配ID段。

**Q3: 如何处理过期URL的清理？**
→ 存储时附带时间戳，定期扫描清理；或用LRU/TTL缓存策略。

## 相关题型

- [LeetCode 706. Design HashMap](https://leetcode.com/problems/design-hashmap/) - 哈希表设计
- [LeetCode 146. LRU Cache](https://leetcode.com/problems/lru-cache/) - 缓存设计
- System Design: Design URL Shortener - 系统设计经典题
