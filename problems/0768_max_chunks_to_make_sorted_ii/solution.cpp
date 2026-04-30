// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 768: Max Chunks To Make Sorted II
 * Difficulty: Hard
 * Tags: Stack, Greedy, Array, Sorting, Monotonic Stack
 *
 * Problem Description:
 * You are given an integer array `arr`.
 * 
 * We split `arr` into some number of **chunks** (i.e., partitions), and
 * individually sort each chunk. After concatenating them, the result should
 * equal
 * the sorted array.
 * 
 * Return _the largest number of chunks we can make to sort the array_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [5,4,3,2,1]
 *     **Output:** 1
 *     **Explanation:**
 *     Splitting into two or more chunks will not return the required result.
 * For example, splitting into [5, 4], [3, 2, 1] will result in [4, 5, 1, 2,
 * 3], which isn't sorted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [2,1,3,4,4]
 *     **Output:** 4
 *     **Explanation:**
 *     We can split into two chunks, such as [2, 1], [3, 4, 4].
 * However, splitting into [2, 1], [3], [4], [4] is the highest number of
 * chunks possible.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= arr.length <= 2000`
 *   * `0 <= arr[i] <= 108`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/max-chunks-to-make-sorted-ii/
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
    int maxChunksToSorted(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,4,3,2,1]
    // Test case 2: [2,1,3,4,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
