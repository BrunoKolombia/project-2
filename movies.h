//
// Created by mshir on 11/2/2026.
//
#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>
using namespace std;

// stores movie information
struct Movie {
    int movieID;
    string title;
    int releaseYear;
    vector<string> genres;
    double averageRating;
};

// stores individual user ratings
struct Rating {
    int userID;
    int movieID;
    double rating;
    long timestamp;
};

// function declarations
void loadMovies(vector<Movie>& movies);
void loadRatings(vector<Rating>& ratings);
void calculateAverageRatings(vector<Movie>& movies, const vector<Rating>& ratings);
vector<Movie> searchMovies(const vector<Movie>& movies, const string& searchTerm);
#endif
