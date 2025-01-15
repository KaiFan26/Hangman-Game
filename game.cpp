// Hangman Game
// Your very own Hanel Vujic
// January 2025
// Resources:
// https://github.com/Shreda/pentestTools/blob/master/random-words.txt
// https://ascii.co.uk/art/hangman
//Asking ChatGPT for Ascii Art

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string> 
#include <algorithm> 
#include <chrono>
#include <thread>
#include <set>
using namespace std;

int EASY_GUESSES = 7; //Number of guesses for easy mode
int MEDIUM_GUESSES = 5; //Number of guesses for medium mode
int HARD_GUESSES = 3; //Number of guesses for hard mode

string checkDifficultyInput(string input) { //Helper function to check what difficulty the user wants to play in

    transform(input.begin(), input.end(), input.begin(), ::tolower); //Turn everything to lower case to easily check

    if (input == "e" || input == "easy") { //Check for easy difficulty
        return "easy";
    }
    else if (input == "m" || input == "medium" ) { //or medium difficulty
        return "medium";
    }

    else if (input == "h" || input == "hard" ) { //Or hard difficulty
        return "hard";
    }

    else {
        return "Invalid Choice"; //Or invalidity!
    }
}

void printHangmanTitle() {
    string hangmanTitle = 
        " _                                              \n"
        "| |                                             \n"
        "| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  \n"
        "| '_ \\ / _` | '_ \\ / _` | '_ ` _ \\ / _` | '_ \\ \n"
        "| | | | (_| | | | | (_| | | | | | | (_| | | | |\n"
        "|_| |_|\\__,_|_| |_|\\__, |_| |_| |_|\\__,_|_| |_|\n"
        "                    __/ |                      \n"
        "                   |___/                       \n";

    cout << hangmanTitle;
}

void gameOverScreen() {

    string asciiArt = 
    " ___________.._______\n"
    "| .__________))______|\n"
    "| | / /      ||\n"
    "| |/ /       ||\n"
    "| | /        ||.-''.\n"
    "| |/         |/  _  \\\n"
    "| |          ||  `/,|\n"
    "| |          (\\\\`_.'\n"
    "| |         .-`--'.\n"
    "| |        /Y . . Y\\\n"
    "| |       // |   | \\\\\n"
    "| |      //  | . |  \\\\\n"
    "| |     ')   |   |   (`\n"
    "| |          ||'||\n"
    "| |          || ||\n"
    "| |          || ||\n"
    "| |          || ||\n"
    "| |         / | | \\\n"
    "\"\"\"\"\"\"\"\"\"\"|_`-' `-' |\"\"\"\"|\n"
    "|\"|\"\"\"\"\"\"\"\\\\ \\       '\"|\"|\n"
    "| |        \\\\ \\        | |\n"
    ": :         \\\\ \\       : :  sk\n"
    ". .          `'       . .\n";

    cout << asciiArt << endl;
}

void visualGameOver() {
    string asciiArt = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⣶⡆⠀⣰⣿⠇⣾⡿⠛⠉⠁\n"
"⠀⣠⣴⠾⠿⠿⠀⢀⣾⣿⣆⣀⣸⣿⣷⣾⣿⡿⢸⣿⠟⢓⠀⠀\n"
"⣴⡟⠁⣀⣠⣤⠀⣼⣿⠾⣿⣻⣿⠃⠙⢫⣿⠃⣿⡿⠟⠛⠁⠀\n"
"⢿⣝⣻⣿⡿⠋⠾⠟⠁⠀⠹⠟⠛⠀⠀⠈⠉⠀⠉⠀⠀⠀⠀⠀\n"
"⠀⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⣀⢀⣠⣤⣴⣤⣄⠀\n"
"⠀⠀⠀⠀⣀⣤⣤⢶⣤⠀⠀⢀⣴⢃⣿⠟⠋⢹⣿⣣⣴⡿⠋⠀\n"
"⠀⠀⣰⣾⠟⠉⣿⡜⣿⡆⣴⡿⠁⣼⡿⠛⢃⣾⡿⠋⢻⣇⠀⠀\n"
"⠀⠐⣿⡁⢀⣠⣿⡇⢹⣿⡿⠁⢠⣿⠷⠟⠻⠟⠀⠀⠈⠛⠀⠀\n"
"⠀⠀⠙⠻⠿⠟⠋⠀⠀⠙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";


    cout << asciiArt << endl;
}

