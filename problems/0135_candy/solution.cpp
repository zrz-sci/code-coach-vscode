// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 135: Candy
 * Difficulty: Hard
 * Tags: Greedy, Array
 *
 * Problem Description:
 * There are `n` children standing in a line. Each child is assigned a rating
 * value
 * given in the integer array `ratings`.
 * 
 * You are giving candies to these children subjected to the following
 * requirements:
 * 
 *   * Each child must have at least one candy.
 *   * Children with a higher rating get more candies than their neighbors.
 * 
 * Return _the minimum number of candies you need to have to distribute the
 * candies
 * to the children_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** ratings = [1,0,2]
 *     **Output:** 5
 * **Explanation:** You can allocate to the first, second and third child with
 * 2, 1, 2 candies respectively.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** ratings = [1,2,2]
 *     **Output:** 4
 * **Explanation:** You can allocate to the first, second and third child with
 * 1, 2, 1 candies respectively.
 * The third child gets 1 candy because it satisfies the above two conditions.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == ratings.length`
 *   * `1 <= n <= 2 * 104`
 *   * `0 <= ratings[i] <= 2 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/candy/
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
    int candy(vector<int>& ratings) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,2]
    // Test case 2: [1,2,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
