// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 819: Most Common Word
 * Difficulty: Easy
 * Tags: Array, Hash Table, String, Counting
 *
 * Problem Description:
 * Given a string `paragraph` and a string array of the banned words `banned`,
 * return _the most frequent word that is not banned_. It is **guaranteed**
 * there
 * is **at least one word** that is not banned, and that the answer is
 * **unique**.
 * 
 * The words in `paragraph` are **case-insensitive** and the answer should be
 * returned in **lowercase**.
 * 
 * **Note** that words can not contain punctuation symbols.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** paragraph = "Bob hit a ball, the hit BALL flew far after it was
 * hit.", banned = ["hit"]
 *     **Output:** "ball"
 *     **Explanation:** 
 *     "hit" occurs 3 times, but it is a banned word.
 * "ball" occurs twice (and no other word does), so it is the most frequent
 * non-banned word in the paragraph.
 *     Note that words in the paragraph are not case sensitive,
 * that punctuation is ignored (even if adjacent to words, such as "ball,"),
 * and that "hit" isn't the answer even though it occurs more because it is
 * banned.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** paragraph = "a.", banned = []
 *     **Output:** "a"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= paragraph.length <= 1000`
 * * paragraph consists of English letters, space `' '`, or one of the
 * symbols: `"!?',;."`.
 *   * `0 <= banned.length <= 100`
 *   * `1 <= banned[i].length <= 10`
 *   * `banned[i]` consists of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/most-common-word/
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
    string mostCommonWord(string paragraph, vector<string>& banned) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "Bob hit a ball, the hit BALL flew far after it was hit."
    // Test case 2: ["hit"]
    // Test case 3: "a."
    // Test case 4: []

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
