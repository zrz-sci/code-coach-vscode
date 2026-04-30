// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 229: Majority Element II
 * Difficulty: Medium
 * Tags: Array, Hash Table, Counting, Sorting
 *
 * Problem Description:
 * Given an integer array of size `n`, find all elements that appear more than
 * `⌊
 * n/3 ⌋` times.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,3]
 *     **Output:** [3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1]
 *     **Output:** [1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2]
 *     **Output:** [1,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5 * 104`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 * 
 * **Follow up:** Could you solve the problem in linear time and in `O(1)`
 * space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/majority-element-ii/
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
    vector<int> majorityElement(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,2,3]
    // Test case 2: [1]
    // Test case 3: [1,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
