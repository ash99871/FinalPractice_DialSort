#include <algorithm>
#include <atomic>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

// ── Universe-size guard ───────────────────────────────────────────────────────
static std::pair<bool, uint64_t> universe_size(int mn, int mx) {
    const uint64_t U = static_cast<uint64_t>(
                               static_cast<int64_t>(mx) - static_cast<int64_t>(mn)
                       ) + 1ULL;
    static const uint64_t MAX_U = 10'000'000ULL;
    return {U <= MAX_U, U};
}

// ════════════════════════════════════════════════════════════════════════════════
//  ALGORITHM 1 — DialSort (sequential)
//  Passes: 2  |  Memory: O(U)  |  Prefix sum: NONE
// ════════════════════════════════════════════════════════════════════════════════
static bool dialsort(std::vector<int>& a) {
    const size_t n = a.size();
    if (n <= 1) return true;

    int mn = a[0], mx = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
    }

    auto [ok, U64] = universe_size(mn, mx);
    if (!ok) { std::cerr << "[WARN] dialsort: U > MAX_U\n"; return false; }
    const size_t U = static_cast<size_t>(U64);

    // Pass 1 — Ingestion
    std::vector<int> H(U, 0);
    for (size_t i = 0; i < n; ++i)
        H[static_cast<size_t>(a[i] - mn)]++;

    // Pass 2 — Projection (in-place, no prefix sum)
    size_t out = 0;
    for (size_t y = 0; y < U; ++y) {
        const int val = static_cast<int>(y) + mn;
        for (int c = H[y]; c > 0; --c)
            a[out++] = val;
    }
    return true;
}

static const int NUM_THREADS = std::thread::hardware_concurrency();
// ════════════════════════════════════════════════════════════════════════════════
//  ALGORITHM 2 — DialSort-Parallel (multi-threaded ingestion)
//  Each thread builds a private partial histogram; merge step is additive.
//  Projection is sequential (single geometric scan of H).
// ════════════════════════════════════════════════════════════════════════════════
static bool dialsort_parallel(std::vector<int>& a, int nthreads = NUM_THREADS) {
    const size_t n = a.size();
    if (n <= 1) return true;

    int mn = a[0], mx = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
    }

    auto [ok, U64] = universe_size(mn, mx);
    if (!ok) { std::cerr << "[WARN] dialsort_parallel: U > MAX_U\n"; return false; }
    const size_t U = static_cast<size_t>(U64);

    const int hw = static_cast<int>(std::thread::hardware_concurrency());
    const int nt = std::max(1, std::min(nthreads, hw > 0 ? hw : nthreads));

    std::vector<std::vector<int>> local_H(nt, std::vector<int>(U, 0));

    std::vector<std::thread> workers;
    const size_t chunk = (n + nt - 1) / nt;
    for (int t = 0; t < nt; ++t) {
        workers.emplace_back([&, t]() {
            const size_t lo = t * chunk;
            const size_t hi = std::min(lo + chunk, n);
            auto& lh = local_H[t];
            for (size_t i = lo; i < hi; ++i)
                lh[static_cast<size_t>(a[i] - mn)]++;
        });
    }
    for (auto& w : workers) w.join();

    std::vector<int> H(U, 0);
    for (int t = 0; t < nt; ++t)
        for (size_t y = 0; y < U; ++y)
            H[y] += local_H[t][y];

    size_t out = 0;
    for (size_t y = 0; y < U; ++y) {
        const int val = static_cast<int>(y) + mn;
        for (int c = H[y]; c > 0; --c)
            a[out++] = val;
    }
    return true;
}

// ════════════════════════════════════════════════════════════════════════════════
//  DATA GENERATORS
// ════════════════════════════════════════════════════════════════════════════════
static std::vector<int> gen_uniform(size_t n, int U, uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> d(0, U - 1);
    std::vector<int> a(n);
    for (auto& x : a) x = d(rng);
    return a;
}

static std::vector<int> gen_skewed(size_t n, int U, uint64_t seed) {
    std::mt19937_64 rng(seed);
    const int hot_limit = std::max(1, U / 20);
    std::uniform_int_distribution<int> hot(0, hot_limit - 1);
    std::uniform_int_distribution<int> cold(0, U - 1);
    std::bernoulli_distribution pick_hot(0.80);
    std::vector<int> a(n);
    for (auto& x : a) x = pick_hot(rng) ? hot(rng) : cold(rng);
    return a;
}

static std::vector<int> gen_sorted(size_t n, int U, uint64_t seed) {
    auto a = gen_uniform(n, U, seed);
    std::sort(a.begin(), a.end());
    return a;
}

static std::vector<int> gen_reverse(size_t n, int U, uint64_t seed) {
    auto a = gen_sorted(n, U, seed);
    std::reverse(a.begin(), a.end());
    return a;
}
