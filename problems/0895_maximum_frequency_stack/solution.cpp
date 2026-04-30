// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 895: Maximum Frequency Stack
 * Difficulty: Hard
 * Tags: Stack, Design, Hash Table, Ordered Set
 *
 * Problem Description:
 * Design a stack-like data structure to push elements to the stack and pop the
 * most frequent element from the stack.
 * 
 * Implement the `FreqStack` class:
 * 
 *   * `FreqStack()` constructs an empty frequency stack.
 *   * `void push(int val)` pushes an integer `val` onto the top of the stack.
 *   * `int pop()` removes and returns the most frequent element in the stack. 
 * * If there is a tie for the most frequent element, the element closest to
 * the stack's top is removed and returned.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 * ["FreqStack", "push", "push", "push", "push", "push", "push", "pop", "pop",
 * "pop", "pop"]
 *     [[], [5], [7], [5], [7], [4], [5], [], [], [], []]
 *     **Output**
 *     [null, null, null, null, null, null, null, 5, 7, 5, 4]
 *     
 *     **Explanation**
 *     FreqStack freqStack = new FreqStack();
 *     freqStack.push(5); // The stack is [5]
 *     freqStack.push(7); // The stack is [5,7]
 *     freqStack.push(5); // The stack is [5,7,5]
 *     freqStack.push(7); // The stack is [5,7,5,7]
 *     freqStack.push(4); // The stack is [5,7,5,7,4]
 *     freqStack.push(5); // The stack is [5,7,5,7,4,5]
 * freqStack.pop(); // return 5, as 5 is the most frequent. The stack becomes
 * [5,7,5,7,4].
 * freqStack.pop(); // return 7, as 5 and 7 is the most frequent, but 7 is
 * closest to the top. The stack becomes [5,7,5,4].
 * freqStack.pop(); // return 5, as 5 is the most frequent. The stack becomes
 * [5,7,4].
 * freqStack.pop(); // return 4, as 4, 5 and 7 is the most frequent, but 4 is
 * closest to the top. The stack becomes [5,7].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= val <= 109`
 *   * At most `2 * 104` calls will be made to `push` and `pop`.
 * * It is guaranteed that there will be at least one element in the stack
 * before calling `pop`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-frequency-stack/
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

class FreqStack {
public:
    FreqStack() {
        
    }
    
    void push(int val) {
        
    }
    
    int pop() {
        
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: FreqStack
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["FreqStack","push","push","push","push","push","push","pop","pop","pop","pop"]
    // [[],[5],[7],[5],[7],[4],[5],[],[],[],[]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // FreqStack obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
