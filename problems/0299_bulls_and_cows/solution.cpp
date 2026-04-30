// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 299: Bulls and Cows
 * Difficulty: Medium
 * Tags: Hash Table, String, Counting
 *
 * Problem Description:
 * You are playing the **[Bulls and
 * Cows](https://en.wikipedia.org/wiki/Bulls_and_Cows)** game with your friend.
 * 
 * You write down a secret number and ask your friend to guess what the number
 * is.
 * When your friend makes a guess, you provide a hint with the following info:
 * 
 * * The number of "bulls", which are digits in the guess that are in the
 * correct position.
 * * The number of "cows", which are digits in the guess that are in your
 * secret number but are located in the wrong position. Specifically, the
 * non-bull digits in the guess that could be rearranged such that they become
 * bulls.
 * 
 * Given the secret number `secret` and your friend's guess `guess`, return
 * _the
 * hint for your friend 's guess_.
 * 
 * The hint should be formatted as `"xAyB"`, where `x` is the number of bulls
 * and
 * `y` is the number of cows. Note that both `secret` and `guess` may contain
 * duplicate digits.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** secret = "1807", guess = "7810"
 *     **Output:** "1A3B"
 *     **Explanation:** Bulls are connected with a '|' and cows are underlined:
 *     "1807"
 *       |
 *     "_7_ 8 _10_ "
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** secret = "1123", guess = "0111"
 *     **Output:** "1A1B"
 *     **Explanation:** Bulls are connected with a '|' and cows are underlined:
 *     "1123"        "1123"
 *       |      or     |
 *     "01 _1_ 1"        "011 _1_ "
 * Note that only one of the two unmatched 1s is counted as a cow since the
 * non-bull digits can only be rearranged to allow one 1 to be a bull.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= secret.length, guess.length <= 1000`
 *   * `secret.length == guess.length`
 *   * `secret` and `guess` consist of digits only.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/bulls-and-cows/
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
    string getHint(string secret, string guess) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "1807"
    // Test case 2: "7810"
    // Test case 3: "1123"
    // Test case 4: "0111"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
