// 解法1: 贪心 (跳跃游戏变形) — 最优解
// 时间 O(n + time), 空间 O(time)
//
// 核心思路:
// 1. 预处理: maxEnd[i] = 从位置 i 出发的所有片段中，能到达的最远终点
// 2. 贪心: 类似 Jump Game II，维护 curEnd 和 farthest
//    - curEnd: 当前已选片段覆盖的右边界
//    - farthest: 在 [0, curEnd] 范围内，下一步最远能到的位置
//    - 每次到达 curEnd 时必须选一个新片段 (res++)
//
// 等价转化:
//   clips -> maxEnd 数组 -> 跳跃游戏
//   选片段 <-> 跳一步
//   覆盖 [0, time] <-> 从 0 跳到 >= time
class Solution1 {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        // 预处理: 对每个起点，记录最远终点
        // maxEnd[i] = max(end) for all clips with start == i
        vector<int> maxEnd(time + 1, 0);
        for (auto& c : clips) {
            if (c[0] <= time) {
                maxEnd[c[0]] = max(maxEnd[c[0]], c[1]);
            }
        }

        int res = 0;       // 已选片段数
        int curEnd = 0;     // 当前覆盖右边界
        int farthest = 0;   // 下一步最远可达

        for (int i = 0; i < time; i++) {
            // 更新从位置 i 出发能到的最远距离
            farthest = max(farthest, maxEnd[i]);

            // 如果 farthest <= i，说明有断档，无法继续覆盖
            // 例: clips = [[0,1],[3,5]], time = 5
            //     在 i=1 时 farthest=1, i=2 时 farthest 仍为 1 <= 2 -> 断档
            if (farthest <= i) return -1;

            // 到达当前覆盖边界，必须选一个新片段
            if (i == curEnd) {
                res++;
                curEnd = farthest;
                // 提前终止: 已覆盖到 time
                if (curEnd >= time) return res;
            }
        }

        return -1; // 无法覆盖到 time
    }
};

// 解法2: 动态规划
// 时间 O(n * time), 空间 O(time)
//
// dp[i] = 覆盖 [0, i] 所需的最少片段数
// 初始: dp[0] = 0, 其余 = INT_MAX
// 转移: 对每个片段 [s, e]，如果 dp[s] != INT_MAX，
//       则 dp[j] = min(dp[j], dp[s] + 1) 对所有 s < j <= e
class Solution2 {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        vector<int> dp(time + 1, INT_MAX);
        dp[0] = 0;

        // 按起点排序确保正确的 DP 顺序
        sort(clips.begin(), clips.end());

        for (auto& c : clips) {
            int s = c[0], e = min(c[1], time);
            // 跳过无用片段
            if (s > time) continue;
            if (dp[s] == INT_MAX) continue; // 起点不可达

            // 这个片段可以覆盖 [s, e]，更新 dp[s+1..e]
            for (int j = s + 1; j <= e; j++) {
                dp[j] = min(dp[j], dp[s] + 1);
            }
        }

        return dp[time] == INT_MAX ? -1 : dp[time];
    }
};

// 解法3: 排序 + 区间覆盖经典贪心
// 时间 O(n log n), 空间 O(1) (不含排序空间)
//
// 经典区间覆盖策略:
// 1. 按起点排序
// 2. 在所有 start <= curEnd 的片段中，选 end 最大的
// 3. 更新 curEnd = farthest，重复直到覆盖 [0, time]
class Solution3 {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        // 按起点排序，起点相同按终点降序(优先选更长的)
        sort(clips.begin(), clips.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
        });

        int res = 0;        // 已选片段数
        int curEnd = 0;      // 当前覆盖右边界
        int i = 0;           // clips 扫描指针
        int n = clips.size();

        while (curEnd < time) {
            int farthest = curEnd;

            // 找所有 start <= curEnd 的片段中，end 最大的
            while (i < n && clips[i][0] <= curEnd) {
                farthest = max(farthest, clips[i][1]);
                i++;
            }

            // 无法扩展 -> 有断档
            if (farthest == curEnd) return -1;

            res++;
            curEnd = farthest;
        }

        return res;
    }
};

/*
 * 解法对比:
 * ┌─────────────────────────┬──────────────┬─────────┬──────────────────┐
 * │ 解法                    │ 时间         │ 空间    │ 特点             │
 * ├─────────────────────────┼──────────────┼─────────┼──────────────────┤
 * │ Solution1 跳跃游戏贪心  │ O(n + time)  │ O(time) │ 最优，面试首选   │
 * │ Solution2 DP            │ O(n * time)  │ O(time) │ 经典DP思路       │
 * │ Solution3 排序贪心      │ O(n log n)   │ O(1)    │ 区间覆盖经典方法 │
 * └─────────────────────────┴──────────────┴─────────┴──────────────────┘
 *
 * 图解 (Example 1):
 *   clips: [0,2],[1,5],[1,9],[4,6],[5,9],[8,10], time=10
 *
 *   时间轴: 0  1  2  3  4  5  6  7  8  9  10
 *   [0,2]:  |-----|
 *   [1,9]:     |--------------------------|
 *   [8,10]:                         |-----|
 *
 *   贪心选择: [0,2] + [1,9] + [8,10] = 3 个片段
 *
 * 与 Jump Game II 的等价关系:
 *   Jump Game II: nums = [2, 9, 0, 0, 6, 9, 0, 0, 10, 0]
 *   Video Stitch: maxEnd = [2, 9, 0, 0, 6, 9, 0, 0, 10, 0]
 *   完全相同的贪心逻辑!
 *
 * 易错点:
 * 1. 必须从 0 开始: 没有 start=0 的片段则返回 -1
 * 2. 断档检测: farthest <= i (Solution1) 或 farthest == curEnd (Solution3)
 * 3. maxEnd 数组只需到 time (超出的起点忽略)
 * 4. DP 初始化 dp[0]=0，其余 INT_MAX
 * 5. 提前终止: curEnd >= time 时立即返回
 *
 * 面试追问:
 * Q: 为什么贪心正确? -> 区间覆盖的贪心选择性质
 * Q: 有权重怎么办? -> 加权区间覆盖 DP
 * Q: 时间轴 10^9? -> 离散化或用排序贪心(Solution3)
 */
