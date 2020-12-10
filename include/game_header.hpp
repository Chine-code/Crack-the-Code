#ifndef GAME_HEADER_HPP
#define GAME_HEADER_HPP

#include <iostream>
#include <unordered_set>
#include <fstream>

extern int size;

class Game
{
    int code[5];
public:
    Game() noexcept{for (int i = 0 ;i  < size; i++)  code[i] = 0;}
    explicit Game(int gam[]) noexcept;// default constructor initialized with array of ints
    Game(const Game& g) noexcept;// Copy Constructor
    Game& operator=(const Game& g) noexcept;// Copy Assignment
    Game& operator=(Game&& g) noexcept;// Move Assignment
    Game(Game&& g) noexcept;// Move Constructor
    int& operator[]( int &i){return code[i];}
    
    friend std::ostream& operator<<(std::ostream& output,const Game& g);
    friend std::ofstream& operator<<(std::ofstream &write,const Game& g);
    friend struct HashFun;
    friend struct HashCompare;
};   

struct HashFun
{
    size_t operator()(const Game& g) const; // converts array to digits for bucket
};

struct HashCompare
{
    bool operator()(const Game& code1,const Game& code2) const; // compares two codes
};



#endif