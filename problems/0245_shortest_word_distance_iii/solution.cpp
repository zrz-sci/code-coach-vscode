// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 245: Shortest Word Distance III
 * Difficulty: Medium
 * Tags: Array, String
 *
 * Problem Description:
 * Given an array of strings `wordsDict` and two strings that already exist in
 * the
 * array `word1` and `word2`, return _the shortest distance between the
 * occurrence
 * of these two words in the list_.
 * 
 * **Note** that `word1` and `word2` may be the same. It is guaranteed that
 * they
 * represent **two individual words** in the list.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** wordsDict = ["practice", "makes", "perfect", "coding", "makes"],
 * word1 = "makes", word2 = "coding"
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** wordsDict = ["practice", "makes", "perfect", "coding", "makes"],
 * word1 = "makes", word2 = "makes"
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= wordsDict.length <= 105`
 *   * `1 <= wordsDict[i].length <= 10`
 *   * `wordsDict[i]` consists of lowercase English letters.
 *   * `word1` and `word2` are in `wordsDict`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-word-distance-iii/
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
    int shortestWordDistance(vector<string>& wordsDict, string word1, string word2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["practice", "makes", "perfect", "coding", "makes"]
    // Test case 2: "makes"
    // Test case 3: "coding"
    // Test case 4: ["practice", "makes", "perfect", "coding", "makes"]
    // Test case 5: "makes"
    // Test case 6: "makes"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
