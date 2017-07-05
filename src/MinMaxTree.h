
#ifndef MINMAXTREE_H_
#define MINMAXTREE_H_
#include <vector>
#include <unordered_map>
#include <list>
#include <iostream>
#include <memory>
#include <time.h>
namespace wag
{
///just a base structure
template<class T>
struct Game {
	Game() = default;
	virtual ~Game()
	{
	}
	T getType();
	virtual bool move ( T t )
	{
		return 0;
	}
	;
	virtual int getFitness()
	{
		return 0;
	}
	;
	virtual bool getPlayer()
	{
		return 0;
	}
	;
	virtual std::list<T*> getMoves ( int player )
	{
	}
	;
	Game<T>* clone()
	{
		return new Game();
	}
};
template <class T>
struct OrderedVector{

    std::shared_ptr<T>* nodes;

    OrderedVector(int n){
        nodes = new  std::shared_ptr<T>[n];
    }
    ~OrderedVector(){
        delete[] nodes;
    }
    inline bool compare(std::shared_ptr<T> n1, std::shared_ptr<T> n2){
        return *n1 < *n2;
    }
};

struct TreeConfig {


};

template<class T>
class MinMaxTree
{
	public:
		using GAME = std::shared_ptr<T>;
		static GAME game;
		using Movement = decltype ( game->getType() );

		unsigned short level = 0;
		static const bool MIN = false;
		static const bool MAX = true;
		static long long cont;


		TreeConfig config;
		struct Node : public std::enable_shared_from_this<Node> {
			std::unordered_map<Movement*, std::shared_ptr<Node> > sons;
			std::weak_ptr<Node> dad;
			bool type = 0;
			short fitness = 0;
			short trueFitness = 0;
			Movement* mov;
			Movement* root;
			Movement* transition;

			Node()
			{
				cont++;
			}
			Node ( Node* d , bool type  , int fit, Movement* transition  ) :
				type ( type ), fitness ( fit ), trueFitness ( fit ), root ( d->root ),
				transition( transition )
			{
				dad = d->shared_from_this();
				cont++;
			}
			~Node()
			{
				cont--;

			}
			bool operator<(const Node& n )const{
                return this->fitness < n.fitness;
			}
			void acquireMoves(std::list<Movement*>& allMoves){
			    if(!dad.expired()){
                    auto alce = dad.lock();
                    allMoves.push_front(transition);
                    alce->acquireMoves(allMoves);
			    }
			}
			GAME createGame(){
			    T* alce = static_cast<T*> ( game->clone() );
                GAME future = GAME ( alce );
                std::list<Movement*> moveList;
                acquireMoves(moveList);
                for(auto x: moveList){
                    future->move(*x);
                }
                return future;
			}
			inline void updateFitness()
			{
				if ( sons.size() == 0 ) {
					return;
				};
				auto it = sons.begin();

				fitness = it->second->fitness;
				mov = it->first;
				for ( auto& it : sons ) {
					if ( type == MAX ) {
						if ( it.second->fitness > fitness ) {
							fitness = it.second->fitness;
							mov = it.first;
						} else if ( it.second->fitness == fitness && it.second->trueFitness > fitness ) {
							mov = it.first;
						}
					} else {
						if ( it.second->fitness < fitness ) {
							fitness = it.second->fitness;
							mov = it.first;
						} else if ( it.second->fitness == fitness && it.second->trueFitness < fitness ) {
							mov = it.first;
						}
					}
				}

				if ( !dad.expired() ) {
					auto alce = dad.lock();
					alce->updateFitness();
				}

			}


