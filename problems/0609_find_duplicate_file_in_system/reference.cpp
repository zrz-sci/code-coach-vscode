// LeetCode 609: Find Duplicate File in System
// 方法: 哈希表 + 字符串解析

class Solution {
public:
    vector<vector<string>> findDuplicate(vector<string>& paths) {
        // content -> list of full file paths
        unordered_map<string, vector<string>> contentMap;

        for (const string& path : paths) {
            // 用 istringstream 按空格分割
            istringstream iss(path);
            string dir;
            iss >> dir; // 第一个是目录路径

            string fileInfo;
            while (iss >> fileInfo) {
                // fileInfo 格式: "filename.txt(content)"
                int parenPos = fileInfo.find('(');
                string fileName = fileInfo.substr(0, parenPos);
                string content = fileInfo.substr(parenPos + 1, fileInfo.size() - parenPos - 2);

                string fullPath = dir + "/" + fileName;
                contentMap[content].push_back(fullPath);
            }
        }

        // 过滤出有重复的组
        vector<vector<string>> result;
        for (auto& [content, files] : contentMap) {
            if (files.size() > 1) {
                result.push_back(files);
            }
        }

        return result;
    }
};
