// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1438: Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit
 * Difficulty: Medium
 * Tags: Queue, Array, Ordered Set, Sliding Window, Monotonic Queue, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an array of integers `nums` and an integer `limit`, return the size
 * of the
 * longest **non-empty** subarray such that the absolute difference between
 * any two
 * elements of this subarray is less than or equal to `limit` _._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [8,2,4,7], limit = 4
 *     **Output:** 2 
 *     **Explanation:** All subarrays are: 
 *     [8] with maximum absolute diff |8-8| = 0 <= 4.
 *     [8,2] with maximum absolute diff |8-2| = 6 > 4. 
 *     [8,2,4] with maximum absolute diff |8-2| = 6 > 4.
 *     [8,2,4,7] with maximum absolute diff |8-2| = 6 > 4.
 *     [2] with maximum absolute diff |2-2| = 0 <= 4.
 *     [2,4] with maximum absolute diff |2-4| = 2 <= 4.
 *     [2,4,7] with maximum absolute diff |2-7| = 5 > 4.
 *     [4] with maximum absolute diff |4-4| = 0 <= 4.
 *     [4,7] with maximum absolute diff |4-7| = 3 <= 4.
 *     [7] with maximum absolute diff |7-7| = 0 <= 4. 
 *     Therefore, the size of the longest subarray is 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [10,1,2,4,7,2], limit = 5
 *     **Output:** 4 
 * **Explanation:** The subarray [2,4,7,2] is the longest since the maximum
 * absolute diff is |2-7| = 5 <= 5.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [4,2,2,2,4,4,2,2], limit = 0
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `1 <= nums[i] <= 109`
 *   * `0 <= limit <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/
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
    int longestSubarray(vector<int>& nums, int limit) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [8,2,4,7]
    // Test case 2: 4
    // Test case 3: [10,1,2,4,7,2]
    // Test case 4: 5
    // Test case 5: [4,2,2,2,4,4,2,2]
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
