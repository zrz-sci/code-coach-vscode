// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 3721: Longest Balanced Subarray II
 * Difficulty: Hard
 * Tags: Segment Tree, Array, Hash Table, Divide and Conquer, Prefix Sum
 *
 * Problem Description:
 * You are given an integer array `nums`.
 * 
 * A **subarray** is called **balanced** if the number of **distinct even**
 * numbers
 * in the subarray is equal to the number of **distinct odd** numbers.
 * 
 * Return the length of the **longest** balanced subarray.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** nums = [2,5,4,3]
 * 
 * **Output:** 4
 * 
 * **Explanation:**
 * 
 *   * The longest balanced subarray is `[2, 5, 4, 3]`.
 * * It has 2 distinct even numbers `[2, 4]` and 2 distinct odd numbers `[5,
 * 3]`. Thus, the answer is 4.
 * 
 * **Example 2:**
 * 
 * **Input:** nums = [3,2,2,5,4]
 * 
 * **Output:** 5
 * 
 * **Explanation:**
 * 
 *   * The longest balanced subarray is `[3, 2, 2, 5, 4]`.
 * * It has 2 distinct even numbers `[2, 4]` and 2 distinct odd numbers `[3,
 * 5]`. Thus, the answer is 5.
 * 
 * **Example 3:**
 * 
 * **Input:** nums = [1,2,3,2]
 * 
 * **Output:** 3
 * 
 * **Explanation:**
 * 
 *   * The longest balanced subarray is `[2, 3, 2]`.
 * * It has 1 distinct even number `[2]` and 1 distinct odd number `[3]`.
 * Thus, the answer is 3.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `1 <= nums[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-balanced-subarray-ii/
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
    int longestBalanced(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,5,4,3]
    // Test case 2: [3,2,2,5,4]
    // Test case 3: [1,2,3,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
