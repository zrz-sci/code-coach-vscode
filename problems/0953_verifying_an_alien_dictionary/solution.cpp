// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 953: Verifying an Alien Dictionary
 * Difficulty: Easy
 * Tags: Array, Hash Table, String
 *
 * Problem Description:
 * In an alien language, surprisingly, they also use English lowercase
 * letters, but
 * possibly in a different `order`. The `order` of the alphabet is some
 * permutation
 * of lowercase letters.
 * 
 * Given a sequence of `words` written in the alien language, and the `order`
 * of
 * the alphabet, return `true` if and only if the given `words` are sorted
 * lexicographically in this alien language.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** words = ["hello","leetcode"], order =
 * "hlabcdefgijkmnopqrstuvwxyz"
 *     **Output:** true
 * **Explanation:** As 'h' comes before 'l' in this language, then the
 * sequence is sorted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** words = ["word","world","row"], order =
 * "worldabcefghijkmnpqstuvxyz"
 *     **Output:** false
 * **Explanation:** As 'd' comes after 'l' in this language, then words[0] >
 * words[1], hence the sequence is unsorted.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** words = ["apple","app"], order = "abcdefghijklmnopqrstuvwxyz"
 *     **Output:** false
 * **Explanation:** The first three characters "app" match, and the second
 * string is shorter (in size.) According to lexicographical rules "apple" >
 * "app", because 'l' > '∅', where '∅' is defined as the blank character which
 * is less than any other character ([More
 * info](https://en.wikipedia.org/wiki/Lexicographical_order)).
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 100`
 *   * `1 <= words[i].length <= 20`
 *   * `order.length == 26`
 *   * All characters in `words[i]` and `order` are English lowercase letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/verifying-an-alien-dictionary/
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
    bool isAlienSorted(vector<string>& words, string order) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["hello","leetcode"]
    // Test case 2: "hlabcdefgijkmnopqrstuvwxyz"
    // Test case 3: ["word","world","row"]
    // Test case 4: "worldabcefghijkmnpqstuvxyz"
    // Test case 5: ["apple","app"]
    // Test case 6: "abcdefghijklmnopqrstuvwxyz"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
