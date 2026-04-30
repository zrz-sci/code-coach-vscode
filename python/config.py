"""
Configuration management for LeetCode Tool
"""
import os
import json
from pathlib import Path

CONFIG_DIR = Path.home() / ".algo-helper"
LEGACY_CONFIG_DIR = Path.home() / ".leetcode_tool"  # Old location for migration
CONFIG_FILE = CONFIG_DIR / "config.json"
SESSION_FILE = CONFIG_DIR / "session.json"
CACHE_DIR = CONFIG_DIR / "cache"

# Default: relative to this script → ../../problems/
# If config.json has problems_dir (absolute), that takes priority via load_config()
_SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_PROBLEMS_DIR = str(_SCRIPT_DIR.parent.parent / "problems")

DEFAULT_CONFIG = {
    "problems_dir": DEFAULT_PROBLEMS_DIR,
    "language": "cpp",
    "editor": "code",
    "proxy": "",  # e.g., "http://127.0.0.1:7890"
    "display_language": "cn"  # "cn" for Chinese, "en" for English
}

def _migrate_legacy():
    """Auto-migrate from ~/.leetcode_tool/ to ~/.algo-helper/ if needed."""
    import shutil
    if not LEGACY_CONFIG_DIR.exists():
        return
    if CONFIG_DIR.exists() and SESSION_FILE.exists():
        return  # Already migrated
    CONFIG_DIR.mkdir(parents=True, exist_ok=True)
    for filename in ['session.json', 'config.json']:
        legacy_file = LEGACY_CONFIG_DIR / filename
        new_file = CONFIG_DIR / filename
        if legacy_file.exists() and not new_file.exists():
            shutil.copy2(str(legacy_file), str(new_file))
    # Migrate cache dir
    legacy_cache = LEGACY_CONFIG_DIR / "cache"
    if legacy_cache.exists() and not CACHE_DIR.exists():
        shutil.copytree(str(legacy_cache), str(CACHE_DIR))

# Run migration on import
_migrate_legacy()

def ensure_config_dir():
    """Ensure the config directory exists with secure permissions."""
    CONFIG_DIR.mkdir(parents=True, exist_ok=True)
    # Restrict directory permissions to owner only
    try:
        os.chmod(CONFIG_DIR, 0o700)
    except OSError:
        pass

def load_config():
    """Load configuration from file."""
    ensure_config_dir()
    if CONFIG_FILE.exists():
        try:
            with open(CONFIG_FILE, 'r') as f:
                config = json.load(f)
                return {**DEFAULT_CONFIG, **config}
        except (json.JSONDecodeError, IOError) as e:
            print(f"Warning: Failed to load config file, using defaults: {e}")
    return DEFAULT_CONFIG.copy()

def save_config(config):
    """Save configuration to file."""
    ensure_config_dir()
    with open(CONFIG_FILE, 'w') as f:
        json.dump(config, f, indent=2)

def load_session():
    """Load session data (cookies) from file."""
    ensure_config_dir()
    if SESSION_FILE.exists():
        try:
            with open(SESSION_FILE, 'r') as f:
                return json.load(f)
        except (json.JSONDecodeError, IOError) as e:
            print(f"Warning: Failed to load session file: {e}")
    return None

def save_session(session_data):
    """Save session data to file with restricted permissions."""
    ensure_config_dir()
    with open(SESSION_FILE, 'w') as f:
        json.dump(session_data, f, indent=2)
    # Restrict permissions to owner only (session contains auth cookies)
    try:
        os.chmod(SESSION_FILE, 0o600)
    except OSError:
        pass

def clear_session():
    """Clear saved session data."""
    if SESSION_FILE.exists():
        SESSION_FILE.unlink()

# ============ Cache Functions ============

CACHE_TTL = 3600  # 1 hour default TTL

def save_cache(key: str, data, site: str = 'us'):
    """Save data to file-based cache."""
    import time
    CACHE_DIR.mkdir(parents=True, exist_ok=True)
    cache_file = CACHE_DIR / f"{key}_{site}.json"
    cache_data = {"timestamp": time.time(), "site": site, "data": data}
    try:
        with open(cache_file, 'w') as f:
            json.dump(cache_data, f)
    except (IOError, OSError):
        pass

def load_cache(key: str, site: str = 'us', ttl: int = None):
    """Load data from file-based cache. Returns None if expired or missing."""
    import time
    if ttl is None:
        ttl = CACHE_TTL
    cache_file = CACHE_DIR / f"{key}_{site}.json"
    try:
        if cache_file.exists():
            with open(cache_file, 'r') as f:
                cache_data = json.load(f)
            if cache_data.get('site') == site and time.time() - cache_data.get('timestamp', 0) < ttl:
                return cache_data.get('data')
    except (json.JSONDecodeError, IOError, OSError):
        pass
    return None
