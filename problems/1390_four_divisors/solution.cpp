// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1390: Four Divisors
 * Difficulty: Medium
 * Tags: Array, Math
 *
 * Problem Description:
 * Given an integer array `nums`, return _the sum of divisors of the integers
 * in
 * that array that have exactly four divisors_. If there is no such integer in
 * the
 * array, return `0`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [21,4,7]
 *     **Output:** 32
 *     **Explanation:** 
 *     21 has 4 divisors: 1, 3, 7, 21
 *     4 has 3 divisors: 1, 2, 4
 *     7 has 2 divisors: 1, 7
 *     The answer is the sum of divisors of 21 only.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [21,21]
 *     **Output:** 64
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4,5]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `1 <= nums[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/four-divisors/
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
    int sumFourDivisors(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [21,4,7]
    // Test case 2: [21,21]
    // Test case 3: [1,2,3,4,5]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
