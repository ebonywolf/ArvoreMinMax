
#ifndef CONNECT4
#define CONNECT4

#include <vector>
#include <unordered_map>
typedef unsigned char uchar;

struct Mov {
	uchar mov;
};
std::unordered_map <int , Mov> moves;

struct Connect: public wag::Game<Mov> {

	uchar board[7][6];
	uchar height[7] = {0};
	bool done=0;
	bool player=0;

	Connect() = default;
	Connect ( int a )
	{
		for ( int i = 0; i < 7; i++ ) {
			Mov mov = {i};
			moves[i] = mov;
		}
	}
	virtual ~Connect()
	{
	}
	Mov getType();
	virtual bool move ( Mov t )
	{

		board[t.mov][ height[t.mov] ] = player + 1;
		height[t.mov]++;
		player = !player;
		return 0;
	}
	int checkRolls ( bool player )
	{
		uchar c = player + 1;
		int fit = 0;
		for ( int i = 0; i < 7; i++ ) {
			int cont[2] = {0};
			bool inroll[2] = {0};
			for ( int j = 0; j < 7; j++ ) {
				if ( j < 6 ) {
					if ( board[i][j] == c ) { //vertical
						if ( inroll[0] ) {
							cont[0]++;
						} else {
							inroll[0] = true;
							cont[0] = 1;
						}
					} else {
						inroll[0] = false;
						cont[0] = 1;
					}
					if ( cont[0] == 2 ) { fit++; }
					if ( cont[0] == 3 ) { fit += 5; }
					if ( cont[0] == 4 ) { fit += 200; }

				}
				if ( i < 6 ) {
					if ( board[j][i] == c ) { //horizontal
						if ( inroll[1] ) {
							cont[1]++;
						} else {
							inroll[1] = true;
							cont[1] = 1;

						}
					} else {
						inroll[1] = false;
						cont[1] = 1;
					}
					if ( cont[1] == 2 ) { fit++; }
					if ( cont[1] == 3 ) { fit += 5; }
					if ( cont[1] == 4 ) { fit += 200; }
				}
			}
		}
		for (int i=0;i<4;i++){
            for (int j=0;j<3;j++){
                int cont=0;
                if( board[i][j]==c )cont++;
                if( board[i+1][j+1]==c )cont++;
                if( board[i+2][j+2]==c )cont++;
                if( board[i+3][j+3]==c )cont++;

                if(cont==2)fit++;
                if(cont==3)fit+=3;
                if(cont==4)fit+=200;

                cont=0;
                if( board[i][5-j]==c )cont++;
                if( board[i+1][4-j]==c )cont++;
                if( board[i+2][3-j]==c )cont++;
                if( board[i+3][2-j]==c )cont++;

                if(cont==2)fit++;
                if(cont==3)fit+=3;
                if(cont==4)fit+=200;


            }
		}



		return fit;
	}



	virtual int getFitness()
	{
		int fitness = 0;
		//vertical
		int p1 = checkRolls(0);
	    int p2 = checkRolls(1);

	    if( p1>200 || p2>200){
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
	virtual std::list<Mov*> getMoves ( int player )
	{
	    std::list<Mov*> lista;
	    if( done){
            return lista;
        }

		for ( int i = 0; i < 7; i++ ) {
			if ( height[i] <= 5 ) {
				lista.push_back ( &moves[i] );
			}
		}
		return lista;

	}
	;
	Game<Mov>* clone()
	{
		Connect* g = new Connect();
		*g = *this;
		return g;

	}
};




#endif // CONNECT4
