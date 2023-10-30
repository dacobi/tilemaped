#include "TSettings.h"
#include "configdtd.h"

sKey::sKey(std::string _kname, TKeyType _ktype, double _value){
	kname = _kname;
	ktype = _ktype;
	ivalue = (int)_value;
	fvalue = _value;
	bEnabled = true;	
}

sKey::sKey(std::string _kname, TKeyType _ktype, int _value){
	kname = _kname;
	ktype = _ktype;
	ivalue = _value;
	fvalue = _value;
	bEnabled = true;	
}

sKey::sKey(std::string _kname, TKeyType _ktype, bool _value){
	kname = _kname;
	ktype = _ktype;
	ivalue = _value ? 1 : 0;
	fvalue = ivalue;
	bvalue = _value;
	bEnabled = true;	
}


sKey::sKey(std::string _kname, TKeyType _ktype, std::string _value){
	kname = _kname;
	ktype = _ktype;
	svalue = _value;
	//std::cout << "Setting Key Value: " << svalue << std::endl;
}

void sKey::addOption(std::string text, int value){
	optionvalue.push_back(value);
	optiontext.push_back(text);
}


void sKey::sKeyEnable(){bEnabled = true;}
void sKey::sKeyDisable(){bEnabled = false;}

std::string sKey::getString(){return svalue;}
int sKey::getInteger(){return ivalue;}
double sKey::getFloat(){return fvalue;}
bool sKey::getBool(){return bvalue;}

sKey* Settings::createNewKey(std::string nKeyName, TKeyType nKeyType, int nKeyInteger){
	if(nKeyType == type_int){
		sKey *newKey = new sKey(nKeyName, nKeyType, nKeyInteger);
		keys.push_back(newKey);
		return newKey;
	}	
	return NULL;
}

sKey* Settings::createNewKey(std::string nKeyName, TKeyType nKeyType, bool nKeyBool){
	if(nKeyType == type_bool){
		sKey *newKey = new sKey(nKeyName, nKeyType, nKeyBool);
		keys.push_back(newKey);
		return newKey;
	}	
	return NULL;
}


sKey* Settings::createNewKey(std::string nKeyName, TKeyType nKeyType, double nKeyFloat){
	if(nKeyType == type_float){
		sKey* newKey = new sKey(nKeyName, nKeyType, nKeyFloat);
		keys.push_back(newKey);
		return newKey;
	}	
	return NULL;
}

sKey* Settings::createNewKey(std::string nKeyName, TKeyType nKeyType, std::string nKeyString){
	if(nKeyType == type_string){
		sKey*  newKey = new sKey(nKeyName, nKeyType, nKeyString);
		keys.push_back(newKey);
		return newKey;
	}	
	return NULL;
}

sKey* Settings::getKey(std::string gKeyName){

	for(int i = 1; i < keys.size(); i++){
		if(keys[i]->kname.find(gKeyName) != std::string::npos){
			return keys[i];
		}
	}
	return keys[0];
}

Settings::Settings(){	
	bLoaded = false;
	
	error_catch = new sKey("error_catch", type_int, 0);

	keys.push_back(error_catch);

    for(int i = 0; i < keys.size(); i++){
		keyindex[keys[i]->kname] = i;
	}
		
}


