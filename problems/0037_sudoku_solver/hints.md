# 37. 解数独

## 核心思路

本题本质是一个**约束满足问题 (CSP)**：在 9×9 的网格中，每个空格从 1-9 中选一个数字填入，使得每行、每列、每个 3×3 宫格内数字不重复。这是经典的回溯搜索问题——逐个填空，填不通就退回来换一个数。

## 思维链

1. **读完题第一反应**：每个空格有 9 种选择，暴力穷举所有可能的填法，检查是否合法。但空格最多 81 个，$9^{81}$ 完全不可行。

2. **关键观察**：我们不需要穷举所有组合。每填一个空格，它的选择已经被同行、同列、同宫的已有数字大量限制了。所以我们可以**逐个空格尝试**，每次只尝试合法的数字。

3. **自然想到回溯**：对第一个空格尝试所有合法数字 → 填入后递归处理下一个空格 → 如果后续填不通（没有合法数字可选），就**撤销当前选择**，换下一个数字。

4. **瓶颈在哪？** 每次检查某个数字是否合法需要扫描整行、整列、整个宫格，都是 O(9) 的操作。虽然常数不大，但调用非常频繁。

5. **优化：预处理约束**。用三个布尔数组 `row[i][d]`、`col[j][d]`、`box[b][d]` 记录第 i 行 / 第 j 列 / 第 b 个宫格中数字 d 是否已被使用。这样判断合法性从 O(9) 降到 O(1)。

6. **进阶优化（位运算）**：用 9 位的整数代替布尔数组，通过位运算一次性得到某个空格的所有可选数字。还可以优先填"选择最少"的空格（MRV 启发式），大幅减少搜索分支。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯（每次扫描验证） | 逐格尝试，扫描行/列/宫判断合法性 | O(9^m)，m=空格数 | O(m) 递归栈 | 能说出即可 |
| 回溯 + 布尔数组 | 预维护三个约束数组，O(1) 判断合法 | O(9^m) 但常数小很多 | O(1) 额外（数组固定大小） | ⭐ 必须写出 |
| 回溯 + 位运算 + MRV | 位运算求可选集 + 优先填选择最少的格 | O(9^m) 但剪枝极强 | O(1) 额外 | 加分项 |

> 注：时间复杂度的理论上界都是指数级，但由于约束剪枝，实际运行非常快。

## 关键提示

1. **回溯的返回值**：和一般回溯（如组合、排列）不同，数独找到一个解就要**立即停止**。所以递归函数返回 `bool`，找到解返回 `true`，一路回溯时直接返回，不继续尝试其他分支。

2. **宫格编号公式**：第 (i, j) 个格子所属的 3×3 宫格编号是 `(i/3)*3 + j/3`，范围 [0,8]。这是一个非常重要的映射：
   ```
   宫格编号:
   0 0 0 | 1 1 1 | 2 2 2
   0 0 0 | 1 1 1 | 2 2 2
   0 0 0 | 1 1 1 | 2 2 2
   ------+-------+------
   3 3 3 | 4 4 4 | 5 5 5
   3 3 3 | 4 4 4 | 5 5 5
   3 3 3 | 4 4 4 | 5 5 5
   ------+-------+------
   6 6 6 | 7 7 7 | 8 8 8
   6 6 6 | 7 7 7 | 8 8 8
   6 6 6 | 7 7 7 | 8 8 8
   ```

3. **预处理已有数字**：在回溯之前，先遍历整个 board，把已有数字的约束记录到 `row`/`col`/`box` 数组中。

4. **位运算优化思路**：`row[i]` 是一个 9 位整数，第 d 位为 1 表示数字 d+1 已在第 i 行中。那么 `row[i] | col[j] | box[b]` 的取反就是当前格子的所有可选数字。用 `x & (-x)` 取最低位的 1 来逐个枚举。

5. **MRV (Minimum Remaining Values)**：每次不要按顺序填下一个空格，而是找"可选数字最少"的那个空格先填。这样尽早触发冲突，大幅减少搜索树的大小。

## 解法详解