// Function to get a random word from the file
string getRandomWordFromFile(const string& filename) {
    ifstream file(filename);  // Open the file
    if (!file) {
        cerr << "Error: Could not open the file!" << endl;
        return "";  // Return empty string if the file can't be opened
    }

    vector<string> words;  // Vector to store words
    string word;

    // Read each word from the file and store it in the vector
    while (getline(file, word)) {
        words.push_back(word);
    }

    file.close();  // Close the file

    if (words.empty()) {
        cerr << "Error: The file is empty!" << std::endl;
        return "";  // Return empty string if there are no words
    }

    // Seed random number generator and pick a random word
    srand(time(0));  // Seed the random number generator
    int randomIndex = rand() % words.size();  // Generate a random index

    return words[randomIndex];  // Return the randomly selected word
}

void initializeHiddenWord(vector<char>& wordToGuess, string randomWord) {
    
    for (size_t i = 0; i < randomWord.length(); i++) {
        if (isalpha(randomWord[i])) {
            wordToGuess[i] = '_';
        } 
        else {
            wordToGuess[i] = randomWord[i];
        }
    }

}

void printCurrentStateOfGuess(vector<char>& wordToGuess) {

    cout << "Your word: ";

    for (int i = 0; i < wordToGuess.size(); i++) {
        cout << wordToGuess[i] << " ";
    }
    cout << endl;

}

bool checkIfLetterFound(vector<char>& wordToGuess, string randomWord, string letterGuessed) {

    if (letterGuessed.length() >= 2) {
        cout << "Woah woah woah!! One letter at a time!" << endl;
        return false;
    }

    else if (letterGuessed.length() <= 0) {
        cout << "Ya gotta put something!!" << endl;
        return false;
    }

    if (!isalpha(letterGuessed[0])) {
        cout << "Put a letter!!!" << endl;
        return false;
    }

    transform(letterGuessed.begin(), letterGuessed.end(), letterGuessed.begin(), ::tolower);

    bool newLetterFound = false;

    for (int i = 0; i < wordToGuess.size(); i++) {

        if (randomWord[i] == letterGuessed[0]) {
            wordToGuess[i] = letterGuessed[0];
            newLetterFound = true;
        }
    }

    return newLetterFound;

}

bool checkIfUnderscoresGone(vector<char> wordToGuess) {

    for (int i = 0; i < wordToGuess.size(); i++) {
        if (wordToGuess[i] == '_') {
            return false;
        }
    }

    return true;
}

void startingNoose() {

    string smallerHeadNoose = 
        "   _______\n"
        "  |       |\n"
        "  |     \n"
        "  |   \n"
        "  |   \n"
        "  |   \n"
        "  |       \n"
        "  |       \n"
        "__|__\n";

    cout << smallerHeadNoose << endl;
}

void easyModeFirstGuessWrong() {
    string smallerHeadNoose = 
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    |     |\n"
        "  |    |     |\n"
        "  |     \\___/\n"
        "  |       \n"
        "  |       \n"
        "__|__\n";

    cout << smallerHeadNoose;
}

void easyModeSecondGuessWrong() {
    string secondGuessNoose = 
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    | . . |\n"
        "  |    |  O  |\n"
        "  |     \\___/\n"
        "  |       \n"
        "  |       \n"
        "__|__\n";

    cout << secondGuessNoose;
}

void easyModeThirdGuessWrong() {
    string thirdGuessNoose =
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    | . . |\n"
        "  |    |  O  |\n"
        "  |     \\___/\n"
        "  |       |\n"
        "  |       |\n"
        "__|__\n";

        cout << thirdGuessNoose << endl;
}


void easyModeFourthGuessWrong() {

    string fourthGuessWrong =
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    | . . |\n"
        "  |    |  O  |\n"
        "  |     \\___/\n"
        "  |       |\n"
        "  |       |\n"
        "  |      /\n"
        "  |     /   \n"
        "__|__\n";

        cout << fourthGuessWrong << endl;

}

