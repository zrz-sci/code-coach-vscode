/*
 * LeetCode 631: 设计 Excel 求和公式
 *
 * 【题目本质】
 * 设计一个简化版 Excel，支持 set（赋值）、get（取值）、sum（求和公式）。
 * 核心难点：sum 绑定的公式是"活的"，被引用格子变化时，公式格子的值也要跟着变。
 * 本质是 DAG（有向无环图）上的依赖求值问题。
 *
 * 【解法总览】
 * 解法1: 惰性求值 — set O(1), get O(依赖链), sum O(range面积) — 面试首选 ⭐
 * 解法2: 即时传播 — set O(受影响格子), get O(1), sum O(range面积+传播) — 加分项
 *
 * 依赖关系示意:
 *   set(1,"A",2)  →  A1 = 2 (常量格子)
 *   sum(3,"C",["A1","A1:B2"])  →  C3 = f(A1,B1,A2,B2)
 *
 *   C3 的公式: {A1:2, B1:1, A2:1, B2:1}
 *   (A1 出现两次：一次来自 "A1"，一次来自 "A1:B2")
 *
 *   Excel 表状态:
 *     A B C
 *   1 2 0 0
 *   2 0 0 0        set(2,"B",2) 后:
 *   3 0 0 4        → C3 = 2×2+0+0+2 = 6
 *                      ↑A1×2  ↑B1 ↑A2 ↑B2
 */

// ============================================================
// 解法1: 惰性求值 — get 时递归计算
// set: O(1)  get: O(依赖链总大小)  sum: O(range面积)
//
// 【思路】
// 最自然的想法：格子分两类 —— 常量格子和公式格子。
// 常量格子直接存值，公式格子存"依赖哪些格子+各出现几次"。
// get 时：如果是常量格子直接返回值；如果是公式格子，递归地
// 对所有依赖格子求 get 并乘以出现次数，求和即可。
//
// 为什么这样做够用？
// - 题目规模很小：最多 26×26=676 个格子，最多 100 次操作
// - 递归深度不会很深（依赖链有限）
// - 实现最简洁，面试中容易写对
//
// 关键：set 必须清除公式！否则 get 仍按旧公式计算。
// ============================================================
class Excel {
    int H, W;
    vector<vector<int>> mat;
    // formulas[r][c] 存储格子 (r,c) 的依赖关系
    // key = encode(row, col), value = 该格子在公式中出现的次数
    vector<vector<unordered_map<int, int>>> formulas;

    // 将 (row, col) 编码为唯一整数，方便做 map 的 key
    int encode(int r, int c) { return r * 26 + c; }

    // 解析一个 numbers 元素，可能是 "A1"（单格）或 "B3:F7"（范围）
    // 将解析出的所有格子加入 fm，注意同一格子可能多次出现，用 count 累加
    void parseAndAdd(const string& s, unordered_map<int, int>& fm) {
        size_t pos = s.find(':');
        if (pos == string::npos) {
            // 单个格子，如 "F7" → col='F'-'A'=5, row=7
            int c = s[0] - 'A';
            int r = stoi(s.substr(1));
            fm[encode(r, c)]++;
        } else {
            // 范围，如 "B3:F7"
            int c1 = s[0] - 'A';
            int r1 = stoi(s.substr(1, pos - 1));
            int c2 = s[pos + 1] - 'A';
            int r2 = stoi(s.substr(pos + 2));
            // 展开矩形范围内的每一个格子
            for (int r = r1; r <= r2; r++)
                for (int c = c1; c <= c2; c++)
                    fm[encode(r, c)]++;
        }
    }

public:
    Excel(int height, char width) {
        H = height;
        W = width - 'A' + 1;
        // 行索引从 1 到 H，所以分配 H+1 行
        mat.assign(H + 1, vector<int>(W, 0));
        formulas.assign(H + 1, vector<unordered_map<int, int>>(W));
    }

    void set(int row, char column, int val) {
        int c = column - 'A';
        // 关键：清除公式！否则 get 时仍按公式计算
        formulas[row][c].clear();
        mat[row][c] = val;
    }

    int get(int row, char column) {
        int c = column - 'A';
        // 如果没有公式，直接返回常量值
        if (formulas[row][c].empty()) {
            return mat[row][c];
        }
        // 有公式：递归计算所有依赖格子的值之和
        // 注意 cnt 表示该格子在公式中出现的次数
        int total = 0;
        for (auto& [key, cnt] : formulas[row][c]) {
            int depRow = key / 26;
            int depCol = key % 26;
            // 递归 get —— 如果依赖格子本身也有公式，会继续递归
            // 题目保证无环，所以一定会终止
            total += get(depRow, (char)('A' + depCol)) * cnt;
        }
        return total;
    }

    int sum(int row, char column, vector<string> numbers) {
        int c = column - 'A';
        // 清除旧公式，建立新公式
        formulas[row][c].clear();
        for (const string& s : numbers) {
            parseAndAdd(s, formulas[row][c]);
        }
        // 立即返回当前计算结果
        return get(row, column);
    }
};

