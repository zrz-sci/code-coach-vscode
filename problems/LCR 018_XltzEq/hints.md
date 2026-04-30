# LCR 018. 验证回文串 - 提示与详解

## 难度：Easy | 标签：Two Pointers, String

---

## 一、核心思路

本题（同主站 125 题）要求验证一个字符串在**只考虑字母和数字、忽略大小写**的条件下是否为回文串。核心方法是**双指针对撞**：一个指针从头向尾、另一个从尾向头，跳过非字母数字字符后比较（忽略大小写）。

**核心洞察**：不需要预处理字符串（过滤+转小写再翻转比较），双指针可以原地完成判断，O(1) 额外空间。

---

## 二、思维链：从题目到解法

### Step 1：理解"只考虑字母和数字"

```
输入: "A man, a plan, a canal: Panama"

有效字符提取:
  A m a n a p l a n a c a n a l P a n a m a
→ 全部转小写:
  a m a n a p l a n a c a n a l p a n a m a

正读: amanaplanacanalpanama
反读: amanaplanacanalpanama
相同 → 回文 → true
```

### Step 2：暴力解法 -- 预处理后比较

```
方法: 
1. 遍历字符串, 提取所有字母/数字字符, 转小写 → 新串 cleaned
2. 判断 cleaned 是否等于 reverse(cleaned)

"A man, a plan, a canal: Panama"
  → cleaned = "amanaplanacanalpanama"
  → reversed = "amanaplanacanalpanama"
  → cleaned == reversed → true

时间: O(n), 空间: O(n)
```

### Step 3：优化 -- 双指针对撞（O(1) 空间）

```
"A man, a plan, a canal: Panama"
 ^                             ^
 L                             R

L 和 R 分别从两端开始:
1. L 右移跳过非字母数字字符
2. R 左移跳过非字母数字字符  
3. 比较 tolower(s[L]) 和 tolower(s[R])
   - 相同 → L++, R--, 继续
   - 不同 → return false
4. 当 L >= R 时, 全部匹配完 → return true
```

### Step 4：详细模拟

```
s = "A man, a plan, a canal: Panama"
     0123456789...

L=0, R=29
s[0]='A' (有效), s[29]='a' (有效)
tolower('A')='a' == tolower('a')='a' ✓
L=1, R=28

s[1]=' ' (无效) → L=2
s[2]='m' (有效), s[28]='m' (有效)
'm' == 'm' ✓
L=3, R=27

s[3]='a' (有效), s[27]='a' (有效)
'a' == 'a' ✓
L=4, R=26

s[4]='n' (有效), s[26]='n' (有效)
'n' == 'n' ✓
...
最终 L >= R → return true
```

### Step 5：反例分析

```
s = "race a car"

有效字符: r a c e a c a r
正读: raceacar
反读: racaecar  ← 不同!

双指针:
L=0, R=9
s[0]='r', s[9]='r' → 'r'=='r' ✓
L=1, R=8
s[1]='a', s[8]='a' → 'a'=='a' ✓
L=2, R=7
s[2]='c', s[7]='c' → 'c'=='c' ✓
L=3, R=6 (skip space at 4)
s[3]='e', s[6]='a' → 'e'!='a' ✗
return false
```

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 双指针对撞 | 两端向中间, 跳过无效字符 | O(n) | O(1) | 面试首选 |
| 预处理+翻转 | 提取有效字符后翻转比较 | O(n) | O(n) | 简单直观 |
| 预处理+双指针 | 提取后双指针 | O(n) | O(n) | 折中方案 |

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：如何判断字符是否为字母或数字？</summary>

C++ 中使用 `isalnum(ch)` 函数，返回 true 表示 ch 是字母或数字。也可以手动判断：`(ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')`。

</details>

<details>
<summary>提示 2：如何忽略大小写比较？</summary>

使用 `tolower(ch)` 将字符转为小写后再比较。注意 `tolower` 对数字字符无影响（'0'-'9' 不会被改变），所以可以安全地对所有有效字符使用。

</details>

<details>
<summary>提示 3：双指针的循环条件是什么？</summary>

外层循环条件是 `left < right`。在内层跳过无效字符时也要保证 `left < right`，防止两个指针交叉。当 `left >= right` 时，说明所有有效字符对都匹配，是回文。

</details>

<details>
<summary>提示 4：空字符串算回文吗？</summary>

是的，题目明确说空字符串定义为有效的回文串。如果所有字符都不是字母/数字，提取后也是空串，应该返回 true。双指针天然处理了这种情况（left 从一开始就 >= right）。

</details>

<details>
<summary>提示 5：有没有不用 isalnum/tolower 的写法？</summary>

可以自己写 helper 函数，或者先预处理整个字符串。预处理法：遍历一次把所有字母/数字字符收集到新串（同时转小写），然后双指针比较新串，或者直接比较新串和反转后的新串。

</details>

---

## 五、解法详解

### 解法一：双指针对撞（面试首选，O(1) 空间）

```cpp
class Solution {
public:
    bool isPalindrome(string s) {
        int left = 0, right = (int)s.size() - 1;

        while (left < right) {
            // 跳过左侧非字母数字字符
            while (left < right && !isalnum(s[left])) {
                left++;
            }
            // 跳过右侧非字母数字字符
            while (left < right && !isalnum(s[right])) {
                right--;
            }

            // 比较（忽略大小写）
            if (tolower(s[left]) != tolower(s[right])) {
                return false;
            }

            left++;
            right--;
        }

        return true;
    }
};
```

**指针移动可视化**:

