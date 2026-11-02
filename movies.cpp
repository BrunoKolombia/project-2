
#include "movies.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cctype>
using namespace std;

// loads movie information from movies.dat
void loadMovies(vector<Movie>& movies) {
    ifstream movie_data("../movielens_data/movies.dat");

    if (movie_data.is_open()) {
        string line;

        while (getline(movie_data, line)) {
            // separates the movie fields
            size_t first = line.find("::");
            size_t second = line.find("::", first + 2);

            string movieID = line.substr(0, first);
            string title = line.substr(first + 2, second - first - 2);
            string genresText = line.substr(second + 2);

            // separates the movie title and release year
            size_t yearStart = title.rfind('(');
            size_t yearEnd = title.rfind(')');

            string year = "";
            string movieTitle = title;

            if (yearStart != string::npos && yearEnd != string::npos) {
                year = title.substr(yearStart + 1, yearEnd - yearStart - 1);
                movieTitle = title.substr(0, yearStart - 1);
            }

            // creates a Movie object
            Movie movie;
            movie.movieID = stoi(movieID);
            movie.title = movieTitle;
            movie.releaseYear = year.empty() ? 0 : stoi(year);
            movie.averageRating = 0.0;

            // separates the genres
            size_t start = 0;
            size_t end;

            while ((end = genresText.find('|', start)) != string::npos) {
                movie.genres.push_back(genresText.substr(start, end - start));
                start = end + 1;
            }

            // stores the final genre
            movie.genres.push_back(genresText.substr(start));

            // stores the movie in the vector
            movies.push_back(movie);
        }

        movie_data.close();
        cout << "Movies stored: " << movies.size() << endl;
    } else {
        cout << "Could not open movie file" << endl;
    }
}

// loads rating information from ratings.dat
void loadRatings(vector<Rating>& ratings) {
    ifstream rating_data("../movielens_data/ratings.dat");

    if (rating_data.is_open()) {
        string line;

        while (getline(rating_data, line)) {
            // separates the rating fields
            size_t first = line.find("::");
            size_t second = line.find("::", first + 2);
            size_t third = line.find("::", second + 2);

            string userID = line.substr(0, first);
            string movieID = line.substr(first + 2, second - first - 2);
            string rating = line.substr(second + 2, third - second - 2);
            string timestamp = line.substr(third + 2);

            // creates a Rating object
            Rating individualRating;
            individualRating.userID = stoi(userID);
            individualRating.movieID = stoi(movieID);
            individualRating.rating = stod(rating);
            individualRating.timestamp = stol(timestamp);

            // stores the rating in the vector
            ratings.push_back(individualRating);
        }

        rating_data.close();
        cout << "Ratings stored: " << ratings.size() << endl;
    } else {
        cout << "Could not open rating file" << endl;
    }
}

// calculates the average rating for each movie
void calculateAverageRatings(vector<Movie>& movies, const vector<Rating>& ratings) {
    // organizes the ratings by movie ID
    unordered_map<int, double> totalRatings;
    unordered_map<int, int> ratingCounts;

    // calculates the total ratings and number of ratings for each movie
    for (const Rating& rating : ratings) {
        totalRatings[rating.movieID] += rating.rating;
        ratingCounts[rating.movieID]++;
    }

    // calculates the average rating for each movie
    for (Movie& movie : movies) {
        if (ratingCounts[movie.movieID] > 0) {
            movie.averageRating = totalRatings[movie.movieID] / ratingCounts[movie.movieID];
        }
    }
}
// searches for movies by title
vector<Movie> searchMovies(const vector<Movie>& movies, const string& searchTerm) {
    vector<Movie> searchResults;

    // converts the search term to lowercase
    string lowerSearchTerm = searchTerm;

    for (char& character : lowerSearchTerm) {
        character = tolower(static_cast<unsigned char>(character));
    }

    // checks each movie title for a match
    for (const Movie& movie : movies) {
        string lowerTitle = movie.title;

        for (char& character : lowerTitle) {
            character = tolower(static_cast<unsigned char>(character));
        }

        // stores movies that contain the search term
        if (lowerTitle.find(lowerSearchTerm) != string::npos) {
            searchResults.push_back(movie);
        }
    }

    return searchResults;
}