/*
 * LeetCode 970: Powerful Integers
 *
 * [题目本质]
 * 给定 x, y, bound，找出所有 x^i + y^j <= bound 的不重复值。
 * 由于 bound <= 10^6，底数 >= 2 时指数最多 ~20，直接枚举所有 (i,j) 组合即可。
 *
 * [解法总览]
 * 解法1: 直接双层枚举 + set 去重   — O(log^2(bound)) / O(结果集)  — 面试首选 ⭐
 * 解法2: 预计算幂次数组 + 组合      — O(log^2(bound)) / O(结果集)  — 逻辑更清晰
 * 解法3: bool 标记数组去重          — O(log^2(bound)) / O(bound)   — 常数最小
 */

// ============================================================
// 解法1: 直接双层枚举 + set 去重
// 时间: O(log_x(bound) * log_y(bound))  空间: O(结果集大小)
//
// [思路]
// x^i 的增长速度：当 x >= 2 时，x^i 以指数增长，最多 log_x(bound) 步就超过 bound。
// 当 x = 1 时，x^i 永远是 1，需要特殊处理（只取 i = 0 即可，因为 1^0 = 1^1 = ... = 1）。
//
// 外层循环枚举 pow_x = x^0, x^1, x^2, ...
// 内层循环枚举 pow_y = y^0, y^1, y^2, ...
// 当 pow_x + pow_y <= bound 时，将其加入 set。
//
// 枚举过程 (x=2, y=3, bound=10):
//   pow_x=1: pow_y=1→2, pow_y=3→4, pow_y=9→10  (3 values)
//   pow_x=2: pow_y=1→3, pow_y=3→5, pow_y=9→11>10 stop  (2 values)
//   pow_x=4: pow_y=1→5(dup), pow_y=3→7, pow_y=9→13>10 stop  (1 new)
//   pow_x=8: pow_y=1→9, pow_y=3→11>10 stop  (1 value)
//   pow_x=16 > 10: stop
//   Result: {2, 3, 4, 5, 7, 9, 10}
// ============================================================
class Solution1 {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        unordered_set<int> result;

        // 枚举所有 x^i，当 x = 1 时只取一次
        for (long long pow_x = 1; pow_x <= bound; pow_x *= x) {
            // 枚举所有 y^j，当 y = 1 时只取一次
            for (long long pow_y = 1; pow_x + pow_y <= bound; pow_y *= y) {
                result.insert((int)(pow_x + pow_y));
                // x = 1 时 1^j 永远为 1，只需算一次
                if (y == 1) break;
            }
            // y = 1 时 1^i 永远为 1，只需算一次
            if (x == 1) break;
        }

        return vector<int>(result.begin(), result.end());
    }
};

// ============================================================
// 解法2: 预计算幂次数组 + 组合
// 时间: O(log_x(bound) * log_y(bound))  空间: O(结果集 + log(bound))
//
// [思路]
// 将枚举分为两步：
// 1. 预计算所有 x^i <= bound 的值存入 powersX 数组
// 2. 预计算所有 y^j <= bound 的值存入 powersY 数组
// 3. 双层遍历两个数组，将和 <= bound 的值插入 set
//
// 好处：逻辑更清晰，避免在双层循环中处理 break 条件。
//
// 预计算示例 (x=2, bound=10):
//   powersX = [1, 2, 4, 8]   (2^0=1, 2^1=2, 2^2=4, 2^3=8, 2^4=16>10)
//
// 预计算示例 (y=3, bound=10):
//   powersY = [1, 3, 9]       (3^0=1, 3^1=3, 3^2=9, 3^3=27>10)
//
// 组合求和:
//   1+1=2, 1+3=4, 1+9=10
//   2+1=3, 2+3=5, 2+9=11>10
//   4+1=5(dup), 4+3=7, 4+9=13>10
//   8+1=9, 8+3=11>10
//   → {2, 3, 4, 5, 7, 9, 10}
// ============================================================
class Solution2 {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        // Step 1: 预计算 x 的所有幂次
        vector<long long> powersX;
        for (long long val = 1; val <= bound; val *= x) {
            powersX.push_back(val);
            if (x == 1) break; // 1^anything = 1
        }

