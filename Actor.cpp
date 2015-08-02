#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp




///////////////////////////////////////////////////////
///////////////////---ACTOR---/////////////////////////
///////////////////////////////////////////////////////
Actor::Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* stw) : GraphObject(imageID, startX, startY, dir)
{
    m_id = imageID;
    dead = false;
    setHitPts(0);
    setVisible(true);
    world = stw;
}
int Actor::getHitPts()
{
    cout << m_hitPts;
    return m_hitPts;
}
void Actor::setHitPts(int hitPts)
{
    m_hitPts = hitPts;
}
void Actor::decHitPts(int damage)
{
    m_hitPts -= damage;
}
StudentWorld* Actor::getWorld()
{
    return world;
}
bool Actor::isDead()
{
    return dead;
}
void Actor::setDead()
{
    dead = true;
}
int Actor::getId(Actor* x)
{
    return m_id;
}
void Actor::damaged()
{
    decHitPts(2);
    if(getHitPts() <= 0)
        setDead();
}


///////////////////////////////////////////////////////
//////////////////---PLAYER---/////////////////////////
///////////////////////////////////////////////////////
Player::Player(int startX, int startY, StudentWorld* stw) : Actor(IID_PLAYER, startX, startY,right,stw)
{
    setHitPts(20);
    health = getHitPts();
    
}

void Player::doSomething()
{
    int ch=0;
    if (getWorld()->getKey(ch))
    {
        // user hit a key this tick!
        int x=0; int y=0; int bx=0; int by=0;
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                x = getX()-1;
                bx = x-1;
                y = getY();
                by = y;
                break;
            case KEY_PRESS_RIGHT:
                x = getX()+1;
                bx = x+1;
                y = getY();
                by= y;
                setDirection(right);
                break;
            case KEY_PRESS_DOWN:
                x = getX();
                bx = x;
                y = getY()-1;
                by = y - 1;
                setDirection(down);
                break;
            case KEY_PRESS_UP:
                x = getX();
                bx = x;
                y = getY()+1;
                by = y + 1;
                setDirection(up);
                break;
            case KEY_PRESS_SPACE:
                if(getWorld()->getAmmo() > 0)
                {
                    Bullet* b = new Bullet(getX(),getY(),getWorld());
                    b->setDirection(getDirection());
                    b->moveSelf();
                    getWorld()->addActor(b);
                    getWorld()->decAmmo(1);
                }
                break;
                
        }
        Actor* j = getWorld()->getActor(x,y,IID_JEWEL);
        Actor* e = getWorld()->getActor(x,y,IID_EXIT);
        Actor* el = getWorld()->getActor(x,y,IID_EXTRA_LIFE);
        Actor* a = getWorld()->getActor(x,y,IID_AMMO);
        Actor* rh = getWorld()->getActor(x,y,IID_RESTORE_HEALTH);
        if(getWorld()->actorHere(x,y) == nullptr || j != nullptr || e != nullptr || el != nullptr || a != nullptr|| rh != nullptr)
        {
            Actor* sn = getWorld()->getActor(x,y,IID_SNARLBOT);
            Actor* kl = getWorld()->getActor(x,y,IID_KLEPTOBOT);
            Actor* akl = getWorld()->getActor(x,y,IID_ANGRY_KLEPTOBOT);
            if(sn == nullptr && kl == nullptr && akl == nullptr)
            {
                moveTo(x,y);
            }
        }
        else
        {
            
            Boulder* b = dynamic_cast<Boulder*>(getWorld()->actorHere(x,y));
            Hole* h = dynamic_cast<Hole*>(getWorld()->actorHere(bx,by));
            if(b != 0 && (getWorld()->actorHere(bx,by) == nullptr || h != 0))
            {
                moveTo(x,y);
                b->relocate(bx,by);
            }
        }
    }
}



void Player::restoreHealth()
{
    health = 20;
}

double Player::getHealth()
{
    return (double)health;
}
void Player::damaged()
{
    health -= 2;
    decHitPts(2);
    if(health > 0)
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    
    else
    {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setDead();
        getWorld()->decLives();
        getWorld()->playerDead();
    }
    
}


///////////////////////////////////////////////////////
////////////////////---WALL---/////////////////////////
///////////////////////////////////////////////////////
Wall::Wall(int startX, int startY, StudentWorld* stw) : Actor(IID_WALL, startX, startY, none, stw)
{
    
}
void Wall::doSomething()
{
    
}




