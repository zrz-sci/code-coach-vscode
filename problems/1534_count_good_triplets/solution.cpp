// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1534: Count Good Triplets
 * Difficulty: Easy
 * Tags: Array, Enumeration
 *
 * Problem Description:
 * Given an array of integers `arr`, and three integers `a`, `b` and `c`. You
 * need
 * to find the number of good triplets.
 * 
 * A triplet `(arr[i], arr[j], arr[k])` is **good** if the following
 * conditions are
 * true:
 * 
 *   * `0 <= i < j < k < arr.length`
 *   * `|arr[i] - arr[j]| <= a`
 *   * `|arr[j] - arr[k]| <= b`
 *   * `|arr[i] - arr[k]| <= c`
 * 
 * Where `|x|` denotes the absolute value of `x`.
 * 
 * Return _the number of good triplets_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [3,0,1,1,9,7], a = 7, b = 2, c = 3
 *     **Output:** 4
 * **Explanation:** There are 4 good triplets: [(3,0,1), (3,0,1), (3,1,1),
 * (0,1,1)].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [1,1,2,2,3], a = 0, b = 0, c = 1
 *     **Output:** 0
 *     **Explanation:** No triplet satisfies all conditions.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= arr.length <= 100`
 *   * `0 <= arr[i] <= 1000`
 *   * `0 <= a, b, c <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-good-triplets/
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
    int countGoodTriplets(vector<int>& arr, int a, int b, int c) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,0,1,1,9,7]
    // Test case 2: 7
    // Test case 3: 2
    // Test case 4: 3
    // Test case 5: [1,1,2,2,3]
    // Test case 6: 0
    // Test case 7: 0
    // Test case 8: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
