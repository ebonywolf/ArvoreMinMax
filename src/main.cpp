#include <iostream>
#include "MinMaxTree.h"
#include <vector>
#include <conio.h>
#include <thread>
#include <memory>
#include "Connect4.h"
#include <time.h>
using namespace wag;
using namespace std;





int main(){

    Connect game = Connect(5);
    MinMaxTree<Connect> tree;
    game.player = 1;


    tree.setGame(&game, 1);

   // tree.update(1000);
    tree.update();
    tree.update();
    tree.update();
  //  tree.update();
   // tree.update();
   // tree.update();
   // tree.update();
   // tree.update();



    while(true){
        Mov* mov=0;

        if(game.player==0){
            int alce;
            std::cout<< "Player:" ;
            cin>>alce;
            mov = &moves[alce-1];
        }else{
            mov = tree.getMovement();
            std::cout<< "\n======PCMOVE:"<<(int)mov->mov+1 <<std::endl;
        }
        game.move(*mov );
        tree.sendCommand(mov);
        auto tnow = clock();
        while(tree.cont<2000000.0f){
            tree.update();
            if(clock() -tnow>5000){
                break;
            }
        }

    }





}
