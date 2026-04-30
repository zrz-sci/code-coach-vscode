# 241. 为运算表达式设计优先级 (Different Ways to Add Parentheses)

## 核心思路

本题是经典的**分治递归**问题：给定一个包含数字和运算符（+、-、*）的字符串，返回所有可能的加括号方式所能得到的不同结果。

核心思想：**以每个运算符为"根"进行分割**。选定一个运算符后，左边的子表达式递归产生所有可能结果，右边的子表达式也递归产生所有可能结果，然后对左右结果进行笛卡尔积组合。

这和**构建所有可能的表达式树**是等价的 —— 每种加括号方式对应一棵不同的表达式树，而表达式树的根就是"最后一个被计算的运算符"。

## 思维链

1. **读完题第一反应**：要列举所有加括号的方式。不同的括号方式决定了运算顺序，也就决定了最终结果。例如 `2-1-1` 可以是 `(2-1)-1=0` 或 `2-(1-1)=2`。

2. **怎么枚举所有方式**：选择"最后一个执行的运算符"。比如 `2-1-1`，如果最后执行第一个减号，就是 `(2) - (1-1)`；如果最后执行第二个减号，就是 `(2-1) - (1)`。

3. **自然的递归结构**：
   - 遍历表达式中的每个运算符
   - 以该运算符为分割点，把表达式分成左右两个子表达式
   - 递归求左边所有可能的值，递归求右边所有可能的值
   - 左右结果做笛卡尔积：对每对 (左值, 右值)，用当前运算符计算

4. **base case**：当表达式中没有运算符时，它是一个纯数字，直接返回该数字。

5. **可选优化 —— 记忆化**：相同的子表达式可能从不同的分割路径被递归到，用 HashMap 缓存可以避免重复计算。

## 递归树可视化

**示例: expression = "2*3-4*5"**

```
                          dfs("2*3-4*5")
                     /          |          \
            op='*'(pos 1)   op='-'(pos 3)   op='*'(pos 5)
               /    \          /     \         /      \
        dfs("2") dfs("3-4*5") dfs("2*3") dfs("4*5") dfs("2*3-4") dfs("5")
          [2]      /    \        [6]       [20]       /    \        [5]
              op='-'  op='*'                    op='*'  op='-'
              / \      / \                      / \      / \
          dfs("3") dfs("4*5") dfs("3-4") dfs("5")  dfs("2") dfs("3-4") dfs("2*3") dfs("4")
            [3]      [20]     [-1]     [5]   [2]    [-1]     [6]      [4]
```

**展开第一个分支: op='*' at pos 1**
```
左: dfs("2") = [2]
右: dfs("3-4*5")
    ├── op='-': dfs("3")=[3], dfs("4*5")=[20] → 3-20 = -17
    └── op='*': dfs("3-4")=[-1], dfs("5")=[5] → -1*5 = -5
    右结果: [-17, -5]
组合: 2 * (-17) = -34,  2 * (-5) = -10
```

**展开第二个分支: op='-' at pos 3**
```
左: dfs("2*3") = [6]    (只有一种分割)
右: dfs("4*5") = [20]   (只有一种分割)
组合: 6 - 20 = -14
```

**展开第三个分支: op='*' at pos 5**
```
左: dfs("2*3-4")
    ├── op='*': dfs("2")=[2], dfs("3-4")=[-1] → 2*(-1) = -2
    └── op='-': dfs("2*3")=[6], dfs("4")=[4] → 6-4 = 2
    左结果: [-2, 2]
右: dfs("5") = [5]
组合: -2 * 5 = -10,  2 * 5 = 10
```

**最终结果: [-34, -10, -14, -10, 10]**

```
对应的五种加括号方式:
  (2*(3-(4*5))) = 2*(3-20)   = 2*(-17) = -34
  (2*((3-4)*5)) = 2*((-1)*5) = 2*(-5)  = -10
  ((2*3)-(4*5)) = 6-20       = -14
  ((2*(3-4))*5) = (2*(-1))*5 = (-2)*5  = -10
  (((2*3)-4)*5) = (6-4)*5    = 2*5     = 10
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 分治递归 | 以每个运算符为分割点 | O(C_n * n) | O(C_n * n) | ⭐ 面试首选 |
| 分治 + 记忆化 | 缓存子表达式结果 | O(C_n * n) | O(C_n * n) | 追问优化 |
| 区间 DP | dp[i][j] = 第i到第j个数的所有结果 | O(C_n * n) | O(C_n * n^2) | 理解 DP 形式 |

> 其中 C_n 是第 n 个卡特兰数（n 为运算符个数），C_n ~ 4^n / n^(3/2)

## 为什么是卡特兰数

n 个运算符的表达式，所有加括号方式的数量恰好是第 n 个**卡特兰数** C(n)。

这是因为每种加括号方式对应一棵**有 n 个内部节点的满二叉树**（运算符是内部节点，数字是叶子），而 n 个内部节点的满二叉树的数量正是 C(n)。

```
C(0) = 1,  C(1) = 1,  C(2) = 2,  C(3) = 5,  C(4) = 14, ...

