#include "assets.h"

#include <cute.h>

#include "log.h"

using namespace Cute;

void mount_assets_folder()
{
	Path path = fs_get_base_directory();
	path.normalize();
	path.pop(2);// TODO: Remove this. We're just popping up to the src dir.

	path += "/assets";
	fs_mount(path.c_str(), "/");

	log_info("{} mounted to \"/\"", path.c_str());
}
