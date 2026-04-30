// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1089: Duplicate Zeros
 * Difficulty: Easy
 * Tags: Array, Two Pointers
 *
 * Problem Description:
 * Given a fixed-length integer array `arr`, duplicate each occurrence of zero,
 * shifting the remaining elements to the right.
 * 
 * **Note** that elements beyond the length of the original array are not
 * written.
 * Do the above modifications to the input array in place and do not return
 * anything.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [1,0,2,3,0,4,5,0]
 *     **Output:** [1,0,0,2,3,0,0,4]
 * **Explanation:** After calling your function, the input array is modified
 * to: [1,0,0,2,3,0,0,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [1,2,3]
 *     **Output:** [1,2,3]
 * **Explanation:** After calling your function, the input array is modified
 * to: [1,2,3]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= arr.length <= 104`
 *   * `0 <= arr[i] <= 9`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/duplicate-zeros/
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
    void duplicateZeros(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,2,3,0,4,5,0]
    // Test case 2: [1,2,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
