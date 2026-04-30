// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 242: Valid Anagram
 * Difficulty: Easy
 * Tags: Hash Table, String, Sorting
 *
 * Problem Description:
 * Given two strings `s` and `t`, return `true` if `t` is an anagram of `s`,
 * and
 * `false` otherwise.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s = "anagram", t = "nagaram"
 * 
 * **Output:** true
 * 
 * **Example 2:**
 * 
 * **Input:** s = "rat", t = "car"
 * 
 * **Output:** false
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length, t.length <= 5 * 104`
 *   * `s` and `t` consist of lowercase English letters.
 * 
 * 
 * 
 * **Follow up:** What if the inputs contain Unicode characters? How would you
 * adapt your solution to such a case?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-anagram/
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
    bool isAnagram(string s, string t) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "anagram"
    // Test case 2: "nagaram"
    // Test case 3: "rat"
    // Test case 4: "car"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
