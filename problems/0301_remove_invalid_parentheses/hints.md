# 301. 删除无效的括号

## 核心思路

给定一个包含括号和字母的字符串，删除**最少数量**的无效括号，使字符串变为有效的括号串。返回所有可能的结果（不重复）。

两种主要解法：
- **BFS**：逐层尝试删除一个括号，第一次出现有效串的那一层就是答案（BFS 天然保证"最少删除"）。
- **DFS/回溯**：先扫一遍计算需要删除的左括号数 `rmL` 和右括号数 `rmR`，然后回溯搜索，精确删除指定数量的括号。

## 思维链

1. **读完题第一反应** → 暴力枚举所有删除组合？每个括号可以删或不删，最多 20 个括号，2^20 ≈ 10^6 种组合。对每种组合检查有效性 O(n)，总计 O(n * 2^n)，可以接受（n <= 25）。
2. **怎么保证"删除最少"？** → 如果用 BFS，从原串开始，每层多删一个括号。**第一次在某层发现有效串时，该层所有有效串就是答案**。不需要再往更深层搜索。
3. **BFS 的去重问题** → 删除不同位置的括号可能产生相同的字符串。例如 `(()` 删 index=0 和 index=1 都得到 `(`。用 `set` 去重。
4. **DFS 能否更高效？** → 可以！先计算需要删的左括号数 `rmL` 和右括号数 `rmR`，然后在回溯中只删除必要的括号。通过"跳过连续相同括号"剪枝避免重复。
5. **如何计算 rmL 和 rmR？** → 从左到右扫描：遇到 `(` 则 rmL++；遇到 `)` 如果 rmL > 0 则 rmL--（配对），否则 rmR++（多余的右括号）。

## 决策树（DFS 回溯）

```
s = "()()" → rmL=0, rmR=0 → 已经有效，直接返回 ["()()"]

s = "())" → rmL=0, rmR=1 → 需要删除1个右括号

s = "()())()" → rmL=0, rmR=1 → 需要删除1个右括号

决策树 (DFS 回溯) for s = "()())()"
每个节点: 处理到第 idx 个字符时的状态
选择: 对括号字符可以"保留"或"删除"

                        idx=0  '('  rmL=0, rmR=1
                       /              \
              保留 '('                  (rmL=0, 不能删左括号)
              open=1                   只能保留
                |
            idx=1  ')'  rmL=0, rmR=1
           /              \
    保留 ')'            删除 ')'          ← rmR=1, 可以删
    open=0              rmR→0, open=1
      |                    |
  idx=2 '('            idx=2 '('
  rmL=0, rmR=1         rmL=0, rmR=0
     |                    |
    ...                  ...  (不再需要删任何括号)
     |                    |
  最终得到:             最终得到:
  "(())()"              "()()()"

两条路径分别产生 "(())()" 和 "()()()"
```

