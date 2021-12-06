#include "sdlgfx.h"

bool SDLGfx::init(Memory* mem)
{
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	//SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");

	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	window = SDL_CreateWindow("Z80 Emulator", 10, SDL_WINDOWPOS_CENTERED, APP_WIDTH, APP_HEIGHT, window_flags);

	if (!window)
	{
		printf("Failed to open %d x %d window: %s\n", APP_WIDTH, APP_HEIGHT, SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		return false;
	}

	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, 1440, 1000);
	ImGui_ImplSDL2_InitForSDLRenderer(window);

	ImGui::StyleColorsLight();

	ImGuiStyle* style = &ImGui::GetStyle();

	style->Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.25f);
	//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0 / 255.0f, 95 / 255.0f, 184 / 255.0f, 1.0f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0, 0, 0, 1);
	style->FrameBorderSize = 1.10f;

	/*	spr_offsets[0].x = 8; spr_offsets[0].y = 12;
		spr_offsets[1].x = 8; spr_offsets[1].y = 12;
		spr_offsets[2].x = 8; spr_offsets[2].y = 12;
		spr_offsets[3].x = 8; spr_offsets[3].y = 12;
		spr_offsets[4].x = 8; spr_offsets[4].y = 12;
		spr_offsets[5].x = 8; spr_offsets[5].y = 12;
		spr_offsets[6].x = 8; spr_offsets[6].y = 12;
		spr_offsets[7].x = 8; spr_offsets[7].y = 12;*/

	create_texture();

	decode_graphics(mem);

	return true;
}

void SDLGfx::decode_graphics(Memory* mem)
{
	int x = 0;
	int y = 0;
	int pos = 0;

	for (int i = 0; i < mem->tiles.size() / 16; i++)
	{
		decode_tiles(mem, x, y, i, pos);
		x++;

		if (x % 16 == 0)
		{
			y++;
			x = 0;
		}
	}

	x = 0;
	y = 0;
	pos = 0;

	for (int i = 0; i < mem->sprites.size() / 64; i++)
	{
		decode_sprites(mem, i, pos);
	}

	std::ofstream outFile("tilepix.bin", std::ios::binary);
	outFile.write((char*)sprite_ids, sizeof(sprite_ids));
	outFile.close();
}

void SDLGfx::render_display(Memory* mem)
{
	memset(&display_pix, 0, sizeof(display_pix));

	int i = 0x3dd;

	//top
	for (int y = 0; y <= 8; y += 8)
	{
		for (int x = 0; x <= 216; x += 8)
		{
			u8 palid = mem->ram[0x4400 + i] & 0x3f;
			int offset = mem->ram[0x4000 + i] * 64;

			draw_tile(mem, display_pix, x, y, offset, palid);
			i--;
		}
		i += 0x3c;
	}

	i = 0x3a0;

	//middle
	for (int y = 16; y <= 264; y += 8)
	{
		for (int x = 0; x <= 216; x += 8)
		{
			u8 palid = mem->ram[0x4400 + i] & 0x3f;
			int offset = mem->ram[0x4000 + i] * 64;

			draw_tile(mem, display_pix, x, y, offset, palid);
			i -= 0x20;
		}
		i = i + 0x380 + 1;
	}

	i = 0x1d;

	//bottom
	for (int y = 272; y < 288; y += 8)
	{
		for (int x = 0; x <= 216; x += 8)
		{
			u8 palid = mem->ram[0x4400 + i] & 0x3f;
			int offset = mem->ram[0x4000 + i] * 64;

			draw_tile(mem, display_pix, x, y, offset, palid);
			i--;
		}
		i += 0x3c;
	}

	render_sprites(mem);

	SDL_UpdateTexture(display.texture, NULL, display_pix, display.w * sizeof(unsigned char) * 4);
	SDL_RenderCopy(renderer, display.texture, NULL, NULL);
}

