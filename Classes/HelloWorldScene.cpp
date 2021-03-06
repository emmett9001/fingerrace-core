#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#import "Player.h"
#import "SquareTarget.h"
#import "GameManager.h"
#import "ScoreCounter.h"
#import "TitleSprite.h"

#include <sys/timeb.h>

using namespace cocos2d;

/*!
 * Constructor for HelloWorldScene.
 */
CCScene* HelloWorld::scene(){
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

/*!
 * Sets labels for title screen based on player colors.
 *
 * @param playerColors Array of player colors.
 */
void HelloWorld::setupTitleScreenTextOverlay(ccColor3B playerColors[]){
    titleLayer = CCLayer::node();
    CCLabelTTF *label = CCLabelTTF::labelWithString("BUMP", ROBOTO_FONT, 100*GameManager::sharedManager()->getScaleFactor());
    label->setRotation(-90);
    label->setPosition(CCPoint(this->boundingBox().getMidX() - 63*GameManager::sharedManager()->getScaleFactor(),
                               this->boundingBox().getMinY() + 135*GameManager::sharedManager()->getScaleFactor()));
    titleLayer->addChild(label);

    CCLabelTTF *subLabel = CCLabelTTF::labelWithString("MAP", ROBOTO_FONT, 100*GameManager::sharedManager()->getScaleFactor());
    subLabel->setRotation(-90);
    subLabel->setPosition(CCPoint(this->boundingBox().getMidX() + 55*GameManager::sharedManager()->getScaleFactor(),
                                  this->boundingBox().getMinY() + 115*GameManager::sharedManager()->getScaleFactor()));
    titleLayer->addChild(subLabel);

    insBox1 = CCSprite::spriteWithFile("square.png");
    insBox1->setColor(playerColors[1]);
    titleLayer->addChild(insBox1);

    insBox2 = CCSprite::spriteWithFile("square.png");
    insBox2->setColor(playerColors[0]);
    titleLayer->addChild(insBox2);

    if(GameManager::sharedManager()->tabletDevice()) {
        label->setColor(playerColors[2]);
        subLabel->setColor(playerColors[3]);

        insBox1->setPosition(CCPoint(this->boundingBox().getMaxX() - 60*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMidY()+this->boundingBox().size.height/4));
        insBox1->setScaleX(1.5);
        insBox1->setScaleY(7);

        // TODO - unnecessary code duplication between this and else block
        CCLabelTTF *instructionLabel1 = CCLabelTTF::labelWithString("p1 touch & hold", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
        instructionLabel1->setColor(playerColors[0]);
        instructionLabel1->setRotation(-90);
        instructionLabel1->setPosition(insBox1->getPosition());
        titleLayer->addChild(instructionLabel1);

        insBox2->setPosition(CCPoint(this->boundingBox().getMinX() + 60*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMidY()+this->boundingBox().size.height/4));
        insBox2->setScaleX(1.5);
        insBox2->setScaleY(7);

        CCLabelTTF *instructionLabel2 = CCLabelTTF::labelWithString("p2 touch & hold", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
        instructionLabel2->setColor(playerColors[1]);
        instructionLabel2->setRotation(90);
        instructionLabel2->setPosition(insBox2->getPosition());
        titleLayer->addChild(instructionLabel2);

        insBox3 = CCSprite::spriteWithFile("square.png");
        insBox3->setScaleX(1.5);
        insBox3->setScaleY(7);
        insBox3->setColor(playerColors[3]);
        insBox3->setPosition(CCPoint(this->boundingBox().getMaxX() - 60*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMidY()-this->boundingBox().size.height/4));
        titleLayer->addChild(insBox3);

        CCLabelTTF *instructionLabel3 = CCLabelTTF::labelWithString("p3 touch & hold", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
        instructionLabel3->setColor(playerColors[2]);
        instructionLabel3->setRotation(-90);
        instructionLabel3->setPosition(insBox3->getPosition());
        titleLayer->addChild(instructionLabel3);

        insBox4 = CCSprite::spriteWithFile("square.png");
        insBox4->setScaleX(1.5);
        insBox4->setScaleY(7);
        insBox4->setColor(playerColors[2]);
        insBox4->setPosition(CCPoint(this->boundingBox().getMinX() + 60*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMidY()-this->boundingBox().size.height/4));
        titleLayer->addChild(insBox4);

        CCLabelTTF *instructionLabel4 = CCLabelTTF::labelWithString("p4 touch & hold", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
        instructionLabel4->setColor(playerColors[3]);
        instructionLabel4->setRotation(90);
        instructionLabel4->setPosition(insBox4->getPosition());
        titleLayer->addChild(instructionLabel4);
    } else {
        label->setColor(playerColors[0]);
        subLabel->setColor(playerColors[1]);

        insBox1->setPosition(CCPoint(this->boundingBox().getMaxX() - 60*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMidY()));
        insBox1->setScaleX(2);
        insBox1->setScaleY(9);

        CCLabelTTF *instructionLabel1 = CCLabelTTF::labelWithString("p1 touch & hold", ROBOTO_FONT, 50*GameManager::sharedManager()->getScaleFactor());
        instructionLabel1->setColor(playerColors[0]);
        instructionLabel1->setRotation(-90);
        instructionLabel1->setPosition(insBox1->getPosition());
        titleLayer->addChild(instructionLabel1);

        insBox2->setPosition(CCPoint(this->boundingBox().getMinX() + 60*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMidY()));
        insBox2->setScaleX(2);
        insBox2->setScaleY(9);

        CCLabelTTF *instructionLabel2 = CCLabelTTF::labelWithString("p2 touch & hold", ROBOTO_FONT, 50*GameManager::sharedManager()->getScaleFactor());
        instructionLabel2->setColor(playerColors[1]);
        instructionLabel2->setRotation(90);
        instructionLabel2->setPosition(insBox2->getPosition());
        titleLayer->addChild(instructionLabel2);
    }
    
    howtoButton = CCSprite::spriteWithFile("square.png");
    howtoButton->setColor(playerColors[0]);
    howtoButton->setScaleY(5.8);
    howtoButton->setScaleX(1.5);
    howtoButton->setPosition(CCPoint(this->boundingBox().getMidX() - 45*GameManager::sharedManager()->getScaleFactor(),
                                     this->boundingBox().getMaxY() - 175*GameManager::sharedManager()->getScaleFactor()));
    titleLayer->addChild(howtoButton);
    
    CCLabelTTF *howtoLabel = CCLabelTTF::labelWithString("Instructions", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
    howtoLabel->setColor(playerColors[1]);
    howtoLabel->setRotation(90);
    howtoLabel->setPosition(howtoButton->getPosition());
    titleLayer->addChild(howtoLabel);
    
    instructions = CCLabelTTF::labelWithString("1. Touch and hold to pick your color.\n2. Keep your finger on the screen.\n3. Slide to your colored square.\n4. Go faster than your opponents.",
                                                   CCSize(0, 0), kCCTextAlignmentLeft, ROBOTO_FONT, 25*GameManager::sharedManager()->getScaleFactor());
    instructions->setColor(playerColors[1]);
    instructions->setOpacity(0);
    instructions->setAnchorPoint(CCPoint(0, 0));
    instructions->setPosition(CCPoint(this->boundingBox().getMidX() + 15*GameManager::sharedManager()->getScaleFactor(),
                                      this->boundingBox().getMaxY() - 275*GameManager::sharedManager()->getScaleFactor()));
    titleLayer->addChild(instructions);

    if (!GameManager::sharedManager()->firstRun()) {
        tutButton = CCSprite::spriteWithFile("square.png");
        tutButton->setScale(2);
        tutButton->setColor(playerColors[1]);
        tutButton->setPosition(CCPoint(this->boundingBox().getMidX() + 55*GameManager::sharedManager()->getScaleFactor(),
                                       this->boundingBox().getMaxY() - 100*GameManager::sharedManager()->getScaleFactor()));
        titleLayer->addChild(tutButton);

        tutQLabel = CCLabelTTF::labelWithString("?", ROBOTO_FONT, 80*GameManager::sharedManager()->getScaleFactor());
        tutQLabel->setRotation(90);
        tutQLabel->setColor(playerColors[0]);
        tutQLabel->setPosition(CCPoint(this->boundingBox().getMidX() + 55*GameManager::sharedManager()->getScaleFactor(),
                                       this->boundingBox().getMaxY() - 100*GameManager::sharedManager()->getScaleFactor()));
        titleLayer->addChild(tutQLabel);

        tutELabel = CCLabelTTF::labelWithString("!", ROBOTO_FONT, 80*GameManager::sharedManager()->getScaleFactor());
        tutELabel->setRotation(90);
        tutELabel->setColor(playerColors[0]);
        tutELabel->setOpacity(0);
        tutELabel->setPosition(CCPoint(this->boundingBox().getMidX() + 55*GameManager::sharedManager()->getScaleFactor(),
                                       this->boundingBox().getMaxY() - 100*GameManager::sharedManager()->getScaleFactor()));
        titleLayer->addChild(tutELabel);

        tutNotify = CCLabelTTF::labelWithString("Tutorial ON", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
        tutNotify->setPosition(CCPoint(tutButton->getPosition().x, tutButton->getPosition().y - 150*GameManager::sharedManager()->getScaleFactor()));
        tutNotify->setRotation(90);
        tutNotify->setColor(playerColors[1]);
        tutNotify->setOpacity(0);
        titleLayer->addChild(tutNotify);
    }

    this->addChild(titleLayer, 11);
}

/*!
 * Runs rotation animation on the tutorial button.
 */
void HelloWorld::animateTutButtonActivation(){
    tutQLabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, -90), NULL));
    tutQLabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 0), NULL));
    
    tutELabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, -90), NULL));
    tutELabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 255), NULL));
    
    if(!instructionsVisible){
        tutNotify->setString("Tutorial ON");
        tutNotify->runAction(
                         CCSequence::actions(
                                        CCFadeTo::actionWithDuration(.2, 255),
                                        CCFadeTo::actionWithDuration(1, 0),
                                     NULL)
                             );
    }
}

