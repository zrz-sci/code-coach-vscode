// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 899: Orderly Queue
 * Difficulty: Hard
 * Tags: Math, String, Sorting
 *
 * Problem Description:
 * You are given a string `s` and an integer `k`. You can choose one of the
 * first
 * `k` letters of `s` and append it at the end of the string.
 * 
 * Return _the lexicographically smallest string you could have after applying
 * the
 * mentioned step any number of moves_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "cba", k = 1
 *     **Output:** "acb"
 *     **Explanation:** 
 * In the first move, we move the 1st character 'c' to the end, obtaining the
 * string "bac".
 * In the second move, we move the 1st character 'b' to the end, obtaining the
 * final result "acb".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "baaca", k = 3
 *     **Output:** "aaabc"
 *     **Explanation:** 
 * In the first move, we move the 1st character 'b' to the end, obtaining the
 * string "aacab".
 * In the second move, we move the 3rd character 'c' to the end, obtaining the
 * final result "aaabc".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k <= s.length <= 1000`
 *   * `s` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/orderly-queue/
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
    string orderlyQueue(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "cba"
    // Test case 2: 1
    // Test case 3: "baaca"
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