```
更详细的决策树 for s = "()())()"    rmL=0, rmR=1

位置:        0   1   2   3   4   5   6
字符:        (   )   (   )   )   (   )
            |
       [保留(, open=1]
            |
       [idx=1: ')']
      /            \
 保留), open=0    删除), rmR=0
      |                |
 [idx=2: '(']     [idx=2: '(']
 保留(, open=1    保留(, open=1
      |                |
 [idx=3: ')']     [idx=3: ')']
 保留), open=0    保留), open=0
      |                |
 [idx=4: ')']     [idx=4: ')']
   /      \        保留), open=-1 ✗ (非法!)
保留)   删除)      → 回溯！必须删这个)
open=-1✗  rmR=0    但 rmR 已经=0, 不能再删
          |        → 此分支无效，回溯
 [idx=5: '(']
 保留(, open=1
      |
 [idx=6: ')']
 保留), open=0
      |
 结果: "(())()" ✓

(另一条路径: 在 idx=4 而非 idx=1 处删除 → "()()()" )
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS | 逐层删一个括号，首层有效即答案 | O(n * 2^n) | O(2^n) | 直觉好 |
| DFS 回溯 | 先算 rmL/rmR，精确回溯删除 | O(2^n) | O(n) | 更优 |

## 关键提示

1. **有效括号的判断条件**：
   - 从左到右扫描，维护 `open` 计数器
   - 遇到 `(` 则 open++，遇到 `)` 则 open--
   - 过程中 open 不能为负（右括号不能多于左括号）
   - 最终 open 必须为 0（左右括号数相等）

2. **BFS 天然保证"最少删除"**：
   - 第 0 层: 原串（删除 0 个）
   - 第 1 层: 所有删除 1 个括号后的串
   - 第 2 层: 所有删除 2 个括号后的串
   - 第一次在某层发现有效串 → 该层就是最少删除数 → 收集该层所有有效串后返回

3. **DFS 的预计算剪枝**：
   - 先扫描一遍计算 rmL（需要删的左括号数）和 rmR（需要删的右括号数）
   - 回溯时，只有 rmL > 0 才尝试删左括号，rmR > 0 才尝试删右括号
   - 这大幅减少了搜索空间

4. **DFS 跳过重复括号**：
   - 连续的相同括号（如 `)))`），删除第 1 个和删除第 2 个产生的结果相同
   - 只需在第一个位置尝试删除，后续相同字符跳过
   - 这是避免重复结果的关键剪枝

5. **字母不参与删除**：
   - 字母（a-z）直接保留，不考虑删除
   - 只有 `(` 和 `)` 可以被删除

## 解法详解

### 解法1: BFS — 逐层搜索

**思考过程**: BFS 按"删除个数"逐层扩展。每层尝试删除一个括号生成下一层。用 set 去重。一旦某层出现有效串，该层所有有效串就是答案。

```cpp
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> res;
        unordered_set<string> visited;
        queue<string> q;
        q.push(s);
        visited.insert(s);
        bool found = false;
        
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                string cur = q.front(); q.pop();
                if (isValid(cur)) {
                    res.push_back(cur);
                    found = true;
                }
                if (found) continue;
                for (int j = 0; j < (int)cur.size(); j++) {
                    if (cur[j] != '(' && cur[j] != ')') continue;
                    string next = cur.substr(0, j) + cur.substr(j + 1);
                    if (!visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
            if (found) break;
        }
        return res;
    }
    
    bool isValid(const string& s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') count--;
            if (count < 0) return false;
        }
        return count == 0;
    }
};
```

**BFS 的问题**：set 去重占用大量空间，而且每层要生成大量候选串（很多是无效的）。

---

### 解法2: DFS 回溯 + 剪枝 — 面试首选

**从 BFS 优化**: 先计算出需要删除的精确数量（rmL 和 rmR），然后回溯时**有目标地删除**。不需要 set 去重（通过"跳过连续相同括号"避免重复）。

```
预计算 rmL 和 rmR:
  s = "()())()"
  扫描过程:
    '(' → rmL=1, rmR=0
    ')' → rmL=0, rmR=0  (配对消耗一个 rmL)
    '(' → rmL=1, rmR=0
    ')' → rmL=0, rmR=0  (配对)
    ')' → rmL=0, rmR=1  (多余的右括号)
    '(' → rmL=1, rmR=1
    ')' → rmL=0, rmR=1  (配对)
  结果: rmL=0, rmR=1 → 需要删 0 个左括号，1 个右括号
```

```cpp
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        // Step 1: 计算需要删除的左/右括号数
        int rmL = 0, rmR = 0;
        for (char c : s) {
            if (c == '(') rmL++;
            else if (c == ')') {
                if (rmL > 0) rmL--;  // 与一个左括号配对
                else rmR++;          // 多余的右括号
            }
        }
        
        vector<string> res;
        dfs(s, 0, rmL, rmR, 0, res);
        return res;
    }
    
    void dfs(string& s, int idx, int rmL, int rmR, int open, vector<string>& res) {
        // 终止条件: 扫描完整个字符串
        if (idx == (int)s.size()) {
            if (rmL == 0 && rmR == 0 && open == 0) {
                res.push_back(s);
            }
            return;
        }
        
        char c = s[idx];
        int len = s.size();
        
        // 剪枝1: 剩余字符不够删
        // 剪枝2: 跳过连续相同括号（避免重复结果）
        
        if (c == '(' && rmL > 0) {
            // 选择: 删除这个左括号
            // 跳过连续的 '(' (去重)
            int k = idx;
            while (k < len && s[k] == '(') k++;
            // s[idx..k-1] 都是 '('，我们从 idx 开始删
            string tmp = s;
            s.erase(idx, 1);
            dfs(s, idx, rmL - 1, rmR, open, res);
            s = tmp; // 恢复
        }
        
        if (c == ')' && rmR > 0) {
            // 选择: 删除这个右括号
            string tmp = s;
            s.erase(idx, 1);
            dfs(s, idx, rmL, rmR - 1, open, res);
            s = tmp;
        }
        
        // 选择: 保留当前字符
        if (c == '(') {
            dfs(s, idx + 1, rmL, rmR, open + 1, res);
        } else if (c == ')') {
            if (open > 0) { // 只有 open > 0 时才能保留 ')'
                dfs(s, idx + 1, rmL, rmR, open - 1, res);
            }
        } else {
            // 字母: 直接保留
            dfs(s, idx + 1, rmL, rmR, open, res);
        }
    }
};
```

---

### 解法3: DFS 回溯（更简洁的写法）— 推荐面试写法

```cpp
class Solution {
public:
    vector<string> res;
    
