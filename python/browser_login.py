"""
Browser-based login for LeetCode
Supports automatic login via Playwright (recommended) and manual cookie input (fallback).
"""
import sys
import json
import webbrowser
import http.server
import urllib.parse
from pathlib import Path
from config import save_session, load_session, load_config, save_config

# LeetCode URLs
SITES = {
    'us': {
        'name': 'LeetCode US',
        'base': 'https://leetcode.com',
        'login_url': 'https://leetcode.com/accounts/login/',
    },
    'cn': {
        'name': 'LeetCode China (力扣)',
        'base': 'https://leetcode.cn',
        'login_url': 'https://leetcode.cn/accounts/login/',
    }
}

# Persistent browser profile directory
BROWSER_PROFILE_DIR = Path.home() / ".algo-helper" / "browser_profile"


def _extract_leetcode_cookies(cookies: list) -> tuple:
    """Extract LEETCODE_SESSION and csrftoken from a list of cookie dicts."""
    session_cookie = None
    csrf_cookie = None
    for cookie in cookies:
        if cookie['name'] == 'LEETCODE_SESSION':
            session_cookie = cookie['value']
        elif cookie['name'] == 'csrftoken':
            csrf_cookie = cookie['value']
    return session_cookie, csrf_cookie


def _save_leetcode_cookies(session_cookie: str, csrf_cookie: str, site: str) -> None:
    """Save extracted cookies to session.json."""
    session_data = load_session() or {}
    session_data[f'cookies_{site}'] = {
        'LEETCODE_SESSION': session_cookie,
        'csrftoken': csrf_cookie
    }
    save_session(session_data)


def _select_site_interactive() -> str:
    """Let user choose site interactively in CLI."""
    config = load_config()
    current = config.get('site', 'us')

    print("\nSelect site / 选择站点:")
    print(f"  1. LeetCode US   (leetcode.com)  {'← current' if current == 'us' else ''}")
    print(f"  2. LeetCode CN   (leetcode.cn)   {'← current' if current == 'cn' else ''}")
    print()

    choice = input(f"Enter 1 or 2 (default: {'1' if current == 'us' else '2'}): ").strip()

    if choice == '1':
        site = 'us'
    elif choice == '2':
        site = 'cn'
    else:
        site = current

    # Save the chosen site
    config['site'] = site
    save_config(config)

    return site


def browser_login(site: str = None) -> bool:
    """
    Manual cookie login - opens browser and asks user to paste cookies.
    This is the fallback method when Playwright is not available.
    """
    config = load_config()
    site = site or config.get('site', 'us')
    site_config = SITES.get(site, SITES['us'])

    print(f"\n{'='*50}")
    print(f"  {site_config['name']} Login (Manual)")
    print(f"{'='*50}")

    # Open browser to login page
    print(f"\nOpening {site_config['login_url']} in your browser...")
    webbrowser.open(site_config['login_url'])

    print("\n" + "="*50)
    print("After logging in, get your cookies:")
    print("="*50)
    print("""
1. Press F12 to open Developer Tools
2. Go to Application tab (Chrome) or Storage tab (Safari)
3. Click Cookies -> select the leetcode site
4. Copy the values below:
""")

    # Get cookies from user
    print("Paste LEETCODE_SESSION cookie value:")
    session_cookie = input().strip()

    if not session_cookie:
        print("\nNo session cookie provided. Login cancelled.")
        return False

    print("\nPaste csrftoken cookie value:")
    csrf_cookie = input().strip()

    if not csrf_cookie:
        print("\nNo csrf token provided. Login cancelled.")
        return False

    # Save the session
    _save_leetcode_cookies(session_cookie, csrf_cookie, site)

    print(f"\nLogin successful!")
    print(f"Session saved for {site_config['name']}")
    return True


