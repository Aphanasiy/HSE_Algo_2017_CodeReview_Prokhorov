#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;


struct Edge
{
	int to;
	int cpc;
	int flow = 0;
	int tm;
	int num;
	Edge * back = nullptr;
	Edge(){};
	Edge(int _to, int _tm, int _cpc, int _num): to(_to), tm(_tm), cpc(_cpc), num(_num){};
};


ostream & operator << (ostream & out, vector<int> const& v)
{
	for (auto i : v)
	{
		out << i << ' ';
	}
	return out;
}


const long long INF = (2e9 + 7) * 1e6;


void dfs(const vector<vector<Edge*>> & g, vector<int> & ans, long double & tm, int v = 1)
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


bool djikstra(vector<vector<Edge*>> & g, vector<long long> & q, int start, int finish)
{
	cerr << "DBEGIN" << endl;
	int n = g.size() - 1;
	vector<pair<long long, Edge*>> go(g.size(), {INF, nullptr});
	vector<bool> vis(n, 0);

	go[start] = {0, nullptr};
	while (true)
	{
		int v = -1;
		pair<long long, Edge*> cur = {INF, nullptr};
		for (int i = 1; i <= n; i++)
		{
			if (vis[i] == 0 && go[i] < cur)
			{
				cur = go[i];
				v = i;
			}
		}
		cerr << v << endl;
		if (v == -1)
		{
			break;
		}
		//cerr << "V: " << v << ' ' << go[v].first << endl;
		vis[v] = 1;
		int d = cur.first;
		for (auto i : g[v])
		{
			if (i->cpc > i->flow && go[i->to].first > d + q[v] + i->tm - q[i->to])
			{
				go[i->to] = make_pair(d + q[v] + i->tm - q[i->to], i);
			}
		}
	}
	cerr << finish << endl;
	if (go[finish].second == nullptr)
	{
		return 0;
	}
	cerr << "DDONE" << endl;
	int city = finish;
	/*
	cerr << "KEK" << endl;
	for (int i = 0; i < 2 * n + 2; i++)
	{
		cerr << i << ": " << "Q" << q[i] << ' ' << go[i].first << ' ' << (go[i].second == nullptr ? -1 : go[i].second->back->to)  << endl;
	}
	//*/
	while (go[city].second != nullptr)
	{
		go[city].second->flow += 1;
		go[city].second->back->flow -= 1;
		city = go[city].second->back->to;
	}
	for (int i = 0; i < n; i++)
	{
		q[i] += go[i].first;
	}
	/*
	for (int i = 0; i < 2 * n + 2; i++)
	{
		cerr << q[i] << ' ';
	}
	cerr << endl;
	//*/
	return 1;
}


void bfs(const vector<vector<Edge*>> & g, vector<long long> & q)
{
	int n = g.size() - 1;
	int start = 1;
	vector<bool> vis(g.size(), 0);
	vector<pair<int, Edge*>> p(g.size(), {INF, nullptr});
	p[start] = {0, nullptr};
	queue<int> qu;
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
	for (int i = 0; i <= n; ++i) {
		q[i] = p[i].first;
	}
}

int main()
{
	//ifstream cin("brides.in");
	//ofstream cout("brides.out");

	int n, m, k;
	cin >> n >> m >> k;
	vector<vector<Edge*>> g(n + 1);
	for (int i = 1; i <= m; i++)
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
	vector<vector<int>> ans(k);
	vector<long long> q(n + 1, 0);
	bfs(g, q);
	//cerr << q << endl;
	for (int i = 0; i < k; ++i)
		if (!djikstra(g, q, 1, n)){
			cout << -1 << endl;
			return 0;
		};
	long double tm = 0;
	for (int i = 0; i < k; i++)
	{
		dfs(g, ans[i], tm);
	}
	//cerr.precision(6);
	//cerr << fixed << tm / k << endl;
	cout.precision(6);
	cout << fixed << tm / k << endl;
	for (auto i : ans)
	{
		//cerr << i.size() << ": " << i << endl;
		cout << i.size() << ' ' << i << endl;
	}
}