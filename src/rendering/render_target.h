#pragma once
#include <cute.h>

struct RenderTarget
{
	int w, h;
	CF_Texture backbuffer;
	CF_Texture backbuffer_depth_stencil;
	CF_Canvas canvas;
};

struct Vertex
{
	float x, y;
	float u, v;
};

RenderTarget
make_render_target(int w, int h, CF_Filter filter = CF_FILTER_NEAREST);
void destroy_render_target(RenderTarget render_target);
