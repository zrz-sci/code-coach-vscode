// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 136: Single Number
 * Difficulty: Easy
 * Tags: Bit Manipulation, Array
 *
 * Problem Description:
 * Given a **non-empty** array of integers `nums`, every element appears
 * _twice_
 * except for one. Find that single one.
 * 
 * You must implement a solution with a linear runtime complexity and use only
 * constant extra space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** nums = [2,2,1]
 * 
 * **Output:** 1
 * 
 * **Example 2:**
 * 
 * **Input:** nums = [4,1,2,1,2]
 * 
 * **Output:** 4
 * 
 * **Example 3:**
 * 
 * **Input:** nums = [1]
 * 
 * **Output:** 1
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 3 * 104`
 *   * `-3 * 104 <= nums[i] <= 3 * 104`
 * * Each element in the array appears twice except for one element which
 * appears only once.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/single-number/
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
    int singleNumber(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,2,1]
    // Test case 2: [4,1,2,1,2]
    // Test case 3: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
