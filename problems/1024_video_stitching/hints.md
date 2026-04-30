# 1024. 视频拼接 (Video Stitching)

## 难度: Medium
## 标签: Greedy, Array, Dynamic Programming

---

## 题目理解

有一系列视频片段 clips[i] = [start, end]，代表从 start 到 end 的录像。片段可以自由裁剪。
求：最少选多少个片段，拼接起来覆盖完整时间段 [0, time]。如果不可能则返回 -1。

关键点：
- 片段可以重叠
- 片段可以自由裁剪（只用其中一部分也行）
- 必须从 0 开始覆盖到 time
- 求**最少片段数**

示例理解：
```
clips = [[0,2],[4,6],[8,10],[1,9],[1,5],[5,9]], time = 10
选择 [0,2], [1,9], [8,10] -> 三个片段覆盖 [0,10]
  [0,2]  覆盖 [0,2]
  [1,9]  覆盖 [2,9] (裁剪后)
  [8,10] 覆盖 [9,10] (裁剪后)
答案: 3
```

---

## 核心思路

这道题本质上是**区间覆盖问题**，可以转化为**跳跃游戏 (Jump Game II)** 来用贪心解决。

**转化关键**: 将 clips 预处理为 maxEnd[i] 数组，其中 maxEnd[i] 表示所有从位置 i 出发的片段能到达的最远终点。然后问题变成"从位置 0 出发，每步最远能跳到 maxEnd[i]，最少跳几步到 time"。

---

## 思维链 (从观察到解法)

### 第一步: 识别问题类型
- "最少片段覆盖 [0, time]" -> 经典的**区间覆盖问题**
- 与 LC 45 Jump Game II 高度相似

### 第二步: 预处理 - 建立 maxEnd 数组
对于每个位置 i (0 到 time)，记录所有以 i 为起点的片段中最远的终点。

```
clips = [[0,2],[4,6],[8,10],[1,9],[1,5],[5,9]]
maxEnd[0] = 2  (片段[0,2])
maxEnd[1] = 9  (片段[1,9]最远)
maxEnd[4] = 6  (片段[4,6])
maxEnd[5] = 9  (片段[5,9])
maxEnd[8] = 10 (片段[8,10])
其余位置 maxEnd[i] = 0
```

### 第三步: 贪心扫描 - 类似 Jump Game II
维护两个变量：
- `curEnd`: 当前已选片段能覆盖到的右边界
- `farthest`: 在 [0, curEnd] 范围内，下一步最远能到达的位置

当扫描到 i == curEnd 时，必须"选一个新片段"(res++)，并更新 curEnd = farthest。

```
扫描过程 (time=10):
i=0: farthest = max(0, maxEnd[0]) = 2
     i==curEnd(0), 选片段! res=1, curEnd=2
i=1: farthest = max(2, maxEnd[1]) = 9
i=2: i==curEnd(2), 选片段! res=2, curEnd=9
     curEnd(9) < time(10), 继续
i=3: farthest = max(9, 0) = 9
...
i=8: farthest = max(9, maxEnd[8]) = 10
i=9: i==curEnd(9), 选片段! res=3, curEnd=10
     curEnd(10) >= time(10), 返回 3
```

### 第四步: 处理无解情况
如果在某个位置 i，farthest <= i，说明有"断档"，无法继续覆盖，返回 -1。

---

## 解法表
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| Solution1: 贪心(跳跃游戏) | O(n + time) | O(time) | 最优解，面试首选 |
| Solution2: DP | O(n * time) | O(time) | 经典DP思路 |
| Solution3: 排序+贪心 | O(n log n) | O(1) | 区间覆盖经典贪心 |

---

## 解法详解

### Solution1: 贪心 (跳跃游戏变形)

**算法流程**:
1. 预处理: 对每个起点 i，记录 maxEnd[i] = 所有以 i 为起点的片段的最远终点
2. 贪心扫描: 从 i=0 到 time-1，维护 farthest 和 curEnd
3. 到达 curEnd 时选新片段，更新 curEnd = farthest

**为什么是正确的？**
在每个"跳跃点"(i == curEnd)，我们选择能到达最远的片段，这保证了用最少的片段。这是标准的区间覆盖贪心策略。

```cpp
class Solution1 {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        // 预处理: maxEnd[i] = 从位置 i 出发能到的最远终点
        vector<int> maxEnd(time + 1, 0);
        for (auto& c : clips) {
            if (c[0] <= time) {
                maxEnd[c[0]] = max(maxEnd[c[0]], c[1]);
            }
        }

        int res = 0, curEnd = 0, farthest = 0;
        for (int i = 0; i < time; i++) {
            farthest = max(farthest, maxEnd[i]);
            if (farthest <= i) return -1; // 断档
            if (i == curEnd) {
                res++;
                curEnd = farthest;
                if (curEnd >= time) return res;
            }
        }
        return -1; // 无法覆盖到 time
    }
};
```

---

### Solution2: 动态规划

**定义**: dp[i] = 覆盖 [0, i] 所需的最少片段数
**转移**: 对每个片段 [s, e]，dp[e] = min(dp[e], dp[s] + 1)
**注意**: 需要考虑片段内部的传递，dp[j] = dp[s] + 1 对所有 s < j <= e

