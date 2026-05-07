import * as vscode from 'vscode';
import * as cp from 'child_process';
import * as path from 'path';
import * as os from 'os';

export interface Problem {
    id: string;
    title: string;
    titleSlug: string;
    difficulty: string;
    acRate?: number;
    status?: string;
    paidOnly?: boolean;
    content?: string;
    codeSnippet?: string;
    title_cn?: string;
    title_en?: string;
    content_cn?: string;
    content_en?: string;
}

export interface FetchResult {
    success: boolean;
    filePath?: string;
    title?: string;
    error?: string;
}

export class LeetCodeService {
    private context: vscode.ExtensionContext;
    private pythonPath: string;
    private currentSite: string = 'us';
    private isLoggedIn: boolean = false;
    private username: string = '';

    constructor(context: vscode.ExtensionContext) {
        this.context = context;
        this.pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
        this.currentSite = vscode.workspace.getConfiguration('leetcode').get('site', 'us');
        this.syncProxyConfig();
        this.checkStatus();
    }

    private async syncProxyConfig(): Promise<void> {
        const proxy = vscode.workspace.getConfiguration('leetcode').get('proxy', '');
        // Update the Python config file with proxy setting
        const args = ['config', '--proxy', proxy || ''];
        await this.runCommand(args);
    }

    getToolPath(): string {
        // Python scripts bundled inside the extension
        return path.join(this.context.extensionPath, 'python', 'leetcode_tool.py');
    }

    getProblemsDir(): string {
        // Priority: 1. VS Code setting  2. Relative to extension  3. ~/code-coach
        const configured = vscode.workspace.getConfiguration('leetcode').get<string>('problemsDir');
        if (configured) return configured;
        // Relative to extension: extensionPath/../problems/
        const relative = path.join(this.context.extensionPath, '..', '..', 'problems');
        const fs = require('fs');
        if (fs.existsSync(relative)) return relative;
        // Fallback for standalone installs
        return path.join(os.homedir(), 'code-coach');
    }

    private async runCommand(args: string[]): Promise<{ stdout: string; stderr: string; code: number }> {
        return new Promise((resolve) => {
            const toolPath = this.getToolPath();
            const problemsDir = this.getProblemsDir();
            const fullArgs = [toolPath, '--problems-dir', problemsDir, ...args];

            // Check if tool exists
            const fs = require('fs');
            if (!fs.existsSync(toolPath)) {
                resolve({
                    stdout: '',
                    stderr: `leetcode_tool.py not found at: ${toolPath}. Extension may be corrupted - try reinstalling.`,
                    code: 1
                });
                return;
            }

            cp.execFile(this.pythonPath, fullArgs, {
                cwd: path.dirname(toolPath),
                maxBuffer: 1024 * 1024 * 10
            }, (error, stdout, stderr) => {
                resolve({
                    stdout: stdout || '',
                    stderr: stderr || '',
                    code: error ? (error as any).code || 1 : 0
                });
            });
        });
    }

    async checkStatus(): Promise<void> {
        try {
            const result = await this.runCommand(['status']);
            this.isLoggedIn = result.stdout.includes('Logged in as:');
            const match = result.stdout.match(/Logged in as: ([\w-]+)/);
            if (match) {
                this.username = match[1];
            }

            const siteMatch = result.stdout.match(/Site: (.*)/);
            if (siteMatch) {
                this.currentSite = siteMatch[1].includes('China') ? 'cn' : 'us';
            }
        } catch (e) {
            this.isLoggedIn = false;
        }
    }

    async switchSite(site: string): Promise<void> {
        await this.runCommand(['site', site]);
        this.currentSite = site;
        await vscode.workspace.getConfiguration('leetcode').update('site', site, true);
        await this.checkStatus();
    }

    async login(session: string, csrf: string): Promise<boolean> {
        // Sync proxy settings first
        await this.syncProxyConfig();

        // Manual cookie login uses --cookie flag
        const result = await this.runCommandWithInput(['login', '--cookie'], `${session}\n${csrf}\n`);
        const success = result.stdout.includes('Successfully logged in');
        if (success) {
            await this.checkStatus();
        }
        return success;
    }

