xmlDocPTR doc;
xmlNodePtr cur;

doc = xmlParseFile(container.xml);
   if (doc == NULL ) {
        fprintf(stderr,"Document not parsed successfully.\n");
        return;
    }

cur = xmlDocGetRootElement(doc);

if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return;
    }
	
// Make sure this is the right file
if (xmlStrcmp(cur->name, (const xmlChar *) "container")) {
        fprintf(stderr,"document of the wrong type, root node != container");
        xmlFreeDoc(doc);
        return;
    }
	
// Move to the child node
cur = cur->xmlChildrenNode;
while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) "rootfiles"))) {
        
        // Move deeper to the 'rootfile' element
        xmlNodePtr rootfile = cur->xmlChildrenNode;
        while (rootfile != NULL) {
            if ((!xmlStrcmp(rootfile->name, (const xmlChar *) "rootfile"))) {
                
                // Extract the 'full-path' attribute
                full_path = xmlGetProp(rootfile, (const xmlChar *) "full-path");
                if (full_path != NULL) {
                    printf("Found path: %s\n", full_path);
                    // Use the path
                    xmlFree(full_path);
                    break; 
                }
            }
            rootfile = rootfile->next;
			        }
    }
    cur = cur->next;
}
xmlFreeDoc(doc);