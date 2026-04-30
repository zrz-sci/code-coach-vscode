/*
 * 【题目本质】
 * 在 3x3 九宫格上枚举所有合法的解锁路径（长度 [m,n]），
 * 约束是跳过中间点时该点必须已被选中。
 * 本质是"带约束的排列枚举"，用回溯 + 对称性优化。
 *
 * 【解法总览】
 * 解法1: 回溯 + 跳跃表预处理 + 对称性优化  O(9!) / O(9)  ⭐推荐
 * 解法2: 回溯 + 坐标实时计算中间点          O(9!) / O(9)   无需预处理
 */

// ===================== Solution 1: 回溯 + 跳跃表 + 对称性 =====================
// 思路：
//   预处理 skip[i][j] = 从 i 到 j 必须经过的中间点
//   利用对称性：角点×4 + 边点×4 + 中心点×1
//   DFS 回溯枚举所有合法路径
// 时间 O(9!)（实际远小），空间 O(9)
class Solution1 {
public:
    int numberOfPatterns(int m, int n) {
        // 预处理跳跃中间点（0 表示无需经过中间点）
        vector<vector<int>> skip(10, vector<int>(10, 0));
        // 行跳跃
        skip[1][3] = skip[3][1] = 2;  // 1-3 经过 2
        skip[4][6] = skip[6][4] = 5;  // 4-6 经过 5
        skip[7][9] = skip[9][7] = 8;  // 7-9 经过 8
        // 列跳跃
        skip[1][7] = skip[7][1] = 4;  // 1-7 经过 4
        skip[2][8] = skip[8][2] = 5;  // 2-8 经过 5
        skip[3][9] = skip[9][3] = 6;  // 3-9 经过 6
        // 对角线跳跃
        skip[1][9] = skip[9][1] = 5;  // 1-9 经过 5
        skip[3][7] = skip[7][3] = 5;  // 3-7 经过 5

        vector<bool> visited(10, false);
        int result = 0;

        // 对称性优化：3 类代表点
        result += dfs(1, 1, m, n, skip, visited) * 4;  // 角点 {1,3,7,9}
        result += dfs(2, 1, m, n, skip, visited) * 4;  // 边点 {2,4,6,8}
        result += dfs(5, 1, m, n, skip, visited);       // 中心 {5}

        return result;
    }

private:
    int dfs(int cur, int len, int m, int n,
            vector<vector<int>>& skip, vector<bool>& visited) {
        int count = 0;
        if (len >= m) count++;   // 路径长度达到下界，计入结果
        if (len >= n) return count; // 达到上界，停止延伸

        visited[cur] = true;
        for (int next = 1; next <= 9; next++) {
            if (visited[next]) continue;
            int mid = skip[cur][next];
            // 合法条件：无中间点 或 中间点已访问
            if (mid == 0 || visited[mid]) {
                count += dfs(next, len + 1, m, n, skip, visited);
            }
        }
        visited[cur] = false;  // 回溯

        return count;
    }
};

// ===================== Solution 2: 回溯 + 坐标计算 =====================
// 思路：不预处理跳跃表，实时用坐标计算中间点
//   点 i 的坐标：row = (i-1)/3, col = (i-1)%3
//   两点中间点存在条件：行差和列差之和的各分量都为偶数
//   中间点坐标：((r1+r2)/2, (c1+c2)/2)
// 时间 O(9!)，空间 O(9)
class Solution2 {
public:
    int numberOfPatterns(int m, int n) {
        vector<bool> visited(10, false);
        int result = 0;
        result += dfs(1, 1, m, n, visited) * 4;
        result += dfs(2, 1, m, n, visited) * 4;
        result += dfs(5, 1, m, n, visited);
        return result;
    }

private:
    int dfs(int cur, int len, int m, int n, vector<bool>& visited) {
        int count = 0;
        if (len >= m) count++;
        if (len >= n) return count;

        visited[cur] = true;
        for (int next = 1; next <= 9; next++) {
            if (visited[next]) continue;

            // 计算 cur 和 next 的坐标
            int r1 = (cur - 1) / 3, c1 = (cur - 1) % 3;
            int r2 = (next - 1) / 3, c2 = (next - 1) % 3;

            // 判断是否存在中间点
            int sumR = r1 + r2, sumC = c1 + c2;
            if (sumR % 2 == 0 && sumC % 2 == 0) {
                int mid = (sumR / 2) * 3 + (sumC / 2) + 1;
                if (mid != cur && mid != next && !visited[mid])
                    continue;  // 中间点存在但未访问，非法
            }

            count += dfs(next, len + 1, m, n, visited);
        }
        visited[cur] = false;
        return count;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 | O(9!) | O(9) | 跳跃表直观，不易出错 | 需要手动列举 8 对关系 |
 * | Solution2 | O(9!) | O(9) | 无需预处理，代码更通用 | 坐标计算易写错 |
 *
 * 【易错点】
 * 1. 跳跃关系是双向的：skip[i][j] 和 skip[j][i] 都要设置
 * 2. 不是只有对角线跳跃有中间点，行/列方向的跳跃也有
 * 3. len >= m 时就要计数，不是只在 len == m 时
 * 4. 回溯时必须还原 visited[cur] = false
 * 5. 对称性分组：{1,3,7,9} 角点 ×4, {2,4,6,8} 边点 ×4, {5} 中心 ×1
 *
 * 【面试追问】
 * Q1: 对称性优化为何正确？ → 九宫格有 4 重旋转对称，角/边/中心各自等价
 * Q2: 能否用状态压缩 DP？ → dp[mask][last]，状态数 2^9×9=4608，可行
 * Q3: 如果九宫格改为 4×4 呢？ → 对称性减弱，回溯仍可行但需更多中间点关系
 */