/*!
 * Runs opposite rotation animation on the tutorial button.
 */
void HelloWorld::animateTutButtonDeactivation(){
    tutQLabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, 90), NULL));
    tutQLabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 255), NULL));
    
    tutELabel->runAction(CCSequence::actions(CCRotateTo::actionWithDuration(.3, 90), NULL));
    tutELabel->runAction(CCSequence::actions(CCFadeTo::actionWithDuration(.3, 0), NULL));
    
    if(!instructionsVisible){
        tutNotify->setString("Tutorial OFF");
        tutNotify->runAction(
                             CCSequence::actions(
                                                 CCFadeTo::actionWithDuration(.2, 255),
                                                 CCFadeTo::actionWithDuration(1, 0),
                                                 NULL)
                             );
    }
}

/*!
 * Gets the scores for each player and puts labels for them on the endgame screen.
 */
void HelloWorld::setupEndgameScreenTextOverlay(){
    ccColor3B statColor = GameManager::sharedManager()->getNextColor();
    endgameLayer = CCLayer::node();
    
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;

        char p1Score[1];
        sprintf(p1Score, "%d", GameManager::sharedManager()->winCounts->at(p1->getID()));
        
        CCLabelTTF *p1ScoreLabel = CCLabelTTF::labelWithString(p1Score, ROBOTO_FONT, 200*GameManager::sharedManager()->getScaleFactor());
        p1ScoreLabel->setPosition(p1->homePoint);
        if(p1ScoreLabel->getPosition().x > this->boundingBox().getMidX()){
            p1ScoreLabel->setRotation(-90);
        } else {
            p1ScoreLabel->setRotation(90);
        }
        p1ScoreLabel->setColor(statColor);
        p1ScoreLabel->setOpacity(0);
        endgameLayer->addChild(p1ScoreLabel);
        p1ScoreLabel->runAction(CCFadeIn::actionWithDuration(1.0));
    }

    this->addChild(endgameLayer, 11);
}

