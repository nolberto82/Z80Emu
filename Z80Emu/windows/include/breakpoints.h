#pragma once

#include "types.h"
#include "debugger.h"

class Debugger;

struct bplist
{
	u16 addr;
	u8 enabled;
	u8 type;
};

class Breakpoint
{
public:
	Breakpoint() {}
	~Breakpoint() {}
	void init(Debugger* dbg);
	void add(u16 addr, u8 type);
	void edit(u16 addr, u8 type, s16 newaddr = -1, bool enabled = true);
	void remove(u16 addr);
	bool check(u16 addr, u8 type, bool enabled);
	bool check_access(u16 addr, u8 type, bool enabled = false);
	std::vector<bplist>::iterator find(u16 addr);
	std::vector<bplist> get_breakpoints();

	u16 addr = 0;
	bool enabled = false;
	u8 type = 0;

private:
	Debugger* dbg = nullptr;

	std::vector<bplist> breakpoints;

	enum bptype
	{
		read = 1,
		write = 2,
		exec = 4
	};
};
