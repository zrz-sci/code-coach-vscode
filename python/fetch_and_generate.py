#!/usr/bin/env python3
"""
Unified script: fetch LeetCode problems and generate AI-powered reference.cpp.

Usage:
    # === Batch generation (local only, no LeetCode login needed) ===
    python3 fetch_and_generate.py --range 1001 1300                  # Generate missing references in range
    python3 fetch_and_generate.py --range 1001 1300 --workers 3      # Parallel (max 5)
    python3 fetch_and_generate.py --range 1001 1300 --dry-run        # Preview targets
    python3 fetch_and_generate.py --retry 1003 1050 1077             # Retry specific IDs
    python3 fetch_and_generate.py --check-quality --range 1 1000     # Regenerate low-quality
    python3 fetch_and_generate.py --force --range 500 600            # Force regenerate

    # === Fetch + generate (requires LeetCode login) ===
    python3 fetch_and_generate.py                                    # Fetch all favorites + generate
    python3 fetch_and_generate.py --favorite SLUG                    # Specific favorite
    python3 fetch_and_generate.py --local                            # Re-fetch local problems

    # === API key ===
    export ANTHROPIC_API_KEY="sk-ant-..."                            # Environment variable
    python3 fetch_and_generate.py --api-key "sk-ant-..."             # Or explicit flag
"""
import argparse
import json
import os
import re
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

from config import load_config


# ============================================================
# Helper functions
# ============================================================

def is_sql_or_shell(problem_info: dict) -> bool:
    """Return True if problem is SQL/Database or Shell (should be skipped for C++ generation)."""
    tags = (problem_info.get('tags', '') or '').lower()
    return 'database' in tags or 'shell' in tags


def has_complete_reference(problem_dir: Path) -> bool:
    """Check if a problem directory has reference.cpp."""
    return (problem_dir / "reference.cpp").exists()


def check_reference_quality(problem_dir: Path) -> bool:
    """Return True if reference passes Level 2 quality, False if should be regenerated.
    Aligned with tools/check_reference_quality.py Level 2 checks.
    Design and SQL/Shell problems are considered passing (skip regeneration)."""
    ref_file = problem_dir / "reference.cpp"
    if not ref_file.exists():
        return False
    try:
        ref = ref_file.read_text(encoding='utf-8')

        # Skip SQL/Shell/Design problems — they don't follow standard format
        is_sql = 'SELECT' in ref or 'INSERT INTO' in ref or 'DELETE FROM' in ref or 'GROUP BY' in ref
        is_shell = 'awk ' in ref or 'grep ' in ref or '#!/bin/bash' in ref
        is_design = bool(re.findall(r'class (?!Solution)\w+\s*\{', ref))
        if is_sql or is_shell or is_design:
            return True  # skip these, don't regenerate

        # Level 1: structure
        if len(ref.strip()) < 1000:
            return False
        if '思维链' not in ref or '关键提示' not in ref:
            return False
        # Level 2: content completeness
        if '题目本质' not in ref:
            return False
        if '解法总览' not in ref and '解法概览' not in ref:
            return False
        if '解法对比' not in ref:
            return False
        if '易错点' not in ref:
            return False
        if '面试追问' not in ref:
            return False
        if '相关题型' not in ref:
            return False
        # At least 2 solution classes
        sol_count = len(re.findall(r'class Solution\d?\b', ref))
        if sol_count < 2:
            return False
        return True
    except Exception:
        return False


def resolve_api_key(explicit_key: str = None) -> str:
    """Resolve API key from: explicit arg > env var > VS Code settings."""
    if explicit_key:
        return explicit_key
    key = os.environ.get('ANTHROPIC_API_KEY', '')
    if key:
        return key
    # Try VS Code settings (macOS)
    settings_path = Path.home() / "Library/Application Support/Code/User/settings.json"
    if settings_path.exists():
        try:
            match = re.search(r'"leetcode\.anthropicApiKey"\s*:\s*"([^"]+)"', settings_path.read_text())
            if match:
                return match.group(1)
        except Exception:
            pass
    return ''


