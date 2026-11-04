#include "recommendations.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace {
// Check filters. An empty genre list means any genre is allowed.
bool passesFilters(const Movie& movie, const Preferences& prefs) {
    if (movie.averageRating < prefs.minRating ||
        movie.releaseYear < prefs.minYear ||
        movie.releaseYear > prefs.maxYear) return false;
    if (prefs.genres.empty()) return true;
    for (const string& genre : movie.genres) {
        if (find(prefs.genres.begin(), prefs.genres.end(), genre) != prefs.genres.end())
            return true;
    }
    return false;
}

void rankAndLimit(vector<Recommendation>& results, size_t limit) {
    sort(results.begin(), results.end(), [](const Recommendation& a,
                                            const Recommendation& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.movieID < b.movieID;
    });
    if (results.size() > limit) results.resize(limit);
}

struct SimilarityStats {
    double dot = 0.0;
    double leftSquare = 0.0;
    double rightSquare = 0.0;
    int common = 0;
};
}

vector<Recommendation> contentBasedRecommendations(
    const vector<Movie>& movies, const Preferences& prefs,
    const vector<int>& favoriteIDs, size_t limit) {

    unordered_set<int> favorites(favoriteIDs.begin(), favoriteIDs.end());
    unordered_set<string> desiredGenres(prefs.genres.begin(), prefs.genres.end());
    for (const Movie& movie : movies) {
        if (favorites.count(movie.movieID)) {
            desiredGenres.insert(movie.genres.begin(), movie.genres.end());
        }
    }

    vector<Recommendation> results;
    for (const Movie& movie : movies) {
        if (favorites.count(movie.movieID) || !passesFilters(movie, prefs)) continue;

        int matches = 0;
        for (const string& genre : movie.genres) {
            if (desiredGenres.count(genre)) ++matches;
        }
        double genreScore = desiredGenres.empty() ? 0.0 :
            static_cast<double>(matches) / desiredGenres.size();
        double ratingScore = movie.averageRating / 5.0;
        // Give genre matches the most weight; ratings break close matches.
        double score = desiredGenres.empty() ? ratingScore :
            0.75 * genreScore + 0.25 * ratingScore;

        string reason = to_string(matches) + " matching genre(s); average rating " +
                        to_string(movie.averageRating);
        results.push_back({movie.movieID, score, reason});
    }
    rankAndLimit(results, limit);
    return results;
}

vector<Recommendation> collaborativeRecommendations(
    const vector<Movie>& movies, const vector<Rating>& ratings,
    const Preferences& prefs, const vector<int>& favoriteIDs, size_t limit) {

    if (favoriteIDs.empty()) return {};

    unordered_set<int> favorites(favoriteIDs.begin(), favoriteIDs.end());
    unordered_map<int, const Movie*> byID;
    for (const Movie& movie : movies) byID[movie.movieID] = &movie;

    // Two indexes built from the MovieLens historical rating data.
    unordered_map<int, vector<pair<int, double>>> byUser;
    unordered_map<int, vector<pair<int, double>>> byMovie;
    for (const Rating& rating : ratings) {
        byUser[rating.userID].push_back({rating.movieID, rating.rating});
        byMovie[rating.movieID].push_back({rating.userID, rating.rating});
    }

    unordered_map<int, double> totalSimilarity;
    unordered_map<int, int> favoriteMatches;

    for (int favoriteID : favorites) {
        auto favoriteIt = byMovie.find(favoriteID);
        if (favoriteIt == byMovie.end()) continue;

        unordered_map<int, SimilarityStats> stats;
        // For each person who rated a favorite movie, examine other movies
        // that same person rated. Compare ratings on the shared users.
        for (const auto& [userID, favoriteRating] : favoriteIt->second) {
            const auto& userRatings = byUser.at(userID);
            for (const auto& [candidateID, candidateRating] : userRatings) {
                if (favorites.count(candidateID)) continue;
                auto movieIt = byID.find(candidateID);
                if (movieIt == byID.end() || !passesFilters(*movieIt->second, prefs))
                    continue;
                auto& s = stats[candidateID];
                s.dot += favoriteRating * candidateRating;
                s.leftSquare += favoriteRating * favoriteRating;
                s.rightSquare += candidateRating * candidateRating;
                ++s.common;
            }
        }

        for (const auto& [candidateID, s] : stats) {
            if (s.common < 3) continue; // Avoid one-person coincidences.
            double denominator = sqrt(s.leftSquare * s.rightSquare);
            if (denominator == 0.0) continue;
            double cosine = s.dot / denominator;
            // Penalize pairs with very few shared raters.
            double similarity = cosine * s.common / (s.common + 10.0);
            totalSimilarity[candidateID] += similarity;
            ++favoriteMatches[candidateID];
        }
    }

    vector<Recommendation> results;
    for (const auto& [movieID, score] : totalSimilarity) {
        // More matching favorites should increase the final score.
        string reason = "Similar rating patterns to " +
                        to_string(favoriteMatches[movieID]) + " favorite movie(s)";
        results.push_back({movieID, score, reason});
    }
    rankAndLimit(results, limit);
    return results;
}