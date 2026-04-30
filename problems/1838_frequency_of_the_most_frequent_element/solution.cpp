// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1838: Frequency of the Most Frequent Element
 * Difficulty: Medium
 * Tags: Greedy, Array, Binary Search, Prefix Sum, Sorting, Sliding Window
 *
 * Problem Description:
 * The **frequency** of an element is the number of times it occurs in an
 * array.
 * 
 * You are given an integer array `nums` and an integer `k`. In one operation,
 * you
 * can choose an index of `nums` and increment the element at that index by
 * `1`.
 * 
 * Return _the**maximum possible frequency** of an element after performing
 * **at
 * most** _`k` _operations_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,4], k = 5
 * **Output:** 3**Explanation:** Increment the first element three times and
 * the second element two times to make nums = [4,4,4].
 *     4 has a frequency of 3.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,4,8,13], k = 5
 *     **Output:** 2
 *     **Explanation:** There are multiple optimal solutions:
 * - Increment the first element three times to make nums = [4,4,8,13]. 4 has
 * a frequency of 2.
 * - Increment the second element four times to make nums = [1,8,8,13]. 8 has
 * a frequency of 2.
 * - Increment the third element five times to make nums = [1,4,13,13]. 13 has
 * a frequency of 2.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [3,9,6], k = 2
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `1 <= nums[i] <= 105`
 *   * `1 <= k <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/frequency-of-the-most-frequent-element/
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
    int maxFrequency(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,4]
    // Test case 2: 5
    // Test case 3: [1,4,8,13]
    // Test case 4: 5
    // Test case 5: [3,9,6]
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
