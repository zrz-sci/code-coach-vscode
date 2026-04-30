"""
LeetCode API Client
Handles authentication and API interactions with LeetCode (US and China)
"""
import requests
import json
import re
import time
from typing import Optional, Dict, Any, List
from config import load_session, save_session, clear_session, load_config, save_config, save_cache, load_cache

# LeetCode site configurations
SITES = {
    'us': {
        'name': 'LeetCode US',
        'base': 'https://leetcode.com',
        'graphql': 'https://leetcode.com/graphql',
        'domain': '.leetcode.com'
    },
    'cn': {
        'name': 'LeetCode China (力扣)',
        'base': 'https://leetcode.cn',
        'graphql': 'https://leetcode.cn/graphql/',
        'domain': '.leetcode.cn'
    }
}


class LeetCodeClient:
    def __init__(self, site: str = None):
        """
        Initialize LeetCode client.

        Args:
            site: 'us' for leetcode.com, 'cn' for leetcode.cn (力扣)
                  If not specified, loads from config or defaults to 'us'
        """
        config = load_config()
        self.site = site or config.get('site', 'us')
        self.site_config = SITES.get(self.site, SITES['us'])

        self.session = requests.Session()

        # Configure proxy - explicitly disable system proxy if none configured
        proxy = config.get('proxy', '')
        if proxy:
            self.session.proxies = {
                'http': proxy,
                'https': proxy
            }
        else:
            # Bypass system proxy
            self.session.trust_env = False

        self._setup_headers()
        self._load_saved_session()

    def _setup_headers(self):
        """Setup request headers for the current site."""
        self.session.headers.update({
            'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
            'Accept': 'application/json, text/plain, */*',
            'Accept-Language': 'en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7',
            'Content-Type': 'application/json',
            'Origin': self.site_config['base'],
            'Referer': self.site_config['base'] + '/'
        })

    def switch_site(self, site: str):
        """Switch between US and China LeetCode."""
        if site not in SITES:
            raise ValueError(f"Invalid site: {site}. Must be 'us' or 'cn'")

        self.site = site
        self.site_config = SITES[site]
        self._setup_headers()

        # Save site preference
        config = load_config()
        config['site'] = site
        save_config(config)

        # Reload session for new site
        self.session.cookies.clear()
        self._load_saved_session()

    def get_current_site(self) -> Dict[str, str]:
        """Get current site information."""
        return {
            'code': self.site,
            'name': self.site_config['name'],
            'url': self.site_config['base']
        }

    def _load_saved_session(self):
        """Load saved session cookies if available."""
        session_data = load_session()
        if session_data:
            # Load cookies for current site
            site_cookies = session_data.get(f'cookies_{self.site}', {})
            domain = self.site_config['domain']

            # Set ALL cookies with proper domain
            for name, value in site_cookies.items():
                if value:
                    self.session.cookies.set(name, value, domain=domain)

            # Set CSRF token header
            csrf = site_cookies.get('csrftoken', '')
            if csrf:
                self.session.headers['x-csrftoken'] = csrf

    def _save_current_session(self):
        """Save current session cookies."""
        cookies_dict = {
            'LEETCODE_SESSION': self.session.cookies.get('LEETCODE_SESSION', ''),
            'csrftoken': self.session.cookies.get('csrftoken', '')
        }

        # Load existing session data
        session_data = load_session() or {}
        # Save cookies for current site
        session_data[f'cookies_{self.site}'] = cookies_dict
        save_session(session_data)

    def login(self, leetcode_session: str, csrf_token: str) -> bool:
        """
        Login using LeetCode session cookie and CSRF token.

        To get these values:
        1. Log into leetcode.com (or leetcode.cn for China) in your browser
        2. Open Developer Tools (F12) -> Application -> Cookies
        3. Copy the values of 'LEETCODE_SESSION' and 'csrftoken'
        """
        domain = self.site_config['domain']
        self.session.cookies.set('LEETCODE_SESSION', leetcode_session, domain=domain)
        self.session.cookies.set('csrftoken', csrf_token, domain=domain)
        self.session.headers['x-csrftoken'] = csrf_token

        # Verify login by checking user status
        if self.is_logged_in():
            self._save_current_session()
            return True
        return False

    def logout(self):
        """Clear session and logout from current site."""
        session_data = load_session() or {}
        # Only clear cookies for current site
        if f'cookies_{self.site}' in session_data:
            del session_data[f'cookies_{self.site}']
        save_session(session_data)
        self.session.cookies.clear()

    def is_logged_in(self) -> bool:
        """Check if user is logged in. Falls back to local session check if network unavailable."""
        try:
            query = """
            query globalData {
                userStatus {
                    isSignedIn
                    username
                }
            }
            """
            response = self._graphql_request(query)
            if response and 'data' in response:
                user_status = response['data'].get('userStatus', {})
                return user_status.get('isSignedIn', False)
        except Exception:
            pass
        # Fallback: check if we have a saved session locally
        return self._has_local_session()

    def _has_local_session(self) -> bool:
        """Check if a session cookie exists locally (no network needed)."""
        session_data = load_session()
        if session_data:
            site_cookies = session_data.get(f'cookies_{self.site}', {})
            return bool(site_cookies.get('LEETCODE_SESSION'))
        return False

    def get_username(self) -> Optional[str]:
        """Get current logged in username. Falls back to local cache if network unavailable."""
        try:
            query = """
            query globalData {
                userStatus {
                    username
                }
            }
            """
            response = self._graphql_request(query)
            if response and 'data' in response:
                return response['data'].get('userStatus', {}).get('username')
        except Exception:
            pass
        # Fallback: return cached username or site indicator
        if self._has_local_session():
            return f"(offline-{self.site})"
        return None

    def _graphql_request(self, query: str, variables: Dict = None, max_retries: int = 3) -> Optional[Dict]:
        """Make a GraphQL request to LeetCode with retry logic."""
        payload = {
            'query': query,
            'variables': variables or {}
        }

        # Update referer for problemset requests
        self.session.headers['Referer'] = self.site_config['base'] + '/problemset/'

        delay = 1.0
        for attempt in range(max_retries):
            try:
                response = self.session.post(
                    self.site_config['graphql'],
                    json=payload,
                    timeout=30
                )

                if response.status_code == 400:
                    try:
                        error_data = response.json()
                        print(f"GraphQL Error: {error_data}")
                    except json.JSONDecodeError:
                        print(f"Request failed with 400: {response.text[:200]}")
                    return None

                if response.status_code == 429:
                    # Rate limited - always retry
                    if attempt < max_retries - 1:
                        print(f"Rate limited, retrying in {delay}s...")
                        time.sleep(delay)
                        delay *= 2
                        continue
                    print("Rate limited, max retries reached")
                    return None

                response.raise_for_status()
                return response.json()
            except requests.exceptions.RequestException as e:
                if attempt < max_retries - 1:
                    time.sleep(delay)
                    delay *= 2
                else:
                    print(f"Request failed: {e}")
                    return None
        return None

    def get_problem_list(self, limit: int = 50, skip: int = 0,
                         difficulty: str = None, tags: List[str] = None) -> Optional[Dict]:
        """Get list of problems with optional filtering."""
        # Use site-specific query
        if self.site == 'cn':
            return self._get_problem_list_cn(limit, skip, difficulty, tags)
        else:
            return self._get_problem_list_us(limit, skip, difficulty, tags)

    def _get_problem_list_us(self, limit: int, skip: int,
                              difficulty: str, tags: List[str]) -> Optional[Dict]:
        """Get problem list for US site."""
        query = """
        query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {
            problemsetQuestionList: questionList(
                categorySlug: $categorySlug
                limit: $limit
                skip: $skip
                filters: $filters
            ) {
                total: totalNum
                questions: data {
                    acRate
                    difficulty
                    frontendQuestionId: questionFrontendId
                    paidOnly: isPaidOnly
                    title
                    titleSlug
                    status
                }
            }
        }
        """

        filters = {}
        if difficulty:
            filters['difficulty'] = difficulty.upper()
        if tags:
            filters['tags'] = tags

        variables = {
            'categorySlug': '',
            'limit': limit,
            'skip': skip,
            'filters': filters if filters else {}
        }

        return self._graphql_request(query, variables)

    def _get_problem_list_cn(self, limit: int, skip: int,
                              difficulty: str, tags: List[str]) -> Optional[Dict]:
        """Get problem list for CN site."""
        query = """
        query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {
            problemsetQuestionList(
                categorySlug: $categorySlug
                limit: $limit
                skip: $skip
                filters: $filters
            ) {
                hasMore
                total
                questions {
                    acRate
                    difficulty
                    frontendQuestionId
                    paidOnly
                    title
                    titleSlug
                    status
                }
            }
        }
        """

        filters = {}
        if difficulty:
            filters['difficulty'] = difficulty.upper()
        if tags:
            filters['tags'] = tags

        variables = {
            'categorySlug': '',
            'limit': limit,
            'skip': skip,
            'filters': filters if filters else {}
        }

        result = self._graphql_request(query, variables)

        # Normalize the response format to match US format
        if result and 'data' in result:
            pql = result['data'].get('problemsetQuestionList', {})
            if pql:
                questions = pql.get('questions', [])
                for q in questions:
                    if 'frontendQuestionId' in q:
                        q['frontendQuestionId'] = str(q['frontendQuestionId'])

                result['data']['problemsetQuestionList'] = {
                    'total': pql.get('total', 0),
                    'questions': questions
                }

        return result

    def get_problem(self, title_slug: str) -> Optional[Dict]:
        """Get detailed problem information by title slug."""
        if self.site == 'cn':
            return self._get_problem_cn(title_slug)
        else:
            return self._get_problem_us(title_slug)

    def _get_problem_us(self, title_slug: str) -> Optional[Dict]:
        """Get problem for US site."""
        query = """
        query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
                questionId
                questionFrontendId
                title
                titleSlug
                content
                difficulty
                likes
                dislikes
                topicTags {
                    name
                    slug
                }
                codeSnippets {
                    lang
                    langSlug
                    code
                }
                sampleTestCase
                exampleTestcases
                hints
                stats
                status
            }
        }
        """

        variables = {'titleSlug': title_slug}
        response = self._graphql_request(query, variables)

        if response and 'data' in response:
            return response['data'].get('question')
        return None

    def _get_problem_cn(self, title_slug: str) -> Optional[Dict]:
        """Get problem for CN site."""
        query = """
        query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
                questionId
                questionFrontendId
                title
                translatedTitle
                titleSlug
                content
                translatedContent
                difficulty
                topicTags {
                    name
                    slug
                }
                codeSnippets {
                    lang
                    langSlug
                    code
                }
                sampleTestCase
                exampleTestcases
                hints
                status
            }
        }
        """

        variables = {'titleSlug': title_slug}
        response = self._graphql_request(query, variables)

        if response and 'data' in response:
            q = response['data'].get('question')
            if q:
                # CN API: title=English, translatedTitle=Chinese
                #         content=English HTML, translatedContent=Chinese HTML
                q['title_en'] = q.get('title', '')
                q['title_cn'] = q.get('translatedTitle', '') or q.get('title', '')
                q['content_en'] = q.get('content', '')
                q['content_cn'] = q.get('translatedContent', '') or q.get('content', '')
                # Default display uses English
                q['title'] = q['title_en']
                q['content'] = q['content_en']
            return q
        return None

    def get_problem_by_id(self, problem_id: int) -> Optional[Dict]:
        """Get problem by frontend ID (the number you see on LeetCode)."""
        # Use search to find the problem slug efficiently instead of fetching all 3000+ problems
        results = self.search_problems(str(problem_id))
        if results:
            for q in results:
                if str(q.get('frontendQuestionId')) == str(problem_id):
                    return self.get_problem(q['titleSlug'])

        # Fallback: try a small batch fetch in case search didn't find it
        problems = self.get_problem_list(limit=100, skip=max(0, problem_id - 50))
        if problems and 'data' in problems:
            questions = problems['data'].get('problemsetQuestionList', {}).get('questions', [])
            for q in questions:
                if str(q.get('frontendQuestionId')) == str(problem_id):
                    return self.get_problem(q['titleSlug'])

        return None

    def submit_solution(self, title_slug: str, code: str, lang: str = 'cpp') -> Optional[Dict]:
        """Submit a solution for a problem."""
        # First get the question ID
        problem = self.get_problem(title_slug)
        if not problem:
            print(f"Problem '{title_slug}' not found")
            return None

        question_id = problem['questionId']
        base_url = self.site_config['base']

        submit_url = f"{base_url}/problems/{title_slug}/submit/"
        payload = {
            'question_id': question_id,
            'lang': lang,
            'typed_code': code
        }

        try:
            response = self.session.post(submit_url, json=payload)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Submit failed: {e}")
            return None

    def check_submission(self, submission_id: int) -> Optional[Dict]:
        """Check the status of a submission."""
        base_url = self.site_config['base']
        check_url = f"{base_url}/submissions/detail/{submission_id}/check/"

        try:
            response = self.session.get(check_url)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Check submission failed: {e}")
            return None

    def run_code(self, title_slug: str, code: str, test_input: str, lang: str = 'cpp') -> Optional[Dict]:
        """Run code against test cases without submitting."""
        problem = self.get_problem(title_slug)
        if not problem:
            print(f"Problem '{title_slug}' not found")
            return None

        question_id = problem['questionId']
        base_url = self.site_config['base']

        run_url = f"{base_url}/problems/{title_slug}/interpret_solution/"
        payload = {
            'question_id': question_id,
            'lang': lang,
            'typed_code': code,
            'data_input': test_input
        }

        try:
            response = self.session.post(run_url, json=payload)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Run code failed: {e}")
            return None

    def search_problems(self, keyword: str) -> Optional[List[Dict]]:
        """Search problems by keyword."""
        if self.site == 'cn':
            query = """
            query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {
                problemsetQuestionList(
                    categorySlug: $categorySlug
                    limit: $limit
                    skip: $skip
                    filters: $filters
                ) {
                    questions {
                        frontendQuestionId
                        title
                        titleSlug
                        difficulty
                        status
                    }
                }
            }
            """
        else:
            query = """
            query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {
                problemsetQuestionList: questionList(
                    categorySlug: $categorySlug
                    limit: $limit
                    skip: $skip
                    filters: $filters
                ) {
                    questions: data {
                        frontendQuestionId: questionFrontendId
                        title
                        titleSlug
                        difficulty
                        status
                    }
                }
            }
            """

        variables = {
            'categorySlug': '',
            'limit': 50,
            'skip': 0,
            'filters': {'searchKeywords': keyword}
        }

        response = self._graphql_request(query, variables)
        if response and 'data' in response:
            questions = response['data'].get('problemsetQuestionList', {}).get('questions', [])
            # Normalize CN response
            if self.site == 'cn':
                for q in questions:
                    q['frontendQuestionId'] = str(q.get('frontendQuestionId', ''))
            return questions
        return None

    def get_favorites(self) -> Optional[List[Dict]]:
        """Get user's favorite problem lists (收藏夹)."""
        # CN and US use similar schema for this
        query = """
        query userFavorites {
            favoritesLists {
                allFavorites {
                    idHash
                    name
                    isPublicFavorite
                }
            }
        }
        """

        response = self._graphql_request(query)
        if response and 'data' in response:
            favorites = response['data'].get('favoritesLists', {})
            return favorites.get('allFavorites', [])
        return None

    def get_favorite_problems(self, favorite_slug: str) -> Optional[List[Dict]]:
        """Get problems in a specific favorite list."""
        query = """
        query favoriteQuestionList($favoriteSlug: String!, $skip: Int, $limit: Int) {
            favoriteQuestionList(favoriteSlug: $favoriteSlug, skip: $skip, limit: $limit) {
                questions {
                    questionFrontendId
                    title
                    titleSlug
                    difficulty
                    status
                }
                totalLength
            }
        }
        """

        variables = {
            'favoriteSlug': favorite_slug,
            'skip': 0,
            'limit': 1000
        }

        response = self._graphql_request(query, variables)
        if response and 'data' in response:
            data = response['data'].get('favoriteQuestionList', {})
            questions = data.get('questions', [])
            # Normalize response
            for q in questions:
                q['frontendQuestionId'] = str(q.get('questionFrontendId', ''))
                # Normalize status
                if q.get('status') == 'SOLVED':
                    q['status'] = 'ac'
                elif q.get('status') == 'TO_DO':
                    q['status'] = 'notac'
            return questions
        return None

    def get_user_lists(self) -> Optional[List[Dict]]:
        """Get user's custom problem lists (个人题库/题单)."""
        query = """
        query myLists {
            myCreatedFavoriteList {
                hasMore
                favorites {
                    name
                    slug
                    isPublicFavorite
                }
            }
        }
        """

        response = self._graphql_request(query)
        if response and 'data' in response:
            data = response['data'].get('myCreatedFavoriteList', {})
            return data.get('favorites', [])
        return None

    # ============ Mutation APIs (Create / Modify) ============

    def create_favorite_list(self, name: str, is_public: bool = False) -> Optional[str]:
        """Create a new favorite list on LeetCode. Returns the slug/idHash of the created list."""
        query = """
        mutation createFavorite($name: String!, $isPublicFavorite: Boolean!) {
            createFavorite(name: $name, isPublicFavorite: $isPublicFavorite) {
                ok
                error
                favoriteIdHash
                name
            }
        }
        """

        variables = {
            'name': name,
            'isPublicFavorite': is_public
        }

        response = self._graphql_request(query, variables)
        if response and 'data' in response:
            data = response['data'].get('createFavorite', {})
            if data.get('ok'):
                return data.get('favoriteIdHash')
            else:
                print(f"Create favorite failed: {data.get('error', 'Unknown error')}")
        return None

    def add_question_to_favorite(self, favorite_id_hash: str, question_id: str) -> bool:
        """Add a question to a favorite list by question ID (internal questionId, not frontend ID)."""
        query = """
        mutation addQuestionToFavorite($favoriteIdHash: String!, $questionId: String!) {
            addQuestionToFavorite(favoriteIdHash: $favoriteIdHash, questionId: $questionId) {
                ok
                error
            }
        }
        """

        variables = {
            'favoriteIdHash': favorite_id_hash,
            'questionId': question_id
        }

        response = self._graphql_request(query, variables)
        if response and 'data' in response:
            data = response['data'].get('addQuestionToFavorite', {})
            return data.get('ok', False)
        return False

    def get_question_id_by_frontend_id(self, frontend_id: int) -> Optional[str]:
        """Get the internal questionId from a frontend question ID (e.g., 1 -> "1")."""
        # Method 1: Search by keyword
        results = self.search_problems(str(frontend_id))
        if results:
            for q in results:
                if str(q.get('frontendQuestionId')) == str(frontend_id):
                    problem = self.get_problem(q['titleSlug'])
                    if problem:
                        return problem.get('questionId')

        # Method 2: Search by title slug from problem list (handles short numbers like "1")
        # Fetch a page of problems around the target ID
        skip = max(0, frontend_id - 5)
        result = self.get_problem_list(limit=20, skip=skip)
        if result and 'data' in result:
            questions = result['data'].get('problemsetQuestionList', {}).get('questions', [])
            for q in questions:
                if str(q.get('frontendQuestionId')) == str(frontend_id):
                    slug = q.get('titleSlug')
                    if slug:
                        problem = self.get_problem(slug)
                        if problem:
                            return problem.get('questionId')

        return None

    def batch_add_questions_to_favorite(self, favorite_id_hash: str, frontend_ids: List[int],
                                         progress_callback=None) -> Dict[str, List]:
        """Add multiple questions to a favorite list by their frontend IDs.
        Returns dict with 'success' and 'failed' lists."""
        results = {'success': [], 'failed': []}

        for i, fid in enumerate(frontend_ids):
            if progress_callback:
                progress_callback(i + 1, len(frontend_ids), fid)

            # Get internal question ID
            qid = self.get_question_id_by_frontend_id(fid)
            if not qid:
                results['failed'].append({'id': fid, 'reason': 'not found'})
                continue

            # Add to favorite
            success = self.add_question_to_favorite(favorite_id_hash, qid)
            if success:
                results['success'].append(fid)
            else:
                results['failed'].append({'id': fid, 'reason': 'add failed'})

            # Small delay to avoid rate limiting
            time.sleep(0.3)

        return results