///////////////////////////////////////////////////////
///////////////////---BOULDER---///////////////////////
///////////////////////////////////////////////////////
Boulder::Boulder(int startX, int startY, StudentWorld* stw) : Actor(IID_BOULDER, startX, startY, none, stw)
{
    setHitPts(10);
}
void Boulder::doSomething()
{
    
}
void Boulder::relocate(int x, int y)
{
    moveTo(x,y);
}




///////////////////////////////////////////////////////
/////////////////////---HOLE---////////////////////////
///////////////////////////////////////////////////////
Hole::Hole(int startX, int startY, StudentWorld* stw) : Actor(IID_HOLE, startX, startY, none, stw)
{
    
}
void Hole::doSomething()
{
    if(isDead())
        return;
    Boulder* b = dynamic_cast<Boulder*>(getWorld()->actorHere(getX(),getY()));
    if(b != 0)
    {
        setDead();
        b->setDead();
    }
}









///////////////////////////////////////////////////////
////////////////////---EXIT---/////////////////////////
///////////////////////////////////////////////////////
Exit::Exit(int startX, int startY, StudentWorld* stw) : Actor(IID_EXIT, startX, startY, none, stw)
{
    setVisible(false);
    isVisible = false;
}
void Exit::doSomething()
{
    if(getWorld()->getNumOfJewels() == 0 && isVisible == false)
    {
        setVisible(true);
        isVisible = true;
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY() && isVisible == true)
    {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->levelFinished();
        getWorld()->changeLevProg();
    }
}




///////////////////////////////////////////////////////
///////////////////---GOODIE---////////////////////////
///////////////////////////////////////////////////////
Goodie::Goodie(int startX, int startY, StudentWorld* stw, int Id) : Actor(Id, startX, startY, none, stw)
{
    
}
void Goodie::doSomething()
{
    if(isDead())
        return;
    Actor* p = getWorld()->getActor(getX(),getY(),IID_PLAYER);
    if(p != nullptr)
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}
///////////////////////////////////////////////////////
////////////////////---JEWEL---////////////////////////
///////////////////////////////////////////////////////
Jewel::Jewel(int startX, int startY, StudentWorld* stw) : Goodie(startX, startY, stw, IID_JEWEL)
{
    
}
void Jewel::setReward(Actor* p)
{
    if(p != nullptr)
    {
        p->getWorld()->increaseScore(50);
        getWorld()->decNumOfJewels();
    }
}
void Jewel::doSomething()
{
    Goodie::doSomething();
    Actor* p = getWorld()->getActor(getX(),getY(),IID_PLAYER);
    setReward(p);
}



///////////////////////////////////////////////////////
/////////////////---EXTRA LIFE---//////////////////////
///////////////////////////////////////////////////////
ExtraLife::ExtraLife(int startX, int startY, StudentWorld* stw) : Goodie(startX, startY, stw, IID_EXTRA_LIFE)
{
    
}
void ExtraLife::setReward(Actor* p)
{
    if(p != nullptr)
    {
        p->getWorld()->increaseScore(1000);
        p->getWorld()->incLives();
    }
}
void ExtraLife::doSomething()
{
    Goodie::doSomething();
    Actor* p = getWorld()->getActor(getX(),getY(),IID_PLAYER);
    setReward(p);
}

///////////////////////////////////////////////////////
////////////////////---AMMO---/////////////////////////
///////////////////////////////////////////////////////
Ammo::Ammo(int startX, int startY, StudentWorld* stw) : Goodie(startX, startY, stw, IID_AMMO)
{
    
}
void Ammo::setReward(Actor* p)
{
    if(p != nullptr)
    {
        p->getWorld()->increaseScore(100);
        p->getWorld()->incAmmo(20);
    }
}
void Ammo::doSomething()
{
    Goodie::doSomething();
    Actor* p = getWorld()->getActor(getX(),getY(),IID_PLAYER);
    setReward(p);
}




///////////////////////////////////////////////////////
////////////////---RESTORE HEALTH---///////////////////
///////////////////////////////////////////////////////
RestoreHealth::RestoreHealth(int startX, int startY, StudentWorld* stw) : Goodie(startX, startY, stw, IID_RESTORE_HEALTH)
{
    
}
void RestoreHealth::setReward(Actor* p)
{
    Player* pp = dynamic_cast<Player*>(getWorld()->actorHere(getX(),getY()));
    if(pp != 0)
    {
        pp->getWorld()->increaseScore(500);
        pp->restoreHealth();
    }
}
void RestoreHealth::doSomething()
{
    Goodie::doSomething();
    Player* p = dynamic_cast<Player*>(getWorld()->actorHere(getX(),getY()));
    setReward(p);
}


