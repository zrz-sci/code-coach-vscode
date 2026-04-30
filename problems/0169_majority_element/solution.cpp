// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 169: Majority Element
 * Difficulty: Easy
 * Tags: Array, Hash Table, Divide and Conquer, Counting, Sorting
 *
 * Problem Description:
 * Given an array `nums` of size `n`, return _the majority element_.
 * 
 * The majority element is the element that appears more than `⌊n / 2⌋` times.
 * You
 * may assume that the majority element always exists in the array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,3]
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,2,1,1,1,2,2]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 5 * 104`
 *   * `-109 <= nums[i] <= 109`
 * * The input is generated such that a majority element will exist in the
 * array.
 * 
 * 
 * 
 * **Follow-up:** Could you solve the problem in linear time and in `O(1)`
 * space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/majority-element/
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
    int majorityElement(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,2,3]
    // Test case 2: [2,2,1,1,1,2,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
