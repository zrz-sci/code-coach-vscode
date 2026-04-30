// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 932: Beautiful Array
 * Difficulty: Medium
 * Tags: Array, Math, Divide and Conquer
 *
 * Problem Description:
 * An array `nums` of length `n` is **beautiful** if:
 * 
 *   * `nums` is a permutation of the integers in the range `[1, n]`.
 * * For every `0 <= i < j < n`, there is no index `k` with `i < k < j` where
 * `2 * nums[k] == nums[i] + nums[j]`.
 * 
 * Given the integer `n`, return _any**beautiful** array _`nums` _of
 * length_`n`.
 * There will be at least one valid answer for the given `n`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 4
 *     **Output:** [2,1,4,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 5
 *     **Output:** [3,1,2,5,4]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/beautiful-array/
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
    vector<int> beautifulArray(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 4
    // Test case 2: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
