#include <bits/stdc++.h>
using namespace std;

// Define a struct to represent a point with x and y coordinates.
struct Point {
    int x, y;
};

int n, x, y;
int indexMaxPointX, indexMaxPointY;
int isUpX, isUpY; // Declare variables to store whether the x and y coordinates are increasing or decreasing.
int countChangeX, countChangeY; // Declare variables to store the number of times the x and y coordinates change direction.
Point maxPointX, maxPointY; // Declare variables to store the points with the maximum x and y coordinates.

vector < Point > points; // Declare a vector to store the points.

int main() {
    cin >> n; // Read in the number of points.

    // Loop through each point, read in its x and y coordinates, and store it in the vector.
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        points.push_back({x, y});

        // If the x coordinate of this point is greater than the current maximum x coordinate, update the maxPointX variable and indexMaxPointX variable.
        if (points[i].x > maxPointX.x) {
            maxPointX = points[i];
            indexMaxPointX = i;
        }
        // If the y coordinate of this point is greater than the current maximum y coordinate, update the maxPointY variable and indexMaxPointY variable.
        if (points[i].y > maxPointY.y) {
            maxPointY = points[i];
            indexMaxPointY = i;
        }
    }

    // Loop through the points starting at the index of the point with the maximum x coordinate, and check whether the x coordinates are increasing or decreasing.
    for (int i = indexMaxPointX; i < n - 1; ++i) {
        if (points[i].x < points[i + 1].x) {
            if (isUpX == 0) {
                isUpX = 1; // The x coordinates are increasing.
            } else if (isUpX == -1) {
                isUpX = 1; // The x coordinates have changed direction, so update the isUpX variable and increment the countChangeX variable.
                countChangeX++;
            }
        } else if (points[i].x > points[i + 1].x) {
            if (isUpX == 0) {
                isUpX = -1; // The x coordinates are decreasing.
            } else if (isUpX == 1) {
                isUpX = -1; // The x coordinates have changed direction, so update the isUpX variable and increment the countChangeX variable.
                countChangeX++;
            }
        }
    }

    for (int i = 1; i < indexMaxPointX; ++i) {
        if (points[i].x < points[i + 1].x) {
            if (isUpX == 0) {
                isUpX = 1;
            } else if (isUpX == -1) {
                isUpX = 1;
                countChangeX++;
            }
        } else if (points[i].x > points[i + 1].x) {
            if (isUpX == 0) {
                isUpX = -1;
            } else if (isUpX == 1) {
                isUpX = -1;
                countChangeX++;
            }

        }
    }

    for (int i = indexMaxPointY; i < n - 1; ++i) {
        if (points[i].y < points[i + 1].y) {
            if (isUpY == 0) {
                isUpY = 1;
            } else if (isUpY == -1) {
                isUpY = 1;
                countChangeY++;
            }
        } else if (points[i].y > points[i + 1].y) {
            if (isUpY == 0) {
                isUpY = -1;
            } else if (isUpY == 1) {
                isUpY = -1;
                countChangeY++;
            }

        }
    }

    for (int i = 1; i < indexMaxPointY; ++i) {
        if (points[i].y < points[i + 1].y) {
            if (isUpY == 0) {
                isUpY = 1;
            } else if (isUpY == -1) {
                isUpY = 1;
                countChangeY++;
            }
        } else if (points[i].y > points[i + 1].y) {
            if (isUpY == 0) {
                isUpY = -1;
            } else if (isUpY == 1) {
                isUpY = -1;
                countChangeY++;
            }

        }
    }

    countChangeX >= 2 ? cout << "NO\n" : cout << "YES\n";
    countChangeY >= 2 ? cout << "NO\n" : cout << "YES\n";
}
