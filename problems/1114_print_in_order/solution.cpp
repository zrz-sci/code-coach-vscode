// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1114: Print in Order
 * Difficulty: Easy
 * Tags: Concurrency
 *
 * Problem Description:
 * Suppose we have a class:
 * 
 *     
 *     
 *     public class Foo {
 *       public void first() { print("first"); }
 *       public void second() { print("second"); }
 *       public void third() { print("third"); }
 *     }
 *     
 * 
 * The same instance of `Foo` will be passed to three different threads.
 * Thread A
 * will call `first()`, thread B will call `second()`, and thread C will call
 * `third()`. Design a mechanism and modify the program to ensure that
 * `second()`
 * is executed after `first()`, and `third()` is executed after `second()`.
 * 
 * **Note:**
 * 
 * We do not know how the threads will be scheduled in the operating system,
 * even
 * though the numbers in the input seem to imply the ordering. The input
 * format you
 * see is mainly to ensure our tests' comprehensiveness.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3]
 *     **Output:** "firstsecondthird"
 * **Explanation:** There are three threads being fired asynchronously. The
 * input [1,2,3] means thread A calls first(), thread B calls second(), and
 * thread C calls third(). "firstsecondthird" is the correct output.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,2]
 *     **Output:** "firstsecondthird"
 * **Explanation:** The input [1,3,2] means thread A calls first(), thread B
 * calls third(), and thread C calls second(). "firstsecondthird" is the
 * correct output.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `nums` is a permutation of `[1, 2, 3]`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/print-in-order/
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

class Foo {
public:
    Foo() {
        
    }

    void first(function<void()> printFirst) {
        
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
    }

    void second(function<void()> printSecond) {
        
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
    }

    void third(function<void()> printThird) {
        
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Foo
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // [1,2,3]
    // [1,3,2]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Foo obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