        // Step 2: 预计算 y 的所有幂次
        vector<long long> powersY;
        for (long long val = 1; val <= bound; val *= y) {
            powersY.push_back(val);
            if (y == 1) break; // 1^anything = 1
        }

        // Step 3: 组合求和，set 去重
        unordered_set<int> result;
        for (long long px : powersX) {
            for (long long py : powersY) {
                long long sum = px + py;
                if (sum <= bound) {
                    result.insert((int)sum);
                }
                // 如果 py 已经太大，后面只会更大（powersY 递增）
                // 但由于 powersY 很短（最多 ~20），不加这个优化也 OK
            }
        }

        return vector<int>(result.begin(), result.end());
    }
};

// ============================================================
// 解法3: bool 标记数组去重（常数最优）
// 时间: O(log_x(bound) * log_y(bound))  空间: O(bound)
//
// [思路]
// 用 bool 数组 seen[bound+1] 代替 unordered_set，避免哈希开销。
// 最后遍历 seen 数组收集所有为 true 的下标。
//
// 优点：哈希表有冲突和 rehash 的开销，bool 数组是纯 O(1) 标记。
// 缺点：空间 O(bound) 而非 O(结果集)，但 bound <= 10^6 只用 1MB。
// ============================================================
class Solution3 {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        if (bound < 2) return {}; // 最小可能值是 1+1=2

        vector<bool> seen(bound + 1, false);

        for (long long pow_x = 1; pow_x <= bound; pow_x *= x) {
            for (long long pow_y = 1; pow_x + pow_y <= bound; pow_y *= y) {
                seen[(int)(pow_x + pow_y)] = true;
                if (y == 1) break;
            }
            if (x == 1) break;
        }

        // 收集所有标记为 true 的值
        vector<int> result;
        for (int val = 2; val <= bound; val++) {
            if (seen[val]) {
                result.push_back(val);
            }
        }
        return result;
    }
};

// ============================================================
// [解法对比]
//
// | 解法 | 时间 | 空间 | 去重方式 | 推荐场景 |
// |------|------|------|----------|----------|
// | 直接枚举+set | O(log^2) | O(结果集) | unordered_set | ⭐ 面试首选 |
// | 预计算数组 | O(log^2) | O(结果集) | unordered_set | 逻辑清晰 |
// | bool标记 | O(log^2) | O(bound) | bool数组 | 极致常数 |
//
// [易错点]
//
// 1. x = 1 或 y = 1 时无限循环
//    ✗ 忘记 break → while 条件永远满足
//    ✓ 在循环体末尾加 if (x == 1) break / if (y == 1) break
//
// 2. 忘记去重
//    ✗ 直接 push_back 到 vector → 可能有重复值
//    ✓ 使用 set / unordered_set / bool 数组去重
//
// 3. bound = 0 时的边界
//    最小可能值是 x^0 + y^0 = 2，如果 bound < 2 应返回空
//    循环条件 pow_x + pow_y <= 0 一开始就不满足，自然返回空
//
// 4. 整数类型
//    pow_x *= x 可能溢出 int（虽然 bound=10^6 时不会）
//    养成好习惯用 long long 做循环变量
//
// [面试追问]
//
// Q1: 如果 x = y = 1？
// → 外层跑 1 次，内层跑 1 次，只有 1+1=2。bound >= 2 返回 {2}。
//
// Q2: 结果集最大有多大？
// → bound=10^6 时，实际不同的和不会太多。理论上界：
//   x=2 时 powersX 有 ~20 个值，y=2 时 powersY 也 ~20 个，
//   最多 400 种和，去重后更少。
//
// Q3: 能否对结果排序？
// → 题目说 any order，不需要。如需要，用 set 代替 unordered_set
//   或者最后 sort(result.begin(), result.end())。
// ============================================================
