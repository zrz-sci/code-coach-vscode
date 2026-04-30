// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 243: Shortest Word Distance
 * Difficulty: Easy
 * Tags: Array, String
 *
 * Problem Description:
 * Given an array of strings `wordsDict` and two different strings that already
 * exist in the array `word1` and `word2`, return _the shortest distance
 * between
 * these two words in the list_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** wordsDict = ["practice", "makes", "perfect", "coding", "makes"],
 * word1 = "coding", word2 = "practice"
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** wordsDict = ["practice", "makes", "perfect", "coding", "makes"],
 * word1 = "makes", word2 = "coding"
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= wordsDict.length <= 3 * 104`
 *   * `1 <= wordsDict[i].length <= 10`
 *   * `wordsDict[i]` consists of lowercase English letters.
 *   * `word1` and `word2` are in `wordsDict`.
 *   * `word1 != word2`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-word-distance/
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
    int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["practice", "makes", "perfect", "coding", "makes"]
    // Test case 2: "coding"
    // Test case 3: "practice"
    // Test case 4: ["practice", "makes", "perfect", "coding", "makes"]
    // Test case 5: "makes"
    // Test case 6: "coding"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
