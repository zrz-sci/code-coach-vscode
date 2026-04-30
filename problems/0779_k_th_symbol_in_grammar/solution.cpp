// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 779: K-th Symbol in Grammar
 * Difficulty: Medium
 * Tags: Bit Manipulation, Recursion, Math
 *
 * Problem Description:
 * We build a table of `n` rows (**1-indexed**). We start by writing `0` in the
 * `1st` row. Now in every subsequent row, we look at the previous row and
 * replace
 * each occurrence of `0` with `01`, and each occurrence of `1` with `10`.
 * 
 * * For example, for `n = 3`, the `1st` row is `0`, the `2nd` row is `01`,
 * and the `3rd` row is `0110`.
 * 
 * Given two integer `n` and `k`, return the `kth` (**1-indexed**) symbol in
 * the
 * `nth` row of a table of `n` rows.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 1, k = 1
 *     **Output:** 0
 *     **Explanation:** row 1: _0_
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 2, k = 1
 *     **Output:** 0
 *     **Explanation:** 
 *     row 1: 0
 *     row 2: _0_ 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 2, k = 2
 *     **Output:** 1
 *     **Explanation:** 
 *     row 1: 0
 *     row 2: 0 _1_
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 30`
 *   * `1 <= k <= 2n - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/k-th-symbol-in-grammar/
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
    int kthGrammar(int n, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 1
    // Test case 2: 1
    // Test case 3: 2
    // Test case 4: 1
    // Test case 5: 2
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
