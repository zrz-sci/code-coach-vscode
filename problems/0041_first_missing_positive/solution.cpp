// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 41: First Missing Positive
 * Difficulty: Hard
 * Tags: Array, Hash Table
 *
 * Problem Description:
 * Given an unsorted integer array `nums`. Return the _smallest positive
 * integer_
 * that is _not present_ in `nums`.
 * 
 * You must implement an algorithm that runs in `O(n)` time and uses `O(1)`
 * auxiliary space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,0]
 *     **Output:** 3
 *     **Explanation:** The numbers in the range [1,2] are all in the array.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,4,-1,1]
 *     **Output:** 2
 *     **Explanation:** 1 is in the array but 2 is missing.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [7,8,9,11,12]
 *     **Output:** 1
 *     **Explanation:** The smallest positive integer 1 is missing.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-231 <= nums[i] <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/first-missing-positive/
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
    int firstMissingPositive(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,0]
    // Test case 2: [3,4,-1,1]
    // Test case 3: [7,8,9,11,12]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
