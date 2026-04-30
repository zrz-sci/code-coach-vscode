// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sh

/*
 * LeetCode Problem 192: Word Frequency
 * Difficulty: Medium
 * Tags: Shell
 *
 * Problem Description:
 * Write a bash script to calculate the frequency of each word in a text file
 * `words.txt`.
 * 
 * For simplicity sake, you may assume:
 * 
 * * `words.txt` contains only lowercase characters and space `' '`
 * characters.
 *   * Each word must consist of lowercase characters only.
 *   * Words are separated by one or more whitespace characters.
 * 
 * **Example:**
 * 
 * Assume that `words.txt` has the following content:
 * 
 *     
 *     
 *     the day is sunny the the
 *     the sunny is is
 *     
 * 
 * Your script should output the following, sorted by descending frequency:
 * 
 *     
 *     
 *     the 4
 *     is 3
 *     sunny 2
 *     day 1
 *     
 * 
 * **Note:**
 * 
 * * Don't worry about handling ties, it is guaranteed that each word's
 * frequency count is unique.
 * * Could you write it in one-line using [Unix
 * pipes](http://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO-4.html)?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-frequency/
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



// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Solution
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // a

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
