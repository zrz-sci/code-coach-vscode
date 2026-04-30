// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 187: Repeated DNA Sequences
 * Difficulty: Medium
 * Tags: Bit Manipulation, Hash Table, String, Sliding Window, Hash Function, Rolling Hash
 *
 * Problem Description:
 * The **DNA sequence** is composed of a series of nucleotides abbreviated as
 * `'A'`, `'C'`, `'G'`, and `'T'`.
 * 
 *   * For example, `"ACGAATTCCG"` is a **DNA sequence**.
 * 
 * When studying **DNA** , it is useful to identify repeated sequences within
 * the
 * DNA.
 * 
 * Given a string `s` that represents a **DNA sequence** , return all the
 * **`10`-letter-long** sequences (substrings) that occur more than once in a
 * DNA
 * molecule. You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"
 *     **Output:** ["AAAAACCCCC","CCCCCAAAAA"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "AAAAAAAAAAAAA"
 *     **Output:** ["AAAAAAAAAA"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s[i]` is either `'A'`, `'C'`, `'G'`, or `'T'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/repeated-dna-sequences/
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
    vector<string> findRepeatedDnaSequences(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"
    // Test case 2: "AAAAAAAAAAAAA"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
