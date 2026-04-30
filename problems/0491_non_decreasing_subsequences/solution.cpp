// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 491: Non-decreasing Subsequences
 * Difficulty: Medium
 * Tags: Bit Manipulation, Array, Hash Table, Backtracking
 *
 * Problem Description:
 * Given an integer array `nums`, return _all the different possible
 * non-decreasing
 * subsequences of the given array with at least two elements_. You may return
 * the
 * answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,6,7,7]
 *     **Output:** [[4,6],[4,6,7],[4,6,7,7],[4,7],[4,7,7],[6,7],[6,7,7],[7,7]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [4,4,3,2,1]
 *     **Output:** [[4,4]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 15`
 *   * `-100 <= nums[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/non-decreasing-subsequences/
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
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,6,7,7]
    // Test case 2: [4,4,3,2,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
