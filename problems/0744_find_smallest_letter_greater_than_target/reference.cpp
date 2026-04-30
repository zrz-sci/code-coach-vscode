// 744. Find Smallest Letter Greater Than Target
// 难度: Easy
// 标签: Array, Binary Search

// ============================================================
// 解法一：二分查找（upper_bound） - 推荐
// 时间: O(log n)  空间: O(1)
// ============================================================
// 核心思想：在有序数组中找第一个严格大于 target 的元素
// 等价于 STL upper_bound。用取模处理环绕。

class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        int n = letters.size();
        int lo = 0, hi = n;  // 左闭右开区间 [lo, hi)

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (letters[mid] <= target) {
                // 当前字符 <= target，答案在右半区
                lo = mid + 1;
            } else {
                // 当前字符 > target，可能是答案，缩小右边界
                hi = mid;
            }
        }

        // lo == n 说明所有字符都 <= target，环绕返回第一个
        return letters[lo % n];
    }
};

// ============================================================
// 解法二：线性扫描
// 时间: O(n)  空间: O(1)
// ============================================================
// 由于数组有序，第一个大于 target 的就是答案

class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        for (char c : letters) {
            if (c > target) {
                return c;
            }
        }
        // 所有字符都 <= target，环绕
        return letters[0];
    }
};

// ============================================================
// 解法三：桶计数法
// 时间: O(n)  空间: O(1)  (固定26个字符)
// ============================================================
// 利用字符范围有限（只有26个小写字母），
// 先标记出现过的字符，然后从 target+1 开始环绕查找

class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        bool seen[26] = {};
        for (char c : letters) {
            seen[c - 'a'] = true;
        }

        // 从 target 的下一个字符开始，环绕搜索
        for (int i = 1; i <= 26; ++i) {
            int idx = (target - 'a' + i) % 26;
            if (seen[idx]) {
                return 'a' + idx;
            }
        }

        // 题目保证至少有两个不同字符，不会到这里
        return letters[0];
    }
};

// ============================================================
// 解法四：使用 STL upper_bound（一行解法）
// 时间: O(log n)  空间: O(1)
// ============================================================
// 直接调用标准库，展示对 STL 的熟练程度

class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        auto it = upper_bound(letters.begin(), letters.end(), target);
        return it == letters.end() ? letters[0] : *it;
    }
};

// ============================================================
// 测试用例分析
// ============================================================
// 输入: letters = ['c','f','j'], target = 'a'
// 二分过程: lo=0,hi=3 -> mid=1,'f'>'a' -> hi=1
//          lo=0,hi=1 -> mid=0,'c'>'a' -> hi=0
//          lo=0 -> letters[0]='c'  正确
//
// 输入: letters = ['c','f','j'], target = 'c'
// 二分过程: lo=0,hi=3 -> mid=1,'f'>'c' -> hi=1
//          lo=0,hi=1 -> mid=0,'c'<='c' -> lo=1
//          lo=1 -> letters[1]='f'  正确
//
// 输入: letters = ['x','x','y','y'], target = 'z'
// 二分过程: lo=0,hi=4 -> mid=2,'y'<='z' -> lo=3
//          lo=3,hi=4 -> mid=3,'y'<='z' -> lo=4
//          lo=4 -> letters[4%4]=letters[0]='x'  正确（环绕）
//
// 边界: letters = ['a','b'], target = 'a'
// 答案: 'b'（严格大于）
//
// 边界: letters = ['a','b'], target = 'z'
// 答案: 'a'（环绕）