    async browserLogin(): Promise<{ success: boolean; error?: string }> {
        // Sync proxy settings first
        await this.syncProxyConfig();

        // 'login' now defaults to Playwright browser login
        const result = await this.runCommand(['login']);

        // Debug: log output
        console.log('Browser login stdout:', result.stdout);
        console.log('Browser login stderr:', result.stderr);

        if (result.stdout.includes('Login successful')) {
            await this.checkStatus();
            return { success: true };
        }

        // Check for specific errors
        if (result.stdout.includes('playwright is not installed') || result.stderr.includes('playwright')) {
            return { success: false, error: 'Playwright not installed. Run: pip install playwright && python3 -m playwright install chromium' };
        }

        if (result.stdout.includes('timed out')) {
            return { success: false, error: 'Login timed out. Please try again.' };
        }

        if (result.stdout.includes('Could not capture')) {
            return { success: false, error: 'Could not capture cookies. Please try again or use manual cookie login.' };
        }

        if (result.stdout.includes('Browser error')) {
            return { success: false, error: 'Browser error. Make sure playwright is installed: python3 -m playwright install chromium' };
        }

        // Return detailed error
        const errorDetail = result.stderr || result.stdout || 'Unknown error';
        return { success: false, error: `Login failed: ${errorDetail.substring(0, 200)}` };
    }

    private async runCommandWithInput(args: string[], input: string): Promise<{ stdout: string; stderr: string }> {
        return new Promise((resolve) => {
            const toolPath = this.getToolPath();
            const problemsDir = this.getProblemsDir();
            const fullArgs = [toolPath, '--problems-dir', problemsDir, ...args];

            const proc = cp.spawn(this.pythonPath, fullArgs, {
                cwd: path.dirname(toolPath)
            });

            let stdout = '';
            let stderr = '';

            proc.stdout.on('data', (data) => {
                stdout += data.toString();
            });

            proc.stderr.on('data', (data) => {
                stderr += data.toString();
            });

            proc.on('close', () => {
                resolve({ stdout, stderr });
            });

            proc.stdin.write(input);
            proc.stdin.end();
        });
    }

    async logout(): Promise<void> {
        await this.runCommand(['logout']);
        this.isLoggedIn = false;
        this.username = '';
    }

    // Check if a problem already exists in the local problems directory
    findExistingProblem(idOrSlug: string): string | null {
        const fs = require('fs');
        const problemsDir = this.getProblemsDir();

        if (!fs.existsSync(problemsDir)) {
            return null;
        }

        try {
            const dirs = fs.readdirSync(problemsDir);
            for (const dir of dirs) {
                // Check if directory name starts with the problem ID
                if (idOrSlug.match(/^\d+$/)) {
                    // Input is a number (problem ID) - match with or without zero-padding
                    const paddedId = idOrSlug.padStart(4, '0');
                    if (dir.startsWith(idOrSlug + '_') || dir.startsWith(idOrSlug + '-') ||
                        dir.startsWith(paddedId + '_') || dir.startsWith(paddedId + '-')) {
                        const solutionFile = path.join(problemsDir, dir, 'solution.cpp');
                        if (fs.existsSync(solutionFile)) {
                            return solutionFile;
                        }
                    }
                } else {
                    // Input is a slug - check problem_info.json
                    const infoFile = path.join(problemsDir, dir, 'problem_info.json');
                    if (fs.existsSync(infoFile)) {
                        try {
                            const info = JSON.parse(fs.readFileSync(infoFile, 'utf8'));
                            if (info.titleSlug === idOrSlug) {
                                const solutionFile = path.join(problemsDir, dir, 'solution.cpp');
                                if (fs.existsSync(solutionFile)) {
                                    return solutionFile;
                                }
                            }
                        } catch (e) {
                            // Ignore parse errors
                        }
                    }
                }
            }
        } catch (e) {
            console.error('Error searching for existing problem:', e);
        }

        return null;
    }

