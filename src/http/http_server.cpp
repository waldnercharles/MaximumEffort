#include "http_server.h"
#include "httplib.h" // TODO: This should really be a system header
#include "log.h"
#include <cute.h>

void start_http_server_thread()
{
	cf_thread_create(
		[](void *) {
			start_http_server();
			return 0;
		},
		"HttpServer",
		nullptr
	);
}

void start_http_server()
{
	using namespace httplib;
	httplib::Server s;
	s.Options(R"(.*)", [](const auto &req, auto &res) {
		res.set_header(
			"Access-Control-Allow-Origin",
			req.get_header_value("Origin").c_str()
		);
		res.set_header("Allow", "GET, POST, HEAD, OPTIONS");
		res.set_header(
			"Access-Control-Allow-Headers",
			"X-Requested-With, Content-Type, Accept, Origin, Authorization"
		);
		res.set_header(
			"Access-Control-Allow-Methods",
			"OPTIONS, GET, POST, HEAD"
		);
	});

	s.Get("/nodes", [](const Request &, Response &res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		log_info("GET /passives");

		res.set_content(
			cf_fs_read_entire_file_to_memory_and_nul_terminate(
				"passives/nodes.json",
				nullptr
			),
			"application/json"
		);
	});

	s.Post("/nodes", [](const Request &req, Response &res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		log_info("POST /nodes");
		auto doc = cf_make_json(req.body.c_str(), req.body.size());
		if (!doc.id)
		{
			log_error("Failed to parse JSON from request body");
			return;
		}
		cf_fs_write_string_to_file("passives/nodes.json", req.body.c_str());
	});

	s.Get("/edges", [](const Request &, Response &res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		log_info("GET /edges");

		res.set_content(
			cf_fs_read_entire_file_to_memory_and_nul_terminate(
				"passives/edges.json",
				nullptr
			),
			"application/json"
		);
	});

	s.Post("/edges", [](const Request &req, Response &res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		log_info("POST /edges");
		auto doc = cf_make_json(req.body.c_str(), req.body.size());
		if (!doc.id)
		{
			log_error("Failed to parse JSON from request body");
			return;
		}
		cf_fs_write_string_to_file("passives/edges.json", req.body.c_str());
	});

	log_info("Starting http server http://localhost:8080");
	s.listen("0.0.0.0", 8080);
}
