// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 745: Prefix and Suffix Search
 * Difficulty: Hard
 * Tags: Design, Trie, Array, Hash Table, String
 *
 * Problem Description:
 * Design a special dictionary that searches the words in it by a prefix and a
 * suffix.
 * 
 * Implement the `WordFilter` class:
 * 
 * * `WordFilter(string[] words)` Initializes the object with the `words` in
 * the dictionary.
 * * `f(string pref, string suff)` Returns _the index of the word in the
 * dictionary,_ which has the prefix `pref` and the suffix `suff`. If there is
 * more than one valid index, return **the largest** of them. If there is no
 * such word in the dictionary, return `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["WordFilter", "f"]
 *     [[["apple"]], ["a", "e"]]
 *     **Output**
 *     [null, 0]
 *     **Explanation**
 *     WordFilter wordFilter = new WordFilter(["apple"]);
 * wordFilter.f("a", "e"); // return 0, because the word at index 0 has prefix
 * = "a" and suffix = "e".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 104`
 *   * `1 <= words[i].length <= 7`
 *   * `1 <= pref.length, suff.length <= 7`
 *   * `words[i]`, `pref` and `suff` consist of lowercase English letters only.
 *   * At most `104` calls will be made to the function `f`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/prefix-and-suffix-search/
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

class WordFilter {
public:
    WordFilter(vector<string>& words) {
        
    }
    
    int f(string pref, string suff) {
        
    }
};

/**
 * Your WordFilter object will be instantiated and called as such:
 * WordFilter* obj = new WordFilter(words);
 * int param_1 = obj->f(pref,suff);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: WordFilter
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["WordFilter","f"]
    // [[["apple"]],["a","e"]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // WordFilter obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
