//
//  GameManager.cpp
//  FingerRace
//
//  Created by Emmett Butler on 4/4/13.
//
//

#include "GameManager.h"

#include "FileManager.h"
#include "Player.h"
#include "TitleSprite.h"
#include "SquareTarget.h"
#include "ScoreCounter.h"

#include <sys/timeb.h>

static GameManager *sharedInstance = NULL;

/*!
 * @return System time in seconds.
 */
long double GameManager::getCurrentTimeSeconds(){
    timeb tb;
    ftime( &tb );
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount / 1000.0;
}

/*!
 * @return Singleton game manager class.
 */
GameManager *GameManager::sharedManager(){
    if(sharedInstance == NULL){
        sharedInstance = new GameManager();
        sharedInstance->init();
    }
    return sharedInstance;
}

/*!
 * Initializes the singleton game manager class.
 * Sets screen size-dependent variables.
 * Sets acceptable colors.
 * Checks for the first run flag.
 */
void GameManager::init(){
    players = new std::list<Player *>();
    currentState = kTitleState;
    goalCheckpoints = 20;
#ifdef PHONE_SIM_TABLET
    goalCheckpoints = 20;
#endif
    matchPoints = 3;
    
    if(tabletDevice()){
        queueingTime = 2;
    } else {
        queueingTime = 1;
    }
    
    startTime = getCurrentTimeSeconds();
    
    allowedColors = new std::list<ccColor3B>();

    allowedColors->push_back(ccc3(196, 77, 88));
    allowedColors->push_back(ccc3(78, 205, 196));
    allowedColors->push_back(ccc3(199, 244, 100));
    allowedColors->push_back(ccc3(252, 182, 83));
    allowedColors->push_back(ccc3(85, 98, 112));
    allowedColors->push_back(ccc3(142, 172, 0));
    allowedColors->push_back(ccc3(255, 0, 0));
    allowedColors->push_back(ccc3(0, 255, 0));
    
    usedColors = new std::list<ccColor3B>();

    if (this->firstRun()) {
        this->tutorialActive = true;
    }
}

/*!
 * Defines fixed positions for title sprites and score counters for both phone and tablet layouts.
 */
void GameManager::setupCounterPositions(CCLayer *b){
    baseLayer = b;

    PH_P1SCPOS = CCPoint(b->boundingBox().getMinX() + 20*scaleFactor,
                         b->boundingBox().getMaxY() - 13*scaleFactor);
    PH_P2SCPOS = CCPoint(b->boundingBox().getMaxX() - 366*scaleFactor,
                         b->boundingBox().getMinY() - 30*scaleFactor);
    TAB_P1SCPOS = CCPoint(b->boundingBox().getMinX() + 20*scaleFactor,
                          b->boundingBox().getMaxY() - 13*scaleFactor);
    TAB_P2SCPOS = CCPoint(b->boundingBox().getMaxX() - 366*scaleFactor,
                          b->boundingBox().getMaxY() - 13*scaleFactor);
    TAB_P3SCPOS = CCPoint(b->boundingBox().getMinX() + 20*scaleFactor,
                          b->boundingBox().getMinY() - 30*scaleFactor);
    TAB_P4SCPOS = CCPoint(b->boundingBox().getMaxX() - 366*scaleFactor,
                          b->boundingBox().getMinY() - 30*scaleFactor);
    
    PH_P1TPOS = CCPoint(b->boundingBox().getMidX()+b->getContentSize().width/4, b->boundingBox().getMidY());
    PH_P2TPOS = CCPoint(b->boundingBox().getMidX()-b->getContentSize().width/4, b->boundingBox().getMidY());
    TAB_P1TPOS = CCPoint(b->boundingBox().getMidX()+b->getContentSize().width/4, b->boundingBox().getMidY()+b->getContentSize().height/4);
    TAB_P2TPOS = CCPoint(b->boundingBox().getMidX()-b->getContentSize().width/4, b->boundingBox().getMidY()+b->getContentSize().height/4);
    TAB_P3TPOS = CCPoint(b->boundingBox().getMidX()+b->getContentSize().width/4, b->boundingBox().getMidY()-b->getContentSize().height/4);
    TAB_P4TPOS = CCPoint(b->boundingBox().getMidX()-b->getContentSize().width/4, b->boundingBox().getMidY()-b->getContentSize().height/4);
    
    TitleSprite *p1 = new TitleSprite();  // make one just to have it around
    p1->initWithFile("square.png");
    
    PH_TSCX = b->getContentSize().width/p1->getContentSize().width/2;
    PH_TSCY = b->getContentSize().height/p1->getContentSize().height;
    TAB_TSCX = b->getContentSize().width/p1->getContentSize().width/2;
    TAB_TSCY = b->getContentSize().height/p1->getContentSize().height/2;
    
    free(p1);
}

/*!
 * Determines the location for the given player's title sprite. Dependent on phone/tablet layout.
 *
 * @param i Player identifier.
 * @return Location of title sprite.
 */
CCPoint GameManager::getLayoutPosition(int i){
    switch(i){
        case 0:
            if(tabletDevice()){
                return TAB_P1TPOS;
            } else {
                return PH_P1TPOS;
            }
            break;
        case 1:
            if(tabletDevice()){
                return TAB_P2TPOS;
            } else {
                return PH_P2TPOS;
            }
            break;
        case 2:
            return TAB_P3TPOS;
            break;
        case 3:
            return TAB_P4TPOS;
            break;
    }
    return TAB_P4TPOS;
}

/*!
 * @return Layout scale factor, dependent on phone/tablet.
 */
