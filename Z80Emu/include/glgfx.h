#pragma once

#include "types.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_memory_editor.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "glad/glad.h"

#include <SDL_opengles2.h>

#include "mem.h"

class GLGfx
{
public:
	GLGfx() {};
	~GLGfx() {};

struct texture_t
{
	int w;
	int h;
	SDL_Texture* texture;
};

	bool init();
	void update();

private:
	SDL_GLContext gl_context;
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;
	SDL_Texture* font = nullptr;
	texture_t display = {};
	texture_t tile = {};
	texture_t chars = {};
	SDL_Joystick* joy = nullptr;
	u32 tiles_pixels[128 * 128] = { 0 };
	u32 display_pixels[224 * 288] = { 0 };
	const char* title = "";
	int width = 0;
	int height = 0;
};