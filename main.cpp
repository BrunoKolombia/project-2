//
// Created by mshir on 10/25/2026.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Movie {
    int movieID;
    string title;
    int releaseYear;
    vector<string> genres;
    double averageRating;
};

int main() {
    // creates a vector to store the movie data
    vector<Movie> movies;
    // reads in the movie data
    ifstream movie_data("movielens_data/movies.dat");

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

    // reads in the rating data
    ifstream rating_data("movielens_data/ratings.dat");

    if (rating_data.is_open()) {
        string line;

        while (getline(rating_data, line)) {
            size_t first = line.find("::");
            size_t second = line.find("::", first + 2);
            size_t third = line.find("::", second + 2);

            string userID = line.substr(0, first);
            string movieID = line.substr(first + 2, second - first - 2);
            string rating = line.substr(second + 2, third - second - 2);
        }

        rating_data.close();
    } else {
        cout << "Could not open rating file" << endl;
    }

    return 0;
}