CCPoint GameManager::getLayoutScale(){
    if(tabletDevice()){
        return CCPoint(TAB_TSCX, TAB_TSCY);
    }
    return CCPoint(PH_TSCX, PH_TSCY);
}

/*!
 * Resets all player variables.
 */
void GameManager::resetGameState(){
    std::list<Player *> *players = this->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        p1->currentTarget->removeFromParentAndCleanup(true);
        p1->scoreLabel->removeFromParentAndCleanup(true);
        p1->shineSprite->removeFromParentAndCleanup(true);
        p1->scoreCounter->removeFromParentAndCleanup(true);
        p1->grabNotify->removeFromParentAndCleanup(true);
        if(this->tutorialActive){
            if(p1->tutMessage != NULL){
                p1->tutMessage->removeFromParentAndCleanup(true);
            }
            if(p1->tutColorMessage != NULL){
                p1->tutColorMessage->removeFromParentAndCleanup(true);
            }
        }
        p1->removeFromParentAndCleanup(true);
    }
    this->players->clear();
    this->tutorialActive = false;
}

/*!
 * @return Next available unused color.
 */
ccColor3B GameManager::getNextColor(){
    ccColor3B ret;
    for(std::list<ccColor3B>::iterator iter = allowedColors->begin(); iter != allowedColors->end(); ++iter){
        ret = *iter;
        bool found = false;
        for(std::list<ccColor3B>::iterator iter2 = usedColors->begin(); iter2 != usedColors->end(); ++iter2){
            ccColor3B test = *iter2;
            if(ret.r == test.r && ret.b == test.b && ret.g == test.g){
                found = true;
            }
        }
        if(!found){
            usedColors->push_back(ret);
            return ret;
        }
    }
    return ccc3(255, 255, 200);
}

/*!
 * Resets color list.
 */
void GameManager::resetColors(){
    usedColors->clear();
}

/*!
 * Determines the location for the given player's score counter. Dependent on phone/tablet layout.
 *
 * @param i Player identifier.
 * @return Location of score counter.
 */
CCPoint GameManager::getScoreCounterPosition(int i){

    switch(i){
        case GameManager::kPlayer1:
            if(tabletDevice()){
                return TAB_P1SCPOS;
            } else {
                return PH_P1SCPOS;
            }
            break;
        case GameManager::kPlayer2:
            if(tabletDevice()){
                return TAB_P2SCPOS;
            } else {
                return PH_P2SCPOS;
            }
            break;
        case GameManager::kPlayer3:
            return TAB_P3SCPOS;
            break;
        case GameManager::kPlayer4:
            return TAB_P4SCPOS;
            break;
    }
    return TAB_P4TPOS;
}

/*!
 * @return True if first run, false if not.
 */
bool GameManager::firstRun() {
    return !FileManager::readFile("tut");
}

/*!
 * @return Seconds since game started.
 */
long double GameManager::getElapsed(){
    return getCurrentTimeSeconds() - startTime;
}

/*!
 * @return True if pregame state is active, false if any other state.
 */
bool GameManager::pregameIsActive(){
    return currentState == kPreGameState;
}

/*!
 * @return True if game state is active, false if any other state.
 */
bool GameManager::gameIsActive(){
    return currentState == kInGameState;
}

/*!
 * @return True if title screen state is active, false if any other state.
 */
bool GameManager::titleScreenIsActive(){
    return currentState == kTitleState;
}

/*!
 * Sets pregame state and sets state change time to current system time.
 */
void GameManager::setupGame(){
    currentState = kPreGameState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

/*!
 * @return True if end game state is active, false if any other state.
 */
bool GameManager::endgameScreenIsActive(){
    return currentState == kGameOverState;
}

/*!
 * Sets game state and sets state change time to current system time.
 */
void GameManager::startGame(){
    currentState = kInGameState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

/*!
 * Sets end game state and sets state change time to current system time. Saves first run flag.
 */
void GameManager::endGame(){

    if (this->firstRun()) {
        FileManager::saveFile("tut");
    }

    currentState = kGameOverState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

/*!
 * Sets title state and sets state change time to current system time.
 */
void GameManager::setTitleState(){
    currentState = kTitleState;
    lastStateChangeTime = getCurrentTimeSeconds();
}

/*!
 * @return Time elapsed since last state change.
 */
long double GameManager::timeSinceLastStateChange(){
    return getCurrentTimeSeconds() - lastStateChangeTime;
}

/*!
 * Initializes list to contain scores for all players.
 */
void GameManager::initStats() {
    winCounts = new std::vector<int>(maxPlayers, 0);
}

/*!
 * @return True if tablet layout, false if phone layout.
 */
bool GameManager::tabletDevice(){
#ifdef PHONE_SIM_TABLET
    return true;
#endif
    CCSize screenDimensions = CCEGLView::sharedOpenGLView()->getFrameSize();
    printf("Screen: %0.2f x %0.2f\n", screenDimensions.width, screenDimensions.height);
    if(screenDimensions.width <= 960 && screenDimensions.height <= 640){
        printf("Detected small screen\n");
        return false;
    }
    printf("Detected large screen\n");
    return true;
}

/*!
 * @return True if high res device, false if low res device.
 */
bool GameManager::retinaDevice(){
    CCSize screenDimensions = CCEGLView::sharedOpenGLView()->getFrameSize();
    if(screenDimensions.width < 960 || screenDimensions.height < 640){
        printf("detected non-retina device\n");
        return false;
    }
    printf("detected retina device\n");
    return true;
}

/*!
 * @param Scale factor.
 */
void GameManager::setScaleFactor(float sf){
    this->scaleFactor = sf;
}

/*!
 * @return Scale factor.
 */
float GameManager::getScaleFactor(){
    return this->scaleFactor;
}