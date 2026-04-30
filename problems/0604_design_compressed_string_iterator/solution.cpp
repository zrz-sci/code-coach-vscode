// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 604: Design Compressed String Iterator
 * Difficulty: Easy
 * Tags: Design, Array, String, Iterator
 *
 * Problem Description:
 * Design and implement a data structure for a compressed string iterator. The
 * given compressed string will be in the form of each letter followed by a
 * positive integer representing the number of this letter existing in the
 * original
 * uncompressed string.
 * 
 * Implement the StringIterator class:
 * 
 * * `next()` Returns **the next character** if the original string still has
 * uncompressed characters, otherwise returns a **white space**.
 * * `hasNext()` Returns true if there is any letter needs to be uncompressed
 * in the original string, otherwise returns `false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 * ["StringIterator", "next", "next", "next", "next", "next", "next",
 * "hasNext", "next", "hasNext"]
 *     [["L1e2t1C1o1d1e1"], [], [], [], [], [], [], [], [], []]
 *     **Output**
 *     [null, "L", "e", "e", "t", "C", "o", true, "d", true]
 *     
 *     **Explanation**
 *     StringIterator stringIterator = new StringIterator("L1e2t1C1o1d1e1");
 *     stringIterator.next(); // return "L"
 *     stringIterator.next(); // return "e"
 *     stringIterator.next(); // return "e"
 *     stringIterator.next(); // return "t"
 *     stringIterator.next(); // return "C"
 *     stringIterator.next(); // return "o"
 *     stringIterator.hasNext(); // return True
 *     stringIterator.next(); // return "d"
 *     stringIterator.hasNext(); // return True
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= compressedString.length <= 1000`
 * * `compressedString` consists of lower-case an upper-case English letters
 * and digits.
 * * The number of a single character repetitions in `compressedString` is in
 * the range `[1, 10^9]`
 *   * At most `100` calls will be made to `next` and `hasNext`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/design-compressed-string-iterator/
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

class StringIterator {
public:
    StringIterator(string compressedString) {
        
    }
    
    char next() {
        
    }
    
    bool hasNext() {
        
    }
};

/**
 * Your StringIterator object will be instantiated and called as such:
 * StringIterator* obj = new StringIterator(compressedString);
 * char param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: StringIterator
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["StringIterator","next","next","next","next","next","next","hasNext","next","hasNext"]
    // [["L1e2t1C1o1d1e1"],[],[],[],[],[],[],[],[],[]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // StringIterator obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
