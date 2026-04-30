// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 338: Counting Bits
 * Difficulty: Easy
 * Tags: Bit Manipulation, Dynamic Programming
 *
 * Problem Description:
 * Given an integer `n`, return _an array_`ans` _of length_`n + 1` _such that
 * for
 * each_`i` __(`0 <= i <= n`)_,_`ans[i]`_is the**number of**_`1` _**' s** in
 * the
 * binary representation of _`i`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 2
 *     **Output:** [0,1,1]
 *     **Explanation:**
 *     0 --> 0
 *     1 --> 1
 *     2 --> 10
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 5
 *     **Output:** [0,1,1,2,1,2]
 *     **Explanation:**
 *     0 --> 0
 *     1 --> 1
 *     2 --> 10
 *     3 --> 11
 *     4 --> 100
 *     5 --> 101
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= n <= 105`
 * 
 * 
 * 
 * **Follow up:**
 * 
 * * It is very easy to come up with a solution with a runtime of `O(n log
 * n)`. Can you do it in linear time `O(n)` and possibly in a single pass?
 * * Can you do it without using any built-in function (i.e., like
 * `__builtin_popcount` in C++)?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/counting-bits/
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
    vector<int> countBits(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2
    // Test case 2: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
