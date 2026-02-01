#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

static ull splitmix64(ull x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x ^= (x >> 31);
    return x;
}

static ull fnv1a64(const string& s) {
    ull h = 14695981039346656037ULL;
    for (unsigned char c : s) {
        h ^= (ull)c;
        h *= 1099511628211ULL;
    }
    return h;
}

struct HashFuncGen {
    ull seed;
    ull operator()(const string& s) const {
        return splitmix64(fnv1a64(s) ^ seed);
    }
};

struct RandomStreamGen {
    mt19937_64 rng;
    string alphabet;

    RandomStreamGen(ull seed)
        : rng(seed),
          alphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-") {}

    string next() {
        uniform_int_distribution<int> len_d(1, 30);
        uniform_int_distribution<int> ch_d(0, (int)alphabet.size() - 1);
        int len = len_d(rng);
        string s;
        s.reserve(len);
        for (int i = 0; i < len; i++) s.push_back(alphabet[ch_d(rng)]);
        return s;
    }
};

struct HyperLogLog {
    int B, m;
    vector<unsigned char> reg;
    HashFuncGen h;

    HyperLogLog(int B_, ull seed) : B(B_), m(1 << B_), reg(m, 0), h{seed} {}

    void add(const string& s) {
        ull x = h(s);
        ull idx = x >> (64 - B);
        ull w = (x << B);
        int rho = 1;
        while ((w & (1ULL << 63)) == 0 && rho <= 64) {
            rho++;
            w <<= 1;
        }
        if (rho > (int)reg[idx]) reg[idx] = (unsigned char)rho;
    }

    double estimate() const {
        double alpha;
        if (m == 16) alpha = 0.673;
        else if (m == 32) alpha = 0.697;
        else if (m == 64) alpha = 0.709;
        else alpha = 0.7213 / (1.0 + 1.079 / (double)m);

        double sum = 0.0;
        int zeros = 0;
        for (int i = 0; i < m; i++) {
            sum += ldexp(1.0, -(int)reg[i]);
            if (reg[i] == 0) zeros++;
        }

        double E = alpha * (double)m * (double)m / sum;

        if (E <= 2.5 * (double)m && zeros > 0) {
            E = (double)m * log((double)m / (double)zeros);
        }

        return E;
    }
};

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N = 50000;
    int streams = 30;
    int B = 10;
    int parts = 20;
    ull seed = 42;
    string out = "out";

    for (int i = 1; i + 1 < argc; i += 2) {
        string k = argv[i], v = argv[i + 1];
        if (k == "--n") N = stoi(v);
        else if (k == "--streams") streams = stoi(v);
        else if (k == "--B") B = stoi(v);
        else if (k == "--parts") parts = stoi(v);
        else if (k == "--seed") seed = (ull)stoull(v);
        else if (k == "--out") out = v;
    }

    if (B < 4) B = 4;
    if (B > 20) B = 20;

    vector<int> checkpoints;
    for (int p = 1; p <= parts; p++) {
        int t = (int)((long long)N * p / parts);
        if (t < 1) t = 1;
        if (t > N) t = N;
        if (checkpoints.empty() || checkpoints.back() != t) checkpoints.push_back(t);
    }

    int L = (int)checkpoints.size();
    vector<long double> sum_est(L, 0.0L), sum2_est(L, 0.0L), sum_true(L, 0.0L);

    ofstream ts(out + "_timeseries.csv");
    ts << "stream,step,processed,true_unique,estimate\n";

    for (int s = 0; s < streams; s++) {
        ull sseed = splitmix64(seed ^ (ull)(s + 1));
        RandomStreamGen gen(sseed);
        HyperLogLog hll(B, splitmix64(sseed ^ 0xA5A5A5A5A5A5A5A5ULL));

        unordered_set<string> exact;
        exact.reserve((size_t)N * 2);

        int next_cp = 0;

        for (int i = 1; i <= N; i++) {
            string x = gen.next();
            exact.insert(x);
            hll.add(x);

            if (next_cp < L && i == checkpoints[next_cp]) {
                long double tru = (long double)exact.size();
                long double est = (long double)hll.estimate();

                ts << s << "," << next_cp << "," << i << "," << (unsigned long long)tru << "," << (double)est << "\n";

                sum_true[next_cp] += tru;
                sum_est[next_cp] += est;
                sum2_est[next_cp] += est * est;

                next_cp++;
                if (next_cp >= L) break;
            }
        }
    }

    ofstream st(out + "_stats.csv");
    st << "step,processed,mean_true,mean_est,std_est\n";

    for (int j = 0; j < L; j++) {
        long double mean_true = sum_true[j] / (long double)streams;
        long double mean_est = sum_est[j] / (long double)streams;
        long double ex2 = sum2_est[j] / (long double)streams;
        long double var = ex2 - mean_est * mean_est;
        if (var < 0) var = 0;
        long double sd = sqrt((double)var);

        st << j << "," << checkpoints[j] << "," << (double)mean_true << "," << (double)mean_est << "," << (double)sd << "\n";
    }

    int m = 1 << B;
    double theo1 = 1.04 / sqrt((double)m);
    double theo2 = 1.30 / sqrt((double)m);

    cout << "B=" << B << " m=" << m << "\n";
    cout << "theory_rel_1_04=" << theo1 << "\n";
    cout << "theory_rel_1_30=" << theo2 << "\n";
    cout << "written " << out << "_timeseries.csv and " << out << "_stats.csv\n";

    return 0;
}
