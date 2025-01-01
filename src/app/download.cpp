//
// Copyright (c) 2025, SpargatTeam
// Wrote by Comical
//
#include "download.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream) {
    std::ofstream* outFile = static_cast<std::ofstream*>(stream);
    outFile->write(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

std::string get_download_path() {
    const char* home_dir = std::getenv("HOME");
    if (home_dir) {
        return std::string(home_dir) + "/Downloads/";
    }
    return "./"; 
}

bool download_file(const std::string& url, const std::string& output_path) {
    CURL* curl;
    CURLcode res;
    std::ofstream outFile;
    outFile.open(output_path, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Eroare: Nu pot deschide fișierul " << output_path << " pentru scriere.\n";
        return false;
    }
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Eroare: Nu pot inițializa cURL.\n";
        return false;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Eroare la descărcare: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        outFile.close();
        return false;
    }
    curl_easy_cleanup(curl);
    outFile.close();
    return true;
}

namespace download {
    bool file(const std::string& url) {
        return download_file(url, get_download_path() + "/Downloads/");
    }
}