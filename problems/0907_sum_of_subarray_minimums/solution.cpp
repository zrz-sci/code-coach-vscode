// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 907: Sum of Subarray Minimums
 * Difficulty: Medium
 * Tags: Stack, Array, Dynamic Programming, Monotonic Stack
 *
 * Problem Description:
 * Given an array of integers arr, find the sum of `min(b)`, where `b` ranges
 * over
 * every (contiguous) subarray of `arr`. Since the answer may be large, return
 * the
 * answer **modulo** `109 + 7`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [3,1,2,4]
 *     **Output:** 17
 *     **Explanation:** 
 * Subarrays are [3], [1], [2], [4], [3,1], [1,2], [2,4], [3,1,2], [1,2,4],
 * [3,1,2,4].
 *     Minimums are 3, 1, 2, 4, 1, 1, 2, 1, 1, 1.
 *     Sum is 17.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [11,81,94,43,3]
 *     **Output:** 444
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= arr.length <= 3 * 104`
 *   * `1 <= arr[i] <= 3 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sum-of-subarray-minimums/
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
    int sumSubarrayMins(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,1,2,4]
    // Test case 2: [11,81,94,43,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
