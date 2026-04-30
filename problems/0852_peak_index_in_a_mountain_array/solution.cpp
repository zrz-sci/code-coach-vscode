// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 852: Peak Index in a Mountain Array
 * Difficulty: Medium
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * You are given an integer **mountain** array `arr` of length `n` where the
 * values
 * increase to a **peak element** and then decrease.
 * 
 * Return the index of the peak element.
 * 
 * Your task is to solve it in `O(log(n))` time complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** arr = [0,1,0]
 * 
 * **Output:** 1
 * 
 * **Example 2:**
 * 
 * **Input:** arr = [0,2,1,0]
 * 
 * **Output:** 1
 * 
 * **Example 3:**
 * 
 * **Input:** arr = [0,10,5,2]
 * 
 * **Output:** 1
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= arr.length <= 105`
 *   * `0 <= arr[i] <= 106`
 *   * `arr` is **guaranteed** to be a mountain array.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/peak-index-in-a-mountain-array/
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
    int peakIndexInMountainArray(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,0]
    // Test case 2: [0,2,1,0]
    // Test case 3: [0,10,5,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
