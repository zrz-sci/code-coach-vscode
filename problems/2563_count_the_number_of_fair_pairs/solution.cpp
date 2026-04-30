// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 2563: Count the Number of Fair Pairs
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * Given a **0-indexed** integer array `nums` of size `n` and two integers
 * `lower`
 * and `upper`, return _the number of fair pairs_.
 * 
 * A pair `(i, j)` is **fair** if:
 * 
 *   * `0 <= i < j < n`, and
 *   * `lower <= nums[i] + nums[j] <= upper`
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1,7,4,4,5], lower = 3, upper = 6
 *     **Output:** 6
 * **Explanation:** There are 6 fair pairs: (0,3), (0,4), (0,5), (1,3), (1,4),
 * and (1,5).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,7,9,2,5], lower = 11, upper = 11
 *     **Output:** 1
 *     **Explanation:** There is a single fair pair: (2,3).
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `nums.length == n`
 *   * `-109 <= nums[i] <= 109`
 *   * `-109 <= lower <= upper <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-the-number-of-fair-pairs/
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
    long long countFairPairs(vector<int>& nums, int lower, int upper) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,7,4,4,5]
    // Test case 2: 3
    // Test case 3: 6
    // Test case 4: [1,7,9,2,5]
    // Test case 5: 11
    // Test case 6: 11

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