/*!
 * Creates title sprites for each player and adds them to the title screen.
 */
void HelloWorld::setupTitleScreen(){
    titleSprites = new std::list<TitleSprite *>();
    titleTouchPoints = new std::list<CCPoint>();
    
    ccColor3B *playerColors = (ccColor3B *)malloc(sizeof(ccColor3B) * GameManager::sharedManager()->maxPlayers);
    TitleSprite **tempTs = (TitleSprite **)malloc((sizeof(TitleSprite *) * GameManager::sharedManager()->maxPlayers));
    
    if(!GameManager::sharedManager()->tabletDevice()){
        GameManager::sharedManager()->maxPlayers = 2;
        GameManager::sharedManager()->numPlayers = 2;
    } else {
        printf("Detected large screen\n");
        GameManager::sharedManager()->maxPlayers = 4;
    }
    
    for(int i = 0; i < GameManager::sharedManager()->maxPlayers; i++){
        TitleSprite *p1 = new TitleSprite();
        p1->initWithFile("square.png");
        p1->setPosition(GameManager::sharedManager()->getLayoutPosition(i));
        p1->setScaleX(GameManager::sharedManager()->getLayoutScale().x);
        p1->setScaleY(GameManager::sharedManager()->getLayoutScale().y);
        p1->setColor(GameManager::sharedManager()->getNextColor());
        playerColors[i] = p1->getColor();  // must happend BEFORE setupTitleScreenTextOverlay ffuuu
        this->addChild(p1, 10);
        titleSprites->push_back(p1);
        tempTs[i] = p1;
    }
    setupTitleScreenTextOverlay(playerColors);  // the time-dependency here is awful, awful, awful design - my fault
 
    tempTs[0]->button = insBox1;  // must happen AFTER setupTitleScreenTextOverlay guh
    tempTs[0]->baseScale = tempTs[0]->button->getScaleY();
    tempTs[1]->button = insBox2;
    tempTs[1]->baseScale = tempTs[1]->button->getScaleY();
    if(GameManager::sharedManager()->tabletDevice()){
        tempTs[2]->button = insBox3;
        tempTs[2]->baseScale = tempTs[2]->button->getScaleY();
        tempTs[3]->button = insBox4;
        tempTs[3]->baseScale = tempTs[3]->button->getScaleY();
    }
    
    free(tempTs);
    free(playerColors);
    
    GameManager::sharedManager()->initStats();
}

/*!
 * Destroys endgame screen and creates new title sprites for the title screen.
 */
