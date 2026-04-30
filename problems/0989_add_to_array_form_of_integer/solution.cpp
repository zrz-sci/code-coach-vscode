// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 989: Add to Array-Form of Integer
 * Difficulty: Easy
 * Tags: Array, Math
 *
 * Problem Description:
 * The **array-form** of an integer `num` is an array representing its digits
 * in
 * left to right order.
 * 
 *   * For example, for `num = 1321`, the array form is `[1,3,2,1]`.
 * 
 * Given `num`, the **array-form** of an integer, and an integer `k`, return
 * _the**array-form** of the integer_ `num + k`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** num = [1,2,0,0], k = 34
 *     **Output:** [1,2,3,4]
 *     **Explanation:** 1200 + 34 = 1234
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** num = [2,7,4], k = 181
 *     **Output:** [4,5,5]
 *     **Explanation:** 274 + 181 = 455
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** num = [2,1,5], k = 806
 *     **Output:** [1,0,2,1]
 *     **Explanation:** 215 + 806 = 1021
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= num.length <= 104`
 *   * `0 <= num[i] <= 9`
 *   * `num` does not contain any leading zeros except for the zero itself.
 *   * `1 <= k <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/add-to-array-form-of-integer/
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
    vector<int> addToArrayForm(vector<int>& num, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,0,0]
    // Test case 2: 34
    // Test case 3: [2,7,4]
    // Test case 4: 181
    // Test case 5: [2,1,5]
    // Test case 6: 806

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
