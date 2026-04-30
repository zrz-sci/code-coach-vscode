import * as vscode from 'vscode';
import { LeetCodeService, Problem } from './leetcodeService';

export class FavoritesProvider implements vscode.TreeDataProvider<FavoriteItem> {
    private _onDidChangeTreeData: vscode.EventEmitter<FavoriteItem | undefined | null | void> = new vscode.EventEmitter<FavoriteItem | undefined | null | void>();
    readonly onDidChangeTreeData: vscode.Event<FavoriteItem | undefined | null | void> = this._onDidChangeTreeData.event;

    private needsRefresh: boolean = false;
    private cacheLoaded: boolean = false;
    private favoriteProblemsCache: Map<string, boolean> = new Map();  // Track which favorites have loaded from cache

    constructor(private service: LeetCodeService) {}

    refresh(): void {
        this._onDidChangeTreeData.fire();
    }

    // Force refresh from API (called by refresh button)
    forceRefresh(): void {
        this.needsRefresh = true;
        this.cacheLoaded = false;
        this.favoriteProblemsCache.clear();
        this.refresh();
    }

    getTreeItem(element: FavoriteItem): vscode.TreeItem {
        return element;
    }

    async getChildren(element?: FavoriteItem): Promise<FavoriteItem[]> {
        if (!this.service.getIsLoggedIn()) {
            return [new FavoriteItem('Please login first', '', '', vscode.TreeItemCollapsibleState.None, 'info')];
        }

        if (!element) {
            // Root level - show favorite lists
            // Try to load from cache first
            if (!this.needsRefresh && !this.cacheLoaded) {
                const cached = this.service.loadFavoritesCache();
                if (cached && cached.length > 0) {
                    this.cacheLoaded = true;
                    return this.buildFavoriteItems(cached);
                }
            }

            try {
                const favorites = await this.service.getFavorites();
                if (favorites.length === 0) {
                    return [new FavoriteItem('No favorites found', '', '', vscode.TreeItemCollapsibleState.None, 'info')];
                }
                // Save to cache
                this.service.saveFavoritesCache(favorites);
                this.cacheLoaded = true;
                this.needsRefresh = false;
                return this.buildFavoriteItems(favorites);
            } catch (e) {
                return [new FavoriteItem('Failed to load favorites', '', '', vscode.TreeItemCollapsibleState.None, 'error')];
            }
        } else if (element.favoriteId) {
            // Show problems in this favorite
            // Try to load from cache first
            if (!this.needsRefresh && !this.favoriteProblemsCache.get(element.favoriteId)) {
                const cached = this.service.loadFavoriteProblemsCache(element.favoriteId);
                if (cached && cached.length > 0) {
                    this.favoriteProblemsCache.set(element.favoriteId, true);
                    return this.buildProblemItems(cached);
                }
            }

            try {
                const problems = await this.service.getFavoriteProblems(element.favoriteId);
                // Save to cache
                this.service.saveFavoriteProblemsCache(element.favoriteId, problems);
                this.favoriteProblemsCache.set(element.favoriteId, true);
                return this.buildProblemItems(problems);
            } catch (e) {
                return [new FavoriteItem('Failed to load problems', '', '', vscode.TreeItemCollapsibleState.None, 'error')];
            }
        }

        return [];
    }

    private buildFavoriteItems(favorites: {id: string, name: string}[]): FavoriteItem[] {
        return favorites.map(fav => {
            const item = new FavoriteItem(
                fav.name,
                fav.id,
                '',  // folders don't need slug
                vscode.TreeItemCollapsibleState.Collapsed,
                'folder'
            );
            item.favoriteId = fav.id;
            return item;
        });
    }

    private buildProblemItems(problems: Problem[]): FavoriteItem[] {
        return problems.map(p => {
            const item = new FavoriteItem(
                `${p.id}. ${p.title}`,
                p.difficulty,
                p.titleSlug,  // Pass titleSlug as constructor parameter
                vscode.TreeItemCollapsibleState.None,
                'problem'
            );
            item.id = p.id;  // Use id like ProblemItem
            item.status = p.status;
            item.difficulty = p.difficulty;

            // Set icon: difficulty-colored circle + status indicator in description
            const diff = (p.difficulty || '').toUpperCase();
            switch (diff) {
                case 'EASY':
                    item.iconPath = new vscode.ThemeIcon('circle-filled', new vscode.ThemeColor('charts.green'));
                    item.description = p.status === 'ac' ? 'EASY ✓' : 'EASY';
                    break;
                case 'MEDIUM':
                    item.iconPath = new vscode.ThemeIcon('circle-filled', new vscode.ThemeColor('charts.yellow'));
                    item.description = p.status === 'ac' ? 'MEDIUM ✓' : 'MEDIUM';
                    break;
                case 'HARD':
                    item.iconPath = new vscode.ThemeIcon('circle-filled', new vscode.ThemeColor('charts.red'));
                    item.description = p.status === 'ac' ? 'HARD ✓' : 'HARD';
                    break;
                default:
                    item.iconPath = new vscode.ThemeIcon('circle-outline');
                    item.description = p.difficulty;
            }

            // Use same command as Problems - pass plain object to avoid serialization issues
            item.command = {
                command: 'leetcode.openProblem',
                title: 'Open Problem',
                arguments: [{ id: p.id, slug: p.titleSlug, label: `${p.id}. ${p.title}` }]
            };

            item.contextValue = 'favoriteProblem';

            return item;
        });
    }
}

export class FavoriteItem extends vscode.TreeItem {
    public id?: string;
    favoriteId?: string;
    status?: string;
    difficulty?: string;

    constructor(
        public readonly label: string,
        public description: string,
        public readonly slug: string,  // Add slug to constructor like ProblemItem
        public readonly collapsibleState: vscode.TreeItemCollapsibleState,
        public readonly type: 'folder' | 'problem' | 'info' | 'error'
    ) {
        super(label, collapsibleState);

        if (type === 'folder') {
            this.iconPath = new vscode.ThemeIcon('folder-library');
            this.contextValue = 'favoriteFolder';
        } else if (type === 'info') {
            this.iconPath = new vscode.ThemeIcon('info');
        } else if (type === 'error') {
            this.iconPath = new vscode.ThemeIcon('error');
        }
    }
}
