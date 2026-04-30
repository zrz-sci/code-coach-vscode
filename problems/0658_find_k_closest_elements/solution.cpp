// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 658: Find K Closest Elements
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Binary Search, Sorting, Sliding Window, Heap (Priority Queue)
 *
 * Problem Description:
 * Given a **sorted** integer array `arr`, two integers `k` and `x`, return
 * the `k`
 * closest integers to `x` in the array. The result should also be sorted in
 * ascending order.
 * 
 * An integer `a` is closer to `x` than an integer `b` if:
 * 
 *   * `|a - x| < |b - x|`, or
 *   * `|a - x| == |b - x|` and `a < b`
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** arr = [1,2,3,4,5], k = 4, x = 3
 * 
 * **Output:** [1,2,3,4]
 * 
 * **Example 2:**
 * 
 * **Input:** arr = [1,1,2,3,4,5], k = 4, x = -1
 * 
 * **Output:** [1,1,2,3]
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k <= arr.length`
 *   * `1 <= arr.length <= 104`
 *   * `arr` is sorted in **ascending** order.
 *   * `-104 <= arr[i], x <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-k-closest-elements/
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
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5]
    // Test case 2: 4
    // Test case 3: 3
    // Test case 4: [1,1,2,3,4,5]
    // Test case 5: 4
    // Test case 6: -1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
