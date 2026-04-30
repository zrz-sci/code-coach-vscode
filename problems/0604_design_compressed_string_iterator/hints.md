# 604. 迭代压缩字符串

## 核心思路

设计一个迭代器，逐字符输出压缩字符串的内容。压缩格式为"字母+数字"交替出现（如 `L1e2t1`）。关键是**解析字符和对应的重复次数**，维护当前字符和剩余次数的状态。每次 `next()` 递减计数，计数归零时移到下一组。

## 思维链

1. **解析压缩字符串** — 识别每个"字母+数字"对，数字可能是多位数（如 `a12` 表示 'a' 重复 12 次）
2. **选择解析策略** — 可以预处理全部解析到 queue/vector，也可以惰性解析（用指针按需读取）
3. **维护状态** — 当前字符 `ch` + 剩余次数 `count`
4. **next() 操作** — 返回当前字符并递减 count；count 为 0 时读取下一组
5. **hasNext() 操作** — 检查 count > 0 或后续还有未解析的字符组
6. **边界处理** — 所有字符耗尽时 next() 返回空格 `' '`

## 解法概览

| 解法 | 思路 | 空间复杂度 | 推荐度 |
|------|------|-----------|--------|
| 预处理 + queue | 构造时解析所有字符对到队列 | O(n) | ⭐⭐⭐⭐⭐ |
| 惰性解析（指针） | 用指针按需解析，不预处理 | O(1) 额外空间 | ⭐⭐⭐⭐ |
| 完全展开 | 展开为完整字符串后逐个输出 | O(展开长度) | ⭐⭐ |

## 关键提示

- **数字可以是多位** — 重复次数范围 [1, 10^9]，必须解析完整的多位数字
- **不能完全展开** — 10^9 次重复不可能展开为实际字符串，必须用计数方式
- **next() 返回 char 而非 string** — 注意返回类型是 `char`
- **耗尽后返回空格** — `next()` 在无字符可返回时返回 `' '`（空格字符）
- **long long 计数** — 重复次数最大 10^9，用 int 即可，但多组累计可能需要 long

## 解法详解

### 解法一：预处理到队列（推荐）

```cpp
class StringIterator {
    queue<pair<char, int>> q;  // (字符, 重复次数)
public:
    StringIterator(string compressedString) {
        int i = 0, n = compressedString.size();
        while (i < n) {
            char ch = compressedString[i++];
            int count = 0;
            while (i < n && isdigit(compressedString[i])) {
                count = count * 10 + (compressedString[i++] - '0');
            }
            q.push({ch, count});
        }
    }

    char next() {
        if (!hasNext()) return ' ';
        auto& front = q.front();
        char ch = front.first;
        if (--front.second == 0) q.pop();
        return ch;
    }

    bool hasNext() {
        return !q.empty();
    }
};
```

**关键细节：**
- 构造时一次性解析所有 `(char, count)` 对，压入队列
- `next()` 取队首的字符，递减计数，计数归零时弹出
- `hasNext()` 检查队列是否为空

### 解法二：惰性解析（指针法）

```cpp
class StringIterator {
    string s;
    int i;       // 当前解析位置
    char ch;     // 当前字符
    int count;   // 当前字符剩余次数
public:
    StringIterator(string compressedString)
        : s(compressedString), i(0), ch(' '), count(0) {
        advance();  // 预读第一组
    }

    char next() {
        if (!hasNext()) return ' ';
        char result = ch;
        if (--count == 0) advance();
        return result;
    }

    bool hasNext() {
        return count > 0;
    }

private:
    void advance() {
        if (i >= s.size()) return;
        ch = s[i++];
        count = 0;
        while (i < s.size() && isdigit(s[i])) {
            count = count * 10 + (s[i++] - '0');
        }
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `count = s[i] - '0'` 只读一位 | ✓ while 循环解析多位数字 | 重复次数可能是多位数如 999999999 |
| ✗ 返回 `""` 空字符串 | ✓ 返回 `' '` 空格字符 | 题目要求耗尽时返回空格，类型是 char |
| ✗ 完全展开字符串 | ✓ 用计数方式存储 | 10^9 长度的字符串会内存溢出 |
| ✗ `count` 用 short/byte | ✓ 用 int（至少 32 位） | 重复次数最大 10^9，需要 int 范围 |

## 面试追问

**Q1: 如果压缩字符串非常长（百万级字符对），预处理和惰性解析哪个更好？**
> 惰性解析更优。预处理需要 O(n) 空间存储所有字符对，而惰性解析只需 O(1) 额外空间。如果只调用少量 next()，惰性解析避免了不必要的解析工作。但如果 next() 调用次数与总字符数相当，两者性能差异不大。

**Q2: 如何支持 peek() 操作（查看下一个字符但不消耗）？**
> 在惰性解析方案中，`ch` 已经是下一个要返回的字符，直接返回 `ch` 即可实现 peek()。在队列方案中，返回 `q.front().first`。两种方案都不需要额外状态。

**Q3: 如何支持 reset() 操作回到字符串开头？**
> 惰性解析方案中，重置 `i = 0, count = 0`，再调用 `advance()` 即可。队列方案需要重新解析原始字符串并重新填充队列，因此惰性方案在支持 reset 时更优。

## 相关题型

- [271. 字符串编码与解码](../0271_encode_and_decode_strings/) — 字符串编解码设计
- [341. 扁平化嵌套列表迭代器](../0341_flatten_nested_list_iterator/) — 迭代器设计模式
- [284. 窥探迭代器](../0284_peeking_iterator/) — 迭代器 + peek 功能
- [443. 压缩字符串](../0443_string_compression/) — 字符串压缩的编码端
