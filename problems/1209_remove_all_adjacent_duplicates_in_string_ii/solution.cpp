// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1209: Remove All Adjacent Duplicates in String II
 * Difficulty: Medium
 * Tags: Stack, String
 *
 * Problem Description:
 * You are given a string `s` and an integer `k`, a `k` **duplicate removal**
 * consists of choosing `k` adjacent and equal letters from `s` and removing
 * them,
 * causing the left and the right side of the deleted substring to concatenate
 * together.
 * 
 * We repeatedly make `k` **duplicate removals** on `s` until we no longer can.
 * 
 * Return _the final string after all such duplicate removals have been made_.
 * It
 * is guaranteed that the answer is **unique**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abcd", k = 2
 *     **Output:** "abcd"
 *     **Explanation:** There's nothing to delete.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "deeedbbcccbdaa", k = 3
 *     **Output:** "aa"
 *     **Explanation:** First delete "eee" and "ccc", get "ddbbbdaa"
 *     Then delete "bbb", get "dddaa"
 *     Finally delete "ddd", get "aa"
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "pbbcggttciiippooaais", k = 2
 *     **Output:** "ps"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `2 <= k <= 104`
 *   * `s` only contains lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/remove-all-adjacent-duplicates-in-string-ii/
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
    string removeDuplicates(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcd"
    // Test case 2: 2
    // Test case 3: "deeedbbcccbdaa"
    // Test case 4: 3
    // Test case 5: "pbbcggttciiippooaais"
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
