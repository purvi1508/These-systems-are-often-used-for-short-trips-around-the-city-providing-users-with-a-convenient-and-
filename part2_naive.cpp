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
    int feasibleTrips = 0;
    map<pair<string, string>, int> locationMap;
    vector<vector<time_t>> startTimesList;
    vector<array<long long, 3>> edges; // {start, end, endTime}
    int idx = 0;
    vector<string> res;
    while (getline(inputFile, line))
    {
        line = line.substr(0, line.size() - 1);
        res.push_back(line);
    }
    for (int i=0; i<res.size(); i++)
    {
        auto u = res[i];
        for (int j=0; j<res.size(); j++)
        {
            if (i == j)
                continue;
            auto v = res[j];
            vector<string> res1 = split(u, ',');
            vector<string> res2 = split(v, ',');
            time_t startTime1, endTime1, startTime2, endTime2;
            struct tm tmStart1{}, tmEnd1{}, tmStart2{}, tmEnd2{};
            strptime(res1[1].c_str(), "%m-%d-%Y %H:%M", &tmStart1);
            strptime(res1[2].c_str(), "%m-%d-%Y %H:%M", &tmEnd1);
            strptime(res2[1].c_str(), "%m-%d-%Y %H:%M", &tmStart2);
            strptime(res2[2].c_str(), "%m-%d-%Y %H:%M", &tmEnd2);
            startTime1 = mktime(&tmStart1);
            endTime1 = mktime(&tmEnd1);
            startTime2 = mktime(&tmStart2);
            endTime2 = mktime(&tmEnd2);
            if (tmStart1.tm_hour >= 6 && tmStart1.tm_hour < 18 && tmStart2.tm_hour >= 6 && tmStart2.tm_hour < 18)
            {
                if (res1[5] == res2[3] && res1[6] == res2[4])
                {
                    if (endTime1 <= startTime2)
                        feasibleTrips++;
                }
            }
        }
    }
    cout << "Total Feasible Trips: " <<feasibleTrips << endl;
    inputFile.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Elapsed time: " << durationTotal << " ms" << std::endl;
    return 0;
}
