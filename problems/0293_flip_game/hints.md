# 293. Flip Game (翻转游戏)

**难度**: Easy | **标签**: String

## 核心思路

遍历字符串，找到所有连续 "++" 的位置，将其翻转为 "--"，生成所有可能的下一步状态。

## 思维链

1. 翻转操作：将连续两个 '+' 变为两个 '-'
2. 遍历字符串，检查每个位置 i 和 i+1 是否都是 '+'
3. 如果是，生成一个新字符串：将 i 和 i+1 位置改为 '-'
4. 收集所有合法的新状态并返回

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| 线性扫描 | 遍历找 "++" 并翻转 | O(n^2) | O(n) | ⭐ |
| 原地修改+回溯 | 修改后记录再还原 | O(n^2) | O(n) | |

其中 n = currentState.length()，O(n^2) 因为每次 substr/string copy 是 O(n)

## 关键提示

```
输入: "++++"

位置扫描:
  i=0: s[0]='+'  s[1]='+'  => "--++"
  i=1: s[1]='+'  s[2]='+'  => "+--+"
  i=2: s[2]='+'  s[3]='+'  => "++--"

输出: ["--++", "+--+", "++--"]

边界情况:
  "+"   -> []   (长度<2，无法翻转)
  "-+"  -> []   (无连续 ++)
  "++"  -> ["--"]
  "+-+" -> []   (不连续)
```

## 解法详解

### 解法一：线性扫描（推荐）

```cpp
class Solution {
public:
    vector<string> generatePossibleNextMoves(string currentState) {
        vector<string> result;
        for (int i = 0; i + 1 < (int)currentState.size(); i++) {
            if (currentState[i] == '+' && currentState[i + 1] == '+') {
                string next = currentState;
                next[i] = '-';
                next[i + 1] = '-';
                result.push_back(next);
            }
        }
        return result;
    }
};
```

### 解法二：使用 find 查找

```cpp
class Solution {
public:
    vector<string> generatePossibleNextMoves(string currentState) {
        vector<string> result;
        size_t pos = 0;
        while ((pos = currentState.find("++", pos)) != string::npos) {
            string next = currentState;
            next[pos] = '-';
            next[pos + 1] = '-';
            result.push_back(next);
            pos++;  // 往前移一步继续找
        }
        return result;
    }
};
```

### 解法三：原地修改 + 还原

```cpp
class Solution {
public:
    vector<string> generatePossibleNextMoves(string currentState) {
        vector<string> result;
        for (int i = 0; i + 1 < (int)currentState.size(); i++) {
            if (currentState[i] == '+' && currentState[i + 1] == '+') {
                currentState[i] = currentState[i + 1] = '-';
                result.push_back(currentState);
                currentState[i] = currentState[i + 1] = '+';  // 还原
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `i < currentState.size()` | ✓ `i + 1 < currentState.size()` | 访问 i+1 会越界 |
| ✗ 修改原字符串不还原 | ✓ 每次拷贝或修改后还原 | 否则影响后续检查 |
| ✗ `for (int i = 0; i < currentState.size() - 1; ...)` | ✓ 转为 int 比较 `i + 1 < (int)size()` | size() 返回 unsigned，空串时 size()-1 溢出 |
| ✗ 跳过 '-' 字符后继续 | ✓ 每次只前进一步 | "++-" 中 i=0 有效，i=1 无效，不能跳2步 |

## 面试追问

**Q1: 时间复杂度分析？**
> 遍历 O(n)，每次生成新字符串 O(n)，总计 O(n^2)。最多产生 n-1 个结果。

**Q2: 能否优化到 O(n)？**
> 如果不需要返回完整字符串，只返回位置索引，则可以 O(n)。但题目要求返回字符串，不可避免 O(n) 的拷贝。

**Q3: 这道题和 Flip Game II (294) 的关系？**
> 这道题是 294 的子问题。294 需要判断先手是否必胜，本题就是生成所有合法下一步状态的函数，294 在此基础上做博弈搜索。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 294 | Flip Game II | 进阶版，博弈搜索 |
| 299 | Bulls and Cows | 字符串状态处理 |
| 481 | Magical String | 字符串模拟 |
