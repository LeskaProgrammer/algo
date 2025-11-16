#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>

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

void heapify(vector<int> &nums, int left, int heapSize, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < heapSize && nums[left + l] > nums[left + largest]) {
        largest = l;
    }
    if (r < heapSize && nums[left + r] > nums[left + largest]) {
        largest = r;
    }
    if (largest != i) {
        swap(nums[left + i], nums[left + largest]);
        heapify(nums, left, heapSize, largest);
    }
}

void buildMaxHeap(vector<int> &nums, int left, int right) {
    int heapSize = right - left + 1;
    for (int i = heapSize / 2 - 1; i >= 0; --i) {
        heapify(nums, left, heapSize, i);
    }
}

void heapSort(vector<int> &nums, int left, int right) {
    int heapSize = right - left + 1;
    if (heapSize <= 1) return;

    buildMaxHeap(nums, left, right);

    for (int i = heapSize - 1; i > 0; --i) {
        swap(nums[left], nums[left + i]);
        heapify(nums, left, i, 0);
    }
}

static mt19937 rng_q(712367);

void quick_sort_std(vector<int> &arr, int left, int right) {
    if (left >= right) return;

    int i = left;
    int j = right;
    int pivot_index = left + (int)(rng_q() % (uint64_t)(right - left + 1));
    int x = arr[pivot_index];

    while (i <= j) {
        while (arr[i] < x) ++i;
        while (arr[j] > x) --j;
        if (i <= j) {
            swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    if (left < j) quick_sort_std(arr, left, j);
    if (i < right) quick_sort_std(arr, i, right);
}

void quick_sort_intro(vector<int> &arr, int left, int right, int depth) {
    if (left >= right) return;

    int len = right - left + 1;

    if (depth == 0) {
        heapSort(arr, left, right);
        return;
    }

    if (len < 16) {
        insertion_sort(arr, left, right);
        return;
    }

    int i = left;
    int j = right;
    int pivot_index = left + (int)(rng_q() % (uint64_t)(right - left + 1));
    int x = arr[pivot_index];

    while (i <= j) {
        while (arr[i] < x) ++i;
        while (arr[j] > x) --j;
        if (i <= j) {
            swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    if (left < j) quick_sort_intro(arr, left, j, depth - 1);
    if (i < right) quick_sort_intro(arr, i, right, depth - 1);
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

    SortTester(ArrayGenerator &g, int repeats_)
            : gen(g), repeats(repeats_) {}

    long long time_quick(const vector<int> &input) {
        long long sum = 0;
        for (int r = 0; r < repeats; ++r) {
            vector<int> arr = input;
            auto start = chrono::high_resolution_clock::now();
            quick_sort_std(arr, 0, (int)arr.size() - 1);
            auto elapsed = chrono::high_resolution_clock::now() - start;
            sum += chrono::duration_cast<chrono::microseconds>(elapsed).count();
        }
        return sum / repeats;
    }

    long long time_intro(const vector<int> &input) {
        long long sum = 0;
        for (int r = 0; r < repeats; ++r) {
            vector<int> arr = input;
            int n = (int)arr.size();
            int depth = 0;
            if (n > 0) depth = 2 * (int)log2((double)n);
            auto start = chrono::high_resolution_clock::now();
            quick_sort_intro(arr, 0, n - 1, depth);
            auto elapsed = chrono::high_resolution_clock::now() - start;
            sum += chrono::duration_cast<chrono::microseconds>(elapsed).count();
        }
        return sum / repeats;
    }

    void run_all() {
        cout << "#type size quick intro\n";

        for (int type = 0; type < 3; ++type) {
            for (int n = 500; n <= 100000; n += 100) {
                vector<int> base;
                if (type == 0) base = gen.get_random(n);
                else if (type == 1) base = gen.get_reversed(n);
                else base = gen.get_almost_sorted(n);

                long long quick_time = time_quick(base);
                long long intro_time = time_intro(base);

                cout << type << " " << n << " " << quick_time << " " << intro_time << "\n";
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
    int repeats = 3;

    SortTester tester(gen, repeats);
    tester.run_all();

    return 0;
}
