#include <iostream>
#include "MinMaxTree.h"
#include <vector>
#include <conio.h>
#include <thread>
#include <memory>
#include "Connect4.h"
//#include <SFML/Network.hpp>
#include <time.h>
#include <mutex>
#include "Uttt.h"

using namespace wag;
using namespace std;
//using namespace sf;

/*
int server()
{
	sf::TcpListener listener;
	while ( true ) {

		if ( listener.listen ( 63 ) != sf::Socket::Done ) {
			std::cout << "erro1" << std::endl;
		}
		sf::TcpSocket client;
		std::cout << "waiting for connection" << std::endl;
		if ( listener.accept ( client ) != sf::Socket::Done ) {
			std::cout << "error2" << std::endl;
		}
		char buffer[1024];
        unsigned int received;
		client.receive ( ( void* ) buffer, 1024, received );
		std::cout << "Received:" <<received<<"="<<buffer << std::endl;
		string rec = string(buffer);

        int pos = rec.find("Key")+4;
        int endpos = rec.find("Sec",pos);
        string key = rec.substr(pos, endpos-pos-2 );
        std::cout<< "KEY:"<<key <<"ENDKEY"<<std::endl;

		string response= "HTTP/1.1 101 Web Socket Protocol Handshake\n";
		response+="Upgrade: WebSocket\nConnection: Upgrade\n";
		response+="WebSocket-Origin: localhost\n";
		response+="Sec-WebSocket-Accept:";;
        response+=key;
        response+="\n\r\n";

 // "WebSocket-Location: ws://{$host}{$resource}\r\n"+
        client.send ( response.c_str(), response.length());

		auto func = [&client](){
		    sf::Socket::Status stat= sf::Socket::Done;
            while(stat ==sf::Socket::Done ){
                unsigned char buffer[1024];
                unsigned int received;
                stat=client.receive ( ( void* ) buffer, 1024, received );
                buffer[1023]=0;
                std::cout << "Received:" <<received<<"="<<buffer << std::endl;
            }
		};
		thread nov(func);
        while(true){
            std::cout << "sending:"<< std::endl;
            string a;
            cin>>a;
            a+="\r\n";
            if ( client.send ( a.c_str(), a.length() ) != sf::Socket::Done ) {
                std::cout << "error?" << std::endl;
                break;
            };
        }
        nov.join();
	}
}
int client()
{
    while(true){
        sf::TcpSocket client;
        std::cout<< "Attempting to connect" <<std::endl;
        sf::Socket::Status status = client.connect ( IpAddress ( "localhost" ), 63, sf::seconds ( 5 ) );

        if(status == Socket::Error )continue;

        auto func = [&client](){
		    sf::Socket::Status stat= sf::Socket::Done;
            while(stat ==sf::Socket::Done ){
                unsigned char buffer[1024];
                unsigned int received;
                stat=client.receive ( ( void* ) buffer, 1024, received );
                buffer[1023]=0;
                std::cout << "Received:" <<received<<"="<<buffer << std::endl;
            }
		};
		thread nov(func);
		 while(true){
            std::cout << "sending:"<< std::endl;
            string a;
            cin>>a;
            a+="\r\n";
            if ( client.send ( a.c_str(), a.length() ) != sf::Socket::Done ) {
                std::cout << "error?" << std::endl;
                break;
            };
        }
        nov.join();




    }

}
*/

int main()
{
   // server();

   // client();
    std::cout<< sizeof(MinMaxTree<Connect>::Node) <<std::endl;
	Connect game = Connect ( 5 );
	TreeConfig config;
	config.limit = 20000000;

	MinMaxTree<Connect> tree(config);
	game.player = 0;
	//game.board[3][0]=2;
	//game.board[3][1]=1;


	for (int i=0;i<7;i++){
        for (int j=0;j<6;j++){
          //  game.board[i][j] = alce[5-j][i];
        }
	}

	tree.setGame ( &game, 1 );


	tree.update();
	//tree.update();


	std::mutex mut;
	mutex done;
	bool running=true;
    auto upd= [&tree,&mut, &done,&running](){
        while(running){
            auto tnow = clock();
		    while ( true) {
                mut.lock();
                tree.update(50);
                mut.unlock();

            }
        }
    };
  //  thread nova(upd);
   // nova.detach();

	while ( true ) {
		uchar mov = 0;
		std::cout<< game <<std::endl;


		if ( game.player == 0 ) {
			uchar alce;

			cin >> alce;
           // tree.update(50);
			//mov = tree.getMovement();
			//std::cout << "1 playing:"<<(int)mov<<endl; ;
			mov = alce-1-'0';//&moves[alce - 1];

		} else {
          //  mut.lock();

            tree.update(50);
			mov = tree.getMovement();
			std::cout << "2 playing:"<<(int)mov<<endl; ;
			//mut.unlock();
			//std::cout << "\n======PCMOVE:" << ( int ) mov + 1 << std::endl;
		}

        game.move ( mov );
        game.getFitness();
        if(game.done){
            std::cout<< game <<std::endl;
            std::cout<< "GAME OVER:"<<(!game.player)+1 <<std::endl;
            return 0;
        }


        mut.lock();
		tree.sendCommand ( mov );
		mut.unlock();

	}
	running=false;


}

