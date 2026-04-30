// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 718: Maximum Length of Repeated Subarray
 * Difficulty: Medium
 * Tags: Array, Binary Search, Dynamic Programming, Sliding Window, Hash Function, Rolling Hash
 *
 * Problem Description:
 * Given two integer arrays `nums1` and `nums2`, return _the maximum length of
 * a
 * subarray that appears in**both** arrays_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,2,3,2,1], nums2 = [3,2,1,4,7]
 *     **Output:** 3
 *     **Explanation:** The repeated subarray with maximum length is [3,2,1].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [0,0,0,0,0], nums2 = [0,0,0,0,0]
 *     **Output:** 5
 * **Explanation:** The repeated subarray with maximum length is [0,0,0,0,0].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums1.length, nums2.length <= 1000`
 *   * `0 <= nums1[i], nums2[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-length-of-repeated-subarray/
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
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,2,1]
    // Test case 2: [3,2,1,4,7]
    // Test case 3: [0,0,0,0,0]
    // Test case 4: [0,0,0,0,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
