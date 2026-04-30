// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1929: Concatenation of Array
 * Difficulty: Easy
 * Tags: Array, Simulation
 *
 * Problem Description:
 * Given an integer array `nums` of length `n`, you want to create an array
 * `ans`
 * of length `2n` where `ans[i] == nums[i]` and `ans[i + n] == nums[i]` for `0
 * <= i
 * < n` (**0-indexed**).
 * 
 * Specifically, `ans` is the **concatenation** of two `nums` arrays.
 * 
 * Return _the array_`ans`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1]
 *     **Output:** [1,2,1,1,2,1]
 *     **Explanation:** The array ans is formed as follows:
 *     - ans = [nums[0],nums[1],nums[2],nums[0],nums[1],nums[2]]
 *     - ans = [1,2,1,1,2,1]
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,2,1]
 *     **Output:** [1,3,2,1,1,3,2,1]
 *     **Explanation:** The array ans is formed as follows:
 * - ans = [nums[0],nums[1],nums[2],nums[3],nums[0],nums[1],nums[2],nums[3]]
 *     - ans = [1,3,2,1,1,3,2,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 1000`
 *   * `1 <= nums[i] <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/concatenation-of-array/
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
    vector<int> getConcatenation(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,1]
    // Test case 2: [1,3,2,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
