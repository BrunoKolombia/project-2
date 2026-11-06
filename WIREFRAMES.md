# CineMatch — terminal interface wireframes (existing menu + additive improvements)

These wireframes use the current 1–7 menu as the starting point, with new options 8–10. Labels, ratings and timings are placeholders, not real MovieLens output.

## Screen 1 — menu and saved preferences

```text
Current selections: 2 favorite movie(s) | Genres: Animation, Comedy
Minimum rating: 3.5 | Release years: 1990 - 2005

========== CINEMATCH ==========
1. Search and add favorite movies
2. Update favorite genres
3. Set rating and year preferences
4. Content-Based Recommendations
5. Collaborative Filtering Recommendations
6. Compare Both Algorithms
7. View Favorite Movies
8. Benchmark Dataset Sizes
9. Remove a Favorite Movie
10. Reset Favorites and Preferences
0. Exit
================================
Enter your choice: _
```

## Screen 2 — movie search (unchanged from existing option 1)

```text
Enter a movie title: Toy Story
Search Results:
1. Toy Story (1995)

Select a movie to add (0 to cancel): 1
Toy Story added to favorites!
```

## Screen 3 — recommendation results (unchanged from options 4 and 5)

```text
Running Content-Based Filtering...
Recommended Movies:
1. [Title] ([Release Year])
Average Rating: [0.00–5.00]
Recommendation Score: [score]
Reason: [matched genres and/or rating patterns]
...
Execution Time: [milliseconds] ms
```

## Screen 4 — enhanced comparison (appended to option 6)

```text
========== DETAILED ALGORITHM COMPARISON ==========
(1 warm-up + 3 timed runs per algorithm)
Metric                      Content-Based     Collaborative
-------------------------------------------------------------
Median time (ms)                   [ms]               [ms]
Fastest time (ms)                  [ms]               [ms]
Slowest time (ms)                  [ms]               [ms]
Results returned                   [n]                [n]
Mean movie rating                  [avg]              [avg]
Common recommendations: [n]
Faster median: [algorithm]
(Mean rating and overlap do not measure recommendation accuracy.)
```

## Screen 5 — scaling test (new option 8)

```text
===== RATING DATASET SCALING BENCHMARK =====
Ratings %           Count    Content med ms    Collab med ms
25%                [count]           [ms]              [ms]
50%                [count]           [ms]              [ms]
100%               [count]           [ms]              [ms]
```

## Screen 6 — remove favorite (new option 9)

```text
Selected favorite movies:
1. Toy Story
2. Jumanji
Enter number to remove (0 to cancel): 2
Favorite removed.
```

## Screen 7 — reset confirmation (new option 10)

```text
Reset everything? 1 = Yes, 0 = No: 1
Favorites and preferences reset.
```

## Navigation

```text
Original start-up data/search demonstration
                |
                v
       Existing CineMatch menu
          /    |     \
   Favorites  Filters  Recommendations
     |        |             |
     +--------+-------------+
                |
         Compare algorithms
                |
        Detailed timing table
                |
       Return to main menu
```
