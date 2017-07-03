

#ifndef GAMES
#define GAMES

#include <iostream>
#include "minmax.h"
using namespace std;

struct Movement {
	int x, y;
	bool operator==(Movement& m ){
        return x == m.x && y == m.y;
	}
	friend ostream& operator<<(ostream& os, Movement& g){
	    os<< g.x<<" "<<g.y ;
        return os;

	}
};

struct InvVelha: public wag::Game<Movement> {
	vector<vector<char>> board;
	char done=0;
	bool player ;
	InvVelha() {}
	InvVelha ( int x, int y )
	{
		board.resize ( x );
		for ( int i = 0; i < x; i++ ) {
			board[i].resize ( y );
		}
		for (int i=0;i<board.size();i++){
            for (int j=0;j<board[i].size();j++){
                Movement novo = {i,j};
                moves[i][j]=novo;
            }
		}
	}
	bool getPlayer(){return player;}
	bool move ( Movement t ) override
	{
		char c = 'X';
		if ( player ) { c = 'O'; }

		if ( board[t.x][t.y] != 0 ) { return false; } //invalid move

		board[t.x][t.y] = c ;
		player = !player;
		return true;
	};
	bool checkDefeat ( char c )
	{
		for ( int i = 0; i < board.size(); i++ ) {
			for ( int j = 0; j < board[i].size() ; j++ ) {
                if(i>0 && i < board.size()-1 )
                    if( board[i][j] == c   &&  board[i-1][j] == c &&  board[i+1][j] == c)return true;

                if(j>0 && j < board[i].size()-1 )
                    if( board[i][j] == c   &&  board[i][j+1] == c &&  board[i][j-1] == c)return true;
                if(j>0 && j < board[i].size()-1 && i>0 && i < board.size()-1 ){
                    if( board[i][j] == c   &&  board[i-1][j-1] == c &&  board[i+1][j+1] == c)return true;
                    if( board[i][j] == c   &&  board[i-1][j+1] == c &&  board[i+1][j-1] == c)return true;

                }
			}
		}

		return false;
	}
	bool checkTied (){
		if ( checkDefeat ( 'O' ) || checkDefeat ( 'X' ) ) { return false; }

		int alce=0;
		for ( int i = 0; i < board.size(); i++ ) {
			for ( int j = 0; j < board[i].size(); j++ ) {
				if ( board[i][j] != 0 ) { alce++; }
			}
		}
		if( alce == board.size() * board[0].size() )return true;
		return false;

	}
	int getFitness() override
	{
		if ( checkDefeat ( 'O' ) ) {done='O'; return 5; }
		if ( checkDefeat ( 'X' ) ) {done='x'; return -5; }
		if ( checkTied () ) { done='p';return 2; }
		return 0;
	};
	static unordered_map<int, unordered_map<int, Movement>> moves;

    std::list<Movement*> getMoves ( int player ) override{
        std::list<Movement*> mo;
        if(done){
             //   std::cout<< done <<std::endl;
                return mo;
        }

	    int vic = board.size();
        for ( int i = 0; i < board.size(); i++ ) {
			for ( int j = 0; j < board[i].size(); j++ ) {
				if ( board[i][j] == 0 ) {
                    Movement* alce =&moves[i][j];
                    mo.push_back( alce );
				}
			}
		}
		return mo;
	};
	Game<Movement>* clone() {
	    InvVelha* g = new InvVelha();
	    *g = *this;
	    return g;
    }
	friend ostream& operator<<(ostream& os, InvVelha& g){
	    os<< "\n" <<std::endl;
        for (int i=0;i<g.board[0].size();i++){
            for (int j=0;j<g.board.size();j++){
                g.board[j][i] == 0 ? os<<"_" : os<<g.board[j][i] ;
                os<<" ";
            }
            os<<std::endl;
        }
        return os;

	}
};
unordered_map<int, unordered_map<int, Movement>> InvVelha::moves = unordered_map<int, unordered_map<int, Movement>>();


Movement* playerMov(){
    cout<<"Player Input:";
    int x,y;
    cin>>x>>y;
    x--;y--;
    Movement* m = &InvVelha::moves[x][y];
    return m;
}






#endif // GAMES
