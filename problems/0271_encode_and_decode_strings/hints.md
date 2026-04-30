# 271. Encode and Decode Strings (字符串的编码与解码)

**难度**: Medium | **标签**: Design, Array, String

## 核心思路

将字符串列表编码为单个字符串，核心在于设计一种无歧义的分隔协议。关键挑战是字符串可包含任意 ASCII 字符（包括任何你选的分隔符），因此不能用简单分隔符。最佳方案：**长度前缀编码** -- 每个字符串前加上「长度 + 分隔符」。

## 思维链

1. **朴素想法**: 用特殊字符(如 `#`)分隔 --> 失败！字符串本身可能包含 `#`
2. **转义方案**: 对特殊字符做转义(如 `#` -> `##`) --> 可行但复杂
3. **长度前缀**: `len#string` 格式 --> 简洁且完全无歧义
4. **关键洞察**: 知道长度后直接截取，无需担心内容包含什么字符

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 长度前缀编码 | O(n) | O(1) | ⭐ |
| 转义字符编码 | O(n) | O(1) | |
| 分块编码(chunked) | O(n) | O(1) | |

## 关键提示

```
长度前缀编码协议:

encode(["Hello", "World", ""])
  --> "5#Hello5#World0#"

解码过程:
  "5#Hello5#World0#"
   ^
   读到 '5' 和 '#'，知道接下来5个字符是 "Hello"
              ^
              读到 '5' 和 '#'，知道接下来5个字符是 "World"
                           ^
                           读到 '0' 和 '#'，知道接下来0个字符是 ""

支持任意字符的证明:
  encode(["a#b", "3#x"])
  --> "3#a#b3#3#x"
       ^         
       读 "3" + "#" → 取3字符 "a#b" (内容中的 # 不影响!)
            ^
            读 "3" + "#" → 取3字符 "3#x" (内容中的 3# 不影响!)
```

## 解法详解

### 解法一: 长度前缀编码 (推荐)

```cpp
class Codec {
public:
    // Encodes: "len#str" for each string
    string encode(vector<string>& strs) {
        string encoded;
        for (const string& s : strs) {
            encoded += to_string(s.size()) + "#" + s;
        }
        return encoded;
    }

    // Decodes: parse length, skip '#', extract substring
    vector<string> decode(string s) {
        vector<string> result;
        int i = 0;
        while (i < s.size()) {
            int j = s.find('#', i);            // 找到 '#' 的位置
            int len = stoi(s.substr(i, j - i)); // 解析长度
            result.push_back(s.substr(j + 1, len)); // 提取字符串
            i = j + 1 + len;                   // 移到下一段
        }
        return result;
    }
};
```

### 解法二: 转义字符编码

```cpp
class Codec {
public:
    // 用 "/:" 做分隔，"/" 做转义符
    // '/' -> '//', ':' 不需转义，用 "/:" 分隔
    string encode(vector<string>& strs) {
        string encoded;
        for (const string& s : strs) {
            for (char c : s) {
                if (c == '/') encoded += "//";
                else encoded += c;
            }
            encoded += "/:";  // 字符串分隔符
        }
        return encoded;
    }

    vector<string> decode(string s) {
        vector<string> result;
        string cur;
        int i = 0;
        while (i < s.size()) {
            if (i + 1 < s.size() && s[i] == '/' && s[i + 1] == ':') {
                result.push_back(cur);
                cur.clear();
                i += 2;
            } else if (i + 1 < s.size() && s[i] == '/' && s[i + 1] == '/') {
                cur += '/';
                i += 2;
            } else {
                cur += s[i];
                i++;
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 `,` 或 `\n` 做分隔符 | ✓ 长度前缀 `len#str` | 字符串可含任意ASCII字符 |
| ✗ `stoi(s.substr(i))` 截取到末尾 | ✓ `stoi(s.substr(i, j-i))` 只截取数字部分 | 否则会把后面内容也塞进 stoi |
| ✗ decode 时 `i = j + len` | ✓ `i = j + 1 + len` | 别忘了跳过 `#` 本身 |
| ✗ 不处理空字符串 `""` | ✓ `0#` 自然表示空串 | 长度前缀方案天然支持空串 |

## 面试追问

**Q1: 为什么不能用固定分隔符?**
--> 字符串可包含任意256个ASCII字符，任何单字符分隔符都可能出现在内容中造成歧义。

**Q2: 长度前缀中，如果长度本身超过一位数字怎么办?**
--> `to_string()` 转成字符串，用 `#` 分隔长度和内容，`find('#')` 定位。例如 `200#...200个字符...`。

**Q3: 如果要支持 Unicode / 二进制数据呢?**
--> 长度前缀方案天然支持，因为我们按字节长度截取，不依赖内容解析。可以用 4 字节固定长度的二进制长度头更高效。

## 相关题型

- [297. Serialize and Deserialize Binary Tree](../0297_serialize_and_deserialize_binary_tree/) - 树的序列化
- [449. Serialize and Deserialize BST](../0449_serialize_and_deserialize_bst/) - BST 序列化
- [394. Decode String](../0394_decode_string/) - 字符串解码
- [443. String Compression](../0443_string_compression/) - 字符串压缩
