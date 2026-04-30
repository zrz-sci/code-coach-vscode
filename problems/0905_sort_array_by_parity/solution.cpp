// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 905: Sort Array By Parity
 * Difficulty: Easy
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, move all the even integers at the beginning
 * of
 * the array followed by all the odd integers.
 * 
 * Return _**any array** that satisfies this condition_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,1,2,4]
 *     **Output:** [2,4,3,1]
 * **Explanation:** The outputs [4,2,3,1], [2,4,1,3], and [4,2,1,3] would also
 * be accepted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0]
 *     **Output:** [0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5000`
 *   * `0 <= nums[i] <= 5000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sort-array-by-parity/
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
    vector<int> sortArrayByParity(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,1,2,4]
    // Test case 2: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
