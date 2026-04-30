[file://solution.cpp](solution.cpp)

# 972. Equal Rational Numbers

## 1. Problem Summary

Given two strings `s` and `t`, each representing a non-negative rational number (possibly with a repeating decimal enclosed in parentheses), determine whether the two strings represent the same number. A rational number can appear in three forms: just an integer part, an integer plus a non-repeating decimal, or an integer plus a non-repeating decimal plus a repeating block in parentheses. The key difficulty lies in correctly parsing these three forms and comparing the underlying real values, especially when representations differ superficially but encode the same quantity (e.g., `0.9(9)` equals `1.`).

## 2. Key Insights / "Aha!" Moments

- **Expand the repeating part to sufficient precision.** Because the integer part is at most 4 digits, the non-repeating part is at most 4 digits, and the repeating block is at most 4 digits, you can safely expand the repeating block many times (say 20+ copies) and compare the resulting long decimal strings as floating-point values. With enough digits of expansion, any truly different rationals will differ within the expanded range.

- **Alternatively, convert to an exact fraction.** Given the parts `IntegerPart`, `NonRepeatingPart` of length `p`, and `RepeatingPart` of length `q`, the exact rational value is:
  ```
  IntegerPart + NonRepeatingPart / 10^p + RepeatingPart / (10^p * (10^q - 1))
  ```
  Reduce both fractions to lowest terms and compare numerator/denominator pairs.

- **The `0.9(9) == 1.` trap.** Any approach that compares strings character by character will fail unless you account for the mathematical identity `0.999... = 1`. The expansion or fraction method handles this automatically.

- **Parsing is half the battle.** Carefully split the string at the `.` and `(` characters. If there is no `.`, the decimal part is empty. If there is no `(`, the repeating part is empty.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|----------|------|-------|-------|
| Expand repeating digits + double comparison | O(1) | O(1) | Expand repeating block ~20 times, convert to `double`, compare with epsilon. Fast and simple but relies on floating-point precision. Works due to small input bounds. |
| Exact fraction conversion | O(1) | O(1) | Parse each string into a numerator/denominator pair using the algebraic formula. Reduce by GCD. Compare two pairs. No precision issues. |
| Long-string expansion + string comparison | O(1) | O(1) | Expand both numbers to 20+ decimal digits, then compare digit by digit after normalizing. Must handle carry propagation for the `0.999... = 1` case, which adds complexity. |

## 4. Step-by-Step Solution (Expand + Double)

1. **Parse the string** into three components: `intPart`, `nonRepeat`, and `repeat`.
   - Find the position of `.` (if any). Everything before it is `intPart`.
   - Find the position of `(` (if any). Between `.` and `(` is `nonRepeat`. Between `(` and `)` is `repeat`.
   - If no `.`, then `nonRepeat` and `repeat` are both empty.
   - If no `(`, then `repeat` is empty and everything after `.` is `nonRepeat`.

2. **Build an expanded decimal string.** Concatenate `intPart + "." + nonRepeat + repeat * 20`. Twenty copies of up to 4 repeating digits gives 80 extra digits, far more than enough precision.

3. **Convert the expanded string to a `double`** using `stod()` or equivalent.

4. **Compare the two doubles.** Because the input constraints are extremely small (integer part <= 4 digits), the values fit comfortably in `double` precision. Two truly equal rationals will produce identical doubles after sufficient expansion.

5. **Return** `true` if both doubles are equal, `false` otherwise.

## 5. Complexity Analysis

**Expand + Double approach:**
- **Time Complexity:** O(1) -- all operations are bounded by the fixed maximum lengths (4+4+4 digits) regardless of input. Parsing is linear in string length, and string length is bounded by a constant.
- **Space Complexity:** O(1) -- the expanded string is at most ~90 characters long, a constant.

**Exact Fraction approach:**
- **Time Complexity:** O(1) -- arithmetic on bounded integers, GCD computation on bounded values.
- **Space Complexity:** O(1) -- just a few integer variables for numerator and denominator.

Both approaches are constant time/space because the input size is capped at ~13 characters per string.

## 6. Common Pitfalls

- **Forgetting to handle missing parts.** A string like `"12"` has no decimal point. A string like `"1."` has a decimal point but no non-repeating or repeating part. A string like `"0.5"` has no repeating part. Each combination must be parsed correctly.

- **Not expanding enough digits.** If you only expand the repeating block a few times, the double conversion may not distinguish values that differ only deep in the decimal expansion. Twenty or more repetitions is safe.

- **String-based comparison without carry handling.** If you try to compare digit strings directly, you must handle the case where `0.999...` rounds up to `1.000...`. This requires carry propagation logic, which is error-prone.

- **Integer overflow in the fraction approach.** If you compute `10^p * (10^q - 1)` with large exponents, the denominator can overflow standard `int`. Use `long long` or keep exponents small (they are at most 4, so `10^4 = 10000` -- no overflow with `long long`).

- **Off-by-one in parenthesis extraction.** Make sure you extract the content between `(` and `)` without including the parentheses themselves.

## 7. Pattern Recognition

- **String Parsing Pattern:** Split a structured string into components using delimiter characters. This pattern appears in many problems involving formatted input (IP addresses, file paths, version numbers, etc.).

- **Repeating Decimal / Fraction Conversion:** This is a classic math trick. The repeating decimal `0.(abc)` equals `abc / 999`, and `0.d(abc)` equals `(dabc - d) / 9990`. This formula generalizes to any combination of non-repeating and repeating parts.

- **Floating-Point Comparison with Bounded Input:** When the input domain is small enough that all distinct values differ by more than machine epsilon, direct double comparison is valid. This technique is useful in contest settings.

## 8. Related Problems

- **LeetCode 166 - Fraction to Recurring Decimal:** The reverse direction -- given a fraction, produce the decimal string with repeating part notation. Requires long division and cycle detection.

- **LeetCode 592 - Fraction Addition and Subtraction:** Parse and evaluate a string expression of fractions. Similar parsing challenges and fraction arithmetic.

- **LeetCode 168 - Excel Sheet Column Title / 171 - Excel Sheet Column Number:** Another pair of "conversion" problems where you encode/decode between two representations of the same value.

- **LeetCode 8 - String to Integer (atoi):** A simpler string parsing problem that shares the pattern of carefully extracting numeric components from a formatted string.
