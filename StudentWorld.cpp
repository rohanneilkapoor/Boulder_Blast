#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <sstream>
#include <iomanip>


using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
    
}

int StudentWorld::init()
{
    ostringstream levelSt;
    levelSt.fill('0');
    levelSt << setw(2) << getLevel();
    string m_level = "level" + levelSt.str() + ".dat";
    Level lev(assetDirectory());
    levelFin = false;
    bonus = 1000;
    numOfJewels = 0;
    ammo = 20;
    Level::LoadResult result = lev.loadLevel(m_level);
    if (result == Level::load_fail_file_not_found || getLevel() == 100)
    {
        cerr << "Could not find level00.dat data file\n";
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level:: load_fail_bad_format)
    {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level:: load_success)
    {
        cerr << "Successfully loaded level\n";
        for(int i = 0; i < 15; i++)
        {
            for(int j = 0; j < 15; j++)
            {
                Level::MazeEntry ge = lev.getContentsOf(i,j);
                switch (ge)
                {
                    case Level::player:
                        m_player = new Player(i,j,this);
                        actorList.push_front(m_player);
                        break;
                    case Level::wall:
                        actorList.push_back(new Wall(i,j,this));
                        break;
                    case Level::boulder:
                        actorList.push_front(new Boulder(i,j,this));
                        break;
                    case Level::hole:
                        actorList.push_back(new Hole(i,j,this));
                        break;
                    case Level::jewel:
                        actorList.push_back(new Jewel(i,j,this));
                        numOfJewels++;
                        break;
                    case Level::exit:
                        actorList.push_back(new Exit(i,j,this));
                        break;
                    case Level::extra_life:
                        actorList.push_back(new ExtraLife(i,j,this));
                        break;
                    case Level::ammo:
                        actorList.push_back(new Ammo(i,j,this));
                        break;
                    case Level::restore_health:
                        actorList.push_back(new RestoreHealth(i,j,this));
                        break;
                    case Level::horiz_snarlbot:
                        actorList.push_back(new SnarlBot(i,j,GraphObject::right,this));
                        break;
                    case Level::vert_snarlbot:
                        actorList.push_back(new SnarlBot(i,j,GraphObject::up,this));
                        break;
                    case Level::kleptobot_factory:
                        actorList.push_back(new KleptoBotFactory(i,j,this,"KleptoBot"));
                        break;
                    case Level::angry_kleptobot_factory:
                        actorList.push_back(new KleptoBotFactory(i,j,this,"AngryKleptoBot"));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
    
}


Actor* StudentWorld::actorHere(int x, int y)
{
    list<Actor*>::iterator it;
    it = actorList.begin();
    while(it != actorList.end())
    {
        if((*it)->getX() == x && (*it)->getY() == y)
        {
            return *it;
        }
        it++;
    }
    return nullptr;
}

Actor* StudentWorld::getActor(int x, int y, int Id)
{
    list<Actor*>::iterator it;
    it = actorList.begin();
    while(it != actorList.end())
    {
        if((*it)->getX() == x && (*it)->getY() == y && (*it)->getId(*it) == Id)
        {
            return *it;
        }
        it++;
    }
    return nullptr;
}

list<Actor*> &StudentWorld::getActorList()
{
    return actorList;
}


int StudentWorld:: getNumOfJewels()
{
    return numOfJewels;
}

void StudentWorld:: decNumOfJewels()
{
    numOfJewels--;
}

void StudentWorld:: levelFinished()
{
    levelFin = true;
}

Player* StudentWorld:: getPlayer()
{
    return m_player;
}

int StudentWorld:: getBonus()
{
    return bonus;
}

string StudentWorld:: formatText(int score, int level, int lives, double health, int ammo, int bonus)
{
    ostringstream scoreSt;
    ostringstream levelSt;
    ostringstream livesSt;
    ostringstream healthSt;
    ostringstream ammoSt;
    ostringstream bonusSt;
    
    scoreSt.fill('0');
    scoreSt << setw(7) << score << endl;
    string m_score = scoreSt.str();
    
    levelSt.fill('0');
    levelSt << setw(2) << level << endl;
    string m_level = levelSt.str();
    
    livesSt.fill(' ');
    livesSt << setw(2) << lives << endl;
    string m_lives = livesSt.str();
    
    healthSt.fill(' ');
    double decimal = health/20;
    double percent = decimal*100;
    healthSt << setw(3) << percent << endl;
    string m_health = healthSt.str();
    
    ammoSt.fill(' ');
    ammoSt << setw(3) << ammo << endl;
    string m_ammo = ammoSt.str();
    
    bonusSt.fill(' ');
    bonusSt << setw(4) << bonus << endl;
    string m_bonus = bonusSt.str();
    
    return "Score: " + m_score + "  Level: " + m_level + "  Lives: " + m_lives + "  Health: " + m_health + "%  Ammo: " + m_ammo + "  Bonus: " + m_bonus;
}

void StudentWorld::setDisplayText()
{
    string s = formatText(getScore(),getLevel(),getLives(),m_player->getHealth(),ammo, getBonus());
    setGameStatText(s);
}
int StudentWorld::move()
{
    setDisplayText();
    list<Actor*>::iterator it;
    it = actorList.begin();
    while(it != actorList.end())
    {
        if((*it)->isDead() == false)
            (*it)->doSomething();
        if(playerDied == true)
        {
            bonus = 1000;
            playerDied = false;
            return GWSTATUS_PLAYER_DIED;
        }
        if(levelFin == true)
        {
            increaseScore(bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
        it++;
    }
    it = actorList.begin();
    while(it != actorList.end())
    {
        if((*it)->isDead())
        {
            delete *it;
            it = actorList.erase(it);
        }
        else
        {
            it++;
        }
    }
    if(bonus > 0)
        bonus--;
    if(levelFin == true)
    {
        increaseScore(bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if(playerDied == true)
    {
        bonus = 1000;
        playerDied = false;
        return GWSTATUS_PLAYER_DIED;
    }
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::playerDead()
{
    playerDied = true;
}
int StudentWorld::getAmmo()
{
    return ammo;
}
void StudentWorld::incAmmo(int x)
{
    ammo = ammo + x;
}
void StudentWorld::changeLevProg()
{
    levInProg = true;
}
void StudentWorld::decAmmo(int x)
{
    ammo = ammo - x;
}

void StudentWorld::addActor(Actor* x)
{
    actorList.push_front(x);
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it;
    it = actorList.begin();
    while(it != actorList.end())
    {
        delete *it;
        it = actorList.erase(it);
    }
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}


