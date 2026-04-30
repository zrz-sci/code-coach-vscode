[file://solution.cpp](solution.cpp)

# LeetCode 937 - Reorder Data in Log Files

## 1. Problem Understanding (Restate & Clarify)

We are given an array of log strings. Each log has an identifier (the first word) followed by one or more content words separated by spaces. Logs fall into two categories:
- **Letter-logs:** content words consist entirely of lowercase English letters.
- **Digit-logs:** content words consist entirely of digits.

We must reorder the logs according to three rules:
1. All letter-logs come before all digit-logs.
2. Letter-logs are sorted lexicographically by their content. If two letter-logs have identical content, sort them by their identifier.
3. Digit-logs maintain their original relative order (stable ordering).

The output is the reordered array of log strings.

Key clarifications:
- The identifier is always the first space-delimited token.
- A log is a digit-log if the first character after the identifier is a digit. We only need to check one content character since the problem guarantees all content words are either all letters or all digits.
- There can be up to 100 logs, each up to 100 characters.

## 2. Approach Brainstorming (At Least 2 Approaches)

**Approach A: Partition + Custom Sort**
Separate the logs into two groups: letter-logs and digit-logs. Sort the letter-logs with a custom comparator that first compares content, then identifier on tie. Concatenate the sorted letter-logs with the (unchanged-order) digit-logs.

**Approach B: Single Stable Sort with Custom Comparator**
Use a single stable sort on the entire array with a comparator that:
- Ranks any letter-log before any digit-log.
- Between two letter-logs, compares by content then identifier.
- Between two digit-logs, preserves original order (guaranteed by stable sort returning "equal" for any two digit-logs).

**Approach C: Manual Insertion**
Build the result by iterating through logs, inserting letter-logs into a sorted structure and appending digit-logs to a separate list. Merge at the end.

## 3. Optimal Approach Walkthrough (Step by Step)

We choose **Approach A: Partition + Custom Sort** for clarity.

Step 1 -- Classify each log. For each log string, find the first space to isolate the identifier. Then check the character immediately after the first space: if it is a digit, the log is a digit-log; otherwise it is a letter-log.

Step 2 -- Separate into two vectors: `letterLogs` and `digitLogs`.

Step 3 -- Sort `letterLogs` with a custom comparator:
  - For two logs `a` and `b`, extract content(a) and content(b) (everything after the first space).
  - If content(a) < content(b), then a comes first.
  - If content(a) > content(b), then b comes first.
  - If contents are equal, compare identifiers: identifier(a) < identifier(b) means a comes first.

Step 4 -- Build the result: first all sorted letter-logs, then all digit-logs in original order.

Step 5 -- Return the result.

## 4. Implementation Notes

- To extract the identifier and content, use `str.find(' ')` to locate the first space. `str.substr(0, pos)` gives the identifier, `str.substr(pos + 1)` gives the content.
- The classification check: after finding the first space at position `pos`, check `str[pos + 1]`. If it is between '0' and '9', it is a digit-log. Otherwise, letter-log.
- For the custom comparator in `std::sort`, use a lambda that captures nothing. Extract content and identifier inside the comparator.
- `std::stable_sort` can also work if you prefer Approach B, but Approach A with explicit partitioning is cleaner.
- Be careful with the comparator: it must define a strict weak ordering (no ties returning true for both directions).

## 5. Complexity Analysis

**Time Complexity:** O(n * L * log n) where n is the number of logs and L is the maximum length of a log string. Sorting the letter-logs takes O(n log n) comparisons, each comparison involves string comparison costing O(L). Classification is O(n * L) for finding spaces.

**Space Complexity:** O(n * L) for storing the two separate lists (letter-logs and digit-logs) and the result. If we use in-place stable sort (Approach B), the auxiliary space is O(n) for the sort, but we still need O(L) per comparison.

## 6. Edge Cases & Pitfalls

- **All letter-logs, no digit-logs:** The result is just the sorted letter-logs. The digit-log vector is empty.
- **All digit-logs, no letter-logs:** The result is the original array unchanged.
- **Identical content in letter-logs:** Must fall back to comparing identifiers. For example, "a1 act car" and "b2 act car" should be ordered by "a1" < "b2".
- **Single-word content:** A log like "let1 art" has identifier "let1" and content "art". Still works with the space-splitting approach.
- **Identifier with digits:** An identifier like "dig1" does NOT make it a digit-log. Only the CONTENT (after the identifier) determines the type. A log "dig1 art can" is a letter-log because "art" is letters.
- **Comparator correctness:** When comparing two letter-logs with the same content, comparing identifiers must use < (not <=) to maintain strict weak ordering.

## 7. Mnemonics / Memory Hooks

**"Letters Lead, Digits Drag"** -- Letter-logs always come first; digit-logs trail behind in their original order.

**"Content first, ID breaks ties"** -- The sorting priority for letter-logs is: (1) content lexicographic order, (2) identifier as tiebreaker.

**"First char after space decides type"** -- Quick classification: look at the character right after the first space. Digit means digit-log, letter means letter-log. No need to scan the entire content.

## 8. Related Problems & Patterns

- **LeetCode 179 - Largest Number:** Custom comparator sorting problem. Requires careful design of the comparison function for string-based ordering.
- **LeetCode 1122 - Relative Sort Array:** Two-group sorting with one group having a defined order and the other maintaining natural order.
- **LeetCode 791 - Custom Sort String:** Sorting based on a custom ordering scheme, similar concept of defining priorities.
- **LeetCode 2191 - Sort the Jumbled Numbers:** Another custom comparator problem where the "value" used for sorting differs from the raw representation.
- **Pattern: "Partition and Custom Sort"** -- When different categories of elements have different sorting rules, partition them first, sort each partition independently, and concatenate. This avoids complex unified comparators and makes the logic transparent.
- **Pattern: "Stable Sort for Relative Order Preservation"** -- When some elements must keep their original relative positions (here, digit-logs), stable sort or explicit partitioning guarantees this property.
