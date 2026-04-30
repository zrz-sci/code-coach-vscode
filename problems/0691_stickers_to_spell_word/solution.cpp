// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 691: Stickers to Spell Word
 * Difficulty: Hard
 * Tags: Bit Manipulation, Memoization, Array, Hash Table, String, Dynamic Programming, Backtracking, Bitmask
 *
 * Problem Description:
 * We are given `n` different types of `stickers`. Each sticker has a lowercase
 * English word on it.
 * 
 * You would like to spell out the given string `target` by cutting individual
 * letters from your collection of stickers and rearranging them. You can use
 * each
 * sticker more than once if you want, and you have infinite quantities of each
 * sticker.
 * 
 * Return _the minimum number of stickers that you need to spell out_`target`.
 * If
 * the task is impossible, return `-1`.
 * 
 * **Note:** In all test cases, all words were chosen randomly from the `1000`
 * most
 * common US English words, and `target` was chosen as a concatenation of two
 * random words.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** stickers = ["with","example","science"], target = "thehat"
 *     **Output:** 3
 *     **Explanation:**
 *     We can use 2 "with" stickers, and 1 "example" sticker.
 * After cutting and rearrange the letters of those stickers, we can form the
 * target "thehat".
 * Also, this is the minimum number of stickers necessary to form the target
 * string.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** stickers = ["notice","possible"], target = "basicbasic"
 *     **Output:** -1
 *     Explanation:
 * We cannot form the target "basicbasic" from cutting letters from the given
 * stickers.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == stickers.length`
 *   * `1 <= n <= 50`
 *   * `1 <= stickers[i].length <= 10`
 *   * `1 <= target.length <= 15`
 *   * `stickers[i]` and `target` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/stickers-to-spell-word/
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
    int minStickers(vector<string>& stickers, string target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["with","example","science"]
    // Test case 2: "thehat"
    // Test case 3: ["notice","possible"]
    // Test case 4: "basicbasic"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
