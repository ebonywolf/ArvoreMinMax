
#ifndef CONNECT4
#define CONNECT4

#include <vector>
#include <unordered_map>
#include <iostream>
#include <exception>
typedef unsigned char uchar;

struct Connect: public wag::Game {

	uchar board[7][6];
	uchar height[7] = {0};
	bool done=0;
	bool player=0;
	bool calledFit=0;
	Connect() =default;
	Connect ( int a )
	{
	    for (int i=0;i<7;i++){
            for (int j=0;j<6;j++){
                board[i][j]=0;
            }

	    }

	}
	virtual ~Connect()
	{

	}

	virtual bool move ( uchar t )
	{

        calledFit =0;
		board[t][ height[t] ] = player + 1;
		height[t]++;
		player = !player;

		return 0;
	}

	int checkRolls ( bool player )
	{
		uchar c = player + 1;
		uchar d = !player +1;
		int fit = 0;

		for (int i=0;i<6;i++){
            if(board[3][i]==c)fit+=6;
		}

		///vertical
		for ( int i = 0; i < 7; i++ ) {
			for ( int j = 0; j < 3; j++ ) {
               int cont=0;
                if( board[i][j]==c )cont++;
                if( board[i][1+j]==c )cont++;
                if( board[i][2+j]==c )cont++;
                if( board[i][3+j]==c )cont++;

                if( board[i][j]==d )cont-=5;
                if( board[i][1+j]==d )cont-=5;
                if( board[i][2+j]==d )cont-=5;
                if( board[i][3+j]==d )cont-=5;

                if(cont==2)fit++;
                if(cont==3)fit+=3;
                if(cont==4)fit+=201;

			}
		}
		//horizontal
		for ( int i = 0; i < 6; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
               int cont=0;
                if( board[j][i]==c )cont++;
                if( board[j+1][i]==c )cont++;
                if( board[j+2][i]==c )cont++;
                if( board[j+3][i]==c )cont++;

                if( board[j][i]==d )cont-=5;
                if( board[j+1][i]==d )cont-=5;
                if( board[j+2][i]==d )cont-=5;
                if( board[j+3][i]==d )cont-=5;
                if(cont==2)fit++;
                if(cont==3)fit+=3;
                if(cont==4)fit+=202;

			}
		}
		for (int i=0;i<4;i++){
            for (int j=0;j<3;j++){
                int cont=0;
                if( board[i][j]==c )cont++;
                if( board[i+1][j+1]==c )cont++;
                if( board[i+2][j+2]==c )cont++;
                if( board[i+3][j+3]==c )cont++;

                if( board[i][j]==d )cont-=5;
                if( board[i+1][j+1]==d )cont-=5;
                if( board[i+2][j+2]==d )cont-=5;
                if( board[i+3][j+3]==d )cont-=5;

                if(cont==2)fit++;
                if(cont==3)fit+=3;
                if(cont==4)fit+=204;

                cont=0;
                if( board[i][5-j]==c )cont++;
                if( board[i+1][4-j]==c )cont++;
                if( board[i+2][3-j]==c )cont++;
                if( board[i+3][2-j]==c )cont++;

                if( board[i][5-j]==d )cont-=5;
                if( board[i+1][4-j]==d )cont-=5;
                if( board[i+2][3-j]==d )cont-=5;
                if( board[i+3][2-j]==d )cont-=5;


                if(cont==2)fit++;
                if(cont==3)fit+=3;
                if(cont==4)fit+=208;
            }
		}
		return fit;
	}



	virtual int getFitness()
	{
	    calledFit=1;
		int fitness = 0;
		//vertical
		int p1 = checkRolls(0);
	    int p2 = checkRolls(1);

	    if( p1>150 || p2>150){
                done = true;

	    }

		return p2-p1;
	}
	;
	virtual bool getPlayer()
	{
		return player;
	}
	;
	virtual std::list<uchar> getMoves ( int player )
	{
	    if(!calledFit)getFitness();

	    std::list<uchar> lista;
	    if( done){
            return lista;
        }

		for ( int i = 0; i < 7; i++ ) {
			if ( height[i] <= 5 ) {
				lista.push_back ( i );
			}
		}
		/*
		std::cout<< "MOVES:";
		for(auto x: lista){
		    std::cout<< (int)x<<"  ";
		}
		std::cout<< "" <<std::endl;
		*/
		return lista;

	}
	;
	Game* clone()
	{
		Connect* g = new Connect();
		*g = *this;
		return g;
	}
	friend std::ostream& operator<<(std::ostream& os, Connect& c){
	    for (int i=0;i<14;i++){
            os<< "=";
	    }
	    os<<std::endl;
        for (int j=5;j>=0;j--){
            for (int i=0;i<7;i++){
                os<<(int)c.board[i][j]<<" ";
            }
            os<<std::endl;
        }
        for (int i=0;i<14;i++){
            os<< "=" ;
	    }
        return os;

	}
};




#endif // CONNECT4
