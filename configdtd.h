const char *configdtd = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<!--    DTD for config.   -->\n\n<!ELEMENT settings (keys+)>\n<!ELEMENT keys (setkeyi|setkeyb|setkeyf|setkeys)* >\n<!ELEMENT setkeyi EMPTY>\n<!ELEMENT setkeyb EMPTY>\n<!ELEMENT setkeyf EMPTY>\n<!ELEMENT setkeys EMPTY>\n<!ATTLIST setkeyi keyname CDATA #REQUIRED>\n<!ATTLIST setkeyb keyname CDATA #REQUIRED>\n<!ATTLIST setkeyf keyname CDATA #REQUIRED>\n<!ATTLIST setkeys keyname CDATA #REQUIRED>\n<!ATTLIST setkeyi keyvalue CDATA #REQUIRED>\n<!ATTLIST setkeyb keyvalue CDATA #REQUIRED>\n<!ATTLIST setkeyf keyvalue CDATA #REQUIRED>\n<!ATTLIST setkeys keyvalue CDATA #REQUIRED>\n";