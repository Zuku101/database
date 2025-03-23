#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>

/**
 * @brief Structure representing a single measurement record.
 */
struct Measurement {
    std::string component;
    double temperature;
    long long timestamp;
};

#endif
