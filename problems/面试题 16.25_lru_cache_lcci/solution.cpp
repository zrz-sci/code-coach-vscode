// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 面试题 16.25: LRU Cache LCCI
 * Difficulty: Medium
 * Tags: Design, Hash Table, Linked List, Doubly-Linked List
 *
 * Problem Description:
 * Design and build a "least recently used" cache, which evicts the least
 * recently
 * used item. The cache should map from keys to values (allowing you to insert
 * and
 * retrieve a value associ­ated with a particular key) and be initialized with
 * a
 * max size. When it is full, it should evict the least recently used item.
 * 
 * You should implement following operations:  `get` and `put`.
 * 
 * Get a value by key: `get(key)` \- If key is in the cache, return the value,
 * otherwise return -1.  
 * Write a key-value pair to the cache: `put(key, value)` \- If the key is not
 * in
 * the cache, then write its value to the cache. Evict the least recently used
 * item
 * before writing if necessary.
 * 
 * **Example:**
 * 
 *     
 *     
 *     LRUCache cache = new LRUCache( 2 /* capacity */ );
 *     
 *     cache.put(1, 1);
 *     cache.put(2, 2);
 *     cache.get(1);       // returns 1
 *     cache.put(3, 3);    // evicts key 2
 *     cache.get(2);       // returns -1 (not found)
 *     cache.put(4, 4);    // evicts key 1
 *     cache.get(1);       // returns -1 (not found)
 *     cache.get(3);       // returns 3
 *     cache.get(4);       // returns 4
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/lru-cache-lcci/
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

class LRUCache {
public:
    LRUCache(int capacity) {
        
    }
    
    int get(int key) {
        
    }
    
    void put(int key, int value) {
        
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: LRUCache
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["LRUCache","put","put","get","put","get","put","get","get","get"]
    // [[2],[1,1],[2,2],[1],[3,3],[2],[4,4],[1],[3],[4]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // LRUCache obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
