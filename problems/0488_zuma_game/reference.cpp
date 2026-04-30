// LeetCode 488: Zuma Game - Reference Solution
// 方法: DFS + 剪枝
// 时间: 指数级(数据范围小), 空间: O(状态数)

class Solution {
public:
    int findMinStep(string board, string hand) {
        sort(hand.begin(), hand.end());
        unordered_map<string, int> memo;
        int res = dfs(board, hand, memo);
        return res == INT_MAX ? -1 : res;
    }

private:
    // 消除连续>=3个同色球
    string removeConsecutive(string s) {
        for (int i = 0; i < (int)s.size(); ) {
            int j = i;
            while (j < (int)s.size() && s[j] == s[i]) j++;
            if (j - i >= 3) {
                s = s.substr(0, i) + s.substr(j);
                i = 0; // 重新从头检查（连锁反应）
            } else {
                i = j;
            }
        }
        return s;
    }

    int dfs(string board, string hand, unordered_map<string, int>& memo) {
        if (board.empty()) return 0;
        if (hand.empty()) return INT_MAX;

        string key = board + "#" + hand;
        if (memo.count(key)) return memo[key];

        int res = INT_MAX;
        int i = 0;
        while (i < (int)board.size()) {
            int j = i;
            // 找连续同色段
            while (j < (int)board.size() && board[j] == board[i]) j++;
            int need = 3 - (j - i); // 需要补几个球

            // 在hand中找同色球
            int count = 0;
            for (char c : hand) {
                if (c == board[i]) count++;
            }

            if (count >= need) {
                // 从hand中移除need个同色球
                string newHand = hand;
                int removed = 0;
                for (int k = 0; k < (int)newHand.size() && removed < need; ) {
                    if (newHand[k] == board[i]) {
                        newHand.erase(k, 1);
                        removed++;
                    } else {
                        k++;
                    }
                }
                // 消除并递归
                string newBoard = removeConsecutive(
                    board.substr(0, i) + board.substr(j));
                int sub = dfs(newBoard, newHand, memo);
                if (sub != INT_MAX) {
                    res = min(res, sub + need);
                }
            }
            i = j;
        }

        memo[key] = res;
        return res;
    }
};
