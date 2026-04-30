# 52. N 皇后 II

## 核心思路

本题本质上是：在 n×n 棋盘上逐行放置皇后，利用**回溯**穷举所有合法摆放方案，只需计数不需要返回具体方案。与第 51 题 N-Queens 相比，只需返回数量，因此可以省去构建棋盘字符串的开销，甚至可以用**位运算**极致优化冲突检测。

## 思维链

1. **读完题第一反应**：每行恰好放一个皇后（否则同行冲突），所以可以逐行决策——第 0 行放哪列、第 1 行放哪列……这是一个典型的**排列/组合搜索**问题。
2. **暴力想法**：对每行的 n 个列都尝试放置，放之前检查是否和已放的皇后冲突（同列、同对角线）。这就是经典回溯。
3. **冲突检测的瓶颈**：每次放置时，要遍历之前所有已放皇后检查冲突，时间 O(n)。能否 O(1) 判断冲突？
4. **用集合记录被占用的列和对角线**：
   - 列冲突：同一列 `col` 相同
   - 主对角线冲突：`row - col` 相同
   - 副对角线冲突：`row + col` 相同
   - 用三个 HashSet（或布尔数组）记录，每次 O(1) 判断。
5. **位运算极致优化**：把三个集合压缩成三个整数的 bit 表示，用位运算一次性算出所有可放位置，省去循环检查。
6. **对比第 51 题**：51 题要返回所有方案的棋盘布局，需要维护路径；52 题只要计数，回溯框架更简洁。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 逐个检查 | 逐行放置，放前遍历已放皇后检查冲突 | O(n! × n) | O(n) | 能说出即可 |
| 回溯 + 集合标记 | 用三个集合 O(1) 检测冲突 | O(n!) | O(n) | ⭐ 必须写出 |
| 回溯 + 位运算 | 三个整数位表示冲突，极致优化常数 | O(n!) | O(n) | 加分项 |

## 关键提示

1. **逐行放置**：每行恰好放一个皇后，这样天然避免行冲突，只需检查列和对角线。
2. **对角线编号技巧**：主对角线上所有格子 `row - col` 相同；副对角线上所有格子 `row + col` 相同。画个 4×4 的表验证一下：
   ```
   row-col:          row+col:
   0 -1 -2 -3       0  1  2  3
   1  0 -1 -2       1  2  3  4
   2  1  0 -1       2  3  4  5
   3  2  1  0       3  4  5  6
   ```
3. **回溯的"做选择-递归-撤销"三步**：放皇后 → 递归下一行 → 拿掉皇后（撤销标记）。
4. **位运算思路**：用一个整数的每一位表示一列是否被占。`cols | diag1 | diag2` 的所有为 1 的位就是不能放的位置，取反再与 `(1<<n)-1` 得到所有可放位置。
5. **与第 51 题的关系**：51 题的回溯框架完全一样，只是 51 题要记录路径（棋盘字符串），52 题只需一个计数器。

## 解法详解

### 解法1: 回溯 + 逐个检查 — O(n! × n) / O(n)

**思考过程**: 最朴素的想法——逐行放皇后，每次尝试某列时，遍历之前所有已放的皇后，检查是否同列或同对角线。能通过（n ≤ 9），但每次检查是 O(n)。

```cpp
class Solution {
public:
    int totalNQueens(int n) {
        int count = 0;
        vector<int> queens(n, -1); // queens[row] = col，记录每行皇后放在哪列
        backtrack(queens, 0, n, count);
        return count;
    }
    
    void backtrack(vector<int>& queens, int row, int n, int& count) {
        if (row == n) {
            count++; // 成功放完所有行
            return;
        }
        for (int col = 0; col < n; col++) {
            if (isValid(queens, row, col)) {
                queens[row] = col;        // 做选择
                backtrack(queens, row + 1, n, count); // 递归下一行
                queens[row] = -1;         // 撤销选择
            }
        }
    }
    
    bool isValid(vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; i++) {
            // 同列 or 同对角线（行差 == 列差）
            if (queens[i] == col || abs(queens[i] - col) == abs(i - row))
                return false;
        }
        return true;
    }
};
```

**关键点**: `isValid` 中用 `abs(queens[i] - col) == abs(i - row)` 同时检测了主对角线和副对角线。

---

