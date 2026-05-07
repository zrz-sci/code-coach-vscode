#!/usr/bin/env python3
"""
LeetCode Tool - A full workflow tool for solving LeetCode problems in VS Code

Supports both LeetCode US (leetcode.com) and LeetCode China (leetcode.cn / 力扣)

Usage:
    python leetcode_tool.py site [us|cn]       - Switch or show current site
    python leetcode_tool.py login              - Login (auto-opens browser)
    python leetcode_tool.py login --cookie     - Login with manual cookie paste
    python leetcode_tool.py logout             - Logout and clear session
    python leetcode_tool.py status             - Check login status
    python leetcode_tool.py fetch <id/slug>    - Fetch a problem and create solution file
    python leetcode_tool.py search <keyword>   - Search for problems
    python leetcode_tool.py list [options]     - List problems
    python leetcode_tool.py favorites          - List your favorites (收藏夹)
    python leetcode_tool.py favorite <id>      - Show problems in a favorite list
    python leetcode_tool.py my-lists           - List your custom lists (个人题库)
    python leetcode_tool.py submit <path>      - Submit a solution
    python leetcode_tool.py run <path>         - Run code against test cases
    python leetcode_tool.py test <path>        - Compile and run local tests
    python leetcode_tool.py open <id/slug>     - Fetch problem and open in VS Code
"""

import argparse
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path
from typing import Optional

from api_client import LeetCodeClient, SITES
from config import load_config, save_config
from template_generator import generate_cpp_template, generate_with_structures
from browser_login import browser_login, browser_login_playwright, login_auto, check_playwright_installed, install_playwright


def colorize(text: str, color: str) -> str:
    """Add ANSI color to text. Respects --no-color flag and non-TTY output."""
    if not sys.stdout.isatty() or os.environ.get('NO_COLOR'):
        return text
    colors = {
        'red': '\033[91m',
        'green': '\033[92m',
        'yellow': '\033[93m',
        'blue': '\033[94m',
        'magenta': '\033[95m',
        'cyan': '\033[96m',
        'white': '\033[97m',
        'reset': '\033[0m'
    }
    return f"{colors.get(color, '')}{text}{colors['reset']}"


def get_terminal_width() -> int:
    """Get terminal width, with a sensible default."""
    try:
        return os.get_terminal_size().columns
    except OSError:
        return 120


def difficulty_color(difficulty: str) -> str:
    """Get color based on difficulty."""
    colors = {'Easy': 'green', 'Medium': 'yellow', 'Hard': 'red'}
    return colorize(difficulty, colors.get(difficulty, 'white'))


