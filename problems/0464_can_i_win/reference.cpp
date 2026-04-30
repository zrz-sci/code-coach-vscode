// 【题目本质】博弈论 + 状态压缩：判断先手在最优策略下能否赢
// 【解法总览】解法1: Bitmask+记忆化(map) O(2^n*n)/O(2^n) | 解法2: Bitmask+记忆化(vector) O(2^n*n)/O(2^n)

// ===================== Solution 1: Bitmask 记忆化搜索 (unordered_map) =====================
// 思路：用 bitmask 表示已选数字集合，递归判断当前玩家能否赢
//       选数 i >= remaining 直接赢，否则对手输则我赢
// 时间 O(2^n * n)，空间 O(2^n)
class Solution1 {
public:
    unordered_map<int, bool> memo;

    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        if (desiredTotal <= 0) return true;
        int sum = maxChoosableInteger * (maxChoosableInteger + 1) / 2;
        if (sum < desiredTotal) return false;
        return canWin(maxChoosableInteger, desiredTotal, 0);
    }

    bool canWin(int n, int remaining, int used) {
        if (memo.count(used)) return memo[used];
        for (int i = 1; i <= n; i++) {
            if (used & (1 << i)) continue;
            if (i >= remaining || !canWin(n, remaining - i, used | (1 << i))) {
                return memo[used] = true;
            }
        }
        return memo[used] = false;
    }
};

// ===================== Solution 2: Bitmask 记忆化搜索 (vector) =====================
// 思路：用 vector<char> 代替 map，0=未算 1=赢 2=输，性能更好
// 时间 O(2^n * n)，空间 O(2^n)
class Solution2 {
public:
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        if (desiredTotal <= 0) return true;
        int n = maxChoosableInteger;
        int sum = n * (n + 1) / 2;
        if (sum < desiredTotal) return false;

        vector<char> memo(1 << (n + 1), 0); // 0=unvisited, 1=win, 2=lose
        return canWin(n, desiredTotal, 0, memo);
    }

    bool canWin(int n, int remaining, int used, vector<char>& memo) {
        if (memo[used] != 0) return memo[used] == 1;
        for (int i = 1; i <= n; i++) {
            if (used & (1 << i)) continue;
            if (i >= remaining || !canWin(n, remaining - i, used | (1 << i), memo)) {
                memo[used] = 1;
                return true;
            }
        }
        memo[used] = 2;
        return false;
    }
};

// 【解法对比】
// Solution1: unordered_map 备忘录，代码简洁，哈希有开销
// Solution2: vector 备忘录，cache-friendly，实测更快

// 【易错点】
// 1. 忘记 sum < desiredTotal 的剪枝 -> 所有数用完都不够，返回 false
// 2. desiredTotal <= 0 时返回 false -> 应返回 true（不选就赢）
// 3. bitmask 位移写错：(1 << i) 从 1 开始，不是 0
// 4. 递归时 remaining 没减去当前选择

// 【面试追问】
// Q1: 状态数 2^20 = 1M，是否可接受？-> 可以，每状态 O(20) 遍历
// Q2: 为什么不需要记录谁的回合？-> bitmask 中 1 的个数隐含了轮次
// Q3: 如何优化常数？-> vector 代替 map；或对称性剪枝（但此题无对称性）
