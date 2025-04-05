#ifndef OHM_API_H
#define OHM_API_H

#include <string>

/**
 * Fetches data from Open Hardware Monitor using a given URL.
 *
 * @param url 
 *   The OHM endpoint URL.
 * 
 * @return 
 *   The JSON response as a string.
 */
std::string fetchOHMData(const std::string& url);

#endif
