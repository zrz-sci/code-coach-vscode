// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 4: Median of Two Sorted Arrays
 * Difficulty: Hard
 * Tags: Array, Binary Search, Divide and Conquer
 *
 * Problem Description:
 * Given two sorted arrays `nums1` and `nums2` of size `m` and `n`
 * respectively,
 * return **the median** of the two sorted arrays.
 * 
 * The overall run time complexity should be `O(log (m+n))`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,3], nums2 = [2]
 *     **Output:** 2.00000
 *     **Explanation:** merged array = [1,2,3] and median is 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,2], nums2 = [3,4]
 *     **Output:** 2.50000
 * **Explanation:** merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `nums1.length == m`
 *   * `nums2.length == n`
 *   * `0 <= m <= 1000`
 *   * `0 <= n <= 1000`
 *   * `1 <= m + n <= 2000`
 *   * `-106 <= nums1[i], nums2[i] <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/median-of-two-sorted-arrays/
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
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3]
    // Test case 2: [2]
    // Test case 3: [1,2]
    // Test case 4: [3,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
