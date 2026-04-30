// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 336: Palindrome Pairs
 * Difficulty: Hard
 * Tags: Trie, Array, Hash Table, String, Hash Function
 *
 * Problem Description:
 * You are given a **0-indexed** array of **unique** strings `words`.
 * 
 * A **palindrome pair** is a pair of integers `(i, j)` such that:
 * 
 *   * `0 <= i, j < words.length`,
 *   * `i != j`, and
 * * `words[i] + words[j]` (the concatenation of the two strings) is a
 * palindrome.
 * 
 * Return _an array of all the**palindrome pairs** of _`words`.
 * 
 * You must write an algorithm with `O(sum of words[i].length)` runtime
 * complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["abcd","dcba","lls","s","sssll"]
 *     **Output:** [[0,1],[1,0],[3,2],[2,4]]
 * **Explanation:** The palindromes are
 * ["abcddcba","dcbaabcd","slls","llssssll"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["bat","tab","cat"]
 *     **Output:** [[0,1],[1,0]]
 *     **Explanation:** The palindromes are ["battab","tabbat"]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** words = ["a",""]
 *     **Output:** [[0,1],[1,0]]
 *     **Explanation:** The palindromes are ["a","a"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 5000`
 *   * `0 <= words[i].length <= 300`
 *   * `words[i]` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/palindrome-pairs/
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
    vector<vector<int>> palindromePairs(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["abcd","dcba","lls","s","sssll"]
    // Test case 2: ["bat","tab","cat"]
    // Test case 3: ["a",""]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
