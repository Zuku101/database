#ifndef MEASUREMENT_H
#define MEASUREMENT_H

// Standard library headers
#include <string>

/**
 * @brief Structure representing a single temperature measurement.
 */
struct Measurement {

  /**
   * @brief Name of the hardware component (GPU/CPU/Motherboard).
   */
  std::string component;

  /**
   * @brief Temperature value in Celsius.
   */
  double temperature;

  /**
   * @brief Unix timestamp when measurement was taken.
   */
  long long timestamp;
};

#endif