void HelloWorld::setupTitleScreenFromEndgameScreen(){
    float initTime = .5;
    TitleSprite *p1 = titleSprites->front();
    
    ccColor3B *playerColors = (ccColor3B *)malloc(sizeof(ccColor3B) * GameManager::sharedManager()->maxPlayers);
    TitleSprite **tempTs = (TitleSprite **)malloc((sizeof(TitleSprite *) * GameManager::sharedManager()->maxPlayers));
    
    playerColors[0] = p1->getColor();
    
    p1->runAction(CCSequence::actions(
                                      CCScaleTo::actionWithDuration(initTime, GameManager::sharedManager()->getLayoutScale().x,
                                                                    GameManager::sharedManager()->getLayoutScale().y),
                                          NULL));
    p1->runAction(CCSequence::actions(
                                    CCMoveTo::actionWithDuration(initTime, GameManager::sharedManager()->getLayoutPosition(0)),
                                          NULL));
    
    for(int i = 1; i < GameManager::sharedManager()->maxPlayers; i++){
        TitleSprite *p2 = new TitleSprite();
        p2->initWithFile("square.png");
        p2->setScaleX(GameManager::sharedManager()->getLayoutScale().x);
        p2->setScaleY(GameManager::sharedManager()->getLayoutScale().y);
        p2->setPosition(CCPoint(this->boundingBox().getMinX()-p2->getContentSize().width*p2->getScaleX(), this->boundingBox().getMidY()));
        p2->setColor(GameManager::sharedManager()->getNextColor());
        playerColors[i] = p2->getColor();
        this->addChild(p2, 10);
        titleSprites->push_back(p2);
        tempTs[i] = p2;
        
        p2->runAction(CCSequence::actions(
                                          CCMoveTo::actionWithDuration(.5, GameManager::sharedManager()->getLayoutPosition(i)),
                                          NULL));
    }
    setupTitleScreenTextOverlay(playerColors);
        
    p1->button = insBox1;
    p1->touch = NULL;
        
    tempTs[1]->button = insBox2;
    if(GameManager::sharedManager()->tabletDevice()){
        tempTs[2]->button = insBox3;
        tempTs[3]->button = insBox4;
    }
    
    free(playerColors);
    free(tempTs);
}

/*!
 * Runs scaling animation on the title sprites to reveal the in-game screen.
 */
void HelloWorld::dismissTitleScreen(){
    float animationDuration = .4;
    if(titleLayer != NULL){
        titleLayer->removeFromParentAndCleanup(true);
    }
    for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
        TitleSprite *sp = *iter;
        sp->runAction(CCScaleTo::actionWithDuration(animationDuration, 0));
        
        if(sp->touch != NULL) {
            CCPoint p = CCDirector::sharedDirector()->convertToGL(sp->touch->getLocationInView());
            sp->runAction(CCMoveTo::actionWithDuration(animationDuration, p));

            titleTouchPoints->push_back(p);
        }
    }
}

/*!
 * Destroys the endgame screen.
 */
void HelloWorld::dismissEndgameScreen(){
    if(endgameLayer != NULL){
        endgameLayer->removeFromParentAndCleanup(true);
    }
}

/*!
 * Sets up a single sprite to fill the endgame screen based on the winner of the round.
 *
 * @param winner Player who won the most recent round.
 */
void HelloWorld::setupEndgameScreen(Player *winner){
    printf("Game over screen\n");
    float initTime = 0.5;
    
    titleSprites->clear();
    
    TitleSprite *p1 = new TitleSprite();
    p1->initWithFile("square.png");
    p1->setPosition(CCPoint(this->boundingBox().getMidX(), this->boundingBox().getMidY()));
    p1->setOpacity(0);
    p1->setScaleX(this->getContentSize().width/p1->getContentSize().width);
    p1->setScaleY(this->getContentSize().height/p1->getContentSize().height);
    p1->setColor(winner->color);
    this->addChild(p1, 10);
    titleSprites->push_back(p1);
    
    GameManager::sharedManager()->usedColors->push_back(winner->color);

    // increment win count stat for the winner
    if(GameManager::sharedManager()->winCounts->at(winner->getID()) == GameManager::sharedManager()->matchPoints){
        GameManager::sharedManager()->initStats();  // reset counters when match is over
    }
    GameManager::sharedManager()->winCounts->at(winner->getID())++;
    
    p1->runAction(CCFadeIn::actionWithDuration(initTime));
    setupEndgameScreenTextOverlay();
}

void HelloWorld::RemoveChildSeq(CCNode* pObj){
    this->removeChild(pObj, true);
}

/*!
 * Creates new background sprites and runs animation based on who is winning the game.
 *
 * @param lots True if game is over, false if not.
 */
