#pragma once
#include "common.h"

void mount_assets_folder();

Array<Array<String>> parse_csv_str(const char *str);
Array<Array<String>> parse_csv_file(const char *path);