void SDLGfx::render_sprites(Memory* mem)
{
	for (int i = 0; i < 8; i++)
	{
		u8 sprite_id = mem->ram[0x4ff0 + i * 2] >> 2;
		bool sprite_flipx = mem->ram[0x4ff0 + i * 2] & 0x02;
		bool sprite_flipy = mem->ram[0x4ff0 + i * 2] & 0x01;
		u8 sprite_pal = mem->ram[0x4ff0 + i * 2 + 1];
		u8 sprite_x = 224 - mem->sprite_data[i * 2] + 15;
		u8 sprite_y = 288 - mem->sprite_data[i * 2 + 1] - 16;

		if (sprite_x < 0)
			continue;

		draw_sprites(mem, sprite_x, sprite_y, sprite_id, sprite_pal, sprite_flipx, sprite_flipy);
		//x++;
	}

	//draw_sprites(mem, 0, 0, 0x24, 5, 0, 0);
}

void SDLGfx::render_tiles(Memory* mem, bool sprites)
{
	int x = 0;
	int y = 0;
	int pos = 0;

	memset(tile_pix, 0, sizeof(tile_pix));
	memset(sprite_pix, 0, sizeof(sprite_pix));

	if (sprites)
	{
		for (int i = 0; i < mem->sprites.size() / 64; i++)
		{
			draw_sprite_viewer(mem, x * 16, y * 16, i);
			x++;

			if (x % 8 == 0)
			{
				y++;
				x = 0;
			}
		}
		SDL_UpdateTexture(tile.texture, NULL, sprite_pix, tile.w * sizeof(unsigned char) * 4);
		SDL_RenderCopy(renderer, tile.texture, NULL, NULL);
	}
	else
	{
		for (int i = 0; i < mem->tiles.size() / 16; i++)
		{
			draw_tile_viewer(mem, tile_pix, tile_ids, x * 8, y * 8, i, 1);
			x++;

			if (x % 16 == 0)
			{
				y++;
				x = 0;
			}
		}
		SDL_UpdateTexture(tile.texture, NULL, tile_pix, tile.w * sizeof(unsigned char) * 4);
		SDL_RenderCopy(renderer, tile.texture, NULL, NULL);
	}
}

void SDLGfx::draw_tile(Memory* mem, u32* pixels, int x, int y, int offset, u8 palid)
{
	u32 temp[8 * 4] = { 0 };

	u8 pl[4];
	pl[0] = mem->palettes[palid * 4 + 0];
	pl[1] = mem->palettes[palid * 4 + 1];
	pl[2] = mem->palettes[palid * 4 + 2];
	pl[3] = mem->palettes[palid * 4 + 3];

	for (int j = 0; j < 2; j++)
	{
		for (int yy = 0; yy < 4; yy++)
		{
			for (int xx = 0; xx < 8; xx++)
			{
				u8 pix = tile_ids[offset++];
				u8 pal = pl[pix];
				u8 col = mem->colors[pal];

				u32 c = get_palette(col);

				int pos = (y + (j * 4) + yy) * 224 + (x + xx);

				pixels[pos] = c;

				if (offset == 0x3f7 && j == 1)
				{
					temp[yy * 8 + xx] = c;
				}
			}
		}
		//offset -= 8;
	}

	//if (offset == 0x3f7)
	//{
	//	SDL_UpdateTexture(chars.texture, NULL, temp, chars.w * sizeof(unsigned char) * 4);
	//	SDL_RenderCopy(renderer, chars.texture, NULL, NULL);
	//}
}