ProjectSettings::ProjectSettings(){	
    Editor_SelectionAppend = new sKey("Editor_SelectionAppend", type_bool, true);
	Editor_UseTextureFiltering = new sKey("Editor_UseTextureFiltering", type_int, 0);
	Editor_PixelScale = new sKey("Editor_PixelScale", type_int, 16);
	Editor_OverlayTextColor = new sKey("Editor_OverlayTextColor", type_int, 1);
	Editor_OverlayTextSize = new sKey("Editor_OverlayTextSize", type_int, 14);
	Editor_OverlayToggleColVals = new sKey("Editor_OverlayToggleColVals", type_bool, true);
	Editor_TileSetMaximized = new sKey("Editor_TileSetMaximized", type_bool, false);
	Tile_ShowPixelGrid = new sKey("Tile_ShowPixelGrid", type_bool, true);
	TileSet_ShowPixelGrid = new sKey("TileSet_ShowPixelGrid", type_bool, true);
	TileSet_ShowTileGrid = new sKey("TileSet_ShowTileGrid", type_bool, true);
	TileSet_UpdateMaps = new sKey("TileSet_UpdateMaps", type_bool, true);
	TileSet_WarnBeforeDelete = new sKey("TileSet_WarnBeforeDelete", type_bool, true);
	TileSet_EditWidth = new sKey("TileSet_EditWidth", type_int, 4);
	TileSet_PixelScale = new sKey("TileSet_PixelScale", type_int, 10);	
	TileSet_MaxGridWidth = new sKey("TileSet_MaxGridWidth", type_int, 4);	
	TileSet_MinTileScale = new sKey("TileSet_MinTileScale", type_int, 7);
	TileSet_TileScale = new sKey("TileSet_TileScale", type_int, 4);
	TileSet_ShowOverlay = new sKey("TileSet_ShowOverlay", type_bool, false);
	ColMap_ShowOverlay = new sKey("ColMap_ShowOverlay", type_bool, false);
	SelectionEdit_ShowPixelGrid = new sKey("SelectionEdit_ShowPixelGrid", type_bool, true);
	SelectionEdit_ShowTileGrid = new sKey("SelectionEdit_ShowTileGrid", type_bool, true);
	SelectionEdit_PixelScale = new sKey("SelectionEdit_PixelScale", type_int, 10);	
	Sprite_ShowPixelGrid = new sKey("Sprite_ShowPixelGrid", type_bool, true);
	Sprite_WarnBeforeDelete = new sKey("Sprite_WarnBeforeDelete", type_bool, true);
	Clipboard_SaveToProject = new sKey("Clipboard_SaveToProject", type_bool, false);
			
    init();		
}

ProgramSettings::ProgramSettings(){	
	Sys_Renderer = new sKey("Sys_Renderer", type_int, 0);
	Sys_VSYNC = new sKey("Sys_VSYNC", type_bool, true);
	Sys_FontPath = new sKey("Sys_FontPath", type_string, std::string("NONE"));
	Win_WarnBeforeQuit = new sKey("Win_WarnBeforeQuit", type_bool, true);
	Win_Restore = new sKey("Win_Restore", type_bool, true);
	Win_Width = new sKey("Win_Width", type_int, 1900);
	Win_Height = new sKey("Win_Height", type_int, 1000);
	Win_Maximize = new sKey("Win_Maximize", type_bool, false);
	Win_HighDPI = new sKey("Win_HighDPI", type_bool, false);
	Win_UIScale = new sKey("Win_UIScale", type_int, 100);
	Win_Theme = new sKey("Win_Theme", type_int, 0);
	Theme_SelectionDark = new sKey("Theme_SelectionDark", type_int, (int)0xFFFFFF00);
	Theme_SelectionLight = new sKey("Theme_SelectionLight", type_int, (int)0xFF00FFFF);
	Theme_HighlightDark = new sKey("Theme_HighlightDark", type_int, (int)0xFFFFFFFF);
	Theme_HighlightLight = new sKey("Theme_HighlightLight", type_int, (int)0xFF000000);
	Theme_PixelGridDark = new sKey("Theme_PixelGridDark", type_int, (int)0xFFA0A0A0);
	Theme_PixelGridLight = new sKey("Theme_PixelGridLight", type_int, (int)0xFF202020);

	keys.push_back(Sys_Renderer);
	keys.push_back(Sys_VSYNC);
	keys.push_back(Sys_FontPath);
	keys.push_back(Win_WarnBeforeQuit);
	keys.push_back(Win_Restore);
	keys.push_back(Win_Width);
	keys.push_back(Win_Height);	
	keys.push_back(Win_Maximize);	
	keys.push_back(Win_HighDPI);
	keys.push_back(Win_UIScale);
	keys.push_back(Win_Theme);
	keys.push_back(Theme_SelectionDark);
	keys.push_back(Theme_SelectionLight);
	keys.push_back(Theme_HighlightDark);
	keys.push_back(Theme_HighlightLight);
	keys.push_back(Theme_PixelGridDark);
	keys.push_back(Theme_PixelGridLight);

	for(int i = 0; i < keys.size(); i++){
		keyindex[keys[i]->kname] = i;
	}
}

