# 824. Goat Latin

## 核心思路

按规则逐词转换：元音开头加 "ma"，辅音开头将首字母移至末尾再加 "ma"，最后按单词索引追加对应数量的 'a'。本质是**字符串模拟 + 分词处理**。

## 思维链

1. **分词**：用 `istringstream` 或手动按空格分割 sentence 为单词列表
2. **判断首字母**：检查每个单词的第一个字符是否为元音（注意大小写）
3. **元音处理**：直接在单词末尾追加 "ma"
4. **辅音处理**：将首字母移到末尾，再追加 "ma"
5. **追加 'a'**：第 k 个单词（1-indexed）末尾追加 k 个 'a'
6. **拼接结果**：用空格连接所有转换后的单词

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| istringstream 分词 ⭐ | O(N + K^2) | O(N) | 简洁优雅，N 为总长度，K 为单词数 |
| 手动遍历分词 | O(N + K^2) | O(N) | 不依赖流，适合 C 风格 |

## 关键提示

- **元音集合**：`aeiouAEIOU`，必须同时处理大小写
- 辅音处理是 `word.substr(1) + word[0]`，不是删除首字母
- 'a' 的追加数量从 1 开始递增，不是从 0 开始
- `istringstream` 是 C++ 中按空格分词的惯用方式
- 结果字符串最后不应有多余空格

## 解法详解

### 解法一：istringstream 分词（推荐）

```cpp
class Solution {
public:
    string toGoatLatin(string sentence) {
        unordered_set<char> vowels = {'a','e','i','o','u','A','E','I','O','U'};
        istringstream iss(sentence);
        string word, result;
        int index = 1;
        
        while (iss >> word) {
            if (index > 1) result += " "; // 单词间加空格
            
            if (vowels.count(word[0])) {
                // 元音开头：直接追加 "ma"
                result += word + "ma";
            } else {
                // 辅音开头：首字母移到末尾再追加 "ma"
                result += word.substr(1) + word[0] + "ma";
            }
            
            // 追加 index 个 'a'
            result += string(index, 'a');
            index++;
        }
        
        return result;
    }
};
```

### 解法二：手动遍历分词

```cpp
class Solution {
public:
    string toGoatLatin(string sentence) {
        string vowels = "aeiouAEIOU";
        string result;
        int index = 1;
        int i = 0, n = sentence.size();
        
        while (i < n) {
            // 提取一个单词
            int j = i;
            while (j < n && sentence[j] != ' ') j++;
            string word = sentence.substr(i, j - i);
            
            if (index > 1) result += " ";
            
            if (vowels.find(word[0]) != string::npos) {
                result += word + "ma";
            } else {
                result += word.substr(1) + word[0] + "ma";
            }
            
            result += string(index, 'a');
            index++;
            i = j + 1; // 跳过空格
        }
        
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只检查小写元音 `aeiou` | ✓ 同时检查 `aeiouAEIOU` | 题目说包含大小写字母 |
| ✗ 追加 'a' 从 index=0 开始 | ✓ 从 index=1 开始 | 第一个单词追加 1 个 'a' |
| ✗ 结果首位多加空格 | ✓ 第一个单词前不加空格 | 用 `if (index > 1)` 控制 |
| ✗ 辅音处理为 `word[0] + word.substr(1)` | ✓ `word.substr(1) + word[0]` | 首字母移到末尾，不是保持原位 |
| ✗ `word.substr(1) + word[0] + "ma"` 类型问题 | ✓ 注意 char + string 拼接顺序 | `word[0]` 是 char，与 string 拼接需注意 |

## 面试追问

**Q1: 如果句子非常长（百万级字符），如何优化？**
> 预计算所有 'a' 后缀的最大长度，避免每次创建新 string。用 `reserve` 预分配结果字符串空间，减少动态扩容。也可以用 `string_view` 避免 `substr` 的拷贝。

**Q2: 如果规则变为"辅音开头需要移动所有连续辅音到末尾"（类似 Pig Latin 的真实规则），如何修改？**
> 找到第一个元音的位置 `pos`，将 `word[0..pos-1]` 移到末尾。即 `word.substr(pos) + word.substr(0, pos) + "ma"`。

**Q3: 如何处理 Unicode 字符或多字节编码？**
> 需要使用宽字符类型 `wstring` 或 UTF-8 解码库。简单的 `char` 操作不适用于多字节字符。

## 相关题型

- [LC 917] Reverse Only Letters（字符串条件处理）
- [LC 709] To Lower Case（字符转换）
- [LC 557] Reverse Words in a String III（分词 + 字符串操作）
- [LC 151] Reverse Words in a String（字符串分词）
