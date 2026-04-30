// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 318: Maximum Product of Word Lengths
 * Difficulty: Medium
 * Tags: Bit Manipulation, Array, String
 *
 * Problem Description:
 * Given a string array `words`, return _the maximum value of_
 * `length(word[i]) *
 * length(word[j])` _where the two words do not share common letters_. If no
 * such
 * two words exist, return `0`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["abcw","baz","foo","bar","xtfn","abcdef"]
 *     **Output:** 16
 *     **Explanation:** The two words can be "abcw", "xtfn".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["a","ab","abc","d","cd","bcd","abcd"]
 *     **Output:** 4
 *     **Explanation:** The two words can be "ab", "cd".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** words = ["a","aa","aaa","aaaa"]
 *     **Output:** 0
 *     **Explanation:** No such pair of words.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= words.length <= 1000`
 *   * `1 <= words[i].length <= 1000`
 *   * `words[i]` consists only of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-product-of-word-lengths/
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
    int maxProduct(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["abcw","baz","foo","bar","xtfn","abcdef"]
    // Test case 2: ["a","ab","abc","d","cd","bcd","abcd"]
    // Test case 3: ["a","aa","aaa","aaaa"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
