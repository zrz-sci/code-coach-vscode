[file://solution.cpp](solution.cpp)

# 990. Satisfiability of Equality Equations

## 1. Problem Summary
Given an array of equations where each equation is a 4-character string of the form `"xi==yi"` or `"xi!=yi"` (where xi and yi are lowercase letters representing variables), determine if it is possible to assign integer values to the variables such that all equations are satisfied simultaneously. Return `true` if a valid assignment exists, `false` otherwise. For example, `["a==b", "b!=a"]` is unsatisfiable because a==b contradicts b!=a.

## 2. Key Insights / "Aha!" Moments
- **Union-Find is the perfect data structure**: Equality (`==`) is a transitive, symmetric, reflexive relation -- it defines equivalence classes. Union-Find (Disjoint Set Union) efficiently groups variables into equivalence classes based on equality constraints.
- **Two-pass strategy**: Process all `==` equations first to build equivalence classes, then check all `!=` equations for contradictions. If any `!=` equation connects two variables that are in the same equivalence class, the system is unsatisfiable.
- **Only 26 possible variables**: Since variables are single lowercase letters, the Union-Find structure needs at most 26 elements. This makes the problem very efficient regardless of the number of equations.
- **Self-inequality is always false**: If any equation says `"a!=a"`, that is immediately unsatisfiable. Conversely, `"a==a"` is always true and can be ignored. This is a quick check before the main algorithm.
- **Graph-based alternative**: You can also model this as a graph problem. Build a graph where equality edges connect nodes. Use BFS/DFS to find connected components. Then verify that no inequality edge connects two nodes in the same component.

## 3. Approach Walkthrough (Step-by-Step)
**Approach: Union-Find (Disjoint Set Union)**

1. **Initialize Union-Find**: Create a parent array of size 26 (one for each letter a-z). Initially, each element is its own parent: `parent[i] = i`.
2. **First pass - process equality equations**: Iterate through all equations. For each equation of the form `"x==y"`, union the sets containing `x` and `y`. This merges them into the same equivalence class.
3. **Second pass - check inequality equations**: Iterate through all equations again. For each equation of the form `"x!=y"`, find the roots of `x` and `y`. If they have the same root, they are in the same equivalence class, meaning we previously established `x == y` (directly or transitively). This contradicts `x != y`, so return `false`.
4. **If no contradiction found**: Return `true`. A valid assignment exists (e.g., assign the same integer to all variables in the same equivalence class, different integers to different classes).

**Union-Find operations**:
- **find(x)**: Returns the root of x's set. Use path compression for efficiency.
- **union(x, y)**: Merges the sets containing x and y. Use union by rank for balanced trees.

## 4. Complexity Analysis
- **Time Complexity**: O(N * alpha(26)) where N is the number of equations and alpha is the inverse Ackermann function. Since we have at most 26 elements, alpha(26) is effectively a constant (at most 4). So the time complexity is essentially O(N).
- **Space Complexity**: O(1) since the Union-Find structure has a fixed size of 26 elements. The parent and rank arrays use constant space.
- In practice, this is extremely fast. Even with 500 equations (the maximum), the algorithm completes in microseconds.

## 5. Edge Cases to Watch
- **Self-referencing equality**: `"a==a"` -- always true, union(a, a) is a no-op. No issue.
- **Self-referencing inequality**: `"a!=a"` -- always false. The algorithm correctly detects this since find(a) == find(a), returning false.
- **Transitive contradictions**: `["a==b", "b==c", "a!=c"]` -- a, b, c are all in the same set after processing equalities. The inequality `a!=c` then contradicts this. The algorithm correctly detects this.
- **Long transitive chains**: `["a==b", "b==c", "c==d", ..., "y==z", "a!=z"]` -- all 26 variables in one set, then `a!=z` is detected as a contradiction.
- **All equalities, no inequalities**: Always satisfiable. Return `true`.
- **All inequalities, no equalities**: Satisfiable as long as no inequality is self-referencing (`"x!=x"`). Different variables can be assigned different values.
- **Single equation**: Either `"x==y"` (always satisfiable) or `"x!=y"` (satisfiable if x != y as characters; if `"a!=a"`, unsatisfiable).
- **Duplicate or redundant equations**: Handled naturally. Redundant unions are no-ops, and redundant inequality checks don't cause false positives.

## 6. Similar / Related Problems
- **LeetCode 684 - Redundant Connection**: Uses Union-Find to detect cycles in a graph, which is detecting when a union would merge two already-connected nodes.
- **LeetCode 547 - Number of Provinces (Friend Circles)**: Classic Union-Find problem for grouping connected components.
- **LeetCode 200 - Number of Islands**: Connected components problem often solved with Union-Find or BFS/DFS.
- **LeetCode 399 - Evaluate Division**: Extends the equality/inequality concept to weighted edges (ratios between variables).
- **LeetCode 721 - Accounts Merge**: Union-Find for merging overlapping sets of emails.
- **LeetCode 1319 - Number of Operations to Make Network Connected**: Another Union-Find application.

## 7. Mnemonics / Memory Hooks
- **"Equals first, then complain"**: Always process `==` before `!=`. Build the groups first, then look for contradictions. The order matters.
- **"26 buckets"**: Only 26 possible variables (a-z). The Union-Find is tiny and constant-size regardless of input.
- **"Same group = same value"**: If Union-Find says two variables share a root, they must be equal. Any `!=` between them is a contradiction.
- **"Find the root, not the neighbor"**: Always use `find()` with path compression, never compare parent arrays directly. Two nodes can be in the same set but have different immediate parents.

## 8. Code Template / Pseudocode
```
function equationsPossible(equations):
    // Initialize Union-Find for 26 letters
    parent = [0, 1, 2, ..., 25]
    
    function find(x):
        if parent[x] != x:
            parent[x] = find(parent[x])  // path compression
        return parent[x]
    
    function union(x, y):
        rootX = find(x)
        rootY = find(y)
        if rootX != rootY:
            parent[rootX] = rootY
    
    // First pass: process all equality equations
    for eq in equations:
        if eq[1] == '=':  // "x==y"
            union(eq[0] - 'a', eq[3] - 'a')
    
    // Second pass: check all inequality equations
    for eq in equations:
        if eq[1] == '!':  // "x!=y"
            if find(eq[0] - 'a') == find(eq[3] - 'a'):
                return false  // contradiction!
    
    return true  // no contradictions found
```

Graph-based alternative:
```
function equationsPossible(equations):
    // Build adjacency list from equality equations
    graph = adjacency list for 26 nodes
    for eq in equations:
        if eq[1] == '=':
            add edge between eq[0] and eq[3]
    
    // Find connected components using BFS/DFS
    component = array of size 26, initialized to -1
    componentId = 0
    for each node 0..25:
        if component[node] == -1:
            BFS/DFS from node, marking all reachable nodes with componentId
            componentId++
    
    // Check inequality equations
    for eq in equations:
        if eq[1] == '!':
            if component[eq[0] - 'a'] == component[eq[3] - 'a']:
                return false
    
    return true
```
