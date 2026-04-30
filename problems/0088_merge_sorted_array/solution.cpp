// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 88: Merge Sorted Array
 * Difficulty: Easy
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * You are given two integer arrays `nums1` and `nums2`, sorted in
 * **non-decreasing
 * order** , and two integers `m` and `n`, representing the number of elements
 * in
 * `nums1` and `nums2` respectively.
 * 
 * **Merge** `nums1` and `nums2` into a single array sorted in **non-decreasing
 * order**.
 * 
 * The final sorted array should not be returned by the function, but instead
 * be
 * _stored inside the array_`nums1`. To accommodate this, `nums1` has a length
 * of
 * `m + n`, where the first `m` elements denote the elements that should be
 * merged,
 * and the last `n` elements are set to `0` and should be ignored. `nums2` has
 * a
 * length of `n`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
 *     **Output:** [1,2,2,3,5,6]
 *     **Explanation:** The arrays we are merging are [1,2,3] and [2,5,6].
 * The result of the merge is [_1_ ,_2_ ,2,_3_ ,5,6] with the underlined
 * elements coming from nums1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1], m = 1, nums2 = [], n = 0
 *     **Output:** [1]
 *     **Explanation:** The arrays we are merging are [1] and [].
 *     The result of the merge is [1].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums1 = [0], m = 0, nums2 = [1], n = 1
 *     **Output:** [1]
 *     **Explanation:** The arrays we are merging are [] and [1].
 *     The result of the merge is [1].
 * Note that because m = 0, there are no elements in nums1. The 0 is only
 * there to ensure the merge result can fit in nums1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `nums1.length == m + n`
 *   * `nums2.length == n`
 *   * `0 <= m, n <= 200`
 *   * `1 <= m + n <= 200`
 *   * `-109 <= nums1[i], nums2[j] <= 109`
 * 
 * 
 * 
 * **Follow up:** Can you come up with an algorithm that runs in `O(m + n)`
 * time?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/merge-sorted-array/
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
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,0,0,0]
    // Test case 2: 3
    // Test case 3: [2,5,6]
    // Test case 4: 3
    // Test case 5: [1]
    // Test case 6: 1
    // Test case 7: []
    // Test case 8: 0
    // Test case 9: [0]
    // Test case 10: 0
    // Test case 11: [1]
    // Test case 12: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