### 解法1: 基础回溯（逐格扫描验证） — O(9^m) / O(m)

**思考过程**：最直接的方式——遍历 board 找到一个空格，尝试填入 1-9，每次用一个 `isValid` 函数扫描该行/列/宫格检查是否冲突。填入后递归处理下一个空格，如果递归返回 false，就撤销。

```cpp
class Solution {
public:
    void solveSudoku(vector<vector<char>>& board) {
        solve(board);
    }
    
    bool solve(vector<vector<char>>& board) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') continue;
                // 尝试填入 '1' 到 '9'
                for (char c = '1'; c <= '9'; c++) {
                    if (isValid(board, i, j, c)) {
                        board[i][j] = c;         // 做选择
                        if (solve(board)) return true;  // 递归
                        board[i][j] = '.';        // 撤销选择
                    }
                }
                // 1-9 都不行，说明前面的选择有问题，需要回溯
                return false;
            }
        }
        // 所有格子都填满了，找到解
        return true;
    }
    
    bool isValid(vector<vector<char>>& board, int row, int col, char c) {
        for (int i = 0; i < 9; i++) {
            if (board[row][i] == c) return false;  // 检查行
            if (board[i][col] == c) return false;  // 检查列
            // 检查 3x3 宫格
            int bi = (row / 3) * 3 + i / 3;
            int bj = (col / 3) * 3 + i % 3;
            if (board[bi][bj] == c) return false;
        }
        return true;
    }
};
```

**关键点**：
- `return false` 在尝试完 '1'-'9' 都不行时触发——这不是 bug，这是回溯的核心：告诉上一层"此路不通"。
- 双重 for 循环每次从头扫描找空格看起来低效，但因为填过的格子不再是 '.'，实际只在"当前空格"处停留。
- `isValid` 每次都扫描 27 个格子，调用频率高时有优化空间。

### 解法2: 回溯 + 布尔约束数组 — O(9^m) / O(1) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是每次调用 `isValid` 都要扫描行/列/宫格。用三个预计算的布尔数组，将合法性检查从 O(9) 降到 O(1)。

```
决策树（以某一行的 3 个空格为例）:

                    []
            /       |       \
     格1填1      格1填4    格1填7   ← 只尝试合法数字
       / \          |
  格2填3 格2填6  格2填2
    |       |       |
  格3填8  格3填5  格3填... 
    ✓       ✓     ← 找到解就立即返回 true，不再继续
```

```cpp
class Solution {
public:
    // row[i][d] = true 表示第 i 行已有数字 d+1
    bool rowUsed[9][9];
    bool colUsed[9][9];
    bool boxUsed[9][9];
    
    void solveSudoku(vector<vector<char>>& board) {
        memset(rowUsed, false, sizeof(rowUsed));
        memset(colUsed, false, sizeof(colUsed));
        memset(boxUsed, false, sizeof(boxUsed));
        
        // 预处理：记录已有数字的约束
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    int d = board[i][j] - '1';  // 数字映射到 0-8
                    int b = (i / 3) * 3 + j / 3;
                    rowUsed[i][d] = colUsed[j][d] = boxUsed[b][d] = true;
                }
            }
        }
        solve(board, 0, 0);
    }
    
    // 从 (r, c) 开始逐格填写
    bool solve(vector<vector<char>>& board, int r, int c) {
        // 找到下一个空格
        while (r < 9 && board[r][c] != '.') {
            c++;
            if (c == 9) { c = 0; r++; }
        }
        if (r == 9) return true;  // 所有格子填完
        
        int b = (r / 3) * 3 + c / 3;
        for (int d = 0; d < 9; d++) {
            // O(1) 合法性检查
            if (rowUsed[r][d] || colUsed[c][d] || boxUsed[b][d]) continue;
            
            // 做选择
            board[r][c] = '1' + d;
            rowUsed[r][d] = colUsed[c][d] = boxUsed[b][d] = true;
            
            // 递归：从当前位置的下一格开始（避免每次从头扫描）
            int nc = c + 1, nr = r;
            if (nc == 9) { nc = 0; nr++; }
            if (solve(board, nr, nc)) return true;
            
            // 撤销选择
            board[r][c] = '.';
            rowUsed[r][d] = colUsed[c][d] = boxUsed[b][d] = false;
        }
        return false;
    }
};
```

