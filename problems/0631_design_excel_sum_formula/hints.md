# 631. 设计 Excel 求和公式

## 核心思路

本题本质上是一个**依赖图（DAG）上的动态求值问题**：每个单元格要么存一个常量值，要么存一个"公式"（依赖其他单元格的求和）。当某个单元格被 `set` 修改时，所有直接或间接依赖它的单元格的值都要自动更新。

## 思维链

1. **读完题第一反应**：需要一个二维矩阵存值，`set/get` 很简单。难点在于 `sum` —— 它给某个格子绑定了一个"公式"，这个公式是"长期作用的"，意味着当被引用的格子变了，这个格子也要跟着变。

2. **最朴素想法**：每个格子如果有公式，就存下它依赖哪些格子。`get` 的时候递归地去求值 —— 如果该格子有公式就递归算公式里所有依赖格子的值之和；如果没公式就直接返回常量。`set` 只需要清掉公式并存常量。这样 `set` 是 O(1)，但 `get` 可能很慢（递归深度取决于依赖链）。

3. **瓶颈分析**：上面方案 `get` 每次都要递归重算，如果依赖链很长或很宽（一个格子依赖整个矩形区域），`get` 的开销就大。但题目约束很小（最多 26×26 的表，最多 100 次操作），所以**递归求值完全可以接受**。

4. **另一种思路**：也可以在 `set` 的时候就"传播"更新 —— 维护反向依赖图，当某格子被 set 时，沿着反向依赖找到所有需要更新的格子，重新计算它们的值。这更接近真实 Excel 的实现方式。

5. **关键设计决策**：
   - 每个格子存两样东西：一个 `int val`（当前值），一个"公式"（依赖列表）
   - `set(r, c, v)`: 清除该格子的公式，直接赋值
   - `sum(r, c, numbers)`: 解析 numbers 建立依赖关系，存为公式
   - `get(r, c)`: 如果有公式就递归求值，否则返回 val

6. **公式存储细节**：公式里同一个格子可能出现多次（如示例中 `["A1", "A1:B2"]`，A1 被引用了两次），所以用 `unordered_map<key, count>` 或直接存展开后的所有格子坐标列表。

## 解法概览

| 解法 | 思路 | set | get | sum | 空间 | 面试 |
|------|------|-----|-----|-----|------|------|
| 惰性求值（get时递归） | 存公式不存值，get递归算 | O(1) | O(依赖链) | O(range面积) | O(H×W + 公式) | ⭐ 必须写出 |
| 即时传播（set时更新） | 维护反向依赖图，set时DFS/拓扑更新 | O(受影响格子数) | O(1) | O(range面积) | O(H×W + 依赖图) | 加分项 |

## 关键提示

1. **公式是"活的"**：`sum(3,"C",["A1","A1:B2"])` 之后，如果你改了 A1 的值，C3 的值也要变。这意味着你必须存储"公式"本身，而不是只存一次计算结果。

2. **set 清除公式**：当对一个格子 `set` 时，该格子之前绑定的任何公式都要被清除。

3. **重复引用**：`["A1", "A1:B2"]` 中 A1 出现了两次（一次单独引用 + 一次在范围内），所以 A1 的值要被加两次。用 `map<坐标, 次数>` 来存储可以优雅处理。

4. **递归求值时注意**：如果 B 有公式依赖 A，C 有公式依赖 B，那 `get(C)` 需要递归到 B 再递归到 A。题目保证无环，所以不会死循环。

5. **ASCII 依赖图示例**：
```
示例操作后的依赖关系:
  sum(3,"C", ["A1", "A1:B2"])

  C3 依赖: A1(×2), B1(×1), A2(×1), B2(×1)
  (因为 "A1" 贡献一个 A1，"A1:B2" 贡献 A1,B1,A2,B2)

  当 set(2,"B",2) 时:
  B2 的值变为 2
  → get(C3) = A1×2 + B1×1 + A2×1 + B2×1
            = 2×2 + 0 + 0 + 2 = 6 ✓
```

## 解法详解

### 解法1: 惰性求值（get 时递归计算）— ⭐ 面试首选

**思考过程**：

最自然的想法：格子要么是"常量格子"（被 set 过），要么是"公式格子"（被 sum 过）。对于公式格子，我们不预算值，而是在 `get` 时递归地去算。这样 `set` 和 `sum` 都很轻量，复杂度集中在 `get` 上。

数据结构设计：
- `int mat[H][W]`: 存常量值
- `unordered_map<int, unordered_map<int, int>> formulas[H][W]` 或者更简单地，对每个格子存一个 `vector<pair<int,int>>` 或 `map<key, count>`

