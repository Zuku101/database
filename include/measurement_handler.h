#pragma once
#include <memory> // 💥 konieczne!
#include "data_source.h"
#include "storage.h"

class MeasurementHandler {
public:
    static MeasurementHandler& getInstance();
    void handleMonitoring(const std::string& component);
    void addSingleRecord(const std::string& componentName);

private:
    MeasurementHandler();
    StorageManager storage;
    std::unique_ptr<DataSource> source;

    bool kbhit() const;
    void clearInputBuffer();
};