**关键点**：
- 传入 `(r, c)` 参数避免每次从 (0,0) 开始扫描空格，减少无谓遍历。
- 数字用 0-8 索引而非 1-9，与数组下标对齐，避免 off-by-one。
- 三个数组总共 9×9×3 = 243 个 bool，固定大小，空间 O(1)。

### 解法3: 回溯 + 位运算 + MRV 启发式 — 极致优化 加分项

**从解法2优化**：
1. 用 9 位整数代替布尔数组，位运算一次性求出所有可选数字。
2. 每次选"可选数字最少"的空格先填（MRV 启发式），尽早剪枝。

```
位运算示例：
  row[0] = 0b101100110  表示第0行已有数字 {2,3,6,7,9}
  col[2] = 0b010001100  表示第2列已有数字 {3,4,8}
  box[0] = 0b100100010  表示宫格0已有数字 {2,6,9}

  已用 = row | col | box = 0b111101110
  可用 = ~已用 & 0x1FF = 0b000010001  → 可选数字 {1, 5}
  
  用 x & (-x) 取最低位1 → 得到数字1
  用 x & (x-1) 去掉最低位1 → 剩下数字5
```

```cpp
class Solution {
public:
    int row[9], col[9], box[9];
    
    void solveSudoku(vector<vector<char>>& board) {
        memset(row, 0, sizeof(row));
        memset(col, 0, sizeof(col));
        memset(box, 0, sizeof(box));
        
        vector<pair<int,int>> blanks;  // 收集所有空格
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == '.') {
                    blanks.push_back({i, j});
                } else {
                    int mask = 1 << (board[i][j] - '1');
                    row[i] |= mask;
                    col[j] |= mask;
                    box[(i/3)*3 + j/3] |= mask;
                }
            }
        }
        solve(board, blanks);
    }
    
    bool solve(vector<vector<char>>& board, vector<pair<int,int>>& blanks) {
        if (blanks.empty()) return true;
        
        // MRV: 找可选数字最少的空格
        int bestIdx = 0, minChoices = 10;
        for (int k = 0; k < blanks.size(); k++) {
            auto [i, j] = blanks[k];
            int used = row[i] | col[j] | box[(i/3)*3 + j/3];
            int available = (~used) & 0x1FF;
            int cnt = __builtin_popcount(available);
            if (cnt < minChoices) {
                minChoices = cnt;
                bestIdx = k;
                if (cnt == 0) return false;  // 提前剪枝：无解
                if (cnt == 1) break;         // 只有一个选择，必须填它
            }
        }
        
        auto [r, c] = blanks[bestIdx];
        int b = (r / 3) * 3 + c / 3;
        int available = (~(row[r] | col[c] | box[b])) & 0x1FF;
        
        // 将选中的空格从列表移除
        swap(blanks[bestIdx], blanks.back());
        blanks.pop_back();
        
        // 枚举所有可选数字（用位运算逐个提取）
        while (available) {
            int pick = available & (-available);  // 最低位的 1
            int d = __builtin_ctz(pick);          // 对应数字 d+1
            
            board[r][c] = '1' + d;
            row[r] |= pick;
            col[c] |= pick;
            box[b] |= pick;
            
            if (solve(board, blanks)) return true;
            
            // 撤销
            board[r][c] = '.';
            row[r] ^= pick;
            col[c] ^= pick;
            box[b] ^= pick;
            
            available &= (available - 1);  // 去掉最低位的 1
        }
        
        // 恢复 blanks
        blanks.push_back({r, c});
        swap(blanks[blanks.size() - 1], blanks[blanks.size() - 1]);  // 不需要特别恢复顺序
        return false;
    }
};
```

