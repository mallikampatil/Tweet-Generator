//  prog4TweetGenerator.cpp
//     Generate somewhat random tweets based on a datafile.
//
//  CS 141 Spring 2019
//  Author: Andres Tapia and Mallika Patil
//
//  Open up a data file and find a starting point for some set of consecutive words
//  and print the starting word.
//
//  Next find all occurrences in the datafile of those consecutive words, and collect the set
//  of words that immediately follow each of those occurrences.  Choose one of the words
//  in this newly created set and add it to the set of consecutive words, shifting them
//  over by one so the first word gets overwritten by the second, and so on.
//
//  Repeat this process until >=30 words are displayed and an end-of-sentence word is found,
//  or until a displayed word has a newline or a return character at the end of it.
//
#include <iostream>
#include <cctype>       // For the letter checking functions
#include <fstream>      // For file input
#include <vector>
#include <cstring>      // For strlen()
#include <cstdlib>      // For rand()
using namespace std;

//---------------------------------------------------------------------
void getInputWords( vector<string> &allWords)   // List of all the words in the selected input file
{
    // Prompt for and get user choice for which data file to use
    cout << "Menu Options: \n"
         << "  1. Use Trump tweets \n"
         << "  2. Use Dalai Lama tweets \n"
         << "  3. Use Mixed Trump / Dalai Lama tweets \n"
         << "  4. Use small test file \n"
         << "  5. Use tiny test file \n"
         << "Your choice: ";
    int dataSelectionOption;
    cin >> dataSelectionOption;
    cout << endl; 
    
    ifstream inputFileStream;  // declare the input file stream
    // open input file and verify
    switch( dataSelectionOption) {
        case 1: inputFileStream.open("TrumpTweets.txt"); break;
        case 2: inputFileStream.open("DalaiLamaTweets.txt"); break;
        case 3: inputFileStream.open("TrumpLama.txt"); break;
        case 4: inputFileStream.open("Test.txt"); break;
        case 5: inputFileStream.open("tiny.txt"); break;
        default: cout << "Invalid option chosen, exiting program. ";
            exit( -1);
            break;
    }
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file.  Exiting..." << endl;
        exit( -1);
    }
    
    char c = ' ';
    char inputWord[ 81];
    
    // Read a character at a time from the input file, separating out individual words.
    inputWord[ 0] = '\0';
    int index = 0;
    while( inputFileStream.get(c)) {
        if( !isprint( c) && c != '\r' && c != '\n') {
            continue;   // Skip non-printable characters and get the next one
        }
        if( c != ' ') {
            // If it is not a return or newline, add it to the string.
            // If it is a return or newline character, only add it if there
            //    are already other characters in the string.
            if( (c != '\r' && c != '\n') ||
                ( (c == '\r' || c == '\n') && index > 0)
              ) {
                inputWord[ index++] = c;
            }
        }
        // End the word when encountering a space or a return character.
        if( c == ' ' || c == '\r' || c == '\n'){
            // Null terminate the input word. Store it if its length is > 0 and it is printable.
            inputWord[ index] = '\0';
            if( strlen( inputWord) > 0 && isprint( inputWord[0])) {
                allWords.push_back( string( inputWord));
            }
            // Check for special case where there is a space at the end of the line.  We don't want
            // to lose the end of line character ('\n' or '\r'), so we concatenate it to the
            // last word that was previously stored on the list.  First check to see if this is the case:
            if( (c == '\r' || c == '\n') && strlen( inputWord) == 0) {
                long indexOfLastWord = allWords.size() - 1;
                allWords.at( indexOfLastWord).append("\r");
            }
            
            index = 0;    // Restart the index for the next word
        }
    }//end while( inputFileStream...)
}
//---------------------------------------------------------------------
void checkWords( vector<string> startWords,  // List of first words in sentences
                 vector<string> allWords,    // All words from input file
                 vector<string> wordList,    // List of all words following search phrase
                 string nextWord)            // Nexts word found after search phrase
{
    int debugMenuOption = 0;
    
    do {
        // Prompt for and get user choice for which debug option to use
        cout << " \n"
             << "    >>> Debug menu options: <<<\n"
             << "        1. Display a start word \n"
             << "        2. Display one of all words \n"
             << "        3. Display wordWindow and next words \n"
             << "        4. Exit debugging \n"
             << "    Your choice -> ";
        cin >> debugMenuOption;
        
        int wordIndexValue = 0;    // Used to read user input for debug options below
        
        switch( debugMenuOption) {
            case 1: cout << "    Enter an index value from 0 to " << startWords.size()-1 << " to display a start word: ";
                    cin >> wordIndexValue;
                    cout << "    " << startWords.at( wordIndexValue) << endl;
                    break;
            case 2: cout << "    Enter an index value from 0 to " << allWords.size()-1 << " to display one of all words: ";
                    cin >> wordIndexValue;
                    cout << "    " << allWords.at( wordIndexValue) << endl;
                    break;
            case 3: cout << "    WordWindow and next words are: ";
                    for( int i = 0; i < wordList.size(); i++) {
                        cout << wordList.at( i) << " ";
                    }
                    cout << nextWord <<endl;
                    break;
            case 4: exit(-1); break;
        }//end switch(...)
    }while( debugMenuOption != 4);
    
}//end debug()

