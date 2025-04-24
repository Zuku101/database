// Standard library headers
#include <chrono>
#include <iostream>
#include <thread>

// Project headers
#include "api/ohm_api.h"
#include "api/ohm_data.h"
#include "benchmark/benchmark.h"
#include "benchmark/sqlite_storage.h"
#include "config/config.h"
#include "inputs/file_source.h"
#include "storage/index_manager.h"
#include "storage/storage.h"

using namespace std;
using namespace std::chrono;

/**
 * @brief Fetches a single Measurement for the given component from OHM.
 *
 * @param component
 *   Name of the hardware component ("CPU", "GPU", "Motherboard")
 *
 * @return Measurement
 *   The temperature and timestamp for the requested component
 *
 * @throws runtime_error
 *   If fetching or parsing the OHM data fails
 */
static Measurement getMeasurementFromOHM(const string& component) {
    string json = fetchOHMData(OHM_URL);
    if (json.empty()) throw runtime_error("Failed to fetch data from OHM");
    auto j = nlohmann::json::parse(json);
    OHMData d(j);

    double temp = -1.0;
    if      (component == "CPU")         temp = d.getCPUTemperature();
    else if (component == "GPU")         temp = d.getGPUTemperature();
    else if (component == "Motherboard") temp = d.getMotherboardTemperature();

    if (temp < 0) throw runtime_error("Failed to get temperature for " + component);
    return {component, temp, d.getTimestamp()};
}

/**
 * @brief Benchmarks saving measurements to JSON storage.
 *
 * @param components
 *   List of component names to record
 * @param numRecords
 *   Number of batches (each batch writes one record per component)
 * @param interval
 *   Delay in seconds between batches (0 for no delay)
 *
 * @return long long
 *   Total time spent saving in microseconds
 */
long long benchmarkSaveJson(const vector<string>& components,
                            int numRecords,
                            int interval)
{
    cout << "\n=== JSON Save Benchmark ===\n";
    StorageManager storage;
    long long total = 0;

    for (int i = 0; i < numRecords; ++i) {
        cout << "Saving JSON batch " << (i+1) << "/" << numRecords << "...\n";
        for (auto& comp : components) {
            auto t0 = high_resolution_clock::now();
            try {
                auto m = getMeasurementFromOHM(comp);
                storage.saveRecord(m);
            } catch (exception& e) {
                cout << "Error: " << e.what() << "\n";
                continue;
            }
            auto t1 = high_resolution_clock::now();
            long long dt = duration_cast<microseconds>(t1 - t0).count();
            total += dt;
            cout << "Record for " << comp
                 << " saved in " << dt << " µs\n";
        }
        if (interval > 0) this_thread::sleep_for(seconds(interval));
    }
    return total;
}

/**
 * @brief Benchmarks reading measurements from JSON storage.
 *
 * @param components
 *   List of component names to read
 * @param numRecords
 *   Maximum number of records to read per component (0 for all)
 * @param interval
 *   Delay in seconds between reading different components (0 for no delay)
 *
 * @return long long
 *   Total time spent reading in milliseconds
 */
long long benchmarkReadJson(const std::vector<std::string>& components,
                            int numRecords,
                            int interval)
{
    cout << "\n=== JSON Read Benchmark ===\n";
    long long total = 0;
    FileSource src;

    for (const auto& comp : components) {
        cout << "Reading JSON batch for " << comp << " (" 
             << numRecords << " records)...\n";

        auto t0 = high_resolution_clock::now();
        auto recs = src.getMeasurements(comp, numRecords, false);
        auto t1 = high_resolution_clock::now();

        long long dt = duration_cast<milliseconds>(t1 - t0).count();
        total += dt;

        cout << "Read " << recs.size() << " records for " << comp
             << " in " << dt << " ms\n";

        for (const auto& m : recs) {
            cout << " - Temp: " << m.temperature << "°C"
                 << ", Timestamp: " << m.timestamp << "\n";
        }

        if (interval > 0) this_thread::sleep_for(seconds(interval));
    }

    return total;
}

