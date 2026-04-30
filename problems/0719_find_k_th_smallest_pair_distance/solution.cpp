// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 719: Find K-th Smallest Pair Distance
 * Difficulty: Hard
 * Tags: Array, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * The **distance of a pair** of integers `a` and `b` is defined as the
 * absolute
 * difference between `a` and `b`.
 * 
 * Given an integer array `nums` and an integer `k`, return _the_ `kth`
 * _smallest**distance among all the pairs**_ `nums[i]` _and_ `nums[j]`
 * _where_ `0
 * <= i < j < nums.length`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,1], k = 1
 *     **Output:** 0
 *     **Explanation:** Here are all the pairs:
 *     (1,3) -> 2
 *     (1,1) -> 0
 *     (3,1) -> 2
 *     Then the 1st smallest distance pair is (1,1), and its distance is 0.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,1], k = 2
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,6,1], k = 3
 *     **Output:** 5
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `2 <= n <= 104`
 *   * `0 <= nums[i] <= 106`
 *   * `1 <= k <= n * (n - 1) / 2`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-k-th-smallest-pair-distance/
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
    int smallestDistancePair(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,1]
    // Test case 2: 1
    // Test case 3: [1,1,1]
    // Test case 4: 2
    // Test case 5: [1,6,1]
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
