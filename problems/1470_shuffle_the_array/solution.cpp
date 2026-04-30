// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1470: Shuffle the Array
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * Given the array `nums` consisting of `2n` elements in the form
 * `[x1,x2,...,xn,y1,y2,...,yn]`.
 * 
 * _Return the array in the form_ `[x1,y1,x2,y2,...,xn,yn]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,5,1,3,4,7], n = 3
 *     **Output:** [2,3,5,4,1,7] 
 * **Explanation:** Since x1=2, x2=5, x3=1, y1=3, y2=4, y3=7 then the answer
 * is [2,3,5,4,1,7].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4,4,3,2,1], n = 4
 *     **Output:** [1,4,2,3,3,2,4,1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,2,2], n = 2
 *     **Output:** [1,2,1,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 500`
 *   * `nums.length == 2n`
 *   * `1 <= nums[i] <= 10^3`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shuffle-the-array/
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
    vector<int> shuffle(vector<int>& nums, int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,5,1,3,4,7]
    // Test case 2: 3
    // Test case 3: [1,2,3,4,4,3,2,1]
    // Test case 4: 4
    // Test case 5: [1,1,2,2]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
