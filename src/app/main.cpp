//
// Copyright (c) 2025, SpargatTeam
// Wrote by Comical
//
#include "download.h"
#include "ui.h"
#include <iostream>

int main() {
    // TODO:i mplement on fluxium later
    std::string link = "https://example.com/samplefile.zip"; 
    if (download::file(link)) {
        std::cout << "File downloaded successfully to Downloads folder.\n";
    } else {
        std::cerr << "Failed to download file.\n";
    }
    return flux_ui::BrowserApp::run();
}