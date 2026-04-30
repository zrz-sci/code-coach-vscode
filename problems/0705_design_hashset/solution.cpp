// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 705: Design HashSet
 * Difficulty: Easy
 * Tags: Design, Array, Hash Table, Linked List, Hash Function
 *
 * Problem Description:
 * Design a HashSet without using any built-in hash table libraries.
 * 
 * Implement `MyHashSet` class:
 * 
 *   * `void add(key)` Inserts the value `key` into the HashSet.
 * * `bool contains(key)` Returns whether the value `key` exists in the
 * HashSet or not.
 * * `void remove(key)` Removes the value `key` in the HashSet. If `key` does
 * not exist in the HashSet, do nothing.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 * ["MyHashSet", "add", "add", "contains", "contains", "add", "contains",
 * "remove", "contains"]
 *     [[], [1], [2], [1], [3], [2], [2], [2], [2]]
 *     **Output**
 *     [null, null, null, true, false, null, true, null, false]
 *     
 *     **Explanation**
 *     MyHashSet myHashSet = new MyHashSet();
 *     myHashSet.add(1);      // set = [1]
 *     myHashSet.add(2);      // set = [1, 2]
 *     myHashSet.contains(1); // return True
 *     myHashSet.contains(3); // return False, (not found)
 *     myHashSet.add(2);      // set = [1, 2]
 *     myHashSet.contains(2); // return True
 *     myHashSet.remove(2);   // set = [1]
 *     myHashSet.contains(2); // return False, (already removed)
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= key <= 106`
 *   * At most `104` calls will be made to `add`, `remove`, and `contains`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/design-hashset/
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

class MyHashSet {
public:
    MyHashSet() {
        
    }
    
    void add(int key) {
        
    }
    
    void remove(int key) {
        
    }
    
    bool contains(int key) {
        
    }
};

/**
 * Your MyHashSet object will be instantiated and called as such:
 * MyHashSet* obj = new MyHashSet();
 * obj->add(key);
 * obj->remove(key);
 * bool param_3 = obj->contains(key);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: MyHashSet
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["MyHashSet","add","add","contains","contains","add","contains","remove","contains"]
    // [[],[1],[2],[1],[3],[2],[2],[2],[2]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // MyHashSet obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
