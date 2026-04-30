// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 321: Create Maximum Number
 * Difficulty: Hard
 * Tags: Stack, Greedy, Array, Two Pointers, Monotonic Stack
 *
 * Problem Description:
 * You are given two integer arrays `nums1` and `nums2` of lengths `m` and `n`
 * respectively. `nums1` and `nums2` represent the digits of two numbers. You
 * are
 * also given an integer `k`.
 * 
 * Create the maximum number of length `k <= m + n` from digits of the two
 * numbers.
 * The relative order of the digits from the same array must be preserved.
 * 
 * Return an array of the `k` digits representing the answer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [3,4,6,5], nums2 = [9,1,2,5,8,3], k = 5
 *     **Output:** [9,8,6,5,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [6,7], nums2 = [6,0,4], k = 5
 *     **Output:** [6,7,6,0,4]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums1 = [3,9], nums2 = [8,9], k = 3
 *     **Output:** [9,8,9]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == nums1.length`
 *   * `n == nums2.length`
 *   * `1 <= m, n <= 500`
 *   * `0 <= nums1[i], nums2[i] <= 9`
 *   * `1 <= k <= m + n`
 *   * `nums1` and `nums2` do not have leading zeros.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/create-maximum-number/
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
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,4,6,5]
    // Test case 2: [9,1,2,5,8,3]
    // Test case 3: 5
    // Test case 4: [6,7]
    // Test case 5: [6,0,4]
    // Test case 6: 5
    // Test case 7: [3,9]
    // Test case 8: [8,9]
    // Test case 9: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
