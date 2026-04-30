# Changelog

All notable changes to Code Coach will be documented in this file.

## [1.1.0] - 2026-04-29

### Added
- Progress tracking: automatic recording of submit results (pass/fail, attempts, timestamps)
- Spaced repetition review schedule based on Ebbinghaus forgetting curve
- Status icons in My Solutions tree (passed / attempted / untouched)
- Session panel: progress bar, daily review count, next problem recommendation
- Smart problem recommendations following My Favorites folder order
- "Review due today" expandable list with click-to-open (problem description + solution)
- 1100+ pre-built hints and reference solutions included in the repo

### Fixed
- File links in hints/reference now use relative paths (portable across machines)

## [1.0.0] - 2026-04-20

### Added
- Browser-based login with Playwright (auto-captures session cookies)
- Support for both LeetCode US (leetcode.com) and CN (leetcode.cn)
- Sidebar panels: Session, Problems, Favorites, My Lists, My Solutions
- Problem description webview with English/Chinese toggle
- AI-generated study hints (hints.md) and reference solutions (reference.cpp) via Claude API
- Local compile & test with g++ (Cmd+Alt+T)
- Run against LeetCode test cases (Cmd+Alt+R)
- Submit solutions directly from VS Code (Cmd+Alt+S) with Beats % display
- Solution management with expandable tree (hints, reference, test cases)
- Re-fetch Favorites with automatic AI hint generation
- Re-fetch Local with multi-select problem picker
- Bilingual problem data (title + description in both English and Chinese)
- Settings panel: display language, API key, re-fetch options
- Offline login status detection (no network needed to show logged-in state)
- Automatic migration from legacy config directory
