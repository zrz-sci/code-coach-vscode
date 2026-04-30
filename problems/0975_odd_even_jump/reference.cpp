[file://solution.cpp](solution.cpp)

// =============================================================================
// LeetCode 975: Odd Even Jump
// Approach 1: Ordered Map (TreeMap) + Reverse DP
//
// Process indices from right to left. Maintain a std::map<int,int> from value
// to index. For each index i:
//   - Odd jump target: lower_bound(arr[i]) gives smallest value >= arr[i]
//   - Even jump target: upper_bound(arr[i]) then --it gives largest value <= arr[i]
// Two DP arrays: odd[i] and even[i] indicate reachability from i.
//
// Time:  O(n log n)
// Space: O(n)
// =============================================================================

class Solution {
public:
    int oddEvenJumps(vector<int>& arr) {
        int n = arr.size();
        if (n == 0) return 0;

        // odd[i] = can reach end starting at i with an odd jump next
        // even[i] = can reach end starting at i with an even jump next
        vector<bool> odd(n, false), even(n, false);
        odd[n - 1] = even[n - 1] = true;

        // Ordered map: value -> index (stores the smallest index for each value
        // as we process right-to-left and overwrite)
        map<int, int> valToIdx;
        valToIdx[arr[n - 1]] = n - 1;

        int count = 1; // Last index is always good

        for (int i = n - 2; i >= 0; i--) {
            // Odd jump: find smallest value >= arr[i]
            auto oddIt = valToIdx.lower_bound(arr[i]);
            if (oddIt != valToIdx.end()) {
                odd[i] = even[oddIt->second];
            }

            // Even jump: find largest value <= arr[i]
            auto evenIt = valToIdx.upper_bound(arr[i]);
            if (evenIt != valToIdx.begin()) {
                --evenIt;
                even[i] = odd[evenIt->second];
            }

            // Insert current index (overwrites if same value exists,
            // giving the smallest index which is what we want)
            valToIdx[arr[i]] = i;

            // A starting index is good if odd[i] is true (first jump is odd)
            if (odd[i]) count++;
        }

        return count;
    }
};

// =============================================================================
// LeetCode 975: Odd Even Jump
// Approach 2: Monotonic Stack to Precompute Jump Targets + DP
//
// Step 1: Sort indices by value to determine odd-jump targets.
//         For odd jumps, sort by (value ascending, index ascending).
//         Then use a monotonic stack to find, for each index, the next
//         index in the sorted order that is to the right in the array.
//
// Step 2: Sort indices by value descending for even-jump targets.
//         For even jumps, sort by (value descending, index ascending).
//         Again use a monotonic stack.
//
// Step 3: DP from right to left using precomputed targets.
//
// Time:  O(n log n) -- dominated by sorting
// Space: O(n)
// =============================================================================

class SolutionMonotonicStack {
public:
    // Given a sorted list of original indices, compute "next right index"
    // for each element using a monotonic stack.
    // nextJump[indices[k]] = the next index in `indices` that appears
    // to the right in the original array.
    vector<int> computeJumpTargets(vector<int>& sortedIndices, int n) {
        vector<int> target(n, -1);
        stack<int> stk; // stores indices in decreasing order

        for (int idx : sortedIndices) {
            // Pop all indices from stack that are smaller than current idx
            // (they jump to idx since idx is the first index to their right
            // in the sorted order)
            while (!stk.empty() && stk.top() < idx) {
                target[stk.top()] = idx;
                stk.pop();
            }
            stk.push(idx);
        }

        return target;
    }

    int oddEvenJumps(vector<int>& arr) {
        int n = arr.size();
        if (n == 0) return 0;

        // Create index arrays sorted for odd and even jumps
        vector<int> oddOrder(n), evenOrder(n);
        for (int i = 0; i < n; i++) oddOrder[i] = evenOrder[i] = i;

        // Odd jump: sort by value ascending, then index ascending
        sort(oddOrder.begin(), oddOrder.end(), [&](int a, int b) {
            return arr[a] != arr[b] ? arr[a] < arr[b] : a < b;
        });

        // Even jump: sort by value descending, then index ascending
        sort(evenOrder.begin(), evenOrder.end(), [&](int a, int b) {
            return arr[a] != arr[b] ? arr[a] > arr[b] : a < b;
        });

        // Compute jump targets using monotonic stack
        vector<int> oddTarget = computeJumpTargets(oddOrder, n);
        vector<int> evenTarget = computeJumpTargets(evenOrder, n);

        // DP
        vector<bool> odd(n, false), even(n, false);
        odd[n - 1] = even[n - 1] = true;

        int count = 1;
        for (int i = n - 2; i >= 0; i--) {
            if (oddTarget[i] != -1) {
                odd[i] = even[oddTarget[i]];
            }
            if (evenTarget[i] != -1) {
                even[i] = odd[evenTarget[i]];
            }
            if (odd[i]) count++;
        }

        return count;
    }
};
