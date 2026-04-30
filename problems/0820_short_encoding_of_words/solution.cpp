// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 820: Short Encoding of Words
 * Difficulty: Medium
 * Tags: Trie, Array, Hash Table, String
 *
 * Problem Description:
 * A **valid encoding** of an array of `words` is any reference string `s` and
 * array of indices `indices` such that:
 * 
 *   * `words.length == indices.length`
 *   * The reference string `s` ends with the `'#'` character.
 * * For each index `indices[i]`, the **substring** of `s` starting from
 * `indices[i]` and up to (but not including) the next `'#'` character is equal
 * to `words[i]`.
 * 
 * Given an array of `words`, return _the**length of the shortest reference
 * string** _`s` _possible of any**valid encoding** of _`words` _._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["time", "me", "bell"]
 *     **Output:** 10
 * **Explanation:** A valid encoding would be s = "time#bell#" and indices =
 * [0, 2, 5].
 * words[0] = "time", the substring of s starting from indices[0] = 0 to the
 * next '#' is underlined in "_time_ #bell#"
 * words[1] = "me", the substring of s starting from indices[1] = 2 to the
 * next '#' is underlined in "ti _me_ #bell#"
 * words[2] = "bell", the substring of s starting from indices[2] = 5 to the
 * next '#' is underlined in "time#_bell_ #"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["t"]
 *     **Output:** 2
 *     **Explanation:** A valid encoding would be s = "t#" and indices = [0].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 2000`
 *   * `1 <= words[i].length <= 7`
 *   * `words[i]` consists of only lowercase letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/short-encoding-of-words/
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
    int minimumLengthEncoding(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["time","me","bell"]
    // Test case 2: ["t"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
