/*
 * LeetCode 937 - Reorder Data in Log Files
 * Approach: Partition into letter-logs and digit-logs, then custom sort
 *
 * Rules:
 *   1. Letter-logs before all digit-logs
 *   2. Letter-logs sorted by content first, then by identifier on ties
 *   3. Digit-logs keep their original relative order
 *
 * Classification: check the first character after the first space.
 *   If it is a digit -> digit-log.  Otherwise -> letter-log.
 *
 * Time:  O(n * L * log n)  -- n logs, L max length, sorting dominates
 * Space: O(n * L)          -- for the two partitioned vectors + result
 */

class Solution {
public:
    vector<string> reorderLogFiles(vector<string>& logs) {
        vector<string> letterLogs;
        vector<string> digitLogs;

        // ---- Step 1: Classify each log ----
        for (const string& log : logs) {
            // Find the first space to separate identifier from content
            int spacePos = log.find(' ');
            // The character right after the first space determines the type
            // (problem guarantees content is either all-letters or all-digits)
            if (isdigit(log[spacePos + 1])) {
                digitLogs.push_back(log);
            } else {
                letterLogs.push_back(log);
            }
        }

        // ---- Step 2: Sort letter-logs with custom comparator ----
        // Priority: (1) content lexicographic order
        //           (2) identifier as tiebreaker if content is identical
        sort(letterLogs.begin(), letterLogs.end(),
            [](const string& a, const string& b) {
                // Extract identifier and content for log a
                int spaceA = a.find(' ');
                string idA = a.substr(0, spaceA);
                string contentA = a.substr(spaceA + 1);

                // Extract identifier and content for log b
                int spaceB = b.find(' ');
                string idB = b.substr(0, spaceB);
                string contentB = b.substr(spaceB + 1);

                // Compare by content first
                if (contentA != contentB) {
                    return contentA < contentB;
                }
                // If content is identical, compare by identifier
                return idA < idB;
            }
        );

        // ---- Step 3: Build result = sorted letter-logs + digit-logs ----
        vector<string> result;
        result.reserve(logs.size());

        for (const string& log : letterLogs) {
            result.push_back(log);
        }
        for (const string& log : digitLogs) {
            result.push_back(log);
        }

        return result;
    }
};

/*
 * Walkthrough with:
 *   logs = ["dig1 8 1 5 1", "let1 art can", "dig2 3 6",
 *           "let2 own kit dig", "let3 art zero"]
 *
 * Step 1 - Classification:
 *   "dig1 8 1 5 1"   -> first char after space = '8' -> digit-log
 *   "let1 art can"    -> first char after space = 'a' -> letter-log
 *   "dig2 3 6"        -> first char after space = '3' -> digit-log
 *   "let2 own kit dig"-> first char after space = 'o' -> letter-log
 *   "let3 art zero"   -> first char after space = 'a' -> letter-log
 *
 *   letterLogs = ["let1 art can", "let2 own kit dig", "let3 art zero"]
 *   digitLogs  = ["dig1 8 1 5 1", "dig2 3 6"]
 *
 * Step 2 - Sort letter-logs:
 *   Comparing contents:
 *     "art can" vs "own kit dig" -> "art can" < "own kit dig"
 *     "art can" vs "art zero"    -> "art can" < "art zero"
 *     "art zero" vs "own kit dig"-> "art zero" < "own kit dig"
 *
 *   Sorted: ["let1 art can", "let3 art zero", "let2 own kit dig"]
 *
 * Step 3 - Concatenate:
 *   result = ["let1 art can", "let3 art zero", "let2 own kit dig",
 *             "dig1 8 1 5 1", "dig2 3 6"]
 *
 * This matches the expected output!
 *
 * ---------------------------------------------------------------
 * Alternative: Single stable_sort approach
 *
 * Instead of partitioning, use stable_sort on the entire array with
 * a comparator that:
 *   - If both are letter-logs: compare by content, then identifier
 *   - If one is letter and one is digit: letter comes first
 *   - If both are digit-logs: return false (equal, stable sort
 *     preserves original order)
 *
 * This is slightly more elegant but the comparator is more complex:
 *
 *   stable_sort(logs.begin(), logs.end(),
 *       [](const string& a, const string& b) {
 *           int spA = a.find(' '), spB = b.find(' ');
 *           bool aIsDigit = isdigit(a[spA + 1]);
 *           bool bIsDigit = isdigit(b[spB + 1]);
 *           if (aIsDigit && bIsDigit) return false;   // keep order
 *           if (aIsDigit) return false;                // b (letter) first
 *           if (bIsDigit) return true;                 // a (letter) first
 *           // Both are letter-logs: compare content, then id
 *           string cA = a.substr(spA+1), cB = b.substr(spB+1);
 *           if (cA != cB) return cA < cB;
 *           return a.substr(0, spA) < b.substr(0, spB);
 *       });
 * ---------------------------------------------------------------
 */