void HelloWorld::iterateBackground(bool lots){
    int limit = 1 + currentWinner()->checkpointCount;
    if(limit == 0){
        limit = 1;
    }
    if(lots){
        limit = 20;
    }
    for(int i = 0; i < limit; i++){
        CCSprite *p = CCSprite::spriteWithFile("square.png");
        p->setScale(.04*(arc4random() % 10));
        p->setColor(currentWinner()->getColor());
        p->setOpacity(arc4random() % 255);
        CCFiniteTimeAction *moveAct;
        float speedFactor = (.1*(GameManager::sharedManager()->goalCheckpoints - currentWinner()->checkpointCount+1));
        CCAction* removeChild = CCCallFuncO::create(this, callfuncO_selector(HelloWorld::RemoveChildSeq), p);
        if(i % 2 == 0){
            p->setPosition(CCPoint(this->boundingBox().getMaxX(), arc4random() % (int)this->boundingBox().getMaxY()));
            moveAct = CCMoveTo::actionWithDuration(.02*speedFactor*(arc4random() % 100), CCPoint(this->boundingBox().getMinX(), p->getPosition().y));
        } else {
            p->setPosition(CCPoint(this->boundingBox().getMinX(), arc4random() % (int)this->boundingBox().getMaxY()));
            moveAct = CCMoveTo::actionWithDuration(.02*speedFactor*(arc4random() % 100), CCPoint(this->boundingBox().getMaxX(), p->getPosition().y));
        }
        this->addChild(p, 0);
        
        p->runAction(CCSequence::actions(moveAct, removeChild, NULL));
    }
}

/*!
 * Initializes the singleton game manager.
 * Sets up motion blur variables.
 * Schedules the tick method.
 */
bool HelloWorld::init(){
    if(!CCLayer::init()){ return false; }
    
    // first-time setup
    GameManager::sharedManager();

    if(!GameManager::sharedManager()->retinaDevice()){
        CCDirector::sharedDirector()->setContentScaleFactor(2);
        GameManager::sharedManager()->setScaleFactor(.5);
    } else {
        CCDirector::sharedDirector()->setContentScaleFactor(1);
        GameManager::sharedManager()->setScaleFactor(1);
    }

    GameManager::sharedManager()->setupCounterPositions(this);
    
    this->setContentSize(CCEGLView::sharedOpenGLView()->getFrameSize());
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->setTouchEnabled(true);
    this->kRenderTextureCount = 6;
    this->currentRenderTextureIndex = 0;
    
    CCSize winSize = this->getContentSize();
    renderTextures = CCArray::arrayWithCapacity(kRenderTextureCount);
    renderTextures->retain();
    
    for (int i = 0; i < kRenderTextureCount; i++){
        CCRenderTexture *rtx = CCRenderTexture::renderTextureWithWidthAndHeight((int)winSize.width, (int)winSize.height);

        rtx->setPosition(CCPointMake(winSize.width / 2, winSize.height / 2));
        
        CCSprite* renderSprite = CCSprite::spriteWithTexture(rtx->getSprite()->getTexture());
        renderSprite->setPosition(rtx->getPosition());
        
        this->addChild(renderSprite, 100 + i);
        rtx->setUserData(renderSprite);
        renderTextures->addObject(rtx);
    }
    
    setupTitleScreen();
    
    printf("Finished init\n");
    
    this->schedule(schedule_selector(HelloWorld::tick), .0001);
    
    return true;
}

/*!
 * Renders all sprites with motion blur.
 */
void HelloWorld::visit(){
    CCRenderTexture* rtx = (CCRenderTexture *)renderTextures->objectAtIndex(currentRenderTextureIndex);
    rtx->beginWithClear(0, 0, 0, 0);
    
    CCObject* node;
    CCARRAY_FOREACH(this->getChildren(), node){
        if (((CCNode *)node)->getTag() == GameManager::kMotionBlurTag){
            ((CCNode *)node)->visit();
        }
    }
    
    rtx->end();
    
    this->selectNextRenderTexture();
    int index = currentRenderTextureIndex;
    for (int i = 0; i < kRenderTextureCount; i++){
        CCRenderTexture* rtx = (CCRenderTexture*)renderTextures->objectAtIndex(currentRenderTextureIndex);
        CCSprite* renderSprite = (CCSprite*)rtx->getUserData();
        renderSprite->setOpacity((255.0f / kRenderTextureCount) * (i + 1));
        renderSprite->setScaleY(-1);
        this->reorderChild(renderSprite, 100+i);
        this->selectNextRenderTexture();
        
        index++;
        if (index >= kRenderTextureCount) {
            index = 0;
        }
    }
    
    CCARRAY_FOREACH(this->getChildren(), node){
        if (((CCNode *)node)->getTag() != GameManager::kMotionBlurTag){
            ((CCNode *)node)->visit();
        }
    }
}

void HelloWorld::selectNextRenderTexture(){
	currentRenderTextureIndex++;
	if (currentRenderTextureIndex >= kRenderTextureCount){
		currentRenderTextureIndex = 0;
	}
}

/*!
 * Main loop. Calls all relevant methods for processing each game state.
 */
