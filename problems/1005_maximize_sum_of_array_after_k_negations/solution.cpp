// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1005: Maximize Sum Of Array After K Negations
 * Difficulty: Easy
 * Tags: Greedy, Array, Sorting
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, modify the array in the
 * following way:
 * 
 *   * choose an index `i` and replace `nums[i]` with `-nums[i]`.
 * 
 * You should apply this process exactly `k` times. You may choose the same
 * index
 * `i` multiple times.
 * 
 * Return _the largest possible sum of the array after modifying it in this
 * way_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,2,3], k = 1
 *     **Output:** 5
 *     **Explanation:** Choose index 1 and nums becomes [4,-2,3].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,-1,0,2], k = 3
 *     **Output:** 6
 *     **Explanation:** Choose indices (1, 2, 2) and nums becomes [3,1,0,2].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [2,-3,-1,5,-4], k = 2
 *     **Output:** 13
 *     **Explanation:** Choose indices (1, 4) and nums becomes [2,3,-1,5,4].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-100 <= nums[i] <= 100`
 *   * `1 <= k <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximize-sum-of-array-after-k-negations/
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
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,3]
    // Test case 2: 1
    // Test case 3: [3,-1,0,2]
    // Test case 4: 3
    // Test case 5: [2,-3,-1,5,-4]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
