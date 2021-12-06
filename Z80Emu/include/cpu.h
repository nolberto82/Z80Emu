#pragma once

#include "types.h"
#include "mem.h"

#if RUN_TESTS
const int CYCLES_PER_FRAME = 1000000;
#else
const int CYCLES_PER_FRAME = 3072000 / 60;
#endif
#define TOTAL_REGS 23

const int TESTS_START = 0x13a;

#define FC 0x01
#define FN 0x02
#define FP 0x04
#define FX 0x08
#define FH 0x10
#define FY 0x20
#define FZ 0x40
#define FS 0x80

class Memory;

typedef struct
{
	std::string name;
	std::string format;
}sregdata;

class Cpu
{
public:
	union
	{
		struct
		{
			u8 f, a;
		}; u16 af;
	};

	union
	{
		struct
		{
			u8 c, b;
		};
		u16 bc;
	};

	union
	{
		struct
		{
			u8 e, d;
		};
		u16 de;
	};

	union
	{
		struct
		{
			u8 l, h;
		};
		u16 hl;
	};

	union
	{
		struct
		{
			u8 ixl, ixh;
		};
		u16 ix;
	};

	union
	{
		struct
		{
			u8 iyl, iyh;
		};
		u16 iy;
	};

	union
	{
		struct
		{
			u8 sa, sf;
		};
		u16 saf;
	};

	union
	{
		struct
		{
			u8 sb, sc;
		};
		u16 sbc;
	};

	union
	{
		struct
		{
			u8 sd, se;
		};
		u16 sde;
	};

	union
	{
		struct
		{
			u8 sh, sl;
		};
		u16 shl;
	};

	union
	{
		struct
		{
			u8 sx, sy;
		};
		u16 sxy;
	};

	union
	{
		struct
		{
			u8 z, w;
		};
		u16 wz;
	};

	u16 sp;
	u16 pc;
	u16 prevpc;
	u8 i;
	u8 r;

public:
	Cpu() {}
	~Cpu() {}

	void reset();
	void step();
	void set_write_addr(u16 addr) { writeaddr = addr; }
	u16 get_write_addr() { return writeaddr; }
	void set_read_addr(u16 addr) { readaddr = addr; }
	u16 get_read_addr() { return readaddr; }
	void set_interrupt(u8 v) { inte = v; }
	int* get_im() { return &im; }
	bool* get_iff1() { return &iff1; }
	bool* get_iff2() { return &iff2; }
	bool* get_halt() { return &halt; }
	int* get_state() { return &state; }
	void set_state(int v) { state = v; }
	int* get_cycles() { return &cycles; }
	std::vector<sregdata> get_reginfo() { return reginfo; }
	void set_port(u8 v);

	void init(Memory* mem)
	{
		this->mem = mem;
		reset();
	}

	std::string resstr = "";
	std::string crcstr = "";

private:
	Memory* mem;

	std::vector<sregdata> reginfo;
	u8* reg_8bits[8];
	u16* reg_16bits[8];

	typedef void (Cpu::* functions)();

	std::vector<disasmdata> op_00;

	//disasm

	int numloops = 0;
	int state, cycles, curr_cycles, im, beamx, beamy, writeaddr, readaddr = 0;
	bool fc, fn, fp, fv, f3, f5, fh, fz, fs = false;
	bool iff1, iff2, inte, halt, crashed = false;
	bool is_conditional;

	void exec_00(u8 op);
	void exec_cb(u8 op);
	void exec_dd(u8 op);
	void exec_ed(u8 op);
	void exec_fd(u8 op);
	void exec_ddfd(u8 op, u8 b1, u16 reg);

	void op_adc8(u8 r1);
	u16 op_adc(u16 r1, u16 r2);
	void op_add8(u8 r1);
	u16 op_add(u16 r1, u16 r2);
	void op_and(u8 r1);
	void op_bit(u8 r1, u8 r2, s16 addr = -1);
	void op_call(u8 op, bool flag);
	void op_ccf();
	void op_cp(u8 r1);
	void op_cpd();
	void op_cpdr(u8 op);
	void op_cpi();
	void op_cpir(u8 op);
	void op_cpl();
	void op_daa();
	u8 op_dec8(u8 r1);
	void op_dechl(u16 r1, u16 r2);
	void op_djnz(u8 r1);
	void op_ex(u16* r1, u16* r2);
	void op_exx();
	void op_halt();
	u8 op_inc8(u8 r1);
	void op_inchl(u16 r1, u16 r2);
	void op_jp(u8 op, bool flag);
	bool op_jr(u8 op, bool flag);
	void op_ldd();
	void op_lddr(u8);
	void op_ldi();
	void op_ldir(u8 op);
	void op_neg();
	void op_or(u8 r1);
	u8 op_pop8();
	u16 op_pop();
	void op_push8(u8 r1);
	void op_push(u16 r1);
	void op_ret(u8 r1, bool flag);
	u8 op_res(u8 r1, u8 r2);
	u8 op_rl(u8 r1);
	void op_rla8();
	u8 op_rlc(u8 r1);
	void op_rlca();
	void op_rld();
	u8 op_rr(u8 r1);
	u8 op_rrc(u8 r1);
	void op_rrca();
	void op_rra();
	void op_rrd();
	void op_rst(u8 r1);
	u8 op_set(u8 r1, u8 r2);
	void op_sbc8(u8 r1);
	u16 op_sbc(u16 r1, u16 r2);
	void op_scf();
	u8 op_sla(u8 r1);
	u8 op_sra(u8 r1);
	u8 op_sll(u8 r1);
	u8 op_srl(u8 r1);
	void op_sub8(u8 r1);
	void op_xor(u8 r1);

	void handle_interrupts();
	void exchange_values(u16* r1, u16* r2);
	void add_pc(u8 op);
	void add_cyc(u8 op);
	void set_flag(bool flag, u8 v);
	bool get_parity(u8 op);
	u16 get_addr(u16 a1, u16 a2);
	void advance(u8 op);
};

enum cgame
{
	pacmidway,
	mspacman,
	pacnamco
};

enum cstate
{
	running = 1,
	debugging = 2,
	crashed = 4
};