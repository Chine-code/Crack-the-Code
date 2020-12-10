#include <iostream>
#include "game_header.hpp"
#include "game_functions.hpp"
#include <iomanip>
#include <chrono>
#include <memory>

// g++ -g -std=c++17 -I include src/*.cpp (to compile)
int size; // max number of codes
static constexpr int max_tries = 8;

int main()
{
    std::cout << "How many digits do u want to use to play: "
              << "\nChoose between 2 and 5\n";
    do
    {
        std::cin >> size;
    }while(size > 5 || size < 2);
    
    std::unordered_set<Game,HashFun,HashCompare> game;// generates storage for all possible values/codes
    fillBag(game); // fill game with all possible codes
 
    std::string username;
    std::cout << "Enter Username: ";
    std::cin >> username;

    std::fstream file("Result.txt",std::ios::trunc | std::ios::out | std::ios::in);
    if (!file.is_open())    {std::cout << "Result sheet not available";return -1;}
    
    int player[size] = {0};
    std::cout << "Write your code: ";
    getPlayerCode(player);// gets player's code

    Game Player(player);// initialise player
    std::cout << "\nPlayer code " << Player << '\t'; // player code

    file << username << '(' << Player << ')' 
         << std::setw(64) << std::setiosflags(std::ios::right)
         << "AI(Artificial Intelligence) "  << '[';
    
    for (int i = 0; i < size; i++)  file << '*';
    file << ']' << std::endl;


    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    std::cout << "AI code is ";
    for (int i = 0; i < size; i++)  std::cout << '*';
    
    std::unordered_set<Game,HashFun,HashCompare>::iterator it = game.begin();
    for (int i = 0; i < getRandomvalue(game,eng); i++) ++it; // generates AI code through getRandomvalue function

    Game AI(std::move(*it)); // AI code initialised here
//    std::cout << "AI " << AI; // print AI code

    std::array<int,2> potAndPan[max_tries] = {0};// stores record for pot and pan digits respectively

    Game called_code[max_tries]; // AI stores called values it has called in this array

    int call = -1; // number of times/rounds that have been played
    bool AI_wins = false,player_wins = false;

    bool is_AI = false;
    while(!AI_wins && !player_wins) // loops until there is a winner or AI tries (max_tries) number of times
    {
        ++call;

        // Player call begins here
        std::cout << "\nYour turn make a call: ";
        getPlayerCode(player);

        Game codeCalled(player);
        std::cout << "You called " << codeCalled << std::endl;
        std::cout << "You have ";
        player_wins = getPotOrPan(potAndPan,AI,call,codeCalled,false);// false when player calls
        // player call ends here

        it = game.begin();
        // AI calls
        for (int i = 0; i < getRandomvalue(game,eng) && it != game.end(); i++) ++it;
//        std::cout << "\nI want to call " << *it; // AI sets a value at random

        Game AI_call(std::move(*it));
        
        if (call != 0) // if this is not the first time,then it compares value with previously passed
        {
            for ( it = game.begin(); false == ifCodeIsPossible(called_code,potAndPan,AI_call,call)
                  && it != game.end(); it++)
                    AI_call = std::move(*it);//Does checking for next call which must satisfy true for previous calls

            std::cout << "\nAI calls " << AI_call << std::endl;
            std::cout << "AI gets ";
            called_code[call] = std::move(AI_call); // stores AI call in array
            AI_wins = getPotOrPan(potAndPan,Player,call,AI_call,true);

            game.erase(called_code[call]);// removes called value from set of possible answers
            removesInvalidPossibilities(game,potAndPan,call,AI_call);
        }
        else // AI first time call hence no checking is done
        {
            std::cout << "\nAI calls " << AI_call << std::endl;
            std::cout << "AI gets ";
            AI_wins = getPotOrPan(potAndPan,Player,call,AI_call,true);//true when AI calls
            called_code[call] = std::move(AI_call);// call shoud be zero
            
            std::cin.get();
            game.erase(called_code[call]);// removes called value from set of possible answers
            removesInvalidPossibilities(game,potAndPan,call,AI_call);// deletes invalid possibilities
        }
    
        // checks for a win
        if (AI_wins && player_wins) std::cout << "\nIt's a tie\n";
        else if (player_wins)   std::cout << "\nYou won.\n";
        else if (AI_wins)   
        {
            std::ofstream AI_Code("Result.txt",std::ios::app);
            AI_Code << "\nAI code is " << AI;
            std::cout << "\nI guess AI will takeover the world.\n";
            AI_Code.close();
        }
        if (call == max_tries)
        {
            std::ofstream AI_Code("Result.txt",std::ios::app);
            std::cout << "I quit.You win.\n";
            std::cout << "\nAI Code is " << AI;
            AI_Code << "\nAI Code is " << AI;
            AI_Code.close();
            player_wins = true;
        }
    }
    file.close();
}