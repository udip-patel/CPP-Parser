#include <iostream>
#include <cstring>
#include <string>


int main(){
    std::string input;
    std::cout << "PARSER - LAB 4\n\n";
    while(true){
        std::cout << "Please enter a sentence to parse:\n";
        std::cin >> input;

        for(int i = 0; i < input.length(); i++){
            std::cout << input.at(i) << std::endl;
        }
    }

}
