// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 830: Positions of Large Groups
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * In a string `s` of lowercase letters, these letters form consecutive groups
 * of
 * the same character.
 * 
 * For example, a string like `s = "abbxxxxzyy"` has the groups `"a"`, `"bb"`,
 * `"xxxx"`, `"z"`, and `"yy"`.
 * 
 * A group is identified by an interval `[start, end]`, where `start` and `end`
 * denote the start and end indices (inclusive) of the group. In the above
 * example,
 * `"xxxx"` has the interval `[3,6]`.
 * 
 * A group is considered **large**  if it has 3 or more characters.
 * 
 * Return _the intervals of every**large** group sorted in **increasing order
 * by
 * start index**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abbxxxxzzy"
 *     **Output:** [[3,6]]
 * **Explanation:** "xxxx" is the only large group with start index 3 and end
 * index 6.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abc"
 *     **Output:** []
 * **Explanation:** We have groups "a", "b", and "c", none of which are large
 * groups.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "abcdddeeeeaabbbcd"
 *     **Output:** [[3,5],[6,9],[12,14]]
 *     **Explanation:** The large groups are "ddd", "eeee", and "bbb".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `s` contains lowercase English letters only.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/positions-of-large-groups/
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
    vector<vector<int>> largeGroupPositions(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abbxxxxzzy"
    // Test case 2: "abc"
    // Test case 3: "abcdddeeeeaabbbcd"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