```cpp
class Solution2 {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        // dp[i] = 覆盖 [0, i] 所需最少片段数
        vector<int> dp(time + 1, INT_MAX);
        dp[0] = 0;

        // 按起点排序
        sort(clips.begin(), clips.end());

        for (auto& c : clips) {
            int s = c[0], e = min(c[1], time);
            if (s > time || dp[s] == INT_MAX) continue;
            // 片段 [s, e] 可以覆盖 s 到 e 之间的任何位置
            for (int j = s + 1; j <= e; j++) {
                dp[j] = min(dp[j], dp[s] + 1);
            }
        }

        return dp[time] == INT_MAX ? -1 : dp[time];
    }
};
```

---

### Solution3: 排序 + 区间覆盖贪心

**经典区间覆盖**: 按起点排序，每次在起点 <= 当前右边界的片段中选终点最远的。

```cpp
class Solution3 {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        // 按起点排序，起点相同按终点降序
        sort(clips.begin(), clips.end(), [](const auto& a, const auto& b) {
            return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
        });

        int res = 0, curEnd = 0, i = 0;
        int n = clips.size();

        while (curEnd < time) {
            int farthest = curEnd;
            // 在所有起点 <= curEnd 的片段中，找终点最远的
            while (i < n && clips[i][0] <= curEnd) {
                farthest = max(farthest, clips[i][1]);
                i++;
            }
            if (farthest == curEnd) return -1; // 无法扩展
            res++;
            curEnd = farthest;
        }

        return res;
    }
};
```

---

## 三种解法的对比图解

```
输入: clips = [[0,2],[4,6],[8,10],[1,9],[1,5],[5,9]], time = 10

时间轴: 0  1  2  3  4  5  6  7  8  9  10
[0,2]:  |-----|
[1,5]:     |-----------|
[1,9]:     |--------------------------|
[4,6]:              |-----|
[5,9]:                 |-----------|
[8,10]:                         |-----|

Solution1 (跳跃游戏):
  maxEnd: [2, 9, 0, 0, 6, 9, 0, 0, 10, 0, 0]
  i=0: farthest=2, 选! curEnd=2, res=1
  i=1: farthest=9
  i=2: 选! curEnd=9, res=2
  ...
  i=8: farthest=10
  i=9: 选! curEnd=10>=10, res=3 -> 返回 3

Solution3 (排序贪心):
  排序后: [0,2],[1,5],[1,9],[4,6],[5,9],[8,10]
  Round 1: curEnd=0, 看 start<=0 的片段 -> [0,2], farthest=2, res=1
  Round 2: curEnd=2, 看 start<=2 的片段 -> [1,5],[1,9], farthest=9, res=2
  Round 3: curEnd=9, 看 start<=9 的片段 -> [4,6],[5,9],[8,10], farthest=10, res=3
  curEnd=10>=10, 返回 3
```

---

## 易错点

1. **必须从 0 开始**: 如果没有任何片段的 start == 0，直接返回 -1
2. **断档检测**: farthest <= i 时说明有间断，无法覆盖
3. **maxEnd 数组大小**: 应该是 time+1，且只关心 0 到 time 范围内的起点
4. **DP 解法的初始化**: dp[0] = 0，其余为 INT_MAX，注意溢出
5. **贪心不需要精确的片段选择**: 只需要知道最远能到哪里，具体选哪个片段无所谓
6. **clips 的 start 可能大于 time**: 预处理时需跳过

---

## 与 Jump Game II 的对比

```
Jump Game II (LC 45):           Video Stitching (LC 1024):
nums[i] = 从位置 i 能跳的距离    clips = 一系列 [start, end] 区间
maxReach[i] = i + nums[i]        maxEnd[i] = 从位置 i 出发最远终点
目标: 从 0 跳到 n-1              目标: 从 0 覆盖到 time
```

两题的贪心逻辑完全相同! Video Stitching 多了一步"将 clips 转化为 maxEnd 数组"的预处理。

---

## 面试追问递进链

1. **"为什么贪心是正确的？"**
   -> 区间覆盖问题的贪心选择性质：在当前覆盖范围内，选终点最远的片段一定不亏

2. **"DP 怎么做？"**
   -> dp[i] = 覆盖 [0,i] 的最少片段数，对每个片段更新

3. **"如果片段有权重(cost)怎么办？"**
   -> 不能用贪心了，必须用加权区间覆盖 DP

4. **"如何输出具体选了哪些片段？"**
   -> 在贪心过程中记录每次选择的片段 index

5. **"时间轴很大(如 10^9)怎么办？"**
   -> 离散化坐标，或者用排序+贪心(不依赖 maxEnd 数组)

---

## 相关题型

- [45. Jump Game II](../0045_jump_game_ii/) - 几乎完全相同的贪心策略
- [55. Jump Game](../0055_jump_game/) - 判断是否可达(不求最少步数)
- [1326. Minimum Number of Taps to Open to Water a Garden](../1326_minimum_number_of_taps_to_open_to_water_a_garden/) - 换皮题，区间覆盖
