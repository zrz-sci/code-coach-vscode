// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 315: Count of Smaller Numbers After Self
 * Difficulty: Hard
 * Tags: Binary Indexed Tree, Segment Tree, Array, Binary Search, Divide and Conquer, Ordered Set, Merge Sort
 *
 * Problem Description:
 * Given an integer array `nums`, return _an integer array_`counts`
 * _where_`counts[i]`_is the number of smaller elements to the right
 * of_`nums[i]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [5,2,6,1]
 *     **Output:** [2,1,1,0]
 *     **Explanation:**
 *     To the right of 5 there are **2** smaller elements (2 and 1).
 *     To the right of 2 there is only **1** smaller element (1).
 *     To the right of 6 there is **1** smaller element (1).
 *     To the right of 1 there is **0** smaller element.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-1]
 *     **Output:** [0]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [-1,-1]
 *     **Output:** [0,0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-104 <= nums[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-of-smaller-numbers-after-self/
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
    vector<int> countSmaller(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,2,6,1]
    // Test case 2: [-1]
    // Test case 3: [-1,-1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