```
每个格子的状态:
┌─────────────────┐
│ 常量格子:        │   mat[r][c] = val, formula 为空
│ 公式格子:        │   formula = {(r1,c1):count1, (r2,c2):count2, ...}
└─────────────────┘

get(r,c):
  如果 formula 为空 → 返回 mat[r][c]
  否则 → Σ (get(依赖格子) × count)
```

```cpp
class Excel {
    int H, W;
    vector<vector<int>> mat;
    // formulas[r][c] = map<(row,col), count> 表示该格子依赖哪些格子，各出现几次
    vector<vector<unordered_map<int, int>>> formulas;
    
    int encode(int r, int c) { return r * 26 + c; }
    
    // 解析 "A1" → (row=1, col=0), 解析 "B3:F7" → 展开为所有格子
    void parseAndAdd(const string& s, unordered_map<int, int>& fm) {
        int pos = s.find(':');
        if (pos == string::npos) {
            // 单个格子 "A1"
            int c = s[0] - 'A';
            int r = stoi(s.substr(1));
            fm[encode(r, c)]++;
        } else {
            // 范围 "B3:F7"
            int c1 = s[0] - 'A';
            int r1 = stoi(s.substr(1, pos - 1));
            int c2 = s[pos + 1] - 'A';
            int r2 = stoi(s.substr(pos + 2));
            for (int r = r1; r <= r2; r++)
                for (int c = c1; c <= c2; c++)
                    fm[encode(r, c)]++;
        }
    }
    
public:
    Excel(int height, char width) {
        H = height;
        W = width - 'A' + 1;
        mat.assign(H + 1, vector<int>(W, 0));
        formulas.assign(H + 1, vector<unordered_map<int, int>>(W));
    }
    
    void set(int row, char column, int val) {
        int c = column - 'A';
        formulas[row][c].clear();  // 清除公式！
        mat[row][c] = val;
    }
    
    int get(int row, char column) {
        int c = column - 'A';
        if (formulas[row][c].empty()) return mat[row][c];
        int sum = 0;
        for (auto& [key, cnt] : formulas[row][c]) {
            int r2 = key / 26, c2 = key % 26;
            sum += get(r2, (char)('A' + c2)) * cnt;
        }
        return sum;
    }
    
    int sum(int row, char column, vector<string> numbers) {
        int c = column - 'A';
        formulas[row][c].clear();
        for (auto& s : numbers) {
            parseAndAdd(s, formulas[row][c]);
        }
        return get(row, column);
    }
};
```

**关键点**：
- `set` 必须清除公式，否则该格子仍被当作公式格子
- `get` 递归时，如果依赖格子本身也是公式格子，会继续递归 —— 题目保证无环所以不会死循环
- 同一个格子被引用多次要正确累加 count

### 解法2: 即时传播（set 时沿反向依赖更新）— 加分项

**从解法1优化**：解法1 的 `get` 可能要递归多层。如果 `get` 被频繁调用，可以改为：每个格子始终存最新值，`set` 时沿反向依赖图传播更新，`get` 直接返回 O(1)。

```
维护两种关系:
正向依赖: formulas[r][c] = 该格子依赖哪些格子 (用于计算值)
反向依赖: dependents[r][c] = 哪些格子依赖该格子 (用于传播更新)

set(r, c, val):
  1. 清除 (r,c) 的旧公式和旧反向依赖
  2. mat[r][c] = val
  3. 找到所有受影响的格子（拓扑排序），重新计算它们的值

get(r, c):
  直接返回 mat[r][c]  — O(1)!
```

