// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sh

/*
 * LeetCode Problem 193: Valid Phone Numbers
 * Difficulty: Easy
 * Tags: Shell
 *
 * Problem Description:
 * Given a text file `file.txt` that contains a list of phone numbers (one per
 * line), write a one-liner bash script to print all valid phone numbers.
 * 
 * You may assume that a valid phone number must appear in one of the
 * following two
 * formats: (xxx) xxx-xxxx or xxx-xxx-xxxx. (x means a digit)
 * 
 * You may also assume each line in the text file must not contain leading or
 * trailing white spaces.
 * 
 * **Example:**
 * 
 * Assume that `file.txt` has the following content:
 * 
 *     
 *     
 *     987-123-4567
 *     123 456 7890
 *     (123) 456-7890
 *     
 * 
 * Your script should output the following valid phone numbers:
 * 
 *     
 *     
 *     987-123-4567
 *     (123) 456-7890
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-phone-numbers/
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
    // 0

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
