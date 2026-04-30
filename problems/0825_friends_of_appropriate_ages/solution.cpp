// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 825: Friends Of Appropriate Ages
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * There are `n` persons on a social media website. You are given an integer
 * array
 * `ages` where `ages[i]` is the age of the `ith` person.
 * 
 * A Person `x` will not send a friend request to a person `y` (`x != y`) if
 * any of
 * the following conditions is true:
 * 
 *   * `age[y] <= 0.5 * age[x] + 7`
 *   * `age[y] > age[x]`
 *   * `age[y] > 100 && age[x] < 100`
 * 
 * Otherwise, `x` will send a friend request to `y`.
 * 
 * Note that if `x` sends a request to `y`, `y` will not necessarily send a
 * request
 * to `x`. Also, a person will not send a friend request to themself.
 * 
 * Return _the total number of friend requests made_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** ages = [16,16]
 *     **Output:** 2
 *     **Explanation:** 2 people friend request each other.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** ages = [16,17,18]
 *     **Output:** 2
 *     **Explanation:** Friend requests are made 17 -> 16, 18 -> 17.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** ages = [20,30,100,110,120]
 *     **Output:** 3
 * **Explanation:** Friend requests are made 110 -> 100, 120 -> 110, 120 ->
 * 100.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == ages.length`
 *   * `1 <= n <= 2 * 104`
 *   * `1 <= ages[i] <= 120`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/friends-of-appropriate-ages/
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
    int numFriendRequests(vector<int>& ages) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [16,16]
    // Test case 2: [16,17,18]
    // Test case 3: [20,30,100,110,120]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
