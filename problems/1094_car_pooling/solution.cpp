// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1094: Car Pooling
 * Difficulty: Medium
 * Tags: Array, Prefix Sum, Sorting, Simulation, Heap (Priority Queue)
 *
 * Problem Description:
 * There is a car with `capacity` empty seats. The vehicle only drives east
 * (i.e.,
 * it cannot turn around and drive west).
 * 
 * You are given the integer `capacity` and an array `trips` where `trips[i] =
 * [numPassengersi, fromi, toi]` indicates that the `ith` trip has
 * `numPassengersi`
 * passengers and the locations to pick them up and drop them off are `fromi`
 * and
 * `toi` respectively. The locations are given as the number of kilometers due
 * east
 * from the car's initial location.
 * 
 * Return `true` _if it is possible to pick up and drop off all passengers for
 * all
 * the given trips, or_`false` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** trips = [[2,1,5],[3,3,7]], capacity = 4
 *     **Output:** false
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** trips = [[2,1,5],[3,3,7]], capacity = 5
 *     **Output:** true
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= trips.length <= 1000`
 *   * `trips[i].length == 3`
 *   * `1 <= numPassengersi <= 100`
 *   * `0 <= fromi < toi <= 1000`
 *   * `1 <= capacity <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/car-pooling/
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
    bool carPooling(vector<vector<int>>& trips, int capacity) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[2,1,5],[3,3,7]]
    // Test case 2: 4
    // Test case 3: [[2,1,5],[3,3,7]]
    // Test case 4: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
