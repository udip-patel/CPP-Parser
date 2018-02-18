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

//declaration of recursive functions
void factor(AttrSet &v0);
void term (AttrSet &v0);



int nextIndex;//the pointer to the next element of the input sentence
char currentChar;//the current character as it is read from the input
Symbol currentSym;//the current symbol to be parsed
AttrSet symVal;//value of the current symbol

int errorFlag = 0;//whether or not an error was encountered 0/1 (T/F)

std::string input;


void nextChar(){
    //since next index represents the next index to traverse into
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



//Recursive descenting functions
void expression(AttrSet &v0){
    AttrSet v1{ 0 };
    AttrSet v2{ 0 };
    term(v1);

    while((currentSym == Symbol::plus) || (currentSym == Symbol::minus)){
        Symbol op = currentSym;//store the symbol for reference
        //continue parsing
        parseSymbol();
        term(v2);
        //perform the operation based on the symbol
        if(op == Symbol::plus) v1.num += v2.num;
        else v1.num -= v2.num;
    }
    //assign result to parent
    v0.num = v1.num;
}


void term(AttrSet &v0){
    AttrSet v1{ 0 };
    AttrSet v2{ 0 };
    factor(v1);

    while((currentSym == Symbol::times) || (currentSym == Symbol::division)){
        Symbol op = currentSym;
        parseSymbol();
        factor(v2);

        if(op == Symbol::times) v1.num *= v2.num;
        else {
            if(v2.num != 0) v1.num /= v2.num;
            else{
                errorFlag = 1;
                std::cout << "Invalid Input, Please enter the denominator. ";
            }
        }
    }
    v0.num = v1.num;
}


void factor(AttrSet &v0){
    if(currentSym == Symbol::integer){
        v0.num = symVal.num;
    }
    else if(currentSym == Symbol::lParen){
        parseSymbol();
        expression(v0);

        if(currentSym != Symbol::rParen){
            std::cout << "Error. Brackets not closed properly. ";
            errorFlag = 1;
        }
    }
    parseSymbol();
}


int main(){
    std::cout << "PARSER - LAB 4\n\n";
    while(true){
        std::cout << "Please enter a sentence to parse:\n";
        std::cin >> input;
        nextIndex = 0;

        AttrSet result{ 0 };
        errorFlag = 0;//revert errorFlag to none

        nextChar();
        parseSymbol();
        //call the actual descenting functions
        expression(result);

        if(currentSym == Symbol::eof){
            if(errorFlag == 0){
                std::cout << "successful parse. val: " << result.num << std::endl;
            }
            else{
                std::cout << "Last value tracked: " << result.num << std::endl;
            }
        }
        else{
            std::cout << "failure. Invalid Input" << std::endl;
        }
    }

}
