# 520. 检测大写字母 (Detect Capital)

**难度**: Easy | **标签**: String

## 核心思路

统计大写字母个数，合法的三种情况对应：全大写（count == len）、全小写（count == 0）、首字母大写（count == 1 且首字母是大写）。

## 思维链

1. 三种合法模式 → "ALL UPPER" / "all lower" / "First Upper"
2. 统计大写字母数量 `cnt`
3. `cnt == 0` → 全小写 ✓
4. `cnt == len` → 全大写 ✓
5. `cnt == 1 && isupper(word[0])` → 仅首字母大写 ✓
6. 其余情况 → 不合法

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| 计数法 | O(n) | O(1) | ⭐⭐⭐ |
| 直接判断三种模式 | O(n) | O(1) | ⭐⭐ |

## 关键提示

1. `isupper(c)` 判断字符是否大写
2. `cnt == 1` 时还要确认是首字母大写，不能是中间大写如 "fLag"
3. 单字符始终合法

## 解法详解

### 解法1: 计数法

```cpp
class Solution {
public:
    bool detectCapitalUse(string word) {
        int cnt = 0;
        for (char c : word) {
            if (isupper(c)) cnt++;
        }
        return cnt == 0 || cnt == (int)word.size() || (cnt == 1 && isupper(word[0]));
    }
};
```

### 解法2: 直接判断三种模式

```cpp
class Solution {
public:
    bool detectCapitalUse(string word) {
        int n = word.size();
        // 全大写
        if (all_of(word.begin(), word.end(), ::isupper)) return true;
        // 全小写
        if (all_of(word.begin(), word.end(), ::islower)) return true;
        // 首字母大写，其余小写
        if (isupper(word[0]) && all_of(word.begin() + 1, word.end(), ::islower)) return true;
        return false;
    }
};
```

## 易错点

| 错误 | 正确 |
|------|------|
| ✗ `cnt == 1` 就返回 true | ✓ 还要检查 `isupper(word[0])`，排除 "mL" |
| ✗ 忘记处理单字符 | ✓ 单字符 cnt=0 或 cnt=1 都合法，自然满足 |
| ✗ 用 `word[i] >= 'A' && word[i] <= 'Z'` 的手写判断 | ✓ 直接用 `isupper()` 更清晰 |

## 面试追问

- **Q1**: 能否用正则表达式解？
  → 可以：`^[A-Z]+$|^[a-z]+$|^[A-Z][a-z]+$`
- **Q2**: 如果增加第四种模式（如驼峰命名）怎么改？
  → 增加一个检查：首字母小写 + 后续可有单个大写字母开头的段
- **Q3**: 时间复杂度能否优于 O(n)？
  → 不能，至少需要遍历一次字符串

## 相关题型

- [709. 转换成小写字母](https://leetcode.com/problems/to-lower-case/)
- [2129. 将标题首字母大写](https://leetcode.com/problems/capitalize-the-title/)