### 解法2: 回溯 + 集合标记 — O(n!) / O(n) ⭐ 面试首选

**从解法1优化**: 解法1每次检查冲突要遍历之前所有行 O(n)，瓶颈在冲突检测。用三个集合分别记录被占用的**列、主对角线、副对角线**，将检测降为 O(1)。

```
// 决策树 (n=4):
//                       row=0
//           /      |        |       \
//       col=0   col=1    col=2    col=3
//       /  \      |  \      ...
//   col=2 col=3  col=3  ...
//     |     ✗     |
//   col=1       col=0       ← 冲突剪掉的分支用 ✗ 表示
//     ✗           |
//               col=2
//                 ✓  → count++
//
// n=4 最终找到 2 个解:
// 解1: [1,3,0,2]  即 .Q.. / ...Q / Q... / ..Q.
// 解2: [2,0,3,1]  即 ..Q. / Q... / ...Q / .Q..
```

```cpp
class Solution {
public:
    int totalNQueens(int n) {
        int count = 0;
        // 三个集合记录被占用的列、主对角线(row-col)、副对角线(row+col)
        unordered_set<int> cols, diag1, diag2;
        backtrack(0, n, cols, diag1, diag2, count);
        return count;
    }
    
    void backtrack(int row, int n,
                   unordered_set<int>& cols,
                   unordered_set<int>& diag1,
                   unordered_set<int>& diag2,
                   int& count) {
        if (row == n) {
            count++;
            return;
        }
        for (int col = 0; col < n; col++) {
            // O(1) 冲突检测：列、主对角线、副对角线是否被占用
            if (cols.count(col) || diag1.count(row - col) || diag2.count(row + col))
                continue;
            
            // 做选择：标记占用
            cols.insert(col);
            diag1.insert(row - col);
            diag2.insert(row + col);
            
            backtrack(row + 1, n, cols, diag1, diag2, count);
            
            // 撤销选择：取消占用
            cols.erase(col);
            diag1.erase(row - col);
            diag2.erase(row + col);
        }
    }
};
```

**关键点**: `row - col` 标识主对角线，`row + col` 标识副对角线。可以用 `vector<bool>` 替代 `unordered_set` 更快（注意 `row - col` 可能为负，要加偏移）。

---

### 解法3: 回溯 + 位运算 — O(n!) / O(n) 加分项

**从解法2优化**: 用三个整数的 bit 位替代集合，位运算一次性算出所有可用列，省去逐列检查的循环开销。

```
// 位运算核心思路 (以 n=4, row=2 为例):
//
// 假设 row0 放 col1, row1 放 col3:
//   cols  = 0b1010   (第1、3列被占)
//   diag1 = 0b0100   (主对角线传到当前行的影响，左移一位)
//   diag2 = 0b0001   (副对角线传到当前行的影响，右移一位)
//
//   occupied = cols | diag1 | diag2 = 0b1111 ← 哦不，假设没这么满
//   avail = ~occupied & ((1<<4)-1) ← 可用位
//   取最低位 1: pos = avail & (-avail) → 尝试该列
//   去掉该位: avail -= pos → 尝试下一列
```

```cpp
class Solution {
public:
    int totalNQueens(int n) {
        int count = 0;
        solve(n, 0, 0, 0, count);
        return count;
    }
    
    // cols: 哪些列被占 (bit=1表示被占)
    // diag1: 主对角线冲突传播 (每下一行左移一位)
    // diag2: 副对角线冲突传播 (每下一行右移一位)
    void solve(int n, int cols, int diag1, int diag2, int& count) {
        if (cols == (1 << n) - 1) {
            // 所有列都放了皇后，说明 n 个皇后全部放完
            count++;
            return;
        }
        
        // avail: 当前行可用的列 (bit=1 表示可放)
        int avail = ((1 << n) - 1) & ~(cols | diag1 | diag2);
        
        while (avail) {
            // 取最低位的 1，即选一个可用列
            int pos = avail & (-avail);
            avail -= pos; // 去掉这一位，下次循环尝试其他列
            
            // 递归下一行:
            // cols | pos: 标记该列被占
            // (diag1 | pos) << 1: 主对角线影响左移一位传给下一行
            // (diag2 | pos) >> 1: 副对角线影响右移一位传给下一行
            solve(n, cols | pos, (diag1 | pos) << 1, (diag2 | pos) >> 1, count);
        }
    }
};
```

