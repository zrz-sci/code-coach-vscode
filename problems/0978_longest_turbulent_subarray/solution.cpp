// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 978: Longest Turbulent Subarray
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Sliding Window
 *
 * Problem Description:
 * Given an integer array `arr`, return _the length of a maximum size turbulent
 * subarray of_ `arr`.
 * 
 * A subarray is **turbulent** if the comparison sign flips between each
 * adjacent
 * pair of elements in the subarray.
 * 
 * More formally, a subarray `[arr[i], arr[i + 1], ..., arr[j]]` of `arr` is
 * said
 * to be turbulent if and only if:
 * 
 *   * For `i <= k < j`: 
 *     * `arr[k] > arr[k + 1]` when `k` is odd, and
 *     * `arr[k] < arr[k + 1]` when `k` is even.
 *   * Or, for `i <= k < j`: 
 *     * `arr[k] > arr[k + 1]` when `k` is even, and
 *     * `arr[k] < arr[k + 1]` when `k` is odd.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [9,4,2,10,7,8,8,1,9]
 *     **Output:** 5
 *     **Explanation:** arr[1] > arr[2] < arr[3] > arr[4] < arr[5]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [4,8,12,16]
 *     **Output:** 2
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** arr = [100]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= arr.length <= 4 * 104`
 *   * `0 <= arr[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-turbulent-subarray/
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
    int maxTurbulenceSize(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [9,4,2,10,7,8,8,1,9]
    // Test case 2: [4,8,12,16]
    // Test case 3: [100]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
