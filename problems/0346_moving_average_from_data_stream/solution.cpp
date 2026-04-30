// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 346: Moving Average from Data Stream
 * Difficulty: Easy
 * Tags: Design, Queue, Array, Data Stream
 *
 * Problem Description:
 * Given a stream of integers and a window size, calculate the moving average
 * of
 * all integers in the sliding window.
 * 
 * Implement the `MovingAverage` class:
 * 
 * * `MovingAverage(int size)` Initializes the object with the size of the
 * window `size`.
 * * `double next(int val)` Returns the moving average of the last `size`
 * values of the stream.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["MovingAverage", "next", "next", "next", "next"]
 *     [[3], [1], [10], [3], [5]]
 *     **Output**
 *     [null, 1.0, 5.5, 4.66667, 6.0]
 *     
 *     **Explanation**
 *     MovingAverage movingAverage = new MovingAverage(3);
 *     movingAverage.next(1); // return 1.0 = 1 / 1
 *     movingAverage.next(10); // return 5.5 = (1 + 10) / 2
 *     movingAverage.next(3); // return 4.66667 = (1 + 10 + 3) / 3
 *     movingAverage.next(5); // return 6.0 = (10 + 3 + 5) / 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= size <= 1000`
 *   * `-105 <= val <= 105`
 *   * At most `104` calls will be made to `next`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/moving-average-from-data-stream/
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

class MovingAverage {
public:
    MovingAverage(int size) {
        
    }
    
    double next(int val) {
        
    }
};

/**
 * Your MovingAverage object will be instantiated and called as such:
 * MovingAverage* obj = new MovingAverage(size);
 * double param_1 = obj->next(val);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: MovingAverage
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["MovingAverage","next","next","next","next"]
    // [[3],[1],[10],[3],[5]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // MovingAverage obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
