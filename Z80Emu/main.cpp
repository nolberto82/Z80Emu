#include <cstdio>

#include "debugger.h"
#include "cpu.h"
#include "mem.h"
#include "glgfx.h"

int main(int argc, char* argv[])
{
	Cpu* cpu = new Cpu();
	Memory* mem = new Memory();
	Debugger* dbg = new Debugger();
	Breakpoint* bpk = new Breakpoint();
	SDLGfx* sdl = new SDLGfx();

	bool rom_loaded = true;

	mem->init(cpu);
	cpu->init(mem);

#if RUN_TESTS
	if (!mem->load_test())
		rom_loaded = false;
	mem->set_test_number(0);
#else
	if (!mem->load_roms())
		rom_loaded = false;
#endif

	if (rom_loaded)
	{
		dbg->init(cpu, mem, bpk, sdl);

		dbg->update();

		sdl->clean();
	}

	delete bpk;
	delete sdl;
	delete cpu;
	delete mem;
	delete dbg;

	//delete gl;

	return 0;
}