def discover_targets(problems_dir: Path, id_range: tuple = None, retry_ids: list = None,
                     force: bool = False, check_quality: bool = False,
                     max_count: int = 0) -> list:
    """
    Discover problems that need hint generation.

    Returns list of (pid: int, problem_dir: Path, problem_info: dict).
    Skips SQL/Shell problems automatically.
    """
    targets = []

    for d in sorted(problems_dir.iterdir()):
        if not d.is_dir() or d.name.startswith('.'):
            continue
        info_file = d / "problem_info.json"
        if not info_file.exists():
            continue

        try:
            info = json.loads(info_file.read_text(encoding='utf-8'))
            pid = int(info.get('id', 0))
        except (json.JSONDecodeError, ValueError, IOError):
            continue

        if pid <= 0:
            continue

        # Filter by ID
        if retry_ids:
            if pid not in retry_ids:
                continue
        elif id_range:
            if pid < id_range[0] or pid > id_range[1]:
                continue

        # Skip SQL/Shell
        if is_sql_or_shell(info):
            continue

        # Skip problems with no real content (stubs)
        content = info.get('content', '') or ''
        if len(content.strip()) < 50:
            continue

        # Check if generation is needed
        if force:
            pass  # always include
        elif check_quality:
            if has_complete_reference(d) and check_reference_quality(d):
                continue  # quality is fine, skip
        else:
            if has_complete_reference(d):
                continue  # already done

        targets.append((pid, d, info))

    targets.sort(key=lambda x: x[0])

    if max_count > 0:
        targets = targets[:max_count]

    return targets


def generate_one(pid: int, problem_dir: Path, problem_info: dict, api_key: str, output_name: str = "reference.cpp") -> tuple:
    """Wrapper around generate_reference with exception handling.
    Returns (pid, success, error_message)."""
    try:
        success = generate_reference(problem_dir, problem_info, api_key, output_name=output_name)
        return (pid, success, '')
    except Exception as e:
        return (pid, False, str(e))


