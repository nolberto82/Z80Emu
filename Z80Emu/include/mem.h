#pragma once

#include "types.h"

#include "cpu.h"

class Cpu;

struct sprite_t
{
	u8 x, y, id, flipx, flipy, pal;
};

static sprite_t sprite[8];

class Memory
{
public:
	Memory() {}
	~Memory() {}

	bool load_roms();
	bool load_file(const char* filename, std::vector<u8>& rom, int offset);
	bool load_test();
	bool load_test_tap();
	void reset();
	void reset_test();
	u8 rb(u16 addr, bool opcode = false);
	u8* read_byte_ptr(u16 addr);
	u16 rw(u16 addr);
	u16* read_word_ptr(u16 addr);
	void wb(u16 addr, u8 v);
	void ww(u16 addr, u16 v);
	void set_port(u8 id, u8 v) { ports[id] = v; }
	u8 get_port(u8 id) { return ports[id]; }
	int get_test_number() { return test_number; }
	void set_test_number(int test);
	//std::vector<u8> get_tiles() { return tiles; }
	//std::vector<u8> get_sprites() { return sprites; }
	//std::vector<u8> get_palettes() { return palettes; }
	//std::vector<u8> get_colors() { return colors; }
	//std::vector<u8> get_ram() { return ram; }
	//std::vector<u8> get_spr_data() { return sprite_data; }

	void init(Cpu* cpu)
	{
		this->cpu = cpu;

		ram.resize(0x10000);
		tiles.resize(0x1000);
		sprites.resize(0x1000);
		colors.resize(0x20);
		palettes.resize(0x100);
		ports.resize(0xa0);
		sprite_data.resize(0x10);
	}

	std::vector<u8> ram;
	std::vector<u8> tiles;
	std::vector<u8> sprites;
	std::vector<u8> colors;
	std::vector<u8> palettes;
	std::vector<u8> ports;

	std::vector<u8> sprite_data;

private:
	Cpu* cpu = nullptr;

	u8 test_number = 0;
};

