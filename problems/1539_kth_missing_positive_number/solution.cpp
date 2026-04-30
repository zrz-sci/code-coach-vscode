// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1539: Kth Missing Positive Number
 * Difficulty: Easy
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * Given an array `arr` of positive integers sorted in a **strictly increasing
 * order** , and an integer `k`.
 * 
 * Return _the_ `kth` _**positive** integer that is **missing** from this
 * array._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [2,3,4,7,11], k = 5
 *     **Output:** 9
 * **Explanation:** The missing positive integers are
 * [1,5,6,8,9,10,12,13,...]. The 5th missing positive integer is 9.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [1,2,3,4], k = 2
 *     **Output:** 6
 * **Explanation:** The missing positive integers are [5,6,7,...]. The 2nd
 * missing positive integer is 6.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= arr.length <= 1000`
 *   * `1 <= arr[i] <= 1000`
 *   * `1 <= k <= 1000`
 *   * `arr[i] < arr[j]` for `1 <= i < j <= arr.length`
 * 
 * 
 * 
 * **Follow up:**
 * 
 * Could you solve this problem in less than O(n) complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/kth-missing-positive-number/
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
    int findKthPositive(vector<int>& arr, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,3,4,7,11]
    // Test case 2: 5
    // Test case 3: [1,2,3,4]
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
