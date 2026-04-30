// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 75: Sort Colors
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * Given an array `nums` with `n` objects colored red, white, or blue, sort
 * them
 * **[in-place](https://en.wikipedia.org/wiki/In-place_algorithm) **so that
 * objects
 * of the same color are adjacent, with the colors in the order red, white, and
 * blue.
 * 
 * We will use the integers `0`, `1`, and `2` to represent the color red,
 * white,
 * and blue, respectively.
 * 
 * You must solve this problem without using the library's sort function.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,0,2,1,1,0]
 *     **Output:** [0,0,1,1,2,2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,0,1]
 *     **Output:** [0,1,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 300`
 *   * `nums[i]` is either `0`, `1`, or `2`.
 * 
 * 
 * 
 * **Follow up:** Could you come up with a one-pass algorithm using only
 * constant
 * extra space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sort-colors/
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
    void sortColors(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,0,2,1,1,0]
    // Test case 2: [2,0,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
