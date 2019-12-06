#include "guessgame.h"

int main()
{
    //create a GuessGame with the 'secret' number 42
    GuessGame g(42);

    //now try a couple of wrong guesses
    g.guess( 12 );
    g.guess( 99 );

    //finally, try the correct guess
    g.guess( 42 );

    return 0;
}