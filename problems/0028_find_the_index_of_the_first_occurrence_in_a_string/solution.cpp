// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 28: Find the Index of the First Occurrence in a String
 * Difficulty: Easy
 * Tags: Two Pointers, String, String Matching
 *
 * Problem Description:
 * Given two strings `needle` and `haystack`, return the index of the first
 * occurrence of `needle` in `haystack`, or `-1` if `needle` is not part of
 * `haystack`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** haystack = "sadbutsad", needle = "sad"
 *     **Output:** 0
 *     **Explanation:** "sad" occurs at index 0 and 6.
 *     The first occurrence is at index 0, so we return 0.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** haystack = "leetcode", needle = "leeto"
 *     **Output:** -1
 *     **Explanation:** "leeto" did not occur in "leetcode", so we return -1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= haystack.length, needle.length <= 104`
 *   * `haystack` and `needle` consist of only lowercase English characters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/
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
    int strStr(string haystack, string needle) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "sadbutsad"
    // Test case 2: "sad"
    // Test case 3: "leetcode"
    // Test case 4: "leeto"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
