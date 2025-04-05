// Standard library headers
#include <iostream>

// Third-party libraries
#include <curl/curl.h>

// Project headers
#include "api/ohm_api.h"

using namespace std;

/**
 * Callback function for cURL to write response data to a string.
 *
 * @param contents
 *   Pointer to the received data.
 * @param size
 *   Size of each data chunk.
 * @param nmemb
 *   Number of data chunks.
 * @param output
 *   Pointer to the string where data is stored.
 *
 * @return
 *   Total size of data written.
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
  size_t totalSize = size * nmemb;
  output->append((char*)contents, totalSize);
  return totalSize;
}

/**
 * Fetches JSON data from Open Hardware Monitor.
 *
 * @param url
 *   The OHM endpoint URL.
 *
 * @return
 *   The JSON response as a string.
 */
std::string fetchOHMData(const std::string& url) {
  CURL* curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      cerr << "cURL error: " << curl_easy_strerror(res) << endl;
    }

    curl_easy_cleanup(curl);
  }
  else {
    cerr << "cURL initialization failed\n";
  }

  return response;
}
