#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

void insertion_sort(vector<int> &arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void merge_range(vector<int> &arr, int left, int mid, int right) {
    vector<int> temp;
    temp.reserve(right - left + 1);

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i]);
            ++i;
        } else {
            temp.push_back(arr[j]);
            ++j;
        }
    }

    while (i <= mid) {
        temp.push_back(arr[i]);
        ++i;
    }

    while (j <= right) {
        temp.push_back(arr[j]);
        ++j;
    }

    for (int k = 0; k < (int)temp.size(); ++k) {
        arr[left + k] = temp[k];
    }
}

void merge_sort_std(vector<int> &arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort_std(arr, left, mid);
    merge_sort_std(arr, mid + 1, right);
    merge_range(arr, left, mid, right);
}

void merge_sort_hybrid(vector<int> &arr, int left, int right, int threshold) {
    if (right - left + 1 <= threshold) {
        insertion_sort(arr, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    merge_sort_hybrid(arr, left, mid, threshold);
    merge_sort_hybrid(arr, mid + 1, right, threshold);
    merge_range(arr, left, mid, right);
}

struct ArrayGenerator {
    int max_n;
    int min_value;
    int max_value;
    mt19937 rng;
    vector<int> random_data;
    vector<int> reversed_data;
    vector<int> almost_sorted_data;

    ArrayGenerator(int max_n_, int min_value_, int max_value_)
            : max_n(max_n_), min_value(min_value_), max_value(max_value_), rng(42) {
        random_data.resize(max_n);
        uniform_int_distribution<int> dist(min_value, max_value);
        for (int i = 0; i < max_n; ++i) {
            random_data[i] = dist(rng);
        }

        vector<int> sorted = random_data;
        sort(sorted.begin(), sorted.end());

        reversed_data = sorted;
        reverse(reversed_data.begin(), reversed_data.end());

        almost_sorted_data = sorted;
        int swaps = max_n / 100;
        if (swaps < 1) swaps = 1;
        uniform_int_distribution<int> pos(0, max_n - 1);
        for (int i = 0; i < swaps; ++i) {
            int a = pos(rng);
            int b = pos(rng);
            if (a != b) {
                swap(almost_sorted_data[a], almost_sorted_data[b]);
            }
        }
    }

    vector<int> get_random(int n) const {
        return vector<int>(random_data.begin(), random_data.begin() + n);
    }

    vector<int> get_reversed(int n) const {
        return vector<int>(reversed_data.begin(), reversed_data.begin() + n);
    }

    vector<int> get_almost_sorted(int n) const {
        return vector<int>(almost_sorted_data.begin(), almost_sorted_data.begin() + n);
    }
};

struct SortTester {
    ArrayGenerator &gen;
    int repeats;
    vector<int> thresholds;

    SortTester(ArrayGenerator &g, int repeats_, const vector<int> &ths)
            : gen(g), repeats(repeats_), thresholds(ths) {}

    long long time_merge(const vector<int> &input) {
        long long sum = 0;
        for (int r = 0; r < repeats; ++r) {
            vector<int> arr = input;
            auto start = chrono::high_resolution_clock::now();
            merge_sort_std(arr, 0, (int)arr.size() - 1);
            auto elapsed = chrono::high_resolution_clock::now() - start;
            sum += chrono::duration_cast<chrono::microseconds>(elapsed).count();
        }
        return sum / repeats;
    }

    long long time_hybrid(const vector<int> &input, int threshold) {
        long long sum = 0;
        for (int r = 0; r < repeats; ++r) {
            vector<int> arr = input;
            auto start = chrono::high_resolution_clock::now();
            merge_sort_hybrid(arr, 0, (int)arr.size() - 1, threshold);
            auto elapsed = chrono::high_resolution_clock::now() - start;
            sum += chrono::duration_cast<chrono::microseconds>(elapsed).count();
        }
        return sum / repeats;
    }

    void run_all() {
        cout << "#type size merge";
        for (int t : thresholds) cout << " hybrid_" << t;
        cout << "\n";

        for (int type = 0; type < 3; ++type) {
            for (int n = 500; n <= 100000; n += 100) {
                vector<int> base;
                if (type == 0) base = gen.get_random(n);
                else if (type == 1) base = gen.get_reversed(n);
                else base = gen.get_almost_sorted(n);

                long long merge_time = time_merge(base);
                cout << type << " " << n << " " << merge_time;

                for (int t : thresholds) {
                    long long hybrid_time = time_hybrid(base, t);
                    cout << " " << hybrid_time;
                }
                cout << "\n";
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int max_n = 100000;
    int min_value = 0;
    int max_value = 6000;

    ArrayGenerator gen(max_n, min_value, max_value);
    vector<int> thresholds = {5, 10, 20, 30, 50};
    int repeats = 3;

    SortTester tester(gen, repeats, thresholds);
    tester.run_all();

    return 0;
}