**关键点**：
- MRV 启发式的威力：如果某个空格只剩 1 个选择，直接填入不需要分支；如果某个空格 0 个选择，立即返回 false 剪枝。
- `swap + pop_back` 是 O(1) 删除列表元素的技巧（不关心顺序时）。
- 位运算 `x & (-x)` 提取最低位1，`__builtin_ctz` 获取尾部零的个数（即该位的位置）。

## 解法对比

| | 解法1: 基础回溯 | 解法2: 布尔数组 | 解法3: 位运算+MRV |
|---|---|---|---|
| 合法性检查 | O(9) 扫描 | O(1) 查表 | O(1) 位运算 |
| 搜索顺序 | 固定从左到右、从上到下 | 同左 | 动态选最受约束的格 |
| 代码复杂度 | 最简单 | 中等 | 较复杂 |
| 实际速度 | 慢 | 快 | 极快 |
| 面试适用性 | 能通过但不推荐 | ⭐ 首选 | 加分展示 |

## 易错点

1. **回溯返回值忘记处理**：
   - ✗ `solve(board, nr, nc);` 忽略返回值，导致找到解后不停止，继续尝试其他选择，最终把已解的 board 又恢复成了 `.`
   - ✓ `if (solve(board, nr, nc)) return true;` 找到解立即传播 true

2. **空格用完后的终止条件遗漏**：
   - ✗ 没有 `if (r == 9) return true;`，导致越界访问
   - ✓ 遍历到 r==9 说明所有格子都填完了，返回 true

3. **宫格编号算错**：
   - ✗ `box = (i / 3) + (j / 3)` → 最多只有 0-5 这 6 种值，不同宫格编号冲突
   - ✓ `box = (i / 3) * 3 + j / 3` → 产生 0-8 的 9 种编号

4. **预处理时忘记初始化约束数组**：
   - ✗ 直接开始回溯，已有数字的约束没有记录，导致填入与已有数字冲突的值
   - ✓ 回溯之前先遍历 board，把所有非 '.' 的数字标记到约束数组中

5. **位运算版本中 `0x1FF` 掩码遗漏**：
   - ✗ `int available = ~used;` → 高位全是 1，`popcount` 计算错误
   - ✓ `int available = (~used) & 0x1FF;` → 只保留低 9 位

## 面试追问

**Q1: 暴力回溯能过吗？瓶颈在哪？**
> 能过，因为数独的约束非常强，实际搜索树不大。瓶颈在 `isValid` 每次扫描 27 个格子，调用次数极多时影响常数。改用预计算的布尔数组可以将每次检查从 O(9) 降到 O(1)。

**Q2: 如何进一步加速搜索？**
> 两个方向：(1) 用位运算代替布尔数组，一条 OR 指令就能得到约束的并集；(2) 用 MRV 启发式，优先填"可选数字最少"的格子。这是 CSP 求解中的经典策略，能大幅减少搜索树的分支因子。

**Q3: 如果要验证数独是否有唯一解怎么做？**
> 修改回溯函数：找到第一个解后不立即返回，继续搜索。如果找到第二个解就返回 "多解"。全部搜索完只找到一个解就返回 "唯一解"。具体做法是用一个计数器，`count++` 当 count > 1 时剪枝返回。

**Q4: 数独生成器怎么做？**
> 先在空棋盘上随机填入若干数（保证合法），用 solver 解出一个完整解。然后逐个"挖洞"（移除数字），每次挖洞后检查是否仍有唯一解。如果不是唯一解就恢复该数字。

## 相关题型

- **36. 有效的数独** — 本题的前置题，只需判断当前 board 是否合法，不需要填数。复用完全相同的 `row/col/box` 约束数组结构，但不需要回溯。
- **51. N 皇后** — 同样的回溯框架，区别：数独是在 9×9 格子中填数字，N 皇后是在 N×N 棋盘上放皇后。约束从行/列/宫格变成行/列/对角线。约束数组的设计思路完全一致。
- **52. N 皇后 II** — N 皇后的计数版本，回溯框架一样，只是把收集结果改成计数器。
- **79. 单词搜索** — 二维网格上的回溯搜索，区别：搜索方向是上下左右相邻格子而非固定位置，需要 visited 数组防止重复访问。