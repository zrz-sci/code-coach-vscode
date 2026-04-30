// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 162: Find Peak Element
 * Difficulty: Medium
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * A peak element is an element that is strictly greater than its neighbors.
 * 
 * Given a **0-indexed** integer array `nums`, find a peak element, and return
 * its
 * index. If the array contains multiple peaks, return the index to **any of
 * the
 * peaks**.
 * 
 * You may imagine that `nums[-1] = nums[n] = -∞`. In other words, an element
 * is
 * always considered to be strictly greater than a neighbor that is outside the
 * array.
 * 
 * You must write an algorithm that runs in `O(log n)` time.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,1]
 *     **Output:** 2
 * **Explanation:** 3 is a peak element and your function should return the
 * index number 2.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1,3,5,6,4]
 *     **Output:** 5
 * **Explanation:** Your function can return either index number 1 where the
 * peak element is 2, or index number 5 where the peak element is 6.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 1000`
 *   * `-231 <= nums[i] <= 231 - 1`
 *   * `nums[i] != nums[i + 1]` for all valid `i`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-peak-element/
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
    int findPeakElement(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,1]
    // Test case 2: [1,2,1,3,5,6,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
