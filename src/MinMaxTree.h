/*
 * MinMaxTree.h
 *
 *  Created on: 22 de jun de 2017
 *      Author: wagner
 */

#ifndef MINMAXTREE_H_
#define MINMAXTREE_H_
#include <vector>
#include <unordered_map>
#include <list>
#include <iostream>
#include <memory>
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
    virtual bool move( T t )
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
    virtual std::list<T*> getMoves( int player )
    {
    }
    ;
    Game<T>* clone()
    {
        return new Game();
    }

};
struct TreeConfig {

};

template<class T>
class MinMaxTree {
    using GAME = std::shared_ptr<T>;
    GAME game;
    using Movement = decltype ( game->getType() );

    unsigned short level = 0;
    static const bool MIN = false;
    static const bool MAX = true;


    TreeConfig config;
    struct Node : public std::enable_shared_from_this<Node> {
        std::unordered_map<Movement*, std::shared_ptr<Node> > sons;
        GAME game;
        bool type = 0;
        int fitness = 0;
        unsigned short level = 0;
        Movement* mov;
        std::weak_ptr<Node> dad;
        bool deleted = 0;

        Node()
        {
        }
        Node( Node* d , bool type , GAME game , int fit ) :
                type( type ), game( game ), fitness( fit )
        {
            dad = d->shared_from_this();
        }
        ~Node()
        {
            //    std::cout<< "deleted:"<<std::endl;
        }
        void deleteSons()
        {
            deleted = true;
            for ( auto it : sons ) {
                it.second->deleteSons();
            }

        }
        inline void updateFitness()
        {
            int alce = sons.size();
            // std::cout<< "Size:"<<alce <<std::endl;
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
                    }
                }
                else {
                    if ( it.second->fitness < fitness ) {
                        fitness = it.second->fitness;
                        mov = it.first;
                    }
                }
            }
            if ( !dad.expired() ) {
                auto alce = dad.lock();
                alce->updateFitness();
            }

        }
        void createSons( std::list<std::shared_ptr<Node> >& next )
        {
            std::list<Node*> novos;
            if ( !game ) {
                std::cout << "Invalid Game" << std::endl;
            }
            std::list<Movement*> moves = game->getMoves( type );

            for ( auto movement : moves ) {
                T* alce = static_cast<T*>( game->clone() );
                GAME future = GAME( alce );

                future->move( *movement );

                int fit = future->getFitness();

                std::shared_ptr<Node> novoson = std::shared_ptr<Node>( new Node( this , !type , future , fit ) );
                //   std::cout<<std::endl<< fit<<" "<<movement;
                //   std::cout<< *future <<std::endl;
                sons[movement] = novoson;
                next.push_back( novoson );
            }
            if ( sons.size() > 0 ) {
                updateFitness();
            }
            game.reset();
        }

    };

    std::shared_ptr<Node> start;
    std::list<std::shared_ptr<Node> > nextUpdate;
public:

    MinMaxTree()
    {
    }

    MinMaxTree( TreeConfig config ) :
            config( config )
    {
    }
    void setGame( T* g )
    {
        game = GAME( g );

        start = std::shared_ptr<Node>( new Node() );
        T* alce = static_cast<T*>( g->clone() );
        start->game = GAME( alce );
        //static_cast<GAME*> ( g->clone() );

        start->type = !game->getPlayer();
        nextUpdate.push_back( start );
    }
    void update()
    {
        std::cout << "updating:" << nextUpdate.size() << std::endl;
        std::list<std::shared_ptr<Node> > next;
        for ( auto n : nextUpdate ) {
            if ( !n->deleted ) {
                n->createSons( next );
            }
        }
        nextUpdate = next;                 //std::move(next);
    }
    Movement* getMovement()
    {
        std::cout << "Type:" << start->type << std::endl;
        for ( auto x : start->sons ) {
            std::cout << x.second->fitness << " ";
        }
        std::cout << std::endl;
        return start->mov;
    }
    void sendCommand( Movement* m )
    {
        std::shared_ptr<Node> next;
        if ( start->sons.count( m ) ) {
            next = start->sons[m];
        }
        else {
            for ( auto& it : start->sons ) {
                if ( *it.first == *m ) {
                    next = it.second;
                    break;
                }
            }
        }
        if ( next == NULL ) {
            throw "FUUU";
        }
        for ( auto& it : start->sons ) {
            if ( it.second != next ) {
                it.second->deleteSons();
                it.second->deleted = true;
            }
        }
        start->deleted = true;
        start = next;
    }
};

} /* namespace wag */

#endif /* MINMAXTREE_H_ */
