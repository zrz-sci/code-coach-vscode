# 51. N 皇后

## 核心思路

本质上是一个**逐行放置皇后的回溯搜索问题**：每行必须恰好放一个皇后，核心在于如何高效判断当前位置是否与已放置的皇后冲突（同列、同对角线）。

## 思维链

1. **读完题第一反应**：n×n 棋盘放 n 个皇后，互不攻击。最暴力的做法是在 n² 个格子中选 n 个，然后检查是否合法 → 组合数 C(n²,n) 太大，不可行。

2. **关键观察 — 每行恰好一个皇后**：因为有 n 个皇后和 n 行，且同行不能有两个皇后，所以**每行必须恰好放一个**。问题简化为：第 0 行放在哪列、第 1 行放在哪列……每行做一个选择。

3. **这就是回溯的经典结构**：逐行决策，每行有 n 个列可选。选了之后检查冲突，有冲突就剪枝，没冲突就继续下一行。到第 n 行说明放完了，记录答案。

4. **冲突检查是瓶颈**：每次放置需要检查同列、主对角线（↘）、副对角线（↙）是否已有皇后。暴力检查需要遍历之前所有行 → O(n)。

5. **用集合 O(1) 判断冲突**：维护三个集合 `cols`、`diag1`、`diag2`，分别记录已占用的列号、主对角线编号（row-col）、副对角线编号（row+col）。放置/撤销时更新集合。

6. **极致优化 — 位运算**：用整数的二进制位代替集合，列/主对角线/副对角线各用一个 int 表示，冲突判断和可用位置都是 O(1) 位运算。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 逐行暴力检查 | 每行尝试每列，O(n)检查冲突 | O(n! × n) | O(n²) | 能说出即可 |
| 回溯 + 集合优化 | 用三个集合 O(1)判冲突 | O(n!) | O(n²) | ⭐ 必须写出 |
| 回溯 + 位运算优化 | 用位运算代替集合 | O(n!) | O(n²) | 加分项 |

## 关键提示

- **提示1**：皇后互不攻击 + n 行 n 皇后 → 每行恰好一个。问题变成"为每行选一个列号"。

- **提示2**：冲突有三种 — 同列、主对角线（↘）、副对角线（↙）。同一主对角线上 `row - col` 相同，同一副对角线上 `row + col` 相同。

- **提示3（决策树可视化，n=4）**：
```
//                          row=0
//              /       |         |        \
//          col=0    col=1     col=2     col=3
//            Q...     .Q..     ..Q.     ...Q
//           /|\       /|\       /|\      /|\
//        row=1尝试  row=1尝试   ...       ...
//        col=1✗(对角) col=0✗(列) ...
//        col=2✓      col=2✗(对角)
//        col=3✗(对角) col=3✓
//        ...          ...
```

- **提示4**：回溯框架三要素 — ① 路径：已放置的皇后位置 ② 选择列表：当前行可选的列 ③ 结束条件：`row == n`。

- **提示5**：位运算优化的关键 — `available = ~(cols | diag1 | diag2) & ((1<<n)-1)` 得到所有可放位置，`pos & (-pos)` 取出最低位的 1。

## 解法详解

### 解法1: 回溯 + 逐行暴力检查 — O(n! × n) / O(n²)

**思考过程**：最直觉的做法。逐行放皇后，每次放之前遍历前面所有已放置的皇后检查是否冲突。

```cpp
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        // queens[i] = 第i行皇后所在的列号
        vector<int> queens(n, -1);
        backtrack(result, queens, n, 0);
        return result;
    }
    
private:
    void backtrack(vector<vector<string>>& result, vector<int>& queens, int n, int row) {
        if (row == n) {
            // 所有行都放完了，构造棋盘字符串
            result.push_back(buildBoard(queens, n));
            return;
        }
        for (int col = 0; col < n; col++) {
            if (isValid(queens, row, col)) {
                queens[row] = col;          // 做选择
                backtrack(result, queens, n, row + 1);
                queens[row] = -1;           // 撤销选择
            }
        }
    }
    
    // 暴力检查：遍历前面所有行，看是否同列或同对角线
    bool isValid(vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; i++) {
            // 同列 || 主对角线(行差==列差) || 副对角线
            if (queens[i] == col ||
                abs(queens[i] - col) == abs(i - row)) {
                return false;
            }
        }
        return true;
    }
    
    vector<string> buildBoard(vector<int>& queens, int n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
};
```

