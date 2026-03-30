#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <expat.h>

// Stores path to content.opf
typedef struct {
    char opf_path[256];
} container_data_t;

// Stores references to files in mantifest
typedef struct {
    char id[128];
    char href[256];
} manifest_item_t;

// 
typedef struct {
    manifest_item_t manifest[200];
    int manifest_count;
    char spine_ids[200][128];
    int spine_count;
	char toc_id[128];
    char toc_href[256];	// Path to toc
} opf_data_t;

// Metadata from content.opf
typefed struct {
	char author[256];
	char title[256];
	bool in_title;
	bool in_author;
} book_metadata;

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
// Scans manifest and spine
void start_opf(void *user_data, const char *name, const char **atts) {
    opf_data_t *opf = (*opf_data_t)user_data;

    if (strcmp(name, "item") == 0) {
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

// Parse content.opf
void parse_opf(const char *xml, size_t len, opf_data_t *opf) {
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, opf);
    XML_SetElementHandler(parser, start_opf, NULL);

    if (XML_Parse(parser, xml, len, 1) == XML_STATUS_ERROR) {
        fprintf(stderr, "XML Parse error (content.opf): %s\n",
                XML_ErrorString(XML_GetErrorCode(parser)));
	}

    XML_ParserFree(parser);
}
		
// Starts search for relevant tags
void start_metadata(void *user_data, const char *name, const char **atts) {
    metadata_t *data = (metadata_t *)user_data;

    if (strcmp(name, "dc:title") == 0) {
        data->in_title = true;
        data->title[0] = '\0';
    } 
    else if (strcmp(name, "dc:creator") == 0) {
        data->in_author = true;
        data->author[0] = '\0';
    }
}

// Pulls title or author
void char_data_metadata(void *user_data, const char *s, int len) {
    metadata_t *data = (metadata_t *)user_data;
    
    if (data->in_title) {
        strncat(data->title, s, len); 
    } 
    else if (data->in_author) {
        strncat(data->author, s, len);
    }
}

// Closes with seeing title or creator
void end_metadata(void *user_data, const char *name) {
    metadata_t *data = (metadata_t *)user_data;

    if (strcmp(name, "dc:title") == 0) {
        data->in_title = false;
    } 
    else if (strcmp(name, "dc:creator") == 0) {
        data->in_author = false;
    }
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

// XHTML Parser for tags 
void start_xhtml(void *user_data, const char *name, const char **atts) {
    strcpy(current_tag, name);
    if (   )
        
}

void end_xhtml(void *user_data, const char *name) {
    current_tag[0] = '\0';
}

void char_data_xhtml(void *user_data, const char *s, int len) {
    if (stop_after_chapter) return;

    char text[512];
    snprintf(text, sizeof(text), "%.*s", len, s);

    if (strcmp(current_tag, "h1") == 0 || strcmp(current_tag, "h2") == 0) {
        // Look for chapter heading
        if (!chapter_found && chapter_heading(text)) {
            chapter_found = 1;
            in_chapter = 1;
            printf("=== %s ===\n", text);
            return;
        }
        else if (chapter_found && chapter_heading(text)) {
            // Next chapter reached
            stop_after_chapter = 1;
            return;
        }
    }

    if (in_chapter)
        printf("%.*s", len, s);
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
	
    char *opf_path = find_opf_path(container_xml, container_size);

    opf_data_t opf = {0};
    parse_opf(opf_xml, opf_size, &opf);

    free(opf_path);
    return 0;
}
