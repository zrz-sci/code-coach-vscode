// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1776: Car Fleet II
 * Difficulty: Hard
 * Tags: Stack, Array, Math, Monotonic Stack, Heap (Priority Queue)
 *
 * Problem Description:
 * There are `n` cars traveling at different speeds in the same direction
 * along a
 * one-lane road. You are given an array `cars` of length `n`, where `cars[i] =
 * [positioni, speedi]` represents:
 * 
 * * `positioni` is the distance between the `ith` car and the beginning of
 * the road in meters. It is guaranteed that `positioni < positioni+1`.
 *   * `speedi` is the initial speed of the `ith` car in meters per second.
 * 
 * For simplicity, cars can be considered as points moving along the number
 * line.
 * Two cars collide when they occupy the same position. Once a car collides
 * with
 * another car, they unite and form a single car fleet. The cars in the formed
 * fleet will have the same position and the same speed, which is the initial
 * speed
 * of the **slowest** car in the fleet.
 * 
 * Return an array `answer`, where `answer[i]` is the time, in seconds, at
 * which
 * the `ith` car collides with the next car, or `-1` if the car does not
 * collide
 * with the next car. Answers within `10-5` of the actual answers are accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** cars = [[1,2],[2,1],[4,3],[7,2]]
 *     **Output:** [1.00000,-1.00000,3.00000,-1.00000]
 * **Explanation:** After exactly one second, the first car will collide with
 * the second car, and form a car fleet with speed 1 m/s. After exactly 3
 * seconds, the third car will collide with the fourth car, and form a car
 * fleet with speed 2 m/s.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** cars = [[3,4],[5,4],[6,3],[9,1]]
 *     **Output:** [2.00000,1.00000,1.50000,-1.00000]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= cars.length <= 105`
 *   * `1 <= positioni, speedi <= 106`
 *   * `positioni < positioni+1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/car-fleet-ii/
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
    vector<double> getCollisionTimes(vector<vector<int>>& cars) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[2,1],[4,3],[7,2]]
    // Test case 2: [[3,4],[5,4],[6,3],[9,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
