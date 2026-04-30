// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 349: Intersection of Two Arrays
 * Difficulty: Easy
 * Tags: Array, Hash Table, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * Given two integer arrays `nums1` and `nums2`, return _an array of their
 * intersection_. Each element in the result must be **unique** and you may
 * return
 * the result in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,2,2,1], nums2 = [2,2]
 *     **Output:** [2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [4,9,5], nums2 = [9,4,9,8,4]
 *     **Output:** [9,4]
 *     **Explanation:** [4,9] is also accepted.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums1.length, nums2.length <= 1000`
 *   * `0 <= nums1[i], nums2[i] <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/intersection-of-two-arrays/
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
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,2,1]
    // Test case 2: [2,2]
    // Test case 3: [4,9,5]
    // Test case 4: [9,4,9,8,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
