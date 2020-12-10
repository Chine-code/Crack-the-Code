#include <iostream>
#include "game_header.hpp"

Game::Game(int gam[]) noexcept
{
//    std::cout << "Created\n";
    for (int i = 0; i < size;i++)
        code[i] = gam[i];
}

Game::Game(const Game& g) noexcept
{
//   std::cout << "Copied:";
    for (int i = 0; i < size; i++) 
        code[i] = g.code[i];
}

Game&  Game::operator=(const Game& g) noexcept
{
//    std::cout << "Copy ass:";
    
    for (int i = 0; i < size; i++)
        code[i] = g.code[i];
    
    return *this;
}

Game::Game(Game&& g) noexcept
{
//    std::cout << "Moved:\n";
    for (int i = 0; i < size; i++)
        code[i] = std::move(g.code[i]);
}

Game& Game::operator=(Game&& g) noexcept
{
//    std::cout << "move ass:";
    for (int i = 0; i < size; i++)
        code[i] = std::move(g.code[i]);
        return *this;
}

size_t HashFun::operator()(const Game &g)  const// converts array to digits for bucket
{
    int sum = 0,digit = 1;
    for (int i = 1; i < size; ++i)  digit *= 10;
    for (int i = 0; i < size; i++)
    {
        sum += g.code[i];
        digit /= 10;
    }
    return sum;
}

bool HashCompare::operator()(const Game &code1,const Game &code2) const // compares two codes
{
    for (int i = 0; i < size; i++)
        if (code1.code[i] != code2.code[i])
            return false;
    
    return true;
}

std::ostream& operator<<(std::ostream& output,const Game& g)
{
    for (int i = 0; i < size; ++i)
        output << g.code[i];
    
    return output;
}

std::ofstream& operator<<(std::ofstream &write,const Game& g)
{
    for (int i = 0; i < size; ++i)
        write << g.code[i];
    
    return write;
}

