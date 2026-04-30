// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 49: Group Anagrams
 * Difficulty: Medium
 * Tags: Array, Hash Table, String, Sorting
 *
 * Problem Description:
 * Given an array of strings `strs`, group the anagrams together. You can
 * return
 * the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** strs = ["eat","tea","tan","ate","nat","bat"]
 * 
 * **Output:** [["bat"],["nat","tan"],["ate","eat","tea"]]
 * 
 * **Explanation:**
 * 
 *   * There is no string in strs that can be rearranged to form `"bat"`.
 * * The strings `"nat"` and `"tan"` are anagrams as they can be rearranged to
 * form each other.
 * * The strings `"ate"`, `"eat"`, and `"tea"` are anagrams as they can be
 * rearranged to form each other.
 * 
 * **Example 2:**
 * 
 * **Input:** strs = [""]
 * 
 * **Output:** [[""]]
 * 
 * **Example 3:**
 * 
 * **Input:** strs = ["a"]
 * 
 * **Output:** [["a"]]
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= strs.length <= 104`
 *   * `0 <= strs[i].length <= 100`
 *   * `strs[i]` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/group-anagrams/
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
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["eat","tea","tan","ate","nat","bat"]
    // Test case 2: [""]
    // Test case 3: ["a"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
