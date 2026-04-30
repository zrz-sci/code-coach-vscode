# 458. 可怜的小猪 (Poor Pigs)

## 核心思路
每只猪在 T = minutesToTest / minutesToDie 轮测试中有 (T+1) 种状态（第 1 轮死、第 2 轮死...、不死），p 只猪能区分 (T+1)^p 种情况。找最小 p 使 (T+1)^p >= buckets。

## 思维链
1. 最多可测试 T = minutesToTest / minutesToDie 轮
2. 每只猪每轮要么死要么活 → 每只猪有 (T+1) 种结局（第 1~T 轮死，或存活）
3. p 只猪的结局组合 = (T+1)^p 种，每种对应一个桶
4. 需要 (T+1)^p >= buckets → p = ceil(log(buckets) / log(T+1))

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| Solution1 | 信息论/进制编码 ⭐ | O(log n) | O(1) | 最优解 |
| Solution2 | 动态规划 | O(n * p) | O(n * p) | 通用但复杂 |

## 关键提示
1. **状态编码**：每只猪是一个"位"，每位有 (T+1) 种取值，相当于 (T+1) 进制数
2. **信息论视角**：p 只猪提供 p * log2(T+1) bit 信息，需覆盖 log2(buckets) bit
3. **不需要开根号/log**：循环乘法更安全

## 解法详解

### Solution 1: 信息论 / 进制编码 ⭐
**思路**：每只猪可编码 (T+1) 种状态，p 只猪可编码 (T+1)^p 种。找最小 p 满足 (T+1)^p >= buckets。

```cpp
class Solution {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        int T = minutesToTest / minutesToDie; // 最大测试轮数
        int base = T + 1; // 每只猪的状态数
        int pigs = 0;
        long long power = 1;
        while (power < buckets) {
            power *= base;
            pigs++;
        }
        return pigs;
    }
};
```

### Solution 2: 使用 log 公式
**思路**：直接用对数公式 `ceil(log(buckets) / log(T+1))`。

```cpp
class Solution {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        int T = minutesToTest / minutesToDie;
        if (buckets == 1) return 0;
        return (int)ceil(log(buckets) / log(T + 1));
    }
};
```

## 易错点
- ✗ 认为每只猪只能测一次 → ✓ 活着的猪可以继续测试，共 T 轮
- ✗ 用 log 计算有浮点精度问题 → ✓ 循环乘法更安全
- ✗ buckets = 1 时忘记特判 → ✓ 只有一桶不需要任何猪

## 面试追问
**Q1: 为什么每只猪有 (T+1) 种状态而不是 2 种？**
A: 如果只能测 1 轮，每只猪确实只有 死/活 2 种。但可以测 T 轮：猪可能在第 1 轮死、第 2 轮死...第 T 轮死、或全程不死，共 T+1 种可区分的结局。

**Q2: 请用具体例子解释 2 只猪如何区分 4 桶（T=1）？**
A: 2 只猪，1 轮测试，base=2，2^2=4。编码：桶1=(0,0)，桶2=(1,0)，桶3=(0,1)，桶4=(1,1)。猪A 喝桶2和桶4，猪B 喝桶3和桶4。哪只猪死就能确定哪桶有毒。

**Q3: 如果猪不会立即死亡而是概率性死亡，如何处理？**
A: 这就变成信息论中的噪声信道编码问题，需要用更多猪来冗余编码，类似纠错码。

## 相关题型
- [319. 灯泡开关](https://leetcode.com/problems/bulb-switcher/) - 数学推理
- [292. Nim 游戏](https://leetcode.com/problems/nim-game/) - 博弈论/数学
- [1227. 飞机座位分配概率](https://leetcode.com/problems/airplane-seat-assignment-probability/) - 概率推理
