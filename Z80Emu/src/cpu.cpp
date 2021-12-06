#include "cpu.h"

void Cpu::step()
{
	u8 op = mem->rb(pc, true);

	exec_00(op);

#if !RUN_TESTS
	if (halt)
		op_halt();
	else if (cycles > CYCLES_PER_FRAME)
		handle_interrupts();
#endif // !RUN_TESTS
}

void Cpu::set_port(u8 v)
{
	u8 id = mem->rb(pc + 1);
	mem->set_port(mem->rb(pc + 1), v);
	wz = (id + 1) | a << 8;
}

void Cpu::exec_00(u8 op)
{
	switch (op)
	{
	case 0x00:
	{
		advance(op);
		break;
	}
	case 0x01:
	{
		bc = mem->rw(pc + 1);
		advance(op);
		break;
	}
	case 0x02:
	{
		mem->wb(bc, a);
		w = a;
		z = c + 1;
		advance(op);
		break;
	}
	case 0x03:
	{
		bc++;
		advance(op);
		break;
	}
	case 0x04:
	{
		b = op_inc8(b);
		advance(op);
		break;
	}
	case 0x05:
	{
		b = op_dec8(b);
		advance(op);
		break;
	}
	case 0x06:
	{
		b = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x07:
	{
		op_rlca();
		advance(op);
		break;
	}

	case 0x09:
	{
		hl = op_add(hl, bc);
		advance(op);
		break;
	}
	case 0x0a:
	{
		a = mem->rb(bc);
		wz = bc + 1;
		advance(op);
		break;
	}
	case 0x0b:
	{
		bc--;
		advance(op);
		break;
	}
	case 0x0c:
	{
		c = op_inc8(c);
		advance(op);
		break;
	}
	case 0x0d:
	{
		c = op_dec8(c);
		advance(op);
		break;
	}
	case 0x0e:
	{
		c = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x0f:
	{
		op_rrca();
		advance(op);
		break;
	}
	case 0x10:
	{
		op_djnz(op);
		break;
	}
	case 0x11:
	{
		de = mem->rw(pc + 1);
		advance(op);
		break;
	}
	case 0x12:
	{
		mem->wb(de, a);
		w = a;
		z = e + 1;
		advance(op);
		break;
	}
	case 0x13:
	{
		de++;
		advance(op);
		break;
	}
	case 0x14:
	{
		d = op_inc8(d);
		advance(op);
		break;
	}
	case 0x15:
	{
		d = op_dec8(d);
		advance(op);
		break;
	}
	case 0x16:
	{
		d = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x17:
	{
		op_rla8();
		advance(op);
		break;
	}
	case 0x18:
	{
		op_jr(op, true);
		wz = pc;
		break;
	}
	case 0x19:
	{
		hl = op_add(hl, de);
		advance(op);
		break;
	}
	case 0x1a:
	{
		a = mem->rb(de);
		wz = de + 1;
		advance(op);
		break;
	}
	case 0x1b:
	{
		de--;
		advance(op);
		break;
	}
	case 0x1c:
	{
		e = op_inc8(e);
		advance(op);
		break;
	}
	case 0x1d:
	{
		e = op_dec8(e);
		advance(op);
		break;
	}
	case 0x1e:
	{
		e = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x1f:
	{
		op_rra();
		advance(op);
		break;
	}
	case 0x20:
	{
		if (op_jr(op, !(f & FZ))) wz = pc;
		break;
	}

	case 0x21:
	{
		hl = mem->rw(pc + 1);
		advance(op);
		break;
	}
	case 0x22:
	{
		mem->ww(wz = mem->rw(pc + 1), hl);
		wz++;
		advance(op);
		break;
	}
	case 0x23:
	{
		hl++;
		advance(op);
		break;
	}
	case 0x24:
	{
		h = op_inc8(h);
		advance(op);
		break;
	}
	case 0x25:
	{
		h = op_dec8(h);
		advance(op);
		break;
	}
	case 0x26:
	{
		h = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x27:
	{
		op_daa();
		advance(op);
		break;
	}
	case 0x28:
	{
		if (op_jr(op, (f & FZ))) wz = pc;
		break;
	}
	case 0x29:
	{
		hl = op_add(hl, hl);
		advance(op);
		break;
	}
	case 0x2a:
	{
		hl = mem->rw(wz = mem->rw(pc + 1));
		advance(op);
		break;
	}
	case 0x2b:
	{
		hl--;
		advance(op);
		break;
	}
	case 0x2c:
	{
		l = op_inc8(l);
		advance(op);
		break;
	}
	case 0x2d:
	{
		l = op_dec8(l);
		advance(op);
		break;
	}
	case 0x2e:
	{
		l = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x2f:
	{
		op_cpl();
		advance(op);
		break;
	}
	case 0x30:
	{
		if (op_jr(op, !(f & FC))) wz = pc;
		break;
	}
	case 0x31:
	{
		sp = mem->rw(pc + 1);
		advance(op);
		break;
	}
	case 0x32:
	{
		mem->wb(wz = mem->rw(pc + 1), a);
		wz++;
		w = a;
		advance(op);
		break;
	}
	case 0x33:
	{
		sp++;
		advance(op);
		break;
	}
	case 0x34:
	{
		mem->wb(hl, op_inc8(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x35:
	{
		mem->wb(hl, op_dec8(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x36:
	{
		mem->wb(hl, mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0x37:
	{
		op_scf();
		advance(op);
		break;
	}
	case 0x38:
	{
		if (op_jr(op, (f & FC))) wz = pc;
		break;
	}
	case 0x39:
	{
		hl = op_add(hl, sp);
		advance(op);
		break;
	}
	case 0x3a:
	{
		a = mem->rb(wz = mem->rw(pc + 1));
		wz++;
		advance(op);
		break;
	}
	case 0x3b:
	{
		sp--;
		advance(op);
		break;
	}
	case 0x3c:
	{
		a = op_inc8(a);
		advance(op);
		break;
	}
	case 0x3d:
	{
		a = op_dec8(a);
		advance(op);
		break;
	}
	case 0x3e:
	{
		a = mem->rb(pc + 1);
		advance(op);
		break;
	}
	case 0x3f:
	{
		op_ccf();
		advance(op);
		break;
	}
	case 0x40:
	{
		advance(op);
		break;
	}

	case 0x41:
	{
		b = c;
		advance(op);
		break;
	}
	case 0x42:
	{
		b = d;
		advance(op);
		break;
	}
	case 0x43:
	{
		b = e;
		advance(op);
		break;
	}
	case 0x44:
	{
		b = h;
		advance(op);
		break;
	}
	case 0x45:
	{
		b = l;
		advance(op);
		break;
	}
	case 0x46:
	{
		b = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x47:
	{
		b = a;
		advance(op);
		break;
	}
	case 0x48:
	{
		c = b;
		advance(op);
		break;
	}
	case 0x49:
	{
		advance(op);
		break;
	}
	case 0x4a:
	{
		c = d;
		advance(op);
		break;
	}
	case 0x4b:
	{
		c = e;
		advance(op);
		break;
	}

	case 0x4c:
	{
		c = h;
		advance(op);
		break;
	}
	case 0x4d:
	{
		c = l;
		advance(op);
		break;
	}
	case 0x4e:
	{
		c = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x4f:
	{
		c = a;
		advance(op);
		break;
	}
	case 0x50:
	{
		d = b;
		advance(op);
		break;
	}
	case 0x51:
	{
		d = c;
		advance(op);
		break;
	}
	case 0x52:
	{
		advance(op);
		break;
	}
	case 0x53:
	{
		d = e;
		advance(op);
		break;
	}
	case 0x54:
	{
		d = h;
		advance(op);
		break;
	}
	case 0x55:
	{
		d = l;
		advance(op);
		break;
	}
	case 0x56:
	{
		d = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x57:
	{
		d = a;
		advance(op);
		break;
	}
	case 0x58:
	{
		e = b;
		advance(op);
		break;
	}
	case 0x59:
	{
		e = c;
		advance(op);
		break;
	}
	case 0x5a:
	{
		e = d;
		advance(op);
		break;
	}
	case 0x5b:
	{
		add_pc(op);
		break;
	}
	case 0x5c:
	{
		e = h;
		advance(op);
		break;
	}
	case 0x5d:
	{
		e = l;
		advance(op);
		break;
	}
	case 0x5e:
	{
		e = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x5f:
	{
		e = a;
		advance(op);
		break;
	}
	case 0x60:
	{
		h = b;
		advance(op);
		break;
	}
	case 0x61:
	{
		h = c;
		advance(op);
		break;
	}
	case 0x62:
	{
		h = d;
		advance(op);
		break;
	}
	case 0x63:
	{
		h = e;
		advance(op);
		break;
	}
	case 0x64:
	{
		advance(op);
		break;
	}
	case 0x65:
	{
		h = l;
		advance(op);
		break;
	}
	case 0x66:
	{
		h = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x67:
	{
		h = a;
		advance(op);
		break;
	}
	case 0x68:
	{
		l = b;
		advance(op);
		break;
	}
	case 0x69:
	{
		l = c;
		advance(op);
		break;
	}
	case 0x6a:
	{
		l = d;
		advance(op);
		break;
	}
	case 0x6b:
	{
		l = e;
		advance(op);
		break;
	}
	case 0x6c:
	{
		l = h;
		advance(op);
		break;
	}
	case 0x6d:
	{
		advance(op);
		break;
	}
	case 0x6e:
	{
		l = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x6f:
	{
		l = a;
		advance(op);
		break;
	}

	case 0x70:
	{
		mem->wb(hl, b);
		advance(op);
		break;
	}
	case 0x71:
	{
		mem->wb(hl, c);
		advance(op);
		break;
	}
	case 0x72:
	{
		mem->wb(hl, d);
		advance(op);
		break;
	}
	case 0x73:
	{
		mem->wb(hl, e);
		advance(op);
		break;
	}
	case 0x74:
	{
		mem->wb(hl, h);
		advance(op);
		break;
	}
	case 0x75:
	{
		mem->wb(hl, l);
		advance(op);
		break;
	}
	case 0x76:
	{
		halt = true;
		break;
	}
	case 0x77:
	{
		mem->wb(hl, a);
		advance(op);
		break;
	}
	case 0x78:
	{
		a = b;
		advance(op);
		break;
	}
	case 0x79:
	{
		a = c;
		advance(op);
		break;
	}
	case 0x7a:
	{
		a = d;
		advance(op);
		break;
	}
	case 0x7b:
	{
		a = e;
		advance(op);
		break;
	}
	case 0x7c:
	{
		a = h;
		advance(op);
		break;
	}
	case 0x7d:
	{
		a = l;
		advance(op);
		break;
	}
	case 0x7e:
	{
		a = mem->rb(hl);
		advance(op);
		break;
	}
	case 0x7f:
	{
		advance(op);
		break;
	}
	case 0x80:
	{
		op_add8(b);
		advance(op);
		break;
	}
	case 0x81:
	{
		op_add8(c);
		advance(op);
		break;
	}
	case 0x82:
	{
		op_add8(d);
		advance(op);
		break;
	}
	case 0x83:
	{
		op_add8(e);
		advance(op);
		break;
	}
	case 0x84:
	{
		op_add8(h);
		advance(op);
		break;
	}
	case 0x85:
	{
		op_add8(l);
		advance(op);
		break;
	}
	case 0x86:
	{
		op_add8(mem->rb(hl));
		advance(op);
		break;
	}
	case 0x87:
	{
		op_add8(a);
		advance(op);
		break;
	}
	case 0x88:
	{
		op_adc8(b);
		advance(op);
		break;
	}
	case 0x89:
	{
		op_adc8(c);
		advance(op);
		break;
	}
	case 0x8a:
	{
		op_adc8(d);
		advance(op);
		break;
	}
	case 0x8b:
	{
		op_adc8(e);
		advance(op);
		break;
	}
	case 0x8c:
	{
		op_adc8(h);
		advance(op);
		break;
	}
	case 0x8d:
	{
		op_adc8(l);
		advance(op);
		break;
	}
	case 0x8e:
	{
		op_adc8(mem->rb(hl));
		advance(op);
		break;
	}
	case 0x8f:
	{
		op_adc8(a);
		advance(op);
		break;
	}
	case 0x90:
	{
		op_sub8(b);
		advance(op);
		break;
	}
	case 0x91:
	{
		op_sub8(c);
		advance(op);
		break;
	}
	case 0x92:
	{
		op_sub8(d);
		advance(op);
		break;
	}
	case 0x93:
	{
		op_sub8(e);
		advance(op);
		break;
	}
	case 0x94:
	{
		op_sub8(h);
		advance(op);
		break;
	}
	case 0x95:
	{
		op_sub8(l);
		advance(op);
		break;
	}
	case 0x96:
	{
		op_sub8(mem->rb(hl));
		advance(op);
		break;
	}
	case 0x97:
	{
		op_sub8(a);
		advance(op);
		break;
	}
	case 0x98:
	{
		op_sbc8(b);
		advance(op);
		break;
	}
	case 0x99:
	{
		op_sbc8(c);
		advance(op);
		break;
	}
	case 0x9a:
	{
		op_sbc8(d);
		advance(op);
		break;
	}
	case 0x9b:
	{
		op_sbc8(e);
		advance(op);
		break;
	}
	case 0x9c:
	{
		op_sbc8(h);
		advance(op);
		break;
	}
	case 0x9d:
	{
		op_sbc8(l);
		advance(op);
		break;
	}
	case 0x9e:
	{
		op_sbc8(mem->rb(hl));
		advance(op);
		break;
	}
	case 0x9f:
	{
		op_sbc8(a);
		advance(op);
		break;
	}
	case 0xa0:
	{
		op_and(b);
		advance(op);
		break;
	}
	case 0xa1:
	{
		op_and(c);
		advance(op);
		break;
	}
	case 0xa2:
	{
		op_and(d);
		advance(op);
		break;
	}
	case 0xa3:
	{
		op_and(e);
		advance(op);
		break;
	}
	case 0xa4:
	{
		op_and(h);
		advance(op);
		break;
	}
	case 0xa5:
	{
		op_and(l);
		advance(op);
		break;
	}
	case 0xa6:
	{
		op_and(mem->rb(hl));
		advance(op);
		break;
	}
	case 0xa7:
	{
		op_and(a);
		advance(op);
		break;
	}
	case 0xa8:
	{
		op_xor(b);
		advance(op);
		break;
	}
	case 0xa9:
	{
		op_xor(c);
		advance(op);
		break;
	}
	case 0xaa:
	{
		op_xor(d);
		advance(op);
		break;
	}
	case 0xab:
	{
		op_xor(e);
		advance(op);
		break;
	}
	case 0xac:
	{
		op_xor(h);
		advance(op);
		break;
	}
	case 0xad:
	{
		op_xor(l);
		advance(op);
		break;
	}
	case 0xae:
	{
		op_xor(mem->rb(hl));
		advance(op);
		break;
	}
	case 0xaf:
	{
		op_xor(a);
		advance(op);
		break;
	}
	case 0xb0:
	{
		op_or(b);
		advance(op);
		break;
	}
	case 0xb1:
	{
		op_or(c);
		advance(op);
		break;
	}
	case 0xb2:
	{
		op_or(d);
		advance(op);
		break;
	}
	case 0xb3:
	{
		op_or(e);
		advance(op);
		break;
	}
	case 0xb4:
	{
		op_or(h);
		advance(op);
		break;
	}
	case 0xb5:
	{
		op_or(l);
		advance(op);
		break;
	}
	case 0xb6:
	{
		op_or(mem->rb(hl));
		advance(op);
		break;
	}
	case 0xb7:
	{
		op_or(a);
		advance(op);
		break;
	}
	case 0xb8:
	{
		op_cp(b);
		advance(op);
		break;
	}
	case 0xb9:
	{
		op_cp(c);
		advance(op);
		break;
	}
	case 0xba:
	{
		op_cp(d);
		advance(op);
		break;
	}
	case 0xbb:
	{
		op_cp(e);
		advance(op);
		break;
	}
	case 0xbc:
	{
		op_cp(h);
		advance(op);
		break;
	}
	case 0xbd:
	{
		op_cp(l);
		advance(op);
		break;
	}
	case 0xbe:
	{
		op_cp(mem->rb(hl));
		advance(op);
		break;
	}
	case 0xbf:
	{
		op_cp(a);
		advance(op);
		break;
	}
	case 0xc0:
	{
		op_ret(op, !(f & FZ));
		break;
	}
	case 0xc1:
	{
		bc = op_pop();
		advance(op);
		break;
	}
	case 0xc2:
	{
		op_jp(op, !(f & FZ));
		break;
	}
	case 0xc3:
	{
		op_jp(op, true);
		break;
	}
	case 0xc4:
	{
		op_call(op, !(f & FZ));
		add_cyc(op);
		break;
	}
	case 0xc5:
	{
		op_push(bc);
		advance(op);
		break;
	}
	case 0xc6:
	{
		op_add8(mem->rb(pc + 1));
		advance(op);
		break;
	}

	case 0xc8:
	{
		op_ret(op, f & FZ);
		break;
	}
	case 0xc9:
	{
		op_ret(op, true);
		break;
	}

	case 0xca:
	{
		op_jp(op, (f & FZ));
		break;
	}
	case 0xcb:
	{
		exec_cb(op);
		break;
	}
	case 0xcc:
	{
		op_call(op, (f & FZ));
		add_cyc(op);
		break;
	}
	case 0xcd:
	{
		op_call(op, true);
		add_cyc(op);
		break;
	}
	case 0xce:
	{
		op_adc8(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xcf:
	{
		op_rst(0x08);
		add_cyc(op);
		break;
	}
	case 0xd0:
	{
		op_ret(op, !(f & FC));
		break;
	}
	case 0xd1:
	{
		de = op_pop();
		advance(op);
		break;
	}
	case 0xd2:
	{
		op_jp(op, !(f & FC));
		break;
	}
	case 0xd3:
	{
		set_port(a);
		advance(op);
		break;
	}
	case 0xd4:
	{
		op_call(op, !(f & FC));
		add_cyc(op);
		break;
	}
	case 0xd5:
	{
		op_push(de);
		advance(op);
		break;
	}
	case 0xd6:
	{
		op_sub8(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xd7:
	{
		op_rst(0x10);
		add_cyc(op);
		break;
	}
	case 0xd8:
	{
		op_ret(op, f & FC);
		break;
	}
	case 0xd9:
	{
		op_exx();
		advance(op);
		break;
	}
	case 0xda:
	{
		op_jp(op, (f & FC));
		break;
	}
	case 0xdb:
	{
		a = mem->get_port(0);
		advance(op);
		break;
	}
	case 0xdc:
	{
		op_call(op, (f & FC));
		add_cyc(op);
		break;
	}
	case 0xdd:
	{
		exec_dd(op);
		break;
	}
	case 0xde:
	{
		op_sbc8(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xdf:
	{
		op_rst(0x18);
		add_cyc(op);
		break;
	}
	case 0xe1:
	{
		hl = op_pop();
		advance(op);
		break;
	}
	case 0xe5:
	{
		op_push(hl);
		advance(op);
		break;
	}
	case 0xe6:
	{
		op_and(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xe7:
	{
		op_rst(0x20);
		add_cyc(op);
		break;
	}
	case 0xe9:
	{
		pc = hl;
		add_cyc(op);
		break;
	}
	case 0xea:
	{
		op_jp(op, (f & FP));
		break;
	}
	case 0xeb:
	{
		op_ex(&de, &hl);
		advance(op);
		break;
	}
	case 0xed:
	{
		exec_ed(op);
		break;
	}
	case 0xee:
	{
		op_xor(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xef:
	{
		op_rst(0x28);
		add_cyc(op);
		break;
	}
	case 0xf1:
	{
		af = op_pop();
		advance(op);
		break;
	}
	case 0xf3:
	{
		iff1 = false;
		advance(op);
		break;
	}
	case 0xf5:
	{
		op_push(af);
		advance(op);
		break;
	}
	case 0xf6:
	{
		op_or(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xf7:
	{
		op_rst(0x30);
		add_cyc(op);
		break;
	}
	case 0xf8:
	{
		op_ret(op, f & FS);
		add_cyc(op);
		break;
	}
	case 0xf9:
	{
		sp = hl;
		advance(op);
		break;
	}
	case 0xfa:
	{
		op_jp(op, (f & FS));
		break;
	}
	case 0xfb:
	{
		iff1 = true;
		advance(op);
		break;
	}
	case 0xfd:
	{
		exec_fd(op);
		break;
	}
	case 0xfe:
	{
		op_cp(mem->rb(pc + 1));
		advance(op);
		break;
	}
	case 0xff:
	{
		op_rst(0x38);
		add_cyc(op);
		break;
	}
	default:
	{
		printf("%04X %02X\n", pc, op);
		state = cstate::debugging;
		break;
	}


	}

#if RUN_TESTS
	if (op == 0x76)
	{
		printf("%04X %02X\n", pc, op);
		state = cstate::debugging;
	}
#endif // 0
}

void Cpu::exec_cb(u8 op)
{
	u8 b1 = mem->rb(pc + 1);

	switch (b1)
	{
	case 0x00:
	{
		b = op_rlc(b);
		advance(op);
		break;
	}
	case 0x01:
	{
		c = op_rlc(c);
		advance(op);
		break;
	}
	case 0x02:
	{
		d = op_rlc(d);
		advance(op);
		break;
	}
	case 0x03:
	{
		e = op_rlc(e);
		advance(op);
		break;
	}
	case 0x04:
	{
		h = op_rlc(h);
		advance(op);
		break;
	}
	case 0x05:
	{
		l = op_rlc(l);
		advance(op);
		break;
	}
	case 0x06:
	{
		mem->wb(hl, op_rlc(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x07:
	{
		a = op_rlc(a);
		advance(op);
		break;
	}
	case 0x08:
	{
		b = op_rrc(b);
		advance(op);
		break;
	}
	case 0x09:
	{
		c = op_rrc(c);
		advance(op);
		break;
	}
	case 0x0a:
	{
		d = op_rrc(d);
		advance(op);
		break;
	}
	case 0x0b:
	{
		e = op_rrc(e);
		advance(op);
		break;
	}
	case 0x0c:
	{
		h = op_rrc(h);
		advance(op);
		break;
	}
	case 0x0d:
	{
		l = op_rrc(l);
		advance(op);
		break;
	}
	case 0x0e:
	{
		mem->wb(hl, op_rrc(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x0f:
	{
		a = op_rrc(a);
		advance(op);
		break;
	}
	case 0x10:
	{
		b = op_rl(b);
		advance(op);
		break;
	}
	case 0x11:
	{
		c = op_rl(c);
		advance(op);
		break;
	}
	case 0x12:
	{
		d = op_rl(d);
		advance(op);
		break;
	}
	case 0x13:
	{
		e = op_rl(e);
		advance(op);
		break;
	}
	case 0x14:
	{
		h = op_rl(h);
		advance(op);
		break;
	}
	case 0x15:
	{
		l = op_rl(l);
		advance(op);
		break;
	}
	case 0x16:
	{
		mem->wb(hl, op_rl(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x17:
	{
		a = op_rl(a);
		advance(op);
		break;
	}
	case 0x18:
	{
		b = op_rr(b);
		advance(op);
		break;
	}
	case 0x19:
	{
		c = op_rr(c);
		advance(op);
		break;
	}
	case 0x1a:
	{
		d = op_rr(d);
		advance(op);
		break;
	}
	case 0x1b:
	{
		e = op_rr(e);
		advance(op);
		break;
	}
	case 0x1c:
	{
		h = op_rr(h);
		advance(op);
		break;
	}
	case 0x1d:
	{
		l = op_rr(l);
		advance(op);
		break;
	}
	case 0x1e:
	{
		mem->wb(hl, op_rr(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x1f:
	{
		a = op_rr(a);
		advance(op);
		break;
	}
	case 0x20:
	{
		b = op_sla(b);
		advance(op);
		break;
	}
	case 0x21:
	{
		c = op_sla(c);
		advance(op);
		break;
	}
	case 0x22:
	{
		d = op_sla(d);
		advance(op);
		break;
	}
	case 0x23:
	{
		e = op_sla(e);
		advance(op);
		break;
	}
	case 0x24:
	{
		h = op_sla(h);
		advance(op);
		break;
	}
	case 0x25:
	{
		l = op_sla(l);
		advance(op);
		break;
	}
	case 0x26:
	{
		mem->wb(hl, op_sla(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x27:
	{
		a = op_sla(a);
		advance(op);
		break;
	}
	case 0x28:
	{
		b = op_sra(b);
		advance(op);
		break;
	}
	case 0x29:
	{
		c = op_sra(c);
		advance(op);
		break;
	}
	case 0x2a:
	{
		d = op_sra(d);
		advance(op);
		break;
	}
	case 0x2b:
	{
		e = op_sra(e);
		advance(op);
		break;
	}
	case 0x2c:
	{
		h = op_sra(h);
		advance(op);
		break;
	}
	case 0x2d:
	{
		l = op_sra(l);
		advance(op);
		break;
	}
	case 0x2e:
	{
		mem->wb(hl, op_sra(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x2f:
	{
		a = op_sra(a);
		advance(op);
		break;
	}
	case 0x30:
	{
		b = op_sll(b);
		advance(op);
		break;
	}
	case 0x31:
	{
		c = op_sll(c);
		advance(op);
		break;
	}
	case 0x32:
	{
		d = op_sll(d);
		advance(op);
		break;
	}
	case 0x33:
	{
		e = op_sll(e);
		advance(op);
		break;
	}
	case 0x34:
	{
		h = op_sll(h);
		advance(op);
		break;
	}
	case 0x35:
	{
		l = op_sll(l);
		advance(op);
		break;
	}
	case 0x36:
	{
		mem->wb(hl, op_sll(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x37:
	{
		a = op_sll(a);
		advance(op);
		break;
	}
	case 0x38:
	{
		b = op_srl(b);
		advance(op);
		break;
	}
	case 0x39:
	{
		c = op_srl(c);
		advance(op);
		break;
	}
	case 0x3a:
	{
		d = op_srl(d);
		advance(op);
		break;
	}
	case 0x3b:
	{
		e = op_srl(e);
		advance(op);
		break;
	}
	case 0x3c:
	{
		h = op_srl(h);
		advance(op);
		break;
	}
	case 0x3d:
	{
		l = op_srl(l);
		advance(op);
		break;
	}
	case 0x3e:
	{
		mem->wb(hl, op_srl(mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x3f:
	{
		a = op_srl(a);
		advance(op);
		break;
	}
	case 0x40:
	{
		op_bit(0, b);
		advance(op);
		break;
	}
	case 0x41:
	{
		op_bit(0, c);
		advance(op);
		break;
	}
	case 0x42:
	{
		op_bit(0, d);
		advance(op);
		break;
	}
	case 0x43:
	{
		op_bit(0, e);
		advance(op);
		break;
	}
	case 0x44:
	{
		op_bit(0, h);
		advance(op);
		break;
	}
	case 0x45:
	{
		op_bit(0, l);
		advance(op);
		break;
	}
	case 0x46:
	{
		op_bit(0, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x47:
	{
		op_bit(0, a);
		advance(op);
		break;
	}
	case 0x48:
	{
		op_bit(1, b);
		advance(op);
		break;
	}
	case 0x49:
	{
		op_bit(1, c);
		advance(op);
		break;
	}
	case 0x4a:
	{
		op_bit(1, d);
		advance(op);
		break;
	}
	case 0x4b:
	{
		op_bit(1, e);
		advance(op);
		break;
	}
	case 0x4c:
	{
		op_bit(1, h);
		advance(op);
		break;
	}
	case 0x4d:
	{
		op_bit(1, l);
		advance(op);
		break;
	}
	case 0x4e:
	{
		op_bit(1, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x4f:
	{
		op_bit(1, a);
		advance(op);
		break;
	}
	case 0x50:
	{
		op_bit(2, b);
		advance(op);
		break;
	}
	case 0x51:
	{
		op_bit(2, c);
		advance(op);
		break;
	}
	case 0x52:
	{
		op_bit(2, d);
		advance(op);
		break;
	}
	case 0x53:
	{
		op_bit(2, e);
		advance(op);
		break;
	}
	case 0x54:
	{
		op_bit(2, h);
		advance(op);
		break;
	}
	case 0x55:
	{
		op_bit(2, l);
		advance(op);
		break;
	}
	case 0x56:
	{
		op_bit(2, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x57:
	{
		op_bit(2, a);
		advance(op);
		break;
	}
	case 0x58:
	{
		op_bit(3, b);
		advance(op);
		break;
	}
	case 0x59:
	{
		op_bit(3, c);
		advance(op);
		break;
	}
	case 0x5a:
	{
		op_bit(3, d);
		advance(op);
		break;
	}
	case 0x5b:
	{
		op_bit(3, e);
		advance(op);
		break;
	}
	case 0x5c:
	{
		op_bit(3, h);
		advance(op);
		break;
	}
	case 0x5d:
	{
		op_bit(3, l);
		advance(op);
		break;
	}
	case 0x5e:
	{
		op_bit(3, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x5f:
	{
		op_bit(3, a);
		advance(op);
		break;
	}
	case 0x60:
	{
		op_bit(4, b);
		advance(op);
		break;
	}
	case 0x61:
	{
		op_bit(4, c);
		advance(op);
		break;
	}
	case 0x62:
	{
		op_bit(4, d);
		advance(op);
		break;
	}
	case 0x63:
	{
		op_bit(4, e);
		advance(op);
		break;
	}
	case 0x64:
	{
		op_bit(4, h);
		advance(op);
		break;
	}
	case 0x65:
	{
		op_bit(4, l);
		advance(op);
		break;
	}
	case 0x66:
	{
		op_bit(4, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x67:
	{
		op_bit(4, a);
		advance(op);
		break;
	}
	case 0x68:
	{
		op_bit(5, b);
		advance(op);
		break;
	}
	case 0x69:
	{
		op_bit(5, c);
		advance(op);
		break;
	}
	case 0x6a:
	{
		op_bit(5, d);
		advance(op);
		break;
	}
	case 0x6b:
	{
		op_bit(5, e);
		advance(op);
		break;
	}
	case 0x6c:
	{
		op_bit(5, h);
		advance(op);
		break;
	}
	case 0x6d:
	{
		op_bit(5, l);
		advance(op);
		break;
	}
	case 0x6e:
	{
		op_bit(5, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x6f:
	{
		op_bit(5, a);
		advance(op);
		break;
	}
	case 0x70:
	{
		op_bit(6, b);
		advance(op);
		break;
	}
	case 0x71:
	{
		op_bit(6, c);
		advance(op);
		break;
	}
	case 0x72:
	{
		op_bit(6, d);
		advance(op);
		break;
	}
	case 0x73:
	{
		op_bit(6, e);
		advance(op);
		break;
	}
	case 0x74:
	{
		op_bit(6, h);
		advance(op);
		break;
	}
	case 0x75:
	{
		op_bit(6, l);
		advance(op);
		break;
	}
	case 0x76:
	{
		op_bit(6, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x77:
	{
		op_bit(6, a);
		advance(op);
		break;
	}
	case 0x78:
	{
		op_bit(7, b);
		advance(op);
		break;
	}
	case 0x79:
	{
		op_bit(7, c);
		advance(op);
		break;
	}
	case 0x7a:
	{
		op_bit(7, d);
		advance(op);
		break;
	}
	case 0x7b:
	{
		op_bit(7, e);
		advance(op);
		break;
	}
	case 0x7c:
	{
		op_bit(7, h);
		advance(op);
		break;
	}
	case 0x7d:
	{
		op_bit(7, l);
		advance(op);
		break;
	}
	case 0x7e:
	{
		op_bit(7, mem->rb(hl), hl);
		advance(op);
		break;
	}
	case 0x7f:
	{
		op_bit(7, a);
		advance(op);
		break;
	}
	case 0x80:
	{
		b = op_res(0, b);
		advance(op);
		break;
	}
	case 0x81:
	{
		c = op_res(0, c);
		advance(op);
		break;
	}
	case 0x82:
	{
		d = op_res(0, d);
		advance(op);
		break;
	}
	case 0x83:
	{
		e = op_res(0, e);
		advance(op);
		break;
	}
	case 0x84:
	{
		h = op_res(0, h);
		advance(op);
		break;
	}
	case 0x85:
	{
		l = op_res(0, l);
		advance(op);
		break;
	}
	case 0x86:
	{
		mem->wb(hl, op_res(0, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x87:
	{
		a = op_res(0, a);
		advance(op);
		break;
	}
	case 0x88:
	{
		b = op_res(1, b);
		advance(op);
		break;
	}
	case 0x89:
	{
		c = op_res(1, c);
		advance(op);
		break;
	}
	case 0x8a:
	{
		d = op_res(1, d);
		advance(op);
		break;
	}
	case 0x8b:
	{
		e = op_res(1, e);
		advance(op);
		break;
	}
	case 0x8c:
	{
		h = op_res(1, h);
		advance(op);
		break;
	}
	case 0x8d:
	{
		l = op_res(1, l);
		advance(op);
		break;
	}
	case 0x8e:
	{
		mem->wb(hl, op_res(1, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x8f:
	{
		a = op_res(1, a);
		advance(op);
		break;
	}
	case 0x90:
	{
		b = op_res(2, b);
		advance(op);
		break;
	}
	case 0x91:
	{
		c = op_res(2, c);
		advance(op);
		break;
	}
	case 0x92:
	{
		d = op_res(2, d);
		advance(op);
		break;
	}
	case 0x93:
	{
		e = op_res(2, e);
		advance(op);
		break;
	}
	case 0x94:
	{
		h = op_res(2, h);
		advance(op);
		break;
	}
	case 0x95:
	{
		l = op_res(2, l);
		advance(op);
		break;
	}
	case 0x96:
	{
		mem->wb(hl, op_res(2, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x97:
	{
		a = op_res(2, a);
		advance(op);
		break;
	}
	case 0x98:
	{
		b = op_res(3, b);
		advance(op);
		break;
	}
	case 0x99:
	{
		c = op_res(3, c);
		advance(op);
		break;
	}
	case 0x9a:
	{
		d = op_res(3, d);
		advance(op);
		break;
	}
	case 0x9b:
	{
		e = op_res(3, e);
		advance(op);
		break;
	}
	case 0x9c:
	{
		h = op_res(3, h);
		advance(op);
		break;
	}
	case 0x9d:
	{
		l = op_res(3, l);
		advance(op);
		break;
	}
	case 0x9e:
	{
		mem->wb(hl, op_res(3, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0x9f:
	{
		a = op_res(3, a);
		advance(op);
		break;
	}
	case 0xa0:
	{
		b = op_res(4, b);
		advance(op);
		break;
	}
	case 0xa1:
	{
		c = op_res(4, c);
		advance(op);
		break;
	}
	case 0xa2:
	{
		d = op_res(4, d);
		advance(op);
		break;
	}
	case 0xa3:
	{
		e = op_res(4, e);
		advance(op);
		break;
	}
	case 0xa4:
	{
		h = op_res(4, h);
		advance(op);
		break;
	}
	case 0xa5:
	{
		l = op_res(4, l);
		advance(op);
		break;
	}
	case 0xa6:
	{
		mem->wb(hl, op_res(4, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xa7:
	{
		a = op_res(4, a);
		advance(op);
		break;
	}
	case 0xa8:
	{
		b = op_res(5, b);
		advance(op);
		break;
	}
	case 0xa9:
	{
		c = op_res(5, c);
		advance(op);
		break;
	}
	case 0xaa:
	{
		d = op_res(5, d);
		advance(op);
		break;
	}
	case 0xab:
	{
		e = op_res(5, e);
		advance(op);
		break;
	}
	case 0xac:
	{
		h = op_res(5, h);
		advance(op);
		break;
	}
	case 0xad:
	{
		l = op_res(5, l);
		advance(op);
		break;
	}
	case 0xae:
	{
		mem->wb(hl, op_res(5, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xaf:
	{
		a = op_res(5, a);
		advance(op);
		break;
	}
	case 0xb0:
	{
		b = op_res(6, b);
		advance(op);
		break;
	}
	case 0xb1:
	{
		c = op_res(6, c);
		advance(op);
		break;
	}
	case 0xb2:
	{
		d = op_res(6, d);
		advance(op);
		break;
	}
	case 0xb3:
	{
		e = op_res(6, e);
		advance(op);
		break;
	}
	case 0xb4:
	{
		h = op_res(6, h);
		advance(op);
		break;
	}
	case 0xb5:
	{
		l = op_res(6, l);
		advance(op);
		break;
	}
	case 0xb6:
	{
		mem->wb(hl, op_res(6, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xb7:
	{
		a = op_res(6, a);
		advance(op);
		break;
	}
	case 0xb8:
	{
		b = op_res(7, b);
		advance(op);
		break;
	}
	case 0xb9:
	{
		c = op_res(7, c);
		advance(op);
		break;
	}
	case 0xba:
	{
		d = op_res(7, d);
		advance(op);
		break;
	}
	case 0xbb:
	{
		e = op_res(7, e);
		advance(op);
		break;
	}
	case 0xbc:
	{
		h = op_res(7, h);
		advance(op);
		break;
	}
	case 0xbd:
	{
		l = op_res(7, l);
		advance(op);
		break;
	}
	case 0xbe:
	{
		mem->wb(hl, op_res(7, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xbf:
	{
		a = op_res(7, a);
		advance(op);
		break;
	}
	case 0xc0:
	{
		b = op_set(0, b);
		advance(op);
		break;
	}
	case 0xc1:
	{
		c = op_set(0, c);
		advance(op);
		break;
	}
	case 0xc2:
	{
		d = op_set(0, d);
		advance(op);
		break;
	}
	case 0xc3:
	{
		e = op_set(0, e);
		advance(op);
		break;
	}
	case 0xc4:
	{
		h = op_set(0, h);
		advance(op);
		break;
	}
	case 0xc5:
	{
		l = op_set(0, l);
		advance(op);
		break;
	}
	case 0xc6:
	{
		mem->wb(hl, op_set(0, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xc7:
	{
		a = op_set(0, a);
		advance(op);
		break;
	}
	case 0xc8:
	{
		b = op_set(1, b);
		advance(op);
		break;
	}
	case 0xc9:
	{
		c = op_set(1, c);
		advance(op);
		break;
	}
	case 0xca:
	{
		d = op_set(1, d);
		advance(op);
		break;
	}
	case 0xcb:
	{
		e = op_set(1, e);
		advance(op);
		break;
	}
	case 0xcc:
	{
		h = op_set(1, h);
		advance(op);
		break;
	}
	case 0xcd:
	{
		l = op_set(1, l);
		advance(op);
		break;
	}
	case 0xce:
	{
		mem->wb(hl, op_set(1, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xcf:
	{
		a = op_set(1, a);
		advance(op);
		break;
	}
	case 0xd0:
	{
		b = op_set(2, b);
		advance(op);
		break;
	}
	case 0xd1:
	{
		c = op_set(2, c);
		advance(op);
		break;
	}
	case 0xd2:
	{
		d = op_set(2, d);
		advance(op);
		break;
	}
	case 0xd3:
	{
		e = op_set(2, e);
		advance(op);
		break;
	}
	case 0xd4:
	{
		h = op_set(2, h);
		advance(op);
		break;
	}
	case 0xd5:
	{
		l = op_set(2, l);
		advance(op);
		break;
	}
	case 0xd6:
	{
		mem->wb(hl, op_set(2, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xd7:
	{
		a = op_set(2, a);
		advance(op);
		break;
	}
	case 0xd8:
	{
		b = op_set(3, b);
		advance(op);
		break;
	}
	case 0xd9:
	{
		c = op_set(3, c);
		advance(op);
		break;
	}
	case 0xda:
	{
		d = op_set(3, d);
		advance(op);
		break;
	}
	case 0xdb:
	{
		e = op_set(3, e);
		advance(op);
		break;
	}
	case 0xdc:
	{
		h = op_set(3, h);
		advance(op);
		break;
	}
	case 0xdd:
	{
		l = op_set(3, l);
		advance(op);
		break;
	}
	case 0xde:
	{
		mem->wb(hl, op_set(3, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xdf:
	{
		a = op_set(3, a);
		advance(op);
		break;
	}
	case 0xe0:
	{
		b = op_set(4, b);
		advance(op);
		break;
	}
	case 0xe1:
	{
		c = op_set(4, c);
		advance(op);
		break;
	}
	case 0xe2:
	{
		d = op_set(4, d);
		advance(op);
		break;
	}
	case 0xe3:
	{
		e = op_set(4, e);
		advance(op);
		break;
	}
	case 0xe4:
	{
		h = op_set(4, h);
		advance(op);
		break;
	}
	case 0xe5:
	{
		l = op_set(4, l);
		advance(op);
		break;
	}
	case 0xe6:
	{
		mem->wb(hl, op_set(4, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xe7:
	{
		a = op_set(4, a);
		advance(op);
		break;
	}
	case 0xe8:
	{
		b = op_set(5, b);
		advance(op);
		break;
	}
	case 0xe9:
	{
		c = op_set(5, c);
		advance(op);
		break;
	}
	case 0xea:
	{
		d = op_set(5, d);
		advance(op);
		break;
	}
	case 0xeb:
	{
		e = op_set(5, e);
		advance(op);
		break;
	}
	case 0xec:
	{
		h = op_set(5, h);
		advance(op);
		break;
	}
	case 0xed:
	{
		l = op_set(5, l);
		advance(op);
		break;
	}
	case 0xee:
	{
		mem->wb(hl, op_set(5, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xef:
	{
		a = op_set(5, a);
		advance(op);
		break;
	}
	case 0xf0:
	{
		b = op_set(6, b);
		advance(op);
		break;
	}
	case 0xf1:
	{
		c = op_set(6, c);
		advance(op);
		break;
	}
	case 0xf2:
	{
		d = op_set(6, d);
		advance(op);
		break;
	}
	case 0xf3:
	{
		e = op_set(6, e);
		advance(op);
		break;
	}
	case 0xf4:
	{
		h = op_set(6, h);
		advance(op);
		break;
	}
	case 0xf5:
	{
		l = op_set(6, l);
		advance(op);
		break;
	}
	case 0xf6:
	{
		mem->wb(hl, op_set(6, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xf7:
	{
		a = op_set(6, a);
		advance(op);
		break;
	}
	case 0xf8:
	{
		b = op_set(7, b);
		advance(op);
		break;
	}
	case 0xf9:
	{
		c = op_set(7, c);
		advance(op);
		break;
	}
	case 0xfa:
	{
		d = op_set(7, d);
		advance(op);
		break;
	}
	case 0xfb:
	{
		e = op_set(7, e);
		advance(op);
		break;
	}
	case 0xfc:
	{
		h = op_set(7, h);
		advance(op);
		break;
	}
	case 0xfd:
	{
		l = op_set(7, l);
		advance(op);
		break;
	}
	case 0xfe:
	{
		mem->wb(hl, op_set(7, mem->rb(hl)));
		advance(op);
		break;
	}
	case 0xff:
	{
		a = op_set(7, a);
		advance(op);
		break;
	}
	default:
	{
		printf("%04X %02X %02X\n", pc, op, b1);
		state = cstate::debugging;
		break;
	}
	}
}

void Cpu::exec_dd(u8 op)
{
	u8 b1 = mem->rb(pc + 1);

	switch (b1)
	{
	case 0x09:
	{
		ix = op_add(ix, bc);
		advance(op);
		break;
	}

	case 0x19:
	{
		ix = op_add(ix, de);
		advance(op);
		break;
	}

	case 0x21:
	{
		ix = mem->rw(pc + 2);
		advance(op);
		break;
	}
	case 0x22:
	{
		mem->ww(mem->rw(pc + 2), ix);
		advance(op);
		break;
	}
	case 0x23:
	{
		ix++;
		advance(op);
		break;
	}
	case 0x24:
	{
		ixh = op_inc8(ixh);
		advance(op);
		break;
	}
	case 0x25:
	{
		ixh = op_dec8(ixh);
		advance(op);
		break;
	}
	case 0x26:
	{
		ixh = mem->rb(pc + 2);
		advance(op);
		break;
	}

	case 0x29:
	{
		ix = op_add(ix, ix);
		advance(op);
		break;
	}
	case 0x2a:
	{
		ix = mem->rw(mem->rw(pc + 2));
		advance(op);
		break;
	}
	case 0x2b:
	{
		ix--;
		advance(op);
		break;
	}
	case 0x2c:
	{
		ixl = op_inc8(ixl);
		advance(op);
		break;
	}
	case 0x2d:
	{
		ixl = op_dec8(ixl);
		advance(op);
		break;
	}
	case 0x2e:
	{
		ixl = mem->rb(pc + 2);
		advance(op);
		break;
	}

	case 0x34:
	{
		op_inchl(ix, pc + 2);
		advance(op);
		break;
	}
	case 0x35:
	{
		op_dechl(ix, pc + 2);
		advance(op);
		break;
	}
	case 0x36:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, mem->rb(pc + 3));
		advance(op);
		break;
	}

	case 0x39:
	{
		ix = op_add(ix, sp);
		advance(op);
		break;
	}

	case 0x40:
	{
		pc += 2;
		break;
	}

	case 0x46:
	{
		wz = ix + mem->rw(pc + 2);
		b = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x4e:
	{
		wz = ix + mem->rb(pc + 2);
		c = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x56:
	{
		wz = ix + mem->rb(pc + 2);
		d = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x5e:
	{
		wz = ix + mem->rb(pc + 2);
		e = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x66:
	{
		wz = ix + mem->rb(pc + 2);
		h = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x6e:
	{
		wz = ix + mem->rb(pc + 2);
		l = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x70:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, b);
		advance(op);
		break;
	}
	case 0x71:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, c);
		advance(op);
		break;
	}
	case 0x72:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, d);
		advance(op);
		break;
	}
	case 0x73:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, e);
		advance(op);
		break;
	}
	case 0x74:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, h);
		advance(op);
		break;
	}
	case 0x75:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, l);
		advance(op);
		break;
	}

	case 0x77:
	{
		wz = ix + mem->rb(pc + 2);
		mem->wb(wz, a);
		advance(op);
		break;
	}

	case 0x7e:
	{
		wz = ix + mem->rb(pc + 2);
		a = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x84:
	{
		op_add8(ixh);
		advance(op);
		break;
	}
	case 0x85:
	{
		op_add8(ixl);
		advance(op);
		break;
	}
	case 0x86:
	{
		op_add8(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0x8c:
	{
		op_adc8(ixh);
		advance(op);
		break;
	}
	case 0x8d:
	{
		op_adc8(ixl);
		advance(op);
		break;
	}
	case 0x8e:
	{
		op_adc8(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0x94:
	{
		op_sub8(ixh);
		advance(op);
		break;
	}
	case 0x95:
	{
		op_sub8(ixl);
		advance(op);
		break;
	}
	case 0x96:
	{
		op_sub8(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0x9c:
	{
		op_sbc8(ixh);
		advance(op);
		break;
	}
	case 0x9d:
	{
		op_sbc8(ixl);
		advance(op);
		break;
	}
	case 0x9e:
	{
		op_sbc8(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xa4:
	{
		op_and(ixh);
		advance(op);
		break;
	}
	case 0xa5:
	{
		op_and(ixl);
		advance(op);
		break;
	}
	case 0xa6:
	{
		op_and(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xac:
	{
		op_xor(ixh);
		advance(op);
		break;
	}
	case 0xad:
	{
		op_xor(ixl);
		advance(op);
		break;
	}
	case 0xae:
	{
		op_xor(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xb4:
	{
		op_or(ixh);
		advance(op);
		break;
	}
	case 0xb5:
	{
		op_or(ixl);
		advance(op);
		break;
	}
	case 0xb6:
	{
		op_or(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xbc:
	{
		op_cp(ixh);
		advance(op);
		break;
	}
	case 0xbd:
	{
		op_cp(ixl);
		advance(op);
		break;
	}
	case 0xbe:
	{
		op_cp(mem->rb(ix + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xcb:
	{
		exec_ddfd(op, b1, ix);
		break;
	}



	case 0xe1:
	{
		ix = op_pop();
		advance(op);
		break;
	}

	case 0xe5:
	{
		op_push(ix);
		advance(op);
		break;
	}

	default:
	{
		printf("%04X %02X %02X\n", pc, op, b1);
		state = cstate::debugging;
	}
	}
}

void Cpu::exec_ed(u8 op)
{
	u8 b1 = mem->rb(pc + 1);

	switch (b1)
	{
	case 0x42:
	{
		hl = op_sbc(hl, bc);
		advance(op);
		break;
	}
	case 0x43:
	{
		mem->ww(mem->rw(pc + 2), bc);
		advance(op);
		break;
	}
	case 0x44:
	{
		op_neg();
		advance(op);
		break;
	}

	case 0x47:
	{
		i = a;
		advance(op);
		break;
	}

	case 0x4a:
	{
		hl = op_adc(hl, bc);
		advance(op);
		break;
	}
	case 0x4b:
	{
		bc = mem->rw(mem->rw(pc + 2));
		advance(op);
		break;
	}

	case 0x52:
	{
		hl = op_sbc(hl, de);
		advance(op);
		break;
	}
	case 0x53:
	{
		mem->ww(mem->rw(pc + 2), de);
		advance(op);
		break;
	}

	case 0x5a:
	{
		hl = op_adc(hl, de);
		advance(op);
		break;
	}
	case 0x5b:
	{
		de = mem->rw(mem->rw(pc + 2));
		advance(op);
		break;
	}

	case 0x5e:
	{
		im = 2;
		advance(op);
		break;
	}

	case 0x62:
	{
		hl = op_sbc(hl, hl);
		advance(op);
		break;
	}

	case 0x67:
	{
		wz = hl + 1;
		op_rrd();
		advance(op);
		break;
	}

	case 0x6a:
	{
		hl = op_adc(hl, hl);
		advance(op);
		break;
	}

	case 0x6f:
	{
		wz = hl + 1;
		op_rld();
		advance(op);
		break;
	}

	case 0x72:
	{
		hl = op_sbc(hl, sp);
		advance(op);
		break;
	}
	case 0x73:
	{
		mem->ww(wz = mem->rw(pc + 2), sp);
		wz++;
		advance(op);
		break;
	}

	case 0x7a:
	{
		hl = op_adc(hl, sp);
		advance(op);
		break;
	}
	case 0x7b:
	{
		sp = mem->rw(wz = mem->rw(pc + 2));
		wz++;
		advance(op);
		break;
	}

	case 0xa0:
	{
		op_ldi();
		advance(op);
		break;
	}
	case 0xa1:
	{
		op_cpi();
		advance(op);
		break;
	}

	case 0xa8:
	{
		op_ldd();
		advance(op);
		break;
	}
	case 0xa9:
	{
		op_cpd();
		advance(op);
		break;
	}

	case 0xb0:
	{
		op_ldir(op);
		break;
	}
	case 0xb1:
	{
		op_cpir(op);
		break;
	}

	case 0xb8:
	{
		op_lddr(op);
		break;
	}
	case 0xb9:
	{
		op_cpdr(op);
		break;
	}
	default:
	{
		printf("%04X %02X %02X\n", pc, op, b1);
		state = cstate::debugging;
	}
	}
}

void Cpu::exec_fd(u8 op)
{
	u8 b1 = mem->rb(pc + 1);

	switch (b1)
	{
	case 0x09:
	{
		iy = op_add(iy, bc);
		advance(op);
		break;
	}

	case 0x19:
	{
		iy = op_add(iy, de);
		advance(op);
		break;
	}

	case 0x21:
	{
		iy = mem->rw(pc + 2);
		advance(op);
		break;
	}
	case 0x22:
	{
		mem->ww(mem->rw(pc + 2), iy);
		advance(op);
		break;
	}
	case 0x23:
	{
		iy++;
		advance(op);
		break;
	}

	case 0x26:
	{
		iyh = mem->rb(pc + 2);
		advance(op);
		break;
	}

	case 0x29:
	{
		iy = op_add(iy, iy);
		advance(op);
		break;
	}
	case 0x2a:
	{
		iy = mem->rw(mem->rw(pc + 2));
		advance(op);
		break;
	}
	case 0x2b:
	{
		iy--;
		advance(op);
		break;
	}

	case 0x2e:
	{
		iyl = mem->rb(pc + 2);
		advance(op);
		break;
	}

	case 0x34:
	{
		op_inchl(iy, pc + 2);
		advance(op);
		break;
	}
	case 0x35:
	{
		op_dechl(ix, pc + 2);
		advance(op);
		break;
	}
	case 0x36:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, mem->rb(pc + 3));
		advance(op);
		break;
	}

	case 0x39:
	{
		iy = op_add(iy, sp);
		advance(op);
		break;
	}

	case 0x46:
	{
		wz = iy + mem->rb(pc + 2);
		b = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x4e:
	{
		wz = iy + mem->rb(pc + 2);
		c = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x56:
	{
		wz = iy + mem->rb(pc + 2);
		d = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x5e:
	{
		wz = iy + mem->rb(pc + 2);
		e = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x66:
	{
		wz = iy + mem->rb(pc + 2);
		h = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x6e:
	{
		wz = iy + mem->rb(pc + 2);
		l = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x70:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, b);
		advance(op);
		break;
	}
	case 0x71:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, c);
		advance(op);
		break;
	}
	case 0x72:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, d);
		advance(op);
		break;
	}
	case 0x73:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, e);
		advance(op);
		break;
	}
	case 0x74:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, h);
		advance(op);
		break;
	}
	case 0x75:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, l);
		advance(op);
		break;
	}

	case 0x77:
	{
		wz = iy + mem->rb(pc + 2);
		mem->wb(wz, a);
		advance(op);
		break;
	}

	case 0x7e:
	{
		wz = iy + mem->rb(pc + 2);
		a = mem->rb(wz);
		advance(op);
		break;
	}

	case 0x84:
	{
		op_add8(iyh);
		advance(op);
		break;
	}
	case 0x85:
	{
		op_add8(iyl);
		advance(op);
		break;
	}
	case 0x86:
	{
		op_add8(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}


	case 0x8c:
	{
		op_adc8(iyh);
		advance(op);
		break;
	}
	case 0x8d:
	{
		op_adc8(iyl);
		advance(op);
		break;
	}
	case 0x8e:
	{
		op_adc8(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0x94:
	{
		op_sub8(iyh);
		advance(op);
		break;
	}
	case 0x95:
	{
		op_sub8(iyl);
		advance(op);
		break;
	}
	case 0x96:
	{
		op_sub8(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0x9c:
	{
		op_sbc8(iyh);
		advance(op);
		break;
	}
	case 0x9d:
	{
		op_sbc8(iyl);
		advance(op);
		break;
	}
	case 0x9e:
	{
		op_sbc8(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xa4:
	{
		op_and(iyh);
		advance(op);
		break;
	}
	case 0xa5:
	{
		op_and(iyl);
		advance(op);
		break;
	}
	case 0xa6:
	{
		op_and(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xac:
	{
		op_xor(iyh);
		advance(op);
		break;
	}
	case 0xad:
	{
		op_xor(iyl);
		advance(op);
		break;
	}
	case 0xae:
	{
		op_xor(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xb4:
	{
		op_or(iyh);
		advance(op);
		break;
	}
	case 0xb5:
	{
		op_or(iyl);
		advance(op);
		break;
	}
	case 0xb6:
	{
		op_or(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xbc:
	{
		op_cp(iyh);
		advance(op);
		break;
	}
	case 0xbd:
	{
		op_cp(iyl);
		advance(op);
		break;
	}
	case 0xbe:
	{
		op_cp(mem->rb(iy + mem->rb(pc + 2)));
		advance(op);
		break;
	}

	case 0xcb:
	{
		exec_ddfd(op, b1, iy);
		break;
	}

	case 0xe1:
	{
		iy = op_pop();
		advance(op);
		break;
	}

	case 0xe5:
	{
		op_push(iy);
		advance(op);
		break;
	}
	default:
	{
		printf("%04X %02X %02X\n", pc, op, b1);
		state = cstate::debugging;
		break;
	}
	}
}

void Cpu::exec_ddfd(u8 op, u8 b1, u16 reg)
{
	u8 b3 = mem->rb(pc + 3);

	wz = reg + mem->rb(pc + 2);

	switch (b3)
	{

	case 0x06:
	{
		mem->wb(wz,op_rlc(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x0e:
	{
		mem->wb(wz,op_rrc(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x16:
	{
		mem->wb(wz,op_rl(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x1e:
	{
		mem->wb(wz,op_rr(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x26:
	{
		mem->wb(wz,op_sla(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x2e:
	{
		mem->wb(wz,op_sra(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x36:
	{
		mem->wb(wz,op_sll(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x3e:
	{
		mem->wb(wz,op_srl(mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x46:
	{
		//wz = reg + mem->rb(pc + 2);
		op_bit(0, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x4e:
	{
		op_bit(1, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x56:
	{
		op_bit(2, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x5e:
	{
		op_bit(3, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x66:
	{
		op_bit(4, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x6e:
	{
		op_bit(5, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x76:
	{
		op_bit(6, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x7e:
	{
		op_bit(7, mem->rb(wz), wz);
		advance(op);
		break;
	}

	case 0x86:
	{
		mem->wb(wz,op_res(0, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x8e:
	{
		mem->wb(wz,op_res(1, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x96:
	{
		mem->wb(wz,op_res(2, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0x9e:
	{
		mem->wb(wz,op_res(3, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xa6:
	{
		mem->wb(wz,op_res(4, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xae:
	{
		mem->wb(wz,op_res(5, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xb6:
	{
		mem->wb(wz,op_res(6, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xbe:
	{
		mem->wb(wz,op_res(7, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xc6:
	{
		mem->wb(wz,op_set(0, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xce:
	{
		mem->wb(wz,op_set(1, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xd6:
	{
		mem->wb(wz,op_set(2, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xde:
	{
		mem->wb(wz,op_set(3, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xe6:
	{
		mem->wb(wz,op_set(4, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xee:
	{
		mem->wb(wz,op_set(5, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xf6:
	{
		mem->wb(wz,op_set(6, mem->rb(wz)));
		advance(op);
		break;
	}

	case 0xfe:
	{
		mem->wb(wz,op_set(7, mem->rb(wz)));
		advance(op);
		break;
	}

	default:
	{
		printf("%04X %02X %02X %02X\n", pc, op, b1, b3);
		state = cstate::debugging;
	}
	}
}

void Cpu::op_adc8(u8 r1)
{
	u8 cf = (f & 1);
	int v = a + r1 + cf;

	set_flag(v > 0xff, FC);
	set_flag(0, FN);
	set_flag((~(a ^ r1) & (a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((a & 0xf) + (r1 & 0xf) + cf) & FH, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

u16 Cpu::op_adc(u16 r1, u16 r2)
{
	int c = (f & 1);
	int v = r1 + r2 + c;
	wz = r1 + 1;

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
	int v = a + r1;

	set_flag(v > 0xff, FC);
	set_flag(0, FN);
	set_flag((~(a ^ r1) & (a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((a & 0xf) + (r1 & 0xf)) & FH, FH);
	set_flag(v & FY, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

u16 Cpu::op_add(u16 r1, u16 r2)
{
	int v = r1 + r2;
	wz = r1 + 1;

	set_flag(0, FN);
	set_flag(v > 0xffff, FC);
	set_flag((((r1 & 0xfff) + (r2 & 0xfff))) & 0x1000, FH);
	set_flag((v >> 11) & 1, FX);
	set_flag((v >> 13) & 1, FY);

	return v;
}

void Cpu::op_and(u8 r1)
{
	int v = a & r1;

	set_flag(0, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(1, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

void Cpu::op_bit(u8 r1, u8 r2, s16 addr)
{
	int v = r2 & (1 << r1);

	if (addr > -1)
	{
		wz = addr;
		set_flag(w & FX, FX);
		set_flag(w & FY, FY);
	}
	else
	{
		set_flag(r2 & FX, FX);
		set_flag(r2 & FY, FY);
	}

	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(1, FH);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

}

void Cpu::op_call(u8 op, bool flag)
{
	if (flag)
	{
		op_push(pc + 3);
		wz = pc = mem->rw(pc + 1);
		cycles += disasm_00[op].cycles;
	}
	else
	{
		pc += 3;
		cycles += disasm_00[op].cycles2;
	}
}

void Cpu::op_ccf()
{

	u8 c = f & FC;
	set_flag(c, FH);
	set_flag(!(f & FC), FC);
	set_flag(0, FN);
	set_flag(a & FX, FX);
	set_flag(a & FY, FY);
}

void Cpu::op_cp(u8 r1)
{
	int v = a - r1;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((a ^ v) & (a ^ r1) & 0x80), FP);
	set_flag((r1 >> 3) & 1, FX);
	set_flag(((a & 0xf) - (r1 & 0xf) & 0x10), FH);
	set_flag((r1 >> 5) & 1, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);
}

void Cpu::op_cpd()
{
	int v = a - mem->rb(hl);

	set_flag(1, FN);
	set_flag(bc - 1, FP);
	set_flag(((v & 0xf) > (a & 0xf)), FH);
	if (f & FH) v--;
	set_flag(v & FX, FX);
	set_flag(v & 0x02, FY);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);

	bc--;
	hl--;
	wz--;
}

void Cpu::op_cpdr(u8 op)
{
	op_cpd();

	if ((s16)bc == 0 || f & FZ)
	{
		cycles += disasm_ed[mem->rb(pc + 1)].cycles2;
		add_pc(op);
	}
	else
	{
		wz = pc + 1;
		cycles += disasm_ed[mem->rb(pc + 1)].cycles;
	}

}

void Cpu::op_cpi()
{
	int v = a - mem->rb(hl);

	set_flag(1, FN);
	set_flag(bc - 1, FP);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(((v & 0xf) > (a & 0xf)), FH);
	if (f & FH) v--;
	set_flag(v & FX, FX);
	set_flag(v & 0x02, FY);
	set_flag(v & 0x80, FS);

	bc--;
	hl++;
	wz++;
}

void Cpu::op_cpir(u8 op)
{
	op_cpi();

	if ((s16)bc == 0 || (f & FZ))
	{
		cycles += disasm_ed[mem->rb(pc + 1)].cycles2;
		add_pc(op);
	}
	else
	{
		wz = pc + 1;
		cycles += disasm_ed[mem->rb(pc + 1)].cycles;
	}
}

void Cpu::op_cpl()
{
	u8 r1 = ~a;

	set_flag(1, FN);
	set_flag((r1 >> 3) & 1, FX);
	set_flag(1, FH);
	set_flag((r1 >> 5) & 1, FY);

	a = r1;
}

void Cpu::op_daa()
{
	int v = a;

	if (f & FN)
	{
		if (f & FH || (a & 0xf) > 9)
			v -= 6;
		if (f & FC || (a > 0x99))
			v -= 0x60;

		//set_flag(((a & 0xf) - (v & 0xf)) & FH, FH);
	}
	else
	{
		if ((f & FH) || (a & 0xf) > 9)
			v += 6;
		if ((f & FC) || (a > 0x99))
			v += 0x60;

		//set_flag(((a & 0xf) + (v & 0xf)) & FH, FH);
	}

	set_flag(f & FC || a > 0x99, FC);
	set_flag(get_parity(v), FP);
	set_flag(((a & 0x10) ^ (v & 0x10)) & FH, FH);
	set_flag((v & 0xff) == 0, FZ);
	set_flag(v & 0x80, FS);
	set_flag(v & FX, FX);
	set_flag(v & FY, FY);

	a = v;
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
	if (--b > 0)
	{
		wz = pc += (s8)mem->rb(pc + 1) + 2;
		cycles += disasm_00[mem->rb(pc)].cycles;
	}
	else
	{
		pc += 2;
		cycles += disasm_00[mem->rb(pc)].cycles2;
	}

}

void Cpu::op_ex(u16* op, u16* op2)
{
	exchange_values(op, op2);
}

void Cpu::op_exx()
{
	exchange_values(&bc, &sbc);
	exchange_values(&de, &sde);
	exchange_values(&hl, &shl);
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
			u16 intaddr = i << 8 | mem->get_port(0);
			sp -= 2;
			pc = mem->rw(intaddr);
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
	wz = mem->rw(pc + 1);
	if (flag)
	{
		pc = wz;
		cycles += disasm_00[op].cycles;
	}
	else
	{
		pc += 3;
		cycles += disasm_00[op].cycles2;
	}
}

bool Cpu::op_jr(u8 op, bool flag)
{
	if (flag)
	{
		pc += (s8)mem->rb(pc + 1) + 2;
		cycles += disasm_00[op].cycles;
		return true;
	}

	pc += 2;
	cycles += disasm_00[op].cycles2;

	return false;
}

void Cpu::op_ldd()
{
	mem->wb(de, mem->rb(hl));

	set_flag(0, FN);
	set_flag(bc - 1, FP);
	set_flag(0, FH);
	u8 xy = mem->rb(hl);
	set_flag((xy + a) & FX, FX);
	set_flag((xy + a) & FN, FY);

	bc--;
	hl--;
	de--;
}

void Cpu::op_lddr(u8 op)
{
	op_ldd();

	if ((s16)bc == 0)
	{
		cycles += disasm_ed[mem->rb(pc + 1)].cycles2;
		add_pc(op);
	}
	else
		cycles += disasm_ed[mem->rb(pc + 1)].cycles;
}

void Cpu::op_ldi()
{
	mem->wb(de, mem->rb(hl));

	set_flag(0, FN);
	set_flag(bc - 1, FP);
	set_flag(0, FH);
	u8 xy = mem->rb(hl);
	set_flag((xy + a) & FX, FX);
	set_flag((xy + a) & FN, FY);

	bc--;
	hl++;
	de++;
}

void Cpu::op_ldir(u8 op)
{
	op_ldi();

	if ((s16)bc == 0)
	{
		cycles += disasm_ed[mem->rb(pc + 1)].cycles2;
		add_pc(op);
	}
	else
	{
		wz = pc + 1;
		cycles += disasm_ed[mem->rb(pc + 1)].cycles;
	}
}

void Cpu::op_neg()
{
	u8 v = 0 - a;

	set_flag(a != 0, FC);
	set_flag(1, FN);
	set_flag(a == 0x80, FP);
	set_flag(v & FX, FX);
	set_flag(((a & 0xf) + (v & 0xf) & 0x10), FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

void Cpu::op_or(u8 r1)
{
	int v = a | r1;

	set_flag(0, FC);
	set_flag(0, FN);
	set_flag(get_parity(v), FP);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

u8 Cpu::op_pop8()
{
	u8 l;
	l = mem->rb(sp++);

	return l;
}

u16 Cpu::op_pop()
{
	u8 h, l;
	l = mem->rb(sp++);
	h = mem->rb(sp++);

	return h << 8 | l;
}

void Cpu::op_push8(u8 r1)
{
	mem->wb(--sp, r1);
}

void Cpu::op_push(u16 r1)
{
	mem->wb(--sp, r1 >> 8);
	mem->wb(--sp, r1 & 0xff);
}

void Cpu::op_ret(u8 op, bool flag)
{
	if (flag)
	{
		pc = op_pop();
		cycles += disasm_00[op].cycles;
		wz = pc;
	}
	else
	{
		pc++;
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
	u8 c = f & 1;
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
	u16 v = a << 1;
	u8 oc = f & 1;
	u8 c = v >> 8;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	a = v | oc;
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
	u16 v = a << 1;
	u8 c = v >> 8;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	a = v | c;
}

void Cpu::op_rld()
{
	u8 v = mem->rb(hl);

	mem->wb(hl, (v << 4) + ((a & 0xf)));
	a = (a & 0xf0) + (v >> 4);

	set_flag(0, FN);
	set_flag(get_parity(a), FP);
	set_flag(a & FX, FX);
	set_flag(0, FH);
	set_flag(a & FY, FY);
	set_flag(a == 0, FZ);
	set_flag(a & 0x80, FS);
}

u8 Cpu::op_rr(u8 r1)
{
	u8 oc = f & 1;
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
	u8 c = a & 1;
	u8 v = a = a >> 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	a |= c << 7;
}

void Cpu::op_rra()
{
	u8 oc = f & 1;
	u8 c = a & 1;
	u8 v = a >> 1;

	set_flag(c, FC);
	set_flag(0, FN);
	set_flag(v & FX, FX);
	set_flag(0, FH);
	set_flag(v & FY, FY);

	a = v | (oc << 7);
}

void Cpu::op_rrd()
{
	u8 v = mem->rb(hl);

	mem->wb(hl, (v >> 4) + ((a & 0xf) << 4));
	a = (a & 0xf0) + (v & 0xf);

	set_flag(0, FN);
	set_flag(get_parity(a), FP);
	set_flag(a & FX, FX);
	set_flag(0, FH);
	set_flag(a & FY, FY);
	set_flag(a == 0, FZ);
	set_flag(a & 0x80, FS);
}

void Cpu::op_rst(u8 r1)
{
	op_push(pc + 1);
	pc = wz = r1;
}

u8 Cpu::op_set(u8 r1, u8 r2)
{
	return r2 | (1 << r1);
}

void Cpu::op_sbc8(u8 r1)
{
	u8 cf = f & 1;
	int v = a - r1 - cf;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((a ^ r1) & (a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((a & 0xf) - (r1 & 0xf) - cf) & FH, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

u16 Cpu::op_sbc(u16 r1, u16 r2)
{
	u8 cf = f & 1;
	int v = r1 - r2 - cf;
	wz = r1 + 1;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((r1 ^ r2) & (r1 ^ v) & 0x8000), FP);
	set_flag((v >> 11) & 1, FX);
	set_flag((((r1 & 0xfff) - (r2 & 0xfff) - cf)) & 0x1000, FH);
	set_flag((v >> 13) & 1, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x8000, FS);

	return v;
}

void Cpu::op_scf()
{
	set_flag(1, FC);
	set_flag(0, FN);
	set_flag(a & FX, FX);
	set_flag(0, FH);
	set_flag(a & FY, FY);
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
	//u8 oc = f & 1;
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
	u16 o = a;
	int v = a - r1;

	set_flag(v < 0, FC);
	set_flag(1, FN);
	set_flag(((a ^ r1) & (a ^ v) & 0x80), FP);
	set_flag(v & FX, FX);
	set_flag(((a & 0xf) - (r1 & 0xf)) & FH, FH);
	set_flag(v & FY, FY);
	set_flag(v == 0, FZ);
	set_flag(v & 0x80, FS);

	a = v;
}

void Cpu::op_xor(u8 r1)
{
	u8 v = a ^= r1;

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
		u16 intaddr = i << 8 | mem->get_port(0);
		op_push(pc);
		pc = mem->rw(intaddr);
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
	u8 b1 = mem->rb(pc + 1);
	u8 b3 = mem->rb(pc + 3);

	if (op == 0xcb)
		pc += disasm_cb[mem->rb(pc + 1)].size;
	else if (op == 0xdd)
	{
		if (b1 == 0xcb)
			pc += disasm_ddcb[b3].size;
		else
			pc += disasm_dd[b1].size;
	}
	else if (op == 0xed)
		pc += disasm_ed[mem->rb(pc + 1)].size;
	else if (op == 0xfd)
	{
		if (b1 == 0xcb)
			pc += disasm_fdcb[b3].size;
		else
			pc += disasm_fd[b1].size;
	}
	else
		pc += disasm_00[op].size;
}

void Cpu::add_cyc(u8 op)
{
	//u8 op = mem->rb(rgt.pc + 0);
	u8 b1 = mem->rb(pc + 1);
	u8 b3 = mem->rb(pc + 3);

	if (op == 0xcb)
		cycles += disasm_cb[mem->rb(pc + 1)].cycles;
	else if (op == 0xdd)
	{
		if (b1 == 0xcb)
			cycles += disasm_ddcb[b3].cycles;
		else
			cycles += disasm_dd[b1].cycles;
	}
	else if (op == 0xed)
		cycles += disasm_ed[mem->rb(pc + 1)].cycles;
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
		f |= v;
	else
		f &= ~v;
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

void Cpu::reset()
{
	af = 0x0000; bc = 0x0000; de = 0x0000; hl = 0x0000;
	saf = 0x0000; sbc = 0x0000; sde = 0x0000; shl = 0x0000;
	ix = 0x0000; iy = 0x0000; wz = 0x0000; sp = 0x0000;
	r = 0x00; i = 0x00;
	im = false; iff1 = false; iff2 = false; halt = false;
	pc = 0x0000;

	reginfo =
	{
		{ "PC", "%s=%-5.04X"},{ "SP", "%s=%-5.04X"},
		{ "AF", "%s=%-5.04X"},{ "BC", "%s=%-5.04X"},
		{ "DE", "%s=%-5.04X"},{ "HL", "%s=%-5.04X"},
		{ "IX", "%s=%-5.04X"},{ "IY", "%s=%-5.04X"},
		{ "AF2", "%s=%-5.04X"},{ "BC2", "%s=%-5.04X"},
		{ "DE2", "%s=%-5.04X"},{ "HL2", "%s=%-5.04X"},
		{ "WZ", "%s=%-5.04X",},{ "R", "%-5.02X",},
		{ "I","%-5.02X"},{ "IM","%-5.d"},
		{ "IFF1","%-5.d"},{ "IFF2", "%-5.d"},
		{ "HALT", "%-5.d"}
	};

	//rg_8bits[0] = &b;
	//rg_8bits[1] = &c;
	//rg_8bits[2] = &d;
	//rg_8bits[3] = &e;
	//rg_8bits[4] = &h;
	//rg_8bits[5] = &l;
	//rg_8bits[6] = &mem->ram[hl];
	//rg_8bits[7] = &a;

	//rg_16bits[0] = &bc;
	//rg_16bits[1] = &de;
	//rg_16bits[2] = &hl;
	//rg_16bits[3] = &sp;

	memset(&r, 0, sizeof(r));
	im = 0;
	iff1 = false;
	iff2 = false;
	halt = false;

	ix = 0xffff;
	iy = 0xffff;

	cycles = 0;

	mem->reset();
}

