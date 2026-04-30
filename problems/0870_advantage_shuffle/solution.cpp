// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 870: Advantage Shuffle
 * Difficulty: Medium
 * Tags: Greedy, Array, Two Pointers, Sorting
 *
 * Problem Description:
 * You are given two integer arrays `nums1` and `nums2` both of the same
 * length.
 * The **advantage** of `nums1` with respect to `nums2` is the number of
 * indices
 * `i` for which `nums1[i] > nums2[i]`.
 * 
 * Return _any permutation of_`nums1` _that maximizes its**advantage** with
 * respect
 * to _`nums2`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [2,7,11,15], nums2 = [1,10,4,11]
 *     **Output:** [2,11,7,15]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [12,24,8,32], nums2 = [13,25,32,11]
 *     **Output:** [24,32,8,12]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums1.length <= 105`
 *   * `nums2.length == nums1.length`
 *   * `0 <= nums1[i], nums2[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/advantage-shuffle/
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
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,7,11,15]
    // Test case 2: [1,10,4,11]
    // Test case 3: [12,24,8,32]
    // Test case 4: [13,25,32,11]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
