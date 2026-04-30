// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 521: Longest Uncommon Subsequence I
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * Given two strings `a` and `b`, return _the length of the**longest uncommon
 * subsequence** between _`a` _and_ `b`. _If no such uncommon subsequence
 * exists,
 * return_ `-1` _._
 * 
 * An **uncommon subsequence** between two strings is a string that is a
 * **subsequence of exactly one of them**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** a = "aba", b = "cdc"
 *     **Output:** 3
 * **Explanation:** One longest uncommon subsequence is "aba" because "aba" is
 * a subsequence of "aba" but not "cdc".
 *     Note that "cdc" is also a longest uncommon subsequence.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** a = "aaa", b = "bbb"
 *     **Output:** 3
 *     **Explanation:**  The longest uncommon subsequences are "aaa" and "bbb".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** a = "aaa", b = "aaa"
 *     **Output:** -1
 * **Explanation:** Every subsequence of string a is also a subsequence of
 * string b. Similarly, every subsequence of string b is also a subsequence of
 * string a. So the answer would be -1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= a.length, b.length <= 100`
 *   * `a` and `b` consist of lower-case English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-uncommon-subsequence-i/
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
    int findLUSlength(string a, string b) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aba"
    // Test case 2: "cdc"
    // Test case 3: "aaa"
    // Test case 4: "bbb"
    // Test case 5: "aaa"
    // Test case 6: "aaa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