    async fetchProblem(idOrSlug: string): Promise<FetchResult> {
        // Check if problem already exists locally
        const existingFile = this.findExistingProblem(idOrSlug);
        if (existingFile) {
            console.log(`[LeetCode] Problem ${idOrSlug} already exists: ${existingFile}`);
            // Read title from problem_info.json
            const fs = require('fs');
            const infoPath = path.join(path.dirname(existingFile), 'problem_info.json');
            let title = 'Unknown';
            try {
                if (fs.existsSync(infoPath)) {
                    const info = JSON.parse(fs.readFileSync(infoPath, 'utf8'));
                    title = info.title || 'Unknown';
                }
            } catch (e) {
                // Ignore
            }
            return {
                success: true,
                filePath: existingFile,
                title: title
            };
        }

        const result = await this.runCommand(['fetch', idOrSlug]);

        if (result.stdout.includes('not found')) {
            return { success: false, error: 'Problem not found' };
        }

        // Parse the output to get the file path
        const fileMatch = result.stdout.match(/Solution file: (.+\.cpp)/);
        const titleMatch = result.stdout.match(/Problem \d+: (.+)/);

        if (fileMatch) {
            const filePath = fileMatch[1].trim();
            // Remove ANSI color codes
            const cleanPath = filePath.replace(/\x1b\[[0-9;]*m/g, '');
            const fullPath = path.isAbsolute(cleanPath) ? cleanPath : path.join(this.getProblemsDir(), cleanPath);
            // Validate path is within problems directory (prevent path traversal)
            const resolvedPath = path.resolve(fullPath);
            const resolvedBase = path.resolve(this.getProblemsDir());
            if (!resolvedPath.startsWith(resolvedBase + path.sep) && resolvedPath !== resolvedBase) {
                return { success: false, error: 'Invalid file path returned' };
            }
            return {
                success: true,
                filePath: fullPath,
                title: titleMatch ? titleMatch[1].replace(/\x1b\[[0-9;]*m/g, '') : undefined
            };
        }

        return { success: false, error: result.stderr || 'Unknown error' };
    }

    async getProblems(limit: number = 50, skip: number = 0, difficulty?: string): Promise<Problem[]> {
        const args = ['list', '-n', limit.toString(), '-s', skip.toString()];
        if (difficulty) {
            args.push('-d', difficulty);
        }

        const result = await this.runCommand(args);
        return this.parseProblemsOutput(result.stdout);
    }

    async searchProblems(keyword: string): Promise<Problem[]> {
        const result = await this.runCommand(['search', keyword]);
        return this.parseProblemsOutput(result.stdout);
    }

    async getProblemDetail(slug: string): Promise<Problem | null> {
        // For now, we use fetch which gives us the problem details
        const result = await this.runCommand(['fetch', slug]);

        if (result.stdout.includes('not found')) {
            return null;
        }

        // Parse basic info from output
        const idMatch = result.stdout.match(/Problem (\d+):/);
        const titleMatch = result.stdout.match(/Problem \d+: (.+)/);
        const diffMatch = result.stdout.match(/Difficulty: (\w+)/);

        if (idMatch && titleMatch) {
            return {
                id: idMatch[1],
                title: titleMatch[1].replace(/\x1b\[[0-9;]*m/g, '').trim(),
                titleSlug: slug,
                difficulty: diffMatch ? diffMatch[1].replace(/\x1b\[[0-9;]*m/g, '') : 'Unknown'
            };
        }

        return null;
    }

    async getProblemDetailById(idOrSlug: string): Promise<Problem | null> {
        // Fetch problem by ID (works for both CN and US sites)
        const result = await this.runCommand(['fetch', idOrSlug]);

        if (result.stdout.includes('not found')) {
            return null;
        }

        // Parse basic info from output
        const idMatch = result.stdout.match(/Problem (\d+):/);
        const titleMatch = result.stdout.match(/Problem \d+: (.+)/);
        const diffMatch = result.stdout.match(/Difficulty: (\w+)/);
        const fileMatch = result.stdout.match(/Solution file: (.+\.cpp)/);

        if (idMatch && titleMatch) {
            // Try to read problem content from problem_info.json
            let content = '';
            let contentCn = '';
            let contentEn = '';
            let titleCn = '';
            let titleEn = '';
            if (fileMatch) {
                const filePath = fileMatch[1].trim().replace(/\x1b\[[0-9;]*m/g, '');
                const fullPath = path.isAbsolute(filePath) ? filePath : path.join(this.getProblemsDir(), filePath);
                const infoPath = path.join(path.dirname(fullPath), 'problem_info.json');
                try {
                    const fs = require('fs');
                    if (fs.existsSync(infoPath)) {
                        const info = JSON.parse(fs.readFileSync(infoPath, 'utf8'));
                        content = info.content || '';
                        contentCn = info.content_cn || '';
                        contentEn = info.content_en || '';
                        titleCn = info.title_cn || '';
                        titleEn = info.title_en || '';
                    }
                } catch (e) {
                    // Ignore errors reading problem_info.json
                }
            }

            return {
                id: idMatch[1],
                title: titleMatch[1].replace(/\x1b\[[0-9;]*m/g, '').trim(),
                titleSlug: idOrSlug,
                difficulty: diffMatch ? diffMatch[1].replace(/\x1b\[[0-9;]*m/g, '') : 'Unknown',
                content: content,
                title_cn: titleCn,
                title_en: titleEn,
                content_cn: contentCn,
                content_en: contentEn,
            };
        }

        return null;
    }

    async getProblemFromFile(filePath: string): Promise<Problem | null> {
        // Read problem details directly from problem_info.json
        const fs = require('fs');
        const infoPath = path.join(path.dirname(filePath), 'problem_info.json');

        try {
            if (fs.existsSync(infoPath)) {
                const info = JSON.parse(fs.readFileSync(infoPath, 'utf8'));
                return {
                    id: info.id || '',
                    title: info.title || '',
                    titleSlug: info.titleSlug || '',
                    difficulty: info.difficulty || 'Unknown',
                    content: info.content || '',
                    title_cn: info.title_cn || '',
                    title_en: info.title_en || '',
                    content_cn: info.content_cn || '',
                    content_en: info.content_en || '',
                };
            }
        } catch (e) {
            console.error('Error reading problem_info.json:', e);
        }

        return null;
    }

    private parseProblemsOutput(output: string): Problem[] {
        const problems: Problem[] = [];
        const lines = output.split('\n');

        for (const line of lines) {
            // Skip header and separator lines
            if (line.startsWith('ID') || line.startsWith('-') || line.startsWith('Fetching') ||
                line.startsWith('Showing') || line.startsWith('Searching') || !line.trim()) {
                continue;
            }

            // Remove ANSI codes first
            const cleanLine = line.replace(/\x1b\[[0-9;]*m/g, '');

            // New format: ID  Slug  status+Title  Difficulty  AC Rate
            // Example: 1      two-sum                                       ✓ Two Sum                                EASY         50.0%
            const match = cleanLine.match(/^(\d+)\s+(\S+)\s+([✓○🔒]?\s*)(.+?)\s+(Easy|Medium|Hard|EASY|MEDIUM|HARD)\s*(\d+\.\d+%)?/i);

            if (match) {
                const status = match[3].includes('✓') ? 'ac' : match[3].includes('○') ? 'notac' : '';
                const paidOnly = match[3].includes('🔒') || match[4].includes('🔒');

                problems.push({
                    id: match[1],
                    title: match[4].replace('🔒', '').trim(),
                    titleSlug: match[2].trim(),  // Use actual slug from output
                    difficulty: match[5],
                    acRate: match[6] ? parseFloat(match[6]) : undefined,
                    status: status,
                    paidOnly: paidOnly
                });
            } else {
                // Debug: log lines that didn't match
                console.log('[LeetCode] Failed to parse problem line:', cleanLine);
            }
        }

        return problems;
    }

    private titleToSlug(title: string): string {
        return title.toLowerCase()
            .replace(/[^a-z0-9\s-]/g, '')
            .replace(/\s+/g, '-')
            .replace(/-+/g, '-')
            .trim();
    }

    getCurrentSite(): string {
        return this.currentSite;
    }

    async setDisplayLanguage(lang: string): Promise<void> {
        await this.runCommand(['config', '--lang', lang]);
        // Clear cached problems so they reload with new language
        this.clearAllCache();
    }

    getSiteName(): string {
        return this.currentSite === 'cn' ? 'LeetCode China (力扣)' : 'LeetCode US';
    }

    getIsLoggedIn(): boolean {
        return this.isLoggedIn;
    }

    getUsername(): string {
        return this.username;
    }

    async getFavorites(): Promise<{id: string, name: string}[]> {
        const result = await this.runCommand(['favorites']);
        const favorites: {id: string, name: string}[] = [];
        const lines = result.stdout.split('\n');

        for (const line of lines) {
            // Skip header lines
            if (line.startsWith('ID') || line.startsWith('-') || line.startsWith('Fetching') || !line.trim()) {
                continue;
            }
            // Parse: ID (15 chars) Name (40 chars) Public
            const match = line.match(/^(\S+)\s+(.+?)\s+(Yes|No)\s*$/);
            if (match) {
                favorites.push({
                    id: match[1].trim(),
                    name: match[2].trim()
                });
            }
        }
        return favorites;
    }

    async getFavoriteProblems(favoriteId: string): Promise<Problem[]> {
        const result = await this.runCommand(['favorite', favoriteId]);
        return this.parseFavoriteProblemsOutput(result.stdout);
    }

    private parseFavoriteProblemsOutput(output: string): Problem[] {
        const problems: Problem[] = [];
        const lines = output.split('\n');

        for (const line of lines) {
            // Skip header and info lines
            if (line.startsWith('ID') || line.startsWith('-') || line.startsWith('Fetching') ||
                line.startsWith('Found') || !line.trim()) {
                continue;
            }

            // Remove ANSI codes
            const cleanLine = line.replace(/\x1b\[[0-9;]*m/g, '');

            // Format: ID (8 chars) Slug (50 chars) status+title (40 chars) difficulty
            // Example: 1        two-sum                                            ✓ Two Sum                                  EASY
            // Use a more flexible regex that captures: ID, slug, optional status, title, difficulty
            const match = cleanLine.match(/^(\d+)\s+(\S+)\s+([✓○]?\s*)(.+?)\s+(EASY|MEDIUM|HARD|Easy|Medium|Hard)\s*$/i);
            if (match) {
                const status = match[3].includes('✓') ? 'ac' : match[3].includes('○') ? 'notac' : '';
                problems.push({
                    id: match[1].trim(),
                    title: match[4].trim(),
                    titleSlug: match[2].trim(),  // Use actual slug from output
                    difficulty: match[5],
                    status: status
                });
            } else {
                // Debug: log lines that didn't match
                console.log('[LeetCode] Failed to parse favorite line:', cleanLine);
            }
        }
        return problems;
    }

    async addToFavorite(favoriteId: string, problemId: string): Promise<{success: boolean, error?: string}> {
        const result = await this.runCommand(['add-to-favorite', favoriteId, problemId]);
        const output = result.stdout + result.stderr;
        if (output.includes('Done') || output.includes('Added') || output.includes('success')) {
            return { success: true };
        }
        return { success: false, error: output.trim().split('\n')[0] || 'Unknown error' };
    }

    async createFavorite(name: string): Promise<{success: boolean, id?: string, error?: string}> {
        const result = await this.runCommand(['create-favorite', name]);
        const output = result.stdout;
        if (output.includes('Created')) {
            const idMatch = output.match(/ID:\s*(\S+)/);
            return { success: true, id: idMatch ? idMatch[1] : undefined };
        }
        return { success: false, error: (result.stderr || output).trim().split('\n')[0] || 'Unknown error' };
    }

    async getMyLists(): Promise<{id: string, name: string}[]> {
        const result = await this.runCommand(['my-lists']);
        const lists: {id: string, name: string}[] = [];
        const lines = result.stdout.split('\n');

        for (const line of lines) {
            // Skip header lines
            if (line.startsWith('ID') || line.startsWith('-') || line.startsWith('Fetching') || !line.trim()) {
                continue;
            }
            // Parse: ID (15 chars) Name (40 chars) Public
            const match = line.match(/^(\S+)\s+(.+?)\s+(Yes|No)\s*$/);
            if (match) {
                lists.push({
                    id: match[1].trim(),
                    name: match[2].trim()
                });
            }
        }
        return lists;
    }

    // ============ Cache Methods ============

    private getCacheDir(): string {
        return path.join(this.getProblemsDir(), '.cache');
    }

    private ensureCacheDir(): void {
        const fs = require('fs');
        const cacheDir = this.getCacheDir();
        if (!fs.existsSync(cacheDir)) {
            fs.mkdirSync(cacheDir, { recursive: true });
        }
    }

    saveProblemsCache(problems: Problem[]): void {
        const fs = require('fs');
        this.ensureCacheDir();
        const cachePath = path.join(this.getCacheDir(), `problems_${this.currentSite}.json`);
        const cacheData = {
            timestamp: Date.now(),
            site: this.currentSite,
            problems: problems
        };
        fs.writeFileSync(cachePath, JSON.stringify(cacheData, null, 2));
    }

    loadProblemsCache(): Problem[] | null {
        const fs = require('fs');
        const cachePath = path.join(this.getCacheDir(), `problems_${this.currentSite}.json`);
        try {
            if (fs.existsSync(cachePath)) {
                const data = JSON.parse(fs.readFileSync(cachePath, 'utf8'));
                if (data.site === this.currentSite && data.problems) {
                    return data.problems;
                }
            }
        } catch (e) {
            console.error('Error loading problems cache:', e);
        }
        return null;
    }

    saveFavoritesCache(favorites: {id: string, name: string}[]): void {
        const fs = require('fs');
        this.ensureCacheDir();
        const cachePath = path.join(this.getCacheDir(), `favorites_${this.currentSite}.json`);
        const cacheData = {
            timestamp: Date.now(),
            site: this.currentSite,
            favorites: favorites
        };
        fs.writeFileSync(cachePath, JSON.stringify(cacheData, null, 2));
    }

    loadFavoritesCache(): {id: string, name: string}[] | null {
        const fs = require('fs');
        const cachePath = path.join(this.getCacheDir(), `favorites_${this.currentSite}.json`);
        try {
            if (fs.existsSync(cachePath)) {
                const data = JSON.parse(fs.readFileSync(cachePath, 'utf8'));
                if (data.site === this.currentSite && data.favorites) {
                    return data.favorites;
                }
            }
        } catch (e) {
            console.error('Error loading favorites cache:', e);
        }
        return null;
    }

    saveFavoriteProblemsCache(favoriteId: string, problems: Problem[]): void {
        const fs = require('fs');
        this.ensureCacheDir();
        const cachePath = path.join(this.getCacheDir(), `favorite_${this.currentSite}_${favoriteId}.json`);
        const cacheData = {
            timestamp: Date.now(),
            site: this.currentSite,
            favoriteId: favoriteId,
            problems: problems
        };
        fs.writeFileSync(cachePath, JSON.stringify(cacheData, null, 2));
    }

    loadFavoriteProblemsCache(favoriteId: string): Problem[] | null {
        const fs = require('fs');
        const cachePath = path.join(this.getCacheDir(), `favorite_${this.currentSite}_${favoriteId}.json`);
        try {
            if (fs.existsSync(cachePath)) {
                const data = JSON.parse(fs.readFileSync(cachePath, 'utf8'));
                if (data.site === this.currentSite && data.problems) {
                    return data.problems;
                }
            }
        } catch (e) {
            console.error('Error loading favorite problems cache:', e);
        }
        return null;
    }

    clearAllCache(): void {
        const fs = require('fs');
        const cacheDir = this.getCacheDir();
        try {
            if (fs.existsSync(cacheDir)) {
                const files = fs.readdirSync(cacheDir);
                for (const file of files) {
                    fs.unlinkSync(path.join(cacheDir, file));
                }
            }
        } catch (e) {
            console.error('Error clearing cache:', e);
        }
    }

    // Pick a random untouched/attempted problem, optionally filtered by difficulty
    getRandomProblem(difficulty?: string): { id: string; title: string; difficulty: string; solutionPath: string } | null {
        const fs = require('fs');
        const problemsDir = this.getProblemsDir();

        if (!fs.existsSync(problemsDir)) {
            return null;
        }

        const candidates: { id: string; title: string; difficulty: string; solutionPath: string }[] = [];

        try {
            const dirs = fs.readdirSync(problemsDir, { withFileTypes: true })
                .filter((d: any) => d.isDirectory());

            for (const dir of dirs) {
                const solutionPath = path.join(problemsDir, dir.name, 'solution.cpp');
                const infoPath = path.join(problemsDir, dir.name, 'problem_info.json');

                if (!fs.existsSync(solutionPath) || !fs.existsSync(infoPath)) {
                    continue;
                }

                try {
                    const info = JSON.parse(fs.readFileSync(infoPath, 'utf-8'));
                    if (info.progress?.status === 'passed') {
                        continue;
                    }
                    if (difficulty && info.difficulty?.toLowerCase() !== difficulty.toLowerCase()) {
                        continue;
                    }
                    candidates.push({
                        id: info.id || '',
                        title: info.title || dir.name,
                        difficulty: info.difficulty || '',
                        solutionPath
                    });
                } catch {
                    continue;
                }
            }
        } catch {
            // ignore
        }

        if (candidates.length === 0) {
            return null;
        }

        return candidates[Math.floor(Math.random() * candidates.length)];
    }

    // Find the next untouched/attempted problem (by problem number order)
    getNextProblem(): { id: string; title: string; difficulty: string; solutionPath: string } | null {
        const fs = require('fs');
        const problemsDir = this.getProblemsDir();

        if (!fs.existsSync(problemsDir)) {
            return null;
        }

        // Follow My Favorite folder order (skip 0_High_Freq*)
        const folderOrder = [
            '1_Array', '2_LinkedList', '3_HashTable', '4_String',
            '5_TwoPointers', '6_Stack_Queue', '7_Binary_Tree',
            '8_Back_Tracking', '9_Greedy_Algorithm', '10_Dynamic_Program'
        ];

        const seen = new Set<string>();
        const allDirs = fs.readdirSync(problemsDir);

        try {
            const favoritesCache = this.loadFavoritesCache();
            if (favoritesCache && favoritesCache.length > 0) {
                for (const folderName of folderOrder) {
                    const folder = favoritesCache.find((f: any) => f.name === folderName);
                    if (!folder) { continue; }

                    const problems = this.loadFavoriteProblemsCache(folder.id);
                    if (!problems) { continue; }

                    for (const p of problems) {
                        if (seen.has(p.id)) { continue; }
                        seen.add(p.id);

                        const dirName = allDirs.find((d: string) => {
                            const num = d.split('_')[0].replace(/^0+/, '');
                            return num === p.id;
                        });
                        if (!dirName) { continue; }

                        const solutionPath = path.join(problemsDir, dirName, 'solution.cpp');
                        const infoPath = path.join(problemsDir, dirName, 'problem_info.json');
                        if (!fs.existsSync(solutionPath) || !fs.existsSync(infoPath)) { continue; }

                        try {
                            const info = JSON.parse(fs.readFileSync(infoPath, 'utf-8'));
                            if (info.progress?.status === 'passed') { continue; }
                            return {
                                id: info.id || p.id,
                                title: info.title || p.title,
                                difficulty: info.difficulty || p.difficulty,
                                solutionPath
                            };
                        } catch {
                            continue;
                        }
                    }
                }
            }
        } catch {
            // ignore
        }

        return null;
    }

    // Reset a solution to initial template for re-practice (重做)
    // Keeps: problem_info.json, reference.cpp, testcases.txt
    // Resets: solution.cpp back to empty template
    // Deletes: compiled binary
    async resetSolution(solutionPath: string): Promise<{ success: boolean; filePath?: string; error?: string }> {
        try {
            const result = await this.runCommand(['reset', solutionPath]);

            if (result.stdout.includes('Solution reset')) {
                // Parse the regenerated file path from output
                const fileMatch = result.stdout.match(/File: (.+\.cpp)/);
                let filePath = solutionPath;
                if (fileMatch) {
                    const cleanPath = fileMatch[1].trim().replace(/\x1b\[[0-9;]*m/g, '');
                    filePath = path.isAbsolute(cleanPath) ? cleanPath : path.join(this.getProblemsDir(), cleanPath);
                }
                return { success: true, filePath };
            }

            return { success: false, error: result.stderr || result.stdout || 'Unknown error' };
        } catch (e) {
            console.error('Error resetting solution:', e);
            return { success: false, error: String(e) };
        }
    }

    // Clear a single solution (delete the problem folder)
    clearSolution(solutionPath: string): boolean {
        const fs = require('fs');
        try {
            const problemDir = path.dirname(solutionPath);
            if (fs.existsSync(problemDir)) {
                // Remove all files in the directory
                const files = fs.readdirSync(problemDir);
                for (const file of files) {
                    fs.unlinkSync(path.join(problemDir, file));
                }
                // Remove the directory
                fs.rmdirSync(problemDir);
                return true;
            }
        } catch (e) {
            console.error('Error clearing solution:', e);
        }
        return false;
    }

    // Clear all solutions (delete the problems folder content)
    clearAllSolutions(): number {
        const fs = require('fs');
        const problemsDir = this.getProblemsDir();
        let count = 0;

        try {
            if (fs.existsSync(problemsDir)) {
                const dirs = fs.readdirSync(problemsDir, { withFileTypes: true })
                    .filter((d: any) => d.isDirectory());

                for (const dir of dirs) {
                    const dirPath = path.join(problemsDir, dir.name);
                    // Remove all files in the directory
                    const files = fs.readdirSync(dirPath);
                    for (const file of files) {
                        fs.unlinkSync(path.join(dirPath, file));
                    }
                    // Remove the directory
                    fs.rmdirSync(dirPath);
                    count++;
                }
            }
        } catch (e) {
            console.error('Error clearing all solutions:', e);
        }

        return count;
    }
}
