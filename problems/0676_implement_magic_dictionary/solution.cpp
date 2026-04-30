// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 676: Implement Magic Dictionary
 * Difficulty: Medium
 * Tags: Depth-First Search, Design, Trie, Hash Table, String
 *
 * Problem Description:
 * Design a data structure that is initialized with a list of **different**
 * words.
 * Provided a string, you should determine if you can change exactly one
 * character
 * in this string to match any word in the data structure.
 * 
 * Implement the `MagicDictionary` class:
 * 
 *   * `MagicDictionary()` Initializes the object.
 * * `void buildDict(String[] dictionary)` Sets the data structure with an
 * array of distinct strings `dictionary`.
 * * `bool search(String searchWord)` Returns `true` if you can change
 * **exactly one character** in `searchWord` to match any string in the data
 * structure, otherwise returns `false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["MagicDictionary", "buildDict", "search", "search", "search", "search"]
 * [[], [["hello", "leetcode"]], ["hello"], ["hhllo"], ["hell"],
 * ["leetcoded"]]
 *     **Output**
 *     [null, null, false, true, false, false]
 *     
 *     **Explanation**
 *     MagicDictionary magicDictionary = new MagicDictionary();
 *     magicDictionary.buildDict(["hello", "leetcode"]);
 *     magicDictionary.search("hello"); // return False
 * magicDictionary.search("hhllo"); // We can change the second 'h' to 'e' to
 * match "hello" so we return True
 *     magicDictionary.search("hell"); // return False
 *     magicDictionary.search("leetcoded"); // return False
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= dictionary.length <= 100`
 *   * `1 <= dictionary[i].length <= 100`
 *   * `dictionary[i]` consists of only lower-case English letters.
 *   * All the strings in `dictionary` are **distinct**.
 *   * `1 <= searchWord.length <= 100`
 *   * `searchWord` consists of only lower-case English letters.
 *   * `buildDict` will be called only once before `search`.
 *   * At most `100` calls will be made to `search`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/implement-magic-dictionary/
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

class MagicDictionary {
public:
    MagicDictionary() {
        
    }
    
    void buildDict(vector<string> dictionary) {
        
    }
    
    bool search(string searchWord) {
        
    }
};

/**
 * Your MagicDictionary object will be instantiated and called as such:
 * MagicDictionary* obj = new MagicDictionary();
 * obj->buildDict(dictionary);
 * bool param_2 = obj->search(searchWord);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: MagicDictionary
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["MagicDictionary", "buildDict", "search", "search", "search", "search"]
    // [[], [["hello","leetcode"]], ["hello"], ["hhllo"], ["hell"], ["leetcoded"]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // MagicDictionary obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