///////////////////////////////////////////////////////
////////////////////---BULLET---///////////////////////
///////////////////////////////////////////////////////
Bullet::Bullet(int startX, int startY, StudentWorld* stw) : Actor(IID_BULLET, startX, startY,none, stw)
{
    
}

void Bullet::check()
{
    Actor* hitWall = getWorld()->getActor(getX(),getY(),IID_WALL);
    Actor* hitBoulder = getWorld()->getActor(getX(),getY(),IID_BOULDER);
    Actor* hitPlayer = getWorld()->getActor(getX(),getY(),IID_PLAYER);
    Actor* hitSnRobot = getWorld()->getActor(getX(),getY(),IID_SNARLBOT);
    Actor* hitKlepto = getWorld()->getActor(getX(),getY(),IID_KLEPTOBOT);
    Actor* hitAngKlepto = getWorld()->getActor(getX(),getY(),IID_ANGRY_KLEPTOBOT);
    Actor* hitKlepFactory = getWorld()->getActor(getX(),getY(),IID_ROBOT_FACTORY);
    if(hitPlayer != nullptr)
    {
        hitPlayer->damaged();
        setDead();
        doNotCont = true;
    }
    else if(hitBoulder != nullptr)
    {
        hitBoulder->damaged();
        setDead();
        doNotCont = true;
    }
    else if (hitSnRobot != nullptr)
    {
        hitSnRobot->damaged();
        setDead();
        doNotCont = true;
    }
    else if(hitWall != nullptr)
    {
        setDead();
        doNotCont = true;
    }
    else if (hitKlepto != nullptr)
    {
        hitKlepto->damaged();
        setDead();
        doNotCont = true;
    }
    else if (hitAngKlepto != nullptr)
    {
        hitAngKlepto->damaged();
        setDead();
        doNotCont = true;
    }
    else if (hitKlepFactory != nullptr)
    {
        setDead();
        doNotCont = true;
    }
    
}

void Bullet::moveSelf()
{
    if(getDirection() == right)
        moveTo(getX()+1,getY());
    else if(getDirection() == left)
        moveTo(getX()-1,getY());
    else if(getDirection() == up)
        moveTo(getX(),getY()+1);
    else if(getDirection() == down)
        moveTo(getX(),getY()-1);
}
void Bullet::doSomething()
{
    if(isDead())
        return;
    check();
    if(!doNotCont)
    {
        moveSelf();
        check();
    }
}