```
s = "A man, a plan, a canal: Panama"
     ^                            ^
     L                            R

Step 1: s[L]='A', s[R]='a' → 'a'=='a' ✓ → L++, R--
     " man, a plan, a canal: Panam "
      ^                          ^

Step 2: s[L]=' ' → skip → L=2
     " man, a plan, a canal: Panam "
       ^                        ^
     s[L]='m', s[R]='m' → 'm'=='m' ✓

... 持续匹配直到 L >= R → return true
```

### 解法二：预处理 + 翻转比较

```cpp
class Solution {
public:
    bool isPalindrome(string s) {
        // Step 1: 提取字母和数字，转小写
        string cleaned;
        for (char ch : s) {
            if (isalnum(ch)) {
                cleaned += tolower(ch);
            }
        }

        // Step 2: 比较与反转是否相同
        string reversed = cleaned;
        reverse(reversed.begin(), reversed.end());
        return cleaned == reversed;
    }
};
```

### 解法三：预处理 + 双指针

```cpp
class Solution {
public:
    bool isPalindrome(string s) {
        // Step 1: 提取有效字符并转小写
        string cleaned;
        for (char ch : s) {
            if (isalnum(ch)) {
                cleaned += tolower(ch);
            }
        }

        // Step 2: 双指针判断 cleaned 是否回文
        int left = 0, right = (int)cleaned.size() - 1;
        while (left < right) {
            if (cleaned[left] != cleaned[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
};
```

---

## 六、isalnum 和 tolower 细节

```
isalnum(ch) 的行为:
  'a'-'z' → true     (小写字母)
  'A'-'Z' → true     (大写字母)
  '0'-'9' → true     (数字)
  ' ', ',', ':', etc. → false (其他)

tolower(ch) 的行为:
  'A' → 'a'
  'Z' → 'z'
  'a' → 'a' (不变)
  '3' → '3' (不变, 数字不受影响)

⚠️ C++ 注意事项:
tolower 接收 unsigned char, 传入负值(如中文字符)可能有未定义行为
安全写法: tolower(static_cast<unsigned char>(ch))
本题保证 ASCII 字符, 所以直接 tolower(ch) 安全
```

---

## 七、边界情况分析

```
1. 空串 "" → true (空串是回文)
2. 只有非字母数字: ",.;:" → true (有效字符为空)
3. 单个字符: "a" → true
4. 全大写: "ABA" → true
5. 数字: "121" → true
6. 字母+数字混合: "a1b1a" → true
7. 大小写混合: "Aa" → true (忽略大小写)
8. 几乎回文: "ab" → false
```

---

## 八、易错点

### Bug 1：内层跳过时忘记检查 left < right

```cpp
// 错误: 可能越界
while (!isalnum(s[left])) left++;
while (!isalnum(s[right])) right--;

// 正确: 必须检查 left < right
while (left < right && !isalnum(s[left])) left++;
while (left < right && !isalnum(s[right])) right--;
```

### Bug 2：比较时忘记转小写

```cpp
// 错误: 大小写敏感比较
if (s[left] != s[right]) return false;

// 正确: 转小写后比较
if (tolower(s[left]) != tolower(s[right])) return false;
```

### Bug 3：空串初始化 right 为 -1

```cpp
int right = s.size() - 1; // s.size() 返回 size_t (unsigned)
// 如果 s 为空, s.size() - 1 = 巨大正数!
// 安全写法:
int right = (int)s.size() - 1; // 空串时 right = -1, while(0 < -1) 不进入
```

### Bug 4：把数字当成无效字符

```cpp
// 错误: 只检查字母
bool isValid(char c) { return isalpha(c); }

// 正确: 检查字母和数字
bool isValid(char c) { return isalnum(c); }
// "0P" → 有效字符 "0p", 不是回文
```

---

## 九、面试追问（递进链）

### Q1：能否不用库函数 isalnum/tolower？

可以手写：
```cpp
bool myIsAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
        || (c >= '0' && c <= '9');
}
char myToLower(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}
```

### Q2：如果允许删除最多一个字符使其成为回文呢？

这就是 LeetCode 680 "Valid Palindrome II"。双指针发现不匹配时，分别尝试跳过左边或右边一个字符，检查剩余部分是否回文。

### Q3：如果要求返回最长回文子串呢？

这就是 LeetCode 5 "Longest Palindromic Substring"，需要中心扩展法或 Manacher 算法。

### Q4：时间复杂度能优于 O(n) 吗？

不能。至少需要读取整个字符串才能判断（最后一个字符可能影响结果），所以 O(n) 是下界。

### Q5：Unicode 字符串怎么处理？

需要正确处理 UTF-8 编码，字母/数字的判断要用 Unicode 分类。C++ 中可以用 `<locale>` 或第三方 ICU 库。本题保证 ASCII，不需要考虑。

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 125 | Valid Palindrome | 本题主站版本（完全相同） |
| 680 | Valid Palindrome II | 允许删除一个字符 |
| 234 | Palindrome Linked List | 链表版回文判断 |
| 9 | Palindrome Number | 数字回文判断 |
| 5 | Longest Palindromic Substring | 最长回文子串 |
| 面试题 01.04 | Palindrome Permutation | 判断能否组成回文 |

---

## 十一、总结

```
         LCR 018 / 125. Valid Palindrome
                    |
         双指针对撞 (left, right)
              /           \
     跳过非字母数字        比较字符
     left++/right--       tolower() 忽略大小写
              \           /
         left < right 循环
                    |
         全部匹配 → true
         发现不同 → false
```

**记忆口诀**：两端出发向中间，跳过杂字比字母；大小写别忘转换，交叉即为回文串。
