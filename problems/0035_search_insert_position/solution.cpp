// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 35: Search Insert Position
 * Difficulty: Easy
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * Given a sorted array of distinct integers and a target value, return the
 * index
 * if the target is found. If not, return the index where it would be if it
 * were
 * inserted in order.
 * 
 * You must write an algorithm with `O(log n)` runtime complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,5,6], target = 5
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,5,6], target = 2
 *     **Output:** 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,5,6], target = 7
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-104 <= nums[i] <= 104`
 *   * `nums` contains **distinct** values sorted in **ascending** order.
 *   * `-104 <= target <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/search-insert-position/
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
    int searchInsert(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,5,6]
    // Test case 2: 5
    // Test case 3: [1,3,5,6]
    // Test case 4: 2
    // Test case 5: [1,3,5,6]
    // Test case 6: 7

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
