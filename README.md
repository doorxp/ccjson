# ccjson
json for cocos2dx, support ios, macdesktop, android etc.
C ++ cocos2dx native implementation

```
const char *jsonString = CCJSONSTR({'a':"b","arr":["中文","日文","English"]});

CCJSONPtr json = CCJSON::parse(jsonString);
CCJSONPtr arr = json->objectForKey("arr");

CCLOG("%s",json->objectForKey("a")->as_string().c_str());

//list Child node
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

//get Multi-level child nodes
CCJSONPtr abbrev = glossDiv->objectForKeys("GlossList","GlossEntry","Abbrev",NULL);

CCLOG("%s", abbrev->as_string().c_str());

//get Multi-level child nodes with path
CCJSONPtr glossSeeAlso = glossDiv->objectForPath("GlossList/GlossEntry/GlossDef/GlossSeeAlso");

CCJSON_FOREACH(glossSeeAlso, i) {
  CCLOG("%s", i->as_string().c_str());
}
else {
  CCLOG("no data");
}

//get Multi-level child nodes with path, Array
CCJSONPtr XML = glossDiv->objectForPath("GlossList/GlossEntry/GlossDef/GlossSeeAlso/@0");

CCLOG("%s", XML->as_string().c_str());

//create json

CCJSONPtr node = CCJSON::node();
node->push("a", "a");

CCJSONPtr ar = CCJSON::node();
ar->push("AAAA");
ar->push("BBBB");
ar->push(1);
ar->push(1.5);
ar->push(true);

node->push(ar);

CCLOG("%s",node->str().c_str());

```

