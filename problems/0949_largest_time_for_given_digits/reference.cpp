// LeetCode 949: Largest Time for Given Digits
// 解法一：全排列枚举（推荐）
// 时间复杂度: O(1) - 固定24种排列
// 空间复杂度: O(1)

class Solution {
public:
    string largestTimeFromDigits(vector<int>& arr) {
        // 排序，确保 next_permutation 能遍历所有排列
        sort(arr.begin(), arr.end());

        int maxMinutes = -1;  // 用总分钟数记录最大合法时间

        // 遍历所有 4! = 24 种排列
        do {
            int hours = arr[0] * 10 + arr[1];
            int minutes = arr[2] * 10 + arr[3];

            // 检查是否为合法的24小时制时间
            if (hours <= 23 && minutes <= 59) {
                maxMinutes = max(maxMinutes, hours * 60 + minutes);
            }
        } while (next_permutation(arr.begin(), arr.end()));

        // 无合法时间
        if (maxMinutes == -1) return "";

        // 将总分钟数转回 HH:MM 格式
        int h = maxMinutes / 60;
        int m = maxMinutes % 60;

        // 手动格式化，保证前导零
        string result = "";
        result += (char)('0' + h / 10);
        result += (char)('0' + h % 10);
        result += ':';
        result += (char)('0' + m / 10);
        result += (char)('0' + m % 10);

        return result;
    }
};


// 解法二：四重循环枚举（无需排列API）
// 时间复杂度: O(1) - 固定24种组合
// 空间复杂度: O(1)

class Solution2 {
public:
    string largestTimeFromDigits(vector<int>& arr) {
        int maxMinutes = -1;

        // 枚举4个位置分别使用哪个数字
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (j == i) continue;
                for (int k = 0; k < 4; k++) {
                    if (k == i || k == j) continue;
                    // 第四个位置自动确定
                    int l = 6 - i - j - k;  // 0+1+2+3 = 6

                    int hours = arr[i] * 10 + arr[j];
                    int minutes = arr[k] * 10 + arr[l];

                    if (hours <= 23 && minutes <= 59) {
                        maxMinutes = max(maxMinutes, hours * 60 + minutes);
                    }
                }
            }
        }

        if (maxMinutes == -1) return "";

        // 使用 to_string 和手动补零
        int h = maxMinutes / 60;
        int m = maxMinutes % 60;

        string result = "";
        if (h < 10) result += "0";
        result += to_string(h);
        result += ":";
        if (m < 10) result += "0";
        result += to_string(m);

        return result;
    }
};


// 解法三：从最大时间倒推
// 时间复杂度: O(1) - 最多1440次检查
// 空间复杂度: O(1)

class Solution3 {
public:
    string largestTimeFromDigits(vector<int>& arr) {
        // 对输入排序以便使用 multiset 进行匹配
        sort(arr.begin(), arr.end());

        // 从 23:59 开始倒推，找第一个能用给定数字组成的时间
        for (int h = 23; h >= 0; h--) {
            for (int m = 59; m >= 0; m--) {
                // 提取当前时间的4个数字
                vector<int> digits = {h / 10, h % 10, m / 10, m % 10};
                sort(digits.begin(), digits.end());

                // 如果排序后与输入相同，说明可以组成这个时间
                if (digits == arr) {
                    // 还需要确保 arr 排序了——已在开头排序
                    string result = "";
                    if (h < 10) result += "0";
                    result += to_string(h);
                    result += ":";
                    if (m < 10) result += "0";
                    result += to_string(m);
                    return result;
                }
            }
        }

        return "";
    }
};
