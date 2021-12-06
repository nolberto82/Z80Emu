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

	bool load_roms(int gamei);
	bool load_file(const char* filename, std::vector<u8>& rom, int offset);
	bool load_test();
	void reset();
	void reset_test();
	u8 rb(u16 addr, bool opcode = false);
	u16 rw(u16 addr);
	void wb(u16 addr, u8 v);
	void ww(u16 addr, u16 v);
	void set_port(u8 id, u8 v) { ports[id] = v; }
	u8 get_port(u8 id) { return ports[id]; }
	int get_test_number() { return test_number; }
	void set_test_number(int test);

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