def browser_login_playwright(site: str = None) -> bool:
    """
    Automated browser login using Playwright with persistent context.
    Opens a browser window, user logs in normally, cookies are captured automatically.
    Browser profile is saved so login state persists between sessions.
    """
    try:
        from playwright.sync_api import sync_playwright
    except ImportError:
        print("Error: playwright is not installed.")
        print("Please run: pip install playwright && playwright install chromium")
        return False

    config = load_config()
    site = site or config.get('site', 'us')
    site_config = SITES.get(site, SITES['us'])

    # Persistent browser profile per site
    profile_dir = BROWSER_PROFILE_DIR / site
    profile_dir.mkdir(parents=True, exist_ok=True)

    print(f"\n{'='*50}")
    print(f"  {site_config['name']} Login")
    print(f"{'='*50}")
    print("\n1. A browser window will open")
    print("2. Login with your LeetCode account")
    print("3. Once logged in, the window will close automatically")
    print("\nStarting browser...\n")

    try:
        with sync_playwright() as p:
            # Use persistent context - browser remembers login state
            print("Launching browser...")
            try:
                context = p.chromium.launch_persistent_context(
                    str(profile_dir),
                    headless=False,
                    args=['--disable-blink-features=AutomationControlled'],
                )
            except Exception:
                # Fallback to WebKit if Chromium not available
                print("Chromium not available, trying WebKit...")
                try:
                    context = p.webkit.launch_persistent_context(
                        str(profile_dir),
                        headless=False,
                    )
                except Exception as e:
                    print(f"No browser available: {e}")
                    print("Please run: python3 -m playwright install chromium")
                    return False

            page = context.pages[0] if context.pages else context.new_page()

            # First load the site to make cookies available in persistent context
            print(f"Opening {site_config['base']}...")
            page.goto(site_config['base'], wait_until='domcontentloaded')
            page.wait_for_timeout(2000)

            # Check if already logged in from persistent context
            cookies = context.cookies()
            session_cookie, csrf_cookie = _extract_leetcode_cookies(cookies)
            if session_cookie and csrf_cookie:
                # Check if we're on the login page (session expired)
                current_url = page.url
                if '/login' not in current_url and '/accounts/login' not in current_url:
                    _save_leetcode_cookies(session_cookie, csrf_cookie, site)
                    print(f"\nLogin successful! (restored from saved session)")
                    print(f"Session saved for {site_config['name']}")
                    context.close()
                    return True
                # Session cookies exist but expired, fall through to login

            # Navigate to login page
            print(f"Opening {site_config['login_url']}")
            page.goto(site_config['login_url'], wait_until='domcontentloaded')

            # Wait for user to login
            print("Waiting for you to login...")
            print("(The window will close automatically after successful login)\n")

            try:
                # Poll for login success using cookies (reliable for both US and CN)
                # This approach doesn't depend on DOM selectors or URL patterns,
                # which differ between US and CN sites.
                timeout_seconds = 300
                for i in range(timeout_seconds * 2):  # check every 0.5s
                    page.wait_for_timeout(500)

                    # Check if still on login page
                    current_url = page.url
                    if '/login' in current_url or '/accounts/login' in current_url:
                        continue

                    # Not on login page — check for valid cookies
                    cookies = context.cookies()
                    session_cookie, csrf_cookie = _extract_leetcode_cookies(cookies)
                    if session_cookie and csrf_cookie:
                        print("Login detected, capturing cookies...")
                        page.wait_for_timeout(1000)

                        # Re-read cookies (may have been updated)
                        cookies = context.cookies()
                        session_cookie, csrf_cookie = _extract_leetcode_cookies(cookies)
                        if session_cookie and csrf_cookie:
                            break
                else:
                    # Timeout
                    context.close()
                    print(f"\nLogin timed out (5 minutes). Please try again.")
                    return False

                context.close()

                _save_leetcode_cookies(session_cookie, csrf_cookie, site)
                print(f"\nLogin successful!")
                print(f"Session saved for {site_config['name']}")
                return True

            except Exception as e:
                context.close()
                print(f"\nLogin failed: {e}")
                return False

    except Exception as e:
        print(f"\nBrowser error: {e}")
        return False


def login_auto(site: str = None) -> bool:
    """
    Automatic login orchestrator.
    If site is not specified, asks user to choose.
    Uses Playwright if available, otherwise falls back to manual cookie input.
    """
    # If site not explicitly set, let user choose
    if site is None:
        site = _select_site_interactive()

    if check_playwright_installed():
        return browser_login_playwright(site)
    else:
        print("="*50)
        print("  Playwright is not installed")
        print("="*50)
        print("\nFor the best login experience (auto-open browser, no cookie copying),")
        print("install Playwright with:")
        print()
        print("  pip install playwright && python3 -m playwright install chromium")
        print()
        print("Falling back to manual cookie login...\n")
        return browser_login(site)


def check_playwright_installed() -> bool:
    """Check if playwright and browser are installed."""
    try:
        from playwright.sync_api import sync_playwright
        return True
    except ImportError:
        return False


def install_playwright():
    """Install playwright and browser."""
    import subprocess
    print("Installing playwright...")
    subprocess.run([sys.executable, "-m", "pip", "install", "playwright"], check=True)
    print("Installing Chromium browser...")
    subprocess.run([sys.executable, "-m", "playwright", "install", "chromium"], check=True)
    print("Done! You can now use: python3 leetcode_tool.py login")


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Browser-based LeetCode login')
    parser.add_argument('--site', choices=['us', 'cn'], default=None,
                        help='Site to login to (us or cn)')
    parser.add_argument('--install', action='store_true',
                        help='Install playwright and browser')
    parser.add_argument('--cookie', action='store_true',
                        help='Use manual cookie paste method')
    args = parser.parse_args()

    if args.install:
        install_playwright()
    elif args.cookie:
        browser_login(args.site)
    else:
        login_auto(args.site)
