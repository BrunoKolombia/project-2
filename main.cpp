#include <iostream>
#include "movies.h"
#include "recommendations.h"
#include <chrono>
#include <iomanip>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "interface.h"

using namespace std;

int main() {
    // creates vectors to store the movie and rating data
    vector<Movie> movies;
    vector<Rating> ratings;

    // loads the movie and rating data
    loadMovies(movies);
    loadRatings(ratings);

    // calculates the average ratings for each movie
    calculateAverageRatings(movies, ratings);

    // tests data loading by displaying the first movie
    if (!movies.empty()) {
        cout << "\nFirst Movie Loaded:" << endl;
        cout << "Movie ID: " << movies[0].movieID << endl;
        cout << "Title: " << movies[0].title << endl;
        cout << "Release Year: " << movies[0].releaseYear << endl;

        cout << "Genres: ";
        for (string genre : movies[0].genres) {
            cout << genre << " ";
        }
        cout << endl;
        cout << "Average Rating: " << movies[0].averageRating << endl;
    }

    // tests data loading by displaying the first rating
    if (!ratings.empty()) {
        cout << "\nFirst Rating Loaded:" << endl;
        cout << "User ID: " << ratings[0].userID << endl;
        cout << "Movie ID: " << ratings[0].movieID << endl;
        cout << "Rating: " << ratings[0].rating << endl;
        cout << "Timestamp: " << ratings[0].timestamp << endl;
    }

    // allows users to search for movies
    string searchTerm;

    cout << "\nEnter a movie title to search: ";
    getline(cin, searchTerm);

    // searches for matching movies
    vector<Movie> searchResults = searchMovies(movies, searchTerm);

    // displays the search results
    if (!searchResults.empty()) {
        cout << "\nSearch Results:" << endl;
        for (int i = 0; i < searchResults.size(); i++) {
            cout << i + 1 << ". " << searchResults[i].title
                 << " (" << searchResults[i].releaseYear << ")" << endl;
        }

        // allows users to select a movie
        int selection;

        cout << "\nSelect a movie to view its information: ";
        cin >> selection;
        if (selection >= 1 && selection <= searchResults.size()) {
            displayMovieInfo(searchResults[selection - 1]);
        } else {
            cout << "Invalid selection." << endl;
        }

    } else {
        cout << "No matching movies found." << endl;
    }

    // allows users to select their favorite genres
    vector<string> favoriteGenres = selectFavoriteGenres();

    // displays the user's selected genres
    cout << "\nYour Favorite Genres:" << endl;

    for (const string& genre : favoriteGenres) {
        cout << genre << endl;
    }

    // checks that movie and rating data loaded
    if (movies.empty() || ratings.empty()) {
        cout << "Cannot generate recommendations." << endl;
        return 1;
    }

    // store user preferences
    Preferences preferences;
    preferences.genres = favoriteGenres;

    // store the IDs of the user's favorite movies
    vector<int> favoriteIDs;

    // map movie IDs to their corresponding movies
    // provides fast access to movie information
    unordered_map<int, const Movie*> movieLookup;

    for (const Movie& movie : movies) {
        movieLookup[movie.movieID] = &movie;
    }

    // clear remaining input from earlier selections
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // helper function for reading valid integers
    auto readInteger = [](const string& prompt,
                          int minimum,
                          int maximum) {
        int value;
        string input;

        while (true) {
            cout << prompt;
            if (!getline(cin, input)) {
                return minimum;
            }

            stringstream stream(input);
            char extra;

            if ((stream >> value) &&
                !(stream >> extra) &&
                value >= minimum &&
                value <= maximum) {
                return value;
            }

            cout << "Invalid input. Try again." << endl;
        }
    };

    // helper function for displaying recommendations
    auto displayRecommendations =
        [&](const vector<Recommendation>& results) {

        if (results.empty()) {
            cout << "\nNo recommendations found." << endl;
            return;
        }

        cout << "\nRecommended Movies:" << endl;

        for (int i = 0; i < results.size(); i++) {
            const Recommendation& recommendation = results[i];

            auto it = movieLookup.find(recommendation.movieID);

            if (it == movieLookup.end()) {
                continue;
            }

            const Movie& movie = *(it->second);

            cout << "\n" << i + 1 << ". " << movie.title
                 << " (" << movie.releaseYear << ")" << endl;

            cout << "Average Rating: "
                 << fixed << setprecision(2)
                 << movie.averageRating << endl;

            cout << "Recommendation Score: "
                 << recommendation.score << endl;

            cout << "Reason: "
                 << recommendation.reason << endl;
        }
    };

    // interactive recommendation menu
    int choice = -1;

    while (choice != 0) {
        // Show selected preferences before displaying menu
        cinePrintPreferences(preferences, favoriteIDs);

        cout << "\n========== CINEMATCH ==========" << endl;
        cout << "1. Search and add favorite movies" << endl;
        cout << "2. Update favorite genres" << endl;
        cout << "3. Set rating and year preferences" << endl;
        cout << "4. Content-Based Recommendations" << endl;
        cout << "5. Collaborative Filtering Recommendations" << endl;
        cout << "6. Compare Both Algorithms" << endl;
        cout << "7. View Favorite Movies" << endl;
        cout << "8. Benchmark Dataset Sizes" << endl;
        cout << "9. Remove a Favorite Movie" << endl;
        cout << "10. Reset Favorites and Preferences" << endl;
        cout << "0. Exit" << endl;
        cout << "================================" << endl;

        choice = readInteger("Enter your choice: ", 0, 10);

        // search for and select favorite movies
        if (choice == 1) {

            string title;

            cout << "\nEnter a movie title: ";
            getline(cin, title);

            vector<Movie> results = searchMovies(movies, title);

            if (results.empty()) {
                cout << "No matching movies found." << endl;
                continue;
            }

            cout << "\nSearch Results:" << endl;

            for (int i = 0; i < results.size(); i++) {
                cout << i + 1 << ". " << results[i].title
                     << " (" << results[i].releaseYear << ")"
                     << endl;
            }

            int selection = readInteger(
                "\nSelect a movie to add (0 to cancel): ",
                0,
                static_cast<int>(results.size())
            );

            if (selection == 0) {
                continue;
            }

            int movieID = results[selection - 1].movieID;

            // prevent duplicate favorites
            if (find(favoriteIDs.begin(),
                     favoriteIDs.end(),
                     movieID) == favoriteIDs.end()) {

                favoriteIDs.push_back(movieID);

                cout << results[selection - 1].title
                     << " added to favorites!" << endl;

            } else {
                cout << "Movie is already in favorites." << endl;
            }
        }

        // update selected genres
        else if (choice == 2) {

            preferences.genres = selectFavoriteGenres();

            cout << "\nUpdated Favorite Genres:" << endl;

            for (const string& genre : preferences.genres) {
                cout << genre << endl;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // set rating and release year filters
        else if (choice == 3) {

            preferences.minRating = readInteger(
                "\nMinimum average rating (0-5): ", 0, 5
            );

            preferences.minYear = readInteger(
                "Minimum release year (0-2100): ", 0, 2100
            );

            preferences.maxYear = readInteger(
                "Maximum release year: ",
                preferences.minYear,
                2100
            );

            cout << "\nPreferences updated!" << endl;
        }

        // run content-based filtering
        else if (choice == 4) {

            cout << "\nRunning Content-Based Filtering..."
                 << endl;

            auto start = chrono::steady_clock::now();

            vector<Recommendation> results =
                contentBasedRecommendations(
                    movies,
                    preferences,
                    favoriteIDs
                );

            auto finish = chrono::steady_clock::now();

            displayRecommendations(results);

            double elapsedTime =
                chrono::duration<double, milli>(
                    finish - start
                ).count();

            cout << "\nExecution Time: "
                 << elapsedTime << " ms" << endl;
        }

        // run collaborative filtering
        else if (choice == 5) {

            if (favoriteIDs.empty()) {
                cout << "\nSelect at least one favorite movie first."
                     << endl;
                continue;
            }

            cout << "\nRunning Collaborative Filtering..."
                 << endl;

            auto start = chrono::steady_clock::now();

            vector<Recommendation> results =
                collaborativeRecommendations(
                    movies,
                    ratings,
                    preferences,
                    favoriteIDs
                );

            auto finish = chrono::steady_clock::now();

            displayRecommendations(results);

            double elapsedTime =
                chrono::duration<double, milli>(
                    finish - start
                ).count();

            cout << "\nExecution Time: "
                 << elapsedTime << " ms" << endl;
        }

        // compare both recommendation algorithms
        else if (choice == 6) {

            if (favoriteIDs.empty()) {
                cout << "\nSelect at least one favorite movie first."
                     << endl;
                continue;
            }

            cout << "\n===== ALGORITHM COMPARISON =====" << endl;

            auto start1 = chrono::steady_clock::now();

            vector<Recommendation> contentResults =
                contentBasedRecommendations(
                    movies,
                    preferences,
                    favoriteIDs
                );

            auto finish1 = chrono::steady_clock::now();

            cout << "\nCONTENT-BASED FILTERING" << endl;
            displayRecommendations(contentResults);

            auto start2 = chrono::steady_clock::now();

            vector<Recommendation> collaborativeResults =
                collaborativeRecommendations(
                    movies,
                    ratings,
                    preferences,
                    favoriteIDs
                );

            auto finish2 = chrono::steady_clock::now();

            cout << "\nCOLLABORATIVE FILTERING" << endl;
            displayRecommendations(collaborativeResults);

            double contentTime =
                chrono::duration<double, milli>(
                    finish1 - start1
                ).count();

            double collaborativeTime =
                chrono::duration<double, milli>(
                    finish2 - start2
                ).count();

            cout << "\n===== PERFORMANCE RESULTS =====" << endl;

            cout << "Content-Based: "
                 << contentTime << " ms" << endl;

            cout << "Collaborative Filtering: "
                 << collaborativeTime << " ms" << endl;

            if (contentTime < collaborativeTime) {
                cout << "Content-Based Filtering was faster."
                     << endl;
            } else if (collaborativeTime < contentTime) {
                cout << "Collaborative Filtering was faster."
                     << endl;
            } else {
                cout << "Both algorithms took the same time."
                     << endl;
            }

            // detailed performance metrics and recommendation comparisons
            cinePrintDetailedComparison(
                movies, ratings, preferences, favoriteIDs,
                contentResults, collaborativeResults, movieLookup
            );

        }

        // display currently selected favorite movies
        else if (choice == 7) {

            if (favoriteIDs.empty()) {
                cout << "\nNo favorite movies selected." << endl;
                continue;
            }

            cout << "\nYour Favorite Movies:" << endl;

            for (int movieID : favoriteIDs) {

                auto it = movieLookup.find(movieID);

                if (it != movieLookup.end()) {
                    cout << "- " << it->second->title << endl;
                }
            }
        }
        // benchmark algorithms with different amounts of historical ratings
        else if (choice == 8) {
            cineBenchmarkDatasetSizes(movies, ratings, preferences, favoriteIDs);
        }

        // remove one of the user's selected favorite movies
        else if (choice == 9) {
            if (favoriteIDs.empty()) {
                cout << "No favorite movies to remove." << endl;
                continue;
            }

            cout << "\nSelected Favorite Movies:" << endl;
            for (size_t i = 0; i < favoriteIDs.size(); ++i) {
                auto found = movieLookup.find(favoriteIDs[i]);
                if (found != movieLookup.end()) {
                    cout << i + 1 << ". " << found->second->title << endl;
                }
            }

            int selection = readInteger(
                "Enter number to remove (0 to cancel): ",
                0, static_cast<int>(favoriteIDs.size())
            );
            if (selection != 0) {
                favoriteIDs.erase(favoriteIDs.begin() + selection - 1);
                cout << "Favorite removed." << endl;
            }
        }

        // reset saved preferences and favorites after confirmation
        else if (choice == 10) {
            int confirm = readInteger(
                "Reset everything? 1 = Yes, 0 = No: ", 0, 1
            );
            if (confirm == 1) {
                favoriteIDs.clear();
                preferences = Preferences{};
                cout << "Favorites and preferences reset." << endl;
            }
        }

        else if (choice == 0) {
            cout << "\nThank you for using CineMatch!"
                 << endl;
        }
    }

    return 0;
}