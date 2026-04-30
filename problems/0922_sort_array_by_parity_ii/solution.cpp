// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 922: Sort Array By Parity II
 * Difficulty: Easy
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * Given an array of integers `nums`, half of the integers in `nums` are
 * **odd** ,
 * and the other half are **even**.
 * 
 * Sort the array so that whenever `nums[i]` is odd, `i` is **odd** , and
 * whenever
 * `nums[i]` is even, `i` is **even**.
 * 
 * Return _any answer array that satisfies this condition_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,2,5,7]
 *     **Output:** [4,5,2,7]
 * **Explanation:** [4,7,2,5], [2,5,4,7], [2,7,4,5] would also have been
 * accepted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,3]
 *     **Output:** [2,3]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 2 * 104`
 *   * `nums.length` is even.
 *   * Half of the integers in `nums` are even.
 *   * `0 <= nums[i] <= 1000`
 * 
 * 
 * 
 * **Follow Up:** Could you solve it in-place?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sort-array-by-parity-ii/
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
    vector<int> sortArrayByParityII(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,5,7]
    // Test case 2: [2,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
