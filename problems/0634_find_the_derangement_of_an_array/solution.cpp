// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 634: Find the Derangement of An Array
 * Difficulty: Medium
 * Tags: Math, Dynamic Programming, Combinatorics
 *
 * Problem Description:
 * In combinatorial mathematics, a **derangement** is a permutation of the
 * elements
 * of a set, such that no element appears in its original position.
 * 
 * You are given an integer `n`. There is originally an array consisting of `n`
 * integers from `1` to `n` in ascending order, return _the number
 * of**derangements** it can generate_. Since the answer may be huge, return it
 * **modulo** `109 + 7`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 2
 * **Explanation:** The original array is [1,2,3]. The two derangements are
 * [2,3,1] and [3,1,2].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 2
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-the-derangement-of-an-array/
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
    int findDerangement(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
