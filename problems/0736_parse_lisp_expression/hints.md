# 736. Parse Lisp Expression - Lisp 语法解析

## 难度: Hard

## 标签: Stack, Recursion, Hash Table, String

## 核心思路

本题要求实现一个简化版的 Lisp 表达式求值器。表达式有三种操作 (`let`/`add`/`mult`)，核心难点是**递归下降解析 + 作用域管理**。用一个指针逐字符扫描字符串，根据首字符判断表达式类型，递归求值。`let` 表达式的作用域需要通过拷贝 scope（或用栈式结构）来实现隔离。

## 思维链

1. **分类表达式类型**: 以 `(` 开头是括号表达式，以数字/`-` 开头是整数，以小写字母开头是变量
2. **括号表达式内部**: 读取操作符（`let`/`add`/`mult`），根据类型分别处理
3. **add/mult 简单**: 递归求两个子表达式的值，做加法或乘法
4. **let 最复杂**: 循环读取 (变量名, 表达式) 对做赋值，最后一个表达式是返回值
5. **作用域隔离**: 每进入一个 let，拷贝当前 scope 副本；let 结束后自动恢复外层 scope
6. **判断 let 的结尾**: 读到一个 token 后，如果紧跟 `)` 说明它是最终返回值表达式

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 递归下降解析器 | O(n^2) | O(n^2) | 最自然的写法，scope 拷贝导致平方开销 |
| 栈式作用域优化 | O(n) | O(n) | 用栈管理变量，进出 let 时 push/pop，避免拷贝 |

## 关键提示

1. **指针引用传递**: 解析函数的 `pos` 参数必须是引用（`int& pos`），保证递归调用后位置正确推进
2. **scope 拷贝语义**: `eval(s, pos, scope)` 中 scope 按值传递，进入 let 的绑定不影响外部
3. **token 结束判断**: 一个 token 以空格或 `)` 结尾，解析时注意 `pos` 停在分隔符上
4. **let 的最终表达式判别**: 读完一个元素后，若下一个字符是 `)` 说明该元素就是返回值
5. **负数解析**: `-` 开头可能是负数，需要单独处理 `-` 后跟数字的情况

## 解法详解

### 解法一：递归下降解析器（推荐）

**思路**: 实现三个辅助函数 `eval`（递归求值）、`parseToken`（读取变量名或操作符）、`parseInt`（读取整数），通过一个全局指针 `pos` 逐步推进。

**核心逻辑**:
- `eval(s, pos, scope)`:
  - 首字符 `(` -> 跳过，读操作符 -> 根据 `add`/`mult`/`let` 分支处理
  - 首字符数字或 `-` -> 调用 `parseInt`
  - 首字符小写字母 -> 调用 `parseToken` 得变量名，从 scope 查值
- `let` 分支的循环逻辑:
  1. 跳过空格
  2. 如果当前字符是 `(`、数字或 `-`，直接递归求值作为候选返回值
  3. 否则读取变量名；若变量名后紧跟 `)`，则该变量就是返回值
  4. 否则跳过空格，递归求值赋给该变量，继续循环

**时间复杂度**: O(n^2) - 每层 let 可能拷贝整个 scope

**空间复杂度**: O(n^2) - 嵌套 let 的 scope 拷贝

### 解法二：栈式作用域优化

**思路**: 用 `unordered_map<string, vector<int>>` 为每个变量维护一个值栈。进入 let 绑定变量时 `push_back`，退出 let 时 `pop_back`。这样避免 scope 整体拷贝。

**核心差异**: 不再按值传递 scope，而是全局维护变量栈。需要记录每个 let 绑定了哪些变量，退出时逐一回退。

**时间复杂度**: O(n)

**空间复杂度**: O(n)

## 易错点

1. **空格跳过时机**: `pos++` 跳过空格的时机容易搞错，必须在读取下一个 token 前确保 `pos` 指向有效字符
2. **let 最终表达式的判断**: 容易将 let 的变量赋值和最终返回值搞混。关键是判断读完一个 token 后下一个字符是否为 `)`
3. **scope 传递方式**: 如果 scope 用引用传递，let 内部的绑定会污染外层作用域，导致结果错误
4. **嵌套括号解析**: `(let x 2 (mult x (let x 3 y 4 (add x y))))` 中内层 let 的 `x=3` 不应影响外层 `x=2`
5. **变量名 vs 关键字**: `add`/`let`/`mult` 是保护关键字不会用作变量名，但解析时仍需正确区分上下文
6. **parseInt 与 parseToken 选择**: 以 `-` 开头时是负整数，以小写字母开头才是变量名

## 面试追问

1. **如果新增 sub/div 操作怎么办?** 在 `eval` 的分支里新增对应逻辑即可，解析框架不变
2. **如何支持布尔类型和 if 表达式?** 扩展返回类型为 variant，新增 `if` 分支判断条件后选择求值哪个子表达式
3. **如何实现 lambda 和函数调用?** 需要在 scope 中存储函数定义（参数列表+函数体），调用时建立新的局部 scope
4. **如何优化嵌套 let 的性能?** 用栈式作用域管理（变量名 -> 值栈），避免 O(n) 的 scope 拷贝
5. **如果表达式可能语法错误，如何做错误处理?** 在每个解析步骤加入边界检查和异常抛出，如未匹配的括号、未定义的变量等

## 相关题型

- [385. Mini Parser](https://leetcode.com/problems/mini-parser/) - 嵌套结构解析
- [394. Decode String](https://leetcode.com/problems/decode-string/) - 递归解析带嵌套的字符串
- [439. Ternary Expression Parser](https://leetcode.com/problems/ternary-expression-parser/) - 表达式解析
- [770. Basic Calculator IV](https://leetcode.com/problems/basic-calculator-iv/) - 高级表达式求值
- [224. Basic Calculator](https://leetcode.com/problems/basic-calculator/) - 基础计算器递归解析
- [726. Number of Atoms](https://leetcode.com/problems/number-of-atoms/) - 递归解析化学式