void ProjectSettings::init(){	
	keys.push_back(Editor_SelectionAppend);
	keys.push_back(Editor_UseTextureFiltering);
	keys.push_back(Editor_PixelScale);
	keys.push_back(Editor_OverlayTextColor);
	keys.push_back(Editor_OverlayTextSize);
	keys.push_back(Editor_OverlayToggleColVals);
	keys.push_back(Editor_TileSetMaximized);
	keys.push_back(Tile_ShowPixelGrid);
	keys.push_back(TileSet_ShowPixelGrid);
	keys.push_back(TileSet_ShowTileGrid);
    keys.push_back(TileSet_UpdateMaps);
	keys.push_back(TileSet_WarnBeforeDelete);
    keys.push_back(TileSet_EditWidth);
	keys.push_back(TileSet_PixelScale);
	keys.push_back(TileSet_MaxGridWidth);
	keys.push_back(TileSet_MinTileScale);
	keys.push_back(TileSet_TileScale);
	keys.push_back(TileSet_ShowOverlay);
	keys.push_back(ColMap_ShowOverlay);
	keys.push_back(SelectionEdit_ShowPixelGrid);
	keys.push_back(SelectionEdit_ShowTileGrid);
	keys.push_back(SelectionEdit_PixelScale);
	keys.push_back(Sprite_ShowPixelGrid);
	keys.push_back(Sprite_WarnBeforeDelete);
	keys.push_back(Clipboard_SaveToProject);
		
    for(int i = 0; i < keys.size(); i++){
		keyindex[keys[i]->kname] = i;
	}
}

void ProjectSettings::close(){

	keys.clear();

	keys.push_back(error_catch);

	init();

	Editor_SelectionAppend->bvalue = true;
	Tile_ShowPixelGrid->bvalue = true;
	TileSet_ShowPixelGrid->bvalue = true;
	TileSet_ShowTileGrid->bvalue = true;
	TileSet_UpdateMaps->bvalue = true;
	TileSet_WarnBeforeDelete->bvalue = true;
	TileSet_EditWidth->ivalue = 4;
	TileSet_PixelScale->ivalue = 10;
	TileSet_MaxGridWidth->ivalue = 4;
	TileSet_MinTileScale->ivalue = 7;
	TileSet_TileScale->ivalue = 4;
	TileSet_ShowOverlay->bvalue = false;
	ColMap_ShowOverlay->bvalue = false;
	SelectionEdit_ShowPixelGrid->bvalue = true;
	SelectionEdit_ShowTileGrid->bvalue = true;
	SelectionEdit_PixelScale->ivalue = 10;
	Sprite_ShowPixelGrid->bvalue = true;
	Sprite_WarnBeforeDelete->bvalue = true;
	Editor_UseTextureFiltering->ivalue = 0;
	Editor_PixelScale->ivalue = 16;
	Editor_OverlayTextColor->ivalue = 1;
	Editor_OverlayTextSize->ivalue = 14;
	Editor_OverlayToggleColVals->bvalue = true;
	Editor_TileSetMaximized->bvalue = false;
	Clipboard_SaveToProject->bvalue = false;
	
	bLoaded = false;
}

int ProjectSettings::removeKey(std::string dKey){

	sKey *cSpriteKey = getKey(dKey);

	if(cSpriteKey == keys[0]){
		return 1;
	}

	for(int ki = 0;  ki < keys.size(); ki++){
		if(cSpriteKey == keys[ki]){
			keys.erase(keys.begin()+ki);
			
			for(int i = 0; i < keys.size(); i++){
				keyindex[keys[i]->kname] = i;
			}
			return 0;
		}
	}

	return 1;	
}

sKey* ProjectSettings::getMapTileZeroKey(int cMNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cMNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = getKey(std::string("TileMap"+sSnum+"_TileZeroTransparent"));

	if(cSpriteKey == keys[0]){		
		return NULL;
	}

	return cSpriteKey;	
}

sKey* ProjectSettings::createMapTileZeroKey(int cMNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cMNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = createNewKey(std::string("TileMap"+sSnum+"_TileZeroTransparent"), type_bool, false);
	return cSpriteKey;
}

int ProjectSettings::removeMapTileZeroKey(int cMNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cMNum << std::endl;
	sconv >> sSnum;
	return removeKey(std::string("TileMap"+sSnum+"_TileZeroTransparent"));
}

