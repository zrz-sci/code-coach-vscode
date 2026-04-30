// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 525: Contiguous Array
 * Difficulty: Medium
 * Tags: Array, Hash Table, Prefix Sum
 *
 * Problem Description:
 * Given a binary array `nums`, return _the maximum length of a contiguous
 * subarray
 * with an equal number of_`0` _and_`1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1]
 *     **Output:** 2
 * **Explanation:** [0, 1] is the longest contiguous subarray with an equal
 * number of 0 and 1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1,0]
 *     **Output:** 2
 * **Explanation:** [0, 1] (or [1, 0]) is a longest contiguous subarray with
 * equal number of 0 and 1.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1,1,1,1,1,0,0,0]
 *     **Output:** 6
 * **Explanation:** [1,1,1,0,0,0] is the longest contiguous subarray with
 * equal number of 0 and 1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `nums[i]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/contiguous-array/
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
    int findMaxLength(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1]
    // Test case 2: [0,1,0]
    // Test case 3: [0,1,1,1,1,1,0,0,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
