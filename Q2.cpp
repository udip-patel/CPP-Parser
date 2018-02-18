#include <iostream>
#include <cstring>
#include <string>

enum class Symbol{
    eof, plus, minus, times, division, lParen, rParen, integer, other
};
//struct used to hold synthesized attributes
struct AttrSet{
    int num;
};



int nextIndex;//the pointer to the next element of the input sentence
char currentChar;//the current character as it is read from the input
Symbol currentSym;//the current symbol to be parsed
AttrSet symVal;//value of the current symbol

std::string input;


void nextChar(){
    if(nextIndex < input.length()) {
        currentChar = input.at(nextIndex);
        nextIndex++;
    }
    //ELSE, end of sentence, signify this with the number 0
    else currentChar = '\0';
}

void parseSymbol(){
    switch(currentChar){
        case 0:
            currentSym = Symbol::eof; break;
        case '+':
            currentSym = Symbol::plus; nextChar(); break;
        case '-':
            currentSym = Symbol::minus; nextChar(); break;
        case '*':
            currentSym = Symbol::times; nextChar(); break;
        case '/':
            currentSym = Symbol::division; nextChar(); break;
        case '(':
            currentSym = Symbol::lParen; nextChar(); break;
        case ')':
            currentSym = Symbol::rParen; nextChar(); break;
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
            currentSym = Symbol::integer;
            symVal.num = atoi(&currentChar);
            nextChar();
            //keep taking consecutive numbers and storing the actual value in symVal
            while(isdigit(currentChar)){
                symVal.num *=10; symVal.num += atoi(&currentChar);
                nextChar();
            }
            break;
        default:
            currentSym = Symbol::other;
    }
}





int main(){
    std::cout << "PARSER - LAB 4\n\n";
    while(true){
        std::cout << "Please enter a sentence to parse:\n";
        std::cin >> input;
        nextIndex = 0;

        nextChar();
        parseSymbol();
        //call the actual descenting functions

        if(currentSym == Symbol::eof){
            std::cout << "successful parse";
        }
        else{
            std::cout << "fail";
        }
    }

}
