// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 面试题 10.01: Sorted Merge LCCI
 * Difficulty: Easy
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * You are given two sorted arrays, A and B, where A has a large enough buffer
 * at
 * the end to hold B. Write a method to merge B into A in sorted order.
 * 
 * Initially the number of elements in A and B are _m_  and _n_ respectively.
 * 
 * **Example:**
 * 
 *     
 *     
 *     **Input:**
 *     A = [1,2,3,0,0,0], m = 3
 *     B = [2,5,6],       n = 3
 *     
 *     **Output:**  [1,2,2,3,5,6]
 * 
 * **Note:**
 * 
 *   * `A.length == n + m`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sorted-merge-lcci/
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
    void merge(vector<int>& A, int m, vector<int>& B, int n) {
        
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

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
