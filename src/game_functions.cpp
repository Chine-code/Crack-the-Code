#include "game_header.hpp"
#include "game_functions.hpp"
#include <algorithm> // std::next_permutation
#include <array>
#include <fstream>
#include <iomanip>
#include <random>


void fillBag(std::unordered_set<Game,HashFun,HashCompare> &game)
// fills bag with all possible input
{
    int bag[] = {1,2,3,4,5,6,7,8,9};// bag for permutating all possible code occurences

    do
    {
        int code[size];

        for (int i = 0; i < size; ++i)
            code[i] = bag[i];

        game.insert( Game(code) );
    }while (std::next_permutation(bag,bag + 9)); // loops until all possible permutation has been achieved
    // after loop game is full of  9 P size(9 permutations size) numbers

}

void getPlayerCode(int array[])
// get code from Player(could be Player's code or Player's call)
{
    int repitition[10] = {0};
    bool isValid = false;
    char c;
    repitition[0] = 1; // prevents integer zero from being passed
    for (int i = 0; i < size; ++i)
    {
        std::cin >> c;std::ios::skipws;
        if (isdigit(c) && c != '0' && repitition[c - 48] == 0)
        // checks if its a non-zero digit value or and if it has been repeated before
        {
            array[i] = c - 48; // converts character to integer
            repitition[c - 48] = 1; // sets booleanflag of integer to true
            isValid = true;
        }
        else if(c == '0' || repitition[c - 48] == 1 || isalpha(c))
        // handles alphabets or zero characters
        {
            std::cout << "Invalid input.Possible errors are either "
                      << " a repeated number or a zero value which is "
                      << "not permitted"
                      << " or an alphabet\n";
            isValid = false;
            break;
        }
        else break;// handles other errors
    }
    if (!isValid)
    {
        std::cout << "Code is not Valid\nTry again\n";
        std::cin.clear();
        std::cin.ignore(100,'\n'); // clear invalid input
        getPlayerCode(array); // get player code again
    }
}

int getRandomvalue(std::unordered_set<Game,HashFun,HashCompare> &g,std::default_random_engine &engine)
// generates random number for AI
{
    std::uniform_int_distribution<int> dist(0,g.size() - 1);
    return dist(engine);
}

void removesInvalidPossibilities(std::unordered_set<Game,HashFun,HashCompare> &g,std::array<int,2> pop[],
                                 const int &call,Game &codeChecked)
// removes invalidPossibilities 
{
    std::unordered_set<Game,HashFun,HashCompare>::iterator it = g.begin();
    auto signature = [&](Game &compare, Game &codeChecked)->int
    {
        int pot = 0,pan = 0;
        for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; j++)
                    {
                        if (codeChecked[i] == compare[j] && i == j)   ++pot;
                        else if(codeChecked[i] == compare[j]) ++pan;
                    }
            }

        return (pot + pan);
    };

    if(pop[call][0] == 0 && pop[call][1] == 0)// if 0 pot and 0 pan then remove codes with all similar values
    {         
        for (it = g.begin();it != g.end();)
        {
            Game compare(std::move(*it));
            if (signature(compare,codeChecked) == size)  it = g.erase(it);
            else    ++it;
        }
    }
    else // remove code that does not contain at least one value in it
    {
        bool isPossible = false;

        for (it = g.begin(); it != g.end();)
        {
            Game  compare(std::move(*it));
            if(signature(compare,codeChecked) == 0)  it = g.erase(it);
            else ++it;
        }
    }
}

bool getPotOrPan(std::array<int,2> potAndPan[],Game &code,int call,Game &codeCalled,bool AI)
// gets pot or/and pan for both AI and Player,if AI calls then store pot and pan values to array<int,2>
{
    std::ofstream writing("Result.txt",std::ios::app);
    
    if (!writing.is_open()) {std::cout << "Couldn't open";exit(EXIT_FAILURE); }
    
    int pot = 0,pan = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            if (codeCalled[i] == code[j] && i == j) ++pot;
            else if (codeCalled[i] == code[j]) ++pan;
        }
    
    std::cout << pot << (pot > 1 ? " pots" : " pot")
              << " and " << pan << (pan > 1 ? " pans" : " pan");

    if(AI == false) writing << '\n';// if players turn then new line
    else writing << std::setw(30) << std::setiosflags(std::ios::right);// else shift to the right
    
    writing << codeCalled << '-' << '>' << pot << ' ' << (pot > 1 ? "pots" : "pot")
            << " and " << pan << ' ' << (pan > 1 ? "pans" : "pan");
            

    if (AI == true)// if AI calls then store values of pot and pan in array
        {potAndPan[call][0] = pot;potAndPan[call][1] = pan;writing << std::endl;}


    writing.close();
    return (pot == size && pan == 0);
}

bool ifCodeIsPossible(Game array[],std::array<int,2> pot_and_pan[],Game &now,const int &call)
// returns true if it satisfies conditions of all prevously called values
{
    for (int i = 0; i < call; i++) // extracts a code from game array
    {   
        int pot = 0,pan = 0;//std::cout << std::endl;

        for (int j = 0; j < size; j++)// extracts the digits of code
        {    for (int k = 0; k < size; k++) // compares one individual digit to four digits to get pot or pan
            {
                if (array[i][j] == now[k] && j == k) ++pot;
                else if (array[i][j] == now[k]) ++pan;
            }
        }

        if (pot != pot_and_pan[i][0] || pan != pot_and_pan[i][1])
            return false;

    }
    return true;
}