void SDLGfx::draw_sprites(Memory* mem, int x, int y, int i, u8 palid, int flipx, int flipy)
{
	//u8 palid = mem->ram[0x4400 + i] & 0x3f;
	int offset = i * 256;
	//y = 32;
	//x = 0;

	if (x < 0 || x >= 224)
		return;

	u8 pl[4];
	pl[0] = mem->palettes[palid * 4 + 0];
	pl[1] = mem->palettes[palid * 4 + 1];
	pl[2] = mem->palettes[palid * 4 + 2];
	pl[3] = mem->palettes[palid * 4 + 3];

	//for (int j = 0; j < 2; j++)
	//{
	for (int yy = 0; yy < 16; yy++)
	{
		for (int xx = 0; xx < 16; xx++)
		{
			u8 pix = sprite_ids[offset++];
			u8 pal = pl[pix];
			u8 col = mem->colors[pal];

			if (pix == 0)
				continue;

			int fx = flipx ? 15 - xx : xx;
			int fy = flipy ? 15 - yy : yy;

			if (x + fx < 0 || x + fx > 224)
				continue;

			u32 c = get_palette(col);
			int pos = (y + fy) * 224 + (x + fx);

			display_pix[pos] = c;
		}
	}
	//offset -= 8;
//}
}

void SDLGfx::draw_tile_viewer(Memory* mem, u32* pixels, u8* color_ids, int x, int y, int i, int size)
{
	u8 palid = 0x1;
	int offset = i * 64 * size;
	int tilepos = y * 128 + x;
	u32 temp[8 * 4];

	u8 pl[4];
	pl[0] = mem->palettes[palid * 4 + 0];
	pl[1] = mem->palettes[palid * 4 + 1];
	pl[2] = mem->palettes[palid * 4 + 2];
	pl[3] = mem->palettes[palid * 4 + 3];

	for (int j = 0; j < 2 + size; j++)
	{
		memset(temp, 0, sizeof(temp));

		for (int yy = 0; yy < 4; yy++)
		{
			for (int xx = 0; xx < 8; xx++)
			{
				u8 pix = color_ids[offset++];
				u8 pal = pl[pix];
				u8 col = mem->colors[pal];

				u32 c = get_palette(col);

				//if (pix)
				temp[yy * 8 + xx] = c;

				tile_pix[(y + (j * 4) + yy) * 128 * size + (x + xx)] = c;
			}
		}
	}
}

void SDLGfx::draw_sprite_viewer(Memory* mem, int x, int y, int i)
{
	u8 palid = 0x1;
	int offset = i * 256;
	int base = i * 256;
	u32 temp[8 * 4];

	u8 pl[4];
	pl[0] = mem->palettes[palid * 4 + 0];
	pl[1] = mem->palettes[palid * 4 + 1];
	pl[2] = mem->palettes[palid * 4 + 2];
	pl[3] = mem->palettes[palid * 4 + 3];

	memset(temp, 0, sizeof(temp));

	for (int yy = 0; yy < 16; yy++)
	{
		for (int xx = 0; xx < 16; xx++)
		{
			u8 pix = sprite_ids[offset++];
			u8 pal = pl[pix];
			u8 col = mem->colors[pal];

			u32 c = get_palette(col);

			sprite_pix[(y + yy) * 128 + (x + xx)] = c;
		}
	}
}

void SDLGfx::decode_tiles(Memory* mem, int x, int y, int i, int& pos)
{
	int offset = i * 16 + 15;
	u8 temp[8 * 4];

	for (int j = 0; j < 2; j++)
	{
		memset(temp, 0, sizeof(temp));
		for (int xx = 7; xx >= 0; xx--)
		{
			u8 tid = mem->tiles[offset - xx];

			u8 h = 3, l = 0;

			for (int yy = 3; yy >= 0; yy--)
			{
				u8 pix = ((tid >> h++)) & 2 | (tid >> l++) & 1;
				temp[yy * 8 + xx] = pix;
			}
		}

		memcpy(&tile_ids[pos], temp, sizeof(temp));
		offset -= 8;
		pos += 32;
	}
}

