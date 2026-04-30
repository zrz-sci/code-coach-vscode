// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1512: Number of Good Pairs
 * Difficulty: Easy
 * Tags: Array, Hash Table, Math, Counting
 *
 * Problem Description:
 * Given an array of integers `nums`, return _the number of**good pairs**_.
 * 
 * A pair `(i, j)` is called _good_ if `nums[i] == nums[j]` and `i` < `j`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,1,1,3]
 *     **Output:** 4
 * **Explanation:** There are 4 good pairs (0,3), (0,4), (3,4), (2,5)
 * 0-indexed.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,1,1]
 *     **Output:** 6
 *     **Explanation:** Each pair in the array are _good_.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 100`
 *   * `1 <= nums[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-good-pairs/
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
    int numIdenticalPairs(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,1,1,3]
    // Test case 2: [1,1,1,1]
    // Test case 3: [1,2,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
