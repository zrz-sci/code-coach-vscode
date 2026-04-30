// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 646: Maximum Length of Pair Chain
 * Difficulty: Medium
 * Tags: Greedy, Array, Dynamic Programming, Sorting
 *
 * Problem Description:
 * You are given an array of `n` pairs `pairs` where `pairs[i] = [lefti,
 * righti]`
 * and `lefti < righti`.
 * 
 * A pair `p2 = [c, d]` **follows** a pair `p1 = [a, b]` if `b < c`. A
 * **chain** of
 * pairs can be formed in this fashion.
 * 
 * Return _the length longest chain which can be formed_.
 * 
 * You do not need to use up all the given intervals. You can select pairs in
 * any
 * order.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** pairs = [[1,2],[2,3],[3,4]]
 *     **Output:** 2
 *     **Explanation:** The longest chain is [1,2] -> [3,4].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** pairs = [[1,2],[7,8],[4,5]]
 *     **Output:** 3
 *     **Explanation:** The longest chain is [1,2] -> [4,5] -> [7,8].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == pairs.length`
 *   * `1 <= n <= 1000`
 *   * `-1000 <= lefti < righti <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-length-of-pair-chain/
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
    int findLongestChain(vector<vector<int>>& pairs) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[2,3],[3,4]]
    // Test case 2: [[1,2],[7,8],[4,5]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
