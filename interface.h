#ifndef CINEMATCH_INTERFACE_EXTRAS_H
#define CINEMATCH_INTERFACE_EXTRAS_H


#include "recommendations.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct CineTiming {
    double medianMs = 0.0;
    double fastestMs = 0.0;
    double slowestMs = 0.0;
};

// one unmeasured warm-up, followed by three measured algorithm calls.
// timing excludes terminal output and dataset loading.
template <typename Callable>
CineTiming cineTimeAlgorithm(Callable run) {
    run(); // warm-up
    std::vector<double> times;
    times.reserve(3);
    for (int i = 0; i < 3; ++i) {
        const auto start = std::chrono::steady_clock::now();
        const auto result = run();
        const auto end = std::chrono::steady_clock::now();
        times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
        // Explicitly consume the result so this is a real algorithm call.
        if (result.size() == std::numeric_limits<std::size_t>::max())
            std::cout << "Unreachable result size\n";
    }
    std::sort(times.begin(), times.end());
    return {times[1], times.front(), times.back()};
}

inline double cineAverageRating(
    const std::vector<Recommendation>& recommendations,
    const std::unordered_map<int, const Movie*>& movieLookup) {
    double sum = 0.0;
    std::size_t count = 0;
    for (const auto& rec : recommendations) {
        const auto found = movieLookup.find(rec.movieID);
        if (found == movieLookup.end()) continue;
        sum += found->second->averageRating;
        ++count;
    }
    return count ? sum / static_cast<double>(count) : 0.0;
}

inline std::size_t cineCommonMovies(
    const std::vector<Recommendation>& left,
    const std::vector<Recommendation>& right) {
    std::unordered_set<int> ids;
    for (const auto& rec : left) ids.insert(rec.movieID);
    std::size_t common = 0;
    for (const auto& rec : right) if (ids.count(rec.movieID)) ++common;
    return common;
}

inline void cinePrintPreferences(const Preferences& preferences,
                                 const std::vector<int>& favoriteIDs) {
    std::cout << "\nCurrent selections: " << favoriteIDs.size() << " favorite movie(s) | Genres: ";
    if (preferences.genres.empty()) std::cout << "Any";
    for (std::size_t i = 0; i < preferences.genres.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << preferences.genres[i];
    }
    std::cout << "\nMinimum rating: " << preferences.minRating
              << " | Release years: " << preferences.minYear
              << " - " << preferences.maxYear << "\n";
}

inline void cinePrintDetailedComparison(
    const std::vector<Movie>& movies,
    const std::vector<Rating>& ratings,
    const Preferences& preferences,
    const std::vector<int>& favoriteIDs,
    const std::vector<Recommendation>& contentResults,
    const std::vector<Recommendation>& collaborativeResults,
    const std::unordered_map<int, const Movie*>& movieLookup) {

    CineTiming content = cineTimeAlgorithm([&] {
        return contentBasedRecommendations(movies, preferences, favoriteIDs);
    });
    CineTiming collaborative = cineTimeAlgorithm([&] {
        return collaborativeRecommendations(movies, ratings, preferences, favoriteIDs);
    });

    std::cout << "\n========== DETAILED ALGORITHM COMPARISON ==========\n"
              << "(1 warm-up + 3 timed runs per algorithm)\n"
              << std::left << std::setw(27) << "Metric"
              << std::right << std::setw(16) << "Content-Based"
              << std::setw(18) << "Collaborative" << '\n'
              << std::string(61, '-') << '\n'
              << std::fixed << std::setprecision(3);
    auto line = [](const std::string& name, double a, double b) {
        std::cout << std::left << std::setw(27) << name
                  << std::right << std::setw(16) << a
                  << std::setw(18) << b << '\n';
    };
    line("Median time (ms)", content.medianMs, collaborative.medianMs);
    line("Fastest time (ms)", content.fastestMs, collaborative.fastestMs);
    line("Slowest time (ms)", content.slowestMs, collaborative.slowestMs);
    line("Results returned", static_cast<double>(contentResults.size()),
         static_cast<double>(collaborativeResults.size()));
    line("Mean movie rating", cineAverageRating(contentResults, movieLookup),
         cineAverageRating(collaborativeResults, movieLookup));
    std::cout << "Common recommendations: "
              << cineCommonMovies(contentResults, collaborativeResults) << '\n';
    std::cout << "Faster median: "
              << (content.medianMs < collaborative.medianMs ? "Content-Based" :
                  collaborative.medianMs < content.medianMs ? "Collaborative" : "Tie")
              << "\n(Mean rating and overlap do not measure recommendation accuracy.)\n";
}

inline void cineBenchmarkDatasetSizes(
    const std::vector<Movie>& movies,
    const std::vector<Rating>& ratings,
    const Preferences& preferences,
    const std::vector<int>& favoriteIDs) {
    if (favoriteIDs.empty()) {
        std::cout << "Choose at least one favorite movie before benchmarking.\n";
        return;
    }
    std::cout << "\n===== RATING DATASET SCALING BENCHMARK =====\n";
    std::cout << "Note: Content-Based does not use individual ratings at recommendation time.\n"
              << "The same movie list / average ratings are used for all percentages.\n"
              << "Only the collaborative input's historical rating count changes.\n";
    std::cout << std::left << std::setw(12) << "Ratings %"
              << std::right << std::setw(14) << "Count"
              << std::setw(19) << "Content med ms"
              << std::setw(19) << "Collab med ms" << '\n';
    for (int pct : {25, 50, 100}) {
        std::size_t count = ratings.size() * static_cast<std::size_t>(pct) / 100;
        std::vector<Rating> sample(ratings.begin(), ratings.begin() + count);
        CineTiming a = cineTimeAlgorithm([&] {
            return contentBasedRecommendations(movies, preferences, favoriteIDs);
        });
        CineTiming b = cineTimeAlgorithm([&] {
            return collaborativeRecommendations(movies, sample, preferences, favoriteIDs);
        });
        std::cout << std::left << std::setw(12) << (std::to_string(pct) + "%")
                  << std::right << std::setw(14) << count
                  << std::fixed << std::setprecision(3)
                  << std::setw(19) << a.medianMs
                  << std::setw(19) << b.medianMs << '\n';
    }
    std::cout << "Rows reflect a deterministic prefix of ratings, not a randomized sample.\n";
}

#endif