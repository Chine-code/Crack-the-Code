#ifndef GAME_FUNCTIONS_HPP
#define GAME_FUNCTIONS_HPP

#include "game_header.hpp"
#include <random>

void fillBag(std::unordered_set<Game,HashFun,HashCompare> &game);// fills bag with all possible input

void getPlayerCode(int array[]);// get code from Player(could be Player's code or Player's call)

int getRandomvalue(std::unordered_set<Game,HashFun,HashCompare> &g,std::default_random_engine &engine);
// generates random number for AI

void removesInvalidPossibilities(std::unordered_set<Game,HashFun,HashCompare> &g,std::array<int,2> pop[],
                                 const int &call,Game &codeChecked);
// removes invalidPossibilities 

bool getPotOrPan(std::array<int,2> potAndPan[],Game &code,int call,Game &codeCalled,bool AI);
// gets pot or/and pan for both AI and Player,if AI calls then store pot and pan values to array<int,2>

bool ifCodeIsPossible(Game array[],std::array<int,2> pot_and_pan[],Game &now,const int &call);
// returns true if it satisfies conditions of all prevously called values


#endif