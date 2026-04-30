import * as vscode from 'vscode';
import { LeetCodeService, Problem } from './leetcodeService';

export class ProblemProvider implements vscode.TreeDataProvider<ProblemItem> {
    private _onDidChangeTreeData: vscode.EventEmitter<ProblemItem | undefined | null | void> = new vscode.EventEmitter<ProblemItem | undefined | null | void>();
    readonly onDidChangeTreeData: vscode.Event<ProblemItem | undefined | null | void> = this._onDidChangeTreeData.event;

    private problems: Problem[] = [];
    private difficultyFilter: string = '';
    private searchKeyword: string = '';
    private isLoading: boolean = false;
    private needsRefresh: boolean = false;  // Flag to force refresh from API
    private cacheLoaded: boolean = false;   // Flag to track if cache was loaded

    constructor(private service: LeetCodeService) {}

    refresh(): void {
        this._onDidChangeTreeData.fire();
    }

    // Force refresh from API (called by refresh button)
    forceRefresh(): void {
        this.needsRefresh = true;
        this.cacheLoaded = false;
        this.refresh();
    }

    setDifficultyFilter(difficulty: string): void {
        this.difficultyFilter = difficulty;
        this.searchKeyword = '';
        this.needsRefresh = true;  // Filter change needs fresh data
        this.refresh();
    }

    setSearchKeyword(keyword: string): void {
        this.searchKeyword = keyword;
        this.needsRefresh = true;  // Search needs fresh data
        this.refresh();
    }

    getTreeItem(element: ProblemItem): vscode.TreeItem {
        return element;
    }

    async getChildren(element?: ProblemItem): Promise<ProblemItem[]> {
        if (element) {
            return [];
        }

        if (this.isLoading) {
            return [new ProblemItem('Loading...', '', '', vscode.TreeItemCollapsibleState.None)];
        }

        // Try to load from cache first (only for default view without filters)
        if (!this.needsRefresh && !this.cacheLoaded && !this.searchKeyword && !this.difficultyFilter) {
            const cached = this.service.loadProblemsCache();
            if (cached && cached.length > 0) {
                this.problems = cached;
                this.cacheLoaded = true;
                return this.buildProblemItems();
            }
        }

        this.isLoading = true;

        try {
            if (this.searchKeyword) {
                this.problems = await this.service.searchProblems(this.searchKeyword);
            } else {
                // Fetch all problems (up to 5000)
                this.problems = await this.service.getProblems(5000, 0, this.difficultyFilter || undefined);
                // Save to cache only for default view (no filters)
                if (!this.difficultyFilter) {
                    this.service.saveProblemsCache(this.problems);
                }
            }
        } catch (e) {
            this.isLoading = false;
            return [new ProblemItem('Failed to load problems', '', '', vscode.TreeItemCollapsibleState.None)];
        }

        this.isLoading = false;
        this.needsRefresh = false;
        this.cacheLoaded = true;

        return this.buildProblemItems();
    }

    private buildProblemItems(): ProblemItem[] {
        if (this.problems.length === 0) {
            return [new ProblemItem('No problems found', '', '', vscode.TreeItemCollapsibleState.None)];
        }

        return this.problems.map(p => {
            const item = new ProblemItem(
                `${p.id}. ${p.title}`,
                p.difficulty,
                p.titleSlug,
                vscode.TreeItemCollapsibleState.None
            );

            // Store problem ID for fetching
            item.id = p.id;

            // Set icon based on difficulty (case insensitive)
            const diff = p.difficulty.toUpperCase();
            switch (diff) {
                case 'EASY':
                    item.iconPath = new vscode.ThemeIcon('circle-filled', new vscode.ThemeColor('charts.green'));
                    break;
                case 'MEDIUM':
                    item.iconPath = new vscode.ThemeIcon('circle-filled', new vscode.ThemeColor('charts.yellow'));
                    break;
                case 'HARD':
                    item.iconPath = new vscode.ThemeIcon('circle-filled', new vscode.ThemeColor('charts.red'));
                    break;
            }

            // Add status indicator
            if (p.status === 'ac') {
                item.description = `${p.difficulty} ✓`;
            } else if (p.status === 'notac') {
                item.description = `${p.difficulty} ○`;
            } else {
                item.description = p.difficulty;
            }

            if (p.paidOnly) {
                item.description = `🔒 ${item.description}`;
            }

            if (p.acRate) {
                item.tooltip = `${p.title}\nDifficulty: ${p.difficulty}\nAcceptance: ${p.acRate}%`;
            }

            item.contextValue = 'problem';
            item.command = {
                command: 'leetcode.openProblem',
                title: 'Open Problem',
                arguments: [{ id: p.id, slug: p.titleSlug, label: `${p.id}. ${p.title}` }]
            };

            return item;
        });
    }
}

export class ProblemItem extends vscode.TreeItem {
    public id?: string;

    constructor(
        public readonly label: string,
        public description: string,
        public readonly slug: string,
        public readonly collapsibleState: vscode.TreeItemCollapsibleState
    ) {
        super(label, collapsibleState);
    }
}
