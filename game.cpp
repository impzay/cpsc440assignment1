#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void* input(ALLEGRO_THREAD* ptr, void* arg);
void* timer(ALLEGRO_THREAD* ptr, void* arg);

class logic {
public:
    logic();
    void introduction();
    bool createLists();
    bool playGame();
    string scrambler(string word);
    void end();

private:
    int numCorrect,
        smallWordLength,
        mediumWordLength,
        largeWordLength;
    string word;
    string smallWords[50];
    string mediumWords[50];
    string largeWords[50];
};

logic game;
bool game_over = false;
bool time_up = false;

int main(){
    al_init();
    srand(time(0)); 

	ALLEGRO_THREAD* create1 = NULL, * create2 = NULL; //return values
	create1 = al_create_thread(input, NULL);
	create2 = al_create_thread(timer, NULL);

	while (!game_over && !time_up) {
        if (!game_over && !time_up) {
			al_start_thread(create1);
			al_start_thread(create2);
        } else {
            al_destroy_thread(create1);
			al_destroy_thread(create2);
        }
	}
    if (game_over) {
        cout << "Game over.\n";
    }
    else {
        cout << "Time's up.\n";
    }
	
	return 0;
}

void* input(ALLEGRO_THREAD* ptr, void* arg) {
    game.introduction();
	if (game.createLists()) {
		game.playGame();
	}
    game.end();
    game_over = true;
    
    return NULL;
}

void* timer(ALLEGRO_THREAD* ptr, void* arg) {
	al_rest(5);
	time_up = true;

	return NULL;
}

logic::logic() {
	numCorrect = 0;
	smallWordLength = 0;
	mediumWordLength = 0;
	largeWordLength = 0;
    word = "";
}

void logic::introduction() {
	cout << "Welcome to the Scrambling Intelligence Test!\n";
	cout << "You will be given a scrambled word and you have to guess the original word.\n";
	cout << "You have 60 seconds to guess as many words as possible. Good luck!\n\n";
}

bool logic::createLists() {
    //open dictionary.txt and read words into arrays (4-5,6-7,8+)
	ifstream file("dictionary.txt");
    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return false;
    }
    string word;
    while (file >> word){
		//cout << word << endl;
		//loops through all words in file and sorts them by size into the 3 arrays, also keeps track of how many words are in each array
        int len = word.length();

		if (len >= 4 && len <= 5) {
			smallWords[smallWordLength] = word;
			smallWordLength++;
		}
		else if (len >= 6 && len <= 7) {
			mediumWords[mediumWordLength] = word;
			mediumWordLength++;
		}
		else if (len >= 8) {
			largeWords[largeWordLength] = word;
			largeWordLength++;
		}
    }
    file.close();
    return true;
}

bool logic::playGame() {
	//scramble and give 5 words to the player | 2 from smallWords, 2 from mediumWords, 1 from largeWords
    numCorrect = 0;
    string plrAnswer;
    string currWord;
    string scrambledWord;

    cout << "\n GAME START \n\n";
    //loops for the first 2 round in the small word array, selects a random word from the array, scrambles it, then the player tries to solve it
    //it repeats for the medium and large arrays
    for (int round = 1; round <= 2; round++) {
		int randIndex = rand() % smallWordLength;
		currWord = smallWords[randIndex];
		scrambledWord = scrambler(currWord);
		cout << "Round " << round << ": " << scrambledWord << endl;
		cout << "Your answer: ";
		cin >> plrAnswer;
		if (plrAnswer == currWord) {
			cout << "Correct!\n\n";
			numCorrect++;
		}else {
			cout << "INCORRECT! The correct word was: " << currWord << "\n";
		}
    }

    for (int round = 3; round <= 4; round++) {
        int randIndex = rand() % mediumWordLength;
        currWord = mediumWords[randIndex];
        scrambledWord = scrambler(currWord);

        cout << "Round " << round << ": " << scrambledWord << endl;
        cout << "Your answer: ";
        cin >> plrAnswer;
        if (plrAnswer == currWord) {
            cout << "Correct!\n\n";
            numCorrect++;
        }
        else {
            cout << "INCORRECT! The correct word was: " << currWord << "\n";
        }
    }
    int randomIndex = rand() % largeWordLength;
    currWord = largeWords[randomIndex];
    scrambledWord = scrambler(currWord);
    
    cout << "\n FINAL ROUND! " << scrambledWord << endl;
    cout << "Your answer: ";
    cin >> plrAnswer;

    if (plrAnswer == currWord) {
        cout << "Correct!\n";
        numCorrect++;
    }
    else {
        cout << "INCORRECT! The answer was: " << currWord << endl;
    }

    return (numCorrect >= 3);  //ereturn true if they got at least 3
}

string logic::scrambler(string word) {
    //scramble the word randomly
	int len = word.length();
    
	for (int i = 0; i < len * 4; i++) {
        int pos_1 = rand() % len;
        int pos_2 = rand() % len;

		char temp = word[pos_1];
		word[pos_1] = word[pos_2];
        word[pos_2] = temp;
	}
    //picks two random positions in the word and swaps em, itll do it the 4 time the amount of times the length of the word.

    return word;
}

void logic::end() {
    //display intellect level based on numCorrect
    if (numCorrect >= 5) {
		cout << "Einstein!? Is that you!?\n";
	}
    else if (numCorrect >= 4) {
		cout << "not bad, you might be a genius!\n";
    }
	else if (numCorrect >= 3) {
		cout << "is your name average joe?\n";
	}
    else if (numCorrect >= 1) {
        cout << "you might want to study some more...\n";
    }
    else {
		cout << "yeah.. maybe this game isnt for you...\n";
    }
    cout << "You got " << numCorrect << " correct!\n";
}