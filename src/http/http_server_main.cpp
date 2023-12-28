#include "assets.h"
#include "http_server.h"

int main(int, char *[])
{
	cf_make_app(
		nullptr,
		0,
		0,
		0,
		0,
		CF_APP_OPTIONS_HIDDEN | CF_APP_OPTIONS_NO_GFX | CF_APP_OPTIONS_NO_AUDIO,
		nullptr
	);
	mount_dir("../../../assets");
	Path path = cf_fs_get_base_directory();
	path.normalize();
	path += "../../../assets";
	cf_fs_set_write_directory(path.c_str());
	start_http_server();
	return 0;
}