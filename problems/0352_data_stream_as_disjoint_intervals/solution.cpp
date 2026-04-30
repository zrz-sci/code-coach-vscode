// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 352: Data Stream as Disjoint Intervals
 * Difficulty: Hard
 * Tags: Union Find, Design, Hash Table, Binary Search, Data Stream, Ordered Set
 *
 * Problem Description:
 * Given a data stream input of non-negative integers `a1, a2, ..., an`,
 * summarize
 * the numbers seen so far as a list of disjoint intervals.
 * 
 * Implement the `SummaryRanges` class:
 * 
 *   * `SummaryRanges()` Initializes the object with an empty stream.
 *   * `void addNum(int value)` Adds the integer `value` to the stream.
 * * `int[][] getIntervals()` Returns a summary of the integers in the stream
 * currently as a list of disjoint intervals `[starti, endi]`. The answer
 * should be sorted by `starti`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 * ["SummaryRanges", "addNum", "getIntervals", "addNum", "getIntervals",
 * "addNum", "getIntervals", "addNum", "getIntervals", "addNum",
 * "getIntervals"]
 *     [[], [1], [], [3], [], [7], [], [2], [], [6], []]
 *     **Output**
 * [null, null, [[1, 1]], null, [[1, 1], [3, 3]], null, [[1, 1], [3, 3], [7,
 * 7]], null, [[1, 3], [7, 7]], null, [[1, 3], [6, 7]]]
 *     
 *     **Explanation**
 *     SummaryRanges summaryRanges = new SummaryRanges();
 *     summaryRanges.addNum(1);      // arr = [1]
 *     summaryRanges.getIntervals(); // return [[1, 1]]
 *     summaryRanges.addNum(3);      // arr = [1, 3]
 *     summaryRanges.getIntervals(); // return [[1, 1], [3, 3]]
 *     summaryRanges.addNum(7);      // arr = [1, 3, 7]
 *     summaryRanges.getIntervals(); // return [[1, 1], [3, 3], [7, 7]]
 *     summaryRanges.addNum(2);      // arr = [1, 2, 3, 7]
 *     summaryRanges.getIntervals(); // return [[1, 3], [7, 7]]
 *     summaryRanges.addNum(6);      // arr = [1, 2, 3, 6, 7]
 *     summaryRanges.getIntervals(); // return [[1, 3], [6, 7]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= value <= 104`
 *   * At most `3 * 104` calls will be made to `addNum` and `getIntervals`.
 *   * At most `102` calls will be made to `getIntervals`.
 * 
 * 
 * 
 * **Follow up:** What if there are lots of merges and the number of disjoint
 * intervals is small compared to the size of the data stream?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/data-stream-as-disjoint-intervals/
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

class SummaryRanges {
public:
    SummaryRanges() {
        
    }
    
    void addNum(int value) {
        
    }
    
    vector<vector<int>> getIntervals() {
        
    }
};

/**
 * Your SummaryRanges object will be instantiated and called as such:
 * SummaryRanges* obj = new SummaryRanges();
 * obj->addNum(value);
 * vector<vector<int>> param_2 = obj->getIntervals();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: SummaryRanges
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["SummaryRanges","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals"]
    // [[],[1],[],[3],[],[7],[],[2],[],[6],[]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // SummaryRanges obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