**关键点**:
- `avail & (-avail)` 提取最低位的 1（lowbit 技巧）
- 主对角线影响**左移**传播，副对角线影响**右移**传播——画图理解：放在 (row, col) 的皇后，到 row+1 行时攻击 col-1（主对角线方向）和 col+1（副对角线方向）
- 终止条件 `cols == (1<<n)-1` 表示所有 n 列都被占了

## 解法对比

| | 解法1: 逐个检查 | 解法2: 集合标记 | 解法3: 位运算 |
|---|---|---|---|
| 冲突检测 | O(n) 遍历已放皇后 | O(1) 集合查找 | O(1) 位运算 |
| 常数因子 | 大 | 中（哈希开销） | 最小 |
| 代码复杂度 | 最简单 | 中等 | 需要理解位运算 |
| 面试推荐 | 入门展示理解 | ⭐ 首选 | 追问时加分 |
| 与51题差异 | 去掉路径记录 | 去掉路径记录 | 去掉路径记录 |

**与第 51 题的代码差异**：51 题需要 `vector<string> board` 记录棋盘状态，在 `row == n` 时把 board 加入结果集。52 题只需要 `count++`，不需要 board。解法2 中的回溯框架完全一样，只改了收集结果的那一行。

## 易错点

1. **对角线编号偏移**:
   - ✗ 用 `vector<bool> diag1(n)` —— `row - col` 可能为负数，越界！
   - ✓ 用 `vector<bool> diag1(2*n-1)`，索引用 `row - col + n - 1` 偏移

2. **位运算方向搞反**:
   - ✗ 主对角线右移、副对角线左移 —— 方向反了
   - ✓ 主对角线**左移**（下一行影响往左扩散），副对角线**右移**（下一行影响往右扩散）
   - 画图验证：皇后在 (0,2)，到 (1,?) 时攻击 col=1（左，主对角线）和 col=3（右，副对角线）

3. **位运算终止条件写错**:
   - ✗ `if (row == n)` —— 位运算版本没有传 row 参数
   - ✓ `if (cols == (1<<n)-1)` —— 所有列都被占满

4. **忘记撤销选择**（解法1、2）:
   - ✗ 只做 `cols.insert(col)` 没有对应的 `cols.erase(col)`
   - ✓ 解法3 不需要手动撤销，因为用参数传递，天然回溯

## 面试追问

**Q1: 这道题和第 51 题有什么区别？代码需要改哪里？**
→ 51 题要返回所有方案的棋盘布局，需要维护 `vector<string> board` 作为路径，在 `row==n` 时收集。52 题只要计数，不需要路径，代码更简洁。回溯框架完全一样，只改收集结果的部分。

**Q2: 时间复杂度为什么是 O(n!) 而不是 O(n^n)？**
→ 第 0 行有 n 种选择，第 1 行最多 n-1 种（至少一列被占），第 2 行最多 n-2 种……所以上界是 n!。实际因为对角线约束，比 n! 还小，但 n! 是紧上界分析。

**Q3: 能否用位运算进一步优化？（追问位运算解法的细节）**
→ 用三个 int 表示列、主对角线、副对角线的占用状态。`~(cols|diag1|diag2) & mask` 一步得出所有可用位置。`lowbit = x & (-x)` 逐个取出尝试。不需要 for 循环遍历所有列。

**Q4: n 很大（比如 n=15）时还能优化吗？**
→ 位运算解法已经是实际最快的回溯。理论上可以用对称性剪枝（第一行只搜一半列，结果×2，奇数 n 的中间列单独算）。但 n>15 后即使优化也很难快速计算，需要查表或更高级的数学方法。

## 相关题型

- **51. N-Queens** — 完全相同的回溯框架，区别：51 要返回所有方案（`vector<vector<string>>`），52 只返回计数。代码差异：51 多了 board 的构建和路径收集。
- **36. Valid Sudoku / 37. Sudoku Solver** — 类似的约束传播 + 回溯，区别：数独有行、列、3×3 宫格三种约束，可以复用"集合/位运算标记冲突"的技巧。
- **46. Permutations** — 复用"逐位选择 + 回溯"模板，N皇后相当于在排列基础上增加了对角线约束。
- **77. Combinations** — 回溯基础题，帮助理解回溯框架的"选择-递归-撤销"三步。