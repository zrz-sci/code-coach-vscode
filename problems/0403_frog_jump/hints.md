# 403. Frog Jump - 青蛙过河

## 核心思路
青蛙从位置 0 出发，上一跳 k 步则下一跳只能 k-1/k/k+1 步，判断能否到达最后一块石头。核心是**动态规划 + 哈希表**：为每块石头记录"到达它时可能的跳跃步数集合"，然后从前往后转移。

## 思维链
1. 状态定义：`dp[stone] = {可能的跳跃步数集合}`
2. 初始状态：`dp[0] = {0}`（青蛙站在起点，上一跳为 0）
3. 转移：对于 stone 位置、跳跃步数 k，尝试跳到 stone+k-1, stone+k, stone+k+1
4. 答案：`dp[lastStone]` 非空则为 true
5. 优化：用 `unordered_map<int, unordered_set<int>>` 快速查找石头位置

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ HashMap DP | O(n^2) | O(n^2) | 最直观，面试首选 |
| ⭐ 记忆化搜索 | O(n^2) | O(n^2) | DFS + memo，思路自然 |

## 关键提示
1. **快速剪枝**：`stones[1] != 1` 直接返回 false（第一跳必须是 1）
2. **石头位置查找**：用 set/map 存石头位置，O(1) 判断目标位置是否有石头
3. **步数 k 必须 > 0**：k-1 可能为 0，跳 0 步无意义，需要过滤
4. **数据范围**：stone 值可达 2^31-1，不能用数组索引，必须用哈希表

## 解法详解

### 解法一：HashMap DP（推荐）
```cpp
class Solution {
public:
    bool canCross(vector<int>& stones) {
        unordered_map<int, unordered_set<int>> dp;
        for (int s : stones) dp[s] = {};
        dp[0].insert(0);
        
        for (int s : stones) {
            for (int k : dp[s]) {
                for (int next : {k - 1, k, k + 1}) {
                    if (next > 0 && dp.count(s + next)) {
                        dp[s + next].insert(next);
                    }
                }
            }
        }
        return !dp[stones.back()].empty();
    }
};
```

### 解法二：记忆化搜索
```cpp
class Solution {
    unordered_map<int, int> stoneIdx;
    unordered_map<long long, bool> memo;
public:
    bool dfs(vector<int>& stones, int idx, int k) {
        if (idx == (int)stones.size() - 1) return true;
        long long key = (long long)idx * 2001 + k;
        if (memo.count(key)) return memo[key];
        for (int next : {k - 1, k, k + 1}) {
            if (next > 0 && stoneIdx.count(stones[idx] + next)) {
                if (dfs(stones, stoneIdx[stones[idx] + next], next))
                    return memo[key] = true;
            }
        }
        return memo[key] = false;
    }
    
    bool canCross(vector<int>& stones) {
        for (int i = 0; i < (int)stones.size(); i++)
            stoneIdx[stones[i]] = i;
        return dfs(stones, 0, 0);
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 用 `vector<set<int>>` 按下标 | 用 `unordered_map<int, set<int>>` 按位置 | stone 值可达 2^31，不能当数组索引 |
| 忘记过滤 `k-1 == 0` | `if (next > 0)` | 跳 0 步原地不动，会死循环 |
| 遍历 set 时修改 set | 先收集再修改，或用拷贝 | 在迭代 dp[s] 的同时可能间接修改它 |
| 没有提前剪枝 | 检查 `stones[1] == 1` | 第一跳只能是 1，第二块石头必须在位置 1 |

## 面试追问

**Q1: 为什么时间复杂度是 O(n^2)？**
> 每块石头最多被 n 种不同步数到达（因为最大步数随跳跃次数线性增长），所以状态总数 O(n^2)，每个状态转移 O(1)。

**Q2: 能否用 BFS 解决？**
> 可以。将 (stone_position, last_jump_k) 作为状态入队，用 visited set 去重，本质和 DP 等价。

**Q3: 如果要输出具体跳跃路径怎么办？**
> 在 DP 中额外记录每个状态的前驱 (prev_stone, prev_k)，到达终点后回溯路径。

## 相关题型
- [1306. Jump Game III](https://leetcode.com/problems/jump-game-iii/) - 跳跃类 BFS
- [55. Jump Game](https://leetcode.com/problems/jump-game/) - 贪心跳跃
- [1696. Jump Game VI](https://leetcode.com/problems/jump-game-vi/) - DP + 单调队列
