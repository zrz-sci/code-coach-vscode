// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 66: Plus One
 * Difficulty: Easy
 * Tags: Array, Math
 *
 * Problem Description:
 * You are given a **large integer** represented as an integer array `digits`,
 * where each `digits[i]` is the `ith` digit of the integer. The digits are
 * ordered
 * from most significant to least significant in left-to-right order. The large
 * integer does not contain any leading `0`'s.
 * 
 * Increment the large integer by one and return _the resulting array of
 * digits_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** digits = [1,2,3]
 *     **Output:** [1,2,4]
 *     **Explanation:** The array represents the integer 123.
 *     Incrementing by one gives 123 + 1 = 124.
 *     Thus, the result should be [1,2,4].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** digits = [4,3,2,1]
 *     **Output:** [4,3,2,2]
 *     **Explanation:** The array represents the integer 4321.
 *     Incrementing by one gives 4321 + 1 = 4322.
 *     Thus, the result should be [4,3,2,2].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** digits = [9]
 *     **Output:** [1,0]
 *     **Explanation:** The array represents the integer 9.
 *     Incrementing by one gives 9 + 1 = 10.
 *     Thus, the result should be [1,0].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= digits.length <= 100`
 *   * `0 <= digits[i] <= 9`
 *   * `digits` does not contain any leading `0`'s.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/plus-one/
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
    vector<int> plusOne(vector<int>& digits) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [4,3,2,1]
    // Test case 3: [9]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