sKey* ProjectSettings::getFrameScaleKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = getKey(std::string("Sprite"+sSnum+"_FrameMinScale"));

	if(cSpriteKey == keys[0]){		
		return NULL;
	}

	return cSpriteKey;	
}

sKey* ProjectSettings::createFrameScaleKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = createNewKey(std::string("Sprite"+sSnum+"_FrameMinScale"), type_int, 5);
	return cSpriteKey;
}

int ProjectSettings::removeFrameScaleKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	return removeKey(std::string("Sprite"+sSnum+"_FrameMinScale"));	
}

sKey* ProjectSettings::getSpriteScaleKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = getKey(std::string("Sprite"+sSnum+"_PixelScale"));

	if(cSpriteKey == keys[0]){		
		return NULL;
	}

	return cSpriteKey;	
}

sKey* ProjectSettings::createSpriteScaleKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = createNewKey(std::string("Sprite"+sSnum+"_PixelScale"), type_int, 16);
	return cSpriteKey;
}

int ProjectSettings::removeSpriteScaleKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	return removeKey(std::string("Sprite"+sSnum+"_PixelScale"));	
}


sKey* ProjectSettings::getSpriteGridKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = getKey(std::string("Sprite"+sSnum+"_MaxGridWidth"));

	if(cSpriteKey == keys[0]){		
		return NULL;
	}

	return cSpriteKey;	
}

sKey* ProjectSettings::createSpriteGridKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	sKey *cSpriteKey = createNewKey(std::string("Sprite"+sSnum+"_MaxGridWidth"), type_int, 4);
	return cSpriteKey;
}

int ProjectSettings::removeSpriteGridKey(int cSNum){

	std::stringstream sconv;
	std::string sSnum;
	sconv << cSNum << std::endl;
	sconv >> sSnum;
	return removeKey(std::string("Sprite"+sSnum+"_MaxGridWidth"));
}


int Settings::load(std::string filename){
	if(bLoaded) return 0;

	std::stringstream convert;
	std::string tmpStr;
	int errorCount = 0;
	
	xmlDocPtr myDoc;
	xmlNodePtr myNode, tmpNode;
	xmlParserCtxtPtr ctxt;
	xmlValidCtxtPtr mctxt;
	xmlKeepBlanksDefault(0);
	
	ctxt = xmlNewParserCtxt();
	
	myDoc = xmlCtxtReadFile(ctxt, filename.c_str(), NULL, 0); //XML_PARSE_DTDVALID

	mctxt = xmlNewValidCtxt();
		
	xmlParserInputBufferPtr buf = xmlParserInputBufferCreateMem(configdtd, strlen(configdtd), XML_CHAR_ENCODING_NONE);
	xmlDtdPtr myDtd = xmlIOParseDTD(NULL, buf, XML_CHAR_ENCODING_NONE);
	//xmlFreeParserInputBuffer(buf);

	if(xmlValidateDtd(mctxt, myDoc, myDtd)){
		//std::cout << "Settings are Valid" << std::endl;
	} else {
		std::cout << "Settings are INVALID" << std::endl;
		xmlFreeDoc(myDoc);
		return 1;
	}

	if (myDoc == NULL){
        return 1;
	}
	
	if(ctxt->valid == 0){
		xmlFreeDoc(myDoc);
		return 1;
	}
	
	myNode = xmlDocGetRootElement(myDoc);
	if (myNode == NULL) {
		xmlFreeDoc(myDoc);
		return 1;
	}
	
	tmpStr = "settings";
	if(tmpStr != (const char *)myNode->name) {
		xmlFreeDoc(myDoc);
		return 1;
	}
	
	myNode = myNode->xmlChildrenNode;
	myNode = myNode->xmlChildrenNode;
	while(myNode && xmlIsBlankNode(myNode)){
		myNode = myNode->next;
	}
	
	if(myNode == 0){
		xmlFreeDoc(myDoc);
		return 1;
	}
	
	while(myNode){
		tmpStr = (const char *)myNode->name;
		int type = error;
		if(tmpStr == "setkeyi") type = type_int;
		if(tmpStr == "setkeyb") type = type_bool;
		if(tmpStr == "setkeyf") type = type_float;
		if(tmpStr == "setkeys") type = type_string;		
		
		if(type != error){
			tmpStr = (const char *)xmlGetProp(myNode, (const xmlChar *)"keyname");
			int key = keyindex[tmpStr];
			std::string cDynSKey = tmpStr;

			tmpStr = (const char *)xmlGetProp(myNode, (const xmlChar *)"keyvalue");

			if(key == 0){
				switch (type)
				{
				case type_int:
					createNewKey(cDynSKey, type_int, 0);
					break;
				case type_float:
					createNewKey(cDynSKey, type_float, 0.0);
					break;
				case type_bool:
					createNewKey(cDynSKey, type_bool, false);
					break;
				case type_string:
					createNewKey(cDynSKey, type_string, "");
					break;				
				default:
					break;
				}
			
				key = keys.size()-1;
				keyindex[keys[key]->kname] = key;
			}
			
			double value;
			keys[key]->ivalue = 0;
			keys[key]->fvalue = 0;
			
			if(type != type_string){
				convert << tmpStr << std::endl;
				convert >> value;         
				keys[key]->ivalue = (int)value;
				keys[key]->fvalue = value;
				keys[key]->bvalue = keys[key]->ivalue ? true : false;
			} else {
				//std::cout << "Setting Key Value: " << tmpStr << std::endl;
			}
			keys[key]->ktype = (TKeyType)type;
			keys[key]->svalue = tmpStr;
			
						
		}
		
		myNode = myNode->next;
	}
	
	xmlFreeDoc(myDoc);
		
	bLoaded = true;
	return 0;
}


