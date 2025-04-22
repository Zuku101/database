#pragma once

// Standard library headers
#include <chrono>
#include <string>
#include <vector>

// Project headers
#include "storage/measurement.h"

std::vector<Measurement> generateRandomMeasurements(int count);

void benchmarkSave(const std::vector<Measurement>& measurements, const std::string& component);

void benchmarkRead(const std::string& component, int recordCount);

void runBenchmark();
