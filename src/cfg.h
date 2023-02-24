#ifndef CFG_H
#define CFG_H
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define CFG_PATH "./conf/cfg.json"

int get_cfg_value(const char* profile, char* title, char* key, char* value);

int get_mysql_info(char* mysql_user, char* mysql_pwd, char* mysql_db);

#endif
