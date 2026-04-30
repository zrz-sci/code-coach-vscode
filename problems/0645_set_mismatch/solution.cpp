// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 645: Set Mismatch
 * Difficulty: Easy
 * Tags: Bit Manipulation, Array, Hash Table, Sorting
 *
 * Problem Description:
 * You have a set of integers `s`, which originally contains all the numbers
 * from
 * `1` to `n`. Unfortunately, due to some error, one of the numbers in `s` got
 * duplicated to another number in the set, which results in **repetition of
 * one**
 * number and **loss of another** number.
 * 
 * You are given an integer array `nums` representing the data status of this
 * set
 * after the error.
 * 
 * Find the number that occurs twice and the number that is missing and return
 * _them in the form of an array_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,2,4]
 *     **Output:** [2,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1]
 *     **Output:** [1,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 104`
 *   * `1 <= nums[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/set-mismatch/
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
    vector<int> findErrorNums(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,2,4]
    // Test case 2: [1,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
