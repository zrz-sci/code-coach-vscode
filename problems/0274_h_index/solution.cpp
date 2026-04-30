// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 274: H-Index
 * Difficulty: Medium
 * Tags: Array, Counting Sort, Sorting
 *
 * Problem Description:
 * Given an array of integers `citations` where `citations[i]` is the number of
 * citations a researcher received for their `ith` paper, return _the
 * researcher 's
 * h-index_.
 * 
 * According to the [definition of h-index on
 * Wikipedia](https://en.wikipedia.org/wiki/H-index): The h-index is defined
 * as the
 * maximum value of `h` such that the given researcher has published at least
 * `h`
 * papers that have each been cited at least `h` times.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** citations = [3,0,6,1,5]
 *     **Output:** 3
 * **Explanation:** [3,0,6,1,5] means the researcher has 5 papers in total and
 * each of them had received 3, 0, 6, 1, 5 citations respectively.
 * Since the researcher has 3 papers with at least 3 citations each and the
 * remaining two with no more than 3 citations each, their h-index is 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** citations = [1,3,1]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == citations.length`
 *   * `1 <= n <= 5000`
 *   * `0 <= citations[i] <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/h-index/
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

class Solution {
public:
    int hIndex(vector<int>& citations) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,0,6,1,5]
    // Test case 2: [1,3,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
