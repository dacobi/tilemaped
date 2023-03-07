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

sKey::sKey(std::string _kname, TKeyType _ktype, std::string _value){
	kname = _kname;
	ktype = _ktype;
	svalue = _value;
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
bool sKey::getBool(){return (ivalue > 0);}

sKey* Settings::createNewKey(std::string nKeyName, TKeyType nKeyType, int nKeyInteger){
	if(nKeyType == type_int || nKeyType == type_bool){
		sKey *newKey = new sKey(nKeyName, nKeyType, nKeyInteger);
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

    Editor_SelectionAppend = new sKey("Editor_SelectionAppend", type_bool, true);
	Tile_ShowPixelGrid = new sKey("Tile_ShowPixelGrid", type_bool, true);
	TileSet_ShowPixelGrid = new sKey("TileSet_ShowPixelGrid", type_bool, true);
	TileSet_ShowTileGrid = new sKey("TileSet_ShowTileGrid", type_bool, true);
	TileSet_UpdateMaps = new sKey("TileSet_UpdateMaps", type_bool, true);
	TileSet_WarnBeforeDelete = new sKey("TileSet_WarnBeforeDelete", type_bool, true);
	TileSet_EditWidth = new sKey("TileSet_EditWidth", type_int, 4);
	SelectionEdit_ShowPixelGrid = new sKey("SelectionEdit_ShowPixelGrid", type_bool, true);
	SelectionEdit_ShowTileGrid = new sKey("SelectionEdit_ShowTileGrid", type_bool, true);
	

	keys.push_back(error_catch);
    keys.push_back(Editor_SelectionAppend);
	keys.push_back(Tile_ShowPixelGrid);
	keys.push_back(TileSet_ShowPixelGrid);
	keys.push_back(TileSet_ShowTileGrid);
    keys.push_back(TileSet_UpdateMaps);
	keys.push_back(TileSet_WarnBeforeDelete);
    keys.push_back(TileSet_EditWidth);
	keys.push_back(SelectionEdit_ShowPixelGrid);
	keys.push_back(SelectionEdit_ShowTileGrid);

    for(int i = 0; i < keys.size(); i++){
		keyindex[keys[i]->kname] = i;
	}
		
}

void Settings::close(){
	Editor_SelectionAppend->ivalue = true;
	Tile_ShowPixelGrid->ivalue = true;
	TileSet_ShowPixelGrid->ivalue = true;
	TileSet_ShowTileGrid->ivalue = true;
	TileSet_UpdateMaps->ivalue = true;
	TileSet_WarnBeforeDelete->ivalue = true;
	TileSet_EditWidth->ivalue = 4;
	SelectionEdit_ShowPixelGrid->ivalue = true;
	SelectionEdit_ShowTileGrid->ivalue = true;
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
		std::cout << "Settings are Valid" << std::endl;
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
			tmpStr = (const char *)xmlGetProp(myNode, (const xmlChar *)"keyvalue");
			
			double value;
			keys[key]->ivalue = 0;
			keys[key]->fvalue = 0;
			
			if(type != type_string){
				convert << tmpStr << std::endl;
				convert >> value;         
				keys[key]->ivalue = (int)value;
				keys[key]->fvalue = value;
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
		}else{
			if(keys[i]->ktype == type_int){
				convert << keys[i]->ivalue << std::endl;
			}
			if(keys[i]->ktype == type_float){
				convert << keys[i]->fvalue << std::endl;
			}
			if(keys[i]->ktype == type_bool){
				convert << keys[i]->ivalue << std::endl;
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