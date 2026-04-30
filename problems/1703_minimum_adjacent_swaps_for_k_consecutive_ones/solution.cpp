// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1703: Minimum Adjacent Swaps for K Consecutive Ones
 * Difficulty: Hard
 * Tags: Greedy, Array, Prefix Sum, Sliding Window
 *
 * Problem Description:
 * You are given an integer array, `nums`, and an integer `k`. `nums`
 * comprises of
 * only `0`'s and `1`'s. In one move, you can choose two **adjacent** indices
 * and
 * swap their values.
 * 
 * Return _the**minimum** number of moves required so that _`nums` _has_`k`
 * _**consecutive** _`1` _' s_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,0,1,0,1], k = 2
 *     **Output:** 1
 * **Explanation:** In 1 move, nums could be [1,0,0,0,_1_ ,_1_] and have 2
 * consecutive 1's.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,0,0,0,0,1,1], k = 3
 *     **Output:** 5
 * **Explanation:** In 5 moves, the leftmost 1 can be shifted right until nums
 * = [0,0,0,0,0,_1_ ,_1_ ,_1_].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,0,1], k = 2
 *     **Output:** 0
 *     **Explanation:** nums already has 2 consecutive 1's.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `nums[i]` is `0` or `1`.
 *   * `1 <= k <= sum(nums)`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-adjacent-swaps-for-k-consecutive-ones/
 */


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

class Solution {
public:
    int minMoves(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,0,1,0,1]
    // Test case 2: 2
    // Test case 3: [1,0,0,0,0,0,1,1]
    // Test case 4: 3
    // Test case 5: [1,1,0,1]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
