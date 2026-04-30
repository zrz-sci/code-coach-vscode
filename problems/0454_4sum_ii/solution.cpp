// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 454: 4Sum II
 * Difficulty: Medium
 * Tags: Array, Hash Table
 *
 * Problem Description:
 * Given four integer arrays `nums1`, `nums2`, `nums3`, and `nums4` all of
 * length
 * `n`, return the number of tuples `(i, j, k, l)` such that:
 * 
 *   * `0 <= i, j, k, l < n`
 *   * `nums1[i] + nums2[j] + nums3[k] + nums4[l] == 0`
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,2], nums2 = [-2,-1], nums3 = [-1,2], nums4 = [0,2]
 *     **Output:** 2
 *     **Explanation:**
 *     The two tuples are:
 * 1. (0, 0, 0, 1) -> nums1[0] + nums2[0] + nums3[0] + nums4[1] = 1 + (-2) +
 * (-1) + 2 = 0
 * 2. (1, 1, 0, 0) -> nums1[1] + nums2[1] + nums3[0] + nums4[0] = 2 + (-1) +
 * (-1) + 0 = 0
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [0], nums2 = [0], nums3 = [0], nums4 = [0]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums1.length`
 *   * `n == nums2.length`
 *   * `n == nums3.length`
 *   * `n == nums4.length`
 *   * `1 <= n <= 200`
 *   * `-228 <= nums1[i], nums2[i], nums3[i], nums4[i] <= 228`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/4sum-ii/
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
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2]
    // Test case 2: [-2,-1]
    // Test case 3: [-1,2]
    // Test case 4: [0,2]
    // Test case 5: [0]
    // Test case 6: [0]
    // Test case 7: [0]
    // Test case 8: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
