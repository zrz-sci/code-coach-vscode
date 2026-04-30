// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 594: Longest Harmonious Subsequence
 * Difficulty: Easy
 * Tags: Array, Hash Table, Counting, Sorting, Sliding Window
 *
 * Problem Description:
 * We define a harmonious array as an array where the difference between its
 * maximum value and its minimum value is **exactly** `1`.
 * 
 * Given an integer array `nums`, return the length of its longest harmonious
 * subsequence among all its possible subsequences.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** nums = [1,3,2,2,5,2,3,7]
 * 
 * **Output:** 5
 * 
 * **Explanation:**
 * 
 * The longest harmonious subsequence is `[3,2,2,2,3]`.
 * 
 * **Example 2:**
 * 
 * **Input:** nums = [1,2,3,4]
 * 
 * **Output:** 2
 * 
 * **Explanation:**
 * 
 * The longest harmonious subsequences are `[1,2]`, `[2,3]`, and `[3,4]`, all
 * of
 * which have a length of 2.
 * 
 * **Example 3:**
 * 
 * **Input:** nums = [1,1,1,1]
 * 
 * **Output:** 0
 * 
 * **Explanation:**
 * 
 * No harmonic subsequence exists.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 104`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-harmonious-subsequence/
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
    int findLHS(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,2,2,5,2,3,7]
    // Test case 2: [1,2,3,4]
    // Test case 3: [1,1,1,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
