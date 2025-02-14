#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void watchDisplayFile() {
    string lastLine;
    
    while (true) {
        ifstream file("display.txt");
        if (file) {
            string line;
            while (getline(file, line)) {
                if (line != lastLine) {  // Prevent duplicate prints
                    cout << line << endl;
                    lastLine = line;
                }
            }
        } else {
            cerr << "Error reading display file!" << endl;
        }
        
        this_thread::sleep_for(chrono::milliseconds(500));  // Check every 500ms
    }
}

int main() {
    cout << "Lexicon Display Terminal" << endl;
    cout << "-------------------------" << endl;

    watchDisplayFile();  // Start monitoring the display file

    return 0;
}
