// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 801: Minimum Swaps To Make Sequences Increasing
 * Difficulty: Hard
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * You are given two integer arrays of the same length `nums1` and `nums2`. In
 * one
 * operation, you are allowed to swap `nums1[i]` with `nums2[i]`.
 * 
 * * For example, if `nums1 = [1,2,3,_8_]`, and `nums2 = [5,6,7,_4_]`, you can
 * swap the element at `i = 3` to obtain `nums1 = [1,2,3,4]` and `nums2 =
 * [5,6,7,8]`.
 * 
 * Return _the minimum number of needed operations to make_`nums1` _and_`nums2`
 * _**strictly increasing**_. The test cases are generated so that the given
 * input
 * always makes it possible.
 * 
 * An array `arr` is **strictly increasing** if and only if `arr[0] < arr[1] <
 * arr[2] < ... < arr[arr.length - 1]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,3,5,4], nums2 = [1,2,3,7]
 *     **Output:** 1
 *     **Explanation:** 
 *     Swap nums1[3] and nums2[3]. Then the sequences are:
 *     nums1 = [1, 3, 5, 7] and nums2 = [1, 2, 3, 4]
 *     which are both strictly increasing.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [0,3,5,8,9], nums2 = [2,1,4,6,9]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums1.length <= 105`
 *   * `nums2.length == nums1.length`
 *   * `0 <= nums1[i], nums2[i] <= 2 * 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-swaps-to-make-sequences-increasing/
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
    int minSwap(vector<int>& nums1, vector<int>& nums2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,5,4]
    // Test case 2: [1,2,3,7]
    // Test case 3: [0,3,5,8,9]
    // Test case 4: [2,1,4,6,9]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
