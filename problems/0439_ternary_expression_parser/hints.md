# 439. Ternary Expression Parser

## 核心思路
三元表达式从右向左结合。可以用 **栈从右往左扫描** 逐步归约，或者用 **递归** 从左往右解析（利用 `?` 计数找到对应的 `:`）。

## 思维链
1. 表达式形如 `cond ? expr1 : expr2`，右结合
2. **栈方法**：从右往左扫描，遇到 `?` 前面的条件字符时归约栈顶两个值
3. **递归方法**：从左往右，第一个字符是条件，跳过 `?`，递归解析 true 分支，找到匹配的 `:`，递归解析 false 分支
4. 找匹配 `:` 的关键：用 `?` 和 `:` 的计数差来定位同层级的 `:`

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 栈（从右往左） | O(n) | O(n) | 最直观，面试首选 |
| 递归解析 | O(n) | O(n) | 自顶向下，逻辑清晰 |

## 关键提示
1. **右结合**：`F?1:T?4:5` 等价于 `F?1:(T?4:5)`，从右往左处理天然满足
2. **栈归约**：从右往左扫描，遇到非 `?/:` 字符入栈；遇到 `?` 时取栈顶两个值，根据条件选一个
3. **递归分割**：条件后的 `?` 后面是 true 分支，需要找到同层级的 `:` 来分割 true/false 分支
4. **所有数字都是一位数**：简化了解析

## 解法详解

### 解法一：栈（从右往左）⭐

**思路**：从右往左遍历表达式。遇到字母/数字入栈，遇到 `:` 跳过，遇到 `?` 时从栈取两个值，根据 `?` 前面的条件字符选择保留哪个。

```cpp
string parseTernary(string expression) {
    stack<char> st;
    for (int i = expression.size() - 1; i >= 0; i--) {
        char c = expression[i];
        if (!st.empty() && st.top() == '?') {
            st.pop(); // pop '?'
            char first = st.top(); st.pop();
            st.pop(); // pop ':'
            char second = st.top(); st.pop();
            st.push(c == 'T' ? first : second);
        } else {
            st.push(c);
        }
    }
    return string(1, st.top());
}
```

**复杂度**：时间 O(n)，空间 O(n)

### 解法二：递归解析

**思路**：用下标 pos 从左往右解析。当前字符是条件，如果后面跟 `?` 则递归解析 true/false 分支。

```cpp
string parseTernary(string expression) {
    int pos = 0;
    return string(1, parse(expression, pos));
}

char parse(string& s, int& pos) {
    char c = s[pos++];
    if (pos < s.size() && s[pos] == '?') {
        pos++; // skip '?'
        char trueVal = parse(s, pos);
        pos++; // skip ':'
        char falseVal = parse(s, pos);
        return c == 'T' ? trueVal : falseVal;
    }
    return c;
}
```

## 易错点
- ✗ 从左往右用栈处理，但三元表达式右结合导致顺序错误 → ✓ 从右往左扫描
- ✗ 栈归约时弄错 first/second 的含义 → ✓ 从右往左扫描时，first 是 true 分支（先入栈），second 是 false 分支
- ✗ 递归时忘记跳过 `:` → ✓ 解析完 true 分支后 `pos++` 跳过 `:`

## 面试追问

**Q1: 如果数字不是一位数而是多位数怎么办？**
需要额外解析数字：在遇到数字时继续读取直到非数字字符，构建完整数字字符串。栈中存 string 而非 char。

**Q2: 如何处理左结合的三元表达式？**
左结合意味着 `T?1:F?2:3` 等价于 `(T?1:F)?2:3`。用栈从左往右处理即可，或递归时优先匹配最外层。

**Q3: 时间复杂度能否优于 O(n)？**
不能。必须至少读取每个字符一次，O(n) 已是下界。

## 相关题型
- [394. Decode String](https://leetcode.com/problems/decode-string/) - 栈解析嵌套结构
- [224. Basic Calculator](https://leetcode.com/problems/basic-calculator/) - 栈解析表达式
- [385. Mini Parser](https://leetcode.com/problems/mini-parser/) - 递归/栈解析嵌套
