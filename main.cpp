
#include <iostream>
#include "movies.h"
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

        for (const Movie& movie : searchResults) {
            cout << movie.title << " (" << movie.releaseYear << ")" << endl;
        }
    } else {
        cout << "No matching movies found." << endl;
    }

    return 0;
}
