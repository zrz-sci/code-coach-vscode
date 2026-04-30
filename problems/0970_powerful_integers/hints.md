# 970. Powerful Integers

[file://solution.cpp](./solution.cpp)

## 核心思路

给定三个整数 x, y, bound，返回所有 x^i + y^j <= bound 的不同结果值。由于 bound <= 10^6 且底数 >= 1，指数的上界非常有限（最多 log2(10^6) ~ 20），可以直接枚举所有 (i, j) 对，用 set 去重。

## 思维链

1. **读完题第一反应** --> 需要找出所有 x^i + y^j <= bound 的不同值。暴力枚举所有 (i, j) 组合？但 i 和 j 的范围是多少？如果底数很大，指数很快就超过 bound；如果底数是 1，则 1^i 永远是 1，需要特殊处理。

2. **确定枚举范围** --> 当 x >= 2 时，x^i 增长至少以指数速度增长，x^i <= bound 意味着 i <= log_x(bound) <= log_2(10^6) ~ 20。所以 i 最多 20 左右。同理 j 最多 20。总共最多 400 种组合，枚举完全可行。

3. **x = 1 或 y = 1 的边界** --> 1^i = 1 对所有 i 都成立，如果不加限制会无限循环。解决方案：当 x = 1 时，只让 i 取 0（因为 1^0 = 1^1 = ... = 1，只需算一次）。y = 1 同理。

4. **去重** --> 不同的 (i, j) 可能产生相同的 x^i + y^j，题目要求 each value at most once。使用 unordered_set 或 set 自动去重。

5. **优化思考** --> 外层枚举 x^i（从 i=0 开始，当 x^i > bound 时停），内层枚举 y^j（从 j=0 开始，当 x^i + y^j > bound 时停）。总共 O(log(bound)^2) 次枚举，加上 set 插入 O(1) 平均或 O(log n)。

6. **返回格式** --> 将 set 转成 vector 返回。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 + Set | 双层循环枚举所有 x^i 和 y^j，set 去重 | O(log^2(bound)) | O(结果集大小) | 唯一合理解法 |

## 关键提示

1. **x = 1 的特殊处理**: 如果不做特殊处理，while 循环中 pow_x 永远是 1，循环永不终止。必须在 x = 1 时让外层循环只执行一次（i = 0 后 break），y = 1 时同理让内层循环只执行一次。

2. **整数溢出风险**: 虽然 bound <= 10^6，但如果 x = 100，则 100^3 = 10^6 已经到达边界。用 long long 或者在循环条件中直接比较 pow_x <= bound 即可，不需要提前计算 x^20 之类的大数。

3. **为什么 set 而不是 sort + unique？** set 在插入时自动去重，代码更简洁。结果集很小（bound <= 10^6 时最多几百个值），性能差异可忽略。

4. **枚举过程可视化**:
```
x = 2, y = 3, bound = 10

x^i: 1(i=0), 2(i=1), 4(i=2), 8(i=3), 16 > 10 stop
y^j: 1(j=0), 3(j=1), 9(j=2), 27 > 10 stop

i=0, j=0: 1+1=2  ✓
i=0, j=1: 1+3=4  ✓
i=0, j=2: 1+9=10 ✓
i=1, j=0: 2+1=3  ✓
i=1, j=1: 2+3=5  ✓
i=1, j=2: 2+9=11 > 10 stop
i=2, j=0: 4+1=5  (重复，set去重)
i=2, j=1: 4+3=7  ✓
i=2, j=2: 4+9=13 > 10 stop
i=3, j=0: 8+1=9  ✓
i=3, j=1: 8+3=11 > 10 stop

结果: {2, 3, 4, 5, 7, 9, 10}
```

5. **循环终止条件的另一种写法**: 可以用 `for (long long a = 1; a <= bound; a = (x == 1) ? bound + 1 : a * x)` 把特殊情况合并到循环条件中，但不如显式 break 清晰。

## 解法详解

### 解法1: 暴力枚举 + Set -- O(log^2(bound)) / O(结果集)

**思考过程**: bound <= 10^6，底数 >= 2 时指数最多约 20。直接双层循环枚举所有 x^i 和 y^j 的组合，将和 <= bound 的值放入 set 去重。

```cpp
class Solution {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        unordered_set<int> result;
        
        // 枚举 x^i，当 x = 1 时只取 i = 0
        for (long long pow_x = 1; pow_x <= bound; pow_x *= x) {
            // 枚举 y^j，当 y = 1 时只取 j = 0
            for (long long pow_y = 1; pow_x + pow_y <= bound; pow_y *= y) {
                result.insert((int)(pow_x + pow_y));
                if (y == 1) break; // 避免 1^j 无限循环
            }
            if (x == 1) break; // 避免 1^i 无限循环
        }
        
        return vector<int>(result.begin(), result.end());
    }
};
```

**关键点**:
- `pow_x` 和 `pow_y` 用 long long 防止乘法溢出（虽然本题 bound 小，习惯良好）
- break 语句处理 x = 1 和 y = 1 的无限循环问题
- unordered_set 保证 O(1) 平均插入和自动去重

---

### 解法1 变体: 预计算幂次数组

**思路**: 先把所有可能的 x^i 和 y^j 存入数组，再双层遍历求和。逻辑更清晰。

```cpp
class Solution {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        vector<long long> powersX, powersY;
        
        // 预计算所有 x^i <= bound
        for (long long val = 1; val <= bound; val *= x) {
            powersX.push_back(val);
            if (x == 1) break;
        }
        // 预计算所有 y^j <= bound
        for (long long val = 1; val <= bound; val *= y) {
            powersY.push_back(val);
            if (y == 1) break;
        }
        
        unordered_set<int> result;
        for (long long px : powersX) {
            for (long long py : powersY) {
                if (px + py <= bound) {
                    result.insert((int)(px + py));
                }
            }
        }
        
        return vector<int>(result.begin(), result.end());
    }
};
```

## 解法对比

| | 直接枚举 | 预计算幂次 |
|---|---|---|
| 时间 | O(log_x(bound) * log_y(bound)) | O(log_x(bound) * log_y(bound)) |
| 空间 | O(结果集) | O(log_x(bound) + log_y(bound) + 结果集) |
| 代码量 | 少 | 稍多 |
| 可读性 | 好 | 更好（逻辑分离） |
| 面试推荐 | 首选 | 如果面试官要求清晰度 |

## 易错点

1. **x = 1 或 y = 1 时无限循环**
   - x `pow_x *= 1` 永远是 1，循环永不终止
   - o 在循环体末尾加 `if (x == 1) break;`

2. **忘记去重**
   - x 不同的 (i, j) 组合可能产生相同的和，如 2^2 + 3^0 = 5 和 2^0 + 3^1 = 4 虽不同，但 2^2 + 3^0 = 5 和 2^1 + 3^1 = 5 是相同的
   - o 使用 set 或 unordered_set 自动去重

3. **循环条件写错导致漏算**
   - x `pow_x < bound` 应该是 `pow_x <= bound`（因为 x^i + y^0 = x^i + 1 可能 <= bound）
   - 但内层条件是 `pow_x + pow_y <= bound`，所以外层用 `pow_x <= bound` 足够（因为 pow_y >= 1）
   - 更精确：外层可以用 `pow_x + 1 <= bound`（即 `pow_x <= bound - 1`），但直接 `pow_x <= bound` 更简洁安全

4. **返回值顺序**
   - 题目说 "any order"，所以不需要排序。直接从 set 构造 vector 即可。

5. **bound = 0 的边界**
   - x^0 + y^0 = 2 > 0，所以结果为空。循环条件 `pow_x + pow_y <= 0` 一开始就不满足，自然返回空。

## 面试追问

**Q1: 时间复杂度具体是多少？**
> 当 x >= 2, y >= 2 时，外层循环 O(log_x(bound)) 次，内层 O(log_y(bound)) 次。bound = 10^6 时大约 20 * 20 = 400 次。当 x = 1 时外层只跑 1 次，总共 O(log_y(bound))。最坏 O(log^2(bound))。

**Q2: 能否不用 set？**
> 可以。bound <= 10^6，用一个 bool 数组 seen[bound+1] 标记是否已出现，空间 O(bound)。或者把所有结果存入 vector，最后 sort + unique + erase 去重。但 set 更简洁。

**Q3: 如果 bound 非常大（比如 10^18），怎么办？**
> 指数个数仍然只有 O(log(bound)) 约 60 个，枚举量 60*60 = 3600。但结果集可能很大。需要用 unordered_set<long long> 存储。核心算法不变，只是数据类型改成 long long。

**Q4: x = 1 且 y = 1 的情况？**
> 两个循环都只跑一次，结果只有 1^0 + 1^0 = 2。如果 bound >= 2 返回 {2}，否则返回空。

## 相关题型

- **LC 326: Power of Three** -- 判断一个数是否是 3 的幂，考察对数和循环除法，本题中枚举幂次的循环与之类似
- **LC 231: Power of Two** -- 判断 2 的幂，可以用位运算 n & (n-1) == 0，本题中 x = 2 的情况下幂次增长最慢
- **LC 342: Power of Four** -- 判断 4 的幂，本题的枚举框架可直接复用
- **LC 829: Consecutive Numbers Sum** -- 也是枚举 + 数学边界分析的题型，需要确定枚举上界