/**
 * @brief Benchmarks saving measurements to SQLite database.
 *
 * @param components
 *   List of component names to record
 * @param numRecords
 *   Number of batches (each batch writes one record per component)
 * @param interval
 *   Delay in seconds between batches (0 for no delay)
 *
 * @return long long
 *   Total time spent saving in microseconds
 */
long long benchmarkSaveSqlite(const vector<string>& components,
                              int numRecords,
                              int interval)
{
    cout << "\n=== SQLite Save Benchmark ===\n";
    SQLiteStorageManager sqlite;
    long long total = 0;

    for (int i = 0; i < numRecords; ++i) {
        cout << "Saving SQLite batch " << (i+1) << "/" << numRecords << "...\n";
        for (auto& comp : components) {
            auto m = getMeasurementFromOHM(comp);
            auto t0 = high_resolution_clock::now();
            sqlite.saveRecord(m);
            auto t1 = high_resolution_clock::now();
            long long dt = duration_cast<microseconds>(t1 - t0).count();
            total += dt;
            cout << "Record for " << comp
                 << " saved in " << dt << " µs\n";
        }
        if (interval > 0) this_thread::sleep_for(seconds(interval));
    }
    return total;
}

/**
 * @brief Benchmarks reading measurements from SQLite database.
 *
 * @param components
 *   List of component names to read
 * @param numRecords
 *   Maximum number of records to read per component
 * @param interval
 *   Delay in seconds between reading different components (0 for no delay)
 *
 * @return long long
 *   Total time spent reading in milliseconds
 */
long long benchmarkReadSqlite(const std::vector<std::string>& components,
                              int numRecords,
                              int interval)
{
    cout << "\n=== SQLite Read Benchmark ===\n";
    SQLiteStorageManager sqlite;
    long long total = 0;

    for (const auto& comp : components) {
        cout << "Reading SQLite batch for " << comp << " ("
             << numRecords << " records)...\n";

        auto t0 = high_resolution_clock::now();
        auto recs = sqlite.loadRecords(comp, numRecords);
        auto t1 = high_resolution_clock::now();

        long long dt = duration_cast<milliseconds>(t1 - t0).count();
        total += dt;

        cout << "Read " << recs.size() << " records for " << comp
             << " in " << dt << " ms\n";

        for (const auto& m : recs) {
            cout << " - Temp: " << m.temperature << "°C"
                 << ", Timestamp: " << m.timestamp << "\n";
        }

        if (interval > 0) this_thread::sleep_for(seconds(interval));
    }

    return total;
}

/**
 * @brief Runs all benchmarks (JSON save/read and SQLite save/read) and prints a summary.
 */
void runBenchmark() {
    vector<string> components = {"CPU","GPU","Motherboard"};
    int numRecords, interval;

    cout << "Enter number of records: ";
    cin  >> numRecords;
    cout << "Enter interval (s): ";
    cin  >> interval;

    auto totalJsonSave = benchmarkSaveJson(components, numRecords, interval);
    auto totalJsonRead = benchmarkReadJson(components, numRecords, interval);
    auto totalSqlSave  = benchmarkSaveSqlite(components, numRecords, interval);
    auto totalSqlRead  = benchmarkReadSqlite(components, numRecords, interval);

    int recCount = numRecords * components.size();
    cout << "\n--- Summary ---\n";
    cout << "JSON:   total save = " << totalJsonSave
         << " µs, avg = " << (totalJsonSave/ double(recCount)) << " µs/rec\n";
    cout << "JSON:   total read = " << totalJsonRead
         << " ns, avg = " << (totalJsonRead/ double(recCount)) << " ns/rec\n";
    cout << "SQLite: total save = " << totalSqlSave
         << " µs, avg = " << (totalSqlSave/ double(recCount)) << " µs/rec\n";
    cout << "SQLite: total read = " << totalSqlRead
         << " ns, avg = " << (totalSqlRead/ double(recCount)) << " ns/rec\n";
}
