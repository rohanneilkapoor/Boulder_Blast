#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
using namespace std;
class Actor;
class Player;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Actor* actorHere(int x, int y);
    Actor* getActor(int x, int y, int Id);
    int getNumOfJewels();
    void decNumOfJewels();
    void levelFinished();
    void playerDead();
    int getAmmo();
    void incAmmo(int x);
    void decAmmo(int x);
    string formatText(int score, int level, int lives, double health, int ammo, int bonus);
    void setDisplayText();
    int getBonus();
    void addActor(Actor* x);
    void changeLevProg();
    list<Actor*> &getActorList();
    Player* getPlayer();
    ~StudentWorld();
private:
    list<Actor*> actorList;
    Player* m_player;
    int numOfJewels = 0;
    int ammo = 20;
    bool levelFin = false;
    bool playerDied = false;
    int bonus = 1000;
    bool levInProg;
};

#endif // STUDENTWORLD_H_