class LeetCodeTool:
    def __init__(self, site: str = None):
        self.client = LeetCodeClient(site)
        self.config = load_config()
        self.problems_dir = Path(self.config.get('problems_dir', 'problems'))
        self.problems_dir.mkdir(parents=True, exist_ok=True)

    def switch_site(self, site: str = None):
        """Switch between US and China LeetCode or show current site."""
        if site:
            if site not in SITES:
                print(colorize(f"✗ Invalid site: {site}. Use 'us' or 'cn'", 'red'))
                return False
            self.client.switch_site(site)
            site_info = self.client.get_current_site()
            print(colorize(f"✓ Switched to {site_info['name']}", 'green'))
            print(f"  URL: {site_info['url']}")
            return True
        else:
            site_info = self.client.get_current_site()
            print(colorize(f"Current site: {site_info['name']}", 'cyan'))
            print(f"  Code: {site_info['code']}")
            print(f"  URL: {site_info['url']}")
            print("\nAvailable sites:")
            for code, info in SITES.items():
                marker = "→ " if code == site_info['code'] else "  "
                print(f"  {marker}{code}: {info['name']} ({info['base']})")
            return True

    def login(self):
        """Interactive login using session cookies."""
        site_info = self.client.get_current_site()
        site_url = site_info['url']
        site_name = site_info['name']

        print(colorize(f"=== {site_name} Login ===", 'cyan'))
        print(f"\nTo login, you need to get your session cookies from the browser:")
        print(f"1. Log into {site_url} in your browser")
        print("2. Open Developer Tools (F12) -> Application -> Cookies")
        print("3. Copy the values of 'LEETCODE_SESSION' and 'csrftoken'\n")

        leetcode_session = input("Enter LEETCODE_SESSION cookie: ").strip()
        csrf_token = input("Enter csrftoken cookie: ").strip()

        if not leetcode_session or not csrf_token:
            print(colorize("Error: Both cookies are required", 'red'))
            return False

        print("\nVerifying credentials...")
        if self.client.login(leetcode_session, csrf_token):
            username = self.client.get_username()
            print(colorize(f"✓ Successfully logged in as: {username} on {site_name}", 'green'))
            return True
        else:
            print(colorize("✗ Login failed. Please check your cookies.", 'red'))
            return False

    def logout(self):
        """Logout and clear session."""
        site_info = self.client.get_current_site()
        self.client.logout()
        print(colorize(f"✓ Logged out from {site_info['name']} successfully", 'green'))

    def status(self):
        """Check login status."""
        site_info = self.client.get_current_site()
        print(colorize(f"Site: {site_info['name']}", 'cyan'))

        # Suppress network error output during status check
        import io, contextlib
        f = io.StringIO()
        with contextlib.redirect_stdout(f), contextlib.redirect_stderr(f):
            logged_in = self.client.is_logged_in()
            username = self.client.get_username() if logged_in else None

        if logged_in:
            print(colorize(f"✓ Logged in as: {username}", 'green'))
        else:
            print(colorize("✗ Not logged in", 'yellow'))
            print(f"Use 'python leetcode_tool.py login' to login to {site_info['name']}")

    def fetch(self, problem_identifier: str, open_in_editor: bool = False):
        """Fetch a problem and create solution file."""
        site_info = self.client.get_current_site()
        print(f"Fetching problem from {site_info['name']}: {problem_identifier}")

        # Try to fetch by ID first
        problem = None
        if problem_identifier.isdigit():
            problem = self.client.get_problem_by_id(int(problem_identifier))
        else:
            # Try as slug
            problem = self.client.get_problem(problem_identifier)

        if not problem:
            print(colorize(f"✗ Problem '{problem_identifier}' not found", 'red'))
            return None

        # Generate the solution file
        solution_file = generate_with_structures(problem, str(self.problems_dir))

        problem_id = problem.get('questionFrontendId', '?')
        title = problem.get('title', 'Unknown')
        difficulty = problem.get('difficulty', 'Unknown')

        print(colorize(f"\n✓ Problem {problem_id}: {title}", 'green'))
        print(f"  Difficulty: {difficulty_color(difficulty)}")
        print(f"  Solution file: {colorize(solution_file, 'cyan')}")

        if open_in_editor:
            editor = self.config.get('editor', 'code')
            subprocess.run([editor, solution_file])
            print(f"  Opened in {editor}")

        return solution_file

    def search(self, keyword: str):
        """Search for problems by keyword."""
        site_info = self.client.get_current_site()
        print(f"Searching on {site_info['name']} for: {keyword}\n")

        results = self.client.search_problems(keyword)
        if not results:
            print(colorize("No problems found", 'yellow'))
            return

        print(f"{'ID':<6} {'Slug':<45} {'Title':<40} {'Difficulty':<12}")
        print("-" * 105)

        for problem in results[:20]:  # Show top 20
            pid = problem.get('frontendQuestionId', '?')
            slug = problem.get('titleSlug', '')  # Include titleSlug
            title = problem.get('title', 'Unknown')[:38]
            diff = problem.get('difficulty', 'Unknown')
            status = problem.get('status', '')

            status_mark = ''
            if status == 'ac':
                status_mark = colorize('✓ ', 'green')
            elif status == 'notac':
                status_mark = colorize('○ ', 'yellow')

            print(f"{pid:<6} {slug:<45} {status_mark}{title:<40} {difficulty_color(diff):<12}")

    def list_problems(self, limit: int = 20, difficulty: str = None, skip: int = 0):
        """List problems with optional filtering."""
        site_info = self.client.get_current_site()
        print(f"Fetching problem list from {site_info['name']}...\n")

        all_questions = []
        current_skip = skip
        batch_size = 100  # LeetCode API limit per request

        # Fetch in batches if limit > 100
        while len(all_questions) < limit:
            fetch_limit = min(batch_size, limit - len(all_questions))
            result = self.client.get_problem_list(limit=fetch_limit, skip=current_skip, difficulty=difficulty)

            if not result or 'data' not in result:
                break

            questions = result['data'].get('problemsetQuestionList', {}).get('questions', [])
            if not questions:
                break

            all_questions.extend(questions)
            current_skip += len(questions)

            # If we got fewer than requested, we've reached the end
            if len(questions) < fetch_limit:
                break

        total = result['data'].get('problemsetQuestionList', {}).get('total', 0) if result else 0

        if not all_questions:
            print(colorize("Failed to fetch problems", 'red'))
            return

        print(f"Showing {len(all_questions)} of {total} problems\n")
        print(f"{'ID':<6} {'Slug':<45} {'Title':<40} {'Difficulty':<12} {'AC Rate':<10}")
        print("-" * 115)

        for problem in all_questions:
            pid = problem.get('frontendQuestionId', '?')
            slug = problem.get('titleSlug', '')  # Include titleSlug
            title = problem.get('title', 'Unknown')[:38]
            diff = problem.get('difficulty', 'Unknown')
            ac_rate = f"{problem.get('acRate', 0):.1f}%"
            status = problem.get('status', '')
            paid = problem.get('paidOnly', False)

            status_mark = ''
            if status == 'ac':
                status_mark = colorize('✓ ', 'green')
            elif status == 'notac':
                status_mark = colorize('○ ', 'yellow')

            if paid:
                title = colorize(f"🔒 {title}", 'yellow')

            print(f"{pid:<6} {slug:<45} {status_mark}{title:<40} {difficulty_color(diff):<12} {ac_rate:<10}")

    def list_favorites(self):
        """List user's favorite problem lists (收藏夹)."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']} to view favorites", 'red'))
            return

        print(f"Fetching favorites from {site_info['name']}...\n")

        favorites = self.client.get_favorites()
        if not favorites:
            print(colorize("No favorites found or failed to fetch", 'yellow'))
            return

        print(f"{'ID':<15} {'Name':<40} {'Public':<10}")
        print("-" * 65)

        for fav in favorites:
            fav_id = fav.get('idHash', '?')
            name = fav.get('name', 'Unknown')[:38]
            is_public = fav.get('isPublicFavorite', False)
            public_mark = colorize('Yes', 'cyan') if is_public else 'No'

            print(f"{fav_id:<15} {name:<40} {public_mark:<10}")

    def show_favorite(self, favorite_slug: str):
        """Show problems in a specific favorite list."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']}", 'red'))
            return

        print(f"Fetching favorite problems from {site_info['name']}...\n")

        problems = self.client.get_favorite_problems(favorite_slug)
        if not problems:
            print(colorize("No problems found or failed to fetch", 'yellow'))
            return

        print(f"Found {len(problems)} problems\n")
        print(f"{'ID':<8} {'Slug':<50} {'Title':<40} {'Difficulty':<12}")
        print("-" * 110)

        for problem in problems:
            pid = problem.get('frontendQuestionId', '?')
            slug = problem.get('titleSlug', '')  # Don't truncate slug
            title = problem.get('title', 'Unknown')[:38]
            diff = problem.get('difficulty', 'Unknown')
            status = problem.get('status', '')

            status_mark = ''
            if status == 'ac':
                status_mark = colorize('✓ ', 'green')
            elif status == 'notac':
                status_mark = colorize('○ ', 'yellow')

            print(f"{pid:<8} {slug:<50} {status_mark}{title:<40} {difficulty_color(diff):<12}")

    def list_user_lists(self):
        """List user's custom problem lists (个人题库/题单)."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']} to view lists", 'red'))
            return

        print(f"Fetching your problem lists from {site_info['name']}...\n")

        lists = self.client.get_user_lists()
        if not lists:
            print(colorize("No lists found or failed to fetch", 'yellow'))
            return

        print(f"{'ID':<15} {'Name':<40} {'Public':<10}")
        print("-" * 65)

        for lst in lists:
            lst_id = lst.get('slug', '?')
            name = lst.get('name', 'Unknown')[:38]
            is_public = lst.get('isPublicFavorite', False)
            public_mark = colorize('Yes', 'cyan') if is_public else 'No'

            print(f"{lst_id:<15} {name:<40} {public_mark:<10}")

    def create_favorite(self, name: str, is_public: bool = False):
        """Create a new favorite list on LeetCode."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']} to create a list", 'red'))
            return None

        print(f"Creating favorite list '{name}' on {site_info['name']}...")

        id_hash = self.client.create_favorite_list(name, is_public)
        if id_hash:
            print(colorize(f"✓ Created favorite list: {name}", 'green'))
            print(f"  ID: {id_hash}")
            return id_hash
        else:
            print(colorize("✗ Failed to create favorite list", 'red'))
            return None

    def add_to_favorite(self, favorite_id: str, problem_ids: list):
        """Add problems to a favorite list by their frontend IDs."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']}", 'red'))
            return

        print(f"Adding {len(problem_ids)} problems to favorite list on {site_info['name']}...\n")

        def progress(current, total, fid):
            print(f"  [{current}/{total}] Adding problem #{fid}...", end=' ', flush=True)

        results = self.client.batch_add_questions_to_favorite(
            favorite_id, problem_ids, progress_callback=progress
        )

        # Print individual results inline
        for fid in results['success']:
            # The progress line already printed, add result
            pass

        print(f"\n{colorize('Done!', 'green')}")
        print(f"  Added: {len(results['success'])} problems")
        if results['failed']:
            print(colorize(f"  Failed: {len(results['failed'])} problems", 'yellow'))
            for fail in results['failed']:
                print(f"    - #{fail['id']}: {fail['reason']}")

    def test_local(self, solution_path: str):
        """Compile and run the solution locally."""
        solution_file = Path(solution_path)
        if not solution_file.exists():
            print(colorize(f"✗ File not found: {solution_path}", 'red'))
            return False

        print(f"Compiling {solution_file.name}...")

        # Compile
        output_file = solution_file.parent / "solution"
        compile_cmd = [
            'g++', '-std=c++17', '-O2', '-Wall',
            str(solution_file), '-o', str(output_file)
        ]

        result = subprocess.run(compile_cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(colorize("✗ Compilation failed:", 'red'))
            print(result.stderr)
            return False

        print(colorize("✓ Compilation successful", 'green'))

        # Run
        print("\nRunning tests...")
        print("-" * 40)

        try:
            result = subprocess.run([str(output_file)], capture_output=True, text=True, timeout=30)
        except subprocess.TimeoutExpired:
            print(colorize("✗ Test execution timed out (30s limit)", 'red'))
            return False

        print(result.stdout)
        if result.stderr:
            print(colorize("Stderr:", 'yellow'))
            print(result.stderr)

        return result.returncode == 0

    def submit(self, solution_path: str):
        """Submit a solution to LeetCode."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']} to submit", 'red'))
            return False

        solution_file = Path(solution_path)
        if not solution_file.exists():
            print(colorize(f"✗ File not found: {solution_path}", 'red'))
            return False

        # Get problem info
        info_file = solution_file.parent / "problem_info.json"
        if not info_file.exists():
            print(colorize("✗ problem_info.json not found. Cannot determine which problem to submit.", 'red'))
            return False

        with open(info_file, 'r') as f:
            info = json.load(f)

        title_slug = info.get('titleSlug')
        if not title_slug:
            print(colorize("✗ Could not determine problem slug", 'red'))
            return False

        # Read solution code
        with open(solution_file, 'r') as f:
            code = f.read()

        # Extract only the Solution class (remove test harness)
        code = self._extract_solution_class(code)

        print(f"Submitting to {site_info['name']}: {info.get('title', title_slug)}")
        print("...")

        result = self.client.submit_solution(title_slug, code, 'cpp')
        if not result:
            print(colorize("✗ Submission failed", 'red'))
            return False

        submission_id = result.get('submission_id')
        if not submission_id:
            print(colorize("✗ No submission ID received", 'red'))
            return False

        print(f"Submission ID: {submission_id}")
        print("Checking result", end='', flush=True)

        # Poll for result
        for _ in range(30):  # Max 30 seconds
            time.sleep(1)
            print('.', end='', flush=True)

            status = self.client.check_submission(submission_id)
            if status and status.get('state') == 'SUCCESS':
                print()
                self._print_submission_result(status)
                accepted = status.get('status_msg') == 'Accepted'
                # Record progress
                self._update_progress(info_file, accepted)
                return accepted

        print(colorize("\n✗ Timeout waiting for result", 'yellow'))
        return False

    def run_tests(self, solution_path: str, test_input: str = None):
        """Run code against LeetCode test cases (requires login)."""
        site_info = self.client.get_current_site()

        if not self.client.is_logged_in():
            print(colorize(f"✗ You must be logged in to {site_info['name']} to run tests", 'red'))
            print("Use 'python leetcode_tool.py test <path>' for local testing")
            return False

        solution_file = Path(solution_path)
        if not solution_file.exists():
            print(colorize(f"✗ File not found: {solution_path}", 'red'))
            return False

        # Get problem info
        info_file = solution_file.parent / "problem_info.json"
        if not info_file.exists():
            print(colorize("✗ problem_info.json not found", 'red'))
            return False

        with open(info_file, 'r') as f:
            info = json.load(f)

        title_slug = info.get('titleSlug')

        # Read solution code
        with open(solution_file, 'r') as f:
            code = f.read()

        code = self._extract_solution_class(code)

        # Use provided test input or default from file
        if not test_input:
            test_file = solution_file.parent / "testcases.txt"
            if test_file.exists():
                with open(test_file, 'r') as f:
                    test_input = f.read().strip()
            else:
                test_input = info.get('testCases', '')

        print(f"Running tests on {site_info['name']}: {info.get('title', title_slug)}")
        print(f"Test input:\n{test_input}\n")

        result = self.client.run_code(title_slug, code, test_input, 'cpp')
        if not result:
            print(colorize("✗ Failed to run code", 'red'))
            return False

        interpret_id = result.get('interpret_id')
        if not interpret_id:
            print(colorize("✗ No interpret ID received", 'red'))
            return False

        print("Running", end='', flush=True)

        # Poll for result - use the correct site URL
        base_url = site_info['url']
        for _ in range(30):
            time.sleep(1)
            print('.', end='', flush=True)

            check_url = f"{base_url}/submissions/detail/{interpret_id}/check/"
            try:
                response = self.client.session.get(check_url)
                response.raise_for_status()
                status = response.json()
            except Exception as e:
                print(colorize(f"\n✗ Network error: {e}", 'red'))
                return False

            if status.get('state') == 'SUCCESS':
                print()
                self._print_run_result(status)
                return True

        print(colorize("\n✗ Timeout waiting for result", 'yellow'))
        return False

    def _update_progress(self, info_file: Path, accepted: bool):
        """Update progress tracking in problem_info.json after submit."""
        from datetime import datetime, timedelta
        try:
            with open(info_file, 'r') as f:
                info = json.load(f)

            progress = info.get('progress', {})
            now = datetime.now().isoformat()

            # Increment attempts
            progress['attempts'] = progress.get('attempts', 0) + 1

            # First attempt time
            if 'firstAttemptTime' not in progress:
                progress['firstAttemptTime'] = now

            if accepted:
                progress['status'] = 'passed'
                if 'passedTime' not in progress:
                    progress['passedTime'] = now
                # Set review schedule (Ebbinghaus)
                progress['lastReviewTime'] = now
                review_count = progress.get('reviewCount', 0)
                intervals = [1, 3, 7, 14, 30]  # days
                days = intervals[min(review_count, len(intervals) - 1)]
                next_review = datetime.now() + timedelta(days=days)
                progress['nextReviewTime'] = next_review.isoformat()
                progress['reviewCount'] = review_count + 1
            else:
                if progress.get('status') != 'passed':
                    progress['status'] = 'attempted'

            info['progress'] = progress
            with open(info_file, 'w') as f:
                json.dump(info, f, indent=2, ensure_ascii=False)

        except Exception as e:
            # Don't let progress tracking break submit flow
            print(f"  (progress update skipped: {e})")

    def _extract_solution_class(self, code: str) -> str:
        """Extract the solution code from the full code file (handles both Solution class and design problems)."""
        lines = code.split('\n')
        result = []
        in_class = False
        brace_count = 0
        in_main = False
        in_block_comment = False
        skip_header = True  # Skip the problem description header
        has_seen_brace = False  # Track whether we've seen an opening brace in current class

        for line in lines:
            stripped = line.strip()

            # Handle block comments (/* ... */)
            if not in_block_comment:
                if '/*' in stripped:
                    # Check if it's a single-line block comment
                    comment_start = stripped.index('/*')
                    rest_after_start = stripped[comment_start + 2:]
                    if '*/' in rest_after_start:
                        # Single-line block comment - only skip if the entire line is a comment
                        if stripped.startswith('/*') and stripped.endswith('*/'):
                            continue
                        # Otherwise it's inline, keep the line
                    else:
                        # Multi-line block comment starts
                        in_block_comment = True
                        continue
            else:
                if '*/' in line:
                    in_block_comment = False
                continue

            # Skip everything after "int main()" and test harness
            if re.match(r'^\s*int\s+main\s*\(', line):
                in_main = True
            if in_main:
                continue

            # Skip test harness comment section
            if '// ====================' in line:
                break

            # Include common data structures and includes
            if line.startswith('#include') or line.startswith('using namespace'):
                skip_header = False
                result.append(line)
                continue

            # Skip header comments until we see code
            if skip_header and (stripped.startswith('//') or stripped.startswith('*') or not stripped):
                continue

            skip_header = False

            # Track any class definition (Solution, MyQueue, MinStack, etc.)
            if re.match(r'^class\s+\w+', stripped) or re.match(r'^struct\s+\w+', stripped):
                in_class = True
                brace_count = 0
                has_seen_brace = False

            if in_class:
                result.append(line)
                brace_count += line.count('{') - line.count('}')
                if '{' in line:
                    has_seen_brace = True
                # Only end class when brace_count returns to 0 AFTER we've seen at least one brace
                if has_seen_brace and brace_count <= 0:
                    in_class = False
            elif stripped and not stripped.startswith('//'):
                # Include struct/typedef definitions before the class
                if any(keyword in line for keyword in ['struct ', 'typedef ']):
                    result.append(line)

        return '\n'.join(result)

    def _print_submission_result(self, status: dict):
        """Print formatted submission result."""
        status_msg = status.get('status_msg', 'Unknown')

        if status_msg == 'Accepted':
            print(colorize("✓ Accepted!", 'green'))
            runtime = status.get('status_runtime', 'N/A')
            memory = status.get('status_memory', 'N/A')
            runtime_pct = status.get('runtime_percentile')
            memory_pct = status.get('memory_percentile')
            print(f"  Runtime: {runtime}", end='')
            if runtime_pct is not None:
                print(f"  (Beats {runtime_pct:.1f}%)", end='')
            print()
            print(f"  Memory:  {memory}", end='')
            if memory_pct is not None:
                print(f"  (Beats {memory_pct:.1f}%)", end='')
            print()
        else:
            print(colorize(f"✗ {status_msg}", 'red'))

            if status.get('compile_error'):
                print(f"\nCompile Error:\n{status.get('full_compile_error', status.get('compile_error'))}")

            if status.get('runtime_error'):
                print(f"\nRuntime Error:\n{status.get('full_runtime_error', status.get('runtime_error'))}")

            if status_msg == 'Wrong Answer':
                print(f"\nLast testcase: {status.get('input_formatted', status.get('last_testcase', 'N/A'))}")
                print(f"Expected: {status.get('expected_output', 'N/A')}")
                print(f"Output: {status.get('code_output', 'N/A')}")

    def _print_run_result(self, status: dict):
        """Print formatted run result."""
        if status.get('run_success'):
            print(colorize("✓ Run completed", 'green'))
            print(f"\nOutput: {status.get('code_answer', status.get('code_output', []))}")
            if status.get('expected_code_answer'):
                print(f"Expected: {status.get('expected_code_answer')}")
        else:
            print(colorize("✗ Run failed", 'red'))

            if status.get('compile_error'):
                print(f"\nCompile Error:\n{status.get('full_compile_error', '')}")

            if status.get('runtime_error'):
                print(f"\nRuntime Error:\n{status.get('full_runtime_error', '')}")


    def generate_reference(self, problem_dir_or_file: str, api_key: str):
        """Generate reference.cpp for a single problem using AI."""
        problem_dir = Path(problem_dir_or_file)
        if problem_dir.is_file():
            problem_dir = problem_dir.parent

        info_file = problem_dir / 'problem_info.json'
        if not info_file.exists():
            print(colorize(f"✗ problem_info.json not found in {problem_dir}", 'red'))
            return False

        with open(info_file, 'r') as f:
            info = json.load(f)

        pid = info.get('id', '?')
        title = info.get('title', 'Unknown')

        # Check if already exists
        ref_exists = (problem_dir / 'reference.cpp').exists()
        if ref_exists:
            print(colorize(f"✓ {pid}. {title} — reference already exists", 'green'))
            return True

        print(f"Generating AI reference for {pid}. {title}...", flush=True)

        try:
            from fetch_and_generate import generate_reference
            success = generate_reference(problem_dir, info, api_key)
            if success:
                print(colorize(f"✓ Generated reference.cpp", 'green'))
                return True
            else:
                print(colorize("✗ Generation failed", 'red'))
                return False
        except ImportError:
            print(colorize("✗ Missing dependency: pip install anthropic", 'red'))
            return False
        except Exception as e:
            print(colorize(f"✗ Error: {e}", 'red'))
            return False

    def reset_solution(self, problem_dir_or_file: str):
        """Reset a solution to its initial template state for re-practice (重做).

        Keeps: problem_info.json, reference.cpp, testcases.txt
        Resets: solution.cpp (back to empty template)
        Deletes: compiled binary
        """
        problem_dir = Path(problem_dir_or_file)
        # If a file path was given (e.g. solution.cpp), use its parent directory
        if problem_dir.is_file():
            problem_dir = problem_dir.parent

        info_file = problem_dir / 'problem_info.json'
        if not info_file.exists():
            print(colorize(f"✗ problem_info.json not found in {problem_dir}", 'red'))
            return None

        with open(info_file, 'r') as f:
            info = json.load(f)

        title_slug = info.get('titleSlug', '')
        if not title_slug:
            print(colorize("✗ titleSlug not found in problem_info.json", 'red'))
            return None

        # Check if codeSnippets are stored locally
        code_snippets = info.get('codeSnippets')

        if code_snippets:
            # Rebuild a problem dict from local data for template generator
            problem_data = {
                'questionFrontendId': info.get('id', '0'),
                'title': info.get('title', ''),
                'titleSlug': title_slug,
                'difficulty': info.get('difficulty', ''),
                'content': info.get('content', ''),
                'topicTags': [{'name': t.strip()} for t in info.get('tags', '').split(',') if t.strip()],
                'exampleTestcases': info.get('testCases', ''),
                'codeSnippets': code_snippets,
                'title_cn': info.get('title_cn', ''),
                'title_en': info.get('title_en', ''),
                'content_cn': info.get('content_cn', ''),
                'content_en': info.get('content_en', ''),
            }
        else:
            # Re-fetch from API to get codeSnippets
            print(f"  codeSnippets not cached, fetching from API...")
            problem_data = self.client.get_problem(title_slug)
            if not problem_data:
                # Fallback: try by ID
                pid = info.get('id', '')
                if pid and pid.isdigit():
                    problem_data = self.client.get_problem_by_id(int(pid))
            if not problem_data:
                print(colorize(f"✗ Could not fetch problem '{title_slug}'. Are you online/logged in?", 'red'))
                return None
            # Store codeSnippets for future resets
            info['codeSnippets'] = problem_data.get('codeSnippets', [])
            with open(info_file, 'w') as f:
                json.dump(info, f, indent=2)

        # Delete solution.cpp and compiled binary
        solution_file = problem_dir / 'solution.cpp'
        binary_file = problem_dir / 'solution'
        if solution_file.exists():
            solution_file.unlink()
        if binary_file.exists():
            binary_file.unlink()

        # Regenerate solution.cpp from template
        parent_dir = str(problem_dir.parent)
        new_file = generate_with_structures(problem_data, parent_dir)

        # Add reference link if file exists
        if new_file:
            sol_path = Path(new_file)
            ref_new = problem_dir / 'reference_new.cpp'
            ref_old = problem_dir / 'reference.cpp'
            ref_path = ref_new if ref_new.exists() else ref_old
            if ref_path.exists():
                content = sol_path.read_text()
                link_block = f' * 💡 参考解法: file://{ref_path}\n *\n'
                content = content.replace('/*\n', '/*\n' + link_block, 1)
                sol_path.write_text(content)

        problem_id = info.get('id', '?')
        title = info.get('title', 'Unknown')
        print(colorize(f"\n✓ Solution reset: {problem_id}. {title}", 'green'))
        print(f"  File: {colorize(new_file, 'cyan')}")
        return new_file


