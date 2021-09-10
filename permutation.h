#include <bits/stdc++.h>

using namespace std;

class permutation {
	int n;
	int *p;
	static const int mod = 1000000007;
	static int modinv(int, int);
	static vector<int> sieve(int);
	static int comp(vector<int>, vector<int>);

	public:
		permutation(int n, int a[]);
		~permutation();
		permutation(permutation const &q);
		permutation const operator=(permutation const &q);
		int size() const;
		int* to_array() const;
		permutation const operator-() const;
		permutation const operator*(permutation const &q) const;
		permutation const square_root() const;
		permutation const operator^(long long int i) const;
		bool is_power(permutation const &q) const;
		int log(permutation const &q) const;
};

int permutation :: comp(vector<int> a, vector<int> b) {
	return a.size() < b.size();
}

int permutation :: modinv(int a, int m)  {
	int m1 = m;
	int x = 1;
	int y = 0;
	int q, t;
	while (a > 1) {
		q = a/m1;
		t = m1;
		m1 = a%m1;
		a = t;
		t = y;
		y = x - q*y;
		x = t;
	}
	return (x < 0 ? x+m : x);
}

vector<int> permutation :: sieve(int N) {
	vector<int> pr;
	vector<int> lp(N+1, 0);
	for (int i = 2; i <= N; i++) {
		if (lp[i] == 0) {
			lp[i] = i;
			pr.push_back(i);
		}
		for (int j = 0; j < (int)pr.size() && pr[j] <= lp[i] && i*pr[j] <= N; j++)
        	lp[i*pr[j]] = pr[j];
	}
	return lp;
}

permutation :: permutation(int n, int a[]) {
	this->n = n;
	p = new int[n];
	for (int i = 0; i < n; i++)
		p[i] = a[i];
}

permutation :: ~permutation() {
	delete[] p;
}

permutation :: permutation(permutation const &q) {
	n = q.n;
	p = new int[n];
	for (int i = 0; i < n; i++)
		p[i] = q.p[i];
}

permutation const permutation :: operator=(permutation const &q) {
	n = q.n;
	delete[] p;
	p = new int[n];
	for (int i = 0; i < n; i++)
		p[i] = q.p[i];
	return *this;
}

int permutation :: size() const {
	return n;
}

int* permutation :: to_array() const {
	int *arr = new int[n];
	for (int i = 0; i < n; i++)
		arr[i] = p[i];
	return arr;
}

permutation const permutation :: operator-() const {
	permutation ans(*this);
	for (int i = 0; i < n; i++)
		ans.p[i] = p[n-i-1];
	return ans;
}

permutation const permutation :: operator*(permutation const &q) const {
	permutation ans(*this);
	for (int i = 0; i < n; i++)
		ans.p[i] = p[q.p[i]];
	return ans;
}

permutation const permutation :: square_root() const {
	permutation ans(*this);
	bool *visited = new bool[n];
	for (int i = 0; i < n; i++)
		visited[i] = false;
	vector<vector<int>> cycles;
	vector<int> cycle;
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			cycle.clear();
			int j = p[i];
			int len = 1;
			visited[i] = true;
			cycle.push_back(i);
			while (j != i) {
				visited[j] = true;
				cycle.push_back(j);
				len++;
				j = p[j];
			}
			if (len%2 == 1)
				for (int j = 0; j < len; j++)
					ans.p[cycle[j]] = cycle[(j+(len+1)/2) % len];
			else
				cycles.push_back(cycle);
		}
	}
	sort(cycles.begin(), cycles.end(), comp);
	int i = 0;
	while (i + 1 < cycles.size()) {
		if (cycles[i].size() != cycles[i+1].size())
			break;
		for (int j = 0; j < cycles[i].size(); j++) {
			ans.p[cycles[i][j]] = cycles[i+1][j];
			ans.p[cycles[i+1][j]] = cycles[i][(j+1) % cycles[i].size()];
		}
		i += 2;
	}
	if (i != cycles.size()) {
		for (int i = 0; i < n; i++)
			ans.p[i] = i;
	}
	return ans;
}

permutation const permutation :: operator^(long long int i) const {
	permutation ans(*this);
	bool *visited = new bool[n];
	for (int i1 = 0; i1 < n; i1++)
		visited[i1] = false;
	vector<int> cycle;
	for (int i1 = 0; i1 < n; i1++) {
		if (!visited[i1]) {
			cycle.clear();
			int len = 1;
			int j = p[i1];
			visited[i1] = true;
			cycle.push_back(i1);
			while (j != i1) {
				visited[j] = true;
				len++;
				cycle.push_back(j);
				j = p[j];
			}
			int shift = i % len;
			for (int k = 0; k < len; k++)
				ans.p[cycle[k]] = cycle[(k+shift)%len];
		}
	}
	return ans;
}

