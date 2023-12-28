#pragma once
#include "common.h"

void mount_dir(const char *dir);
void mount_assets_dir();

Array<Array<String>> parse_csv_str(const char *str);
Array<Array<String>> parse_csv_file(const char *path);
