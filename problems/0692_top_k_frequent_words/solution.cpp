// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 692: Top K Frequent Words
 * Difficulty: Medium
 * Tags: Trie, Array, Hash Table, String, Bucket Sort, Counting, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an array of strings `words` and an integer `k`, return _the_`k` _most
 * frequent strings_.
 * 
 * Return the answer **sorted** by **the frequency** from highest to lowest.
 * Sort
 * the words with the same frequency by their **lexicographical order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["i","love","leetcode","i","love","coding"], k = 2
 *     **Output:** ["i","love"]
 *     **Explanation:** "i" and "love" are the two most frequent words.
 *     Note that "i" comes before "love" due to a lower alphabetical order.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** words =
 * ["the","day","is","sunny","the","the","the","sunny","is","is"], k = 4
 *     **Output:** ["the","is","sunny","day"]
 * **Explanation:** "the", "is", "sunny" and "day" are the four most frequent
 * words, with the number of occurrence being 4, 3, 2 and 1 respectively.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 500`
 *   * `1 <= words[i].length <= 10`
 *   * `words[i]` consists of lowercase English letters.
 *   * `k` is in the range `[1, The number of **unique** words[i]]`
 * 
 * 
 * 
 * **Follow-up:** Could you solve it in `O(n log(k))` time and `O(n)` extra
 * space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/top-k-frequent-words/
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
    vector<string> topKFrequent(vector<string>& words, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["i","love","leetcode","i","love","coding"]
    // Test case 2: 2
    // Test case 3: ["the","day","is","sunny","the","the","the","sunny","is","is"]
    // Test case 4: 4

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
