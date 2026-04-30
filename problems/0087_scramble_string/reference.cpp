/*
 * LeetCode 87: 扰乱字符串 (Scramble String)
 *
 * 【题目本质】
 * 判断 s2 是否可以通过对 s1 进行递归的"分割+可选交换"操作得到。
 * 扰乱过程形成一棵二叉树：每个节点选一个分割点，选择是否交换左右子树。
 *
 * 【解法总览】
 * 解法1: 记忆化递归     — O(n^4) / O(n^3)  — 自顶向下，直觉清晰
 * 解法2: 三维DP         — O(n^4) / O(n^3)  — 自底向上，效率更高 ⭐面试首选
 */

// ============================================================
// 解法1: 记忆化递归 — 自顶向下搜索 + 字符频次剪枝
// 时间: O(n^4)  空间: O(n^3)
//
// 【思路】
// 直接按题意递归：对于 s1 和 s2 的等长子串，枚举分割点 k:
//   - 不交换: s1前k → s2前k, s1后(n-k) → s2后(n-k)
//   - 交换:   s1前k → s2后k, s1后(n-k) → s2前(n-k)
// 任意一种分割方式成立就返回 true。
//
// 剪枝: 先检查两个子串的字符频次是否相同，不同则直接 false。
//
// 递归树示意 (s1="great", s2="rgeat"):
//
//   isScramble("great", "rgeat")
//   ├── k=1: 不交换 → ("g","r") 频次不同 → skip
//   │         交换   → ("g","t") 频次不同 → skip
//   ├── k=2: 不交换 → isScramble("gr","rg") && isScramble("eat","eat")
//   │                  ├── "gr" vs "rg": k=1 交换 → ("g","g")✓ && ("r","r")✓ → true!
//   │                  └── "eat" vs "eat": 相等 → true!
//   │         → 两个都 true → 返回 true!
//   └── (后面的分割点不用再试)
//
// ============================================================
class Solution1 {
public:
    unordered_map<string, bool> memo;

    bool isScramble(string s1, string s2) {
        // base case: 完全相同
        if (s1 == s2) return true;
        if (s1.size() != s2.size()) return false;

        // 检查缓存
        string key = s1 + "#" + s2;
        if (memo.count(key)) return memo[key];

        int n = s1.size();

        // 字符频次剪枝: 频次不同不可能是扰乱关系
        // 例: s1="abc" vs s2="abd" → 'c'和'd'频次不同 → false
        vector<int> cnt(26, 0);
        for (int i = 0; i < n; i++) {
            cnt[s1[i] - 'a']++;
            cnt[s2[i] - 'a']--;
        }
        for (int c : cnt) {
            if (c != 0) return memo[key] = false;
        }

        // 枚举分割点 k: 左段长度 k, 右段长度 n-k
        for (int k = 1; k < n; k++) {
            // 情况1: 不交换
            // s1[0..k-1] ↔ s2[0..k-1], s1[k..n-1] ↔ s2[k..n-1]
            if (isScramble(s1.substr(0, k), s2.substr(0, k)) &&
                isScramble(s1.substr(k), s2.substr(k)))
                return memo[key] = true;

            // 情况2: 交换
            // s1[0..k-1] ↔ s2[n-k..n-1], s1[k..n-1] ↔ s2[0..n-k-1]
            //
            // 关键: 交换后 s1 的前 k 个对应 s2 的 **后** k 个
            //       所以 s2 的起点是 n-k, 不是 k!
            if (isScramble(s1.substr(0, k), s2.substr(n - k)) &&
                isScramble(s1.substr(k), s2.substr(0, n - k)))
                return memo[key] = true;
        }

        return memo[key] = false;
    }
};