void HelloWorld::tick(float dt){
    ttime = GameManager::sharedManager()->getElapsed();
    
    if(GameManager::sharedManager()->gameIsActive()){
        iterateBackground(false);
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            p1->remainingCheckpoints = GameManager::sharedManager()->goalCheckpoints - p1->checkpointCount;
            p1->updateScoreText();
            if(p1->checkpointCount >= GameManager::sharedManager()->goalCheckpoints){
                GameManager::sharedManager()->endGame();
                setupEndgameScreen(p1);
            }
            if(GameManager::sharedManager()->tutorialActive){
                if(currentWinner()->checkpointCount > GameManager::sharedManager()->goalCheckpoints - 5){
                    p1->tutMessage->setString("Faster!!");
                    p1->tutColorMessage->setVisible(false);
                } else if(p1->timeSinceLastCheckpoint() > 5 && p1->touchActive){
                    showTutorialSlideToNotify(p1, p1->tutMessage->getPosition());
                }
            }
        }
    } else if(GameManager::sharedManager()->titleScreenIsActive()){
        numQueuedPlayers = 0;
        for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
            TitleSprite *sp = *iter;
            if(sp->isQueued()){
                numQueuedPlayers++;
            }
        }
        if(numQueuedPlayers > 0){
            printf("%0.2f: queued: %d\n", ttime, numQueuedPlayers);
        }
        
        if(GameManager::sharedManager()->getCurrentTimeSeconds() - lastPlayerQueueTime > GameManager::sharedManager()->queueingTime &&
           numQueuedPlayers <= GameManager::sharedManager()->maxPlayers && numQueuedPlayers > 1){
            dismissTitleScreen();
            GameManager::sharedManager()->numPlayers = numQueuedPlayers;
            setupGameScreen();  // depends on numQueuedPlayers being correct
            GameManager::sharedManager()->setupGame();
        }
        
        numQueuedPlayers = 0;
    } else if(GameManager::sharedManager()->pregameIsActive()){
        if (GameManager::sharedManager()->timeSinceLastStateChange() > .5){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p = *iter;
                p->spawnNewTarget(nextTargetPosition(p));
            }
            GameManager::sharedManager()->startGame();
        }
    } else if(GameManager::sharedManager()->endgameScreenIsActive()){
        iterateBackground(true);
        if(GameManager::sharedManager()->timeSinceLastStateChange() > 4){
            GameManager::sharedManager()->resetGameState();
            dismissEndgameScreen();
            setupTitleScreenFromEndgameScreen();
            GameManager::sharedManager()->setTitleState();
            if(GameManager::sharedManager()->usedColors->size() >= GameManager::sharedManager()->allowedColors->size()){
                GameManager::sharedManager()->resetColors();
            }
        }
        if(GameManager::sharedManager()->tutorialActive){
            GameManager::sharedManager()->tutorialActive = false;
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                p1->tutMessage->setVisible(false);
                p1->tutColorMessage->setVisible(false);
            }
        }
    }
}

/*!
 * Creates new players, assigns their initial touch, and creates their first target.
 */
void HelloWorld::setupGameScreen(){
    for(int i = 0; i < GameManager::sharedManager()->numPlayers; i++){
        printf("Init player %d\n", i+1);
        CCTouch *t = NULL;
        CCPoint tp;
        TitleSprite *ts = NULL;
        while(ts == NULL) {
            printf("titlesprites: %d\n", titleSprites->size());
            if(titleSprites->front()->touch == NULL) {
                titleSprites->pop_front();
            } else {
                ts = titleSprites->front();
                titleSprites->pop_front();
            }
        }

        t = ts->touch;
        tp = titleTouchPoints->front();
        titleTouchPoints->pop_front();
        
        Player *p = new Player();
        p->init(tp, ts->getColor(), this);
        p->initTerritory(this->boundingBox());
        p->initScoreCounter();
        p->homePoint = ts->getPosition();
        p->touch = t;
        p->spawnNewTarget(p->startingPoint);
        p->initScoreLabel();
        
        if(GameManager::sharedManager()->tutorialActive){
            p->tutMessage = CCLabelTTF::labelWithString("Slide to", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
            p->tutColorMessage = CCLabelTTF::labelWithString("your color", ROBOTO_FONT, 40*GameManager::sharedManager()->getScaleFactor());
            p->tutMessage->setOpacity(0);
            p->tutColorMessage->setOpacity(0);
            p->tutMessage->runAction(CCSequence::actions(
                                                         CCFadeTo::actionWithDuration(.3, 255),
                                                         NULL));
            
            p->tutColorMessage->runAction(CCSequence::actions(
                                                              CCFadeTo::actionWithDuration(.3, 255),
                                                              NULL));
            this->addChild(p->tutMessage);
            this->addChild(p->tutColorMessage);
            
            showTutorialSlideToNotify(p, tp);
        }
        
        this->addChild(p);
        GameManager::sharedManager()->players->push_back(p);
    }
}

/*!
 * Shows initial tutorial label during the game.
 *
 * @param p  Player to notify.
 * @param tp Location for the tutorial label.
 */
void HelloWorld::showTutorialSlideToNotify(Player *p, CCPoint tp){
    int sep = 75;
    p->tutMessage->setString("Slide to");
    p->tutColorMessage->setString("your color");
    p->tutColorMessage->setColor(p->color);
    if(tp.x > this->boundingBox().getMidX()){
        p->tutMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40*GameManager::sharedManager()->getScaleFactor(),
                                           this->boundingBox().getMidY() - sep*GameManager::sharedManager()->getScaleFactor()));
        p->tutMessage->setRotation(-90);
        p->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40*GameManager::sharedManager()->getScaleFactor(),
                                                this->boundingBox().getMidY() + sep*GameManager::sharedManager()->getScaleFactor()));
        p->tutColorMessage->setRotation(-90);
    } else {
        p->tutMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40*GameManager::sharedManager()->getScaleFactor(),
                                           this->boundingBox().getMidY() + sep*GameManager::sharedManager()->getScaleFactor()));
        p->tutMessage->setRotation(90);
        p->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40*GameManager::sharedManager()->getScaleFactor(),
                                                this->boundingBox().getMidY() - sep*GameManager::sharedManager()->getScaleFactor()));
        p->tutColorMessage->setRotation(90);
    }
    p->tutColorMessage->setVisible(true);
    p->tutColorMessage->setOpacity(255);
}

