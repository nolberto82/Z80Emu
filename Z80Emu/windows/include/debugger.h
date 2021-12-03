#pragma once

#include "types.h"
#include "cpu.h"
#include "mem.h"
#include "breakpoints.h"
#include "sdlgfx.h"
#include "glgfx.h"

#define TEXTSIZE 512
#define TOTAL_REGS 23

class Breakpoint;
class SDLGfx;
class GLGfx;

class Debugger
{
public:
	Debugger() {}
	~Debugger() {}

	void init(Cpu* cpu, Memory* mem, Breakpoint* bpk, SDLGfx* sdl)
	{
		this->cpu = cpu; this->mem = mem; this->bpk = bpk; this->sdl = sdl;;

		mdisasm.emplace(0x00, disasm_00);
		mdisasm.emplace(0xcb, disasm_cb); 
		mdisasm.emplace(0xdd, disasm_dd); 
		mdisasm.emplace(0xed, disasm_ed); 
		mdisasm.emplace(0xfd, disasm_fd);
	}

	void update_gl();
	void update();

private:
	void step();
	void gui(ImGuiIO io);
	void show_disassembly(u16 pc, int& lineoffset);
	void show_registers();
	void show_memory();
	void show_breakpoints();
	void show_buttons(u16 pc, u16& inputaddr, int& lineoffse, bool& is_jump, ImGuiIO io);
	void logger(u16 pc);
	std::vector<disasmentry> disasm(const char* text, u16 pc, bool get_registers = false);
	disasmdata get_disasm_entry(u8 op, u16 pc);
	u16 get_reg_value(int i);
	void get_test_messages();

	Cpu* cpu = nullptr;
	Memory* mem = nullptr;
	Breakpoint* bpk = nullptr;
	SDLGfx* sdl = nullptr;
	//GLGfx* gl = nullptr;

	std::vector<std::string> res;
	std::vector<disasmentry> vdentry;
	std::map<u8, disasmdata*> mdisasm;

	std::ofstream outFile;

	bool stepping = false;
	bool logging = false;
	bool showtiles = false;
	int numloops = 0;
	int item_id = 0;
	int test_number = 0;

	std::vector<std::string> test_result;

	enum bptype
	{
		bp_read = 1,
		bp_write = 2,
		bp_exec = 4
	};

	enum bpaddtype
	{
		add_bp,
		add_edit,
	};
};