///////////////////////////////////////////////////////
////////////////////---ROBOT---////////////////////////
///////////////////////////////////////////////////////
Robot::Robot(int Id, int startX, int startY, Direction dir, StudentWorld* stw) : Actor(Id, startX, startY,dir,stw)
{
    if(dir == right)
        isHoriz = true;
    else if(dir == up)
        isVert = true;
    tick = findFreq();
}
void Robot::changeRestStat()
{
    if(tick == findFreq())
        restStat = false;
    else
        restStat = true;
    tick--;
    if(tick == 0)
        tick = findFreq();
}
bool Robot::getRestStat()
{
    return restStat;
}
void Robot::fire()
{
    Bullet* snBullet = new Bullet(getX(),getY(),getWorld());
    getWorld()->addActor(snBullet);
    snBullet->setDirection(getDirection());
    snBullet->moveSelf();
    getWorld()->playSound(SOUND_ENEMY_FIRE);
    
}
int Robot::findFreq()
{
    int currLev = 28 - getWorld()->getLevel();
    int ttick = (currLev) / 4;
    if (ttick < 3)
        ttick = 3;
    return ttick;
}
bool Robot::checkForObstacleToFire(StudentWorld* getWorld, Direction getDirection, int getX, int getY)
{
    if(isHoriz == true && getWorld->getPlayer()->getY() == getY)
    {
        bool obstacle = false;
        bool wentInFor = false;
        int beg; int end;
        int playerX = getWorld->getPlayer()->getX();
        if(getDirection == right && getX < playerX)
        {
            beg = getX + 1;
            end = playerX;
        }
        else if(getDirection == left && getX > playerX)
        {
            beg = playerX;
            end = getX-1;
        }
        for(int i = beg; i <= end; i++)
        {
            Actor* wall = getWorld->getActor(i,getY,IID_WALL);
            Actor* boulder = getWorld->getActor(i,getY,IID_BOULDER);
            Actor* snarlBot = getWorld->getActor(i,getY,IID_SNARLBOT);
            Actor* klepBot = getWorld->getActor(i,getY,IID_KLEPTOBOT);
            Actor* angryKlepBot = getWorld->getActor(i,getY,IID_ANGRY_KLEPTOBOT);
            Actor* klepFact = getWorld->getActor(i,getY,IID_ROBOT_FACTORY);
            if(wall != nullptr || boulder != nullptr || snarlBot != nullptr || klepBot != nullptr || angryKlepBot != nullptr || klepFact != nullptr)
            {
                obstacle = true;
                break;
            }
            wentInFor = true;
        }
        if(obstacle == false && wentInFor == true)
        {
            return false;
        }
    }
    else if(isVert == true && getWorld->getPlayer()->getX() == getX)
    {
        bool obstacle = false;
        bool wentInFor = false;
        int beg; int end;
        int playerY = getWorld->getPlayer()->getY();
        if(getDirection == up && getY < playerY)
        {
            beg = getY + 1;
            end = playerY;
        }
        else if(getDirection == down && getY > playerY)
        {
            beg = playerY;
            end = getY-1;
        }
        for(int i = beg; i <= end; i++)
        {
            Actor* wall = getWorld->getActor(getX,i,IID_WALL);
            Actor* boulder = getWorld->getActor(getX,i,IID_BOULDER);
            Actor* snarlBot = getWorld->getActor(getX,i,IID_SNARLBOT);
            Actor* klepBot = getWorld->getActor(getX,i,IID_KLEPTOBOT);
            Actor* angryKlepBot = getWorld->getActor(getX,i,IID_ANGRY_KLEPTOBOT);
            Actor* klepFact = getWorld->getActor(i,getY,IID_ROBOT_FACTORY);
            if(wall != nullptr || boulder != nullptr || snarlBot != nullptr || klepBot != nullptr || angryKlepBot != nullptr || klepFact != nullptr)
            {
                obstacle = true;
                break;
            }
            wentInFor = true;
        }
        if(obstacle == false && wentInFor == true)
        {
            return false;
        }
    }
    return true;
}
string Robot::checkForMove(bool isHorizontal, int checkNext)
{
    if(isHorizontal == true)
    {
        Actor* wall = getWorld()->getActor(checkNext,getY(),IID_WALL);
        Actor* boulder = getWorld()->getActor(checkNext,getY(),IID_BOULDER);
        Actor* snarlBot = getWorld()->getActor(checkNext,getY(),IID_SNARLBOT);
        Actor* player = getWorld()->getActor(checkNext,getY(),IID_PLAYER);
        Actor* hole = getWorld()->getActor(checkNext,getY(),IID_HOLE);
        Actor* klep = getWorld()->getActor(checkNext,getY(),IID_KLEPTOBOT);
        Actor* angryKlep = getWorld()->getActor(checkNext,getY(),IID_ANGRY_KLEPTOBOT);
        Actor* klepFact = getWorld()->getActor(checkNext,getY(),IID_ROBOT_FACTORY);
        if(wall == nullptr && boulder == nullptr && snarlBot == nullptr && player == nullptr && hole ==nullptr && klep == nullptr && angryKlep == nullptr && klepFact == nullptr)
        {
            return "isHorizontal is true";
        }
        
    }
    else
    {
        
        Actor* wall = getWorld()->getActor(getX(),checkNext,IID_WALL);
        Actor* boulder = getWorld()->getActor(getX(),checkNext,IID_BOULDER);
        Actor* snarlBot = getWorld()->getActor(getX(),checkNext,IID_SNARLBOT);
        Actor* player = getWorld()->getActor(getX(),checkNext,IID_PLAYER);
        Actor* hole = getWorld()->getActor(getX(),checkNext,IID_HOLE);
        Actor* klep = getWorld()->getActor(getX(),checkNext,IID_KLEPTOBOT);
        Actor* angryKlep = getWorld()->getActor(getX(),checkNext,IID_ANGRY_KLEPTOBOT);
        Actor* klepFact = getWorld()->getActor(getX(),checkNext,IID_ROBOT_FACTORY);
        if(wall == nullptr && boulder == nullptr && snarlBot == nullptr && player == nullptr && hole ==nullptr && klep == nullptr && angryKlep == nullptr && klepFact == nullptr)
        {
            return "isVertical is true";
        }
        
    }
    return "false";
}
void Robot::move()
{
    int checkNext;
    bool changedRightLeft = false;
    Direction opp;
    if(getDirection() == right)
    {
        checkNext = getX() + 1;
        opp = left;
        changedRightLeft = true;
    }
    else if(getDirection() == left)
    {
        checkNext = getX() - 1;
        opp = right;
        changedRightLeft = true;
    }
    else if(getDirection() == up)
    {
        checkNext = getY() + 1;
        opp = down;
    }
    else
    {
        checkNext = getY() - 1;
        opp = up;
    }
    if(checkForMove(changedRightLeft,checkNext) == "isHorizontal is true")
        moveTo(checkNext,getY());
    else if(checkForMove(changedRightLeft,checkNext) == "isVertical is true")
        moveTo(getX(),checkNext);
    else
        setDirection(opp);
}