**关键点**：`isValid` 每次要遍历 0~row-1 行，是 O(n) 的。在深层递归中这会累积开销。

---

### 解法2: 回溯 + 集合优化 — O(n!) / O(n²) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是每次 `isValid` 要 O(n) 遍历。用三个哈希集合（或 bool 数组）记录已占用的列、主对角线、副对角线，冲突判断降为 O(1)。

```
// 对角线编号规律 (n=4):
//
// 主对角线 (row - col + n - 1):    副对角线 (row + col):
//   3  2  1  0                       0  1  2  3
//   4  3  2  1                       1  2  3  4
//   5  4  3  2                       2  3  4  5
//   6  5  4  3                       3  4  5  6
//
// 同一条 ↘ 方向线上 row-col 相同
// 同一条 ↙ 方向线上 row+col 相同
```

```cpp
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1);
        // 三个标记数组：列、主对角线(row-col偏移n-1)、副对角线(row+col)
        vector<bool> colUsed(n, false);
        vector<bool> diag1(2 * n - 1, false);  // 主对角线 ↘
        vector<bool> diag2(2 * n - 1, false);  // 副对角线 ↙
        backtrack(result, queens, n, 0, colUsed, diag1, diag2);
        return result;
    }
    
private:
    void backtrack(vector<vector<string>>& result, vector<int>& queens,
                   int n, int row,
                   vector<bool>& colUsed, vector<bool>& diag1, vector<bool>& diag2) {
        if (row == n) {
            result.push_back(buildBoard(queens, n));
            return;
        }
        for (int col = 0; col < n; col++) {
            int d1 = row - col + n - 1;  // 主对角线编号，+n-1 保证非负
            int d2 = row + col;           // 副对角线编号
            
            if (colUsed[col] || diag1[d1] || diag2[d2]) continue; // O(1) 冲突判断
            
            // 做选择
            queens[row] = col;
            colUsed[col] = diag1[d1] = diag2[d2] = true;
            
            backtrack(result, queens, n, row + 1, colUsed, diag1, diag2);
            
            // 撤销选择
            queens[row] = -1;
            colUsed[col] = diag1[d1] = diag2[d2] = false;
        }
    }
    
    vector<string> buildBoard(vector<int>& queens, int n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
};
```

**关键点**：主对角线编号 `row - col` 可能为负数，需要加偏移量 `n-1` 使其映射到 `[0, 2n-2]`。

---

### 解法3: 回溯 + 位运算优化 — O(n!) / O(n²)

**从解法2优化**：用三个整数的二进制位代替三个 bool 数组。不仅省空间（常数级），更关键的是可以通过位运算**直接算出所有可用位置**，无需逐列尝试。

```
// 位运算核心思路 (n=4, 当前处理 row=2):
// 
// 假设 row=0 放在 col=1, row=1 放在 col=3:
//   cols  = 0b1010    (col 1 和 col 3 被占)
//   diag1 = 0b?       (主对角线向右移一位)
//   diag2 = 0b?       (副对角线向左移一位)
// 
// available = ~(cols | diag1 | diag2) & 0b1111
// → 为1的位就是可以放皇后的列
// 
// pos = available & (-available)  → 取出最低位的1
// col = __builtin_ctz(pos)        → 该位对应的列号
```

```cpp
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1);
        backtrack(result, queens, n, 0, 0, 0, 0);
        return result;
    }
    
private:
    void backtrack(vector<vector<string>>& result, vector<int>& queens,
                   int n, int row, int cols, int diag1, int diag2) {
        if (row == n) {
            result.push_back(buildBoard(queens, n));
            return;
        }
        
        // 所有可用位置：不在任何攻击范围内的列
        int available = ~(cols | diag1 | diag2) & ((1 << n) - 1);
        
        while (available) {
            int pos = available & (-available);  // 取最低位的1
            int col = __builtin_ctz(pos);        // 该位是第几列
            
            queens[row] = col;
            // 关键：对角线传到下一行时要移位！
            // 主对角线(↘)：向右移1位；副对角线(↙)：向左移1位
            backtrack(result, queens, n, row + 1,
                      cols | pos,
                      (diag1 | pos) >> 1,
                      (diag2 | pos) << 1);
            queens[row] = -1;
            
            available &= (available - 1);  // 去掉最低位的1
        }
    }
    
    vector<string> buildBoard(vector<int>& queens, int n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
};
```

