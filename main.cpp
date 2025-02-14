#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;
using namespace std;

vector<tuple<string, string, string>> lexicon;

// Function to check if the display process is already running
bool isDisplayRunning() {
    #ifdef _WIN32
        system("tasklist | findstr display.exe > nul");
        return !system("tasklist | findstr display.exe > nul");
    #else
        return !system("pgrep -x display > /dev/null");
    #endif
}

// startDisplayTerminal provides cross-platform suppot for launching the display terminal
void startDisplayTerminal() {
    #ifdef _WIN32
        system("start cmd /k display.exe");
    #else
        // Check if display is already running
        if (system("pgrep -x display > /dev/null") != 0) {
            system("x-terminal-emulator -e ./display &");  // Launch only if not running
        }
    #endif
}


// Function to write arbitrary text to a shared file
void sendTextToDisplay(const string& text) {
    ofstream file("display.txt");
    if (file) {
        file << text << endl;
    } else {
        cerr << "Error writing to display file!" << endl;
    }
}

// Function to parse user input into command tokens (handles quoted strings)
vector<string> parseCommand(const string& userCommand) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    stringstream ss;

    for (char ch : userCommand) {
        if (ch == '"') {
            inQuotes = !inQuotes;
            if (!inQuotes) {
                tokens.push_back(ss.str());
                ss.str("");
            }
        } else if (ch == ' ' && !inQuotes) {
            if (!ss.str().empty()) {
                tokens.push_back(ss.str());
                ss.str("");
            }
        } else {
            ss << ch;
        }
    }
    if (!ss.str().empty()) tokens.push_back(ss.str());

    return tokens;
}

// Function to add a new entry to the lexicon
void newEntry(const string& lexeme, const string& definition, const string& partOfSpeech) {
    lexicon.push_back(make_tuple(lexeme, definition, partOfSpeech));
    cout << "New entry added: " << lexeme << " (" << partOfSpeech << ") - " << definition << endl;

}

// Function to execute commands from terminal input
void executeCommand(const string& userCommand) {
    vector<string> tokens = parseCommand(userCommand);

    if (tokens.empty()) {
        cerr << "Error: No command entered." << endl;
        return;
    }

    if (tokens[0] == "n" && tokens.size() == 4) {  
        newEntry(tokens[1], tokens[2], tokens[3]);
    } else {
        cout << "Unknown or invalid command: " << userCommand << endl;
    }
}

// Main function
int main() {
    string userInput;

   
    ofstream clearFile("display.txt", ios::trunc);
    clearFile.close();

    
    startDisplayTerminal();

    cout << "Lexicon Manager. Type commands (e.g., 'n \"word\" \"definition\" \"pos\"'). Type 'exit' to quit.\n";

    while (true) {
        cout << "> ";
        getline(cin, userInput);

        if (userInput == "exit") {
            break;
        }

        executeCommand(userInput);
        sendTextToDisplay(user)
    }

    cout << "Exiting Lexicon Manager.\n";
    return 0;
}
