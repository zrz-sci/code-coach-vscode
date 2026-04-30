// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 881: Boats to Save People
 * Difficulty: Medium
 * Tags: Greedy, Array, Two Pointers, Sorting
 *
 * Problem Description:
 * You are given an array `people` where `people[i]` is the weight of the `ith`
 * person, and an **infinite number of boats** where each boat can carry a
 * maximum
 * weight of `limit`. Each boat carries at most two people at the same time,
 * provided the sum of the weight of those people is at most `limit`.
 * 
 * Return _the minimum number of boats to carry every given person_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** people = [1,2], limit = 3
 *     **Output:** 1
 *     **Explanation:** 1 boat (1, 2)
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** people = [3,2,2,1], limit = 3
 *     **Output:** 3
 *     **Explanation:** 3 boats (1, 2), (2) and (3)
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** people = [3,5,3,4], limit = 5
 *     **Output:** 4
 *     **Explanation:** 4 boats (3), (3), (4), (5)
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= people.length <= 5 * 104`
 *   * `1 <= people[i] <= limit <= 3 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/boats-to-save-people/
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
    int numRescueBoats(vector<int>& people, int limit) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2]
    // Test case 2: 3
    // Test case 3: [3,2,2,1]
    // Test case 4: 3
    // Test case 5: [3,5,3,4]
    // Test case 6: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
