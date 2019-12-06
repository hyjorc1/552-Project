#ifndef _GUESSGAME_H
#define _GUESSGAME_H

/* A simple guess the secret game */
class GuessGame
{
public:
    /* Construct the game with the passed secret */
    GuessGame(int secret);

    /* See if the passed value is equal to the secret */
    bool guess(int value);

private:
    /* The secret that will be guessed */
    int _secret;

    /* The number of guesses */
    int _nguesses;
};

#endif