void HelloWorld::resolveTargetCollision(){
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        printf("Resolving collision\n");
        p1->spawnNewTarget(nextTargetPosition(p1));
    }
}

/*!
 * Called when a touch is initialized. Touch actions are dependent on the game state.
 */
void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event) {
    printf("num touches: %d\n", touches->count());
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        if(GameManager::sharedManager()->gameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                
                if(CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                    if(p1->touch == NULL){
                        p1->activateTouch((CCTouch *)*it);
                        p1->updatePosition(touchLocation);
                        this->addChild(p1);
                        this->addChild(p1->shineSprite);
                        p1->spawnNewTarget(nextTargetPosition(p1));
                        
                        if(GameManager::sharedManager()->tutorialActive && p1->tut_touchHasEnded){
                            p1->tut_touchHasEnded = false;
                            p1->tutMessage->setString("Keep your finger on the screen");
                            p1->tutColorMessage->setVisible(false);
                        }
                        
                        break;
                    }
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                TitleSprite *sp = *iter;
                if(sp->touch == NULL && CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation)){
                    sp->touch = touch;
                    sp->touch->retain();
                    sp->runQueueingAnimation(titleLayer);
                    sp->queue();
                    lastPlayerQueueTime = GameManager::sharedManager()->getCurrentTimeSeconds();
                }
            }
            
            if(!GameManager::sharedManager()->firstRun()) {
                if(CCRect::CCRectContainsPoint(tutButton->boundingBox(), touchLocation)){
                    if(GameManager::sharedManager()->tutorialActive == false){
                        GameManager::sharedManager()->tutorialActive = true;
                        printf("tutorial activated\n");
                        animateTutButtonActivation();
                    } else {
                        GameManager::sharedManager()->tutorialActive = false;
                        printf("tutorial deactivated\n");
                        animateTutButtonDeactivation();
                    }
                }
            }

            if(CCRect::CCRectContainsPoint(howtoButton->boundingBox(), touchLocation)){
                if(instructionsVisible){
                    instructionsVisible = false;
                    instructions->runAction(CCFadeTo::actionWithDuration(.2, 0));
                } else {
                    instructionsVisible = true;
                    instructions->runAction(CCFadeTo::actionWithDuration(.2, 255));
                }
            }
        }
    }
}

/*!
 * Called when a touch is moved. Touch actions are dependent on the game state.
 */
void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *event) {
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        if(GameManager::sharedManager()->gameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                
                if((CCTouch *)*it == p1->touch){
                    p1->updatePosition(touchLocation);
                    
                    if(CCRect::CCRectContainsPoint(p1->currentTarget->boundingBox(), touchLocation)){
                        if(!p1->touchLock){
                            p1->spawnNewTarget(nextTargetPosition(p1));
                            p1->touchLock = true;
                            adjustTargetSize(p1);
                            p1->gainPoint();
                        }
                    } else {
                        p1->touchLock = false;
                    }
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                TitleSprite *sp = *iter;
                if(CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation) && sp->touch != touch){
                    sp->touch = NULL;
                    sp->stopQueueingAnimation();
                    sp->dequeue();
                }
            }
        }
    }
}

/*!
 * Called when a touch is ended. Touch actions are dependent on the game state.
 */
