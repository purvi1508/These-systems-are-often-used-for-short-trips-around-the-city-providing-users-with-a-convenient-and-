#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <time.h>
#include <ctime>
#include <chrono>
#include <map>
#include <array>
#include <algorithm>

using namespace std;

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
    int feasibleTrips = 0, circularTrips = 0;
    map<pair<string, string>, int> locationMap;
    vector<vector<time_t>> startTimesList;
    vector<array<long long, 3>> edges; // {start, end, endTime}
    int idx = 0;
    while (getline(inputFile, line))
    {
        line = line.substr(0, line.size() - 1);
        vector<string> res = split(line, ',');
        time_t startTime, endTime;
        struct tm tmStart{}, tmEnd{};
        strptime(res[1].c_str(), "%m-%d-%Y %H:%M", &tmStart);
        strptime(res[2].c_str(), "%m-%d-%Y %H:%M", &tmEnd);
        if (tmStart.tm_hour >= 6 && tmStart.tm_hour < 18)
        {
            startTime = mktime(&tmStart);
            endTime = mktime(&tmEnd);
            double duration = difftime(endTime, startTime);
            if (locationMap.count({res[3], res[4]}) == 0)
            {
                locationMap[{res[3], res[4]}] = idx++;
                startTimesList.emplace_back();
            }
            if (locationMap.count({res[5], res[6]}) == 0)
            {
                locationMap[{res[5], res[6]}] = idx++;
                startTimesList.emplace_back();
            }
            long long startIdx = locationMap[{res[3], res[4]}];
            long long endIdx = locationMap[{res[5], res[6]}];
            long long endTimeOfTrip = endTime;
            edges.push_back({startIdx, endIdx, endTimeOfTrip});
            startTimesList[startIdx].push_back(startTime);
            if (abs(duration) < 0.1 && res[3] == res[5] && res[4] == res[6])
            {
                circularTrips++;
            }    
        }
    }
    for (auto &u: startTimesList)
    {
        sort(u.begin(), u.end());
    }
    for (auto &[startIdx, endIdx, endTimeOfTrip]: edges)
    {
        int lb = lower_bound(startTimesList[endIdx].begin(), startTimesList[endIdx].end(), endTimeOfTrip) - startTimesList[endIdx].begin();
        feasibleTrips += startTimesList[endIdx].size() - lb;
    }
    cout << feasibleTrips - circularTrips << endl;
    inputFile.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Elapsed time: " << durationTotal << " ms" << std::endl;
    return 0;
}
