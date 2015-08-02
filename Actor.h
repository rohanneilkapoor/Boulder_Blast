#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* stw);
    virtual void doSomething() = 0;
    int getHitPts();
    void setHitPts(int hitPts);
    void decHitPts(int damage);
    bool isDead();
    void setDead();
    int getId(Actor* x);
    virtual void damaged();
    StudentWorld* getWorld();
private:
    int m_hitPts;
    int m_id;
    StudentWorld* world;
    bool dead;
};


class Player: public Actor
{
public:
    Player(int startX, int startY, StudentWorld* stw);
    void restoreHealth();
    double getHealth();
    virtual void doSomething();
    virtual void damaged();
private:
    int health;
};

class Wall: public Actor
{
public:
    Wall(int startX, int startY, StudentWorld* stw);
    virtual void doSomething();
private:
    
};

class Boulder: public Actor
{
public:
    Boulder(int startX, int startY, StudentWorld* stw);
    virtual void doSomething();
    void relocate(int x, int y);
private:
    
};

class Hole: public Actor
{
public:
    Hole(int startX, int startY, StudentWorld* stw);
    virtual void doSomething();
private:
};


class Exit: public Actor
{
public:
    Exit(int startX, int startY, StudentWorld* stw);
    virtual void doSomething();
private:
    bool isVisible;
};

class Goodie: public Actor
{
public:
    Goodie(int startX, int startY, StudentWorld* stw, int Id);
    virtual void doSomething();
    virtual void setReward(Actor* p) = 0;
private:
};

class Jewel: public Goodie
{
public:
    Jewel(int startX, int startY, StudentWorld* stw);
    virtual void setReward(Actor* p);
    virtual void doSomething();
private:
};

class ExtraLife: public Goodie
{
public:
    ExtraLife(int startX, int startY, StudentWorld* stw);
    virtual void setReward(Actor* p);
    virtual void doSomething();
private:
};

class Ammo: public Goodie
{
public:
    Ammo(int startX, int startY, StudentWorld* stw);
    virtual void setReward(Actor* p);
    virtual void doSomething();
private:
};

class RestoreHealth: public Goodie
{
public:
    RestoreHealth(int startX, int startY, StudentWorld* stw);
    virtual void setReward(Actor* p);
    virtual void doSomething();
private:
};

class Bullet: public Actor
{
public:
    Bullet(int startX, int startY, StudentWorld* stw);
    virtual void doSomething();
    void moveSelf();
    void check();
private:
    bool doNotCont = false;
};

class Robot: public Actor
{
public:
    Robot(int Id, int startX, int startY, Direction dir, StudentWorld* stw);
    void changeRestStat();
    bool getRestStat();
    void fire();
    int findFreq();
    bool checkForObstacleToFire(StudentWorld* getWorld, Direction getDirection, int getX, int getY);
    string checkForMove(bool isHorizontal, int checkNext);
    virtual void move();
    virtual void damaged();
private:
    bool restStat = true;
    int tick;
    bool isHoriz = false;
    bool isVert = false;
};
class SnarlBot: public Robot
{
public:
    SnarlBot(int startX, int startY, Direction dir, StudentWorld* stw);
    virtual void damaged();
    virtual void doSomething();
private:
};

class KleptoBot: public Robot
{
public:
    KleptoBot(int startX, int startY, StudentWorld *stw, int Id);
    bool getGoodie();
    void getDistanceBefTurn();
    virtual void doSomething();
    void doSomethingBody(bool changedRightLeft, int checkNext);
    virtual void damaged();
    Direction getRandomDir(Direction current, int &checkNext);
private:
    bool collGoodie = false;
    int collGoodieId = -1;
    int distanceBefTurn = 0;
    int steps = 0;
    bool blocked = false;
};

class AngryKleptoBot: public KleptoBot
{
public:
    AngryKleptoBot(int startX, int startY, StudentWorld *stw);
    virtual void doSomething();
    virtual void damaged();
private:
};

class KleptoBotFactory: public Actor
{
public:
    KleptoBotFactory(int startX, int startY, StudentWorld *stw, string type);
    int count(int x, int y);
    virtual void doSomething();
private:
    string m_type;
};
#endif // ACTOR_H_
