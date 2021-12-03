#pragma once

#include "types.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_sdl.h"
#include "imgui_impl_sdl.h"
#include "imgui_memory_editor.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "mem.h"

struct texture_t
{
	int w;
	int h;
	SDL_Texture* texture;
};

struct sprite_dec_t
{
	u8 x, y;
};

class SDLGfx
{
public:
	SDLGfx() {}
	~SDLGfx() { }

	bool init(Memory* mem);
	void decode_graphics(Memory* mem);
	void render_display(Memory* mem);
	void render_sprites(Memory* mem);
	void render_tiles(Memory* mem, bool sprites);
	void draw_tile(Memory* mem, u32* pixels, int x, int y, int offset, u8 palid);
	void draw_sprites(Memory* mem, int x, int y, int i, u8 palid, int flipx, int flipy);
	void draw_tile_viewer(Memory* mem, u32* pixels, u8* color_ids, int x, int y, int i, int size);
	void draw_sprite_viewer(Memory* mem, int x, int y, int i);
	void decode_tiles(Memory* mem, int x, int y, int i, int& pos);
	void decode_sprites(Memory* mem, int i, int& pos);
	u32 get_palette(u8 col);
	void get_tile_pixels(u32* pixels, std::vector<u8> tiles, int x, int y, int offset, int texsize, int size);
	void begin_frame();
	void end_frame();
	void update_input(int& running, ImGuiIO io);
	void create_texture();
	SDL_Texture* get_display() { return display.texture; };
	SDL_Texture* get_tile() { return tile.texture; };
	SDL_Texture* get_chars() { return chars.texture; };
	SDL_Window* get_window();
	SDL_Renderer* get_renderer();
 
	void clean();

	void update_tile16()
	{
		SDL_UpdateTexture(chars.texture, NULL, tile16, chars.w * 4);
		SDL_RenderCopy(renderer, chars.texture, NULL, NULL);
	}

private:
	Memory* mem;

	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;
	SDL_Texture* font = nullptr;
	texture_t display = {};
	texture_t tile = {};
	texture_t chars = {};
	//sprite_dec_t spr_offsets[8] = {};
	SDL_Joystick* joy = nullptr;
	u32 tile16[16][16];
	u32 tile_pix[128 * 128] = { 0 };
	u32 sprite_pix[128 * 128] = { 0 };
	u32 display_pix[224 * 288] = { 0 };
	u8 tile_ids[128 * 128] = { 0 };
	u8 sprite_ids[128 * 128] = { 0 };
	const char* title = "";
	int width = 0;
	int height = 0;
};

