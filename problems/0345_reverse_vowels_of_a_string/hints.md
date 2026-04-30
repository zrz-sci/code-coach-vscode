# 345. 反转字符串中的元音字母

## 核心思路
使用双指针（左右对撞指针），左指针从头部、右指针从尾部向中间移动。两个指针分别跳过非元音字符，当都停在元音字符上时交换两个字符，然后继续向内移动。

## 思维链
1. 看到"反转特定元素，保持其余不变" → 对撞双指针
2. 元音字母包括大小写：`a, e, i, o, u, A, E, I, O, U`
3. 左指针右移跳过非元音，右指针左移跳过非元音
4. 两个都停在元音上时交换 → `swap(s[left], s[right])`
5. 交换后两指针各进一步，继续寻找下一对
6. 直到 `left >= right` 结束

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 对撞双指针 | O(n) | O(1) | 原地操作，面试首选 |
| 提取元音 + 反转回填 | O(n) | O(n) | 思路简单但空间不优 |

## 关键提示
1. **元音包括大小写**：不要只写小写字母！共 10 个：`aeiouAEIOU`
2. **用 set 或 string 判断**：`string vowels = "aeiouAEIOU"; vowels.find(c) != string::npos`
3. **原地修改**：C++ 中 string 是可变的，直接 `swap` 即可
4. **循环内部的 while 跳过非元音时，注意 `left < right` 的条件**，防止越界

## 解法详解

### 解法一：对撞双指针（推荐）

**思路**：左右指针各自跳过非元音字符，遇到元音时交换。

```cpp
class Solution {
public:
    string reverseVowels(string s) {
        string vowels = "aeiouAEIOU";
        int left = 0, right = s.size() - 1;
        
        while (left < right) {
            // 左指针跳过非元音
            while (left < right && vowels.find(s[left]) == string::npos)
                left++;
            // 右指针跳过非元音
            while (left < right && vowels.find(s[right]) == string::npos)
                right--;
            
            if (left < right) {
                swap(s[left], s[right]);
                left++;
                right--;
            }
        }
        return s;
    }
};
```

**复杂度**：时间 O(n)，空间 O(1)

### 解法二：提取元音 + 反转回填

**思路**：先遍历一次提取所有元音到一个数组中，反转该数组，再遍历一次将元音填回原位。

```cpp
class Solution {
public:
    string reverseVowels(string s) {
        string vowels = "aeiouAEIOU";
        vector<char> extracted;
        
        // 提取所有元音
        for (char c : s) {
            if (vowels.find(c) != string::npos)
                extracted.push_back(c);
        }
        
        // 反转回填
        int idx = extracted.size() - 1;
        for (int i = 0; i < s.size(); i++) {
            if (vowels.find(s[i]) != string::npos) {
                s[i] = extracted[idx--];
            }
        }
        return s;
    }
};
```

**复杂度**：时间 O(n)，空间 O(n)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 只判断小写元音 `"aeiou"` | `"aeiouAEIOU"` | 大写元音也要处理 |
| `while (vowels.find(s[left]) == npos) left++;` | 加 `left < right` 条件 | 内层 while 也需要边界检查 |
| 交换后忘记 `left++; right--;` | 交换后两指针都要移动 | 否则死循环 |
| 用 `unordered_set<char>` 判断元音 | 可以，O(1) 查找 | `find` 在短字符串上也是 O(1)，差别不大 |

## 面试追问

**Q1: 如果要反转辅音字母（非元音），怎么改？**
> 只需把 `find` 的条件取反即可：`vowels.find(s[left]) != string::npos` 变为 `vowels.find(s[left]) == string::npos`。也就是跳过元音而不是跳过非元音。

**Q2: 如果字符串不可变（如 Java 中的 String），怎么处理？**
> 先转为 `char[]` / `StringBuilder`，操作完再转回 `String`。或者使用提取+反转回填的方法，构建新字符串。

**Q3: 时间复杂度是 O(n) 还是 O(n^2)？find 操作影响吗？**
> 是 O(n)。虽然 `vowels.find(c)` 是 O(10)，但 10 是常数。如果担心，可以用 `unordered_set<char>` 将查找优化为严格 O(1)。或者直接用一个 `bool isVowel[256]` 数组预处理。

## 相关题型
- [344. 反转字符串](https://leetcode.com/problems/reverse-string/) - 对撞双指针基础
- [125. 验证回文串](https://leetcode.com/problems/valid-palindrome/) - 对撞双指针 + 跳过非字母数字
- [917. 仅仅反转字母](https://leetcode.com/problems/reverse-only-letters/) - 同一模式：反转特定字符
- [2000. 反转单词前缀](https://leetcode.com/problems/reverse-prefix-of-word/) - 字符串反转变种
