// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 438: Find All Anagrams in a String
 * Difficulty: Medium
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * Given two strings `s` and `p`, return an array of all the start indices of
 * `p`'s
 * anagrams in `s`. You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "cbaebabacd", p = "abc"
 *     **Output:** [0,6]
 *     **Explanation:**
 * The substring with start index = 0 is "cba", which is an anagram of "abc".
 * The substring with start index = 6 is "bac", which is an anagram of "abc".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abab", p = "ab"
 *     **Output:** [0,1,2]
 *     **Explanation:**
 *     The substring with start index = 0 is "ab", which is an anagram of "ab".
 *     The substring with start index = 1 is "ba", which is an anagram of "ab".
 *     The substring with start index = 2 is "ab", which is an anagram of "ab".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length, p.length <= 3 * 104`
 *   * `s` and `p` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-all-anagrams-in-a-string/
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
    vector<int> findAnagrams(string s, string p) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "cbaebabacd"
    // Test case 2: "abc"
    // Test case 3: "abab"
    // Test case 4: "ab"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