void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event){
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

        if(GameManager::sharedManager()->gameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                if((CCTouch *)*it == p1->touch){
                    this->removeChild(p1, false);
                    this->removeChild(p1->shineSprite, false);
                    p1->deactivateTouch();
                    p1->losePoint();
                    
                    if(GameManager::sharedManager()->tutorialActive && !p1->tut_touchHasEnded){
                        p1->tut_touchHasEnded = true;
                        p1->tutMessage->setString("Touch                     to continue");
                        p1->tutColorMessage->setString("your color");
                        float sep = 42;
                        if(p1->tutMessage->getPosition().x > this->boundingBox().getMidX()){
                            p1->tutMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40*GameManager::sharedManager()->getScaleFactor(),
                                                                this->boundingBox().getMidY()));
                            p1->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMaxX() - 40*GameManager::sharedManager()->getScaleFactor(),
                                                                     this->boundingBox().getMidY() - sep*GameManager::sharedManager()->getScaleFactor()));
                        } else {
                            p1->tutMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40*GameManager::sharedManager()->getScaleFactor(),
                                                                this->boundingBox().getMidY()));
                            p1->tutColorMessage->setPosition(CCPoint(this->boundingBox().getMinX() + 40*GameManager::sharedManager()->getScaleFactor(),
                                                                     this->boundingBox().getMidY() + sep*GameManager::sharedManager()->getScaleFactor()));
                        }
                        p1->tutColorMessage->setVisible(true);
                    }
                }
            }
        } else if(GameManager::sharedManager()->titleScreenIsActive()){
            for(std::list<TitleSprite *>::iterator iter = titleSprites->begin(); iter != titleSprites->end(); ++iter){
                TitleSprite *sp = *iter;
                if(sp->touch == touch && CCRect::CCRectContainsPoint(sp->boundingBox(), touchLocation)){
                    sp->touch = NULL;
                    sp->stopQueueingAnimation();
                    sp->dequeue();
                }
            }
        } else if(GameManager::sharedManager()->pregameIsActive()){
            std::list<Player *> *players = GameManager::sharedManager()->players;
            for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
                Player *p1 = *iter;
                if((CCTouch *)*it == p1->touch){
                    this->removeChild(p1, false);
                    this->removeChild(p1->shineSprite, false);
                    p1->deactivateTouch();
                    p1->losePoint();
                }
            }
        }
    }
}

/*!
 * @return Player with the highest score.
 */
Player *HelloWorld::currentWinner(){
    Player *top = NULL;
    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
        Player *p1 = *iter;
        if(top == NULL || p1->checkpointCount > top->checkpointCount){
            top = p1;
        }
    }
    return top;
}

/*!
 * Change a given player's target size based on who is currently winning.
 *
 * @param p Player whose target is changing.
 */
void HelloWorld::adjustTargetSize(Player *p){
    // called when this player's score just increased
    if(p->checkpointCount == currentWinner()->checkpointCount){
        p->shrinkTarget();
    } else {
        std::list<Player *> *players = GameManager::sharedManager()->players;
        for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter){
            Player *p1 = *iter;
            if(p1 != p){
                // grow this player
                p1->growTarget();
            }
        }
    }
}

/*!
 * Finds a new target position based on given player's current territory.
 * Adjusts territory size accordingly.
 *
 * @param p Player who is getting a new target.
 */
CCPoint HelloWorld::nextTargetPosition(Player *p){
    float x, y;
    int targetSize = p->currentTarget->boundingBox().size.width;
    CCPoint *point = new CCPoint(p->getPosition().x, p->getPosition().y);
    CCRect playerBounds = p->currentTarget->boundingBox();
    
    while((point->x == 0 && point->y == 0) || CCRect::CCRectContainsPoint(playerBounds, *point)){
        //printf("Retrying position after overlap\n");
        if (GameManager::sharedManager()->maxPlayers == 2 || p->checkpointCount < GameManager::sharedManager()->goalCheckpoints * 0.4) {
            int territoryAddition = this->boundingBox().size.width / GameManager::sharedManager()->goalCheckpoints * 0.4 / 2;
            p->territory.size.width += territoryAddition;
            
            if (p->getID() == 1) {
                p->territory.origin.x -= territoryAddition;
            }
            
            x = arc4random() % ((int)p->territory.size.width - targetSize) + p->territory.getMinX() + targetSize / 2;
            y = arc4random() % ((int)p->territory.size.height - targetSize) + targetSize / 2;
        } else {
            x = arc4random() % ((int)this->boundingBox().size.width - targetSize) + targetSize / 2;
            y = arc4random() % ((int)this->boundingBox().size.height - targetSize) + targetSize / 2;
        }
        point->setPoint(x, y);
    }
    return *point;
}

/*!
 * @return Total points score by all players during the current game.
 */
int HelloWorld::scoreTotal() {
    int score = 0;

    std::list<Player *> *players = GameManager::sharedManager()->players;
    for(std::list<Player *>::iterator iter = players->begin(); iter != players->end(); ++iter) {
        Player *p1 = *iter;
        score += p1->checkpointCount;
    }

    return score;
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}