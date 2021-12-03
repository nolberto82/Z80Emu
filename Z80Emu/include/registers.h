#pragma once

#include "types.h"

class Z80;

//u16 get_pc() { return regt.pc; }

typedef struct
{
	u16 offset;
	std::string name;
	std::string oper;
	std::string pctext;
	std::string regtext;
	std::string dtext;
	std::string bytetext;
	std::string looptext;
	int size;
}disasmentry;

typedef struct
{
	std::string name;
	std::string format;
}sregdata;

typedef struct
{
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

	struct
	{
		u8 sa, sf;
	};
	u16 saf;

	struct
	{
		u8 sb, sc;
	};
	u16 sbc;

	struct
	{
		u8 sd, se;
	};
	u16 sde;

	struct
	{
		u8 sh, sl;
	};
	u16 shl;

	struct
	{
		u8 sx, sy;
	};
	u16 sxy;

	u16 sp;
	u16 pc;
	u16 prevpc;
	u16 wz;
	u8 i;
	u8 r;
}reg_t;