def run_batch(targets: list, api_key: str, workers: int = 1, dry_run: bool = False, output_name: str = "reference.cpp") -> tuple:
    """
    Execute batch generation.
    Returns (generated_count, skipped_count, failed_ids).
    """
    if not targets:
        print("Nothing to generate!")
        return (0, 0, [])

    workers = min(max(workers, 1), 5)

    print(f"\n{'='*50}")
    print(f"  Targets: {len(targets)} problems | Workers: {workers} | Output: {output_name}")
    print(f"{'='*50}\n")

    if dry_run:
        for pid, d, info in targets:
            title = info.get('title', '?')
            difficulty = info.get('difficulty', '?')
            print(f"  #{pid}. {title} [{difficulty}]")
        print(f"\nDry run: {len(targets)} problems would be generated.")
        return (0, 0, [])

    generated = 0
    failed_ids = []
    start_time = time.time()

    if workers == 1:
        # Sequential
        for i, (pid, problem_dir, info) in enumerate(targets):
            title = info.get('title', '?')
            print(f"[{i+1}/{len(targets)}] #{pid}. {title}...", end=" ", flush=True)
            _, success, err = generate_one(pid, problem_dir, info, api_key, output_name=output_name)
            if success:
                print("OK")
                generated += 1
            else:
                print(f"FAIL: {err[:100]}")
                failed_ids.append(pid)
            time.sleep(1)
    else:
        # Parallel
        with ThreadPoolExecutor(max_workers=workers) as executor:
            futures = {
                executor.submit(generate_one, pid, d, info, api_key, output_name): (pid, info)
                for pid, d, info in targets
            }
            for i, future in enumerate(as_completed(futures)):
                pid, info = futures[future]
                title = info.get('title', '?')
                try:
                    _, success, err = future.result(timeout=180)
                    if success:
                        print(f"[{i+1}/{len(targets)}] #{pid}. {title} — OK")
                        generated += 1
                    else:
                        print(f"[{i+1}/{len(targets)}] #{pid}. {title} — FAIL: {err[:100]}")
                        failed_ids.append(pid)
                except Exception as e:
                    print(f"[{i+1}/{len(targets)}] #{pid}. {title} — ERROR: {e}")
                    failed_ids.append(pid)

    elapsed = time.time() - start_time
    mins, secs = int(elapsed // 60), int(elapsed % 60)

    print(f"\n{'='*50}")
    print(f"  Generated: {generated}/{len(targets)} | Time: {mins}m {secs}s")
    if failed_ids:
        failed_ids.sort()
        print(f"  Failed: {len(failed_ids)}")
        ids_str = ' '.join(str(x) for x in failed_ids)
        print(f"  Retry: python3 fetch_and_generate.py --retry {ids_str}")
    print(f"{'='*50}")

    return (generated, len(targets) - generated - len(failed_ids), failed_ids)


# ============================================================
# AI generation (core function — signature unchanged for compatibility)
# ============================================================

def load_teaching_skills() -> str:
    """Load the teaching skills knowledge base for AI generation prompts."""
    skills_path = Path(__file__).parent / "teaching_skills.md"
    if skills_path.exists():
        return skills_path.read_text(encoding='utf-8')
    return ""


def generate_reference(problem_dir: Path, problem_info: dict, api_key: str, output_name: str = "reference.cpp") -> bool:
    """Generate a unified reference.cpp with analysis and code."""
    import anthropic

    title = problem_info.get('title', '')
    title_cn = problem_info.get('title_cn', '') or title
    pid = problem_info.get('id', '')
    difficulty = problem_info.get('difficulty', '')
    tags = problem_info.get('tags', '')
    content = problem_info.get('content', '')
    content_cn = problem_info.get('content_cn', '') or content

    # Read the solution template to get the function signature
    solution_file = problem_dir / "solution.cpp"
    solution_code = ''
    if solution_file.exists():
        solution_code = solution_file.read_text()

    # Extract function signature only (skip includes and main)
    func_signature = ''
    for line in solution_code.split('\n'):
        stripped = line.strip()
        if stripped and not stripped.startswith('//') and not stripped.startswith('#') \
                and not stripped.startswith('using') and 'main()' not in stripped \
                and stripped not in ('', '{', '}'):
            func_signature += line + '\n'
    func_signature = func_signature[:500]

    # Load teaching methodology
    teaching_skills = load_teaching_skills()

    client = anthropic.Anthropic(api_key=api_key)

    system_prompt = f"""你是一位顶级算法面试教练。你写的 reference 让人读完觉得「我理解了这类问题的思考方式」。

你的文风：像大师写的教科书——每句话都有存在的理由，删掉任何一句都会损失信息。

{teaching_skills}"""

    quality_anchor = """示例（优秀思维链的标准）：
Step 1: 暴力 — 枚举所有对，O(n²)。n=10^4 时约 10^8 操作，卡边缘。
Step 2: 瓶颈 — 内层是线性查找 complement。能否 O(1) 查找？
Step 3: 死胡同 — 排序+双指针可行但打乱下标，O(n log n) 非最优。
Step 4: 突破 — 哈希表把查找降到 O(1)。边查边存避免自配对。"""

    unified_prompt = f"""为 LeetCode {pid}: {title_cn} ({title}) 生成 reference.cpp。
难度: {difficulty} | 标签: {tags}

{content_cn[:3000]}

{content[:2000]}

函数签名:
{func_signature}

===== 三条核心原则 =====

1. 思维链是灵魂 — 模拟从零推导出最优解的完整思考，包括弯路。
   {quality_anchor}

2. 解法多样 — 每种是真正不同的角度，不是同一算法微调。⭐ 标注最优解。

3. 极致精炼 — 这是最重要的原则。
   - 示例执行过程只展示关键 3-5 步状态变化，不要逐步罗列
   - 同一个知识点只说一次，不要在思维链、代码注释、易错点里重复
   - 空间优化不是独立解法，合并到同一个 Solution 里
   - 绝对禁止自我纠错：不要写"不对，让我重新…""等等，这里错了…"等痕迹。
     如果发现计算有误，直接给出正确版本，不要留下纠错过程。

===== 输出（纯 C++，不要 markdown）=====

/*
 * LeetCode {pid}: {title_cn} ({title})
 * 【题目本质】【思维链】【解法总览】【关键提示】
 */
// 解法1-N (class SolutionX) — 关键行注释解释 why
// 【解法对比】【易错点】【面试追问】【相关题型】

以 /* 开头，注释中文，变量英文，不要 #include/main/markdown/HTML，至少 2 种独立解法。"""

    print(f"    Generating reference...")
    response = client.messages.create(
        model="claude-opus-4-6",
        max_tokens=50000,
        system=system_prompt,
        messages=[{"role": "user", "content": unified_prompt}]
    )
    output = response.content[0].text

    # Clean up: remove markdown code block wrappers if present
    ref_content = output.strip()
    if ref_content.startswith('```cpp'):
        ref_content = ref_content[len('```cpp'):].strip()
    elif ref_content.startswith('```c++'):
        ref_content = ref_content[len('```c++'):].strip()
    elif ref_content.startswith('```'):
        ref_content = ref_content[3:].strip()
    if ref_content.endswith('```'):
        ref_content = ref_content[:-3].strip()

    # Post-processing: clean markdown artifacts from comments
    ref_content = re.sub(r'\*\*(.*?)\*\*', r'\1', ref_content)
    ref_content = re.sub(r'\[([^\]]+)\]\(https?://[^\)]+\)', r'\1', ref_content)
    # Remove backtick code fences from comment lines
    lines = ref_content.split('\n')
    cleaned_lines = []
    for line in lines:
        stripped = line.rstrip()
        # Skip backtick-only lines in comments
        if re.match(r'^\s*\*\s*```\w*\s*$', stripped):
            continue
        # Strip inline backticks in comment lines
        if stripped.lstrip().startswith('*') or stripped.lstrip().startswith('//'):
            line = re.sub(r'`([^`\n]+)`', r'\1', line)
        cleaned_lines.append(line)
    ref_content = '\n'.join(cleaned_lines)

    # Write file
    (problem_dir / output_name).write_text(ref_content + '\n', encoding='utf-8')

    # Level 1 quality check
    has_header = ref_content.startswith('/*')
    has_thinking = '思维链' in ref_content
    has_keyhints = '关键提示' in ref_content
    is_long_enough = len(ref_content.strip()) >= 1000
    if not (has_header and has_thinking and has_keyhints and is_long_enough):
        missing = []
        if not has_header: missing.append('/* header')
        if not has_thinking: missing.append('思维链')
        if not has_keyhints: missing.append('关键提示')
        if not is_long_enough: missing.append(f'length({len(ref_content)})')
        print(f"    ⚠ Quality check failed: missing {', '.join(missing)}")

    return True


# ============================================================
# Fetch-related functions (for --favorite / --local modes)
# ============================================================

def get_all_favorite_problems(client) -> list:
    """Fetch all problems from all user favorites."""
    print("Fetching favorites list...")
    favorites = client.get_favorites()
    if not favorites:
        print("No favorites found.")
        return []

    all_problems = []
    seen_slugs = set()

    for fav in favorites:
        fav_id = fav.get('idHash', '')
        fav_name = fav.get('name', 'Unknown')
        print(f"\n  Favorite: {fav_name} ({fav_id})")

        problems = client.get_favorite_problems(fav_id)
        if not problems:
            print(f"    (empty or failed)")
            continue

        for p in problems:
            slug = p.get('titleSlug', '')
            if slug and slug not in seen_slugs:
                seen_slugs.add(slug)
                all_problems.append(p)

        print(f"    {len(problems)} problems")

    print(f"\nTotal unique problems across all favorites: {len(all_problems)}")
    return all_problems


def fetch_problem_if_needed(client, problem: dict, problems_dir: Path) -> Path:
    """Fetch problem and update problem_info.json. Always re-fetches to update bilingual data.
    solution.cpp is preserved if it already exists (handled by template_generator)."""
    from template_generator import generate_with_structures

    slug = problem.get('titleSlug', '')
    pid = str(problem.get('frontendQuestionId', '0'))

    dir_name = f"{pid.zfill(4)}_{slug.replace('-', '_')}"
    problem_dir = problems_dir / dir_name

    # Always fetch to update problem_info.json with bilingual data
    print(f"  Fetching {pid}. {slug}...")
    full_problem = client.get_problem(slug)
    if not full_problem:
        print(f"    Failed to fetch!")
        # Return existing dir if it exists, None otherwise
        return problem_dir if problem_dir.exists() else None

    # Generate/update solution template (solution.cpp won't be overwritten if exists)
    try:
        generate_with_structures(full_problem, str(problems_dir))
    except Exception as e:
        print(f"    Warning: template generation failed ({e}), skipping template")
        # Still return the dir — problem_info.json may have been saved
        problem_dir.mkdir(parents=True, exist_ok=True)
    return problem_dir


def get_local_problems(client, problems_dir: Path) -> list:
    """Get all locally saved problems by reading problem_info.json files."""
    local_problems = []
    for info_file in sorted(problems_dir.glob('*/problem_info.json')):
        try:
            info = json.loads(info_file.read_text())
            slug = info.get('titleSlug', '')
            pid = info.get('id', '0')
            if slug:
                local_problems.append({
                    'titleSlug': slug,
                    'frontendQuestionId': pid,
                    'local_dir': str(info_file.parent),
                })
        except (json.JSONDecodeError, IOError):
            continue
    return local_problems


# ============================================================
# Main entry point
# ============================================================

def main():
    parser = argparse.ArgumentParser(
        description='Fetch LeetCode problems and/or generate AI reference solutions',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --range 1001 1300                Generate missing references for problems 1001-1300
  %(prog)s --range 1001 1300 --workers 3    Same, with 3 parallel workers
  %(prog)s --retry 1003 1050 1077           Retry specific failed problems
  %(prog)s --check-quality --range 1 1000   Regenerate low-quality references in 1-1000
  %(prog)s --favorite my-list               Fetch from favorite + generate
  %(prog)s --local                          Re-fetch local problems + generate
  %(prog)s                                  Fetch all favorites + generate
""")

    # Mode selection
    mode_group = parser.add_argument_group('Mode (pick one)')
    mode_group.add_argument('--range', type=int, nargs=2, metavar=('FROM', 'TO'),
                            help='Generate for problem IDs in range [FROM, TO] (local only)')
    mode_group.add_argument('--retry', type=int, nargs='+', metavar='ID',
                            help='Retry specific problem IDs (local only)')
    mode_group.add_argument('--favorite', help='Fetch + generate from a specific LeetCode favorite')
    mode_group.add_argument('--local', action='store_true',
                            help='Re-fetch locally existing problems, then generate')

    # Generation options
    gen_group = parser.add_argument_group('Generation options')
    gen_group.add_argument('--force', action='store_true',
                           help='Regenerate even if reference already exists')
    gen_group.add_argument('--check-quality', action='store_true',
                           help='Regenerate references that are low quality (< 1000 chars or missing key sections)')
    gen_group.add_argument('--workers', type=int, default=1,
                           help='Parallel workers, 1-5 (default: 1)')
    gen_group.add_argument('--max', type=int, default=0,
                           help='Max problems to process (0 = unlimited)')
    gen_group.add_argument('--dry-run', action='store_true',
                           help='Show targets without generating')
    gen_group.add_argument('--api-key',
                           help='Anthropic API key (or set ANTHROPIC_API_KEY env var)')
    gen_group.add_argument('--output-name', default='reference.cpp',
                           help='Output filename (default: reference.cpp, e.g. reference_v01.cpp)')
    gen_group.add_argument('--problems-dir',
                           help='Override problems directory path')

    args = parser.parse_args()

    # Resolve API key
    api_key = resolve_api_key(args.api_key)
    if not api_key and not args.dry_run:
        print("Error: Anthropic API key required for AI generation.")
        print("  Set ANTHROPIC_API_KEY env var, or use --api-key, or configure in VS Code settings.")
        print("  Use --dry-run to preview targets without generating.")
        sys.exit(1)

    # Resolve problems directory
    if args.problems_dir:
        problems_dir = Path(args.problems_dir)
    else:
        config = load_config()
        problems_dir = Path(config.get('problems_dir', 'problems'))

    if not problems_dir.exists():
        problems_dir.mkdir(parents=True, exist_ok=True)

    # ========== Batch mode: --range or --retry (local only, no LeetCode login) ==========
    if args.range or args.retry:
        id_range = tuple(args.range) if args.range else None
        targets = discover_targets(
            problems_dir=problems_dir,
            id_range=id_range,
            retry_ids=args.retry,
            force=args.force,
            check_quality=args.check_quality,
            max_count=args.max,
        )
        run_batch(targets, api_key, args.workers, args.dry_run, output_name=args.output_name)
        return

    # ========== Fetch mode: --favorite / --local / default (requires LeetCode login) ==========
    from api_client import LeetCodeClient

    client = LeetCodeClient()

    if not client.is_logged_in():
        print("Error: Not logged in. Run 'python3 leetcode_tool.py login' first.")
        print("  (For local-only generation, use --range or --retry instead.)")
        sys.exit(1)

    # Get problems to process
    if args.local:
        problems = get_local_problems(client, problems_dir)
        print(f"Found {len(problems)} local problems to re-fetch.")
    elif args.favorite:
        print(f"Fetching favorite: {args.favorite}")
        problems = client.get_favorite_problems(args.favorite)
        if not problems:
            print("No problems found in this favorite.")
            sys.exit(1)
    else:
        problems = get_all_favorite_problems(client)

    if not problems:
        sys.exit(1)

    # Phase 1: Fetch all problems
    print(f"\n{'='*50}")
    print(f"  Phase 1: Fetching {len(problems)} problems")
    print(f"{'='*50}\n")

    fetched_dirs = []
    for i, p in enumerate(problems):
        slug = p.get('titleSlug', '')
        pid = str(p.get('frontendQuestionId', '0'))
        print(f"[{i+1}/{len(problems)}] {pid}. {slug}")

        problem_dir = fetch_problem_if_needed(client, p, problems_dir)
        if problem_dir:
            fetched_dirs.append(problem_dir)
        else:
            print(f"  Skipped (fetch failed)")

        # Rate limit
        time.sleep(0.5)

    print(f"\nFetched {len(fetched_dirs)} problems.")

    if args.dry_run:
        print("\nDry run complete. Skipping AI generation.")
        return

    # Phase 2: Generate references (reuse run_batch)
    print(f"\n{'='*50}")
    print(f"  Phase 2: Generating AI references")
    print(f"{'='*50}\n")

    # Build targets from fetched dirs
    targets = []
    for problem_dir in fetched_dirs:
        info_file = problem_dir / "problem_info.json"
        if not info_file.exists():
            continue
        try:
            info = json.loads(info_file.read_text(encoding='utf-8'))
            pid = int(info.get('id', 0))
        except (json.JSONDecodeError, ValueError):
            continue

        if is_sql_or_shell(info):
            continue

        if not args.force and has_complete_reference(problem_dir):
            continue

        if args.check_quality and has_complete_reference(problem_dir) and check_reference_quality(problem_dir):
            continue

        targets.append((pid, problem_dir, info))

    targets.sort(key=lambda x: x[0])
    if args.max > 0:
        targets = targets[:args.max]

    run_batch(targets, api_key, args.workers, dry_run=False)


if __name__ == '__main__':
    main()
