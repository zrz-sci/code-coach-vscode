// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 451: Sort Characters By Frequency
 * Difficulty: Medium
 * Tags: Hash Table, String, Bucket Sort, Counting, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given a string `s`, sort it in **decreasing order** based on the
 * **frequency**
 * of the characters. The **frequency** of a character is the number of times
 * it
 * appears in the string.
 * 
 * Return _the sorted string_. If there are multiple answers, return _any of
 * them_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "tree"
 *     **Output:** "eert"
 *     **Explanation:** 'e' appears twice while 'r' and 't' both appear once.
 * So 'e' must appear before both 'r' and 't'. Therefore "eetr" is also a
 * valid answer.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "cccaaa"
 *     **Output:** "aaaccc"
 * **Explanation:** Both 'c' and 'a' appear three times, so both "cccaaa" and
 * "aaaccc" are valid answers.
 * Note that "cacaca" is incorrect, as the same characters must be together.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "Aabb"
 *     **Output:** "bbAa"
 *     **Explanation:** "bbaA" is also a valid answer, but "Aabb" is incorrect.
 *     Note that 'A' and 'a' are treated as two different characters.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 5 * 105`
 *   * `s` consists of uppercase and lowercase English letters and digits.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sort-characters-by-frequency/
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
    string frequencySort(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "tree"
    // Test case 2: "cccaaa"
    // Test case 3: "Aabb"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
