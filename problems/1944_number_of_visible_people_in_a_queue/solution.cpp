// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1944: Number of Visible People in a Queue
 * Difficulty: Hard
 * Tags: Stack, Array, Monotonic Stack
 *
 * Problem Description:
 * There are `n` people standing in a queue, and they numbered from `0` to `n
 * - 1`
 * in **left to right** order. You are given an array `heights` of **distinct**
 * integers where `heights[i]` represents the height of the `ith` person.
 * 
 * A person can **see** another person to their right in the queue if
 * everybody in
 * between is **shorter** than both of them. More formally, the `ith` person
 * can
 * see the `jth` person if `i < j` and `min(heights[i], heights[j]) >
 * max(heights[i+1], heights[i+2], ..., heights[j-1])`.
 * 
 * Return _an array_`answer` _of length_`n` _where_`answer[i]`_is the**number
 * of
 * people** the _`ith` _person can**see** to their right in the queue_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** heights = [10,6,8,5,11,9]
 *     **Output:** [3,1,2,1,1,0]
 *     **Explanation:**
 *     Person 0 can see person 1, 2, and 4.
 *     Person 1 can see person 2.
 *     Person 2 can see person 3 and 4.
 *     Person 3 can see person 4.
 *     Person 4 can see person 5.
 *     Person 5 can see no one since nobody is to the right of them.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** heights = [5,1,2,3,10]
 *     **Output:** [4,1,1,1,0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == heights.length`
 *   * `1 <= n <= 105`
 *   * `1 <= heights[i] <= 105`
 *   * All the values of `heights` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-visible-people-in-a-queue/
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
    vector<int> canSeePersonsCount(vector<int>& heights) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,6,8,5,11,9]
    // Test case 2: [5,1,2,3,10]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