void easyModeFifthGuessWrong() {

     string fifthGuessWrong = 
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    | . . |\n"
        "  |    |  O  |\n"
        "  |     \\___/\n"
        "  |       |\n"
        "  |       |\n"
        "  |      / \\\n"
        "  |     /   \\\n"
        "__|__\n";

        cout << fifthGuessWrong << endl;
}

void easyModeSixthGuessWrong() {

    string sixthGuessWrong = 
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    | . . |\n"
        "  |    |  O  |\n"
        "  |     \\___/\n"
        "  |   \\   | \n"
        "  |     --| \n"
        "  |      / \\ \n"
        "  |     /   \\ \n"
        "__|__\n";

    cout << sixthGuessWrong << endl;
}

void easyModeSeventhGuessWrong() {
    string seventhGuessWrong = 
        "   _______\n"
        "  |       |\n"
        "  |      _|_\n"
        "  |     /   \\\n"
        "  |    | X X |\n"
        "  |    |  O  |\n"
        "  |     \\___/\n"
        "  |   \\  | \n"
        "  |    --|-- \n"
        "  |     / \\  \\\n"
        "  |    /   \\ \n"
        "  |         \n"
        "__|__\n";

        cout << seventhGuessWrong << endl;
}

int main() {

    cout << "Welcome to the Hangman Chamber!" << endl; //Start of the game!!!
    cout << "What difficulty do you want to play in???" << endl;
    cout << "       Easy          Medium        Hard" << endl; //Establish what difficulties are available
    cout << "( Type E or Easy | M or Medium | H or Hard )" << endl;
    string input; //User input 

    // printHangmanTitle();

    set<char> userGuesses; //Keep track of what letter's the user guessed

    cin >> input; //Get input

    string difficultyCheck = checkDifficultyInput(input); //See if they correctly typed in 

    while (difficultyCheck == "Invalid Choice") {
        cout << "Invalid option or perhaps a mistype! Try again... :)" << endl;
        cin >> input;
        difficultyCheck = checkDifficultyInput(input); //Prompt the user again if they mistyped or chose wrong
    }

    string randomWord = getRandomWordFromFile("random-words.txt"); //Get a random word from the text file

    // randomWord = "needle"; //temporary test
    // cout << randomWord << endl;

    vector<char> guessWord(randomWord.length());
    initializeHiddenWord(guessWord, randomWord);

    int numOfGuesses = 0;

    string letterGuess;
    string difficulty;

    bool Game = true;
    bool letterFound;
    int numOfMistakes = 0;

    if (difficultyCheck == "easy") {
        difficulty = "Easy";
        numOfGuesses = 7;
    }

     else if (difficultyCheck == "medium") {
        difficulty = "Medium";
        numOfGuesses = 5;
    }

     else if (difficultyCheck == "hard") {
        difficulty = "Hard";
        numOfGuesses = 3;
    }


    cout << difficulty << " Mode Chosen...." << endl << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "You've got " << numOfGuesses << " guesses..." << endl << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Let's get hangin!" << endl;
    this_thread::sleep_for(chrono::seconds(1));H
    system("clear");

    startingNoose();
    printCurrentStateOfGuess(guessWord);
    cout << "Guess a letter!" << endl;


     while (Game) {

            cin >> letterGuess;
            letterFound = checkIfLetterFound(guessWord, randomWord, letterGuess);

            if (!letterFound) {
                numOfMistakes++;
            }
            cout << "Num of mistakes: " << numOfMistakes << endl;

            switch (numOfMistakes) {

                case 0:
                    startingNoose();
                    break;
                case 1:
                    cout << "We're here" << endl;
                    easyModeFirstGuessWrong(); 
                    break;
                case 2:
                    easyModeSecondGuessWrong();
                    break;
                case 3:
                    easyModeThirdGuessWrong();
                    break;
                case 4:
                    easyModeFourthGuessWrong();
                    break;
                case 5:
                    easyModeFifthGuessWrong();
                    break;
                case 6:
                    easyModeSixthGuessWrong();
                    break;
                case 7:
                    // easyModeSeventhGuessWrong();
                    gameOverScreen();
                    cout << endl << endl << endl;
                    visualGameOver();
                    break;

            }

            printCurrentStateOfGuess(guessWord);

            if (checkIfUnderscoresGone(guessWord)) {
                Game = false;
                cout << "You found them all!" << endl;
            }
            
        }


    return 0;
}