    vector<string> removeInvalidParentheses(string s) {
        int rmL = 0, rmR = 0;
        for (char c : s) {
            if (c == '(') rmL++;
            else if (c == ')') {
                if (rmL > 0) rmL--;
                else rmR++;
            }
        }
        dfs(s, 0, rmL, rmR);
        return res;
    }
    
    void dfs(string s, int start, int rmL, int rmR) {
        if (rmL == 0 && rmR == 0) {
            if (isValid(s)) res.push_back(s);
            return;
        }
        
        for (int i = start; i < (int)s.size(); i++) {
            // 去重: 跳过连续相同的括号
            if (i > start && s[i] == s[i - 1]) continue;
            
            if (s[i] == '(' && rmL > 0) {
                // 删除 s[i] 这个左括号
                dfs(s.substr(0, i) + s.substr(i + 1), i, rmL - 1, rmR);
            }
            if (s[i] == ')' && rmR > 0) {
                // 删除 s[i] 这个右括号
                dfs(s.substr(0, i) + s.substr(i + 1), i, rmL, rmR - 1);
            }
        }
    }
    
    bool isValid(const string& s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') count--;
            if (count < 0) return false;
        }
        return count == 0;
    }
};
```

**关键去重技巧**:
```
s = "())" 中有两个连续的 ')'
如果分别删 index=1 和 index=2，得到的都是 "()"
所以遇到 s[i] == s[i-1] 时跳过，只在连续相同字符的第一个位置尝试删除

i > start (而不是 i > 0):
  start 参数确保我们只在 idx >= start 的位置删除，
  避免在之前的位置重复删除（子问题只需要看后面的字符）
```

## 解法对比

|  | BFS | DFS 回溯 |
|--|-----|---------|
| 时间 | O(n * 2^n) | O(2^n) |
| 空间 | O(2^n) set 去重 | O(n) 递归栈 |
| 去重方式 | set | 跳过连续相同字符 |
| 保证最少删除 | BFS 天然保证 | 预计算 rmL/rmR 保证 |
| 代码量 | 较多 | 较少 |
| 适用场景 | 直觉好，易理解 | **更高效，面试首选** |

## 易错点

1. **忘记处理字母**：
   - ✗ 对每个字符都尝试删除 → 删除字母是不必要的
   - ✓ 只对 `(` 和 `)` 尝试删除，字母直接跳过

2. **DFS 去重遗漏**：
   - ✗ 不跳过连续相同括号 → 产生重复结果如 `["()", "()"]`
   - ✓ `if (i > start && s[i] == s[i-1]) continue;`

3. **rmL/rmR 计算错误**：
   - ✗ 只统计左右括号总数之差 → 无法区分"需要删左"还是"需要删右"
   - ✓ 用配对消除法：`)` 遇到 rmL > 0 就配对（rmL--），否则 rmR++

4. **BFS 中忘记标记 found 后不扩展**：
   - ✗ 找到有效串后仍然向下一层扩展 → 得到非最少删除的结果
   - ✓ `if (found) continue;` 不再生成下一层候选

5. **DFS 中 open 变负**：
   - ✗ 保留 `)` 时不检查 open > 0 → open 变负，后续判断错误
   - ✓ 只有 `open > 0` 时才保留 `)` (即有可配对的左括号)

## 面试追问

**Q1: BFS 和 DFS 哪个更好？**
> DFS + 预计算 rmL/rmR 更好。BFS 需要 set 去重占用大量空间，且每层生成很多无效候选。DFS 有针对性地删除，搜索空间更小。

**Q2: 时间复杂度怎么分析？**
> 最坏情况 O(2^n)：每个括号字符都可以删或不删。但实际因为剪枝（rmL/rmR 限制、open 约束、跳过连续字符），远小于理论上界。n <= 25，2^20 ≈ 10^6，加上剪枝完全够。

**Q3: 如果只需要返回一个有效结果（不是所有），怎么优化？**
> DFS 找到第一个有效结果就返回。或者用贪心：从左到右扫描，遇到 open < 0 时删除最早的多余右括号；再从右到左处理多余左括号。O(n) 时间。

**Q4: 怎么理解 `start` 参数的作用？**
> `start` 确保搜索是**单调递增的**：只在 index >= start 的位置尝试删除。这避免了"先删 index=2 再删 index=1"和"先删 index=1 再删 index=2"产生相同结果的重复。每次递归只向右搜索。

## 相关题型

- **20. 有效的括号** — 括号匹配的基础题，用栈判断有效性。是本题 `isValid` 函数的来源。
- **22. 括号生成** — 给定 n 对括号，生成所有有效组合。同样用回溯 + open/close 计数。
- **1249. 移除无效的括号** — 简化版，只需要返回**一个**有效结果（不是所有），可以用栈 O(n) 解决。
- **32. 最长有效括号** — 找最长有效括号子串，用动态规划或栈。
- **678. 有效的括号字符串** — 含 `*` 通配符的括号匹配，贪心或区间 DP。
