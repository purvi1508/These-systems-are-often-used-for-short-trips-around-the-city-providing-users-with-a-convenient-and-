#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <vector>
#include <cstdlib>
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
    // std::ofstream outputFile("bike_data_rem.csv");
    // if (!outputFile.is_open())
    // {
    //     std::cerr << "Error: File didn't open" << std::endl;
    //     exit(1);
    // }
    FILE *outputFile = fopen("bike_data_rem.csv", "w");
    if (!outputFile)
    {
        std::cerr << "Error: File didn't open" << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(inputFile, line); // Skips Header
    double maxDuration = 0;
    double minDuration = 100000000;
    vector<int> durations;
    int circularTrips = 0, totalMinDurations = 0;
    while (getline(inputFile, line))
    {
        line = line.substr(0, line.size() - 1);
        vector<string> res = split(line, ',');
        time_t startTime, endTime;
        struct tm tmStart
        {
        }, tmEnd{};
        strptime(res[1].c_str(), "%m-%d-%Y %H:%M", &tmStart);
        strptime(res[2].c_str(), "%m-%d-%Y %H:%M", &tmEnd);
        startTime = mktime(&tmStart);
        endTime = mktime(&tmEnd);
        double duration = difftime(endTime, startTime);
        // cout << res[1]  << " " << res[2] << " " << duration <<  endl;
        if (std::abs(duration) > 0.1)
        {
            maxDuration = max(maxDuration, duration);
            minDuration = min(minDuration, duration);
            durations.push_back(duration);
            if (res[3] == res[5] && res[4] == res[6])
            {
                circularTrips++;
            }
            // outputFile << line << endl;
            fprintf(outputFile, "%s\n", line.c_str());
        }
    }
    for (int i = 0; i < durations.size(); i++)
    {
        if ((std::abs(duration) > 0.1) < 0.1)
        {
            totalMinDurations++;
        }
    }
    cout << "Max Duration: " << maxDuration << endl;
    cout << "Min Duration: " << minDuration << endl;
    cout << "Total Min Durations: " << totalMinDurations << endl;
    cout << "Circular Trips: " << (circularTrips * 1.0 / durations.size()) * 100 << endl;

    inputFile.close();
    // outputFile.close();
    fclose(outputFile);
    auto end = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Elapsed time: " << durationTotal << " ms" << std::endl;
    return 0;
}