// ============================================================
// 解法2: 三维 DP — dp[i][j][len] 自底向上 ⭐面试首选
// 时间: O(n^4)  空间: O(n^3)
//
// 【思路】
// 将记忆化递归的三个参数 (s1起点i, s2起点j, 长度len) 变成 DP 数组维度。
//
// 状态定义:
//   dp[i][j][len] = true 表示 s1[i..i+len-1] 和 s2[j..j+len-1] 是扰乱关系
//
// 状态转移 (枚举分割点 k, 1 <= k < len):
//   dp[i][j][len] |= dp[i][j][k]         && dp[i+k][j+k][len-k]     (不交换)
//   dp[i][j][len] |= dp[i][j+len-k][k]   && dp[i+k][j][len-k]       (交换)
//
// 交换情况下标推导 (最易出错的部分):
//
//   s1:  [ i ........... i+k-1 | i+k ......... i+len-1 ]
//         ←───── k ─────→      ←────── len-k ──────→
//
//   不交换:
//   s2:  [ j ........... j+k-1 | j+k ......... j+len-1 ]
//         ←───── k ─────→      ←────── len-k ──────→
//
//   交换 (s1的前k个对应s2的后k个):
//   s2:  [ j ........ j+len-k-1 | j+len-k ..... j+len-1 ]
//         ←──── len-k ────→      ←───── k ─────→
//
//   所以: s1前k → s2[j+len-k .. j+len-1]  即 dp[i][j+len-k][k]
//         s1后(len-k) → s2[j .. j+len-k-1] 即 dp[i+k][j][len-k]
//
// Base case:
//   dp[i][j][1] = (s1[i] == s2[j])
//
// 填表顺序: len 从 2 到 n (外层), i 和 j 内层
//
// DP 填表过程示意 (s1="ab", s2="ba"):
//
//   len=1:
//     dp[0][0][1] = ('a'=='b') = F    dp[0][1][1] = ('a'=='a') = T
//     dp[1][0][1] = ('b'=='b') = T    dp[1][1][1] = ('b'=='a') = F
//
//   len=2 (k只能=1):
//     dp[0][0][2]:
//       不交换: dp[0][0][1] && dp[1][1][1] = F && F = F
//       交换:   dp[0][0+2-1][1] && dp[0+1][0][1]
//             = dp[0][1][1] && dp[1][0][1]
//             = T && T = T  ← 找到了! "ab"交换得到"ba"
//     → dp[0][0][2] = true
//
// ============================================================
class Solution2 {
public:
    bool isScramble(string s1, string s2) {
        int n = s1.size();
        if (n != (int)s2.size()) return false;
        if (s1 == s2) return true;

        // dp[i][j][len]: s1[i..i+len-1] 和 s2[j..j+len-1] 是否扰乱
        vector<vector<vector<bool>>> dp(
            n, vector<vector<bool>>(n, vector<bool>(n + 1, false)));

        // Base case: 长度为 1, 单字符直接比较
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dp[i][j][1] = (s1[i] == s2[j]);
            }
        }

        // 按长度从小到大填表
        // 为什么? 因为 dp[...][...][k] 和 dp[...][...][len-k] 都 < len
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                for (int j = 0; j <= n - len; j++) {
                    // 字符频次剪枝: 先检查再枚举分割点
                    // 这个剪枝虽然不改变渐近复杂度，但实测避免了大量无用计算
                    vector<int> cnt(26, 0);
                    for (int p = 0; p < len; p++) {
                        cnt[s1[i + p] - 'a']++;
                        cnt[s2[j + p] - 'a']--;
                    }
                    bool valid = true;
                    for (int c : cnt) {
                        if (c != 0) { valid = false; break; }
                    }
                    if (!valid) continue;  // 字符不同，跳过

                    // 枚举分割点 k
                    for (int k = 1; k < len; k++) {
                        // 不交换
                        if (dp[i][j][k] && dp[i + k][j + k][len - k]) {
                            dp[i][j][len] = true;
                            break;  // 找到一种就够了
                        }
                        // 交换: s1前k ↔ s2后k
                        if (dp[i][j + len - k][k] && dp[i + k][j][len - k]) {
                            dp[i][j][len] = true;
                            break;
                        }
                    }
                }
            }
        }

        return dp[0][0][n];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间    | 空间    | 面试推荐 |
// |-------------|---------|---------|---------|
// | 记忆化递归   | O(n^4) | O(n^3) | ⭐⭐      |
// | 三维 DP     | O(n^4) | O(n^3) | ⭐⭐⭐    |
//
// 两种方法时间/空间复杂度相同。
// 记忆化递归: 只计算实际需要的状态（稀疏），但 substr 开销大。
// 三维 DP: 填满整个表（密集），但无递归开销和字符串开销，常数更小。
//
// ============================================================
// 【易错点】
//
// 1. 交换情况下 s2 子串的起点:
//    ✗ dp[i][j+k][k] && dp[i+k][j][len-k]   — j+k 是错的!
//    ✓ dp[i][j+len-k][k] && dp[i+k][j][len-k]
//    交换后 s1 的前 k 个对应 s2 的 **最后** k 个，起点是 j+len-k
//
// 2. DP 外层循环顺序:
//    ✗ 先枚举 i, j，再枚举 len
//    ✓ 最外层枚举 len 从 2 到 n
//    dp[...][...][k] 和 dp[...][...][len-k] 的长度都 < len
//
// 3. 字符频次剪枝不可省:
//    虽然不影响渐近复杂度，但实际测试中没有剪枝会 TLE
//    对于 n=30, O(n^4) = 810000 个状态，字符串操作的常数使得需要剪枝
//
// 4. 找到一个成立的 k 就 break:
//    只需要存在一种分割方式使得扰乱成立即可
//    不 break 不影响正确性，但会做无用计算
//
// ============================================================
// 【面试追问 Interview Follow-ups】
//
// Q1: 时间复杂度怎么算?
// A1: O(n^3) 个状态 × O(n) 枚举分割点 = O(n^4)。n<=30, 约 81 万操作。
//
// Q2: 能否优化到低于 O(n^4)?
// A2: 目前已知的多项式算法都是 O(n^4) 量级。字符频次剪枝可以大幅
//     减少实际计算量，但不改变 worst case。
//
// Q3: 记忆化递归 vs 三维 DP 的权衡?
// A3: 记忆化只算 reachable states（可能远少于 n^3），但每次 substr
//     产生新字符串有开销。DP 填满整个表但纯数组操作更快。
//     面试中先讲递归思路，再写 DP 代码。
//
// Q4: 这道题和 LeetCode 241 有什么联系?
// A4: 都是"枚举分割点 + 左右两段分别递归"的分治结构。
//     241 枚举运算符作为分割点，87 枚举任意位置作为分割点。
// ============================================================
