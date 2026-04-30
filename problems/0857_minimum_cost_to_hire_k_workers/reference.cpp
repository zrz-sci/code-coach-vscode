/*
 * LeetCode 857: Minimum Cost to Hire K Workers
 *
 * Approach: Sort by wage/quality ratio + Max-Heap
 *
 * Key Insight:
 *   For any group of k workers, if we pay proportionally to quality,
 *   the "unit rate" is determined by the worker with the highest
 *   wage/quality ratio. Total cost = rate * sum_of_qualities.
 *
 *   Sort workers by ratio ascending. For each worker i (as the rate-setter),
 *   pick the k-1 workers with smallest quality from workers 0..i-1.
 *   Use a max-heap to maintain the k smallest qualities seen so far.
 *
 * Time:  O(n log n)
 * Space: O(n)
 */

// ===================== Solution 1: Sort + Max-Heap (Optimal) =====================

class Solution {
public:
    double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
        int n = quality.size();

        // Step 1: Build (ratio, quality) pairs and sort by ratio ascending
        vector<pair<double, int>> workers(n);
        for (int i = 0; i < n; i++) {
            workers[i] = {(double)wage[i] / quality[i], quality[i]};
        }
        sort(workers.begin(), workers.end());

        // Step 2: Use max-heap to track the k smallest qualities
        // C++ priority_queue is max-heap by default
        priority_queue<int> maxHeap;
        long long qualitySum = 0;
        double ans = 1e18;

        for (auto& [ratio, q] : workers) {
            // Add current worker to the candidate pool
            maxHeap.push(q);
            qualitySum += q;

            // If we have more than k workers, remove the one with highest quality
            // This minimizes qualitySum while keeping exactly k workers
            if ((int)maxHeap.size() > k) {
                qualitySum -= maxHeap.top();
                maxHeap.pop();
            }

            // When we have exactly k workers, compute cost
            // Current ratio is the highest in the group (because we sorted)
            // Total cost = ratio * qualitySum
            if ((int)maxHeap.size() == k) {
                ans = min(ans, ratio * qualitySum);
            }
        }

        return ans;
    }
};

// ===================== Solution 2: Alternative with Struct =====================
// Same approach but using a struct for clarity and explicit sorting

class SolutionV2 {
public:
    double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
        int n = quality.size();

        // Create index array and sort by ratio
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            // Compare wage[a]/quality[a] vs wage[b]/quality[b]
            // Cross-multiply to avoid floating point: wage[a]*quality[b] vs wage[b]*quality[a]
            return (long long)wage[a] * quality[b] < (long long)wage[b] * quality[a];
        });

        priority_queue<int> pq;  // max-heap of quality values
        long long sumQ = 0;
        double result = numeric_limits<double>::max();

        for (int idx : order) {
            int q = quality[idx];
            double rate = (double)wage[idx] / quality[idx];

            pq.push(q);
            sumQ += q;

            if ((int)pq.size() > k) {
                sumQ -= pq.top();
                pq.pop();
            }

            if ((int)pq.size() == k) {
                result = min(result, rate * sumQ);
            }
        }

        return result;
    }
};

// ===================== Walkthrough with Example =====================
/*
 * Example: quality = [10,20,5], wage = [70,50,30], k = 2
 *
 * Step 1: Compute ratios
 *   Worker 0: ratio = 70/10 = 7.0, quality = 10
 *   Worker 1: ratio = 50/20 = 2.5, quality = 20
 *   Worker 2: ratio = 30/5  = 6.0, quality = 5
 *
 * Step 2: Sort by ratio ascending
 *   [(2.5, 20), (6.0, 5), (7.0, 10)]
 *
 * Step 3: Iterate with max-heap
 *   i=0: ratio=2.5, q=20 -> heap=[20], sum=20 -> size=1 < k=2, skip
 *   i=1: ratio=6.0, q=5  -> heap=[20,5], sum=25 -> size=2 == k=2
 *         cost = 6.0 * 25 = 150.0, ans=150.0
 *   i=2: ratio=7.0, q=10 -> heap=[20,10,5], sum=35 -> size=3 > k=2
 *         pop 20, sum=15 -> heap=[10,5], size=2 == k=2
 *         cost = 7.0 * 15 = 105.0, ans=105.0
 *
 * Return 105.0 -- matches expected output!
 *
 * Why it works:
 *   - At ratio 7.0, we pay worker 2: 7.0 * 5 = 35 >= wage[2]=30 (OK)
 *   - We pay worker 0: 7.0 * 10 = 70 >= wage[0]=70 (OK)
 *   - Total = 105
 */

// ===================== Complexity Analysis =====================
/*
 * Time Complexity: O(n log n)
 *   - Sorting: O(n log n)
 *   - Heap operations: each of n workers is pushed/popped at most once -> O(n log k)
 *   - Overall: O(n log n) dominates
 *
 * Space Complexity: O(n)
 *   - Workers array: O(n)
 *   - Heap: O(k)
 *   - Overall: O(n)
 *
 * Edge Cases:
 *   - k == n: must hire all workers, rate = max ratio, cost = max_ratio * total_quality
 *   - k == 1: simply find the worker with minimum wage
 *   - All workers have same ratio: any k workers give same cost
 *   - All workers have same quality: sort by wage, pick k smallest wages
 */
