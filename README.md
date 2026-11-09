<h1>CineMatch: A Preference-Based Movie Discovery System</h1>COP3530 — Project 2
Team: Bruno Kolombia and Michele Hirn
Repository: https://github.com/BrunoKolombia/project-2
<h2>Table of Contents</h2>- Overview
- Requirements
- Get the Project and Dataset
- Build and Run from a Terminal
  - Run in CLion
- Using CineMatch
- Algorithms and Performance
- Files
- Troubleshooting
- Dataset Credit
<h2>Overview</h2>CineMatch is a menu-driven C++ console application that helps users discover movies based on their favorite titles and preferences. It uses the MovieLens 1M dataset and implements two recommendation approaches from scratch:
- Content-based filtering: ranks movies using matching genres and average movie ratings.
- Item-based collaborative filtering: finds movies with similar historical user-rating patterns, using cosine similarity.
The application also allows users to search for movies, select and manage favorites, set filters, view recommendation explanations, compare execution times, and run a dataset-size benchmark. No graphical window or user account is required; input and results appear in the terminal or CLion Run console.
<h2>Requirements</h2>- A C++ compiler supporting C++20 (such as Clang or GCC)
- CMake 3.20 or newer
- The MovieLens 1M dataset (movies.dat and ratings.dat)
- Optional: CLion, or another IDE capable of opening a CMake project
No external C++ recommendation or GUI libraries are needed.
<h2>Get the Project and Dataset</h2>
Clone the repository:
git clone https://github.com/BrunoKolombia/project-2.git
cd project-2
Download and extract MovieLens 1M from https://grouplens.org/datasets/movielens/1m/ . Ensure the extracted files are placed in the movielens_data directory at the root of this project:
project-2/
├── CMakeLists.txt
├── main.cpp
├── movies.cpp
├── movies.h
├── recommendations.cpp
├── recommendations.h
├── interface.h
├── WIREFRAMES.md
└── movielens_data/
    ├── movies.dat
    ├── ratings.dat
    └── users.dat       # Included in MovieLens 1M; not required by CineMatch
Important: If extraction creates an extra ml-1m directory, move movies.dat and ratings.dat out of that directory and directly into movielens_data/. The program reads these two files using relative paths (../movielens_data/movies.dat and ../movielens_data/ratings.dat), so its working directory must be a build directory immediately inside project-2/, as shown below.
<h2>Build and Run from a Terminal</h2>
From the project-2 repository directory, run:
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
cd cmake-build-debug
./CineMatch
On Windows, the executable may be called CineMatch.exe and may be under a configuration subfolder (for example, Debug/). Run it with its working directory set to cmake-build-debug so the dataset paths resolve correctly.
Run in CLion
1. Open the project-2 folder in CLion as a CMake project.
2. Allow CLion to load/reload CMakeLists.txt and configure its C++ toolchain.
3. Select the CineMatch CMake run configuration.
4. Under Run → Edit Configurations, verify that the Working directory is the project's build directory (normally cmake-build-debug), not the source root.
5. Click Run. The program appears in CLion's Run console, not a separate GUI window.
If CLion does not identify the project, check that you opened the top-level folder containing CMakeLists.txt, then reload CMake.
<h2>Using CineMatch</h2>
At startup, CineMatch loads the movie and rating files, calculates average movie ratings, and displays a sample movie and rating as a data-loading test. It then asks you to search for a title, view a matching movie's details, and select favorite genres. For genre selection, enter one genre number at a time and 0 when finished. After this initial demonstration, the interactive menu appears.
Menu option	Function
1	Search for a title and add a movie to favorites
2	Update preferred genres
3	Set minimum average rating and release-year filters
4	Generate content-based recommendations
5	Generate item-based collaborative recommendations
6	Compare recommendation results and performance
7	View favorite movies
8	Benchmark performance with different rating-data sizes
9	Remove a movie from favorites
10	Reset favorites and preferences
0	Exit CineMatch


Suggested test run: Search for Toy Story, use menu option 1 to add it as a favorite (the initial title search only displays movie details), select genres, set a minimum rating of 3 and a release-year range of 1990–2000, then try options 4, 5, 6, and 8. Collaborative filtering requires at least one favorite movie to be selected.
<h2>Algorithms and Performance</h2>
Content-based filtering scores eligible movies based on genre overlap and average rating and ranks the results. Item-based collaborative filtering uses historical ratings of co-rated movies to calculate cosine similarities and rank candidates. Both apply the selected preference filters and exclude favorited movies from recommendations.

The comparison feature displays both recommendation lists and runtime measurements. The detailed comparison reports statistics from repeated runs, recommendation counts, average recommended-movie ratings, and overlap between results. The dataset-size benchmark tests different portions of historical ratings (25%, 50%, and 100%) to examine execution-time scaling. These measurements evaluate runtime and output characteristics, not validated recommendation accuracy.
<h2>Files</h2>File	Purpose
main.cpp	Startup tests, input handling, interactive menu, and feature selection
movies.h, movies.cpp	Movie/rating data structures, dataset loading, search, and movie information
recommendations.h, recommendations.cpp	Preference structures and both recommendation algorithms
interface.h	Interface helpers, performance comparisons, and benchmarks
CMakeLists.txt	CMake build configuration for CineMatch
WIREFRAMES.md	Terminal-interface wireframes
movielens_data/	Location of the MovieLens input files


<h2>Troubleshooting</h2>- Could not open movie file / Could not open rating file: Check that the .dat files are directly inside movielens_data/ and that the run configuration's working directory is cmake-build-debug (or another direct child build directory).
- No CineMatch run configuration: Reopen the repository root as a CMake project and reload CMake.
- No collaborative recommendations: Add at least one movie to favorites and try broader genre, year, or minimum-rating filters.
- Unexpected input when selecting genres: Enter individual numeric selections followed by 0, rather than a comma-separated list.
  <h2>Dataset Credit</h2>MovieLens 1M is provided by GroupLens Research. Refer to the dataset's own terms and citation guidance when distributing or using the data.