void Robot:: damaged()
{
    if(getHitPts() > 0)
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    decHitPts(2);
    if(getHitPts() <= 0)
    {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
    }
}




///////////////////////////////////////////////////////
///////////////////---SNARLBOT---//////////////////////
///////////////////////////////////////////////////////
SnarlBot::SnarlBot(int startX, int startY, Direction dir, StudentWorld* stw) : Robot(IID_SNARLBOT, startX, startY,dir,stw)
{
    setHitPts(10);
}


void SnarlBot::doSomething()
{
    if(isDead())
        return;
    changeRestStat();
    if(getRestStat() == false)
    {
        if(checkForObstacleToFire(getWorld(), getDirection(), getX(), getY()) == false)
            fire();
        else
            move();
    }
}
void SnarlBot:: damaged()
{
    Robot::damaged();
    if(getHitPts() <= 0)
        getWorld()->increaseScore(100);
    
}




///////////////////////////////////////////////////////
//////////////////---KLEPTOBOT---//////////////////////
///////////////////////////////////////////////////////
KleptoBot::KleptoBot(int startX, int startY, StudentWorld* stw, int Id = IID_KLEPTOBOT) : Robot(Id, startX, startY, right, stw)
{
    setHitPts(5);
    getDistanceBefTurn();
}
bool KleptoBot::getGoodie()
{
    Actor* restoreHealth = getWorld()->getActor(getX(),getY(), IID_RESTORE_HEALTH);
    Actor* extraLife = getWorld()->getActor(getX(),getY(), IID_EXTRA_LIFE);
    Actor* ammo = getWorld()->getActor(getX(),getY(), IID_AMMO);
    int x = rand()%10;
    if(x == 3)
    {
        if(restoreHealth != nullptr)
        {
            restoreHealth->setDead();
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            collGoodie = true;
            collGoodieId = IID_RESTORE_HEALTH;
            return true;
        }
        else if(extraLife != nullptr)
        {
            extraLife->setDead();
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            collGoodie = true;
            collGoodieId = IID_EXTRA_LIFE;
            return true;
        }
        else if(ammo != nullptr)
        {
            ammo->setDead();
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            collGoodie = true;
            collGoodieId = IID_AMMO;
            return true;
        }
    }
    return false;
}
void KleptoBot::getDistanceBefTurn()
{
    int x = rand()%6;
    distanceBefTurn = x + 1;
    steps = 0;
}
GraphObject::Direction KleptoBot::getRandomDir(Direction current, int &checkNext)
{
    Direction newDir = getDirection();
    int x = rand()%4;
    bool isHorizontal = false;
    if(current == right || current == left)
        isHorizontal = true;
    do
    {
        switch(x)
        {
            case 0:
                newDir = up;
                checkNext += 1;
                break;
            case 1:
                newDir = down;
                checkNext -= 1;
                break;
            case 2:
                newDir = right;
                checkNext += 1;
                break;
            case 3:
                newDir = left;
                checkNext -= 1;
                break;
        }
        setDirection(newDir);
    }while(checkForMove(isHorizontal, checkNext) == "false");
    return newDir;
}
void KleptoBot:: damaged()
{
    Robot::damaged();
    if(getHitPts() <= 0)
    {
        getWorld()->increaseScore(10);
        if(collGoodie)
        {
            switch(collGoodieId)
            {
                case IID_RESTORE_HEALTH:
                {
                    RestoreHealth* rh = new RestoreHealth(getX(),getY(),getWorld());
                    getWorld()->getActorList().push_back(rh);
                    break;
                }
                case IID_AMMO:
                {
                    Ammo* a = new Ammo(getX(),getY(),getWorld());
                    getWorld()->getActorList().push_back(a);
                    break;
                }
                case IID_EXTRA_LIFE:
                {
                    ExtraLife* el = new ExtraLife(getX(),getY(),getWorld());
                    getWorld()->getActorList().push_back(el);
                    break;
                }
            }
        }
    }
}
void KleptoBot::doSomethingBody(bool changedRightLeft, int checkNext)
{
    if(getDirection() == right)
    {
        changedRightLeft = true;
        checkNext = getX() + 1;
    }
    else if(getDirection() == left)
    {
        changedRightLeft = true;
        checkNext = getX() - 1;
    }
    else if(getDirection() == up)
    {
        checkNext = getY() + 1;
    }
    else if(getDirection() == down)
    {
        checkNext = getY() - 1;
    }
    if(collGoodie == false && getGoodie() == true)
    {
        getGoodie();
        return;
    }
    else if(steps != distanceBefTurn && checkForMove(changedRightLeft,checkNext) != "false")
    {
        if(checkForMove(changedRightLeft,checkNext) == "isHorizontal is true")
        {
            moveTo(checkNext,getY());
            steps++;
        }
        else if(checkForMove(changedRightLeft,checkNext) == "isVertical is true")
        {
            moveTo(getX(),checkNext);
            steps++;
        }
    }
    else if(steps == distanceBefTurn || checkForMove(changedRightLeft,checkNext) == "false")
    {
        steps = 0;
        getDistanceBefTurn();
        getRandomDir(getDirection(), checkNext);
        doSomething();
    }
}

