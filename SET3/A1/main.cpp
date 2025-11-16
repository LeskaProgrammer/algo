#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

Circle c1{1.0, 1.0, 1.0};
Circle c2{1.5, 2.0, std::sqrt(5.0) / 2.0};
Circle c3{2.0, 1.5, std::sqrt(5.0) / 2.0};
Circle C[3] = {c1, c2, c3};

bool inside_all(double x, double y) {
    for (int i = 0; i < 3; ++i) {
        double dx = x - C[i].x;
        double dy = y - C[i].y;
        if (dx * dx + dy * dy > C[i].r * C[i].r) {
            return false;
        }
    }
    return true;
}

double monte_carlo(double xmin, double xmax, double ymin, double ymax, int N, mt19937_64 &rng) {
    uniform_real_distribution<double> dist_x(xmin, xmax);
    uniform_real_distribution<double> dist_y(ymin, ymax);

    int inside = 0;
    for (int i = 0; i < N; ++i) {
        double x = dist_x(rng);
        double y = dist_y(rng);
        if (inside_all(x, y)) {
            ++inside;
        }
    }

    double S_rect = (xmax - xmin) * (ymax - ymin);
    return S_rect * static_cast<double>(inside) / static_cast<double>(N);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    const double S_exact = 0.25 * M_PI + 1.25 * asin(0.8) - 1.0;


    double xmin_w = min({c1.x - c1.r, c2.x - c2.r, c3.x - c3.r});
    double xmax_w = max({c1.x + c1.r, c2.x + c2.r, c3.x + c3.r});
    double ymin_w = min({c1.y - c1.r, c2.y - c2.r, c3.y - c3.r});
    double ymax_w = max({c1.y + c1.r, c2.y + c2.r, c3.y + c3.r});


    double xmin_n = max({c1.x - c1.r, c2.x - c2.r, c3.x - c3.r});
    double xmax_n = min({c1.x + c1.r, c2.x + c2.r, c3.x + c3.r});
    double ymin_n = max({c1.y - c1.r, c2.y - c2.r, c3.y - c3.r});
    double ymax_n = min({c1.y + c1.r, c2.y + c2.r, c3.y + c3.r});

    mt19937_64 rng(42);

    cout.setf(ios::fixed);
    cout.precision(10);
    cout << "#N S_wide S_narrow rel_err_wide rel_err_narrow\n";

    for (int N = 100; N <= 100000; N += 500) {
        double S_wide = monte_carlo(xmin_w, xmax_w, ymin_w, ymax_w, N, rng);
        double S_narrow = monte_carlo(xmin_n, xmax_n, ymin_n, ymax_n, N, rng);

        double rel_w = (S_wide - S_exact) / S_exact;
        double rel_n = (S_narrow - S_exact) / S_exact;

        cout << N << " " << S_wide << " " << S_narrow << " " << rel_w << " " << rel_n << "\n";
    }

    return 0;
}
