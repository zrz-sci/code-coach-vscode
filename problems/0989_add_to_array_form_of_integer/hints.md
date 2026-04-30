[file://solution.cpp](solution.cpp)

# 989. Add to Array-Form of Integer

## 1. Problem Summary
You are given the array-form of an integer `num` (digits stored left to right in a vector) and an integer `k`. Return the array-form of `num + k`. For example, if `num = [1,2,0,0]` and `k = 34`, the result is `[1,2,3,4]` because 1200 + 34 = 1234. The key challenge is handling carries properly and dealing with cases where `k` has more digits than `num` (e.g., `num = [2,1,5]`, `k = 806` gives `[1,0,2,1]` because 215 + 806 = 1021).

## 2. Key Insights / "Aha!" Moments
- This is fundamentally a **digit-by-digit addition** problem, just like adding two numbers on paper from right to left, tracking the carry.
- The clever insight is to treat `k` itself as the "carry". Instead of extracting digits from `k` separately, you can add `k` directly to the rightmost digit of `num` and propagate. Each iteration, you add `k % 10` as the current digit contribution, then set `k = k / 10` to shift to the next digit. This unifies carry and the digits of `k` into one variable.
- You do NOT need to convert the entire array to a single integer. The array can have up to 10,000 digits, which far exceeds what any integer type can hold. Process digit by digit.
- The result may be longer than the input array. For instance, `[9,9,9] + 1 = [1,0,0,0]`. You must handle the extra leading digits.
- Building the result in reverse (from least significant to most significant) and then reversing at the end is the most natural approach.

## 3. Approach Walkthrough (Step-by-Step)
**Approach: Digit-by-digit addition from right to left**

1. **Initialize**: Set `carry = k` (treat k as the initial carry value). Create an empty result vector.
2. **Start from the rightmost digit**: Use an index `i` starting at `num.size() - 1`.
3. **Loop while there are digits left or carry > 0**:
   - If `i >= 0`, add `num[i]` to `carry`.
   - The current result digit is `carry % 10`.
   - Push this digit to the result vector.
   - Update `carry = carry / 10`.
   - Decrement `i`.
4. **Reverse the result**: Since we built the result from least significant to most significant, reverse it to get the correct order.
5. **Return the result**.

**Why this works**: By using `k` (or `carry`) as a running accumulator, we naturally handle both the addition of `k`'s digits and any carries. When `k` still has remaining digits (carry > 0) after processing all of `num`, the loop continues to extract them.

**Alternative approach**: Process `k` and `num` simultaneously by extracting digits from `k` one at a time. This is essentially the same logic but separates the carry from `k`'s digit contribution:
- `digit_from_k = k % 10; k = k / 10;`
- `sum = num[i] + digit_from_k + carry;`
- `result_digit = sum % 10; carry = sum / 10;`

Both approaches have identical complexity. The unified carry approach is more concise.

## 4. Complexity Analysis
- **Time Complexity**: O(max(N, log(k))) where N is the length of the `num` array and log(k) is the number of digits in `k`. We process each digit of both numbers exactly once.
- **Space Complexity**: O(max(N, log(k))) for the result array. No additional auxiliary space is needed beyond the result. The reversal is done in-place.
- The constraints state N <= 10^4 and k <= 10^4, so in practice the operation is very fast. But the algorithm generalizes to arbitrarily large inputs.

## 5. Edge Cases to Watch
- **k = 0**: The result is simply `num` unchanged. The algorithm handles this naturally since carry starts at 0 and no modification occurs.
- **num = [0]**: The result is the array-form of `k`. e.g., `[0] + 123 = [1,2,3]`.
- **Large carry propagation**: `num = [9,9,9,9]`, `k = 1`. Result is `[1,0,0,0,0]`. The carry propagates through every digit and creates a new leading digit.
- **k has more digits than num**: `num = [1]`, `k = 9999`. Result is `[1,0,0,0,0]`. After processing `num`'s single digit, the remaining digits of `k` are handled by the carry loop.
- **No leading zeros**: The problem guarantees `num` has no leading zeros (except for `num = [0]`), and the result should not have leading zeros either. The algorithm naturally avoids this.
- **Both num and k are maximum**: `num` has 10^4 nines and `k = 10^4`. The result has at most 10^4 + 1 digits.

## 6. Similar / Related Problems
- **LeetCode 2 - Add Two Numbers**: Adding two numbers represented as linked lists. Same digit-by-digit addition with carry, but on linked lists instead of arrays.
- **LeetCode 66 - Plus One**: A special case of this problem where `k = 1`.
- **LeetCode 67 - Add Binary**: Same concept but in base 2 instead of base 10.
- **LeetCode 415 - Add Strings**: Adding two numbers represented as strings. Very similar logic.
- **LeetCode 43 - Multiply Strings**: A more complex arithmetic operation on string-represented numbers.
- **LeetCode 371 - Sum of Two Integers**: Addition using bit manipulation instead of digit manipulation.

## 7. Mnemonics / Memory Hooks
- **"k IS the carry"**: The simplest mental model. Instead of maintaining a separate carry variable and extracting digits from k, merge them into one. Start with carry = k, and the standard carry propagation handles everything.
- **"Paper addition, right to left"**: Just like elementary school addition. Write the numbers aligned right, add column by column from right to left, carry the overflow.
- **"Build backwards, flip at the end"**: Since you add from the least significant digit, push results to a vector and reverse at the end. This is a very common pattern in array-building problems.

## 8. Code Template / Pseudocode
```
function addToArrayForm(num, k):
    result = []
    carry = k
    i = num.size() - 1
    
    while i >= 0 OR carry > 0:
        if i >= 0:
            carry += num[i]
            i -= 1
        
        result.push_back(carry % 10)
        carry = carry / 10
    
    reverse(result)
    return result
```

Alternative with explicit carry separation:
```
function addToArrayForm(num, k):
    result = []
    carry = 0
    i = num.size() - 1
    
    while i >= 0 OR k > 0 OR carry > 0:
        sum = carry
        if i >= 0:
            sum += num[i]
            i -= 1
        if k > 0:
            sum += k % 10
            k = k / 10
        
        result.push_back(sum % 10)
        carry = sum / 10
    
    reverse(result)
    return result
```
