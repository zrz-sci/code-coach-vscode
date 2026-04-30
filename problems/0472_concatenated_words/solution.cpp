// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 472: Concatenated Words
 * Difficulty: Hard
 * Tags: Depth-First Search, Trie, Array, String, Dynamic Programming, Sorting
 *
 * Problem Description:
 * Given an array of strings `words` (**without duplicates**), return _all
 * the**concatenated words** in the given list of_ `words`.
 * 
 * A **concatenated word** is defined as a string that is comprised entirely
 * of at
 * least two shorter words (not necessarily distinct) in the given array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** words =
 * ["cat","cats","catsdogcats","dog","dogcatsdog","hippopotamuses","rat","ratcatdogcat"]
 *     **Output:** ["catsdogcats","dogcatsdog","ratcatdogcat"]
 * **Explanation:** "catsdogcats" can be concatenated by "cats", "dog" and
 * "cats";
 *     "dogcatsdog" can be concatenated by "dog", "cats" and "dog"; 
 *     "ratcatdogcat" can be concatenated by "rat", "cat", "dog" and "cat".
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["cat","dog","catdog"]
 *     **Output:** ["catdog"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 104`
 *   * `1 <= words[i].length <= 30`
 *   * `words[i]` consists of only lowercase English letters.
 *   * All the strings of `words` are **unique**.
 *   * `1 <= sum(words[i].length) <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/concatenated-words/
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
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["cat","cats","catsdogcats","dog","dogcatsdog","hippopotamuses","rat","ratcatdogcat"]
    // Test case 2: ["cat","dog","catdog"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
