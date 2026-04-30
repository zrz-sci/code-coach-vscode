// 691. Stickers to Spell Word
// 难度: Hard
// 标签: Bit Manipulation, Memoization, Dynamic Programming, Bitmask

/*
 * 题目描述：
 * 给定 n 种贴纸（每种无限供应），每张贴纸上有一个小写单词。
 * 从贴纸上剪下字母拼出目标字符串 target，求最少需要多少张贴纸。
 * 不可能则返回 -1。target.length <= 15。
 *
 * 核心思路：
 * target 最多 15 个字符，用 bitmask 表示哪些位置已被覆盖。
 * 每次尝试一张贴纸来扩展状态，BFS/DP 求最少贴纸数。
 */

// ==================== 解法一：状压 DP 底部向上（推荐）====================
// 时间 O(2^n * m * n)，空间 O(2^n)
// n=target长度, m=贴纸数
class Solution {
public:
    int minStickers(vector<string>& stickers, string target) {
        int n = target.size();
        int total = 1 << n;

        // 预处理贴纸字符频次
        int m = stickers.size();
        vector<vector<int>> stickerFreq(m, vector<int>(26, 0));
        for (int i = 0; i < m; i++) {
            for (char c : stickers[i]) {
                stickerFreq[i][c - 'a']++;
            }
        }

        // 不可能情况判断：target 中每个字符必须至少被一张贴纸包含
        for (char c : target) {
            bool found = false;
            for (int i = 0; i < m; i++) {
                if (stickerFreq[i][c - 'a'] > 0) {
                    found = true;
                    break;
                }
            }
            if (!found) return -1;
        }

        // dp[mask] = 达到 mask 状态的最少贴纸数
        vector<int> dp(total, INT_MAX);
        dp[0] = 0;

        for (int mask = 0; mask < total; mask++) {
            if (dp[mask] == INT_MAX) continue;

            // 找到第一个未覆盖的位置（锁定搜索顺序，消除排列重复）
            int firstUnset = -1;
            for (int i = 0; i < n; i++) {
                if (!(mask & (1 << i))) {
                    firstUnset = i;
                    break;
                }
            }
            if (firstUnset == -1) continue;

            // 只尝试包含 target[firstUnset] 字符的贴纸
            for (int s = 0; s < m; s++) {
                if (stickerFreq[s][target[firstUnset] - 'a'] == 0) continue;

                // 模拟使用第 s 张贴纸，贪心覆盖尽可能多的字符
                int newMask = mask;
                vector<int> freq = stickerFreq[s];  // 复制频次
                for (int i = 0; i < n; i++) {
                    if (!(newMask & (1 << i)) && freq[target[i] - 'a'] > 0) {
                        freq[target[i] - 'a']--;
                        newMask |= (1 << i);
                    }
                }

                dp[newMask] = min(dp[newMask], dp[mask] + 1);
            }
        }

        return dp[total - 1] == INT_MAX ? -1 : dp[total - 1];
    }
};

// ==================== 解法二：状压 DP + BFS ====================
// 时间 O(2^n * m * n)，空间 O(2^n)
// BFS 天然求最短路径，找到答案即可终止
class Solution_BFS {
public:
    int minStickers(vector<string>& stickers, string target) {
        int n = target.size();
        int total = 1 << n;
        int m = stickers.size();

        vector<vector<int>> sf(m, vector<int>(26, 0));
        for (int i = 0; i < m; i++) {
            for (char c : stickers[i]) sf[i][c - 'a']++;
        }

        // 不可能情况快速判断
        for (char c : target) {
            bool ok = false;
            for (int i = 0; i < m; i++) {
                if (sf[i][c - 'a'] > 0) { ok = true; break; }
            }
            if (!ok) return -1;
        }

        vector<bool> visited(total, false);
        queue<int> q;
        q.push(0);
        visited[0] = true;
        int steps = 0;

        while (!q.empty()) {
            int sz = q.size();
            for (int k = 0; k < sz; k++) {
                int mask = q.front();
                q.pop();

                if (mask == total - 1) return steps;

                // 找第一个未覆盖位
                int firstUnset = 0;
                for (int i = 0; i < n; i++) {
                    if (!(mask & (1 << i))) {
                        firstUnset = i;
                        break;
                    }
                }

                // 枚举包含该字符的贴纸
                for (int s = 0; s < m; s++) {
                    if (sf[s][target[firstUnset] - 'a'] == 0) continue;

                    int newMask = mask;
                    vector<int> freq = sf[s];
                    for (int j = 0; j < n; j++) {
                        if (!(newMask & (1 << j)) && freq[target[j] - 'a'] > 0) {
                            freq[target[j] - 'a']--;
                            newMask |= (1 << j);
                        }
                    }

                    if (!visited[newMask]) {
                        visited[newMask] = true;
                        q.push(newMask);
                    }
                }
            }
            steps++;
        }

        return -1;
    }
};

// ==================== 解法三：记忆化搜索 ====================
// 时间 O(2^n * m * n)，空间 O(2^n)
// 自顶向下，dp[mask] = 从 mask 到全覆盖的最少贴纸数
class Solution_Memo {
public:
    int minStickers(vector<string>& stickers, string target) {
        int n = target.size();
        int total = 1 << n;
        int m = stickers.size();

        vector<vector<int>> sf(m, vector<int>(26, 0));
        for (int i = 0; i < m; i++) {
            for (char c : stickers[i]) sf[i][c - 'a']++;
        }

        // 不可能情况判断
        for (char c : target) {
            bool ok = false;
            for (int i = 0; i < m; i++) {
                if (sf[i][c - 'a'] > 0) { ok = true; break; }
            }
            if (!ok) return -1;
        }

        vector<int> memo(total, -1);
        memo[total - 1] = 0;

        function<int(int)> dp = [&](int mask) -> int {
            if (memo[mask] != -1) return memo[mask];

            int res = INT_MAX;
            int firstUnset = 0;
            for (int i = 0; i < n; i++) {
                if (!(mask & (1 << i))) { firstUnset = i; break; }
            }

            for (int s = 0; s < m; s++) {
                if (sf[s][target[firstUnset] - 'a'] == 0) continue;

                int newMask = mask;
                vector<int> freq = sf[s];
                for (int j = 0; j < n; j++) {
                    if (!(newMask & (1 << j)) && freq[target[j] - 'a'] > 0) {
                        freq[target[j] - 'a']--;
                        newMask |= (1 << j);
                    }
                }

                int sub = dp(newMask);
                if (sub != INT_MAX) {
                    res = min(res, sub + 1);
                }
            }

            memo[mask] = res;
            return res;
        };

        int ans = dp(0);
        return ans == INT_MAX ? -1 : ans;
    }
};

/*
 * 复杂度分析：
 * - 时间：O(2^n * m * n)，n=target长度(<=15), m=贴纸数(<=50)
 *   最坏 32768 * 50 * 15 = 2.4 * 10^7，可以接受
 * - 空间：O(2^n) 用于 dp/visited 数组
 *
 * 关键要点：
 * 1. 状态压缩用 bitmask 表示 target 中每个字符的覆盖情况
 * 2. "第一个未覆盖位"优化：只尝试包含该字符的贴纸，消除排列重复
 * 3. 贴纸支配剪枝：如果贴纸 A 每个字符频次都 >= B，可以移除 B
 * 4. BFS 天然求最短路径，通常比记忆化搜索更快终止
 * 5. 先判断不可能情况（target 中的字符是否全部可覆盖）避免无效搜索
 */
