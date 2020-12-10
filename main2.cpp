#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>

constexpr int factorial(const int &n)
{
    int result = 1;
    for (int i = 1 ; i <= n; i++)
        result *= i;
    
    return result;
}

int permutation(int &&n,const int &size)
// finds the exact number of codes for each bag
{
    return factorial(n) / factorial(n - size);
}

void fillbag(int size,std::vector< std::vector<short> > &game)
{
    short bag[] = {1,2,3,4,5,6,7,8,9};// bag for permutating all possible code occurences
    int count = 0;
    do
    {
        std::vector<short> code(size);

        for (short i = 0; i < size; ++i)
            code[i] = bag[i];

        if (count == 0) game[0] = code;
        else if (code == game[count - 1] && count > 0)continue;
        else    game[count] = (code);
        ++count;
    }while (std::next_permutation(bag,bag + 9)); // loops until all possible permutation has been achieved
}

std::ostream& operator<<(std::ostream& output, std::vector<short> &f)
{
    for (int j = 0; j < f.size(); j++)
        output << f[j];

    return output;
}

void getPlayerCode(int array[],const int &size)
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
        // handles alphabets or zero character(s)
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
        getPlayerCode(array,size); // get player code again
    }
    std::cin.clear();
    std::cin.ignore(100,'\n'); // clear invalid input
}

struct Game
{
    int array[6];
    static int size;
    Game(int a[],int size)
    {
        this->size = size;
        for (int i = 0; i < size; i++)  array[i] = a[i];
    }
};
int Game::size;

// operator overloading for game objects for both ostream& and ofstream&
template<typename T>
T& operator<<(T& output,const Game &f)
{
    for (int j = 0; j < f.size; j++)
        output << f.array[j];

    return output;
}

int getRandomvalue(std::vector< std::vector<short> > &g,std::default_random_engine &engine)
// generates random number for AI
{
    std::uniform_int_distribution<int> dist(0,g.size() - 1);
    return dist(engine);
}

bool getPotOrPan(const Game &caller,const Game &callee,const int &size,bool AI,std::array<short,2> &potAndPan);

void removeBadCodes(std::vector< std::vector<short> > &codes,std::array<short,2> pot_and_pan,Game &AI_call,const int &size);


