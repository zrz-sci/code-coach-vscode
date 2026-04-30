// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1044: Longest Duplicate Substring
 * Difficulty: Hard
 * Tags: String, Binary Search, Suffix Array, Sliding Window, Hash Function, Rolling Hash
 *
 * Problem Description:
 * Given a string `s`, consider all _duplicated substrings_ : (contiguous)
 * substrings of s that occur 2 or more times. The occurrences may overlap.
 * 
 * Return **any** duplicated substring that has the longest possible length.
 * If `s`
 * does not have a duplicated substring, the answer is `""`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "banana"
 *     **Output:** "ana"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abcd"
 *     **Output:** ""
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= s.length <= 3 * 104`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-duplicate-substring/
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
    string longestDupSubstring(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "banana"
    // Test case 2: "abcd"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