int Settings::writedefault(std::string filename){
	std::stringstream convert;
	std::string tmpStr;
	
	xmlDocPtr myDoc;
	xmlNodePtr myNode = NULL;
    xmlNodePtr keynode = NULL;
	
	xmlKeepBlanksDefault(0);
	
	myDoc = xmlNewDoc((const xmlChar *)"1.0");
	myDoc->children = xmlNewDocNode(myDoc, NULL, (const xmlChar *)"settings", NULL);
	
	myNode = xmlNewChild(myDoc->children, NULL, (const xmlChar *)"keys", NULL);  
	
	for(int i = 1; i < keys.size(); i++){	
		if(keys[i]->ktype == type_int) keynode = xmlNewNode(NULL,(const xmlChar *)"setkeyi");
		if(keys[i]->ktype == type_bool) keynode = xmlNewNode(NULL,(const xmlChar *)"setkeyb");
		if(keys[i]->ktype == type_float) keynode = xmlNewNode(NULL,(const xmlChar *)"setkeyf");
		if(keys[i]->ktype == type_string) keynode = xmlNewNode(NULL,(const xmlChar *)"setkeys");
		
		xmlSetProp(keynode, (const xmlChar *)"keyname",  (const xmlChar *)keys[i]->kname.c_str());
		
		if(keys[i]->ktype == type_string){
			tmpStr = keys[i]->svalue;
			//std::cout << "Writing Key Value: " << tmpStr << std::endl;
		}else{
			if(keys[i]->ktype == type_int){
				convert << keys[i]->ivalue << std::endl;
			}
			if(keys[i]->ktype == type_float){
				convert << keys[i]->fvalue << std::endl;
			}
			if(keys[i]->ktype == type_bool){
				int tmpintb = keys[i]->bvalue ? 1 : 0;
				convert << tmpintb << std::endl;
			}

			convert >> tmpStr;
		}
		
		xmlSetProp(keynode, (const xmlChar *)"keyvalue",  (const xmlChar *)tmpStr.c_str());
		
		xmlAddChild(myNode, keynode);  
	}
	
	
	xmlDtdPtr myDtd;
	myDtd = xmlCreateIntSubset(myDoc, (const xmlChar *)"settings", (const xmlChar *)"-//EMPRETRO//DTD XML 1.0//Config//EN", (const xmlChar *)"http://kanhave.xyz//config.dtd"); 	
	//myDtd = xmlCreateIntSubset(myDoc, (const xmlChar *)"settings", (const xmlChar *)"-//EMPRETRO//DTD XML 1.0//Config//EN", (const xmlChar *) std::string(fs::current_path().string() + "config.dtd").c_str()); 	
	
	if(xmlSaveFormatFile(filename.c_str(), myDoc,1) == -1){
		xmlFreeDoc(myDoc);
		return 1;
	}
	else{
		xmlFreeDoc(myDoc);
		return 0;
	}
}