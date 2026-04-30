// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 706: Design HashMap
 * Difficulty: Easy
 * Tags: Design, Array, Hash Table, Linked List, Hash Function
 *
 * Problem Description:
 * Design a HashMap without using any built-in hash table libraries.
 * 
 * Implement the `MyHashMap` class:
 * 
 *   * `MyHashMap()` initializes the object with an empty map.
 * * `void put(int key, int value)` inserts a `(key, value)` pair into the
 * HashMap. If the `key` already exists in the map, update the corresponding
 * `value`.
 * * `int get(int key)` returns the `value` to which the specified `key` is
 * mapped, or `-1` if this map contains no mapping for the `key`.
 * * `void remove(key)` removes the `key` and its corresponding `value` if the
 * map contains the mapping for the `key`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["MyHashMap", "put", "put", "get", "get", "put", "get", "remove", "get"]
 *     [[], [1, 1], [2, 2], [1], [3], [2, 1], [2], [2], [2]]
 *     **Output**
 *     [null, null, null, 1, -1, null, 1, null, -1]
 *     
 *     **Explanation**
 *     MyHashMap myHashMap = new MyHashMap();
 *     myHashMap.put(1, 1); // The map is now [[1,1]]
 *     myHashMap.put(2, 2); // The map is now [[1,1], [2,2]]
 *     myHashMap.get(1);    // return 1, The map is now [[1,1], [2,2]]
 * myHashMap.get(3); // return -1 (i.e., not found), The map is now [[1,1],
 * [2,2]]
 * myHashMap.put(2, 1); // The map is now [[1,1], [2,1]] (i.e., update the
 * existing value)
 *     myHashMap.get(2);    // return 1, The map is now [[1,1], [2,1]]
 *     myHashMap.remove(2); // remove the mapping for 2, The map is now [[1,1]]
 * myHashMap.get(2); // return -1 (i.e., not found), The map is now [[1,1]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= key, value <= 106`
 *   * At most `104` calls will be made to `put`, `get`, and `remove`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/design-hashmap/
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

class MyHashMap {
public:
    MyHashMap() {
        
    }
    
    void put(int key, int value) {
        
    }
    
    int get(int key) {
        
    }
    
    void remove(int key) {
        
    }
};

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap* obj = new MyHashMap();
 * obj->put(key,value);
 * int param_2 = obj->get(key);
 * obj->remove(key);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: MyHashMap
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["MyHashMap","put","put","get","get","put","get","remove","get"]
    // [[],[1,1],[2,2],[1],[3],[2,1],[2],[2],[2]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // MyHashMap obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
