// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 269: Alien Dictionary
 * Difficulty: Hard
 * Tags: Depth-First Search, Breadth-First Search, Graph, Topological Sort, Array, String
 *
 * Problem Description:
 * There is a new alien language that uses the English alphabet. However, the
 * order
 * of the letters is unknown to you.
 * 
 * You are given a list of strings `words` from the alien language's
 * dictionary.
 * Now it is claimed that the strings in `words` are **sorted
 * lexicographically**
 * by the rules of this new language.
 * 
 * If this claim is incorrect, and the given arrangement of string in `words`
 * cannot correspond to any order of letters, return `"".`
 * 
 * Otherwise, return _a string of the unique letters in the new alien language
 * sorted in**lexicographically increasing order** by the new language's
 * rules_ _._
 * If there are multiple solutions, return _**any of them**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["wrt","wrf","er","ett","rftt"]
 *     **Output:** "wertf"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["z","x"]
 *     **Output:** "zx"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** words = ["z","x","z"]
 *     **Output:** ""
 *     **Explanation:** The order is invalid, so return "".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 100`
 *   * `1 <= words[i].length <= 100`
 *   * `words[i]` consists of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/alien-dictionary/
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
    string alienOrder(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["wrt","wrf","er","ett","rftt"]
    // Test case 2: ["z","x"]
    // Test case 3: ["z","x","z"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
