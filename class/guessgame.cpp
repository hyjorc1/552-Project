#include "guessgame.h"
#include <iostream>

GuessGame::GuessGame(int secret)
{
    //save the secret
    this->_secret = secret;

    //set the number of guesses to zero
    this->_nguesses = 0;
}

bool GuessGame::guess(int value)
{
    if (this->_nguesses >= 3)
    {
        std::cout << "Sorry, you have run out of guesses." << std::endl;
        return false;
    }
    else if (value == this->_secret)
    {
        std::cout << "Well done - you have won the game!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Wrong answer. Try again!" << std::endl;
        this->_nguesses += 1;
        return false;
    }
}