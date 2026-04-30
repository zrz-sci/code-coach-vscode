// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 884: Uncommon Words from Two Sentences
 * Difficulty: Easy
 * Tags: Hash Table, String, Counting
 *
 * Problem Description:
 * A **sentence** is a string of single-space separated words where each word
 * consists only of lowercase letters.
 * 
 * A word is **uncommon** if it appears exactly once in one of the sentences,
 * and
 * **does not appear** in the other sentence.
 * 
 * Given two **sentences** `s1` and `s2`, return _a list of all the**uncommon
 * words**_. You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s1 = "this apple is sweet", s2 = "this apple is sour"
 * 
 * **Output:** ["sweet","sour"]
 * 
 * **Explanation:**
 * 
 * The word `"sweet"` appears only in `s1`, while the word `"sour"` appears
 * only in
 * `s2`.
 * 
 * **Example 2:**
 * 
 * **Input:** s1 = "apple apple", s2 = "banana"
 * 
 * **Output:** ["banana"]
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s1.length, s2.length <= 200`
 *   * `s1` and `s2` consist of lowercase English letters and spaces.
 *   * `s1` and `s2` do not have leading or trailing spaces.
 *   * All the words in `s1` and `s2` are separated by a single space.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/uncommon-words-from-two-sentences/
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
    vector<string> uncommonFromSentences(string s1, string s2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "this apple is sweet"
    // Test case 2: "this apple is sour"
    // Test case 3: "apple apple"
    // Test case 4: "banana"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
