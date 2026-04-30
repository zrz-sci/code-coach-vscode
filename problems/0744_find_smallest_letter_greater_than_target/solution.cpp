// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 744: Find Smallest Letter Greater Than Target
 * Difficulty: Easy
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * You are given an array of characters `letters` that is sorted in **non-
 * decreasing order** , and a character `target`. There are **at least two
 * different** characters in `letters`.
 * 
 * Return _the smallest character in_`letters` _that is lexicographically
 * greater
 * than_`target`. If such a character does not exist, return the first
 * character in
 * `letters`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** letters = ["c","f","j"], target = "a"
 *     **Output:** "c"
 * **Explanation:** The smallest character that is lexicographically greater
 * than 'a' in letters is 'c'.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** letters = ["c","f","j"], target = "c"
 *     **Output:** "f"
 * **Explanation:** The smallest character that is lexicographically greater
 * than 'c' in letters is 'f'.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** letters = ["x","x","y","y"], target = "z"
 *     **Output:** "x"
 * **Explanation:** There are no characters in letters that is
 * lexicographically greater than 'z' so we return letters[0].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= letters.length <= 104`
 *   * `letters[i]` is a lowercase English letter.
 *   * `letters` is sorted in **non-decreasing** order.
 *   * `letters` contains at least two different characters.
 *   * `target` is a lowercase English letter.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-smallest-letter-greater-than-target/
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
    char nextGreatestLetter(vector<char>& letters, char target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["c","f","j"]
    // Test case 2: "a"
    // Test case 3: ["c","f","j"]
    // Test case 4: "c"
    // Test case 5: ["x","x","y","y"]
    // Test case 6: "z"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
