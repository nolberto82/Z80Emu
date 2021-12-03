#include "cpu.h"

void Cpu::step()
{
	u16 pc = r.pc;
	u8 op = mem->rb(pc, true);

	switch (op)
	{
	case 0x00: advance(op); break;
	case 0x01: r.bc = mem->rw(r.pc + 1); advance(op); break;
	case 0x02: mem->wb(r.bc, r.a); advance(op); break;
	case 0x03: r.bc++; advance(op); break;
	case 0x04: r.b = op_inc8(r.b); advance(op); break;
	case 0x05: r.b = op_dec8(r.b); advance(op); break;
	case 0x06: r.b = mem->rb(r.pc + 1); advance(op); break;
	case 0x07: op_rlca(); advance(op); break;

	case 0x09: r.hl = op_add(r.hl, r.bc); advance(op); break;
	case 0x0a: r.a = mem->rb(r.bc); advance(op); break;
	case 0x0b: r.bc--; advance(op); break;
	case 0x0c: r.c = op_inc8(r.c); advance(op); break;
	case 0x0d: r.c = op_dec8(r.c); advance(op); break;
	case 0x0e: r.c = mem->rb(r.pc + 1); advance(op); break;
	case 0x0f: op_rrca(); advance(op); break;
	case 0x10: op_djnz(op); break;
	case 0x11: r.de = mem->rw(r.pc + 1); advance(op); break;
	case 0x12: mem->wb(r.de, r.a); advance(op); break;
	case 0x13: r.de++; advance(op); break;
	case 0x14: r.d = op_inc8(r.d); advance(op); break;
	case 0x15: r.d = op_dec8(r.d); advance(op); break;
	case 0x16: r.d = mem->rb(r.pc + 1); advance(op); break;
	case 0x17: op_rla8(); advance(op); break;
	case 0x18: op_jr(op, true); break;
	case 0x19: r.hl = op_add(r.hl, r.de); advance(op); break;
	case 0x1a: r.a = mem->rb(r.de); advance(op); break;
	case 0x1b: r.de--; advance(op); break;
	case 0x1c: r.e = op_inc8(r.e); advance(op); break;
	case 0x1d: r.e = op_dec8(r.e); advance(op); break;
	case 0x1e: r.e = mem->rb(r.pc + 1); advance(op); break;
	case 0x1f: op_rra(); advance(op); break;
	case 0x20: op_jr(op, !(r.f & FZ)); break;

	case 0x21: r.hl = mem->rw(r.pc + 1); advance(op); break;
	case 0x22: mem->ww(mem->rw(r.pc + 1), r.hl); advance(op); break;
	case 0x23: r.hl++; advance(op); break;
	case 0x24: r.h = op_inc8(r.h); advance(op); break;
	case 0x25: r.h = op_dec8(r.h); advance(op); break;
	case 0x26: r.h = mem->rb(r.pc + 1); advance(op); break;
	case 0x27: op_daa(); advance(op); break;
	case 0x28: op_jr(op, (r.f & FZ)); break;
	case 0x29: r.hl = op_add(r.hl, r.hl); advance(op); break;
	case 0x2a: r.hl = mem->rw(mem->rw(r.pc + 1)); advance(op); break;
	case 0x2b: r.hl--; advance(op); break;
	case 0x2c: r.l = op_inc8(r.l); advance(op); break;
	case 0x2d: r.l = op_dec8(r.l); advance(op); break;
	case 0x2e: r.l = mem->rb(r.pc + 1); advance(op); break;
	case 0x2f: op_cpl(); advance(op); break;
	case 0x30: op_jr(op, !(r.f & FC)); break;
	case 0x31: r.sp = mem->rw(r.pc + 1); advance(op); break;
	case 0x32: mem->wb(mem->rw(r.pc + 1), r.a); advance(op); break;
	case 0x33: r.sp++; advance(op); break;
	case 0x34: mem->wb(r.hl, op_inc8(mem->rb(r.hl))); advance(op); break;
	case 0x35: mem->wb(r.hl, op_dec8(mem->rb(r.hl))); advance(op); break;
	case 0x36: mem->wb(r.hl, mem->rb(r.pc + 1)); advance(op); break;
	case 0x37: op_scf(); advance(op); break;
	case 0x38: op_jr(op, (r.f & FC)); break;
	case 0x39: r.hl = op_add(r.hl, r.sp); advance(op); break;
	case 0x3a: r.a = mem->rb(mem->rw(r.pc + 1)); advance(op); break;
	case 0x3b: r.sp--; advance(op); break;
	case 0x3c: r.a = op_inc8(r.a); advance(op); break;
	case 0x3d: r.a = op_dec8(r.a); advance(op); break;
	case 0x3e: r.a = mem->rb(r.pc + 1); advance(op); break;
	case 0x3f: op_ccf(); advance(op); break;
	case 0x40: advance(op); break;

	case 0x41: r.b = r.c; advance(op); break;
	case 0x42: r.b = r.d; advance(op); break;
	case 0x43: r.b = r.e; advance(op); break;
	case 0x44: r.b = r.h; advance(op); break;
	case 0x45: r.b = r.l; advance(op); break;
	case 0x46: r.b = mem->rb(r.hl); advance(op); break;
	case 0x47: r.b = r.a; advance(op); break;
	case 0x48: r.c = r.b; advance(op); break;
	case 0x49: advance(op); break;
	case 0x4a: r.c = r.d; advance(op); break;
	case 0x4b: r.c = r.e; advance(op); break;

	case 0x4c: r.c = r.h; advance(op); break;
	case 0x4d: r.c = r.l; advance(op); break;
	case 0x4e: r.c = mem->rb(r.hl); advance(op); break;
	case 0x4f: r.c = r.a; advance(op); break;
	case 0x50: r.d = r.b; advance(op); break;
	case 0x51: r.d = r.c; advance(op); break;
	case 0x52: advance(op); break;
	case 0x53: r.d = r.e; advance(op); break;
	case 0x54: r.d = r.h; advance(op); break;
	case 0x55: r.d = r.l; advance(op); break;
	case 0x56: r.d = mem->rb(r.hl); advance(op); break;
	case 0x57: r.d = r.a; advance(op); break;
	case 0x58: r.e = r.b; advance(op); break;
	case 0x59: r.e = r.c; advance(op); break;
	case 0x5a: r.e = r.d; advance(op); break;
	case 0x5b: add_pc(op); break;
	case 0x5c: r.e = r.h; advance(op); break;
	case 0x5d: r.e = r.l; advance(op); break;
	case 0x5e: r.e = mem->rb(r.hl); advance(op); break;
	case 0x5f: r.e = r.a; advance(op); break;
	case 0x60: r.h = r.b; advance(op); break;
	case 0x61: r.h = r.c; advance(op); break;
	case 0x62: r.h = r.d; advance(op); break;
	case 0x63: r.h = r.e; advance(op); break;
	case 0x64: advance(op); break;
	case 0x65: r.h = r.l; advance(op); break;
	case 0x66: r.h = mem->rb(r.hl); advance(op); break;
	case 0x67: r.h = r.a; advance(op); break;
	case 0x68: r.l = r.b; advance(op); break;
	case 0x69: r.l = r.c; advance(op); break;
	case 0x6a: r.l = r.d; advance(op); break;
	case 0x6b: r.l = r.e; advance(op); break;
	case 0x6c: r.l = r.h; advance(op); break;
	case 0x6d: advance(op); break;
	case 0x6e: r.l = mem->rb(r.hl); advance(op); break;
	case 0x6f: r.l = r.a; advance(op); break;

	case 0x70: mem->wb(r.hl, r.b); advance(op); break;
	case 0x71: mem->wb(r.hl, r.c); advance(op); break;
	case 0x72: mem->wb(r.hl, r.d); advance(op); break;
	case 0x73: mem->wb(r.hl, r.e); advance(op); break;
	case 0x74: mem->wb(r.hl, r.h); advance(op); break;
	case 0x75: mem->wb(r.hl, r.l); advance(op); break;
	case 0x76: halt = true; break;
	case 0x77: mem->wb(r.hl, r.a); advance(op); break;
	case 0x78: r.a = r.b; advance(op); break;
	case 0x79: r.a = r.c; advance(op); break;
	case 0x7a: r.a = r.d; advance(op); break;
	case 0x7b: r.a = r.e; advance(op); break;
	case 0x7c: r.a = r.h; advance(op); break;
	case 0x7d: r.a = r.l; advance(op); break;
	case 0x7e: r.a = mem->rb(r.hl); advance(op); break;
	case 0x7f: advance(op); break;
	case 0x80: op_add8(r.b); advance(op); break;
	case 0x81: op_add8(r.c); advance(op); break;
	case 0x82: op_add8(r.d); advance(op); break;
	case 0x83: op_add8(r.e); advance(op); break;
	case 0x84: op_add8(r.h); advance(op); break;
	case 0x85: op_add8(r.l); advance(op); break;
	case 0x86: op_add8(mem->rb(r.hl)); advance(op); break;
	case 0x87: op_add8(r.a); advance(op); break;
	case 0x88: op_adc8(r.b); advance(op); break;
	case 0x89: op_adc8(r.c); advance(op); break;
	case 0x8a: op_adc8(r.d); advance(op); break;
	case 0x8b: op_adc8(r.e); advance(op); break;
	case 0x8c: op_adc8(r.h); advance(op); break;
	case 0x8d: op_adc8(r.l); advance(op); break;
	case 0x8e: op_adc8(mem->rb(r.hl)); advance(op); break;
	case 0x8f: op_adc8(r.a); advance(op); break;
	case 0x90: op_sub8(r.b); advance(op); break;
	case 0x91: op_sub8(r.c); advance(op); break;
	case 0x92: op_sub8(r.d); advance(op); break;
	case 0x93: op_sub8(r.e); advance(op); break;
	case 0x94: op_sub8(r.h); advance(op); break;
	case 0x95: op_sub8(r.l); advance(op); break;
	case 0x96: op_sub8(mem->rb(r.hl)); advance(op); break;
	case 0x97: op_sub8(r.a); advance(op); break;
	case 0x98: op_sbc8(r.b); advance(op); break;
	case 0x99: op_sbc8(r.c); advance(op); break;
	case 0x9a: op_sbc8(r.d); advance(op); break;
	case 0x9b: op_sbc8(r.e); advance(op); break;
	case 0x9c: op_sbc8(r.h); advance(op); break;
	case 0x9d: op_sbc8(r.l); advance(op); break;
	case 0x9e: op_sbc8(mem->rb(r.hl)); advance(op); break;
	case 0x9f: op_sbc8(r.a); advance(op); break;
	case 0xa0: op_and(r.b); advance(op); break;
	case 0xa1: op_and(r.c); advance(op); break;
	case 0xa2: op_and(r.d); advance(op); break;
	case 0xa3: op_and(r.e); advance(op); break;
	case 0xa4: op_and(r.h); advance(op); break;
	case 0xa5: op_and(r.l); advance(op); break;
	case 0xa6: op_and(mem->rb(r.hl)); advance(op); break;
	case 0xa7: op_and(r.a); advance(op); break;
	case 0xa8: op_xor(r.b); advance(op); break;
	case 0xa9: op_xor(r.c); advance(op); break;
	case 0xaa: op_xor(r.d); advance(op); break;
	case 0xab: op_xor(r.e); advance(op); break;
	case 0xac: op_xor(r.h); advance(op); break;
	case 0xad: op_xor(r.l); advance(op); break;
	case 0xae: op_xor(mem->rb(r.hl)); advance(op); break;
	case 0xaf: op_xor(r.a); advance(op); break;
	case 0xb0: op_or(r.b); advance(op); break;
	case 0xb1: op_or(r.c); advance(op); break;
	case 0xb2: op_or(r.d); advance(op); break;
	case 0xb3: op_or(r.e); advance(op); break;
	case 0xb4: op_or(r.h); advance(op); break;
	case 0xb5: op_or(r.l); advance(op); break;
	case 0xb6: op_or(mem->rb(r.hl)); advance(op); break;
	case 0xb7: op_or(r.a); advance(op); break;
	case 0xb8: op_cp(r.b); advance(op); break;
	case 0xb9: op_cp(r.c); advance(op); break;
	case 0xba: op_cp(r.d); advance(op); break;
	case 0xbb: op_cp(r.e); advance(op); break;
	case 0xbc: op_cp(r.h); advance(op); break;
	case 0xbd: op_cp(r.l); advance(op); break;
	case 0xbe: op_cp(mem->rb(r.hl)); advance(op); break;
	case 0xbf: op_cp(r.a); advance(op); break;
	case 0xc0: op_ret(op, !(r.f & FZ)); break;
	case 0xc1: r.bc = op_pop(); advance(op); break;
	case 0xc2: op_jp(op, !(r.f & FZ)); break;
	case 0xc3: op_jp(op, true); break;
	case 0xc4: op_call(op, !(r.f & FZ)); add_cyc(op); break;
	case 0xc5: op_push(r.bc); advance(op); break;
	case 0xc6: op_add8(mem->rb(r.pc + 1)); advance(op); break;

	case 0xc8: op_ret(op, r.f & FZ); break;
	case 0xc9: op_ret(op, true); break;

	case 0xca: op_jp(op, (r.f & FZ)); break;

	case 0xcb:
	{
		u8 b1 = mem->rb(r.pc + 1);

		switch (b1)
		{
		case 0x00: r.b = op_rlc(r.b); advance(op); break;
		case 0x01: r.c = op_rlc(r.c); advance(op); break;
		case 0x02: r.d = op_rlc(r.d); advance(op); break;
		case 0x03: r.e = op_rlc(r.e); advance(op); break;
		case 0x04: r.h = op_rlc(r.h); advance(op); break;
		case 0x05: r.l = op_rlc(r.l); advance(op); break;
		case 0x06: mem->rb(op_rlc(mem->rb(r.hl))); advance(op); break;
		case 0x07: r.a = op_rlc(r.a); advance(op); break;
		case 0x08: r.b = op_rrc(r.b); advance(op); break;
		case 0x09: r.c = op_rrc(r.c); advance(op); break;
		case 0x0a: r.d = op_rrc(r.d); advance(op); break;
		case 0x0b: r.e = op_rrc(r.e); advance(op); break;
		case 0x0c: r.h = op_rrc(r.h); advance(op); break;
		case 0x0d: r.l = op_rrc(r.l); advance(op); break;
		case 0x0e: mem->rb(op_rrc(mem->rb(r.hl))); advance(op); break;
		case 0x0f: r.a = op_rrc(r.a); advance(op); break;
		case 0x10: r.b = op_rl(r.b); advance(op); break;
		case 0x11: r.c = op_rl(r.c); advance(op); break;
		case 0x12: r.d = op_rl(r.d); advance(op); break;
		case 0x13: r.e = op_rl(r.e); advance(op); break;
		case 0x14: r.h = op_rl(r.h); advance(op); break;
		case 0x15: r.l = op_rl(r.l); advance(op); break;
		case 0x16: mem->rb(op_rl(mem->rb(r.hl))); advance(op); break;
		case 0x17: r.a = op_rl(r.a); advance(op); break;
		case 0x18: r.b = op_rr(r.b); advance(op); break;
		case 0x19: r.c = op_rr(r.c); advance(op); break;
		case 0x1a: r.d = op_rr(r.d); advance(op); break;
		case 0x1b: r.e = op_rr(r.e); advance(op); break;
		case 0x1c: r.h = op_rr(r.h); advance(op); break;
		case 0x1d: r.l = op_rr(r.l); advance(op); break;
		case 0x1e: mem->rb(op_rr(mem->rb(r.hl))); advance(op); break;
		case 0x1f: r.a = op_rr(r.a); advance(op); break;
		case 0x20: r.b = op_sla(r.b); advance(op); break;
		case 0x21: r.c = op_sla(r.c); advance(op); break;
		case 0x22: r.d = op_sla(r.d); advance(op); break;
		case 0x23: r.e = op_sla(r.e); advance(op); break;
		case 0x24: r.h = op_sla(r.h); advance(op); break;
		case 0x25: r.l = op_sla(r.l); advance(op); break;
		case 0x26: mem->rb(op_sla(mem->rb(r.hl))); advance(op); break;
		case 0x27: r.a=op_sla(r.a); advance(op); break;
		case 0x28: r.b = op_sra(r.b); advance(op); break;
		case 0x29: r.c = op_sra(r.c); advance(op); break;
		case 0x2a: r.d = op_sra(r.d); advance(op); break;
		case 0x2b: r.e = op_sra(r.e); advance(op); break;
		case 0x2c: r.h = op_sra(r.h); advance(op); break;
		case 0x2d: r.l = op_sra(r.l); advance(op); break;
		case 0x2e: mem->rb(op_sra(mem->rb(r.hl))); advance(op); break;
		case 0x2f: r.a = op_sra(r.a); advance(op); break;
		case 0x30: r.b = op_sll(r.b); advance(op); break;
		case 0x31: r.c = op_sll(r.c); advance(op); break;
		case 0x32: r.d = op_sll(r.d); advance(op); break;
		case 0x33: r.e = op_sll(r.e); advance(op); break;
		case 0x34: r.h = op_sll(r.h); advance(op); break;
		case 0x35: r.l = op_sll(r.l); advance(op); break;
		case 0x36: mem->rb(op_sll(mem->rb(r.hl))); advance(op); break;
		case 0x37: r.a = op_sll(r.a); advance(op); break;
		case 0x38: r.b = op_srl(r.b); advance(op); break;
		case 0x39: r.c = op_srl(r.c); advance(op); break;
		case 0x3a: r.d = op_srl(r.d); advance(op); break;
		case 0x3b: r.e = op_srl(r.e); advance(op); break;
		case 0x3c: r.h = op_srl(r.h); advance(op); break;
		case 0x3d: r.l = op_srl(r.l); advance(op); break;
		case 0x3e: mem->rb(op_srl(mem->rb(r.hl))); advance(op); break;
		case 0x3f: r.a = op_srl(r.a); advance(op); break;
		case 0x40: op_bit(0, r.b); advance(op); break;
		case 0x41: op_bit(0, r.c); advance(op); break;
		case 0x42: op_bit(0, r.d); advance(op); break;
		case 0x43: op_bit(0, r.e); advance(op); break;
		case 0x44: op_bit(0, r.h); advance(op); break;
		case 0x45: op_bit(0, r.l); advance(op); break;
		case 0x46: op_bit(0, mem->rb(r.hl)); advance(op); break;
		case 0x47: op_bit(0, r.a); advance(op); break;
		case 0x48: op_bit(1, r.b); advance(op); break;
		case 0x49: op_bit(1, r.c); advance(op); break;
		case 0x4a: op_bit(1, r.d); advance(op); break;
		case 0x4b: op_bit(1, r.e); advance(op); break;
		case 0x4c: op_bit(1, r.h); advance(op); break;
		case 0x4d: op_bit(1, r.l); advance(op); break;
		case 0x4e: op_bit(1, mem->rb(r.hl)); advance(op); break;
		case 0x4f: op_bit(1, r.a); advance(op); break;
		case 0x50: op_bit(2, r.b); advance(op); break;
		case 0x51: op_bit(2, r.c); advance(op); break;
		case 0x52: op_bit(2, r.d); advance(op); break;
		case 0x53: op_bit(2, r.e); advance(op); break;
		case 0x54: op_bit(2, r.h); advance(op); break;
		case 0x55: op_bit(2, r.l); advance(op); break;
		case 0x56: op_bit(2, mem->rb(r.hl)); advance(op); break;
		case 0x57: op_bit(2, r.a); advance(op); break;
		case 0x58: op_bit(3, r.b); advance(op); break;
		case 0x59: op_bit(3, r.c); advance(op); break;
		case 0x5a: op_bit(3, r.d); advance(op); break;
		case 0x5b: op_bit(3, r.e); advance(op); break;
		case 0x5c: op_bit(3, r.h); advance(op); break;
		case 0x5d: op_bit(3, r.l); advance(op); break;
		case 0x5e: op_bit(3, mem->rb(r.hl)); advance(op); break;
		case 0x5f: op_bit(3, r.a); advance(op); break;
		case 0x60: op_bit(4, r.b); advance(op); break;
		case 0x61: op_bit(4, r.c); advance(op); break;
		case 0x62: op_bit(4, r.d); advance(op); break;
		case 0x63: op_bit(4, r.e); advance(op); break;
		case 0x64: op_bit(4, r.h); advance(op); break;
		case 0x65: op_bit(4, r.l); advance(op); break;
		case 0x66: op_bit(4, mem->rb(r.hl)); advance(op); break;
		case 0x67: op_bit(4, r.a); advance(op); break;
		case 0x68: op_bit(5, r.b); advance(op); break;
		case 0x69: op_bit(5, r.c); advance(op); break;
		case 0x6a: op_bit(5, r.d); advance(op); break;
		case 0x6b: op_bit(5, r.e); advance(op); break;
		case 0x6c: op_bit(5, r.h); advance(op); break;
		case 0x6d: op_bit(5, r.l); advance(op); break;
		case 0x6e: op_bit(5, mem->rb(r.hl)); advance(op); break;
		case 0x6f: op_bit(5, r.a); advance(op); break;
		case 0x70: op_bit(6, r.b); advance(op); break;
		case 0x71: op_bit(6, r.c); advance(op); break;
		case 0x72: op_bit(6, r.d); advance(op); break;
		case 0x73: op_bit(6, r.e); advance(op); break;
		case 0x74: op_bit(6, r.h); advance(op); break;
		case 0x75: op_bit(6, r.l); advance(op); break;
		case 0x76: op_bit(6, mem->rb(r.hl)); advance(op); break;
		case 0x77: op_bit(6, r.a); advance(op); break;
		case 0x78: op_bit(7, r.b); advance(op); break;
		case 0x79: op_bit(7, r.c); advance(op); break;
		case 0x7a: op_bit(7, r.d); advance(op); break;
		case 0x7b: op_bit(7, r.e); advance(op); break;
		case 0x7c: op_bit(7, r.h); advance(op); break;
		case 0x7d: op_bit(7, r.l); advance(op); break;
		case 0x7e: op_bit(7, mem->rb(r.hl)); advance(op); break;
		case 0x7f: op_bit(7, r.a); advance(op); break;
		case 0x80: r.b = op_res(0, r.b); advance(op); break;
		case 0x81: r.c = op_res(0, r.c); advance(op); break;
		case 0x82: r.d = op_res(0, r.d); advance(op); break;
		case 0x83: r.e = op_res(0, r.e); advance(op); break;
		case 0x84: r.h = op_res(0, r.h); advance(op); break;
		case 0x85: r.l = op_res(0, r.l); advance(op); break;
		case 0x86: mem->wb(r.hl, op_res(0, mem->rb(r.hl))); advance(op); break;
		case 0x87: r.a = op_res(0, r.a); advance(op); break;
		case 0x88: r.b = op_res(1, r.b); advance(op); break;
		case 0x89: r.c = op_res(1, r.c); advance(op); break;
		case 0x8a: r.d = op_res(1, r.d); advance(op); break;
		case 0x8b: r.e = op_res(1, r.e); advance(op); break;
		case 0x8c: r.h = op_res(1, r.h); advance(op); break;
		case 0x8d: r.l = op_res(1, r.l); advance(op); break;
		case 0x8e: mem->wb(r.hl, op_res(1, mem->rb(r.hl))); advance(op); break;
		case 0x8f: r.a = op_res(1, r.a); advance(op); break;
		case 0x90: r.b = op_res(2, r.b); advance(op); break;
		case 0x91: r.c = op_res(2, r.c); advance(op); break;
		case 0x92: r.d = op_res(2, r.d); advance(op); break;
		case 0x93: r.e = op_res(2, r.e); advance(op); break;
		case 0x94: r.h = op_res(2, r.h); advance(op); break;
		case 0x95: r.l = op_res(2, r.l); advance(op); break;
		case 0x96: mem->wb(r.hl, op_res(2, mem->rb(r.hl))); advance(op); break;
		case 0x97: r.a = op_res(2, r.a); advance(op); break;
		case 0x98: r.b = op_res(3, r.b); advance(op); break;
		case 0x99: r.c = op_res(3, r.c); advance(op); break;
		case 0x9a: r.d = op_res(3, r.d); advance(op); break;
		case 0x9b: r.e = op_res(3, r.e); advance(op); break;
		case 0x9c: r.h = op_res(3, r.h); advance(op); break;
		case 0x9d: r.l = op_res(3, r.l); advance(op); break;
		case 0x9e: mem->wb(r.hl, op_res(3, mem->rb(r.hl))); advance(op); break;
		case 0x9f: r.a = op_res(3, r.a); advance(op); break;
		case 0xa0: r.b = op_res(4, r.b); advance(op); break;
		case 0xa1: r.c = op_res(4, r.c); advance(op); break;
		case 0xa2: r.d = op_res(4, r.d); advance(op); break;
		case 0xa3: r.e = op_res(4, r.e); advance(op); break;
		case 0xa4: r.h = op_res(4, r.h); advance(op); break;
		case 0xa5: r.l = op_res(4, r.l); advance(op); break;
		case 0xa6: mem->wb(r.hl, op_res(4, mem->rb(r.hl))); advance(op); break;
		case 0xa7: r.a = op_res(4, r.a); advance(op); break;
		case 0xa8: r.b = op_res(5, r.b); advance(op); break;
		case 0xa9: r.c = op_res(5, r.c); advance(op); break;
		case 0xaa: r.d = op_res(5, r.d); advance(op); break;
		case 0xab: r.e = op_res(5, r.e); advance(op); break;
		case 0xac: r.h = op_res(5, r.h); advance(op); break;
		case 0xad: r.l = op_res(5, r.l); advance(op); break;
		case 0xae: mem->wb(r.hl, op_res(5, mem->rb(r.hl))); advance(op); break;
		case 0xaf: r.a = op_res(5, r.a); advance(op); break;
		case 0xb0: r.b = op_res(6, r.b); advance(op); break;
		case 0xb1: r.c = op_res(6, r.c); advance(op); break;
		case 0xb2: r.d = op_res(6, r.d); advance(op); break;
		case 0xb3: r.e = op_res(6, r.e); advance(op); break;
		case 0xb4: r.h = op_res(6, r.h); advance(op); break;
		case 0xb5: r.l = op_res(6, r.l); advance(op); break;
		case 0xb6: mem->wb(r.hl, op_res(6, mem->rb(r.hl))); advance(op); break;
		case 0xb7: r.a = op_res(6, r.a); advance(op); break;
		case 0xb8: r.b = op_res(7, r.b); advance(op); break;
		case 0xb9: r.c = op_res(7, r.c); advance(op); break;
		case 0xba: r.d = op_res(7, r.d); advance(op); break;
		case 0xbb: r.e = op_res(7, r.e); advance(op); break;
		case 0xbc: r.h = op_res(7, r.h); advance(op); break;
		case 0xbd: r.l = op_res(7, r.l); advance(op); break;
		case 0xbe: mem->wb(r.hl, op_res(7, mem->rb(r.hl))); advance(op); break;
		case 0xbf: r.a = op_res(7, r.a); advance(op); break;
		case 0xc0: r.b = op_set(0, r.b); advance(op); break;
		case 0xc1: r.c = op_set(0, r.c); advance(op); break;
		case 0xc2: r.d = op_set(0, r.d); advance(op); break;
		case 0xc3: r.e = op_set(0, r.e); advance(op); break;
		case 0xc4: r.h = op_set(0, r.h); advance(op); break;
		case 0xc5: r.l = op_set(0, r.l); advance(op); break;
		case 0xc6: mem->wb(r.hl, op_set(0, mem->rb(r.hl))); advance(op); break;
		case 0xc7: r.a = op_set(0, r.a); advance(op); break;
		case 0xc8: r.b = op_set(1, r.b); advance(op); break;
		case 0xc9: r.c = op_set(1, r.c); advance(op); break;
		case 0xca: r.d = op_set(1, r.d); advance(op); break;
		case 0xcb: r.e = op_set(1, r.e); advance(op); break;
		case 0xcc: r.h = op_set(1, r.h); advance(op); break;
		case 0xcd: r.l = op_set(1, r.l); advance(op); break;
		case 0xce: mem->wb(r.hl, op_set(1, mem->rb(r.hl))); advance(op); break;
		case 0xcf: r.a = op_set(1, r.a); advance(op); break;
		case 0xd0: r.b = op_set(2, r.b); advance(op); break;
		case 0xd1: r.c = op_set(2, r.c); advance(op); break;
		case 0xd2: r.d = op_set(2, r.d); advance(op); break;
		case 0xd3: r.e = op_set(2, r.e); advance(op); break;
		case 0xd4: r.h = op_set(2, r.h); advance(op); break;
		case 0xd5: r.l = op_set(2, r.l); advance(op); break;
		case 0xd6: mem->wb(r.hl, op_set(2, mem->rb(r.hl))); advance(op); break;
		case 0xd7: r.a = op_set(2, r.a); advance(op); break;
		case 0xd8: r.b = op_set(3, r.b); advance(op); break;
		case 0xd9: r.c = op_set(3, r.c); advance(op); break;
		case 0xda: r.d = op_set(3, r.d); advance(op); break;
		case 0xdb: r.e = op_set(3, r.e); advance(op); break;
		case 0xdc: r.h = op_set(3, r.h); advance(op); break;
		case 0xdd: r.l = op_set(3, r.l); advance(op); break;
		case 0xde: mem->wb(r.hl, op_set(3, mem->rb(r.hl))); advance(op); break;
		case 0xdf: r.a = op_set(3, r.a); advance(op); break;
		case 0xe0: r.b = op_set(4, r.b); advance(op); break;
		case 0xe1: r.c = op_set(4, r.c); advance(op); break;
		case 0xe2: r.d = op_set(4, r.d); advance(op); break;
		case 0xe3: r.e = op_set(4, r.e); advance(op); break;
		case 0xe4: r.h = op_set(4, r.h); advance(op); break;
		case 0xe5: r.l = op_set(4, r.l); advance(op); break;
		case 0xe6: mem->wb(r.hl, op_set(4, mem->rb(r.hl))); advance(op); break;
		case 0xe7: r.a = op_set(4, r.a); advance(op); break;
		case 0xe8: r.b = op_set(5, r.b); advance(op); break;
		case 0xe9: r.c = op_set(5, r.c); advance(op); break;
		case 0xea: r.d = op_set(5, r.d); advance(op); break;
		case 0xeb: r.e = op_set(5, r.e); advance(op); break;
		case 0xec: r.h = op_set(5, r.h); advance(op); break;
		case 0xed: r.l = op_set(5, r.l); advance(op); break;
		case 0xee: mem->wb(r.hl, op_set(5, mem->rb(r.hl))); advance(op); break;
		case 0xef: r.a = op_set(5, r.a); advance(op); break;
		case 0xf0: r.b = op_set(6, r.b); advance(op); break;
		case 0xf1: r.c = op_set(6, r.c); advance(op); break;
		case 0xf2: r.d = op_set(6, r.d); advance(op); break;
		case 0xf3: r.e = op_set(6, r.e); advance(op); break;
		case 0xf4: r.h = op_set(6, r.h); advance(op); break;
		case 0xf5: r.l = op_set(6, r.l); advance(op); break;
		case 0xf6: mem->wb(r.hl, op_set(6, mem->rb(r.hl))); advance(op); break;
		case 0xf7: r.a = op_set(6, r.a); advance(op); break;
		case 0xf8: r.b = op_set(7, r.b); advance(op); break;
		case 0xf9: r.c = op_set(7, r.c); advance(op); break;
		case 0xfa: r.d = op_set(7, r.d); advance(op); break;
		case 0xfb: r.e = op_set(7, r.e); advance(op); break;
		case 0xfc: r.h = op_set(7, r.h); advance(op); break;
		case 0xfd: r.l = op_set(7, r.l); advance(op); break;
		case 0xfe: mem->wb(r.hl, op_set(7, mem->rb(r.hl))); advance(op); break;
		case 0xff: r.a = op_set(7, r.a); advance(op); break;
		default:
		printf("%04X %02X %02X\n", r.pc, op, b1);
		state = cstate::debugging;
		}
	}
	break;

	case 0xcc: op_call(op, (r.f & FZ)); add_cyc(op); break;
	case 0xcd: op_call(op, true); add_cyc(op); break;
	case 0xce: op_adc8(mem->rb(r.pc + 1)); advance(op); break;
	case 0xcf: op_rst(0x08); add_cyc(op); break;
	case 0xd0: op_ret(op, !(r.f & FC)); break;
	case 0xd1: r.de = op_pop(); advance(op); break;
	case 0xd2: op_jp(op, !(r.f & FC)); break;
	case 0xd3: mem->set_port(0, r.a); advance(op); break;
	case 0xd4: op_call(op, !(r.f & FC)); add_cyc(op); break;
	case 0xd5: op_push(r.de); advance(op); break;
	case 0xd6: op_sub8(mem->rb(r.pc + 1)); advance(op); break;
	case 0xd7: op_rst(0x10); add_cyc(op); break;
	case 0xd8: op_ret(op, r.f & FC); break;
	case 0xd9: op_exx(); advance(op); break;
	case 0xda: op_jp(op, (r.f & FC)); break;
	case 0xdb: r.a = mem->get_port(0); advance(op); break;
	case 0xdc: op_call(op, (r.f & FC)); add_cyc(op); break;
	case 0xdd:
	{
		u8 b1 = mem->rb(r.pc + 1);
		u16 a = 0;

		switch (b1)
		{
		case 0x09: r.ix = op_add(r.ix, r.bc); advance(op); break;

		case 0x19: r.ix = op_add(r.ix, r.de); advance(op); break;

		case 0x21: r.ix = mem->rw(r.pc + 2); advance(op); break;
		case 0x22: mem->ww(mem->rw(r.pc + 2), r.ix); advance(op); break;
		case 0x23: r.ix++; advance(op); break;
		case 0x24: r.ixh = op_inc8(r.ixh); advance(op); break;
		case 0x25: r.ixh = op_dec8(r.ixh); advance(op); break;
		case 0x26: r.ixh = mem->rb(r.pc + 2); advance(op); break;

		case 0x29: r.ix = op_add(r.ix, r.ix); advance(op); break;
		case 0x2a: r.ix = mem->rw(mem->rw(r.pc + 2)); advance(op); break;
		case 0x2b: r.ix--; advance(op); break;
		case 0x2c: r.ixl = op_inc8(r.ixl); advance(op); break;
		case 0x2d: r.ixl = op_dec8(r.ixl); advance(op); break;
		case 0x2e: r.ixl = mem->rb(r.pc + 2); advance(op); break;

		case 0x34: op_inchl(r.ix, r.pc + 2); advance(op); break;
		case 0x35: op_dechl(r.ix, r.pc + 2); advance(op); break;
		case 0x36: a = get_addr(r.ix, r.pc + 2); mem->wb(a, mem->rb(r.pc + 3)); advance(op); break;

		case 0x39: r.ix = op_add(r.ix, r.sp); advance(op); break;

		case 0x46: r.b = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x4e: r.c = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x56: r.d = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x5e: r.e = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x66: r.h = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x6e: r.l = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x70: mem->wb(r.ix + mem->rb(r.pc + 2), r.b); advance(op); break;
		case 0x71: mem->wb(r.ix + mem->rb(r.pc + 2), r.c); advance(op); break;
		case 0x72: mem->wb(r.ix + mem->rb(r.pc + 2), r.d); advance(op); break;
		case 0x73: mem->wb(r.ix + mem->rb(r.pc + 2), r.e); advance(op); break;
		case 0x74: mem->wb(r.ix + mem->rb(r.pc + 2), r.h); advance(op); break;
		case 0x75: mem->wb(r.ix + mem->rb(r.pc + 2), r.l); advance(op); break;

		case 0x77: mem->wb(r.ix + mem->rb(r.pc + 2), r.a); advance(op); break;

		case 0x7e: r.a = mem->rb(r.ix + mem->rb(r.pc + 2)); advance(op); break;

		case 0x84: op_add8(r.ixh); advance(op); break;
		case 0x85: op_add8(r.ixl); advance(op); break;
		case 0x86: op_add8(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0x8c: op_adc8(r.ixh); advance(op); break;
		case 0x8d: op_adc8(r.ixl); advance(op); break;
		case 0x8e: op_adc8(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0x94: op_sub8(r.ixh); advance(op); break;
		case 0x95: op_sub8(r.ixl); advance(op); break;
		case 0x96: op_sub8(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0x9c: op_sbc8(r.ixh); advance(op); break;
		case 0x9d: op_sbc8(r.ixl); advance(op); break;
		case 0x9e: op_sbc8(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0xa4: op_and(r.ixh); advance(op); break;
		case 0xa5: op_and(r.ixl); advance(op); break;
		case 0xa6: op_and(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0xac: op_xor(r.ixh); advance(op); break;
		case 0xad: op_xor(r.ixl); advance(op); break;
		case 0xae: op_xor(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0xb4: op_or(r.ixh); advance(op); break;
		case 0xb5: op_or(r.ixl); advance(op); break;
		case 0xb6: op_or(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0xbc: op_cp(r.ixh); advance(op); break;
		case 0xbd: op_cp(r.ixl); advance(op); break;
		case 0xbe: op_cp(mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

		case 0xcb:
		{
			u8 b3 = mem->rb(r.pc + 3);

			switch (b3)
			{

			case 0x06: rot_wb_ix(op_rlc(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x0e: rot_wb_ix(op_rrc(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x16: rot_wb_ix(op_rl(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x1e: rot_wb_ix(op_rr(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x26: rot_wb_ix(op_sla(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x2e: rot_wb_ix(op_sra(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x36: rot_wb_ix(op_sll(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x3e: rot_wb_ix(op_srl(mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x46: op_bit(0, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x4e: op_bit(1, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x56: op_bit(2, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x5e: op_bit(3, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x66: op_bit(4, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x6e: op_bit(5, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x76: op_bit(6, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x7e: op_bit(7, mem->rb(r.ix + mem->rb(r.pc + 2))); advance(op); break;

			case 0x86: rot_wb_ix(op_res(0, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x8e: rot_wb_ix(op_res(1, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x96: rot_wb_ix(op_res(2, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x9e: rot_wb_ix(op_res(3, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xa6: rot_wb_ix(op_res(4, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xae: rot_wb_ix(op_res(5, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xb6: rot_wb_ix(op_res(6, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xbe: rot_wb_ix(op_res(7, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xc6: rot_wb_ix(op_set(0, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xce: rot_wb_ix(op_set(1, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xd6: rot_wb_ix(op_set(2, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xde: rot_wb_ix(op_set(3, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xe6: rot_wb_ix(op_set(4, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xee: rot_wb_ix(op_set(5, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xf6: rot_wb_ix(op_set(6, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xfe: rot_wb_ix(op_set(7, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			default:
			printf("%04X %02X %02X %02X\n", r.pc, op, b1, b3);
			state = cstate::debugging;
			}
		}
		break;

		case 0xe1:r.ix = op_pop(); advance(op); break;

		case 0xe5: op_push(r.ix); advance(op); break;

		default:
		printf("%04X %02X %02X\n", r.pc, op, b1);
		state = cstate::debugging;
		}
		break;
	}

	case 0xde: op_sbc8(mem->rb(r.pc + 1)); advance(op); break;
	case 0xdf: op_rst(0x18); add_cyc(op); break;

	case 0xe1: r.hl = op_pop(); advance(op); break;

	case 0xe5: op_push(r.hl); advance(op); break;
	case 0xe6: op_and(mem->rb(r.pc + 1)); advance(op); break;
	case 0xe7: op_rst(0x20); add_cyc(op); break;

	case 0xe9: r.pc = r.hl; add_cyc(op); break;
	case 0xea: op_jp(op, (r.f & FP)); break;
	case 0xeb: op_ex(&r.de, &r.hl); advance(op); break;

	case 0xed:
	{
		u8 b1 = mem->rb(r.pc + 1);

		switch (b1)
		{
		case 0x42: r.hl = op_sbc(r.hl, r.bc); advance(op); break;
		case 0x43: mem->ww(mem->rw(r.pc + 2), r.bc); advance(op); break;
		case 0x44: op_neg(); advance(op); break;

		case 0x47: r.i = r.a; advance(op); break;

		case 0x4a: r.hl = op_adc(r.hl, r.bc); advance(op); break;
		case 0x4b: r.bc = mem->rw(mem->rw(r.pc + 2)); advance(op); break;

		case 0x52: r.hl = op_sbc(r.hl, r.de); advance(op); break;
		case 0x53: mem->ww(mem->rw(r.pc + 2), r.de); advance(op); break;

		case 0x5a: r.hl = op_adc(r.hl, r.de); advance(op); break;
		case 0x5b: r.de = mem->rw(mem->rw(r.pc + 2)); advance(op); break;

		case 0x5e: im = 2; advance(op); break;

		case 0x62: r.hl = op_sbc(r.hl, r.hl); advance(op); break;

		case 0x67: op_rrd(); advance(op); break;

		case 0x6f: op_rld(); advance(op); break;

		case 0x6a: r.hl = op_adc(r.hl, r.hl); advance(op); break;

		case 0x72: r.hl = op_sbc(r.hl, r.sp); advance(op); break;
		case 0x73: mem->ww(mem->rw(r.pc + 2), r.sp); advance(op); break;

		case 0x7a: r.hl = op_adc(r.hl, r.sp); advance(op); break;
		case 0x7b: r.sp = mem->rw(mem->rw(r.pc + 2)); advance(op); break;

		case 0xa0: op_ldi(); advance(op); break;
		case 0xa1: op_cpi(); advance(op); break;

		case 0xa8: op_ldd(); advance(op); break;
		case 0xa9: op_cpd(); advance(op); break;

		case 0xb0: op_ldir(op); break;
		case 0xb1: op_cpir(op); break;

		case 0xb8: op_lddr(op); break;
		case 0xb9: op_cpdr(op); break;

		default:
		printf("%04X %02X %02X\n", r.pc, op, b1);
		state = cstate::debugging;
		}
	}
	break;

	case 0xee: op_xor(mem->rb(r.pc + 1)); advance(op); break;
	case 0xef: op_rst(0x28); add_cyc(op); break;

	case 0xf1: r.af = op_pop(); advance(op); break;

	case 0xf3: iff1 = false; advance(op); break;

	case 0xf5: op_push(r.af); advance(op); break;
	case 0xf6: op_or(mem->rb(r.pc + 1)); advance(op); break;
	case 0xf7: op_rst(0x30); add_cyc(op); break;
	case 0xf8: op_ret(op, r.f & FS); add_cyc(op); break;
	case 0xf9: r.sp = r.hl; advance(op); break;
	case 0xfa: op_jp(op, (r.f & FS)); break;
	case 0xfb: iff1 = true; advance(op); break;

	case 0xfd:
	{
		u8 b1 = mem->rb(r.pc + 1);

		switch (b1)
		{
		case 0x09: r.iy = op_add(r.iy, r.bc); advance(op); break;

		case 0x19: r.iy = op_add(r.iy, r.de); advance(op); break;

		case 0x21: r.iy = mem->rw(r.pc + 2); advance(op); break;
		case 0x22: mem->ww(mem->rw(r.pc + 2), r.iy); advance(op); break;
		case 0x23: r.iy++; advance(op); break;

		case 0x26: r.iyh = mem->rb(r.pc + 2); advance(op); break;

		case 0x29: r.iy = op_add(r.iy, r.iy); advance(op); break;
		case 0x2a: r.iy = mem->rw(mem->rw(r.pc + 2)); advance(op); break;
		case 0x2b: r.iy--; advance(op); break;

		case 0x2e: r.iyl = mem->rb(r.pc + 2); advance(op); break;

		case 0x34: op_inchl(r.iy, r.pc + 2); advance(op); break;
		case 0x35: op_dechl(r.ix, r.pc + 2); advance(op); break;
		case 0x36: mem->wb(r.iy + mem->rb(r.pc + 2), mem->rb(r.pc + 3)); advance(op); break;

		case 0x39: r.iy = op_add(r.iy, r.sp); advance(op); break;

		case 0x46: r.b = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x4e: r.c = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x56: r.d = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x5e: r.e = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x66: r.h = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x6e: r.l = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x70: mem->wb(r.iy + mem->rb(r.pc + 2), r.b); advance(op); break;
		case 0x71: mem->wb(r.iy + mem->rb(r.pc + 2), r.c); advance(op); break;
		case 0x72: mem->wb(r.iy + mem->rb(r.pc + 2), r.d); advance(op); break;
		case 0x73: mem->wb(r.iy + mem->rb(r.pc + 2), r.e); advance(op); break;
		case 0x74: mem->wb(r.iy + mem->rb(r.pc + 2), r.h); advance(op); break;
		case 0x75: mem->wb(r.iy + mem->rb(r.pc + 2), r.l); advance(op); break;
		//case 0x76: mem->wb(r.iy + mem->rb(r.pc + 2), r.b); advance(op); break;
		case 0x77: mem->wb(r.iy + mem->rb(r.pc + 2), r.a); advance(op); break;

		case 0x7e: r.a = mem->rb(r.iy + mem->rb(r.pc + 2)); advance(op); break;

		case 0x84: op_add8(r.iyh); advance(op); break;
		case 0x85: op_add8(r.iyl); advance(op); break;
		case 0x86: op_add8(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;


		case 0x8c: op_adc8(r.iyh); advance(op); break;
		case 0x8d: op_adc8(r.iyl); advance(op); break;
		case 0x8e: op_adc8(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0x94: op_sub8(r.iyh); advance(op); break;
		case 0x95: op_sub8(r.iyl); advance(op); break;
		case 0x96: op_sub8(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0x9c: op_sbc8(r.iyh); advance(op); break;
		case 0x9d: op_sbc8(r.iyl); advance(op); break;
		case 0x9e: op_sbc8(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0xa4: op_and(r.iyh); advance(op); break;
		case 0xa5: op_and(r.iyl); advance(op); break;
		case 0xa6: op_and(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0xac: op_xor(r.iyh); advance(op); break;
		case 0xad: op_xor(r.iyl); advance(op); break;
		case 0xae: op_xor(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0xb4: op_or(r.iyh); advance(op); break;
		case 0xb5: op_or(r.iyl); advance(op); break;
		case 0xb6: op_or(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0xbc: op_cp(r.iyh); advance(op); break;
		case 0xbd: op_cp(r.iyl); advance(op); break;
		case 0xbe: op_cp(mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

		case 0xcb:
		{
			u8 b3 = mem->rb(r.pc + 3);

			switch (b3)
			{
			case 0x06: rot_wb_iy(op_rlc(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x0e: rot_wb_iy(op_rrc(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x16: rot_wb_iy(op_rl(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x1e: rot_wb_iy(op_rr(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x26: rot_wb_iy(op_sla(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x2e: rot_wb_iy(op_sra(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x36: rot_wb_iy(op_sll(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x3e: rot_wb_iy(op_srl(mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x46: op_bit(0, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x4e: op_bit(1, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x56: op_bit(2, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x5e: op_bit(3, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x66: op_bit(4, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x6e: op_bit(5, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x76: op_bit(6, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x7e: op_bit(7, mem->rb(r.iy + mem->rb(r.pc + 2))); advance(op); break;

			case 0x86: rot_wb_iy(op_res(0, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x8e: rot_wb_iy(op_res(1, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x96: rot_wb_iy(op_res(2, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0x9e: rot_wb_iy(op_res(3, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xa6: rot_wb_iy(op_res(4, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xae: rot_wb_iy(op_res(5, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xb6: rot_wb_iy(op_res(6, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xbe: rot_wb_iy(op_res(7, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xc6: rot_wb_iy(op_set(0, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xce: rot_wb_iy(op_set(1, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xd6: rot_wb_iy(op_set(2, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xde: rot_wb_iy(op_set(3, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xe6: rot_wb_iy(op_set(4, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xee: rot_wb_iy(op_set(5, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xf6: rot_wb_iy(op_set(6, mem->rb(r.iy + mem->rb(r.pc + 2)))); advance(op); break;

			case 0xfe: rot_wb_iy(op_set(7, mem->rb(r.ix + mem->rb(r.pc + 2)))); advance(op); break;

			default:
			printf("%04X %02X %02X %02X\n", r.pc, op, b1, b3);
			state = cstate::debugging;
			}
		}
		break;

		case 0xe1: r.iy = op_pop(); add_cyc(op);  add_pc(op); break;

		case 0xe5: op_push(r.iy); advance(op); break;

		default:
		printf("%04X %02X %02X\n", r.pc, op, b1);
		state = cstate::debugging;
		}
	}
	break;

	case 0xfe: op_cp(mem->rb(r.pc + 1)); advance(op); break;
	case 0xff: op_rst(0x38); add_cyc(op); break;

	default:
	printf("%04X %02X\n", r.pc, op);
	state = cstate::debugging;
	break;
	}

	if (halt)
		op_halt();
	else if (cycles > CYCLES_PER_FRAME)
		handle_interrupts();
}

void Cpu::op_adc8(u8 r1)
{
	u8 cf = (r.f & 1);
	int v = r.a + r1 + cf;

	set_flag(v > 0xff, FC);
	set_flag(0, FN);
	set_flag((~(r.a ^ r1) & (r.a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((r.a & 0xf) + (r1 & 0xf) + cf) & FH, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

u16 Cpu::op_adc(u16 r1, u16 r2)
{
	int c = (r.f & 1);
	int v = r1 + r2 + c;

	set_flag(v > 0xffff, FC);
	set_flag(0, FN);
	set_flag((~(r1 ^ r2) & (r1 ^ v) & 0x8000), FP);
	set_flag((v >> 11) & 1, FX);
	set_flag((((r1 & 0xfff) + (r2 & 0xfff) + c)) & 0x1000, FH);
	set_flag((v >> 13) & 1, FY);
	set_flag((v & 0xffff) == 0, FZ);
	set_flag(v & 0x8000, FS);

	return v;
}

void Cpu::op_add8(u8 r1)
{
	u8 o = r1;
	int v = r.a + r1;

	set_flag(v > 0xff, FC);
	set_flag(0, FN);
	set_flag((~(r.a ^ r1) & (r.a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((r.a & 0xf) + (r1 & 0xf)) & FH, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

u16 Cpu::op_add(u16 r1, u16 r2)
{
	u8 f = 0;
	u16 o = r1;
	int v = o + r2;

	set_flag(0, FN);
	set_flag(v > 0xffff, FC);
	set_flag((((r1 & 0xfff) + (r2 & 0xfff))) & 0x1000, FH);
	set_flag((v >> 11) & 1, FX);
	set_flag((v >> 13) & 1, FY);

	return v;
}

void Cpu::op_and(u8 r1)
{
	int v = r.a & r1;

	set_flag(0, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(1, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

void Cpu::op_bit(u8 r1, u8 r2)
{
	int o = r2;
	int v = o & (1 << r1);

	set_flag(0, FN);
	set_flag(fpar[v], FH);
	set_flag(v & FX, FX);
	set_flag(1, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);
}

void Cpu::op_call(u8 op, bool flag)
{
	if (flag)
	{
		op_push(r.pc + 3);
		r.pc = mem->rw(r.pc + 1);
		cycles += disasm_00[op].cycles;
	}
	else
	{
		r.pc += 3;
		cycles += disasm_00[op].cycles2;
	}
}

void Cpu::op_ccf()
{

	u8 c = r.f & FC;
	set_flag(c, FH);
	set_flag(!(r.f & FC), FC);
	set_flag(0, FN);
	set_flag(r.a & FX, FX);
	set_flag(r.a & FY, FY);
}

void Cpu::op_cp(u8 r1)
{
	int v = r.a - r1;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((r.a ^ v) & (r.a ^ r1) & 0x80), FP);
	set_flag((r1 >> 3) & 1, FX);
	set_flag(((r.a & 0xf) - (r1 & 0xf) & 0x10), FH);
	set_flag((r1 >> 5) & 1, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);
}

void Cpu::op_cpd()
{
	u8 hl = mem->rb(r.hl);
	int v = r.a - hl;

	set_flag(1, FN);
	set_flag(r.bc - 1, FP);
	set_flag(((r.a & 0xf) - (hl & 0xf)) & FH, FH);
	u8 xy = v - (r.f & FH ? 1 : 0);
	set_flag(xy & FX, FX);
	set_flag(xy & FN, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.bc--;
	r.hl--;
}

void Cpu::op_cpdr(u8 op)
{
	u8 hl = mem->rb(r.hl);
	int v = r.a - hl;

	r.bc--;
	r.hl--;

	set_flag(1, FN);
	set_flag(r.bc, FP);
	set_flag(((r.a & 0xf) - (hl & 0xf)) & FH, FH);
	u8 xy = v - (r.f & FH ? 1 : 0);
	set_flag(xy & FX, FX);
	set_flag(xy & FN, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	if ((s16)r.bc == 0 || r.f & FZ)
	{
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles2;
		add_pc(op);
	}
	else
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles;
}

void Cpu::op_cpi()
{
	u8 hl = mem->rb(r.hl);
	int v = r.a - hl;

	set_flag(1, FN);
	set_flag(r.bc - 1, FP);
	set_flag(((r.a & 0xf) - (hl & 0xf)) & FH, FH);
	u8 xy = v - (r.f & FH ? 1 : 0);
	set_flag(xy & FX, FX);
	set_flag(xy & FN, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.bc--;
	r.hl++;
}

void Cpu::op_cpir(u8 op)
{
	u8 hl = mem->rb(r.hl);
	int v = r.a - hl;
	r.bc--;
	r.hl++;

	set_flag(1, FN);
	set_flag(r.bc, FP);
	set_flag(((r.a & 0xf) - (hl & 0xf)) & FH, FH);
	u8 xy = v - (r.f & FH ? 1 : 0);
	set_flag(xy & FX, FX);
	set_flag(xy & FN, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	if ((s16)r.bc == 0 || r.f & FZ)
	{
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles2;
		add_pc(op);
	}
}

void Cpu::op_cpl()
{
	u8 r1 = ~r.a;

	set_flag(1, FN);
	set_flag((r1 >> 3) & 1, FX);
	set_flag(1, FH);
	set_flag((r1 >> 5) & 1, FY);

	r.a = r1;
}

void Cpu::op_daa()
{
	int v = r.a;

	if (r.f & FN)
	{
		if (r.f & FH || (r.a & 0xf) > 9)
			v -= 6;
		if (r.f & FC || (r.a > 0x99))
			v -= 0x60;

		//set_flag(((r.a & 0xf) - (v & 0xf)) & FH, FH);
	}
	else
	{
		if ((r.f & FH) || (r.a & 0xf) > 9)
			v += 6;
		if ((r.f & FC) || (r.a > 0x99))
			v += 0x60;

		//set_flag(((r.a & 0xf) + (v & 0xf)) & FH, FH);
	}

	set_flag(r.f & FC || r.a > 0x99, FC);
	set_flag(get_parity(v), FP);
	set_flag(((r.a & 0x10) ^ (v & 0x10)) & FH, FH);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);
	set_flag(v & FX, FX);
	set_flag(v & FY, FY);

	r.a = v;
}

u8 Cpu::op_dec8(u8 r1)
{
	u8 o = r1;
	int v = r1 - 1;

	set_flag(1, FN);
	set_flag(o == 0x80, FP);
	set_flag(v & FX, FX);
	set_flag((v & 0x0f) == 0x0f, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	return v;
}

void Cpu::op_dechl(u16 r1, u16 r2)
{
	u16 a = get_addr(r1, r2);
	mem->wb(a, op_dec8(mem->rb(a)));
}

void Cpu::op_djnz(u8 r1)
{
	if (--r.b > 0)
	{
		r.pc += (s8)mem->rb(r.pc + 1) + 2;
		cycles += disasm_00[mem->rb(r.pc)].cycles;
	}
	else
	{
		r.pc += 2;
		cycles += disasm_00[mem->rb(r.pc)].cycles2;
	}

}

void Cpu::op_ex(u16* op, u16* op2)
{
	exchange_values(op, op2);
}

void Cpu::op_exx()
{
	exchange_values(&r.bc, &r.sbc);
	exchange_values(&r.de, &r.sde);
	exchange_values(&r.hl, &r.shl);
}

void Cpu::op_halt()
{
	while (halt)
	{
		if (cycles < CYCLES_PER_FRAME)
		{
			cycles += 4;
		}
		else
		{
			u16 intaddr = r.i << 8 | mem->get_port(0);
			r.sp -= 2;
			r.pc = mem->rw(intaddr);
			halt = false;
			inte = false;
		}
	}
}

u8 Cpu::op_inc8(u8 r1)
{
	u8 o = r1;
	int v = r1 + 1;

	set_flag(0, FN);
	set_flag(o == 0x7f, FP);
	set_flag(v & FX, FX);
	set_flag((o & 0xf) == 0xf, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	return v;
}

void Cpu::op_inchl(u16 r1, u16 r2)
{
	u16 a = get_addr(r1, r2);
	mem->wb(a, op_inc8(mem->rb(a)));
}

void Cpu::op_jp(u8 op, bool flag)
{
	if (flag)
	{
		r.pc = mem->rw(r.pc + 1);
		cycles += disasm_00[op].cycles;
	}
	else
	{
		r.pc += 3;
		cycles += disasm_00[op].cycles2;
	}
}

void Cpu::op_jr(u8 op, bool flag)
{
	if (flag)
	{
		r.pc += (s8)mem->rb(r.pc + 1) + 2;
		cycles += disasm_00[op].cycles;
	}
	else
	{
		r.pc += 2;
		cycles += disasm_00[op].cycles2;
	}
}

void Cpu::op_ldd()
{
	mem->wb(r.de, mem->rb(r.hl));

	set_flag(0, FN);
	set_flag(r.bc - 1, FP);
	set_flag(0, FH);
	u8 xy = mem->rb(r.hl);
	set_flag((xy + r.a) & FX, FX);
	set_flag((xy + r.a) & FN, FY);

	r.bc--;
	r.hl--;
	r.de--;
}

void Cpu::op_lddr(u8 op)
{
	op_ldd();

	if ((s16)r.bc == 0)
	{
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles2;
		add_pc(op);
	}
	else
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles;
}

void Cpu::op_ldi()
{
	mem->wb(r.de, mem->rb(r.hl));

	set_flag(0, FN);
	set_flag(r.bc - 1, FP);
	set_flag(0, FH);
	u8 xy = mem->rb(r.hl);
	set_flag((xy + r.a) & FX, FX);
	set_flag((xy + r.a) & FN, FY);

	r.bc--;
	r.hl++;
	r.de++;
}

void Cpu::op_ldir(u8 op)
{
	op_ldi();

	if ((s16)r.bc == 0)
	{
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles2;
		add_pc(op);
	}
	else
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles;
}

void Cpu::op_neg()
{
	u8 v = 0 - r.a;

	set_flag(r.a != 0, FC);
	set_flag(1, FN);
	set_flag(r.a == 0x80, FP);
	set_flag(v & FX, FX);
	set_flag(((r.a & 0xf) + (v & 0xf) & 0x10), FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

void Cpu::op_or(u8 r1)
{
	int v = r.a | r1;

	set_flag(0, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

u8 Cpu::op_pop8()
{
	u8 l;
	l = mem->rb(r.sp++);

	return l;
}

u16 Cpu::op_pop()
{
	u8 h, l;
	l = mem->rb(r.sp++);
	h = mem->rb(r.sp++);

	return h << 8 | l;
}

void Cpu::op_push8(u8 r1)
{
	mem->wb(--r.sp, r1);
}

void Cpu::op_push(u16 r1)
{
	mem->wb(--r.sp, r1 >> 8);
	mem->wb(--r.sp, r1 & 0xff);
}

void Cpu::op_ret(u8 op, bool flag)
{
	if (flag)
	{
		r.pc = op_pop();
		cycles += disasm_00[op].cycles;
	}
	else
	{
		r.pc++;
		cycles += disasm_00[op].cycles2;
	}


}

u8 Cpu::op_res(u8 r1, u8 r2)
{
	return r2 & ~(1 << r1);
}

u8 Cpu::op_rl(u8 r1)
{
	int v = r1;
	u8 oc = v >> 7;
	u8 c = r.f & 1;
	v <<= 1;

	set_flag(oc, FC);
	set_flag(0, FN);
	set_flag(get_parity(v | c), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	return v | c;
}

void Cpu::op_rla8()
{
	u16 v = r.a << 1;
	u8 oc = r.f & 1;
	u8 c = v >> 8;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	r.a = v | oc;
}

u8 Cpu::op_rlc(u8 r1)
{
	u8 c;
	int v = r1;
	c = v >> 7;
	v = v << 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v | c), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	return v | c;
}

void Cpu::op_rlca()
{
	u16 v = r.a << 1;
	u8 c = v >> 8;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	r.a = v | c;
}

void Cpu::op_rld()
{
	u8 a = r.a;
	u8 v = mem->rb(r.hl);

	mem->wb(r.hl, (v << 4) + ((r.a & 0xf)));
	r.a = (r.a & 0xf0) + (v >> 4);

	set_flag(0, FN);
	set_flag(get_parity(r.a), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(r.a == 0, FZ);
	set_flag(r.a & 0x80, FS);
}

u8 Cpu::op_rr(u8 r1)
{
	u8 oc = r.f & 1;
	u8 c = r1 & 1;
	int v = r1;
	v = (v >> 1) | (oc << 7);

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	return v;
}

u8 Cpu::op_rrc(u8 r1)
{
	int v = r1;
	u8 c = v & 1;
	v = (v >> 1) | (c << 7);

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	return v;
}

void Cpu::op_rrca()
{
	u8 c = r.a & 1;
	u8 v = r.a = r.a >> 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	r.a |= c << 7;
}

void Cpu::op_rra()
{
	u8 oc = r.f & 1;
	u8 c = r.a & 1;
	u8 v = r.a >> 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	r.a = v | (oc << 7);
}

void Cpu::op_rrd()
{
	u8 a = r.a;
	u8 v = mem->rb(r.hl);

	mem->wb(r.hl, (v >> 4) + ((r.a & 0xf) << 4));
	r.a = (r.a & 0xf0) + (v & 0xf);

	//set_flag(a != 0, FC);
	set_flag(0, FN);
	set_flag(get_parity(r.a), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(r.a == 0, FZ);
	set_flag(r.a & 0x80, FS);
}

void Cpu::op_rst(u8 r1)
{
	op_push(r.pc + 1);
	r.pc = r1;
}

u8 Cpu::op_set(u8 r1, u8 r2)
{
	return r2 | (1 << r1);
}

void Cpu::op_sbc8(u8 r1)
{
	u8 cf = r.f & 1;
	int v = r.a - r1 - cf;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((r.a ^ r1) & (r.a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((r.a & 0xf) - (r1 & 0xf) - cf) & FH, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

u16 Cpu::op_sbc(u16 r1, u16 r2)
{
	int v = r1 - r2 - (r.f & 1);

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((r1 ^ r2) & (r1 ^ v) & 0x8000), FP);
	set_flag((v >> 11) & 1, FX);
	set_flag((((r1 & 0x1000) & (r2 & 0x1000))), FH);
	set_flag((v >> 13) & 1, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x8000, FS);

	return v;
}

void Cpu::op_scf()
{
	set_flag(1, FC);
	set_flag(0, FN);
	set_flag(r.a & FX, FX);
	set_flag(0, FH);
	set_flag(r.a & FY, FY);
}

u8 Cpu::op_sla(u8 r1)
{
	int v = r1;
	u8 c = v >> 7;
	v <<= 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	return v;
}

u8 Cpu::op_sra(u8 r1)
{
	int v = r1;
	u8 c = v & 1;
	v = (v >> 1) | (v & 0x80);

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(r1 & 0x80, FS);

	return v;
}

u8 Cpu::op_sll(u8 r1)
{
	u8 c;
	int v = r1;
	c = v >> 7;
	v = (v << 1) + 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	return v;
}

u8 Cpu::op_srl(u8 r1)
{
	int v = r1;
	u8 c = r1 & 1;
	v = (v >> 1);
	//u8 oc = r.f & 1;
	//u8 c = r1 & 1;
	//int v = r1;
	//v = (v >> 1) | (oc << 7);

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(0, FS);

	return v;
}

void Cpu::op_sub8(u8 r1)
{
	u16 o = r.a;
	int v = r.a - r1;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((r.a ^ r1) & (r.a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((r.a & 0xf) - (r1 & 0xf)) & FH, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	r.a = v;
}

void Cpu::op_xor(u8 r1)
{
	u8 v = r.a ^= r1;

	set_flag(0, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);
}

void Cpu::handle_interrupts()
{
	if (inte)
	{
		u16 intaddr = r.i << 8 | mem->get_port(0);
		op_push(r.pc);
		r.pc = mem->rw(intaddr);
		halt = false;
		inte = false;
	}

	//cycles -= CYCLES_PER_FRAME;
}

void Cpu::exchange_values(u16* r1, u16* r2)
{
	u16 r, n;
	r = *r1;
	n = *r2;
	*r1 = n;
	*r2 = r;
}

void Cpu::add_pc(u8 op)
{
	//u8 op = mem->rb(rgt.pc + 0);
	u8 b1 = mem->rb(r.pc + 1);
	u8 b3 = mem->rb(r.pc + 3);

	if (op == 0xcb)
		r.pc += disasm_cb[mem->rb(r.pc + 1)].size;
	else if (op == 0xdd)
	{
		if (b1 == 0xcb)
			r.pc += disasm_ddcb[b3].size;
		else
			r.pc += disasm_dd[b1].size;
	}
	else if (op == 0xed)
		r.pc += disasm_ed[mem->rb(r.pc + 1)].size;
	else if (op == 0xfd)
	{
		if (b1 == 0xcb)
			r.pc += disasm_fdcb[b3].size;
		else
			r.pc += disasm_fd[b1].size;
	}
	else
		r.pc += disasm_00[op].size;
}

void Cpu::add_cyc(u8 op)
{
	//u8 op = mem->rb(rgt.pc + 0);
	u8 b1 = mem->rb(r.pc + 1);
	u8 b3 = mem->rb(r.pc + 3);

	if (op == 0xcb)
		cycles += disasm_cb[mem->rb(r.pc + 1)].cycles;
	else if (op == 0xdd)
	{
		if (b1 == 0xcb)
			cycles += disasm_ddcb[b3].cycles;
		else
			cycles += disasm_dd[b1].cycles;
	}
	else if (op == 0xed)
		cycles += disasm_ed[mem->rb(r.pc + 1)].cycles;
	else if (op == 0xfd)
	{
		if (b1 == 0xcb)
			cycles += disasm_ddcb[b3].cycles;
		else
			cycles += disasm_fd[b1].cycles;
	}
	else
		cycles += disasm_00[op].cycles;
}

void Cpu::set_flag(bool flag, u8 v)
{
	if (flag)
		r.f |= v;
	else
		r.f &= ~v;
}

bool Cpu::get_parity(u8 op)
{
	u8 bits = 0;
	for (int i = 0; i < 8; i++)
	{
		bits += op & 1;
		op >>= 1;
	}
	return (bits % 2) == 0;
}

u16 Cpu::get_addr(u16 a1, u16 a2)
{
	return a1 + mem->rb(a2);
}

void Cpu::advance(u8 op)
{
	add_cyc(op);
	add_pc(op);
}

void Cpu::rot_wb_ix(u8 v)
{
	u16 a = get_addr(r.ix, r.pc + 2);
	mem->wb(a, v);
}

void Cpu::rot_wb_iy(u8 v)
{
	u16 a = get_addr(r.iy, r.pc + 2);
	mem->wb(a, v);
}

void Cpu::reset()
{
	reginfo =
	{
		{ "PC", "%s=%-5.04X"},{ "SP", "%s=%-5.04X"},
		{ "AF", "%s=%-5.04X"},{ "BC", "%s=%-5.04X"},
		{ "DE", "%s=%-5.04X"},{ "HL", "%s=%-5.04X"},
		{ "IX", "%s=%-5.04X"},{ "IY", "%s=%-5.04X"},
		{ "AF2", "%s=%-5.04X"},{ "BC2", "%s=%-5.04X"},
		{ "DE2", "%s=%-5.04X"},{ "HL2", "%s=%-5.04X"},
		{ "WZ", "%-5.04X",},{ "R", "%-5.02X",},
		{ "I","%-5.02X"},{ "IM","%-5.d"},
		{ "IFF1","%-5.d"},{ "IFF2", "%-5.d"},
		{ "HALT", "%-5.d"}
	};

	//rg_8bits[0] = &r.b;
	//rg_8bits[1] = &r.c;
	//rg_8bits[2] = &r.d;
	//rg_8bits[3] = &r.e;
	//rg_8bits[4] = &r.h;
	//rg_8bits[5] = &r.l;
	//rg_8bits[6] = &mem->ram[r.hl];
	//rg_8bits[7] = &r.a;

	//rg_16bits[0] = &r.bc;
	//rg_16bits[1] = &r.de;
	//rg_16bits[2] = &r.hl;
	//rg_16bits[3] = &r.sp;

	memset(&r, 0, sizeof(r));
	im = 0;
	iff1 = false;
	iff2 = false;
	halt = false;

	r.ix = 0xffff;
	r.iy = 0xffff;

	cycles = 0;

	mem->reset();
}

