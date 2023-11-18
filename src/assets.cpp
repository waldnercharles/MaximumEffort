#include "assets.h"
#include "common.h"
#include "log.h"

#include <cute.h>

void mount_assets_folder()
{
	Path path = cf_fs_get_base_directory();
	path.normalize();

	path += "/assets";
	cf_fs_mount(path.c_str(), "/", true);

	log_info("{} mounted to \"/\"", path.c_str());
}
