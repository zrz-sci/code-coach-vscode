// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 45: Jump Game II
 * Difficulty: Medium
 * Tags: Greedy, Array, Dynamic Programming
 *
 * Problem Description:
 * You are given a **0-indexed** array of integers `nums` of length `n`. You
 * are
 * initially positioned at index 0.
 * 
 * Each element `nums[i]` represents the maximum length of a forward jump from
 * index `i`. In other words, if you are at index `i`, you can jump to any
 * index
 * `(i + j)` where:
 * 
 *   * `0 <= j <= nums[i]` and
 *   * `i + j < n`
 * 
 * Return _the minimum number of jumps to reach index_`n - 1`. The test cases
 * are
 * generated such that you can reach index `n - 1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,3,1,1,4]
 *     **Output:** 2
 * **Explanation:** The minimum number of jumps to reach the last index is 2.
 * Jump 1 step from index 0 to 1, then 3 steps to the last index.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,3,0,1,4]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `0 <= nums[i] <= 1000`
 *   * It's guaranteed that you can reach `nums[n - 1]`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/jump-game-ii/
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
    int jump(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,3,1,1,4]
    // Test case 2: [2,3,0,1,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
