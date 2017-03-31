#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CCJSON.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    const char *jsonString = CCJSONSTR({'a':"b","arr":["中文","日文","English"]});
    
    CCJSONPtr json = CCJSON::parse(jsonString);
    CCJSONPtr arr = json->objectForKey("arr");
    
    CCLOG("%s",json->objectForKey("a")->as_string().c_str());
    
    CCJSON_FOREACH(arr, i) {
        CCLOG("%s", i->as_string().c_str());
    }
    else {
        CCLOG("no data");
    }
    
    
    Data data = FileUtils::getInstance()->getDataFromFile("test.json");
    
    json = CCJSON::parse((const char *)data.getBytes());
    
    CCJSONPtr glossary = json->objectForKey("glossary");
    
    CCJSONPtr title = glossary->objectForKey("title");
    
    CCLOG("%s", title->as_string().c_str());
    
    
    CCJSONPtr glossDiv = glossary->objectForKey("GlossDiv");
    
    CCJSONPtr abbrev = glossDiv->objectForKeys("GlossList","GlossEntry","Abbrev",NULL);
    
    CCLOG("%s", abbrev->as_string().c_str());
    
    
    CCJSONPtr glossSeeAlso = glossDiv->objectForPath("GlossList/GlossEntry/GlossDef/GlossSeeAlso");
    
    CCJSON_FOREACH(glossSeeAlso, i) {
        CCLOG("%s", i->as_string().c_str());
    }
    else {
        CCLOG("no data");
    }
    
    
    CCJSONPtr XML = glossDiv->objectForPath("GlossList/GlossEntry/GlossDef/GlossSeeAlso/@0");
    
    CCLOG("%s", XML->as_string().c_str());
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
