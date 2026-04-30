// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 152: Maximum Product Subarray
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * Given an integer array `nums`, find a subarray that has the largest
 * product, and
 * return _the product_.
 * 
 * The test cases are generated so that the answer will fit in a **32-bit**
 * integer.
 * 
 * **Note** that the product of an array with a single element is the value of
 * that
 * element.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,3,-2,4]
 *     **Output:** 6
 *     **Explanation:** [2,3] has the largest product 6.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-2,0,-1]
 *     **Output:** 0
 * **Explanation:** The result cannot be 2, because [-2,-1] is not a subarray.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 104`
 *   * `-10 <= nums[i] <= 10`
 * * The product of any subarray of `nums` is **guaranteed** to fit in a
 * **32-bit** integer.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-product-subarray/
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
    int maxProduct(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,3,-2,4]
    // Test case 2: [-2,0,-1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
