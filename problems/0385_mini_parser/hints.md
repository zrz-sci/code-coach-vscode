# 385. Mini Parser - 迷你语法分析器

## 核心思路
将嵌套列表的字符串表示反序列化为 NestedInteger 对象。核心是用**栈**模拟递归嵌套结构：遇到 `[` 压栈开始新列表，遇到 `]` 弹栈合并到上层。

## 思维链
1. 字符串可能是纯数字（如 "324"）→ 直接返回单整数
2. 字符串以 `[` 开头 → 嵌套列表，需要处理层级关系
3. `[` 开始新的嵌套层 → 用栈压入新的 NestedInteger
4. 数字或负数 → 解析完整数值，添加到当前层
5. `,` → 分隔符，触发前一个数字的添加
6. `]` → 当前层结束，弹出并添加到上层

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 栈模拟 | O(n) | O(d) | 经典解法，d为最大嵌套深度 |
| 递归下降 | O(n) | O(d) | 代码更简洁，递归实现 |

## 关键提示
1. 特判：如果 s[0] 不是 `[`，则整个字符串是一个整数
2. 遇到 `[`：压入一个空的 NestedInteger 到栈中
3. 遇到数字或 `-`：解析完整数字，创建 NestedInteger(num) 添加到栈顶
4. 遇到 `]`：弹出栈顶，将其添加到新的栈顶中
5. 注意负数处理：`-` 号后面跟数字

## 解法详解

### 解法一：栈模拟
```cpp
class Solution {
public:
    NestedInteger deserialize(string s) {
        if (s[0] != '[') return NestedInteger(stoi(s));
        
        stack<NestedInteger> stk;
        int i = 0, n = s.size();
        
        while (i < n) {
            if (s[i] == '[') {
                stk.push(NestedInteger());
                i++;
            } else if (s[i] == ']') {
                if (stk.size() > 1) {
                    NestedInteger top = stk.top(); stk.pop();
                    stk.top().add(top);
                }
                i++;
            } else if (s[i] == ',') {
                i++;
            } else {
                // 解析数字（包括负数）
                int j = i;
                while (j < n && s[j] != ',' && s[j] != ']') j++;
                stk.top().add(NestedInteger(stoi(s.substr(i, j - i))));
                i = j;
            }
        }
        return stk.top();
    }
};
```

### 解法二：递归下降
```cpp
class Solution {
    int i = 0;
public:
    NestedInteger deserialize(string s) {
        if (s[i] == '[') {
            NestedInteger ni;
            i++;  // 跳过 '['
            while (s[i] != ']') {
                ni.add(deserialize(s));
                if (s[i] == ',') i++;  // 跳过 ','
            }
            i++;  // 跳过 ']'
            return ni;
        } else {
            int j = i;
            while (i < s.size() && s[i] != ',' && s[i] != ']') i++;
            return NestedInteger(stoi(s.substr(j, i - j)));
        }
    }
};
```

## 易错点
| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不特判纯数字 "324" | ✓ `if (s[0] != '[') return NestedInteger(stoi(s))` | 纯数字不含括号，需要特殊处理 |
| ✗ 忘记处理负数 | ✓ 解析数字时包含 `-` 号 | `-123` 中 `-` 是数字的一部分 |
| ✗ `]` 时直接弹栈不检查栈大小 | ✓ 检查 `stk.size() > 1` | 最外层的 `]` 不需要再合并 |
| ✗ 数字解析只取一位 | ✓ 循环直到遇到 `,` 或 `]` | 数字可能是多位的 |

## 面试追问

**Q1: 如果输入可能非法（不匹配的括号），如何处理？**
> 可以在解析过程中检查栈的状态：遇到 `]` 时栈为空则报错；结束时栈应恰好剩一个元素。

**Q2: 这道题和表达式求值有什么关系？**
> 都是用栈处理嵌套结构。表达式求值需要两个栈（操作数和运算符），本题只需一个栈（NestedInteger 层级）。核心思想相同：`[` 进栈，`]` 弹栈。

**Q3: 递归解法和栈解法的本质区别是什么？**
> 本质相同，递归使用系统调用栈，显式栈使用手动栈。递归代码更简洁但有栈溢出风险。对于深度极大的输入，显式栈更安全。

## 相关题型
- [341. Flatten Nested List Iterator](https://leetcode.com/problems/flatten-nested-list-iterator/) - NestedInteger 遍历
- [394. Decode String](https://leetcode.com/problems/decode-string/) - 栈处理嵌套结构
- [224. Basic Calculator](https://leetcode.com/problems/basic-calculator/) - 栈解析表达式