bool permutation :: is_power(permutation const &q) const {
	vector<int> lp = sieve(100);
	vector<int> primepowers(100, -1);
	vector<int> primerems(100, -1);
	vector<bool> visited(n, false);
	vector<int> cycle;
	vector<int> rems;
	vector<int> moduli;
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			int j = p[i];
			int k = q.p[i];
			int shift = 1;
			int len = 1;
			cycle.clear();
			cycle.push_back(i);
			visited[i] = true;
			while (k != j) {
				if (k == i)
					return false;
				shift++;
				len++;
				cycle.push_back(k);
				visited[k] = true;
				k = q.p[k];
			}
			while (k != i) {
				len++;
				cycle.push_back(k);
				visited[k] = true;
				k = q.p[k];
			}
			rems.push_back(shift%len);
			moduli.push_back(len);
			for (j = 0; j < len; j++)
				if (cycle[(j+shift)%len] != p[cycle[j]])
					return false;
		}
	}
	for (int i = 2; i <= 100; i++) {
		primepowers[i] = -1;
		primerems[i] = -1;
	}
	for (int i = 0; i < moduli.size(); i++) {
		int j = moduli[i];
		if (j == 1)
			continue;
		while (j != 1) {
			int prime = lp[j];
			int power = 0;
			while (prime == lp[j]) {
				power++;
				j /= lp[j];
			}
			if (primepowers[prime] == -1) {
				primepowers[prime] = power;
				primerems[prime] = rems[i];
			}
			else {
				if ((primerems[prime] - rems[i]) % ((int) pow(prime, min(power, primepowers[prime]))) != 0)
					return false;
				if (power > primepowers[prime]) {
					primepowers[prime] = power;
					primerems[prime] = rems[i] % ((int) pow(prime, power));
				}
			}
		}
	}
	return true;
}

int permutation :: log(permutation const &q) const {
	vector<int> lp = sieve(100);
	vector<int> primepowers(100, -1);
	vector<int> primerems(100, -1);
	vector<bool> visited(n, false);
	vector<int> cycle;
	vector<int> rems;
	vector<int> moduli;
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			int j = p[i];
			int k = q.p[i];
			int shift = 1;
			int len = 1;
			cycle.clear();
			cycle.push_back(i);
			visited[i] = true;
			while (k != j) {
				if (k == i)
					return -1;
				shift++;
				len++;
				cycle.push_back(k);
				visited[k] = true;
				k = q.p[k];
			}
			while (k != i) {
				len++;
				cycle.push_back(k);
				visited[k] = true;
				k = q.p[k];
			}
			rems.push_back(shift%len);
			moduli.push_back(len);
			for (j = 0; j < len; j++)
				if (cycle[(j+shift)%len] != p[cycle[j]])
					return -1;
		}
	}
	for (int i = 2; i <= 100; i++) {
		primepowers[i] = -1;
		primerems[i] = -1;
	}
	for (int i = 0; i < moduli.size(); i++) {
		int j = moduli[i];
		if (j == 1)
			continue;
		while (j != 1) {
			int prime = lp[j];
			int power = 0;
			while (prime == lp[j]) {
				power++;
				j /= lp[j];
			}
			if (primepowers[prime] == -1) {
				primepowers[prime] = power;
				primerems[prime] = rems[i] % ((int) pow(prime, power));
			}
			else {
				if ((primerems[prime] - rems[i]) % ((int) pow(prime, min(power, primepowers[prime]))) != 0)
					return -1;
				if (power > primepowers[prime]) {
					primepowers[prime] = power;
					primerems[prime] = rems[i] % ((int) pow(prime, power));
				}
			}
		}
	}
	vector<int> finalmoduli;
	vector<int> finalrems;
	long long int prod = 1, tmp2;
	long long int ans = 0;
	int tmp;
	for (int i = 2; i < primepowers.size(); i++) {
		if (primepowers[i] > 0) {
			tmp = ((int) pow(i, primepowers[i]));
			finalmoduli.push_back(tmp);
			finalrems.push_back(primerems[i]);
			prod *= tmp;
		}
	}
	for (int i = 0; i < finalmoduli.size(); i++) {
		tmp2 = prod/finalmoduli[i];
		tmp = modinv(tmp2 % finalmoduli[i], finalmoduli[i]);
		ans = ((ans + ((((tmp2 % prod) * finalrems[i]) % prod) * tmp) % prod) % prod) % mod;
	}
	return ((int) ans);
}