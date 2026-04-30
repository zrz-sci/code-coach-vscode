// LeetCode 969: Pancake Sorting
// 解法一：贪心从大到小排列 —— 推荐解法
// 时间 O(n^2)，空间 O(1) 额外空间

class Solution {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> result;
        int n = arr.size();

        // 从最大值 n 开始，逐个放到正确位置
        for (int target = n; target >= 1; target--) {
            // 找到 target 在当前数组中的位置
            int idx = findIndex(arr, target);

            // 如果已经在正确位置（target-1），跳过
            if (idx == target - 1) {
                continue;
            }

            // 如果不在首位，先翻转到首位
            if (idx != 0) {
                // flip(idx + 1) 把 target 翻到 arr[0]
                reverse(arr.begin(), arr.begin() + idx + 1);
                result.push_back(idx + 1);
            }

            // 现在 target 在 arr[0]，翻转到目标位置 target-1
            reverse(arr.begin(), arr.begin() + target);
            result.push_back(target);
        }

        return result;
    }

private:
    // 在数组中查找值为 target 的元素位置
    int findIndex(const vector<int>& arr, int target) {
        for (int i = 0; i < (int)arr.size(); i++) {
            if (arr[i] == target) {
                return i;
            }
        }
        return -1; // 不应该到达这里
    }
};


// ================================================================
// 解法二：使用 STL find 的简洁版本
// 时间 O(n^2)，空间 O(1) 额外
// ================================================================

class Solution2 {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> flips;
        int n = arr.size();

        for (int size = n; size > 1; size--) {
            // 在 arr[0..size-1] 中找最大值的位置
            // 由于 arr 是 1..n 的排列，size 位置的目标值就是 size
            auto it = find(arr.begin(), arr.begin() + size, size);
            int idx = (int)(it - arr.begin());

            // 已经在正确位置
            if (idx == size - 1) continue;

            // 翻转到首位（如果不在首位）
            if (idx > 0) {
                reverse(arr.begin(), arr.begin() + idx + 1);
                flips.push_back(idx + 1);
            }

            // 翻转到目标位置
            reverse(arr.begin(), arr.begin() + size);
            flips.push_back(size);
        }

        return flips;
    }
};


// ================================================================
// 解法三：带有详细日志的版本（便于理解过程）
// 时间 O(n^2)，空间 O(1) 额外
// ================================================================

class Solution3 {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> result;
        int n = arr.size();

        for (int target = n; target >= 2; target--) {
            // 在 [0, target) 范围内查找 target
            int pos = -1;
            for (int i = 0; i < target; i++) {
                if (arr[i] == target) {
                    pos = i;
                    break;
                }
            }

            // 情况 1: 已经在正确位置 target-1
            if (pos == target - 1) {
                // 不需要翻转，继续下一个
                continue;
            }

            // 情况 2: 在位置 0（数组首部）
            if (pos == 0) {
                // 只需一次翻转，送到位置 target-1
                reverse(arr.begin(), arr.begin() + target);
                result.push_back(target);
                continue;
            }

            // 情况 3: 在中间某个位置
            // 第一步：翻转 [0, pos] 把 target 送到首部
            reverse(arr.begin(), arr.begin() + pos + 1);
            result.push_back(pos + 1);

            // 第二步：翻转 [0, target-1] 把 target 送到位置 target-1
            reverse(arr.begin(), arr.begin() + target);
            result.push_back(target);
        }

        return result;
    }
};


// ================================================================
// 解法四：使用 max_element 的通用版本
// 不依赖 arr 是 1..n 排列的性质
// 时间 O(n^2)，空间 O(1) 额外
// ================================================================

class Solution4 {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> result;
        int n = arr.size();

        for (int size = n; size > 1; size--) {
            // 在前 size 个元素中找最大值位置
            auto maxIt = max_element(arr.begin(), arr.begin() + size);
            int maxIdx = (int)(maxIt - arr.begin());

            // 已经在正确位置
            if (maxIdx == size - 1) continue;

            // 翻转最大值到首位
            if (maxIdx > 0) {
                reverse(arr.begin(), arr.begin() + maxIdx + 1);
                result.push_back(maxIdx + 1);
            }

            // 翻转最大值到 size-1 位置
            reverse(arr.begin(), arr.begin() + size);
            result.push_back(size);
        }

        return result;
    }
};


// ================================================================
// 解法五：递归版本
// 时间 O(n^2)，空间 O(n) 递归栈
// ================================================================

class Solution5 {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> result;
        sortHelper(arr, arr.size(), result);
        return result;
    }

private:
    void sortHelper(vector<int>& arr, int size, vector<int>& result) {
        // 基本情况：只剩一个元素，不需要排序
        if (size <= 1) return;

        // 找最大值在前 size 个元素中的位置
        int maxIdx = 0;
        for (int i = 1; i < size; i++) {
            if (arr[i] > arr[maxIdx]) {
                maxIdx = i;
            }
        }

        // 如果最大值不在末尾
        if (maxIdx != size - 1) {
            // 先翻到首位
            if (maxIdx > 0) {
                reverse(arr.begin(), arr.begin() + maxIdx + 1);
                result.push_back(maxIdx + 1);
            }
            // 再翻到末尾
            reverse(arr.begin(), arr.begin() + size);
            result.push_back(size);
        }

        // 递归处理前 size-1 个元素
        sortHelper(arr, size - 1, result);
    }
};
