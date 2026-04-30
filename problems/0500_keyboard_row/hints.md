# 500. Keyboard Row - 键盘行

## 核心思路
判断每个单词的所有字母是否都在美式键盘的同一行上。核心操作是**字母到行号的映射 + 逐字符校验**。

## 思维链
1. 建立每个字母到键盘行号（0/1/2）的映射
2. 对每个单词，取首字母的行号作为目标行
3. 检查剩余字母是否都在同一行
4. 全部满足则加入结果

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 哈希映射 | O(N·L) | O(1) | 直观高效，26字母固定空间 |
| Set 集合判断 | O(N·L) | O(1) | 利用集合交集/子集 |

## 关键提示
1. **大小写不敏感**：需要统一转小写（或大写）再查行号
2. **三行内容**：`"qwertyuiop"`, `"asdfghjkl"`, `"zxcvbnm"`
3. **映射方式**：数组映射比哈希表更快，26个字母用 `char - 'a'` 索引

## 解法详解

### 解法一：哈希映射 ⭐

**思路**：预建字母→行号映射表，遍历每个单词检查所有字母行号是否一致。

```cpp
class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        // 每个字母对应的键盘行号
        string rows = "12210111011122000010020202";
        // a=1, b=2, c=2, d=1, e=0, f=1, g=1, h=1, i=0, j=1, k=1, l=1,
        // m=2, n=2, o=0, p=0, q=0, r=0, s=1, t=0, u=0, v=2, w=0, x=2, y=0, z=2
        
        vector<string> res;
        for (auto& word : words) {
            char target = rows[tolower(word[0]) - 'a'];
            bool valid = true;
            for (char c : word) {
                if (rows[tolower(c) - 'a'] != target) {
                    valid = false;
                    break;
                }
            }
            if (valid) res.push_back(word);
        }
        return res;
    }
};
```

### 解法二：Set 集合

**思路**：将三行字母分别存入三个 set，判断单词字母集合是否为某行的子集。

```cpp
class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        vector<unordered_set<char>> rows = {
            {'q','w','e','r','t','y','u','i','o','p'},
            {'a','s','d','f','g','h','j','k','l'},
            {'z','x','c','v','b','n','m'}
        };
        
        vector<string> res;
        for (auto& word : words) {
            // 找首字母所在行
            int row = -1;
            for (int i = 0; i < 3; i++) {
                if (rows[i].count(tolower(word[0]))) { row = i; break; }
            }
            // 检查所有字母是否在同一行
            bool valid = true;
            for (char c : word) {
                if (!rows[row].count(tolower(c))) { valid = false; break; }
            }
            if (valid) res.push_back(word);
        }
        return res;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 直接用 `word[0] - 'a'` | `tolower(word[0]) - 'a'` | 输入可能含大写字母 |
| 硬编码行号映射写错 | 仔细校验每个字母的行号 | 26个字母逐一确认 |
| 返回转换后的小写单词 | 返回原始单词 `word` | 题目要求保留原始大小写 |

## 面试追问

**Q1: 如果键盘布局可以自定义（非美式），如何设计？**
> 将三行字符串作为参数传入，动态构建映射表，代码逻辑不变。

**Q2: 如果要求返回每个单词所在的行号（而非筛选），怎么改？**
> 返回 `vector<int>`，对每个满足条件的单词记录行号；不满足的返回 -1。

**Q3: 时间复杂度能否进一步优化？**
> 已经是 O(N*L) 最优，每个字符必须至少看一次。可以用位运算（每行一个 bitmask）做常数优化。

## 相关题型
- [49. Group Anagrams](https://leetcode.com/problems/group-anagrams/) - 字符分类/分组
- [389. Find the Difference](https://leetcode.com/problems/find-the-difference/) - 字符计数
- [409. Longest Palindrome](https://leetcode.com/problems/longest-palindrome/) - 字符频率统计
