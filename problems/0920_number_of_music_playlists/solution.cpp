// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 920: Number of Music Playlists
 * Difficulty: Hard
 * Tags: Math, Dynamic Programming, Combinatorics
 *
 * Problem Description:
 * Your music player contains `n` different songs. You want to listen to `goal`
 * songs (not necessarily different) during your trip. To avoid boredom, you
 * will
 * create a playlist so that:
 * 
 *   * Every song is played **at least once**.
 * * A song can only be played again only if `k` other songs have been played.
 * 
 * Given `n`, `goal`, and `k`, return _the number of possible playlists that
 * you
 * can create_. Since the answer can be very large, return it **modulo** `109
 * + 7`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3, goal = 3, k = 1
 *     **Output:** 6
 * **Explanation:** There are 6 possible playlists: [1, 2, 3], [1, 3, 2], [2,
 * 1, 3], [2, 3, 1], [3, 1, 2], and [3, 2, 1].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 2, goal = 3, k = 0
 *     **Output:** 6
 * **Explanation:** There are 6 possible playlists: [1, 1, 2], [1, 2, 1], [2,
 * 1, 1], [2, 2, 1], [2, 1, 2], and [1, 2, 2].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 2, goal = 3, k = 1
 *     **Output:** 2
 * **Explanation:** There are 2 possible playlists: [1, 2, 1] and [2, 1, 2].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= k < n <= goal <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-music-playlists/
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
    int numMusicPlaylists(int n, int goal, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 3
    // Test case 3: 1
    // Test case 4: 2
    // Test case 5: 3
    // Test case 6: 0
    // Test case 7: 2
    // Test case 8: 3
    // Test case 9: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