def main():
    parser = argparse.ArgumentParser(
        description='LeetCode Tool - Full workflow for solving LeetCode problems (US & China)',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )

    # Global site option
    parser.add_argument('--site', choices=['us', 'cn'],
                       help='Use specific site (us=leetcode.com, cn=leetcode.cn)')
    parser.add_argument('--problems-dir',
                       help='Directory to store problems (default: ~/lc-practice)')

    subparsers = parser.add_subparsers(dest='command', help='Commands')

    # Site command
    site_parser = subparsers.add_parser('site', help='Switch or show current site (us/cn)')
    site_parser.add_argument('target', nargs='?', choices=['us', 'cn'],
                            help='Site to switch to (us=leetcode.com, cn=leetcode.cn/力扣)')

    # Login command
    login_parser = subparsers.add_parser('login', help='Login to LeetCode (auto-opens browser)')
    login_parser.add_argument('--cookie', action='store_true',
                              help='Use manual cookie paste mode (legacy)')
    login_parser.add_argument('--install-browser', action='store_true',
                              help='Install Playwright browser for auto-login')

    # Logout command
    subparsers.add_parser('logout', help='Logout from LeetCode')

    # Status command
    subparsers.add_parser('status', help='Check login status')

    # Fetch command
    fetch_parser = subparsers.add_parser('fetch', help='Fetch a problem')
    fetch_parser.add_argument('problem', help='Problem ID or slug')

    # Search command
    search_parser = subparsers.add_parser('search', help='Search problems')
    search_parser.add_argument('keyword', help='Search keyword')

    # List command
    list_parser = subparsers.add_parser('list', help='List problems')
    list_parser.add_argument('-n', '--limit', type=int, default=20, help='Number of problems')
    list_parser.add_argument('-d', '--difficulty', choices=['easy', 'medium', 'hard'],
                            help='Filter by difficulty')
    list_parser.add_argument('-s', '--skip', type=int, default=0, help='Skip first N problems')

    # Favorites command
    favorites_parser = subparsers.add_parser('favorites', help='List your favorites (收藏夹)')

    # Favorite command (show problems in a favorite)
    favorite_parser = subparsers.add_parser('favorite', help='Show problems in a favorite list')
    favorite_parser.add_argument('slug', help='Favorite slug/ID (from favorites or my-lists command)')

    # My lists command (个人题库/题单)
    subparsers.add_parser('my-lists', help='List your custom problem lists (个人题库)')

    # Create favorite command
    create_fav_parser = subparsers.add_parser('create-favorite', help='Create a new favorite list on LeetCode')
    create_fav_parser.add_argument('name', help='Name for the new favorite list')
    create_fav_parser.add_argument('--public', action='store_true', help='Make the list public')

    # Add to favorite command
    add_fav_parser = subparsers.add_parser('add-to-favorite', help='Add problems to a favorite list')
    add_fav_parser.add_argument('favorite_id', help='Favorite list ID (from favorites command)')
    add_fav_parser.add_argument('problems', nargs='+', type=int, help='Problem IDs to add (e.g., 1 2 3 15 704)')

    # Test command (local)
    test_parser = subparsers.add_parser('test', help='Compile and run locally')
    test_parser.add_argument('path', help='Path to solution.cpp')

    # Run command (on LeetCode)
    run_parser = subparsers.add_parser('run', help='Run on LeetCode test cases')
    run_parser.add_argument('path', help='Path to solution.cpp')
    run_parser.add_argument('-i', '--input', help='Custom test input')

    # Submit command
    submit_parser = subparsers.add_parser('submit', help='Submit solution')
    submit_parser.add_argument('path', help='Path to solution.cpp')

    # Open command
    open_parser = subparsers.add_parser('open', help='Fetch and open in VS Code')
    open_parser.add_argument('problem', help='Problem ID or slug')

    # Config command
    config_parser = subparsers.add_parser('config', help='Configure settings')
    config_parser.add_argument('--proxy', help='Set proxy URL (e.g., http://127.0.0.1:7890)')
    config_parser.add_argument('--lang', choices=['cn', 'en'],
                               help='Set display language for problem titles/descriptions (cn/en)')

    # Generate reference command
    ref_parser = subparsers.add_parser('generate-reference', help='Generate AI reference for a problem')
    ref_parser.add_argument('path', help='Path to solution.cpp or problem directory')
    ref_parser.add_argument('--api-key', help='Anthropic API key (or set ANTHROPIC_API_KEY env var)')

    # Reset command
    reset_parser = subparsers.add_parser('reset', help='Reset solution to initial template (re-do / 重做)')
    reset_parser.add_argument('path', help='Path to solution.cpp or problem directory')

    args = parser.parse_args()

    if not args.command:
        parser.print_help()
        return

    # Initialize tool with optional site override
    tool = LeetCodeTool(site=args.site)

    # Override problems_dir if specified
    if args.problems_dir:
        tool.problems_dir = Path(args.problems_dir)
        tool.problems_dir.mkdir(parents=True, exist_ok=True)

    if args.command == 'site':
        tool.switch_site(args.target)
    elif args.command == 'login':
        if args.install_browser:
            install_playwright()
        elif args.cookie:
            tool.login()
        else:
            # If --site is explicitly set, use it; otherwise login_auto will ask
            site = args.site  # None if not specified
            success = login_auto(site)
            if not success:
                print("\nAuto login failed. You can try:")
                print("  python3 leetcode_tool.py login --cookie        (manual cookie paste)")
                print("  python3 leetcode_tool.py login --install-browser (install auto-login)")
    elif args.command == 'logout':
        tool.logout()
    elif args.command == 'status':
        tool.status()
    elif args.command == 'fetch':
        tool.fetch(args.problem)
    elif args.command == 'search':
        tool.search(args.keyword)
    elif args.command == 'list':
        tool.list_problems(args.limit, args.difficulty, args.skip)
    elif args.command == 'favorites':
        tool.list_favorites()
    elif args.command == 'favorite':
        tool.show_favorite(args.slug)
    elif args.command == 'my-lists':
        tool.list_user_lists()
    elif args.command == 'create-favorite':
        tool.create_favorite(args.name, args.public)
    elif args.command == 'add-to-favorite':
        tool.add_to_favorite(args.favorite_id, args.problems)
    elif args.command == 'test':
        tool.test_local(args.path)
    elif args.command == 'run':
        tool.run_tests(args.path, args.input)
    elif args.command == 'submit':
        tool.submit(args.path)
    elif args.command == 'open':
        tool.fetch(args.problem, open_in_editor=True)
    elif args.command == 'reset':
        tool.reset_solution(args.path)
    elif args.command == 'generate-reference':
        api_key = getattr(args, 'api_key', None) or os.environ.get('ANTHROPIC_API_KEY', '')
        if not api_key:
            print(colorize("✗ Anthropic API key required. Set ANTHROPIC_API_KEY env var or use --api-key", 'red'))
        else:
            tool.generate_reference(args.path, api_key)
    elif args.command == 'config':
        config = load_config()
        changed = False
        if args.proxy is not None:
            config['proxy'] = args.proxy
            changed = True
            if args.proxy:
                print(colorize(f"✓ Proxy set to: {args.proxy}", 'green'))
            else:
                print(colorize("✓ Proxy cleared", 'green'))
        if args.lang is not None:
            config['display_language'] = args.lang
            changed = True
            lang_name = '中文' if args.lang == 'cn' else 'English'
            print(colorize(f"✓ Display language set to: {lang_name} ({args.lang})", 'green'))
        if changed:
            save_config(config)
        else:
            print("Current configuration:")
            for key, value in config.items():
                if key == 'display_language':
                    lang_name = '中文' if value == 'cn' else 'English'
                    print(f"  {key}: {value} ({lang_name})")
                else:
                    print(f"  {key}: {value}")


if __name__ == '__main__':
    main()
