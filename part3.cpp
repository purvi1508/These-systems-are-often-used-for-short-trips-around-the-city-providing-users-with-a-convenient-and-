#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <time.h>
#include <ctime>
#include <chrono>
#include <set>
#include <cmath>

using namespace std;

#define PI 3.14159265358979323846
#define EARTH_RADIUS_METERS 6372797.56085
#define DEGREES_TO_RADIANS PI / 180
#define RADIANS_TO_DEGREES 180 / PI

double calcGPSDistance(double lat1, double lon1, double lat2, double lon2)
{
    double lat_new = lat2 * DEGREES_TO_RADIANS;
    double lat_old = lat1 * DEGREES_TO_RADIANS;
    double lat_diff = (lat1 - lat2) * DEGREES_TO_RADIANS;
    double lng_diff = (lon1 - lon2) * DEGREES_TO_RADIANS;

    double a = sin(lat_diff / 2) * sin(lat_diff / 2) +
               cos(lat_new) * cos(lat_old) *
                   sin(lng_diff / 2) * sin(lng_diff / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    double distance = EARTH_RADIUS_METERS * c;

    return distance;
}

vector<string> split(const string &s, char delim)
{
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }
    return result;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream inputFile("bike_data_new.csv");
    if (!inputFile.is_open())
    {
        std::cerr << "Error: File didn't open" << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(inputFile, line); // Skips Header
    double maxDuration = 0;
    double minDuration = 100000000;
    vector<int> durations;
    set<pair<string, string>> locationMap;
    int idx = 0;
    while (getline(inputFile, line))
    {
        line = line.substr(0, line.size() - 1);
        vector<string> res = split(line, ',');
        if (res[0] == "101")
            break;
        locationMap.insert({res[3], res[4]});
    }
    inputFile.clear();
    cout << "Total Unique Locations: " << locationMap.size() << endl;
    int maximumDistance = 0;
    int minimumDistance = 100000000;
    vector<pair<string, string>> locations;
    for (auto it = locationMap.begin(); it != locationMap.end(); it++)
    {
        locations.push_back(*it);
    }
    for (int i = 0; i < locations.size(); i++)
    {
        for (int j = i + 1; j < locations.size(); j++)
        {
            double lat1 = stod(locations[i].first);
            double lon1 = stod(locations[i].second);
            double lat2 = stod(locations[j].first);
            double lon2 = stod(locations[j].second);
            double distance = calcGPSDistance(lat1, lon1, lat2, lon2);
            if (distance > maximumDistance)
                maximumDistance = distance;
            if (distance < minimumDistance)
                minimumDistance = distance;
        }
    }
    cout << "Maximum Distance: " << maximumDistance << endl;
    cout << "Minimum Distance: " << minimumDistance << endl;

    inputFile.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Elapsed time: " << durationTotal << " ms" << std::endl;
    return 0;
}