void SDLGfx::decode_sprites(Memory* mem, int i, int& pos)
{
	int offset = i * 64;
	int base = i * 256;
	u8 temp[16][16];

	sprite_dec_t spr_off[] =
	{
		{ 8, 12},{ 8, 0},{ 8, 4},{ 8, 8},
		{ 0, 12},{ 0, 0},{ 0, 4},{ 0, 8}
	};

	for (int j = 0; j < 8; j++)
	{
		memset(temp, 0, sizeof(temp));
		memset(tile16, 0, sizeof(tile16));

		for (int xx = 7; xx >= 0; xx--)
		{
			u8 tid = mem->sprites[offset++];

			u8 h = 3;
			u8 l = 0;

			for (int yy = 3; yy >= 0; yy--)
			{
				u8 pix = ((tid >> h++)) & 2 | (tid >> l++) & 1;
				//temp[yy + spr_off[j].y][xx + spr_off[j].x] = pix;

				if (pix)
					tile16[yy + spr_off[j].y][xx + spr_off[j].x] = 0xff0000ff;
				else
					tile16[yy + spr_off[j].y][xx + spr_off[j].x] = 0;

				int p = base + (yy + spr_off[j].y) * 16 + (xx + spr_off[j].x);
				sprite_ids[p] = pix;
			}

			//memcpy(&sprite_ids[pos], temp, 16 * sizeof(u8));
			//offset += 8;
			//pos += 8;
		}
	}

	if (i == 48)
	{
		SDL_UpdateTexture(chars.texture, NULL, tile16, chars.w * sizeof(unsigned char) * 4);
		SDL_RenderCopy(renderer, chars.texture, NULL, NULL);
	}
}

u32 SDLGfx::get_palette(u8 col)
{
	u8 r = ((col >> 0) & 1) * 0x21 | ((col >> 1) & 1) * 0x47 | ((col >> 2) & 1) * 0x97;
	u8 g = ((col >> 3) & 1) * 0x21 | ((col >> 4) & 1) * 0x47 | ((col >> 5) & 1) * 0x97;
	u8 b = ((col >> 6) & 1) * 0x51 | ((col >> 7) & 1) * 0xae;
	return 0xff000000 | b << 16 | g << 8 | r;
}

void SDLGfx::get_tile_pixels(u32* pixels, std::vector<u8> tiles, int x, int y, int offset, int texsize, int size)
{
	u32 temp[8 * 4] = { 0 };

	for (int xx = 0; xx < 8; xx++)
	{
		u8 tid = tiles[offset - xx];
		u8 h = 3, l = 0;

		for (int yy = 3; yy >= 0; yy--)
		{
			u8 pix = ((tid >> h++)) & 2 | (tid >> l++) & 1;
			if (pix)
			{
				temp[yy * 8 + xx] = 0xff0000ff;
				pixels[texsize * yy + y * size + xx + x] = 0xff0000ff;
			}
			else
			{
				temp[yy * 8 + xx] = 0;
				pixels[texsize * yy + y * size + xx + x] = 0;
			}
		}
	}

	//SDL_UpdateTexture(chars.texture, NULL, temp, chars.w * sizeof(unsigned char) * 4);
	//SDL_RenderCopy(renderer, chars.texture, NULL, NULL);
}

void SDLGfx::begin_frame()
{
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
	SDL_RenderClear(renderer);
}

void SDLGfx::end_frame()
{
	// Rendering
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(renderer);
}

void SDLGfx::update_input(int& running, ImGuiIO io)
{
	int wheel = 0;

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_QUIT)
			running = false;
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				io.DisplaySize.x = static_cast<float>(e.window.data1);
				io.DisplaySize.y = static_cast<float>(e.window.data2);
			}
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			wheel = e.wheel.y;
		}
	}

	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);
}

SDL_Window* SDLGfx::get_window()
{
	return window;
}

void SDLGfx::create_texture()
{
	tile.w = 128; tile.h = 128;
	display.w = 224; display.h = 288;
	chars.w = 16; chars.h = 16;
	tile.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, tile.w, tile.h);
	display.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, display.w, display.h);
	chars.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, chars.w, chars.h);
}

SDL_Renderer* SDLGfx::get_renderer()
{
	return renderer;
}

void SDLGfx::clean()
{
	ImGuiSDL::Deinitialize();

	SDL_DestroyTexture(tile.texture);
	SDL_DestroyTexture(display.texture);
	SDL_DestroyTexture(chars.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	ImGui::DestroyContext();

	SDL_Quit();
}
