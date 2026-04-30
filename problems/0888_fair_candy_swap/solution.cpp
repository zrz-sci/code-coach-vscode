// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 888: Fair Candy Swap
 * Difficulty: Easy
 * Tags: Array, Hash Table, Binary Search, Sorting
 *
 * Problem Description:
 * Alice and Bob have a different total number of candies. You are given two
 * integer arrays `aliceSizes` and `bobSizes` where `aliceSizes[i]` is the
 * number
 * of candies of the `ith` box of candy that Alice has and `bobSizes[j]` is the
 * number of candies of the `jth` box of candy that Bob has.
 * 
 * Since they are friends, they would like to exchange one candy box each so
 * that
 * after the exchange, they both have the same total amount of candy. The total
 * amount of candy a person has is the sum of the number of candies in each box
 * they have.
 * 
 * Return a _n integer array_`answer` _where_`answer[0]`_is the number of
 * candies
 * in the box that Alice must exchange, and_`answer[1]`_is the number of
 * candies in
 * the box that Bob must exchange_. If there are multiple answers, you may
 * **return
 * any** one of them. It is guaranteed that at least one answer exists.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** aliceSizes = [1,1], bobSizes = [2,2]
 *     **Output:** [1,2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** aliceSizes = [1,2], bobSizes = [2,3]
 *     **Output:** [1,2]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** aliceSizes = [2], bobSizes = [1,3]
 *     **Output:** [2,3]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= aliceSizes.length, bobSizes.length <= 104`
 *   * `1 <= aliceSizes[i], bobSizes[j] <= 105`
 *   * Alice and Bob have a different total number of candies.
 *   * There will be at least one valid answer for the given input.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/fair-candy-swap/
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
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,1]
    // Test case 2: [2,2]
    // Test case 3: [1,2]
    // Test case 4: [2,3]
    // Test case 5: [2]
    // Test case 6: [1,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