表达式中有 3 个运算符 → C(3) = 5 种加括号方式 → 5 个结果
(与示例 2 的 5 个结果吻合!)
```

## 关键提示

1. **枚举所有运算符位置**：遍历字符串，遇到 `+`、`-`、`*` 就作为分割点。每个运算符对应一种"最后计算这个运算符"的方案。

2. **左右子问题完全独立**：分割后，左半部分的所有可能结果和右半部分的所有可能结果做笛卡尔积。

3. **纯数字判断**：如果遍历完整个表达式都没遇到运算符，说明它是纯数字，直接 `stoi` 返回。

4. **记忆化的 key**：用子字符串（或起止下标对 (i,j)）作为缓存的 key。

5. **和分治排序的异同**：归并排序的分治是固定分割点（中间），本题的分治是枚举所有可能的分割点（每个运算符）。

## 解法详解

### 解法1: 分治递归 — O(C_n * n) / O(C_n * n) ⭐ 面试首选

**思路**：遍历表达式中的每个运算符，以其为分割点递归求解左右子表达式的所有结果，然后笛卡尔积组合。

```cpp
class Solution {
public:
    vector<int> diffWaysToCompute(string expression) {
        vector<int> result;
        
        for (int i = 0; i < (int)expression.size(); i++) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                // 以 c 为分割点
                auto left = diffWaysToCompute(expression.substr(0, i));
                auto right = diffWaysToCompute(expression.substr(i + 1));
                
                // 笛卡尔积组合
                for (int l : left) {
                    for (int r : right) {
                        if (c == '+') result.push_back(l + r);
                        else if (c == '-') result.push_back(l - r);
                        else result.push_back(l * r);
                    }
                }
            }
        }
        
        // base case: 没有运算符，是纯数字
        if (result.empty()) {
            result.push_back(stoi(expression));
        }
        
        return result;
    }
};
```

**关键点**：
- 代码极其简洁，只有二十几行
- base case 的判断巧妙：如果循环结束后 result 仍为空，说明没有运算符
- substr 拆分左右子表达式

### 解法2: 分治 + 记忆化 — 优化常数

**从解法1优化**：相同子表达式可能被多次递归到。例如 `a+b-c*d` 中，子表达式 `b-c` 可能从不同的上层分割中被计算。用 HashMap 缓存可以避免重复。

```cpp
class Solution {
    unordered_map<string, vector<int>> memo;
    
public:
    vector<int> diffWaysToCompute(string expression) {
        // 缓存命中
        if (memo.count(expression)) return memo[expression];
        
        vector<int> result;
        
        for (int i = 0; i < (int)expression.size(); i++) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                auto left = diffWaysToCompute(expression.substr(0, i));
                auto right = diffWaysToCompute(expression.substr(i + 1));
                
                for (int l : left) {
                    for (int r : right) {
                        if (c == '+') result.push_back(l + r);
                        else if (c == '-') result.push_back(l - r);
                        else result.push_back(l * r);
                    }
                }
            }
        }
        
        if (result.empty()) {
            result.push_back(stoi(expression));
        }
        
        memo[expression] = result; // 缓存
        return result;
    }
};
```

### 解法3: 区间 DP — dp[i][j]

**思路**：先预处理，把表达式拆成数字数组 `nums` 和运算符数组 `ops`。定义 `dp[i][j]` = 用第 i 到第 j 个数字（含）能得到的所有可能结果。

转移：枚举分割点 k (i <= k < j)，`dp[i][j]` = 对所有 `dp[i][k]` 中的值 l 和 `dp[k+1][j]` 中的值 r，用 `ops[k]` 组合。

```cpp
class Solution {
public:
    vector<int> diffWaysToCompute(string expression) {
        // 预处理: 提取数字和运算符
        vector<int> nums;
        vector<char> ops;
        int num = 0;
        for (char c : expression) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            } else {
                nums.push_back(num);
                ops.push_back(c);
                num = 0;
            }
        }
        nums.push_back(num);
        
        int n = nums.size();
        // dp[i][j] = 第i到第j个数字能产生的所有结果
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n));
        
        // base case: 单个数字
        for (int i = 0; i < n; i++) {
            dp[i][i] = {nums[i]};
        }
        
        // 区间长度从 2 到 n
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                for (int k = i; k < j; k++) {
                    for (int l : dp[i][k]) {
                        for (int r : dp[k + 1][j]) {
                            if (ops[k] == '+') dp[i][j].push_back(l + r);
                            else if (ops[k] == '-') dp[i][j].push_back(l - r);
                            else dp[i][j].push_back(l * r);
                        }
                    }
                }
            }
        }
        
        return dp[0][n - 1];
    }
};
```

## 解法对比

| | 分治递归 ⭐ | 分治 + 记忆化 | 区间 DP |
|---|---|---|---|
| 核心思想 | 选运算符分割，递归 | 同左 + 缓存 | 区间枚举分割点 |
| 代码复杂度 | 最简洁 | 中等 | 较复杂 |
| 重复计算 | 可能有 | 避免 | 无（自底向上） |
| 面试推荐 | **首选** | 追问优化 | 理解 DP 形式 |

## 易错点

1. **多位数字**
   - 数字可以是 10、99 等两位数，不能把每个字符当成单独的数字
   - 解法1 中 `stoi(expression)` 自然处理多位数
   - 解法3 中预处理时要正确解析多位数

2. **忘记处理纯数字 base case**
   - 如果表达式只是一个数字（如 "42"），循环不会进入任何 if 分支
   - 必须在循环后检查 result 是否为空，为空则 push 数字

3. **substr 的边界**
   - `expression.substr(0, i)` 是左半部分（不含运算符）
   - `expression.substr(i + 1)` 是右半部分（不含运算符）
   - i 是运算符在字符串中的下标

4. **区间 DP 的运算符下标**
   - `ops[k]` 是第 k 个运算符，连接第 k 个数和第 k+1 个数
   - 分割点 k 的范围是 `i <= k < j`（不含 j）

## 面试追问

**Q1（递归 vs 迭代）**：能不能用迭代（DP）代替递归？
> 可以。区间 DP：`dp[i][j]` 表示第 i 到第 j 个数字能得到的所有结果，按区间长度从小到大填表。本质和递归完全等价，只是自底向上。

**Q2（复杂度分析）**：为什么是卡特兰数？
> n 个运算符的表达式，每种加括号方式对应一棵有 n 个内部节点的满二叉树。n 个内部节点的满二叉树数量 = 卡特兰数 C(n) = C(2n,n)/(n+1)。所以结果数量上界为 C(n)。

**Q3（记忆化的效果）**：记忆化能提升多少？
> 不改变最坏情况复杂度（因为不同子表达式的数量本身就和结果数量相关），但能避免完全相同子表达式的重复计算。实际效果取决于表达式中重复子串的数量。

**Q4（变体）**：如果表达式中还有除法怎么办？
> 分治框架不变，只需在笛卡尔积组合时多加一个 `c == '/'` 的分支，注意除数为 0 的情况。如果要求浮点除法，返回类型改为 `vector<double>`。

**Q5（变体）**：如果只需要返回最大值/最小值？
> 可以用区间 DP，dp[i][j] 只存最大值和最小值（不存所有值）。转移时从 dp[i][k].max/min 和 dp[k+1][j].max/min 中选最优组合。注意负数乘负数可能变最大。

## 相关题型

- **95. Unique Binary Search Trees II** — 同样的分治思想：枚举根节点，递归生成左右子树的所有可能，然后笛卡尔积组合。结果数量也是卡特兰数。复用完全相同的"枚举分割点 + 递归 + 组合"框架。
- **96. Unique Binary Search Trees** — 只需要数量不需要枚举，卡特兰数公式直接计算。
- **22. Generate Parentheses** — 生成所有合法括号组合，数量同样是卡特兰数。
- **312. Burst Balloons** — 区间 DP 经典题，同样枚举区间内的分割点。
- **932. Beautiful Array** — 分治构造，类似的"选分割点递归构建"思路。
