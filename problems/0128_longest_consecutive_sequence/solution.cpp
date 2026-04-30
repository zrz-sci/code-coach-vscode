// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 128: Longest Consecutive Sequence
 * Difficulty: Medium
 * Tags: Union Find, Array, Hash Table
 *
 * Problem Description:
 * Given an unsorted array of integers `nums`, return _the length of the
 * longest
 * consecutive elements sequence._
 * 
 * You must write an algorithm that runs in `O(n)` time.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [100,4,200,1,3,2]
 *     **Output:** 4
 * **Explanation:** The longest consecutive elements sequence is [1, 2, 3, 4].
 * Therefore its length is 4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,3,7,2,5,8,4,6,0,1]
 *     **Output:** 9
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,1,2]
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-consecutive-sequence/
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
    int longestConsecutive(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [100,4,200,1,3,2]
    // Test case 2: [0,3,7,2,5,8,4,6,0,1]
    // Test case 3: [1,0,1,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
