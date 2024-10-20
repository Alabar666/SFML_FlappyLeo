#include "flappy.hpp"

int main(){
    std::srand(std::time(nullptr));
    auto flappy = std::make_shared<FlappyBird>();
    flappy->run();
    return 0;
}