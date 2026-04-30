"""
C++ Template Generator for LeetCode Problems
"""
import json
import os
import re
from pathlib import Path
from typing import Dict, Optional
import html2text


def html_to_text(html_content: str) -> str:
    """Convert HTML content to plain text."""
    if not html_content:
        return ""
    h = html2text.HTML2Text()
    h.ignore_links = False
    h.ignore_images = True
    h.body_width = 80
    return h.handle(html_content)


def get_cpp_snippet(problem: Dict) -> str:
    """Extract C++ code snippet from problem data."""
    snippets = problem.get('codeSnippets') or []
    for snippet in snippets:
        if snippet.get('langSlug') == 'cpp':
            return snippet.get('code', '')
    return ""


def format_test_cases(problem: Dict) -> str:
    """Format example test cases for local testing."""
    test_cases = problem.get('exampleTestcases', '') or problem.get('sampleTestCase', '')
    if not test_cases:
        return "// No test cases available"

    lines = test_cases.strip().split('\n')
    formatted = []
    for i, line in enumerate(lines):
        formatted.append(f'    // Test case {i + 1}: {line}')
    return '\n'.join(formatted)


def is_design_problem(problem: Dict) -> bool:
    """Check if this is a design problem (uses custom class instead of Solution)."""
    # Check tags
    tags = problem.get('topicTags', [])
    for tag in tags:
        if tag.get('name', '').lower() == 'design':
            return True

    # Check if code snippet contains "class Solution"
    cpp_snippet = get_cpp_snippet(problem)
    if 'class Solution' not in cpp_snippet:
        return True

    return False


def extract_class_name(cpp_snippet: str) -> str:
    """Extract the main class name from the code snippet."""
    # Look for class definition
    match = re.search(r'class\s+(\w+)\s*\{', cpp_snippet)
    if match:
        return match.group(1)
    return 'Solution'


def generate_design_test_harness(class_name: str, test_cases: str) -> str:
    """Generate test harness for design problems."""
    return f'''int main() {{
    // This is a design problem using class: {class_name}
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
{format_test_cases_str(test_cases)}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // {class_name} obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}}'''


def format_test_cases_str(test_cases: str) -> str:
    """Format test cases as comments."""
    if not test_cases:
        return "    // No test cases available"

    lines = test_cases.strip().split('\n')
    formatted = []
    for i, line in enumerate(lines):
        formatted.append(f'    // {line}')
    return '\n'.join(formatted)


def generate_cpp_template(problem: Dict, output_dir: str) -> str:
    """Generate a C++ solution file with problem description and test harness."""

    problem_id = problem.get('questionFrontendId', '0')
    title = problem.get('title', 'Unknown')
    title_slug = problem.get('titleSlug', 'unknown')
    difficulty = problem.get('difficulty', 'Unknown')
    content = html_to_text(problem.get('content', ''))
    cpp_snippet = get_cpp_snippet(problem)
    test_cases = problem.get('exampleTestcases', '') or problem.get('sampleTestCase', '')

    # Get topic tags
    tags = problem.get('topicTags', [])
    tag_names = ', '.join([t.get('name', '') for t in tags]) if tags else 'None'

    # Check if it's a design problem
    design_problem = is_design_problem(problem)
    class_name = extract_class_name(cpp_snippet) if design_problem else 'Solution'

    # Generate appropriate test harness
    if design_problem:
        test_harness = generate_design_test_harness(class_name, test_cases)
    else:
        test_harness = f'''int main() {{
    Solution solution;

    // Example test cases from LeetCode:
{format_test_cases(problem)}

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {{1, 2, 3}};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}}'''

    # Create directory
    dir_name = f"{problem_id.zfill(4)}_{title_slug.replace('-', '_')}"
    problem_dir = Path(output_dir) / dir_name
    problem_dir.mkdir(parents=True, exist_ok=True)

    # Generate the C++ file
    cpp_content = f'''/*
 * LeetCode Problem {problem_id}: {title}
 * Difficulty: {difficulty}
 * Tags: {tag_names}
 *
 * Problem Description:
{_indent_text(content, ' * ')}
 *
 * Link: https://leetcode.com/problems/{title_slug}/
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

{cpp_snippet}

// ==================== Test Harness ====================
// Modify this section to test your solution locally

{test_harness}
'''

    # Write the solution file (only if it doesn't exist, to preserve user's code)
    solution_file = problem_dir / "solution.cpp"
    if not solution_file.exists():
        with open(solution_file, 'w') as f:
            f.write(cpp_content)

    # Write problem info JSON for tool usage
    info_file = problem_dir / "problem_info.json"
    info = {
        'id': problem_id,
        'title': title,
        'titleSlug': title_slug,
        'difficulty': difficulty,
        'tags': tag_names,
        'testCases': test_cases,
        'content': problem.get('content', ''),  # Save HTML content for webview
        'codeSnippets': problem.get('codeSnippets', []),  # Store for offline reset/redo
        # Store both languages if available (CN site provides both)
        'title_cn': problem.get('title_cn', ''),
        'title_en': problem.get('title_en', '') or title,
        'content_cn': problem.get('content_cn', ''),
        'content_en': problem.get('content_en', '') or problem.get('content', ''),
    }
    with open(info_file, 'w') as f:
        json.dump(info, f, indent=2)

    # Write raw test cases file
    if test_cases:
        test_file = problem_dir / "testcases.txt"
        with open(test_file, 'w') as f:
            f.write(test_cases)

    return str(solution_file)


def _indent_text(text: str, prefix: str) -> str:
    """Indent each line of text with a prefix."""
    lines = text.split('\n')
    indented = []
    for line in lines:
        # Limit line length
        if len(line) > 76:
            words = line.split()
            current_line = []
            current_len = 0
            for word in words:
                if current_len + len(word) + 1 <= 76:
                    current_line.append(word)
                    current_len += len(word) + 1
                else:
                    if current_line:
                        indented.append(prefix + ' '.join(current_line))
                    current_line = [word]
                    current_len = len(word)
            if current_line:
                indented.append(prefix + ' '.join(current_line))
        else:
            indented.append(prefix + line)
    return '\n'.join(indented)


# Common data structure templates for LeetCode
COMMON_STRUCTURES = '''
// ==================== Common Data Structures ====================

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Definition for a Node (N-ary tree / graph with neighbors)
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() : val(0), neighbors(vector<Node*>()) {}
    Node(int _val) : val(_val), neighbors(vector<Node*>()) {}
    Node(int _val, vector<Node*> _neighbors) : val(_val), neighbors(_neighbors) {}
};
'''


def generate_with_structures(problem: Dict, output_dir: str) -> str:
    """Generate C++ template with common data structures included."""
    solution_file = generate_cpp_template(problem, output_dir)

    # Check if problem uses common structures
    content = problem.get('content', '').lower()
    cpp_snippet = get_cpp_snippet(problem).lower()

    needs_structures = any(keyword in content or keyword in cpp_snippet
                         for keyword in ['listnode', 'treenode', 'linked list', 'binary tree', 'node*', 'vector<node'])

    if needs_structures:
        # Read current file and insert structures after includes
        with open(solution_file, 'r') as f:
            file_content = f.read()

        # Find position after using namespace std;
        insert_pos = file_content.find('using namespace std;')
        if insert_pos != -1:
            insert_pos = file_content.find('\n', insert_pos) + 1
            new_content = file_content[:insert_pos] + COMMON_STRUCTURES + file_content[insert_pos:]

            with open(solution_file, 'w') as f:
                f.write(new_content)

    return solution_file
