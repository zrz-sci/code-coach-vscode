# 804. Unique Morse Code Words

## 核心思路

将每个单词按字母映射为摩尔斯电码拼接成字符串，用 HashSet 去重后返回集合大小。本质是**字符串映射 + 去重计数**。

## 思维链

1. **建立映射表**：预定义 26 个字母对应的摩尔斯电码字符串数组
2. **遍历每个单词**：对 words 中的每个 word 进行处理
3. **逐字符转换**：将 word 的每个字符 `c` 映射为 `morse[c - 'a']`，拼接成完整电码字符串
4. **插入集合去重**：将拼接结果插入 `unordered_set<string>`
5. **返回集合大小**：集合的 size 即为不同变换的数量

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| HashSet 去重 ⭐ | O(S) | O(S) | 直观简洁，S为所有字符总数 |
| 排序去重 | O(S + NlogN) | O(S) | 先收集再排序后 unique |

## 关键提示

- 摩尔斯电码表是固定的，直接硬编码为字符串数组最高效
- 字符到索引的映射：`c - 'a'` 即可获得 0~25 的下标
- `unordered_set` 的插入和查找均为 O(1) 平均时间
- 注意：不同单词可能映射到相同的摩尔斯电码（如 "gin" 和 "zen"）

## 解法详解

### 解法一：HashSet 去重（推荐）

```cpp
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        // 26个字母的摩尔斯电码映射
        vector<string> morse = {
            ".-","-...","-.-.","-..",".","..-.","--.","....",
            "..",".---","-.-",".-..","--","-.","---",".--.",
            "--.-",".-.","...","-","..-","...-",".--","-..-",
            "-.--","--.."
        };
        
        unordered_set<string> seen;
        for (const string& word : words) {
            string code;
            for (char c : word) {
                code += morse[c - 'a'];
            }
            seen.insert(code);
        }
        return seen.size();
    }
};
```

### 解法二：排序去重

```cpp
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        vector<string> morse = {
            ".-","-...","-.-.","-..",".","..-.","--.","....",
            "..",".---","-.-",".-..","--","-.","---",".--.",
            "--.-",".-.","...","-","..-","...-",".--","-..-",
            "-.--","--.."
        };
        
        vector<string> codes;
        for (const string& word : words) {
            string code;
            for (char c : word) {
                code += morse[c - 'a'];
            }
            codes.push_back(code);
        }
        sort(codes.begin(), codes.end());
        return unique(codes.begin(), codes.end()) - codes.begin();
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `morse[c]` | ✓ `morse[c - 'a']` | 字符需要减去 'a' 转为索引 |
| ✗ `set<string>` | ✓ `unordered_set<string>` | 无需有序，unordered 更快 |
| ✗ 忘记 `const string&` | ✓ `const string& word` | 避免不必要的字符串拷贝 |
| ✗ 在循环外声明 code 不重置 | ✓ 循环内声明或每次清空 | 否则拼接了多个单词的电码 |

## 面试追问

**Q1: 如果输入规模非常大（百万级单词），如何优化？**
> 可以用哈希值代替完整字符串存储，减少内存开销。对摩尔斯电码字符串计算哈希后存入 `unordered_set<size_t>`，但需注意哈希冲突问题。

**Q2: 如果要求输出所有唯一变换对应的原始单词列表？**
> 改用 `unordered_map<string, vector<string>>`，key 为摩尔斯电码，value 为原始单词列表。最终返回 map 的 size 以及每个 key 对应的 value。

**Q3: 摩尔斯电码是否存在歧义？比如 ".-" + "-..." 和 ".--" + "..." 拼接后相同吗？**
> 是的，摩尔斯电码本身存在前缀歧义问题。本题中因为是按单词逐字符转换且字母边界明确，所以不会产生歧义。但在真实通信中需要分隔符。

## 相关题型

- [LC 1309] Decrypt String from Alphabet to Integer Mapping（字符映射）
- [LC 929] Unique Email Addresses（字符串变换 + 去重）
- [LC 49] Group Anagrams（字符串分组 + 哈希）
- [LC 205] Isomorphic Strings（字符映射关系）
