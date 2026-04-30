// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 164: Maximum Gap
 * Difficulty: Medium
 * Tags: Array, Bucket Sort, Radix Sort, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, return _the maximum difference between two
 * successive elements in its sorted form_. If the array contains less than two
 * elements, return `0`.
 * 
 * You must write an algorithm that runs in linear time and uses linear extra
 * space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,6,9,1]
 *     **Output:** 3
 * **Explanation:** The sorted form of the array is [1,3,6,9], either (3,6) or
 * (6,9) has the maximum difference 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [10]
 *     **Output:** 0
 * **Explanation:** The array contains less than 2 elements, therefore return
 * 0.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `0 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-gap/
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
    int maximumGap(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,6,9,1]
    // Test case 2: [10]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
