#include "assets.h"
#include "log.h"

#include <cute.h>


using namespace Cute;

void mount_assets_folder()
{
	Path path = fs_get_base_directory();
	path.normalize();

	path += "/assets";
	fs_mount(path.c_str(), "/");

	log_info("{} mounted to \"/\"", path.c_str());
}
