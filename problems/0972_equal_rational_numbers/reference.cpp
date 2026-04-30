[file://solution.cpp](solution.cpp)

// =============================================================================
// LeetCode 972: Equal Rational Numbers
// Approach 1: Expand Repeating Part + Double Comparison
//
// Parse each string into (intPart, nonRepeat, repeat). Build an expanded
// decimal string by appending the repeating block many times. Convert to
// double and compare. Works because input bounds are tiny.
//
// Time:  O(1)   -- all lengths bounded by constants
// Space: O(1)
// =============================================================================

class Solution {
public:
    // Helper: parse a rational string into its three components
    // and return an expanded decimal string for numeric comparison
    string expand(const string& s) {
        string intPart, nonRepeat, repeat;

        // Find the decimal point position
        int dotPos = s.find('.');
        if (dotPos == string::npos) {
            // No decimal point at all, e.g. "123"
            intPart = s;
            nonRepeat = "";
            repeat = "";
        } else {
            intPart = s.substr(0, dotPos);
            // Find the opening parenthesis
            int parenPos = s.find('(');
            if (parenPos == string::npos) {
                // No repeating part, e.g. "1.5" or "1."
                nonRepeat = s.substr(dotPos + 1);
                repeat = "";
            } else {
                // Has repeating part, e.g. "0.1(6)" or "0.(52)"
                nonRepeat = s.substr(dotPos + 1, parenPos - dotPos - 1);
                // Extract content between ( and )
                int closePos = s.find(')');
                repeat = s.substr(parenPos + 1, closePos - parenPos - 1);
            }
        }

        // Build the expanded string: intPart.nonRepeat + repeat * 20
        string expanded = intPart + "." + nonRepeat;
        for (int i = 0; i < 20; i++) {
            expanded += repeat;
        }
        return expanded;
    }

    bool isRationalEqual(string s, string t) {
        // Expand both strings and compare as doubles
        double valS = stod(expand(s));
        double valT = stod(expand(t));
        return valS == valT;
    }
};

// =============================================================================
// LeetCode 972: Equal Rational Numbers
// Approach 2: Exact Fraction Conversion
//
// Convert each rational string to an exact fraction (numerator / denominator).
// A number of the form I.NR(RP) where I = integer part, NR = non-repeating
// digits of length p, RP = repeating digits of length q, has the value:
//
//   value = I + NR / 10^p + RP / (10^p * (10^q - 1))
//
// Combine into a single fraction, reduce by GCD, and compare.
//
// Time:  O(1)
// Space: O(1)
// =============================================================================

class SolutionFraction {
public:
    // Compute GCD for fraction reduction
    long long gcd(long long a, long long b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

    // Parse a rational string and return {numerator, denominator}
    pair<long long, long long> toFraction(const string& s) {
        string intPart, nonRepeat, repeat;

        int dotPos = s.find('.');
        if (dotPos == string::npos) {
            intPart = s;
            nonRepeat = "";
            repeat = "";
        } else {
            intPart = s.substr(0, dotPos);
            int parenPos = s.find('(');
            if (parenPos == string::npos) {
                nonRepeat = s.substr(dotPos + 1);
                repeat = "";
            } else {
                nonRepeat = s.substr(dotPos + 1, parenPos - dotPos - 1);
                int closePos = s.find(')');
                repeat = s.substr(parenPos + 1, closePos - parenPos - 1);
            }
        }

        long long I = intPart.empty() ? 0 : stoll(intPart);
        long long NR = nonRepeat.empty() ? 0 : stoll(nonRepeat);
        long long RP = repeat.empty() ? 0 : stoll(repeat);
        int p = nonRepeat.size();  // length of non-repeating part
        int q = repeat.size();     // length of repeating part

        long long pow10p = 1;
        for (int i = 0; i < p; i++) pow10p *= 10;

        long long pow10q = 1;
        for (int i = 0; i < q; i++) pow10q *= 10;

        long long numerator, denominator;

        if (q == 0) {
            // No repeating part: value = I + NR / 10^p
            numerator = I * pow10p + NR;
            denominator = pow10p;
        } else {
            // With repeating part:
            // value = I + NR / 10^p + RP / (10^p * (10^q - 1))
            // Combine over common denominator 10^p * (10^q - 1):
            // numerator = I * pow10p * (pow10q - 1) + NR * (pow10q - 1) + RP
            // denominator = pow10p * (pow10q - 1)
            long long repDenom = pow10q - 1;
            denominator = pow10p * repDenom;
            numerator = I * denominator + NR * repDenom + RP;
        }

        // Reduce the fraction
        long long g = gcd(abs(numerator), abs(denominator));
        numerator /= g;
        denominator /= g;

        return {numerator, denominator};
    }

    bool isRationalEqual(string s, string t) {
        auto fracS = toFraction(s);
        auto fracT = toFraction(t);
        return fracS.first == fracT.first && fracS.second == fracT.second;
    }
};
