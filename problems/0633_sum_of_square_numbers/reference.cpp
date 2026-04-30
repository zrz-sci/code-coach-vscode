// LeetCode 633: Sum of Square Numbers
// 方法：双指针
// 时间复杂度: O(sqrt(c)), 空间复杂度: O(1)

class Solution {
public:
    bool judgeSquareSum(int c) {
        long long left = 0, right = (long long)sqrt(c);
        while (left <= right) {
            long long sum = left * left + right * right;
            if (sum == c) {
                return true;
            } else if (sum < c) {
                left++;
            } else {
                right--;
            }
        }
        return false;
    }
};
