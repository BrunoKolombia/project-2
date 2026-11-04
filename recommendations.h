#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include "movies.h"
#include <cstddef>
#include <string>
#include <vector>

struct Preferences {
    std::vector<std::string> genres;
    double minRating = 0.0;
    int minYear = 0;
    int maxYear = 9999;
};

struct Recommendation {
    int movieID;
    double score;
    std::string reason;
};

std::vector<Recommendation> contentBasedRecommendations(
    const std::vector<Movie>& movies,
    const Preferences& preferences,
    const std::vector<int>& favoriteIDs,
    std::size_t limit = 10);

std::vector<Recommendation> collaborativeRecommendations(
    const std::vector<Movie>& movies,
    const std::vector<Rating>& ratings,
    const Preferences& preferences,
    const std::vector<int>& favoriteIDs,
    std::size_t limit = 10);

#endif