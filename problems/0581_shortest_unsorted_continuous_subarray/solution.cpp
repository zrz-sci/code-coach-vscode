// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 581: Shortest Unsorted Continuous Subarray
 * Difficulty: Medium
 * Tags: Stack, Greedy, Array, Two Pointers, Sorting, Monotonic Stack
 *
 * Problem Description:
 * Given an integer array `nums`, you need to find one **continuous subarray**
 * such
 * that if you only sort this subarray in non-decreasing order, then the whole
 * array will be sorted in non-decreasing order.
 * 
 * Return _the shortest such subarray and output its length_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,6,4,8,10,9,15]
 *     **Output:** 5
 * **Explanation:** You need to sort [6, 4, 8, 10, 9] in ascending order to
 * make the whole array sorted in ascending order.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4]
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-105 <= nums[i] <= 105`
 * 
 * 
 * 
 * **Follow up:** Can you solve it in `O(n)` time complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-unsorted-continuous-subarray/
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
    int findUnsortedSubarray(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,6,4,8,10,9,15]
    // Test case 2: [1,2,3,4]
    // Test case 3: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
