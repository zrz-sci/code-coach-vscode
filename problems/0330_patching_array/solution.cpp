// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 330: Patching Array
 * Difficulty: Hard
 * Tags: Greedy, Array
 *
 * Problem Description:
 * Given a sorted integer array `nums` and an integer `n`, add/patch elements
 * to
 * the array such that any number in the range `[1, n]` inclusive can be
 * formed by
 * the sum of some elements in the array.
 * 
 * Return _the minimum number of patches required_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3], n = 6
 *     **Output:** 1
 *     Explanation:
 * Combinations of nums are [1], [3], [1,3], which form possible sums of: 1,
 * 3, 4.
 * Now if we add/patch 2 to nums, the combinations are: [1], [2], [3], [1,3],
 * [2,3], [1,2,3].
 *     Possible sums are 1, 2, 3, 4, 5, 6, which now covers the range [1, 6].
 *     So we only need 1 patch.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,5,10], n = 20
 *     **Output:** 2
 *     Explanation: The two patches can be [2, 4].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,2], n = 5
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 1000`
 *   * `1 <= nums[i] <= 104`
 *   * `nums` is sorted in **ascending order**.
 *   * `1 <= n <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/patching-array/
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
    int minPatches(vector<int>& nums, int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3]
    // Test case 2: 6
    // Test case 3: [1,5,10]
    // Test case 4: 20
    // Test case 5: [1,2,2]
    // Test case 6: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
