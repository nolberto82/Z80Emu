#include "mem.h"
#include <sdlgfx.h>

bool Memory::load_roms()
{
	if (!load_file("roms/pacman.6e", ram, 0x0000))
		return false;
	if (!load_file("roms/pacman.6f", ram, 0x1000))
		return false;
	if (!load_file("roms/pacman.6h", ram, 0x2000))
		return false;
	if (!load_file("roms/pacman.6j", ram, 0x3000))
		return false;
	if (!load_file("roms/pacman.5e", tiles, 0x0000))
		return false;
	if (!load_file("roms/pacman.5f", sprites, 0x0000))
		return false;
	if (!load_file("roms/82s123.7f", colors, 0x0000))
		return false;
	if (!load_file("roms/82s126.4a", palettes, 0x0000))
		return false;

	return true;
}

bool Memory::load_test()
{
	std::fill(ram.begin(), ram.end(), 0xff);
	std::fill(ports.begin(), ports.end(), 0x00);

	if (!load_file("testfiles/interface.bin", ram, 0x0000))
		return false;
	if (!load_file("testfiles/zexall.bin", ram, 0x0100))
		return false;

	reset();

	ram[0xfffd] = 0x00;
	ram[0xfffe] = 0x00;
	ram[0xffff] = 0x00;

	cpu->resstr = "";

	//ram[0x0000] = 0xd3;
	//ram[0x0001] = 0x00;
	//ram[0x0005] = 0xdb;
	//ram[0x0006] = 0x00;
	//ram[0x0007] = 0xc9;

	return true;
}

//bool Memory::load_test_tap()
//{
//	std::fill(ram.begin(), ram.end(), 0x00);
//	std::fill(ports.begin(), ports.end(), 0x00);
//
//	if (!load_file("testfiles/z80doc.tap", ram, 0x8000))
//		return false;
//
//	//set_test_number(0);
//
//	reset();
//
//
//
//	return true;
//}

bool Memory::load_file(const char* filename, std::vector<u8>& rom, int offset)
{
	int size = 0;
	bool res = true;

	FILE* fp;
	fp = fopen(filename, "rb");

	if (fp == NULL)
	{
		printf("Couldn't open file");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(&rom[offset], 1, size, fp);

	fclose(fp);

	return res;
}

void Memory::reset()
{
	cpu->wz = 0x0000;
#if RUN_TESTS
	cpu->pc = 0x0000;
#else
	std::fill(ram.begin() + 0x4000, ram.end(), 0x00);
	std::fill(ports.begin(), ports.end(), 0x00);

	for (int i = 0x5000; i < 0x5080; i++)
		ram[i] = 0xff;

	for (int i = 0x5080; i < 0x50c0; i++)
		ram[i] = 0xc9;
#endif
}

void Memory::reset_test()
{
	cpu->pc = 0x0100;
	cpu->wz = 0x0000;
}

u8 Memory::rb(u16 addr, bool opcode)
{
	if (!opcode)
		cpu->set_read_addr(addr);
	return ram[addr];
}

u8* Memory::read_byte_ptr(u16 addr)
{
	return &ram[addr];
}

u16 Memory::rw(u16 addr)
{

	cpu->set_read_addr(addr);

	return ram[addr + 1] << 8 | ram[addr];
}

u16* Memory::read_word_ptr(u16 addr)
{

	cpu->set_read_addr(addr);

	u16 res = rw(addr);

	return &res;
}


void Memory::wb(u16 addr, u8 v)
{

	cpu->set_write_addr(addr);

	//if (addr < 0x4000)
	//	return;
	if (addr >= 0x5000 && addr < 0x6000)
	{
		if (addr == 0x5000)
			cpu->set_interrupt(v);
		else if (addr >= 0x5060 && addr <= 0x506f)
			sprite_data[addr & 0xf] = v;
		return;
	}
	else if (addr >= 0x8000)
	{
		ram[addr] = v;
		ram[addr & 0x4fff] = v;

#if RUN_TESTS
		if (addr == 0xfffe)
			ram[addr - 1] = v;
#endif
		return;
	}

	ram[addr] = v;
}

void Memory::ww(u16 addr, u16 v)
{
#if !RUN_TESTS
	if (addr < 0x4000)
		return;
	if (addr >= 0x5000)
		return;
#endif

	wb(addr + 0, v & 0xff);
	wb(addr + 1, v >> 8);

	cpu->set_write_addr(addr);
}

void Memory::set_test_number(int test)
{
	u16 test_offset = TESTS_START + (test * 2);

	u8 l = ram[test_offset + 0];
	u8 h = ram[test_offset + 1];

	ram[TESTS_START + 0] = l;
	ram[TESTS_START + 1] = h;
	ram[TESTS_START + 2] = 0;
	ram[TESTS_START + 3] = 0;
}
