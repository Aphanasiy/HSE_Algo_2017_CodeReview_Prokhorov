#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <queue>

using std::vector;
using std::pair;
using std::cin;
using std::cout;
using std::endl;


const long long INF = (2e9 + 7) * 1e6;


struct Edge {
	int to;
	int cpc;
	int flow = 0;
	int tm;
	int num;
	Edge* back = nullptr;
	Edge(){};
	Edge(int _to, int _tm, int _cpc, int _num)
	: to(_to)
	, tm(_tm)
	, cpc(_cpc)
	, num(_num)
	{}; 
	// This is my style. I know, that it will 
	// work without '_', but I prefer to use it
};


std::ostream& operator << (std::ostream& out, vector<int> const& v) {
	for (auto i : v)
	{
		out << i << ' ';
	}
	return out;
}


void dfs(const vector<vector<Edge*>>& g,
		 vector<int>& ans,
		 long double& tm, int v = 1)
{
	for (auto i : g[v])
	{
		if (i->flow == 1)
		{
			ans.push_back(i->num);
			tm += i->tm;
			i->flow = 0;
			i->back->flow = 0;
			dfs(g, ans, tm, i->to);
			return;
		}
	}
}


bool dijkstra(vector<vector<Edge*>>& g, vector<long long>& p, int start, int finish) {
	// g = graph;
	// p = potential;
	size_t n = g.size() - 1;
	vector<pair<long long, Edge*>> go(g.size(), {INF, nullptr});
	vector<bool> vis(n, 0);

	go[start] = {0, nullptr};
	while (true)
	{
		int v = -1;
		pair<long long, Edge*> cur = {INF, nullptr};
		for (size_t i = 1; i <= n; i++)
		{
			if (vis[i] == 0 && go[i] < cur)
			{
				cur = go[i];
				v = i;
			}
		}
		if (v == -1)
		{
			break;
		}
		vis[v] = 1;
		int d = cur.first;
		for (auto i : g[v])
		{
			if (i->cpc > i->flow && go[i->to].first > d + p[v] + i->tm - p[i->to])
			{
				go[i->to] = {d + p[v] + i->tm - p[i->to], i};
			}
		}
	}
	if (go[finish].second == nullptr)
	{
		return 0;
	}
	int city = finish;
	while (go[city].second != nullptr)
	{
		go[city].second->flow += 1;
		go[city].second->back->flow -= 1;
		city = go[city].second->back->to;
	}
	for (size_t i = 0; i < n; i++)
	{
		p[i] += go[i].first;
	}
	return 1;
}


void bfs(const vector<vector<Edge*>> & g, vector<long long> & q) {
	size_t n = g.size() - 1;
	size_t start = 1;
	vector<bool> vis(g.size(), 0);
	vector<pair<int, Edge*>> p(g.size(), {INF, nullptr});
	p[start] = {0, nullptr};
	std::queue<int> qu;
	qu.push(start);
	vis[start] = 1;
	while (!qu.empty())
	{
		int cur = qu.front();
		qu.pop();

		for (auto i : g[cur])
		{
			if (p[cur].first + i->tm * (i->flow < 0 ? -1 : 1) < p[i->to].first && i->cpc > i->flow)
			{
				p[i->to] = {p[cur].first + i->tm * (i->flow < 0 ? -1 : 1), i};
				if (!vis[i->to])
				{
					qu.push(i->to);
					vis[i->to] = 1;
				}
			}
		}
		vis[cur] = 0;
	}
	if (p[n].second == nullptr)
	{
		return;
	}
	for (size_t i = 0; i <= n; ++i) {
		q[i] = p[i].first;
	}
}


void read(vector<vector<Edge*>>& g, vector<vector<int>>& ans, size_t& k) {
	size_t n, m;
	cin >> n >> m >> k;
	g = vector<vector<Edge*>>(n + 1);
	for (size_t i = 1; i <= m; i++)
	{
		int x, y, t;
		cin >> x >> y >> t;
		Edge * e = new Edge(y, t, 1, i);
		Edge * e_b = new Edge(x, t, 1, i);
		e->back = e_b;
		e_b->back = e;
		g[x].push_back(e);
		g[y].push_back(e_b);
	}
	ans = vector<vector<int>>(k + 1);
	return; 
}


long double solution(vector<vector<Edge*>>& g, vector<vector<int>>& ans, size_t k) {
	vector<long long> q(g.size() + 1, 0);
	bfs(g, q);
	for (size_t i = 0; i < k; ++i)
		if (!dijkstra(g, q, 1, g.size() - 1)){
			cout << -1 << endl;
			return 0;
		};
	long double tm = 0;
	for (size_t i = 0; i < k; i++)
	{
		dfs(g, ans[i], tm);
	
	}
	return tm;
}


void write(vector<vector<int>>& ans, long double tm, size_t k) {
	cout.precision(6);
	cout << std::fixed << tm / k << endl;
	for (auto i : ans)
	{
		cout << i.size() << ' ' << i << endl;
	}
}


int main() {
	vector<vector<Edge*>> g;
	vector<vector<int>> ans;
	size_t k = 0;
	read(g, ans, k);
	write(ans, solution(g, ans, k), k);
}