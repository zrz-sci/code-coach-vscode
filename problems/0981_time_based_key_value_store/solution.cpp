// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 981: Time Based Key-Value Store
 * Difficulty: Medium
 * Tags: Design, Hash Table, String, Binary Search
 *
 * Problem Description:
 * Design a time-based key-value data structure that can store multiple values
 * for
 * the same key at different time stamps and retrieve the key's value at a
 * certain
 * timestamp.
 * 
 * Implement the `TimeMap` class:
 * 
 *   * `TimeMap()` Initializes the object of the data structure.
 * * `void set(String key, String value, int timestamp)` Stores the key `key`
 * with the value `value` at the given time `timestamp`.
 * * `String get(String key, int timestamp)` Returns a value such that `set`
 * was called previously, with `timestamp_prev <= timestamp`. If there are
 * multiple such values, it returns the value associated with the largest
 * `timestamp_prev`. If there are no values, it returns `""`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["TimeMap", "set", "get", "get", "set", "get", "get"]
 * [[], ["foo", "bar", 1], ["foo", 1], ["foo", 3], ["foo", "bar2", 4], ["foo",
 * 4], ["foo", 5]]
 *     **Output**
 *     [null, null, "bar", "bar", null, "bar2", "bar2"]
 *     
 *     **Explanation**
 *     TimeMap timeMap = new TimeMap();
 * timeMap.set("foo", "bar", 1); // store the key "foo" and value "bar" along
 * with timestamp = 1.
 *     timeMap.get("foo", 1);         // return "bar"
 * timeMap.get("foo", 3); // return "bar", since there is no value
 * corresponding to foo at timestamp 3 and timestamp 2, then the only value is
 * at timestamp 1 is "bar".
 * timeMap.set("foo", "bar2", 4); // store the key "foo" and value "bar2"
 * along with timestamp = 4.
 *     timeMap.get("foo", 4);         // return "bar2"
 *     timeMap.get("foo", 5);         // return "bar2"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= key.length, value.length <= 100`
 *   * `key` and `value` consist of lowercase English letters and digits.
 *   * `1 <= timestamp <= 107`
 *   * All the timestamps `timestamp` of `set` are strictly increasing.
 *   * At most `2 * 105` calls will be made to `set` and `get`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/time-based-key-value-store/
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

class TimeMap {
public:
    TimeMap() {
        
    }
    
    void set(string key, string value, int timestamp) {
        
    }
    
    string get(string key, int timestamp) {
        
    }
};

/**
 * Your TimeMap object will be instantiated and called as such:
 * TimeMap* obj = new TimeMap();
 * obj->set(key,value,timestamp);
 * string param_2 = obj->get(key,timestamp);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: TimeMap
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["TimeMap","set","get","get","set","get","get"]
    // [[],["foo","bar",1],["foo",1],["foo",3],["foo","bar2",4],["foo",4],["foo",5]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // TimeMap obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
