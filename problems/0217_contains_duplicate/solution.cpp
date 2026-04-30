// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 217: Contains Duplicate
 * Difficulty: Easy
 * Tags: Array, Hash Table, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, return `true` if any value appears **at least
 * twice** in the array, and return `false` if every element is distinct.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** nums = [1,2,3,1]
 * 
 * **Output:** true
 * 
 * **Explanation:**
 * 
 * The element 1 occurs at the indices 0 and 3.
 * 
 * **Example 2:**
 * 
 * **Input:** nums = [1,2,3,4]
 * 
 * **Output:** false
 * 
 * **Explanation:**
 * 
 * All elements are distinct.
 * 
 * **Example 3:**
 * 
 * **Input:** nums = [1,1,1,3,3,4,3,2,4,2]
 * 
 * **Output:** true
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/contains-duplicate/
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
    bool containsDuplicate(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,1]
    // Test case 2: [1,2,3,4]
    // Test case 3: [1,1,1,3,3,4,3,2,4,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
