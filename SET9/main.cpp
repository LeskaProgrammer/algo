#include <bits/stdc++.h>
using namespace std;

struct TestResult {
    string dataset;
    string algorithm;
    int n;
    double time_us;
    double char_ops;
};

long long char_ops = 0;

bool cmp(const string& a, const string& b) {
    int i = 0;
    while (i < (int)a.size() && i < (int)b.size()) {
        char_ops++;
        if (a[i] != b[i]) return a[i] < b[i];
        i++;
    }
    return a.size() <= b.size();
}

void merge_sort(vector<string>& a, int l, int r) {
    if (r - l <= 1) return;

    int m = (l + r) / 2;
    merge_sort(a, l, m);
    merge_sort(a, m, r);

    vector<string> b;
    int i = l, j = m;

    while (i < m && j < r) {
        if (cmp(a[i], a[j])) b.push_back(a[i++]);
        else b.push_back(a[j++]);
    }

    while (i < m) b.push_back(a[i++]);
    while (j < r) b.push_back(a[j++]);

    for (int k = 0; k < (int)b.size(); k++) {
        a[l + k] = b[k];
    }
}

int cchar(const string& s, int d) {
    if (d >= (int)s.size()) return -1;
    char_ops++;
    return (unsigned char)s[d];
}

void string_quick_sort(vector<string>& a, int l, int r, int d) {
    if (l >= r) return;

    int lt = l, gt = r;
    int v = cchar(a[(l + r) / 2], d);
    int i = l;

    while (i <= gt) {
        int x = cchar(a[i], d);

        if (x < v) {
            swap(a[lt], a[i]);
            lt++;
            i++;
        } else if (x > v) {
            swap(a[i], a[gt]);
            gt--;
        } else {
            i++;
        }
    }

    string_quick_sort(a, l, lt - 1, d);
    if (v != -1) string_quick_sort(a, lt, gt, d + 1);
    string_quick_sort(a, gt + 1, r, d);
}

int get_char(const string& s, int d) {
    if (d >= (int)s.size()) return 0;
    char_ops++;
    return (unsigned char)s[d] + 1;
}

void radix_sort(vector<string>& a, int l, int r, int d) {
    if (r - l <= 1) return;

    const int R = 257;
    vector<int> cnt(R + 1, 0);

    for (int i = l; i < r; i++) {
        cnt[get_char(a[i], d) + 1]++;
    }

    for (int i = 1; i <= R; i++) {
        cnt[i] += cnt[i - 1];
    }

    vector<int> pos = cnt;
    vector<string> b(r - l);

    for (int i = l; i < r; i++) {
        int c = get_char(a[i], d);
        b[pos[c]++] = a[i];
    }

    for (int i = 0; i < r - l; i++) {
        a[l + i] = b[i];
    }

    for (int c = 1; c < R; c++) {
        int left = l + cnt[c];
        int right = l + cnt[c + 1];
        radix_sort(a, left, right, d + 1);
    }
}

int ch(const string& s, int d) {
    if (d >= (int)s.size()) return 0;
    char_ops++;
    return (unsigned char)s[d] + 1;
}

void rq_quick_sort(vector<string>& a, int l, int r, int d) {
    if (l >= r) return;

    int lt = l, gt = r;
    int v = ch(a[(l + r) / 2], d);
    int i = l;

    while (i <= gt) {
        int x = ch(a[i], d);

        if (x < v) {
            swap(a[lt], a[i]);
            lt++;
            i++;
        } else if (x > v) {
            swap(a[i], a[gt]);
            gt--;
        } else {
            i++;
        }
    }

    rq_quick_sort(a, l, lt - 1, d);
    if (v != 0) rq_quick_sort(a, lt, gt, d + 1);
    rq_quick_sort(a, gt + 1, r, d);
}

void radix_quick_sort(vector<string>& a, int l, int r, int d) {
    if (r - l <= 1) return;

    if (r - l < 74) {
        rq_quick_sort(a, l, r - 1, d);
        return;
    }

    const int R = 257;
    vector<int> cnt(R + 1, 0);

    for (int i = l; i < r; i++) {
        cnt[ch(a[i], d) + 1]++;
    }

    for (int i = 1; i <= R; i++) {
        cnt[i] += cnt[i - 1];
    }

    vector<int> pos = cnt;
    vector<string> b(r - l);

    for (int i = l; i < r; i++) {
        int c = ch(a[i], d);
        b[pos[c]++] = a[i];
    }

    for (int i = 0; i < r - l; i++) {
        a[l + i] = b[i];
    }

    for (int c = 1; c < R; c++) {
        int left = l + cnt[c];
        int right = l + cnt[c + 1];
        radix_quick_sort(a, left, right, d + 1);
    }
}

struct StringGenerator {
    string alphabet;
    mt19937 rng;

