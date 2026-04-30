// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 300: Longest Increasing Subsequence
 * Difficulty: Medium
 * Tags: Array, Binary Search, Dynamic Programming
 *
 * Problem Description:
 * Given an integer array `nums`, return _the length of the longest**strictly
 * increasing**_ _**subsequence**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [10,9,2,5,3,7,101,18]
 *     **Output:** 4
 * **Explanation:** The longest increasing subsequence is [2,3,7,101],
 * therefore the length is 4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1,0,3,2,3]
 *     **Output:** 4
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [7,7,7,7,7,7,7]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2500`
 *   * `-104 <= nums[i] <= 104`
 * 
 * 
 * 
 * **Follow up:**  Can you come up with an algorithm that runs in `O(n log(n))`
 * time complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-increasing-subsequence/
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
    int lengthOfLIS(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,9,2,5,3,7,101,18]
    // Test case 2: [0,1,0,3,2,3]
    // Test case 3: [7,7,7,7,7,7,7]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
