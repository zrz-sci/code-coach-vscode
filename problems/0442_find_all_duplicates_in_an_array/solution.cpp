// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 442: Find All Duplicates in an Array
 * Difficulty: Medium
 * Tags: Array, Hash Table, Sorting
 *
 * Problem Description:
 * Given an integer array `nums` of length `n` where all the integers of
 * `nums` are
 * in the range `[1, n]` and each integer appears **at most** **twice** ,
 * return
 * _an array of all the integers that appears**twice**_.
 * 
 * You must write an algorithm that runs in `O(n)` time and uses only
 * _constant_
 * auxiliary space, excluding the space needed to store the output
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,3,2,7,8,2,3,1]
 *     **Output:** [2,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,2]
 *     **Output:** [1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1]
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 105`
 *   * `1 <= nums[i] <= n`
 *   * Each element in `nums` appears **once** or **twice**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-all-duplicates-in-an-array/
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
    vector<int> findDuplicates(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,3,2,7,8,2,3,1]
    // Test case 2: [1,1,2]
    // Test case 3: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
