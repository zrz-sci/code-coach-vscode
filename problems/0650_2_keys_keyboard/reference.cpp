// LeetCode 650: 2 Keys Keyboard
// 解法1: 质因数分解（数学）
// 时间: O(sqrt(n))  空间: O(1)

#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int minSteps(int n) {
        // 答案 = n 的所有质因数之和
        int result = 0;
        for (int factor = 2; factor <= n; factor++) {
            while (n % factor == 0) {
                result += factor;
                n /= factor;
            }
        }
        return result;
    }
};

// 解法2: DP
// 时间: O(n * sqrt(n))  空间: O(n)
class Solution_DP {
public:
    int minSteps(int n) {
        vector<int> dp(n + 1, 0);

        for (int i = 2; i <= n; i++) {
            dp[i] = i;  // 最坏情况：复制1次 + 粘贴(i-1)次
            // 枚举因子 j：先得到 j 个A，再复制+粘贴变成 i 个
            for (int j = i / 2; j >= 2; j--) {
                if (i % j == 0) {
                    dp[i] = dp[j] + i / j;
                    break;  // 找到最大因子即可（它给出最优解）
                }
            }
        }

        return dp[n];
    }
};
