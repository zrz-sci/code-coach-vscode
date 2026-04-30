// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1262: Greatest Sum Divisible by Three
 * Difficulty: Medium
 * Tags: Greedy, Array, Dynamic Programming, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, return _the**maximum possible sum** of
 * elements
 * of the array such that it is divisible by three_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,6,5,1,8]
 *     **Output:** 18
 * **Explanation:** Pick numbers 3, 6, 1 and 8 their sum is 18 (maximum sum
 * divisible by 3).
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [4]
 *     **Output:** 0
 *     **Explanation:** Since 4 is not divisible by 3, do not pick any number.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4,4]
 *     **Output:** 12
 * **Explanation:** Pick numbers 1, 3, 4 and 4 their sum is 12 (maximum sum
 * divisible by 3).
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 4 * 104`
 *   * `1 <= nums[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/greatest-sum-divisible-by-three/
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
    int maxSumDivThree(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,6,5,1,8]
    // Test case 2: [4]
    // Test case 3: [1,2,3,4,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