void KleptoBot::doSomething()
{
    if(isDead())
        return;
    changeRestStat();
    bool changedRightLeft = false;
    int checkNext = 0;
    if(getRestStat() == false)
    {
        doSomethingBody(changedRightLeft,checkNext);
    }
}




///////////////////////////////////////////////////////
///////////////---ANGRY KLEPTOBOT---///////////////////
///////////////////////////////////////////////////////
AngryKleptoBot::AngryKleptoBot(int startX, int startY, StudentWorld* stw) : KleptoBot(startX, startY, stw, IID_ANGRY_KLEPTOBOT)
{
    setHitPts(8);
}

void AngryKleptoBot::damaged()
{
    KleptoBot::damaged();
    if(getHitPts() <= 0)
    {
        getWorld()->increaseScore(10);
    }
}
void AngryKleptoBot::doSomething()
{
    if(isDead())
        return;
    changeRestStat();
    bool changedRightLeft = false;
    int checkNext = 0;
    if(getRestStat() == false)
    {
        if(checkForObstacleToFire(getWorld(), getDirection(), getX(), getY()) == false)
        {
            fire();
            return;
        }
        doSomethingBody(changedRightLeft,checkNext);
    }
}




///////////////////////////////////////////////////////
//////////////---KLEPTOBOT FACTORY---//////////////////
///////////////////////////////////////////////////////
KleptoBotFactory::KleptoBotFactory(int startX, int startY, StudentWorld* stw, string type) : Actor(IID_ROBOT_FACTORY, startX, startY,none,stw)
{
    m_type = type;
}
int KleptoBotFactory::count(int x, int y)
{
    int countBots = 0;
    for(int i = x - 3; i <= x + 3; i++)
    {
        for(int j = y - 3; j < y + 3; j++)
        {
            if(i >= 0 && j >= 0 && i <= 14 && j <= 14)
            {
                Actor* klep = getWorld()->getActor(i,j,IID_KLEPTOBOT);
                Actor* angryKlep = getWorld()->getActor(i,j,IID_ANGRY_KLEPTOBOT);
                if(klep != nullptr || angryKlep != nullptr)
                    countBots++;
            }
        }
    }
    return countBots;
}
void KleptoBotFactory::doSomething()
{
    Actor* klep = getWorld()->getActor(getX(),getY(),IID_KLEPTOBOT);
    if(count(getX(),getY()) < 3 && klep == nullptr)
    {
        int x = rand()%50;
        if(x == 3)
        {
            if(m_type == "KleptoBot")
            {
                KleptoBot* klep = new KleptoBot(getX(),getY(),getWorld(),IID_KLEPTOBOT);
                getWorld()->getActorList().push_back(klep);
            }
            else
            {
                AngryKleptoBot* angryKlep = new AngryKleptoBot(getX(),getY(),getWorld());
                getWorld()->getActorList().push_back(angryKlep);
            }
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}