    StringGenerator(unsigned seed = 42) : rng(seed) {
        string upper, lower, digits, special;
        for (char c = 'A'; c <= 'Z'; c++) upper += c;
        for (char c = 'a'; c <= 'z'; c++) lower += c;
        for (char c = '0'; c <= '9'; c++) digits += c;
        special = "!@#%:;^&*()-";
        alphabet = upper + lower + digits + special;
        sort(alphabet.begin(), alphabet.end());
    }

    string randomString(int minLen = 10, int maxLen = 200) {
        uniform_int_distribution<int> lenDist(minLen, maxLen);
        uniform_int_distribution<int> charDist(0, (int)alphabet.size() - 1);

        int len = lenDist(rng);
        string s;
        s.reserve(len);

        for (int i = 0; i < len; i++) {
            s += alphabet[charDist(rng)];
        }

        return s;
    }

    vector<string> randomArray(int n) {
        vector<string> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = randomString();
        }
        return a;
    }

    vector<string> commonPrefixArray(int n, int prefixLen = 50) {
        vector<string> a;
        a.reserve(n);

        string prefix;
        uniform_int_distribution<int> charDist(0, (int)alphabet.size() - 1);

        for (int i = 0; i < prefixLen; i++) {
            prefix += alphabet[charDist(rng)];
        }

        for (int i = 0; i < n; i++) {
            a.push_back(prefix + randomString(10, 150));
        }

        return a;
    }
};

bool is_sorted_standard(const vector<string>& a) {
    for (int i = 1; i < (int)a.size(); i++) {
        if (a[i] < a[i - 1]) return false;
    }
    return true;
}

TestResult run_once(const vector<string>& input, const string& dataset, const string& algorithm, int repeats = 7) {
    double total_time = 0;
    double total_ops = 0;

    for (int rep = 0; rep < repeats; rep++) {
        vector<string> a = input;
        char_ops = 0;

        auto start = chrono::high_resolution_clock::now();

        if (algorithm == "A1m_merge_sort") {
            merge_sort(a, 0, (int)a.size());
        } else if (algorithm == "A1q_string_quick_sort") {
            if (!a.empty()) string_quick_sort(a, 0, (int)a.size() - 1, 0);
        } else if (algorithm == "A1r_msd_radix_sort") {
            radix_sort(a, 0, (int)a.size(), 0);
        } else if (algorithm == "A1rq_msd_radix_quick_sort") {
            radix_quick_sort(a, 0, (int)a.size(), 0);
        }

        auto finish = chrono::high_resolution_clock::now();

        double elapsed = chrono::duration<double, micro>(finish - start).count();

        if (!is_sorted_standard(a)) {
            cerr << "Sorting error in " << algorithm << '\n';
            exit(1);
        }

        total_time += elapsed;
        total_ops += char_ops;
    }

    return {
        dataset,
        algorithm,
        (int)input.size(),
        total_time / repeats,
        total_ops / repeats
    };
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    StringGenerator generator(42);

    vector<string> randomMax = generator.randomArray(3000);

    vector<string> reversedMax = randomMax;
    sort(reversedMax.begin(), reversedMax.end());
    reverse(reversedMax.begin(), reversedMax.end());

    vector<string> almostMax = randomMax;
    sort(almostMax.begin(), almostMax.end());

    mt19937 rng(123);
    for (int block = 0; block < 3000; block += 100) {
        int l = block;
        int r = min(2999, block + 99);
        uniform_int_distribution<int> dist(l, r);
        int x = dist(rng);
        int y = dist(rng);
        swap(almostMax[x], almostMax[y]);
    }

    vector<string> prefixMax = generator.commonPrefixArray(3000, 50);

    vector<pair<string, vector<string>>> datasets = {
        {"random", randomMax},
        {"reversed", reversedMax},
        {"almost_sorted", almostMax},
        {"common_prefix", prefixMax}
    };

    vector<string> algorithms = {
        "A1m_merge_sort",
        "A1q_string_quick_sort",
        "A1r_msd_radix_sort",
        "A1rq_msd_radix_quick_sort"
    };

    ofstream out("results.csv");
    out << "dataset,n,algorithm,time_us,char_ops\n";

    for (auto& [datasetName, data] : datasets) {
        for (int n = 100; n <= 3000; n += 100) {
            vector<string> input(data.begin(), data.begin() + n);

            for (const string& algorithm : algorithms) {
                TestResult result = run_once(input, datasetName, algorithm, 7);

                out << result.dataset << ','
                    << result.n << ','
                    << result.algorithm << ','
                    << result.time_us << ','
                    << result.char_ops << '\n';

                cerr << result.dataset << ' '
                     << result.n << ' '
                     << result.algorithm << ' '
                     << result.time_us << " us "
                     << result.char_ops << " ops\n";
            }
        }
    }

    return 0;
}