**关键点**：对角线的移位方向要理解 — 如果 row=0 在 col=1 放了皇后，到 row=1 时它的主对角线攻击位置是 col=2（右移），副对角线攻击位置是 col=0（左移）。

---

## 解法对比

| | 解法1: 暴力检查 | 解法2: 集合优化 | 解法3: 位运算 |
|---|---|---|---|
| 冲突判断 | O(n) 遍历已放皇后 | O(1) 查集合 | O(1) 位运算 |
| 枚举可选列 | 逐列尝试 O(n) | 逐列尝试 O(n) | 直接算出可用位 |
| 空间 | queens 数组 O(n) | +三个 bool 数组 O(n) | +三个 int O(1) |
| 代码难度 | 最简单 | 适中 | 需要理解位运算 |
| 面试推荐 | 开场讲 | ⭐ 主力解法 | 追问时展示 |

三种解法的搜索树完全相同（都是 n! 级别的回溯），区别仅在于每个节点的冲突判断开销。

## 易错点

1. **主对角线编号忘记加偏移量**
   - ✗ `diag1[row - col]` → 当 `row < col` 时索引为负，越界！
   - ✓ `diag1[row - col + n - 1]` → 偏移后范围是 `[0, 2n-2]`

2. **位运算解法中对角线移位方向搞反**
   - ✗ 主对角线左移、副对角线右移
   - ✓ 主对角线**右移**（↘ 方向往下一行走，攻击列号+1 → 高位方向，但我们从低位看是右移）、副对角线**左移**（↙ 方向）
   - 建议在纸上画 4×4 格子手动验证

3. **回溯时忘记撤销选择**
   - ✗ 只设 `colUsed[col] = true` 没有在递归后设回 `false`
   - ✓ 做选择 → 递归 → **必须**撤销选择

4. **构造棋盘字符串的时机**
   - ✗ 在回溯过程中直接修改一个全局 `board` 字符串数组（引用问题）
   - ✓ 用 `queens[i]` 记录列号，到终止条件时才构造字符串

## 面试追问

**Q1: 为什么逐行放置不会漏解？**
→ 因为 n 个皇后必须分布在 n 个不同行上（同行会互相攻击），所以每行恰好放一个。逐行决策穷举了所有合法排列。

**Q2: 时间复杂度为什么是 O(n!) 而不是 O(nⁿ)？**
→ 虽然每行有 n 列可选（看起来是 nⁿ），但列不能重复（同列冲突）。第 0 行 n 个选择，第 1 行最多 n-1 个，第 2 行最多 n-2 个……上界是 n!。加上对角线剪枝，实际远小于 n!。

**Q3: 如果只需要返回解的个数（LeetCode 52），代码怎么改？**
→ 把 `result` 换成计数器 `count++`，不需要构造棋盘字符串，也不需要 `queens` 数组（只需三个冲突标记）。位运算版本尤其适合，因为不需要知道具体列号。

**Q4: 能否用迭代代替递归实现？**
→ 可以。用一个栈或者直接用 `queens` 数组模拟回溯：维护当前行 row，尝试下一列；如果当前行所有列都不行，row-- 回退到上一行继续尝试下一列。本质是手动管理回溯栈。

## 相关题型

- **52. N 皇后 II** — 只要求返回解的个数。复用完全相同的回溯框架，区别：去掉棋盘构造，改为计数器。位运算解法最干净。
- **37. 解数独** — 同样是回溯 + 约束传播。复用"用集合/位运算标记已使用元素"的技巧，区别：数独是二维的，需要行/列/宫格三个约束。
- **46. 全排列** — N 皇后本质就是"列号的全排列 + 对角线约束"。复用回溯决策树框架。
- **77. 组合** — 回溯基础模板，理解选择/撤销选择的通用框架。