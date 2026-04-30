// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 383: Ransom Note
 * Difficulty: Easy
 * Tags: Hash Table, String, Counting
 *
 * Problem Description:
 * Given two strings `ransomNote` and `magazine`, return `true`
 * _if_`ransomNote`
 * _can be constructed by using the letters from_`magazine` _and_`false`
 * _otherwise_.
 * 
 * Each letter in `magazine` can only be used once in `ransomNote`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** ransomNote = "a", magazine = "b"
 *     **Output:** false
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** ransomNote = "aa", magazine = "ab"
 *     **Output:** false
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** ransomNote = "aa", magazine = "aab"
 *     **Output:** true
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= ransomNote.length, magazine.length <= 105`
 *   * `ransomNote` and `magazine` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/ransom-note/
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
    bool canConstruct(string ransomNote, string magazine) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "a"
    // Test case 2: "b"
    // Test case 3: "aa"
    // Test case 4: "ab"
    // Test case 5: "aa"
    // Test case 6: "aab"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