// Function to make an array of starting words 
void getStartWords( vector<string> &startWords, vector<string> allWords){
    startWords.resize(1);  // Resize start words array 
    startWords.at(0) = allWords.at(0);  // First word added to startWords vector
    for(int i = 1; i<allWords.size(); i++){
        if ( allWords.at(i - 1)[allWords.at(i - 1).size() - 1] == '\n' || 
             allWords.at(i - 1)[allWords.at(i - 1).size() - 1] == '\r')
            startWords.push_back(allWords.at(i));
    }
}

// Function to make array of words list 
void getWordList(vector<string> &wordList, vector<string> allWords, int startWordIndex, int wordWindowWidth)
{
    for(int i = 0; i < wordWindowWidth; i++){
        wordList.push_back(allWords.at(startWordIndex+i));
    }
}


//---------------------------------------------------------------------
int main()
{
    vector<string> allWords;   // List of all input words
    vector<string> startWords; // List of words that start sentences, to use
                               //   in starting generated tweets.

    cout << "Welcome to the tweet generator. " << endl << endl;
    
    getInputWords( allWords);
    // You must write the function shown below, and call it from here:
    getStartWords( startWords, allWords);
    
    // Allow looping to give more than one tweet
    int wordWindowWidth = 0;
    char userInput = ' ';
    int startWordIndex;
    int nextWordIndex; 
    string nextWord;
    bool debugMode = false;
    
    // Main loop
    while(userInput != 'X') {
        cout << endl; 
        cout << "Enter x to exit, or d/c to debug/continue followed by word window size: ";
        cin >> userInput;
        userInput = toupper( userInput);    // fold user input into upper case
      
        if( userInput == 'X') {
            break;   // Exit program
        }
        
        // Also check for input of 'D' to debug
        if(userInput == 'D'){
          debugMode = true;
        }
      
        // Read in the wordWindowWidth to be used in both the "continue" and the "debug" modes
        cin >> wordWindowWidth;
      
        vector<string> wordList;   // Stores the list of consecutive words that are searched for
      
        // Find a random starting word in the startWords list
        int randNum = rand() % (startWords.size());
        string randWord = startWords.at(randNum);
      
        // Find the selected startWord within the allWords list, and remember its position
        for(int i=0; i < allWords.size(); i++){
            if (randWord == allWords.at(i)){
                startWordIndex = i;
                break; //we only need to find the first occurrence of the random start word. 
            }
        }
      
        // Store the chosen number of consecutive starting words
        getWordList(wordList, allWords, startWordIndex, wordWindowWidth);
        int wordCounter = 0;
      
        while(true) {
            if(wordCounter == 0){
                cout << endl; 
            }
            // Search for adjacent wordWindow words in allWords and find all the words that
            // follow the sequence of that word sequence.  Choose one at random and call it nextWord.
            vector<string> possibleNextWords;
            for(int i = 0; i < allWords.size() - wordWindowWidth; i++){
                bool isFound = true; 
                for(int j = 0; j < wordList.size(); j++){ 
                    if ((allWords.at(i + j)).compare(wordList.at(j)) != 0) { //if the word in allWord is not equal to the word in wordList, then it gets out of the for loop and 
                                                                             //checks the next word in allWords.
                       isFound = false;
                       break;
                    }
                } 
                if(isFound){
                    possibleNextWords.push_back(allWords.at(i + wordList.size())); 
                }
            }
          
            int randomNumber = rand() % (possibleNextWords.size());
            nextWord = possibleNextWords.at(randomNumber);
            
            // If in checkWordsMode then allow displaying single word values using the checkWords(...) function, then exit the program.
            if(debugMode)
            {
              checkWords(startWords, allWords, wordList, nextWord);
            }
            
            // Display the first word in the wordWindow
            cout << " " << wordList.at(0);
            wordCounter++; 
            
            char character = (wordList.at(0)).at((wordList.at(0)).size() - 1);
            // If the first word in the wordWindow ends in a return, then we're done with our output for this tweet.
            if ((character == '\r') || (character == '\n')) { 
                break; 
            }
            
            // If there have been >= 30 words and the word ends with '.', '.' or '!' or '?', then we're done with output for this tweet
            if (((character == '.') || (character == '!') || (character == '?')) && (wordCounter >= 30)) { 
                break; 
            }
          
            // Shift words "left" to prepare for next round, shifting nextWord into the "right" of the wordWindow words.
            wordList.erase(wordList.begin());
            wordList.push_back(nextWord);
          
        }//end while( true)
    }// end while( userInput != 'x')
    
    cout << endl;
    
    return 0;
}//end main()