			void createSons ( std::list<std::shared_ptr<Node> >& next )
			{
			    GAME tnow = createGame();
				std::list<Movement*> moves = tnow->getMoves ( type );
				for ( auto movement : moves ) {
                    T* future = static_cast<T*> ( tnow->clone() );
                   // GAME future = GAME ( alce );
					future->move ( *movement );

					int fit = future->getFitness();

					std::shared_ptr<Node> novoson = std::shared_ptr<Node> ( new Node ( this , !type , fit, movement ) );

					sons[movement] = novoson;
					next.push_back ( novoson );
					delete(future);
				}
				if ( sons.size() > 0 ) {
					updateFitness();
				}

			}
			void firstUpdate ( std::unordered_map<Movement*, std::list<std::shared_ptr<Node>>>& next )
			{

				GAME tnow = createGame();
				std::list<Movement*> moves = tnow->getMoves ( type );
				for ( auto movement : moves ) {
                    T* future = static_cast<T*> ( tnow->clone() );

					future->move ( *movement );

					int fit = future->getFitness();

					std::shared_ptr<Node> novoson = std::shared_ptr<Node> ( new Node ( this , !type  , fit, movement ) );
					novoson->root = movement;
					sons[movement] = novoson;
					next[movement].push_back ( novoson );
					delete(future);
				}
				if ( sons.size() > 0 ) {updateFitness();}//TODO :novo update

			}
			void updateRoot ( Movement* novoRoot, bool first = 0 )
			{
				root = novoRoot;
				if ( !first ) {
					for ( auto& it : sons ) {
						it.second->updateRoot ( novoRoot );
					}
				} else { ///first
					for ( auto& it : sons ) {
						it.second->updateRoot ( it.first );
					}
				}
			}

		};
		using UpdateMap = std::unordered_map< Movement* , std::list<std::shared_ptr<Node>> >;

		std::shared_ptr<Node> start;
		//std::list<std::shared_ptr<Node> > nextUpdate;
		UpdateMap nextUpdate;

	public:

		MinMaxTree()
		{
		}

		MinMaxTree ( TreeConfig config ) :
			config ( config )
		{
		}
		void setGame ( T* g, bool xo=0 )
		{
			nextUpdate.clear();
			start =  std::shared_ptr<Node> ( new Node() );
			T* alce = static_cast<T*> ( g->clone() );
			game =  GAME ( alce );
			start->type = !g->getPlayer() ^xo;

			start->firstUpdate ( nextUpdate );
			// start->updateRoot(0,true );
		}
		void update ()
		{

			for ( auto& it : nextUpdate ) {
				std::list<std::shared_ptr<Node> >& next = it.second;
				std::list<std::shared_ptr<Node> > novo;
				for ( auto& node : next ) {
					node->createSons ( novo );
				}
				next.swap ( novo );
			}

		}
		void update (unsigned long int time)
		{

		    auto tnow = clock();

		    while(true){
                if(nextUpdate.size()==0){

                   return;
                }
                for ( auto& it : nextUpdate ) {
                    std::list<std::shared_ptr<Node> >& next = it.second; //proximo nodos a atualizar
                    std::list<std::shared_ptr<Node> > novo;
                    auto beg = next.begin();
                    while(beg != next.end() ){//iterating over updates
                        auto n = *beg;
                        n->createSons(novo);
                        auto now = beg;
                        beg++;
                        next.erase(now);
                        if( clock() - tnow >= time ){
                            next.splice(next.end(), novo);
                            return;
                        }
                    }
                     next.swap ( novo );
                }
                 if( clock() - tnow >= time ){
                     return;
                 }

		    }

		}

		Movement* getMovement()
		{

            std::cout<< "Fitness:"<<start->fitness <<std::endl;
			return start->mov;
		}
		void sendCommand ( Movement* m )
		{

			std::shared_ptr<Node> next;
			if ( start->sons.count ( m ) ) {
				next = start->sons[m];
			} else {
				throw "FUUU";
			}
			game->move( *m );


			auto& lista = nextUpdate[m];

			UpdateMap novoUpdate;
            if ( next->sons.size() ) { //has sons
				next->updateRoot ( 0, true );
				for ( auto& node : lista ) {
					novoUpdate[ node->root ].push_back ( node );
				}

			} else {
				next->firstUpdate ( novoUpdate );
			}

			nextUpdate = novoUpdate;
			start = next;
		}
};
template<class T>
long long MinMaxTree<T>::cont = 0;

template<class T>
std::shared_ptr<T> MinMaxTree<T>::game=std::shared_ptr<T>();


} /* namespace wag */

#endif /* MINMAXTREE_H_ */
