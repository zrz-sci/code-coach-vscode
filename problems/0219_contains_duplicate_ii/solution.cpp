// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 219: Contains Duplicate II
 * Difficulty: Easy
 * Tags: Array, Hash Table, Sliding Window
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, return `true` _if there
 * are
 * two**distinct indices** _`i` _and_`j` _in the array such that_`nums[i] ==
 * nums[j]`_and_`abs(i - j) <= k`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,1], k = 3
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,1,1], k = 1
 *     **Output:** true
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,1,2,3], k = 2
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 *   * `0 <= k <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/contains-duplicate-ii/
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
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,1]
    // Test case 2: 3
    // Test case 3: [1,0,1,1]
    // Test case 4: 1
    // Test case 5: [1,2,3,1,2,3]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
