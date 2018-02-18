#include <iostream>
#include <cstring>
#include <string>

enum class Symbol{
    eof, plus, minus, times, division, lParen, rParen, integer, boolType, logicAnd, logicOr, negate, equals, notEquals, other
};
//struct used to hold synthesized attributes
struct AttrSet{
    int num;
};
//struct used to store the synthesized 'type' attribute. (boolean/integer)
struct TypeSet{
    std::string value;
    bool marked = false;//whether or not this type's value is changed from its initial default (whether any logical or comparison operations were run or not while parsing the sentence)
};

//declaration of recursive functions
void factor(AttrSet &v0);
void term (AttrSet &v0, TypeSet &type);
void expression(AttrSet &v0, TypeSet &type);



int nextIndex;//the pointer to the next element of the input sentence
char currentChar;//the current character as it is read from the input
Symbol currentSym;//the current symbol to be parsed
AttrSet symVal;//value of the current symbol
TypeSet type;

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
            currentSym = Symbol::rParen; nextChar();  break;
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
        case 't': case 'T':
            currentSym = Symbol::boolType;
            symVal.num = 1;
            nextChar();
            break;
        case 'f': case 'F':
            currentSym = Symbol::boolType;
            symVal.num = 0;
            nextChar();
            break;
        case '&':
            currentSym = Symbol::logicAnd; nextChar(); break;
        case '|':
            currentSym = Symbol::logicOr; nextChar(); break;
        case '~':
            currentSym = Symbol::negate; nextChar(); break;
        case '=':
            currentSym = Symbol::equals; nextChar(); break;
        case '#':
            currentSym = Symbol::notEquals; nextChar(); break;
        default:
            currentSym = Symbol::other;
    }
}



//Recursive descenting functions
void booleanExpression(AttrSet &v0, TypeSet &type){
    AttrSet v1{ 0 };
    AttrSet v2{ 0 };

    if(currentSym == Symbol::negate){
        parseSymbol();
        booleanExpression(v1, type);
        //negate the result of expanding on the boolean expression
        if(v1.num == 0){ v1.num = 1; }
        else if(v1.num == 1) { v1.num = 0; }
        else{
            errorFlag = 1;
            std::cout << "Error. Cannot Use Negation on Any value that is not True or False. ";
        }
        type.value = "boolean";
        type.marked = true;
    }
    else{
        expression(v1, type);

        while(currentSym == Symbol::logicAnd || currentSym == Symbol:: logicOr || currentSym == Symbol::equals || currentSym == Symbol::notEquals){
            Symbol op = currentSym;
            parseSymbol();
            booleanExpression(v2, type);

            if(op == Symbol::logicOr || op == Symbol::logicAnd){
                if((v1.num == 0 || v1.num == 1) && (v2.num == 0 || v2.num == 1)){
                    if(op == Symbol:: logicAnd){
                        if(v1.num == 1 && v2.num == 1) v1.num = 1;
                        else v1.num = 0;
                    }
                    else{
                        if(v1.num == 1 || v2.num == 1) v1.num = 1;
                        else v1.num = 0;
                    }
                }
                else{
                    errorFlag = 1;
                    std::cout << "Error. Cannot compare given integers with &, or |. ";
                }

            }
            else{
                if(op == Symbol::equals){
                    if(v1.num == v2.num) v1.num = 1;
                    else v1.num = 0;
                }
                else{
                    if(v1.num != v2.num) v1.num = 1;
                    else v1.num = 0;
                }
            }
            type.value = "boolean";
            type.marked = true;

        }
    }
    v0.num = v1.num;

}


void expression(AttrSet &v0, TypeSet &type){
    AttrSet v1{ 0 };
    AttrSet v2{ 0 };
    term(v1, type);

    while((currentSym == Symbol::plus) || (currentSym == Symbol::minus)){
        Symbol op = currentSym;//store the symbol for reference
        //continue parsing
        parseSymbol();
        term(v2, type);
        //perform the operation based on the symbol
        if(op == Symbol::plus) v1.num += v2.num;
        else v1.num -= v2.num;
        type.value = "integer";
    }
    //assign result to parent
    v0.num = v1.num;
}


void term(AttrSet &v0, TypeSet &type){
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
        type.value = "integer";
    }
    v0.num = v1.num;
}


void factor(AttrSet &v0){
    if(currentSym == Symbol::integer){
        v0.num = symVal.num;
    }

    if(currentSym == Symbol::boolType){
        v0.num = symVal.num;
    }

    else if(currentSym == Symbol::lParen){
        parseSymbol();
        booleanExpression(v0, type);

        if(currentSym != Symbol::rParen){
            std::cout << currentChar << " " << nextIndex << std::endl;
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
        type.value = "integer";
        errorFlag = 0;//revert errorFlag to none

        nextChar();
        parseSymbol();
        //call the actual descenting functions
        booleanExpression(result, type);

        if(currentSym == Symbol::eof){
            if(errorFlag == 0){
                std::cout << "successful parse." << std::endl;

                //if any logical or comparison op was run AND the final value is a 0 or 1, even if the last action parsed was an integer operation, we shall declare it a type boolean.
                if(type.marked && type.value == "integer"){
                    if(result.num == 0 || result.num == 1) type.value="boolean";
                }

                std::cout << "Type: " << type.value << " value: " << result.num << std::endl;
            }
            else{
                std::cout << "Last value tracked: " << result.num << std::endl;
            }
        }
        else{
            std::cout << currentChar << " " << nextIndex << std::endl;
            std::cout << "failure. Invalid Input" << std::endl;
        }
    }

}
