// LeetCode 710: Random Pick with Blacklist — Reference Solution
// 方法：将 [0, M) 中的黑名单数映射到 [M, n) 中的白名单数
// 构造 O(B)，pick O(1)，空间 O(B)

class Solution {
    unordered_map<int, int> mapping;
    int M;  // 有效数字个数 = n - blacklist.size()

public:
    Solution(int n, vector<int>& blacklist) {
        M = n - blacklist.size();

        // 先把所有黑名单数字放入 set
        unordered_set<int> blackSet(blacklist.begin(), blacklist.end());

        // 从 [M, n) 中找不在黑名单的数字，用来做映射目标
        int next = M;
        for (int b : blacklist) {
            if (b < M) {
                // 找一个 [M, n) 中的白名单数字
                while (blackSet.count(next)) {
                    next++;
                }
                mapping[b] = next;
                next++;
            }
        }
    }

    int pick() {
        int r = rand() % M;
        if (mapping.count(r)) {
            return mapping[r];
        }
        return r;
    }
};
