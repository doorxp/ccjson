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
