// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 732: My Calendar III
 * Difficulty: Hard
 * Tags: Design, Segment Tree, Binary Search, Ordered Set, Prefix Sum
 *
 * Problem Description:
 * A `k`-booking happens when `k` events have some non-empty intersection
 * (i.e.,
 * there is some time that is common to all `k` events.)
 * 
 * You are given some events `[startTime, endTime)`, after each given event,
 * return
 * an integer `k` representing the maximum `k`-booking between all the previous
 * events.
 * 
 * Implement the `MyCalendarThree` class:
 * 
 *   * `MyCalendarThree()` Initializes the object.
 * * `int book(int startTime, int endTime)` Returns an integer `k`
 * representing the largest integer such that there exists a `k`-booking in the
 * calendar.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["MyCalendarThree", "book", "book", "book", "book", "book", "book"]
 *     [[], [10, 20], [50, 60], [10, 40], [5, 15], [5, 10], [25, 55]]
 *     **Output**
 *     [null, 1, 1, 2, 3, 3, 3]
 *     
 *     **Explanation**
 *     MyCalendarThree myCalendarThree = new MyCalendarThree();
 *     myCalendarThree.book(10, 20); // return 1
 *     myCalendarThree.book(50, 60); // return 1
 *     myCalendarThree.book(10, 40); // return 2
 *     myCalendarThree.book(5, 15); // return 3
 *     myCalendarThree.book(5, 10); // return 3
 *     myCalendarThree.book(25, 55); // return 3
 *     
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= startTime < endTime <= 109`
 *   * At most `400` calls will be made to `book`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/my-calendar-iii/
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

class MyCalendarThree {
public:
    MyCalendarThree() {
        
    }
    
    int book(int startTime, int endTime) {
        
    }
};

/**
 * Your MyCalendarThree object will be instantiated and called as such:
 * MyCalendarThree* obj = new MyCalendarThree();
 * int param_1 = obj->book(startTime,endTime);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: MyCalendarThree
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["MyCalendarThree","book","book","book","book","book","book"]
    // [[],[10,20],[50,60],[10,40],[5,15],[5,10],[25,55]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // MyCalendarThree obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