// ============================================================
// 解法2: 即时传播 — set 时更新所有受影响的格子，get O(1)
// set: O(受影响格子数)  get: O(1)  sum: O(range面积 + 传播)
//
// 【思路】
// 解法1 的瓶颈：每次 get 都要递归。如果 get 被频繁调用，
// 可以反过来：让每个格子始终缓存最新值。
// set/sum 操作后，沿着依赖图传播更新所有受影响的格子。
//
// 传播策略：由于规模极小（最多676格子），我们可以暴力地
// 对所有有公式的格子反复重算，直到没有值变化为止。
// 更优雅的做法是维护拓扑序，按序更新一遍即可。
//
// 这里实现"迭代直到稳定"的简单版本。
// ============================================================
class Excel2 {
    int H, W;
    vector<vector<int>> mat;
    vector<vector<unordered_map<int, int>>> formulas;

    int encode(int r, int c) { return r * 26 + c; }

    void parseAndAdd(const string& s, unordered_map<int, int>& fm) {
        size_t pos = s.find(':');
        if (pos == string::npos) {
            int c = s[0] - 'A';
            int r = stoi(s.substr(1));
            fm[encode(r, c)]++;
        } else {
            int c1 = s[0] - 'A', r1 = stoi(s.substr(1, pos - 1));
            int c2 = s[pos + 1] - 'A', r2 = stoi(s.substr(pos + 2));
            for (int r = r1; r <= r2; r++)
                for (int c = c1; c <= c2; c++)
                    fm[encode(r, c)]++;
        }
    }

    // 对所有有公式的格子重新计算值，迭代直到稳定
    // 为什么能收敛？因为题目保证无环（DAG），
    // 最多迭代"最长依赖链"次就能稳定
    void propagate() {
        for (int iter = 0; iter < H * W; iter++) {
            bool changed = false;
            for (int r = 1; r <= H; r++) {
                for (int c = 0; c < W; c++) {
                    if (formulas[r][c].empty()) continue;
                    int newVal = 0;
                    for (auto& [key, cnt] : formulas[r][c]) {
                        newVal += mat[key / 26][key % 26] * cnt;
                    }
                    if (newVal != mat[r][c]) {
                        mat[r][c] = newVal;
                        changed = true;
                    }
                }
            }
            if (!changed) break;  // 所有值都没变，已稳定
        }
    }

public:
    Excel2(int height, char width) {
        H = height;
        W = width - 'A' + 1;
        mat.assign(H + 1, vector<int>(W, 0));
        formulas.assign(H + 1, vector<unordered_map<int, int>>(W));
    }

    void set(int row, char column, int val) {
        int c = column - 'A';
        formulas[row][c].clear();
        mat[row][c] = val;
        propagate();  // 传播更新所有依赖此格子的公式格子
    }

    int get(int row, char column) {
        // O(1)! 因为 mat 始终缓存最新值
        return mat[row][column - 'A'];
    }

    int sum(int row, char column, vector<string> numbers) {
        int c = column - 'A';
        formulas[row][c].clear();
        for (const string& s : numbers) {
            parseAndAdd(s, formulas[row][c]);
        }
        propagate();
        return mat[row][c];
    }
};

/*
 * ============================================================
 * 【解法对比】
 *
 * | 操作 | 解法1(惰性求值)   | 解法2(即时传播)       |
 * |------|-------------------|-----------------------|
 * | set  | O(1)              | O(受影响格子数)        |
 * | get  | O(依赖链总大小)    | O(1)                  |
 * | sum  | O(range面积)      | O(range面积 + 传播)    |
 *
 * 解法1 适合 set 多 get 少的场景（面试首选，代码简洁）。
 * 解法2 适合 get 多 set 少的场景（更接近真实 Excel）。
 * 在本题约束下（最多100次操作，26×26表），两者性能无差别。
 *
 * ============================================================
 * 【易错点】
 *
 * 1. set 后忘清公式:
 *    ✗ mat[row][c] = val;  // 公式还在，get 时仍按公式算
 *    ✓ formulas[row][c].clear(); mat[row][c] = val;
 *
 * 2. 重复引用没累加:
 *    ✗ fm[encode(r,c)] = 1;  // 每次赋值覆盖前面的
 *    ✓ fm[encode(r,c)]++;    // 同一格子出现多次要累加
 *    示例: ["A1", "A1:B2"] 中 A1 出现2次，值要加两遍
 *
 * 3. 行号解析错误:
 *    ✗ int r = s[1] - '0';   // 行号可能是两位数如 "A12"
 *    ✓ int r = stoi(s.substr(1));
 *
 * 4. 编码冲突:
 *    encode(r, c) = r * 26 + c
 *    需确保 r 的范围不超过使 r*26+c 唯一的范围
 *    r ∈ [1,26], c ∈ [0,25]，所以 r*26+c ∈ [26, 701]，无冲突 ✓
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: "长期作用"怎么实现？
 *   → 存储公式（依赖关系），不是存一次计算结果。
 *     get 时根据公式重新计算（解法1），或 set 时传播更新（解法2）。
 *
 * Q2: get 被频繁调用怎么优化？
 *   → 解法2：维护缓存值，set 时传播更新。get 直接返回 O(1)。
 *     传播可以用拓扑排序优化，确保每个格子只重算一次。
 *
 * Q3: 如果允许循环引用？
 *   → sum 时 DFS 检测依赖链是否有环，有环则拒绝操作。
 *     或者建图后跑拓扑排序，有环则报错。
 *
 * Q4: 如果支持 average/max 等其他公式？
 *   → 抽象公式为 {type: SUM/AVG/MAX, deps: [...]}
 *     get 时根据 type 选择不同聚合函数。
 *     对于 AVG 还需要存依赖总数（包括重复）。
 * ============================================================
 */
