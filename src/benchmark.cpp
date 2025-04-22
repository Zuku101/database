// Standard library headers
#include <chrono>
#include <iostream>
#include <thread>

// Project headers
#include "api/ohm_api.h"
#include "api/ohm_data.h"
#include "benchmark.h"
#include "config/config.h"
#include "inputs/file_source.h"
#include "storage/index_manager.h"
#include "storage/storage.h"

using namespace std;
using namespace std::chrono;

Measurement getMeasurementFromOHM(const string& component) {
  std::string ohmJsonString = fetchOHMData(OHM_URL);
  if (ohmJsonString.empty()) {
    throw runtime_error("Failed to fetch data from OHM");
  }

  nlohmann::json ohmData = nlohmann::json::parse(ohmJsonString);
  OHMData sensorData(ohmData);
  double temp = -1.0;
  long long timestamp = sensorData.getTimestamp();

  if (component == "CPU") {
    temp = sensorData.getCPUTemperature();
  }
  else if (component == "GPU") {
    temp = sensorData.getGPUTemperature();
  }
  else if (component == "Motherboard") {
    temp = sensorData.getMotherboardTemperature();
  }

  if (temp == -1.0) {
    throw runtime_error("Failed to get temperature for " + component);
  }

  return {component, temp, timestamp};
}

long long benchmarkSave(const std::vector<string>& components, int numRecords, int interval) {
  long long totalSaveTime = 0;
  StorageManager storage;

  for (int i = 0; i < numRecords; ++i) {
    cout << "Saving record batch " << (i + 1) << " of " << numRecords << "...\n";

    for (const auto& component : components) {
      auto start = high_resolution_clock::now();
      try {
        Measurement measurement = getMeasurementFromOHM(component);
        storage.saveRecord(measurement);
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start);
        totalSaveTime += duration.count();

        cout << "Record for " << component << " saved in " << duration.count() << " microseconds."
             << endl;
      }
      catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
        continue;
      }
    }

    if (interval > 0) {
      this_thread::sleep_for(chrono::seconds(interval));
    }
  }

  return totalSaveTime;
}

long long benchmarkRead(const std::string& component, int recordCount, int interval) {
  FileSource source;
  auto measurements = source.getMeasurements(component, recordCount, false);

  if (measurements.empty()) {
    return 0;
  }

  long long totalReadTime = 0;

  for (const auto& m : measurements) {
    auto start = high_resolution_clock::now();

    auto end = high_resolution_clock::now();
    long long duration = duration_cast<nanoseconds>(end - start).count();
    totalReadTime += duration;

    cout << "Record for " << m.component << " with timestamp " << m.timestamp << " read in "
         << duration << " nanoseconds." << endl;

    if (interval > 0) {
      this_thread::sleep_for(seconds(interval));
    }
  }

  return totalReadTime;
}

void runBenchmark() {
  std::vector<std::string> components = {"CPU", "GPU"};
  // std::vector<std::string> components = {"CPU", "GPU", "Motherboard"};

  int numRecords, interval;

  std::cout << "Enter the number of records to save: ";
  std::cin >> numRecords;
  std::cout << "Enter the interval between records (s): ";
  std::cin >> interval;

  std::cout << "\n=== Benchmark for saving data ===" << std::endl;
  long long totalSaveMicro = benchmarkSave(components, numRecords, interval);

  std::cout << "\n=== Benchmark for reading data ===" << std::endl;
  long long totalReadNano = 0;
  long long totalRecords = 0;

  for (const auto& comp : components) {
    auto measurements = FileSource{}.getMeasurements(comp, numRecords, false);
    if (measurements.empty()) {
      std::cout << "No records for " << comp << ", skipping." << std::endl;
      continue;
    }

    totalRecords += measurements.size();
    totalReadNano += benchmarkRead(comp, numRecords, interval);
  }

  if (numRecords > 0 && !components.empty()) {
    double avgSaveMicro = totalSaveMicro / static_cast<double>(numRecords * components.size());
    std::cout << "\nAverage Save Time per record: " << avgSaveMicro << " microseconds" << std::endl;
  }

  if (totalRecords > 0) {
    double avgReadNano = totalReadNano / static_cast<double>(totalRecords);
    std::cout << "\nAverage Read Time per record: " << avgReadNano << " microseconds" << std::endl;
  }
}
