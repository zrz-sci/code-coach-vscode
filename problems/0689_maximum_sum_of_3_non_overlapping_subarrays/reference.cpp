// LeetCode 689: Maximum Sum of 3 Non-Overlapping Subarrays
// 解法一: left/right 预处理 + 枚举中间子数组
// 解法二: DP 通用做法（可推广到 m 个子数组）
// 时间: O(n), 空间: O(n)

// ============================================================
// 核心思路:
// 1. 预处理 windowSum[i] = nums[i..i+k-1] 的和
// 2. left[i] = [0..i] 中 windowSum 最大的索引（字典序最小）
// 3. right[i] = [i..end] 中 windowSum 最大的索引（字典序最小）
// 4. 枚举中间子数组 j, 用 left[j-k] 和 right[j+k] 找最优左右
// ============================================================

// 解法一: left/right 预处理（推荐，最清晰）
class Solution {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        int m = n - k + 1; // 窗口和数组的大小

        // === Step 1: 滑动窗口计算 windowSum ===
        vector<int> windowSum(m, 0);
        int runningSum = 0;
        for (int i = 0; i < n; i++) {
            runningSum += nums[i];
            if (i >= k) runningSum -= nums[i - k]; // 窗口左端滑出
            if (i >= k - 1) windowSum[i - k + 1] = runningSum; // 窗口完整了
        }

        // === Step 2: 构建 left 数组 ===
        // left[i] = [0..i] 中 windowSum 最大的起始索引
        // 严格大于才更新 → 保证字典序最小（保留最小索引）
        vector<int> left(m, 0);
        int bestLeft = 0;
        for (int i = 0; i < m; i++) {
            if (windowSum[i] > windowSum[bestLeft]) {
                bestLeft = i;
            }
            left[i] = bestLeft;
        }

        // === Step 3: 构建 right 数组 ===
        // right[i] = [i..m-1] 中 windowSum 最大的起始索引
        // 大于等于就更新 → 从右往左扫时取更小索引，保证字典序
        vector<int> right(m, m - 1);
        int bestRight = m - 1;
        for (int i = m - 1; i >= 0; i--) {
            if (windowSum[i] >= windowSum[bestRight]) {
                bestRight = i;
            }
            right[i] = bestRight;
        }

        // === Step 4: 枚举中间子数组起始位置 ===
        // j 的范围: [k, m-1-k]
        //   左侧子数组结束于 j-1, 起始不超过 j-k → left[j-k]
        //   右侧子数组起始不早于 j+k → right[j+k]
        vector<int> ans = {-1, -1, -1};
        int maxTotal = 0;

        for (int j = k; j <= m - 1 - k; j++) {
            int l = left[j - k];       // 左侧最优窗口索引
            int r = right[j + k];      // 右侧最优窗口索引
            int total = windowSum[l] + windowSum[j] + windowSum[r];

            if (total > maxTotal) {
                maxTotal = total;
                ans = {l, j, r};
            }
        }

        return ans;
    }
};

// ============================================================
// 解法二: DP 通用做法（可推广到选 m 个不重叠子数组）
// 时间: O(n * T) where T = 要选的子数组个数
// 空间: O(n * T)
//
// dp[t][i] = 在窗口 0..i 中选 t 个不重叠窗口的最大和
// 转移:
//   不选窗口 i: dp[t][i] = dp[t][i-1]
//   选窗口 i:   dp[t][i] = dp[t-1][i-k] + windowSum[i]
//   dp[t][i] = max(上述两者)
//
// 需要额外回溯数组 choice[t][i] 来恢复选择了哪些窗口
// ============================================================

class Solution2 {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        int m = n - k + 1;
        int T = 3; // 选 3 个子数组

        // 计算窗口和
        vector<int> windowSum(m, 0);
        int runSum = 0;
        for (int i = 0; i < n; i++) {
            runSum += nums[i];
            if (i >= k) runSum -= nums[i - k];
            if (i >= k - 1) windowSum[i - k + 1] = runSum;
        }

        // dp[t][i] = 在窗口 0..i 中选 t 个不重叠窗口的最大和
        // choice[t][i] = true 表示在最优方案中选了窗口 i
        vector<vector<int>> dp(T + 1, vector<int>(m, 0));
        vector<vector<bool>> choice(T + 1, vector<bool>(m, false));

        for (int t = 1; t <= T; t++) {
            for (int i = 0; i < m; i++) {
                // 不选窗口 i
                dp[t][i] = (i > 0) ? dp[t][i - 1] : 0;
                choice[t][i] = false;

                // 选窗口 i（前提: 前 t-1 个可以在 [0..i-k] 中选出）
                int prevEnd = i - k; // 上一个窗口的最大结束位置对应的索引
                int prevBest = 0;
                if (t == 1) {
                    prevBest = 0; // 只选 1 个，不需要之前的
                } else if (prevEnd >= 0) {
                    prevBest = dp[t - 1][prevEnd];
                } else {
                    continue; // 空间不够放 t 个窗口
                }

                int selectVal = prevBest + windowSum[i];
                if (selectVal > dp[t][i]) {
                    dp[t][i] = selectVal;
                    choice[t][i] = true;
                }
            }
        }

        // 回溯: 从 dp[T][m-1] 开始，找出选了哪 T 个窗口
        vector<int> ans;
        int pos = m - 1;
        for (int t = T; t >= 1; t--) {
            // 找到第 t 个窗口的位置
            while (pos >= 0 && !choice[t][pos]) {
                pos--;
            }
            ans.push_back(pos);
            pos -= k; // 跳过当前窗口的 k 个位置
        }

        // ans 是从后往前找的，需要反转
        reverse(ans.begin(), ans.end());
        return ans;
    }
};

// ============================================================
// 示例推演 (解法一):
// nums = [1,2,1,2,6,7,5,1], k = 2
//
// windowSum: [3, 3, 3, 8, 13, 12, 6]   (m = 7)
//            i: 0  1  2  3   4   5  6
//
// left: [0, 0, 0, 3, 4, 4, 4]
//   i=0: best=0 (ws=3)
//   i=1: ws[1]=3 不>3, left[1]=0
//   i=2: ws[2]=3 不>3, left[2]=0
//   i=3: ws[3]=8 > 3, best=3, left[3]=3
//   i=4: ws[4]=13 > 8, best=4, left[4]=4
//   i=5: ws[5]=12 不>13, left[5]=4
//   i=6: ws[6]=6 不>13, left[6]=4
//
// right: [4, 4, 4, 4, 4, 5, 6]
//   i=6: best=6 (ws=6)
//   i=5: ws[5]=12 >= 6, best=5
//   i=4: ws[4]=13 >= 12, best=4
//   i=3: ws[3]=8 不>=13, right[3]=4
//   i=2: 同上, right[2]=4
//   i=1: 同上, right[1]=4
//   i=0: 同上, right[0]=4
//
// 枚举 j in [2, 4]:
//   j=2: l=left[0]=0, r=right[4]=4, total=3+3+13=19
//   j=3: l=left[1]=0, r=right[5]=5, total=3+8+12=23 ★
//   j=4: l=left[2]=0, r=right[6]=6, total=3+13+6=22
//
// 答案: {0, 3, 5}
// ============================================================