```cpp
class Excel {
    int H, W;
    vector<vector<int>> mat;
    vector<vector<unordered_map<int, int>>> formulas;  // 正向：我依赖谁
    // 反向依赖不显式维护，而是在需要时遍历（因为规模很小）
    // 或者可以显式维护 set<int> dependents[H][W]
    
    int encode(int r, int c) { return r * 26 + c; }
    
    void parseAndAdd(const string& s, unordered_map<int, int>& fm) {
        int pos = s.find(':');
        if (pos == string::npos) {
            int c = s[0] - 'A', r = stoi(s.substr(1));
            fm[encode(r, c)]++;
        } else {
            int c1 = s[0] - 'A', r1 = stoi(s.substr(1, pos - 1));
            int c2 = s[pos + 1] - 'A', r2 = stoi(s.substr(pos + 2));
            for (int r = r1; r <= r2; r++)
                for (int c = c1; c <= c2; c++)
                    fm[encode(r, c)]++;
        }
    }
    
    // 重新计算格子 (r,c) 的值（假设它有公式）
    int recalc(int r, int c) {
        if (formulas[r][c].empty()) return mat[r][c];
        int s = 0;
        for (auto& [key, cnt] : formulas[r][c]) {
            s += mat[key / 26][key % 26] * cnt;
        }
        return mat[r][c] = s;
    }
    
    // 拓扑排序找到所有受影响的格子并按正确顺序更新
    void propagate() {
        // 对所有有公式的格子，按拓扑序重新计算
        // 由于规模最多 26×26=676 个格子，暴力多轮迭代也可以
        // 简单做法：迭代直到稳定
        for (int iter = 0; iter < H * W; iter++) {
            bool changed = false;
            for (int r = 1; r <= H; r++)
                for (int c = 0; c < W; c++)
                    if (!formulas[r][c].empty()) {
                        int newVal = 0;
                        for (auto& [key, cnt] : formulas[r][c])
                            newVal += mat[key / 26][key % 26] * cnt;
                        if (newVal != mat[r][c]) {
                            mat[r][c] = newVal;
                            changed = true;
                        }
                    }
            if (!changed) break;
        }
    }
    
public:
    Excel(int height, char width) {
        H = height; W = width - 'A' + 1;
        mat.assign(H + 1, vector<int>(W, 0));
        formulas.assign(H + 1, vector<unordered_map<int, int>>(W));
    }
    
    void set(int row, char column, int val) {
        int c = column - 'A';
        formulas[row][c].clear();
        mat[row][c] = val;
        propagate();  // 传播更新
    }
    
    int get(int row, char column) {
        return mat[row][column - 'A'];  // O(1)
    }
    
    int sum(int row, char column, vector<string> numbers) {
        int c = column - 'A';
        formulas[row][c].clear();
        for (auto& s : numbers) parseAndAdd(s, formulas[row][c]);
        propagate();
        return mat[row][c];
    }
};
```

**关键点**：`propagate` 的正确性依赖于"无环"的保证 —— DAG 上迭代一定会收敛。暴力多轮迭代虽然不优雅，但在这个规模下完全够用。更优雅的做法是维护拓扑序。

## 解法对比

| | 解法1: 惰性求值 | 解法2: 即时传播 |
|---|---|---|
| `set` | O(1) | O(受影响格子数) |
| `get` | O(依赖链总大小) | O(1) |
| `sum` | O(range面积) | O(range面积 + 传播) |
| 实现复杂度 | 简单 | 中等 |
| 适用场景 | set 多 get 少 | get 多 set 少 |
| 面试推荐 | ⭐ 首选（简洁清晰） | 追问时说即可 |

## 易错点

1. **✗ `set` 后忘记清除公式**：
   ```cpp
   void set(int row, char column, int val) {
       mat[row][column - 'A'] = val;  // 忘了清公式！
   }
   ```
   → **✓ 必须 `formulas[row][c].clear()`**，否则 `get` 时仍会按公式计算，忽略你 set 的值。

2. **✗ 解析范围字符串时假设列名只有一个字符但行号可能是两位数**：
   ```cpp
   // "B12" → col='B', row=12
   // 不能简单取 s[1] 作为 row，要用 stoi(s.substr(1))
   ```

3. **✗ 没有处理重复引用**：
   ```cpp
   // ["A1", "A1:B2"] 中 A1 出现两次
   // 如果用 set 存依赖，A1 只算一次，答案就错了
   // 必须用 map 记录次数，或者用 vector 允许重复
   ```

4. **✗ 行索引从1开始但数组从0开始**：题目 row 从 1 开始，你的数组如果从 0 开始，要注意映射关系。

## 面试追问

**Q1（基础理解）**：`sum` 的"长期作用"是什么意思？怎么实现？
> 答：意味着公式绑定后，被引用格子的值变化会自动影响结果。实现方式是存储公式（依赖关系），而不是只存一次计算结果。每次 `get` 时根据公式重新计算。

**Q2（优化）**：如果 `get` 被非常频繁调用，怎么优化？
> 答：改为"即时传播"模式 —— 每个格子始终缓存最新值，`set` 时沿反向依赖图传播更新所有受影响的格子。`get` 直接返回缓存值 O(1)。需要维护反向依赖图，更新时按拓扑序重算。

**Q3（变体）**：如果允许循环引用怎么办？
> 答：需要检测环。在 `sum` 时，从目标格子出发 DFS 看依赖链是否回到自身，如果检测到环则拒绝操作。也可以在建立依赖关系后做拓扑排序，如果存在环则报错。

**Q4（扩展）**：如果不只是 sum，还支持 average、max 等公式呢？
> 答：将公式抽象为一个通用结构，包含操作类型（SUM/AVG/MAX）和依赖列表。`get` 时根据操作类型选择不同的聚合函数即可。

## 相关题型

- **LeetCode 1274. 战舰数量** — 复用矩阵范围解析的思路，但更简单
- **LeetCode 399. 除法求值** — 同样是依赖图上的求值问题，399 用图的 BFS/DFS/并查集，本题的依赖图更简单（DAG + 求和）
- **拓扑排序系列（207, 210 课程表）** — 解法2 的传播更新本质上是拓扑排序，课程表系列练习拓扑排序的基本功