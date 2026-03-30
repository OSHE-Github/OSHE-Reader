#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <expat.h>

// Path to content.opf
typedef struct {
    char opf_path[256];
} container_data_t;

// References to files in mantifest
typedef struct {
    char id[128];
    char href[256];
} manifest_item_t;

// Opf contents (metadata, manifest, spine)
typedef struct {
	char author[256];
	char title[256];
	bool in_title;
	bool in_author;
	
    manifest_item_t manifest[200];
    int manifest_count;
    char spine_ids[200][128];
    int spine_count;
	char toc_id[128];	// toc type
} opf_data_t;

// 
typedef struct {
    toc_entry_t chapters[100];
    int count;
    bool in_text;
    char last_title[128];
} ncx_data_t;

// container.xml Parser
void start_container(void *user_data, const char *name, const char **atts) {
    container_data_t *data = (*container_data_t)user_data;
    if (strcmp(name, "rootfile") == 0) {
        for (int i = 0; atts[i]; i += 2) {
            if (strcmp(atts[i], "full-path") == 0) {
                strcpy(data->opf_path, atts[i + 1]);
			}
		}
    }
}

// Looks for path to container
char *find_opf_path(const char *xml, size_t len) {
    XML_Parser parser = XML_ParserCreate(NULL);
    container_data_t data = {0};

    XML_SetUserData(parser, &data);
    XML_SetElementHandler(parser, start_container, NULL);

    if (XML_Parse(parser, xml, len, 1) == XML_STATUS_ERROR) {
        fprintf(stderr, "XML Parse error (container.xml): %s\n",
                XML_ErrorString(XML_GetErrorCode(parser)));
	}

    XML_ParserFree(parser);
    return strdup(data.opf_path);
}

// content.opf Parser
// Scans for metadata, manifest, and spine
void start_opf(void *user_data, const char *name, const char **atts) {
    opf_data_t *opf = (*opf_data_t)user_data;

    if (strcmp(name, "dc:title") == 0) {
        opf->in_title = true;
        opf->title[0] = '\0';
    } 
    else if (strcmp(name, "dc:creator") == 0) {
        opf->in_author = true;
        opf->author[0] = '\0';
    }
    else if (strcmp(name, "item") == 0) {
        manifest_item_t item = {0};
        for (int i = 0; atts[i]; i += 2) {
            if (strcmp(atts[i], "id") == 0) {
                strcpy(item.id, atts[i + 1]);
			}
            else if (strcmp(atts[i], "href") == 0) {
                strcpy(item.href, atts[i + 1]);
			}
        }
        opf->manifest[opf->manifest_count++] = item;
    }
	else if (strcmp(name, "spine") == 0) {
        for (int i = 0; atts[i]; i += 2) {
            if (strcmp(atts[i], "toc") == 0) {
				strcpy(opf->toc_id, atts[i+1]);
			}
    }
    else if (strcmp(name, "itemref") == 0) {
        for (int i = 0; atts[i]; i += 2) {
            if (strcmp(atts[i], "idref") == 0) {
                strcpy(opf->spine_ids[opf->spine_count++], atts[i + 1]);
			}
		}
    }
}

// Pulls title or author
void char_data_opf(void *user_data, const char *s, int len) {
	opf_data_t *opf = (opf_data_t *)user_data;
    
    if (opf->in_title) {
        strncat(opf->title, s, len); 
    } 
    else if (opf->in_author) {
        strncat(opf->author, s, len);
    }
}

void end_opf(void *user_data, const char *name) {
    opf_data_t *opf = (opf_data_t *)user_data;
	
    if (strcmp(name, "dc:title") == 0) {
		opf->in_title = false;
	}
    if (strcmp(name, "dc:creator") == 0) {
		opf->in_author = false;
	}
}

// Parse content.opf
void parse_opf(const char *xml, size_t len, opf_data_t *opf) {
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, opf);
	
    XML_SetElementHandler(parser, start_opf, end_opf);
	XML_SetCharacterDataHandler(parser, char_data_opf);

    if (XML_Parse(parser, xml, len, 1) == XML_STATUS_ERROR) {
        fprintf(stderr, "XML Parse error (content.opf): %s\n",
                XML_ErrorString(XML_GetErrorCode(parser)));
	}

    XML_ParserFree(parser);
}

// TOC parser
// Marks for chapter name, or gets the id
void start_ncx(void *user_data, const char *name, const char **atts) {
    ncx_data_t *data = (ncx_data_t *)user_data;
	
    if (strcmp(name, "text") == 0) {
        data->in_text = true;
    }
	else if (strcmp(name, "content") == 0) {
        if (data->count < 150) {
            for (int i = 0; atts[i]; i += 2) {
                if (strcmp(atts[i], "src") == 0) {
                    char *src = strdup(atts[i+1]);
                    char *hash = strchr(src, '#');
                    
                    if (hash) {
                        *hash = '\0';
                        strcpy(data->chapters[data->count].anchor, hash + 1);
                    }
                    strcpy(data->chapters[data->count].href, src);
                    strcpy(data->chapters[data->count].title, data->last_title);
                    free(src);
                    data->count++;
                }
            }
        }
    }
}

// Gets chapter name
void char_data_ncx(void *user_data, const char *s, int len) {
    ncx_data_t *data = (ncx_data_t *)user_data;
    if (data->in_text) {
        snprintf(data->last_title, sizeof(data->last_title), "%.*s", len, s);
    }
}

// Closing with tag <text>
void end_ncx(void *user_data, const char *name) {
    ncx_data_t *data = (ncx_data_t *)user_data;
    if (strcmp(name, "text") == 0) {
		data->in_text = false;
	}
}

// Unfinished, xhtml parser

void start_xhtml(void *user_data, const char *tag, const char **atts) {
    xhtml_data_t *xhtml = (*container_data_t)user_data;
	
	if (strcmp(tag, "h1") == 0 || strcmp(tag, "h2") == 0) {
        for (int i = 0; atts[i]; i += 2) {
            if (strcmp(atts[i], "id") == 0) {
				if(strcmp(atts[i + 1], chapters[6].href) {
					        data->in_chapter = true;
				}
			}
		}
    }
	
	if (strcmp(tag, "p") == 0) {
        data->in_paragraph = true;
    }
}


void char_data_xhtml(void *user_data, const char *s, int len) {
    xhtml_data_t *xhtml = (*container_data_t)user_data;
    
    if (xhtml->in_chapter && xhtml->in_paragraph) {
	
}


void end_xhtml(void *user_data, const char *name) {
    xhtml_data_t *xhtml = (*container_data_t)user_data;
	
	if (strcmp(tag, "p") == 0) {
        xhtml->in_paragraph = false;
    }
}


void parse_xhtml(const char *xml, size_t len) {
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, start_xhtml, end_xhtml);
    XML_SetCharacterDataHandler(parser, char_data_xhtml);

    XML_Parse(parser, xml, len, 1);
    XML_ParserFree(parser);
}

// Main
int main() {
    static const char *file_name = "MobyDick.epub";
	static const char *toc_location = "toc.ncx";
	
    char *opf_path = find_opf_path(container_xml, container_size);

    opf_data_t opf = {0};
    parse_opf(opf_xml, opf_size, &opf);

    free(opf_path);
    return 0;
}
