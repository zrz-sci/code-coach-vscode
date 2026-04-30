// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 126: Word Ladder II
 * Difficulty: Hard
 * Tags: Breadth-First Search, Hash Table, String, Backtracking
 *
 * Problem Description:
 * A **transformation sequence** from word `beginWord` to word `endWord` using
 * a
 * dictionary `wordList` is a sequence of words `beginWord -> s1 -> s2 -> ...
 * ->
 * sk` such that:
 * 
 *   * Every adjacent pair of words differs by a single letter.
 * * Every `si` for `1 <= i <= k` is in `wordList`. Note that `beginWord` does
 * not need to be in `wordList`.
 *   * `sk == endWord`
 * 
 * Given two words, `beginWord` and `endWord`, and a dictionary `wordList`,
 * return
 * _all the**shortest transformation sequences** from_ `beginWord` _to_
 * `endWord`
 * _, or an empty list if no such sequence exists. Each sequence should be
 * returned
 * as a list of the words_`[beginWord, s1, s2, ..., sk]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** beginWord = "hit", endWord = "cog", wordList =
 * ["hot","dot","dog","lot","log","cog"]
 * **Output:**
 * [["hit","hot","dot","dog","cog"],["hit","hot","lot","log","cog"]]
 *     **Explanation:**  There are 2 shortest transformation sequences:
 *     "hit" -> "hot" -> "dot" -> "dog" -> "cog"
 *     "hit" -> "hot" -> "lot" -> "log" -> "cog"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** beginWord = "hit", endWord = "cog", wordList =
 * ["hot","dot","dog","lot","log"]
 *     **Output:** []
 * **Explanation:** The endWord "cog" is not in wordList, therefore there is
 * no valid transformation sequence.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= beginWord.length <= 5`
 *   * `endWord.length == beginWord.length`
 *   * `1 <= wordList.length <= 500`
 *   * `wordList[i].length == beginWord.length`
 * * `beginWord`, `endWord`, and `wordList[i]` consist of lowercase English
 * letters.
 *   * `beginWord != endWord`
 *   * All the words in `wordList` are **unique**.
 * * The **sum** of all shortest transformation sequences does not exceed
 * `105`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-ladder-ii/
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
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "hit"
    // Test case 2: "cog"
    // Test case 3: ["hot","dot","dog","lot","log","cog"]
    // Test case 4: "hit"
    // Test case 5: "cog"
    // Test case 6: ["hot","dot","dog","lot","log"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