int main()
{
    srand( time(nullptr) );
    int digits;// no of digits for game
    std::cout << "How many digits do u want to use to play: "
              << "\nChoose between 2 and 5\n";
    
    do
    {
        std::cin >> digits;
    }while(digits > 5 || digits < 2);

    const int size = digits;
// a code is made up of max 9 digits hence the total number of possible codes is/are 9 permutation (size)
    std::vector < std::vector<short> > codes(permutation(9,size));
    fillbag(size,codes);// fills the bag with values of all possible code(s)

    std::string username;
    std::cout << "Enter Username: ";
    std::cin >> username;

    std::fstream file("Result.txt",std::ios::trunc | std::ios::out | std::ios::in);
    if (!file.is_open())    {std::cout << "Result sheet not available";return -1;}

    int player[size] = {0};// used to initialise both player code and AI code through Game objects
    std::cout << "Write your code: ";
    getPlayerCode(player,size);// gets player's code

    Game Player(player,size);// initialise player
    std::cout << "\nPlayer code " << Player << '\t'; // player code

// writes to file("Result.txt") player and AI code
    file << username << '(' << Player << ')' 
         << std::setw(64) << std::setiosflags(std::ios::right)
         << "AI(Artificial Intelligence) "  << '[';
    
    for (int i = 0; i < size; i++)  file << '*';
    file << ']' << std::endl;

    std::cout << "AI code is ";
    for (int i = 0; i < size; i++)  std::cout << '*';

// set AI code by random number gneration
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    int random = getRandomvalue(codes,eng);
// assign AI code to object    
    std::vector<short> ai_code = codes[random];// used by AI to generate code and to call code

/*    std::cout << "Code is ";
    for (int i = 0; i < size; i++)  std::cout << ai_code[i];
*/
    for (int i = 0; i < size ; i++) player[i] = ai_code[i];
    Game AI(player,size); // AI code initialised here

    bool AI_wins = false,player_wins = false;// indicates when either AI or Player has won
    std::array<short,2> AI_thinks;// when AI makes a call,it stores pot and pan values in here

    while(!AI_wins && !player_wins) // loops until there is a winner or AI tries (max_tries) number of times
    {
        // Player call begins here
        std::cout << "\nYour turn make a call: ";
        getPlayerCode(player,size);

        Game codeCalled(player,size);// code called by Player
        std::cout << "You called " << codeCalled << std::endl;
        std::cout << "You have ";
        
        player_wins = getPotOrPan(codeCalled,AI,size,false,AI_thinks);// false when player calls
        // player call ends here

        // AI calls
        std::vector<short> repititon = {0};
        std::random_shuffle(codes.begin(),codes.end());
        int count = 0;
        do
        {
            ai_code = codes[count];
        }while( codes[count++][0] == 0 );// if it is empty,then repeat,it is empty if first digit is zero
        
        for (int i = 0; i < size ; i++) player[i] = ai_code[i];
        Game AI_call(player,size); // AI call initialised here

        std::cout << "\nAI calls " << AI_call << std::endl;
        std::cout  << "AI gets ";
        AI_wins = getPotOrPan(AI_call,Player,size,true,AI_thinks);// true when AI calls
        removeBadCodes(codes,AI_thinks,AI_call,size);

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

    } 

    // for (int i = 0; i < codes.size(); i++)
    // {
    //     std::cout << codes[i] << '\t';
    //     if (i % 20 == 0)    std::cout << '\n';
    // }
}
bool getPotOrPan(const Game &caller,const Game &callee,const int &size,bool AI,std::array<short,2> &potAndPan)
// gets pot or/and pan for both AI and Player,if AI calls then then true
// caller is the person guessing the code
{
    std::ofstream writing("Result.txt",std::ios::app);
    
    if (!writing.is_open()) {std::cout << "Couldn't open";exit(EXIT_FAILURE); }
    
    int pot = 0,pan = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            if (caller.array[i] == callee.array[j] && i == j) ++pot;
            else if (caller.array[i] == callee.array[j]) ++pan;
        }
    
    std::cout << pot << (pot > 1 ? " pots" : " pot")
              << " and " << pan << (pan > 1 ? " pans" : " pan");

    if(AI == false) writing << '\n';// if player's turn then new line("Result.txt")
    else writing << std::setw(30) << std::setiosflags(std::ios::right);// else shift to the right
    
    writing << caller << '-' << '>' << pot << ' ' << (pot > 1 ? "pots" : "pot")
            << " and " << pan << ' ' << (pan > 1 ? "pans" : "pan");
            

    if (AI == true)// if AI calls then store values of pot and pan in array
        {potAndPan[0] = pot;potAndPan[1] = pan;writing << std::endl;}

    writing.close();
    return (pot == size && pan == 0);
}

void removeBadCodes(std::vector< std::vector<short> > &codes,std::array<short,2> pot_and_pan,Game &AI_call,const int &size)
// removes invalid possibilities
// * if code is 0 pot and 0 pan then remove all codes with all same values(4 pan)
// * else remove all codes that have the exact same result of pot and pan when called 
{
//    int pot = 0,pan = 0;

    auto signature = [&](std::vector<short> &compare, Game &codeChecked,int &pot,int &pan )->void
    {
        pot = 0;pan = 0;
        for (short i = 0; i < size; ++i)
            {
                for (short j = 0; j < size; j++)
                    {
                        if (codeChecked.array[i] == compare[j] && i == j)   ++pot;
                        else if(codeChecked.array[i] == compare[j]) ++pan;
                    }
            }
    };



    std::vector< std::vector<short> >::iterator it = codes.begin();
    if(pot_and_pan[0] == 0 && pot_and_pan[1] == 0)
    // if 0 pot and 0 pan then remove codes with all similar values(pot + pan == 4) by replacing thwm with zeros
    {         
        for (int i = 0; i < codes.size(); i++)
        {
            int pot = 0,pan = 0;
            signature(codes[i],AI_call,pot,pan);
            if (pot + pan == size)
                codes[i] = {0};
        }
    }
    else 
// if it is not 0 pot 0 pan,delete all entries that doesn't produce the same exact pot and pan vice-versa after each call 
    {
        for (int i = 0; i < codes.size(); i++)
        {
            int pot = 0,pan = 0;
            signature(codes[i],AI_call,pot,pan);
            if (pot != pot_and_pan[0] || pan != pot_and_pan[1])
                codes[i] = {0};
        }
    }
    
    // for (int j = 0; j < size; j++)// extracts the digits of code
    // {    for (int k = 0; k < size; k++) // compares one individual digit to four digits to get pot or pan
    //     {
    //         if (array[i][j] == now[k] && j == k) ++pot;
    //         else if (array[i][j] == now[k]) ++pan;
    //     }
    // }

    // if (pot != pot_and_pan[0] || pan != pot_and_pan[1])
    //     return false;

    
    // return true;
}
