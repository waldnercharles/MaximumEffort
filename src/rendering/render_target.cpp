#include "rendering/render_target.h"

RenderTarget make_render_target(int w, int h)
{
	RenderTarget render_target;
	render_target.w = w;
	render_target.h = h;

	// Backbuffer
	{
		CF_TextureParams params = cf_texture_defaults();
		params.width = w;
		params.height = h;
		params.render_target = true;
		render_target.backbuffer = cf_make_texture(params);
	}

	// Depth-Stencil
	{
		CF_TextureParams params = cf_texture_defaults();
		params.width = w;
		params.height = h;
		params.render_target = true;
		params.pixel_format = CF_PIXELFORMAT_DEPTH_STENCIL;
		render_target.backbuffer_depth_stencil = cf_make_texture(params);
	}

	// Canvas
	{
		CF_CanvasParams params = cf_canvas_defaults();
		params.target = render_target.backbuffer;
		params.depth_stencil_target = render_target.backbuffer_depth_stencil;
		render_target.canvas = cf_make_canvas(params);
	}

	return render_target;
}

void destroy_render_target(RenderTarget render_target)
{
	if (render_target.backbuffer.id)
	{
		cf_destroy_texture(render_target.backbuffer);
	}

	if (render_target.backbuffer_depth_stencil.id)
	{
		cf_destroy_texture(render_target.backbuffer_depth_stencil);
	}

	if (render_target.canvas.id)
	{
		cf_destroy_canvas(render_target.canvas);
	}
}
