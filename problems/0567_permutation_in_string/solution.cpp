// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 567: Permutation in String
 * Difficulty: Medium
 * Tags: Hash Table, Two Pointers, String, Sliding Window
 *
 * Problem Description:
 * Given two strings `s1` and `s2`, return `true` if `s2` contains a
 * permutation of
 * `s1`, or `false` otherwise.
 * 
 * In other words, return `true` if one of `s1`'s permutations is the
 * substring of
 * `s2`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s1 = "ab", s2 = "eidbaooo"
 *     **Output:** true
 *     **Explanation:** s2 contains one permutation of s1 ("ba").
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s1 = "ab", s2 = "eidboaoo"
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s1.length, s2.length <= 104`
 *   * `s1` and `s2` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/permutation-in-string/
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
    bool checkInclusion(string s1, string s2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ab"
    // Test case 2: "eidbaooo"
    // Test case 3: "ab"
    // Test case 4: "eidboaoo"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
