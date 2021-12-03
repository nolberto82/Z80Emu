#include "debugger.h"
#include "registers.h"

void Debugger::step()
{
	int* state = cpu->get_state();
	int* cycles = cpu->get_cycles();
	static bool log_written = false;
	int divide = 1;
#if RUN_TESTS
	divide = logging ? 10 : 1;
#endif // RUN_TESTS

	if (*state == cstate::running)
	{
		while (*cycles < (CYCLES_PER_FRAME / divide))
		{
			u16 pc = cpu->r.pc;

			for (auto it : bpk->get_breakpoints())
			{
				if (bpk->check(pc, it.enabled))
				{
					*state = cstate::debugging;
					return;
				}

				if (bpk->check(pc, it.enabled))
				{
					*state = cstate::debugging;
					return;
				}

				if (bpk->check_access(cpu->get_write_addr(), bp_write, it.enabled))
				{
					*state = cstate::debugging;
					cpu->set_write_addr(0);
					return;
				}

				if (bpk->check_access(cpu->get_read_addr(), bp_read, it.enabled))
				{
					*state = cstate::debugging;
					cpu->set_read_addr(0);
					return;
				}
			}

			if (*state == cstate::debugging || *state == cstate::crashed)
				return;

			if (logging)
			{
#if RUN_TESTS
				if (cpu->r.pc == 0x1d42 || cpu->r.pc == 0x1d44 || cpu->r.pc == 0x1d46)
				{
					//logger(pc);
					char text[TEXTSIZE] = { 0 };
					vdentry = disasm(text, pc, true);

					//std::ofstream outFile("cpu_trace.log", std::ios_base::app);
					for (const auto& e : vdentry)
					{
						outFile
							<< e.regtext
							<< std::uppercase << std::hex << std::setw(4) << std::setfill('0') << e.offset
							<< ": "
							<< e.dtext
							<< "\n";
					}
				}
#else
				//logger(pc);
				char text[TEXTSIZE] = { 0 };
				vdentry = disasm(text, pc, true);

				//std::ofstream outFile("cpu_trace.log", std::ios_base::app);
				for (const auto& e : vdentry)
				{
					outFile
						<< e.regtext
						<< std::uppercase << std::hex << std::setw(4) << std::setfill('0') << e.offset
						<< ": "
						<< e.dtext
						<< "\n";
				}
#endif // RUN_TESTS
			}

			cpu->step();

#if RUN_TESTS
			get_test_messages();
#endif // RUN_TESTS

		}

		*cycles -= CYCLES_PER_FRAME / divide;
	}
}

void Debugger::update_gl()
{
	//if (!gl->init())
	//	return;

	//gl->update();
}

void Debugger::update()
{
	if (!sdl->init(mem))
		return;

	int running = 1;

	while (running)
	{
		static bool sprites = true;

		ImGuiIO& io = ImGui::GetIO();

		sdl->update_input(running, io);

		sdl->render_tiles(mem, sprites);
		sdl->render_display(mem);

		sdl->begin_frame();

		//ImGui::ShowDemoWindow(&show_demo_window);
		gui(io);
		step();

		if (showtiles)
		{
			static std::string strtiles = "Tiles/Sprites viewer";

			if (ImGui::Begin(strtiles.c_str(), NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
			{
				ImVec2 tsize = ImGui::GetContentRegionAvail();
				tsize.y -= 25;

				ImGui::Checkbox("Show Sprites", &sprites);

				ImGui::Image((void*)sdl->get_tile(), tsize);
			}
			ImGui::End();
		}

		if (ImGui::Begin("Display", NULL))
		{
			ImVec2 tsize = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)sdl->get_display(), tsize/*ImVec2(224, 288)*/);
		}
		ImGui::End();

		//if (ImGui::Begin("Tile8x4", NULL))
		//{
		//	ImVec2 tsize = ImGui::GetContentRegionAvail();
		//	sdl->update_tile16();
		//	ImGui::Image((void*)sdl->get_chars(), tsize);
		//}
		//ImGui::End();

		if (ImGui::Begin("Sprite Info", NULL))
		{
			ImVec2 tsize = ImGui::GetContentRegionAvail();

			for (int i = 0; i < 7; i++)
			{
				u8 id = mem->ram[0x4ff0 + i * 2] & 0xfc;
				u8 fx = (mem->ram[0x4ff0 + i * 2] >> 1 & 0x1);
				u8 fy = mem->ram[0x4ff0 + i * 2] & 0x1;
				u8 pal = mem->ram[0x4ff0 + i * 2 + 1];
				u8 x = 224 - mem->sprite_data[i * 2];
				u8 y = 288 - mem->sprite_data[i * 2 + 1];

				ImGui::Text("Sprite %d: x:%d y:%d pal:%d fx:%d fy:%d", id, x, y, pal, fx, fy);
			}

		}
		ImGui::End();

		static bool written = false;

		if (ImGui::Begin("Tests Info", nullptr))
		{
			//int i = 0;

			////std::sort(test_result.begin(), test_result.end());
			//test_result.erase(std::unique(test_result.begin(), test_result.end()), test_result.end());

			//for (auto& e : test_result)
			//{
			//	ImGui::Text(e.c_str());
			//}
			ImGui::Text(cpu->resstr.c_str());
		}
		ImGui::End();

		sdl->end_frame();
	}
}

void Debugger::gui(ImGuiIO io)
{
	static u16 inputaddr = 0;
	static int lineoffset = 0;
	static bool is_jump = false;
	u16 pc = is_jump ? inputaddr : cpu->r.pc;;

	show_disassembly(pc, lineoffset);
	show_registers();
	show_memory();
	show_breakpoints();
	show_buttons(pc, inputaddr, lineoffset, is_jump, io);
}

void Debugger::show_disassembly(u16 pc, int& lineoffset)
{
	char text[TEXTSIZE] = { 0 };
	u16 currpc = pc;

	std::vector<std::string> vdasm;
	ImU32 tablerowcolor = 0xff00ffff;
	ImU32 tablecolcolor = 0xffe0e0e0;

	ImGuiIO& io = ImGui::GetIO();

	//Show Debugger
	ImGui::Begin("Debugger", NULL, 0);

	if (io.MouseWheel != 0 && (pc + lineoffset < 0x10000))
	{
		if (ImGui::IsWindowHovered())
		{
			if (io.MouseWheel > 0)
				lineoffset -= 6;
			else
				lineoffset += 6;
		}
	}

	if (lineoffset == 0)
		ImGui::SetScrollHereY(1);

	if (ImGui::GetWindowPos().y < 65)
		ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x, 65));

	if (ImGui::GetWindowPos().x < 0)
		ImGui::SetWindowPos(ImVec2(0, ImGui::GetWindowPos().x));

	if (ImGui::BeginTable("", 4))
	{
		ImGui::TableSetupColumn("checkbox", ImGuiTableColumnFlags_WidthFixed, 20.0f);
		ImGui::TableSetupColumn("offset", ImGuiTableColumnFlags_WidthFixed, 40.0f);
		ImGui::TableSetupColumn("disasmtext", ImGuiTableColumnFlags_WidthFixed, 120);
		ImGui::TableSetupColumn("bytes", ImGuiTableColumnFlags_WidthFixed, 110);

		for (int i = 0; i < 37; i++)
		{
			int pcaddr = pc + lineoffset;
			std::vector<disasmentry> vdentry = disasm(text, pcaddr);

			if (pcaddr == currpc)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, tablerowcolor);

			ImGui::TableNextRow();

			bool bpcheck = false;

			for (auto& it : bpk->get_breakpoints())
			{
				if (it.addr == pcaddr)
				{
					bpcheck = true;
					break;
				}
			}

			ImGui::TableNextColumn();

			ImGui::PushID(pc);

			if (ImGui::Checkbox("##checkboxbp", &bpcheck))
			{
				if (bpcheck)
				{
					bpk->add(pcaddr, bp_exec);
				}
				else
				{
					bpk->remove(pcaddr);
				}
			}

			ImGui::PopID();

			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, pcaddr == currpc ? tablerowcolor : tablecolcolor);
			ImGui::TableNextColumn(); ImGui::Text("%04X", vdentry[0].offset);
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, pcaddr == currpc ? tablerowcolor : tablecolcolor);
			ImGui::TableNextColumn(); ImGui::Text(vdentry[0].dtext.c_str());
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, pcaddr == currpc ? tablerowcolor : 0xffffffff);
			ImGui::TableNextColumn(); ImGui::Text(vdentry[0].bytetext.c_str());
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, pcaddr == currpc ? tablerowcolor : tablecolcolor);

			pc += vdentry[0].size;

			if (stepping)
			{
				ImGui::SetScrollHereY(0);
				stepping = false;
			}

			if (pcaddr == currpc)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, tablerowcolor);
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

void Debugger::show_registers()
{
	ImU32 tablecolcolor = 0xffe0e0e0;

	ImGui::Begin("Registers", NULL, 0);

	if (ImGui::BeginTable("Regs", 2))
	{
		if (ImGui::GetWindowPos().y < 65)
			ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x, 65));

		if (ImGui::GetWindowPos().x < 0)
			ImGui::SetWindowPos(ImVec2(0, ImGui::GetWindowPos().y));

		std::vector<sregdata> reginfo = cpu->get_reginfo();

		char flags[9] = { "........" };
		char text[32] = "";

		flags[7] = cpu->r.f & FC ? 'C' : '.';
		flags[6] = cpu->r.f & FN ? 'N' : '.';
		flags[5] = cpu->r.f & FP ? 'P' : '.';
		flags[4] = cpu->r.f & FX ? 'X' : '.';
		flags[3] = cpu->r.f & FH ? 'H' : '.';
		flags[2] = cpu->r.f & FY ? 'Y' : '.';
		flags[1] = cpu->r.f & FZ ? 'Z' : '.';
		flags[0] = cpu->r.f & FS ? 'S' : '.';

		ImGui::TableSetupColumn("regnames", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableSetupColumn("regvalues", ImGuiTableColumnFlags_WidthFixed, 80);

		ImGui::TableNextColumn(); ImGui::Text("cycles");
		ImGui::TableNextColumn(); ImGui::Text("%d", *cpu->get_cycles());

		ImGui::TableNextColumn(); ImGui::Text("flags");
		ImGui::TableNextColumn(); ImGui::Text("%s", flags);

		ImGui::TableNextColumn(); ImGui::Text("------------");
		ImGui::TableNextColumn(); ImGui::Text("-----------");

		//ImGui::TableNextColumn(); ImGui::Text("%11s", "PC");
		//ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, tablecolcolor);

		for (int i = 0; i < 19; i++)
		{
			ImGui::TableNextColumn(); ImGui::Text("%11s", reginfo[i].name.c_str());
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, tablecolcolor);

			if (i < 12)
			{
				ImGui::TableNextColumn(); ImGui::Text("%04X", get_reg_value(i));
			}
			else if (i < 15)
			{
				ImGui::TableNextColumn(); ImGui::Text("%02X", (u8)get_reg_value(i));
			}
			else
			{
				ImGui::TableNextColumn(); ImGui::Text("%d", (int)get_reg_value(i));
			}
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

void Debugger::show_memory()
{
	//Show Memory Editor
	//std::vector<u8> ram = mem->get_ram();

	static MemoryEditor mem_edit;
	mem_edit.DrawWindow("Memory", mem->ram.data(), mem->ram.size());
}

void Debugger::show_breakpoints()
{
	static u16 bplistaddr = 0;
	static bool bpaddchkbox[3] = { false };
	static char bpaddrtext[5] = { 0 };
	static ImVec4 checkcolor[3] = { };
	static bool openpopup = false;
	static bool edit_breakpoint = false;

	if (ImGui::Begin("Breakpoints", NULL, 0))
	{
		if (ImGui::GetWindowPos().y < 65)
			ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x, 65));
		if (ImGui::GetWindowPos().x < 0)
			ImGui::SetWindowPos(ImVec2(0, ImGui::GetWindowPos().y));

		checkcolor[0] = ImVec4(255, 255, 255, 255);
		checkcolor[1] = ImVec4(255, 255, 255, 255);
		checkcolor[2] = ImVec4(255, 255, 255, 255);

		if (bpaddchkbox[0])
			checkcolor[0] = ImVec4(0, 95 / 255.0f, 184 / 255.0f, 1);
		if (bpaddchkbox[1])
			checkcolor[1] = ImVec4(0, 95 / 255.0f, 184 / 255.0f, 1);
		if (bpaddchkbox[2])
			checkcolor[2] = ImVec4(0, 95 / 255.0f, 184 / 255.0f, 1);

		//std::ostringstream ss;
		//ss << std::setw(4) << std::setfill('0') << bplistaddr;
		//bpaddrtext = ss.str();

		ImGui::Text("Address:");
		ImGui::SameLine();
		ImGui::PushItemWidth(55);
		ImGui::InputText("##bpadd", bpaddrtext, IM_ARRAYSIZE(bpaddrtext));
		ImGui::PopItemWidth();

		ImGui::Checkbox("R", &bpaddchkbox[0]);

		ImGui::SameLine(0, 15);

		ImGui::Checkbox("W", &bpaddchkbox[1]);

		ImGui::SameLine(0, 15);

		ImGui::Checkbox("X", &bpaddchkbox[2]);

		if (strlen(bpaddrtext) == 0)
			ImGui::BeginDisabled(true);

		if (ImGui::Button("Add", ImVec2(50, 0)))
		{
			u8 bptype = 0;
			for (int i = 0; i < 3; i++)
			{
				if (bpaddchkbox[i])
				{
					bptype |= 1 << i;
				}
			}

			bplistaddr = std::stoi(bpaddrtext, nullptr, 16);
			bpk->add(bplistaddr, bptype);
		}

		if (strlen(bpaddrtext) == 0)
			ImGui::EndDisabled();

		ImGui::SameLine();

		bool disable_buttons = bpk->get_breakpoints().size() == 0;

		if (disable_buttons)
			ImGui::BeginDisabled(true);

		if (ImGui::Button("Delete", ImVec2(50, 0)))
		{
			for (auto& it : bpk->get_breakpoints())
			{
				if (it.addr == bplistaddr)
				{
					bpk->remove(bplistaddr);
				}
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Edit", ImVec2(50, 0)))
		{
			u8 bptype = 0;
			s16 newaddr = -1;

			for (int i = 0; i < 3; i++)
			{
				if (bpaddchkbox[i])
					bptype |= 1 << i;
			}

			std::stringstream str(bpaddrtext);
			str >> std::hex >> newaddr;

			bpk->edit(bplistaddr, bptype, newaddr);
		}

		if (disable_buttons)
			ImGui::EndDisabled();

		if (ImGui::ListBoxHeader("", ImVec2(-1, ImGui::GetWindowHeight() - 90)))
		{
			int n = 0;

			for (auto it : bpk->get_breakpoints())
			{
				char temp[16];
				char ctype[5] = { 0 };
				ctype[0] = it.enabled ? 'E' : '-';
				ctype[1] = it.type & bp_read ? 'R' : '-';
				ctype[2] = it.type & bp_write ? 'W' : '-';
				ctype[3] = it.type & bp_exec ? 'X' : '-';
				snprintf(temp, sizeof(temp), "%04X %s", it.addr, ctype);

				ImGui::PushID(n);

				bool selected = (item_id == n);

				if (ImGui::Selectable(temp, selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					bplistaddr = it.addr;
					item_id = n;

					for (int i = 0; i < 3; i++)
					{
						if (it.type & (1 << i))
							bpaddchkbox[i] = true;
						else
							bpaddchkbox[i] = false;
					}

					snprintf(bpaddrtext, 5, "%04X", bplistaddr);

					if (ImGui::IsMouseDoubleClicked(0))
					{
						bpk->remove(it.addr);
					}

				}

				if (selected)
					ImGui::SetItemDefaultFocus();

				ImGui::PopID();

				n++;
			}
			ImGui::ListBoxFooter();
		}
	}
	ImGui::End();
}

void Debugger::show_buttons(u16 pc, u16& inputaddr, int& lineoffset, bool& is_jump, ImGuiIO io)
{
	static char inputtext[5] = "";
	static char testtext[2] = "";

	if (ImGui::Begin("Buttons", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(APP_WIDTH, 60));

		if (ImGui::Button("Run", ImVec2(80, 0)))
		{
			cpu->step();
			stepping = false;
			lineoffset = -6;
			is_jump = false;
			*cpu->get_state() = cstate::running;
			std::vector<std::string>().swap(test_result);
		}

		ImGui::SameLine();

		if (ImGui::Button("Reset", ImVec2(80, 0)))
		{
#if RUN_TESTS
			mem->load_test();
			mem->set_test_number(test_number);
#else
			cpu->reset();
			sdl->decode_graphics(mem);
#endif // RUN_TESTS


			lineoffset = 0;
			is_jump = false;
			cpu->set_state(cstate::debugging);
			//std::vector<std::string>().swap(test_result);
		}

		ImGui::SameLine();

		if (ImGui::Button("Step Into", ImVec2(80, 0)))
		{
			cpu->step();
			stepping = true;
			lineoffset = -6;
			is_jump = false;
			cpu->set_state(cstate::debugging);

#if RUN_TESTS
			get_test_messages();
#endif // RUN_TESTS
		}

		ImGui::SameLine();

#if RUN_TESTS
		ImGui::BeginDisabled(true);
#endif

		if (ImGui::Button("Step Over", ImVec2(80, 0)))
		{
			u8 op = mem->rb(pc);
			u8 b1 = mem->rb(pc + 1);
			disasmdata dasm = get_disasm_entry(op, pc);

			if (strstr(dasm.name, "djnz") || strstr(dasm.name, "call") || strstr(dasm.name, "rst"))
			{
				u16 prevpc = pc;
				if (op == 0xc3)
					cpu->step();
				else
				{
					u16 retpc = pc + dasm.size;
					cpu->set_state(cstate::running);

					while (cpu->r.pc != retpc)
					{
						cpu->step();

						gui(io);

						if (*cpu->get_state() != cstate::running)
							break;
					}
				}
			}
			else
				cpu->step();

			cpu->set_state(cstate::debugging);
			stepping = true;
			lineoffset = -6;
			is_jump = false;
		}

#if RUN_TESTS
		ImGui::EndDisabled();
#endif

		ImGui::SameLine();

		std::string log = logging ? "Logging: On" : "Logging: Off";

		if (ImGui::Button(log.c_str(), ImVec2(100, 0)))
		{
			logging = !logging;

			if (logging)
				outFile.open("cpu_trace.log");
			else
				outFile.close();
			//if (!logging)
			//{
			//	std::ofstream outFile("cpu_trace.log");
			//	for (const auto& e : vdentry)
			//	{
			//		outFile
			//			<< e.regtext
			//			<< std::uppercase << std::hex << std::setw(4) << std::setfill('0') << e.offset
			//			<< ": "
			//			<< e.dtext
			//			<< "\n";
			//	}
			//	std::vector<disasmentry>().swap(vdentry);
			//}
		}

		ImGui::SameLine();

		if (ImGui::Button("Dump Memory", ImVec2(100, 0)))
		{
			std::ofstream outFile("ram.bin", std::ios::binary);
			outFile.write((char*)mem->ram.data(), mem->ram.size());
			outFile.close();
		}

		ImGui::SameLine();

		ImGui::PushItemWidth(40);

		if (ImGui::InputText("Jump To Address", inputtext, IM_ARRAYSIZE(inputtext), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::istringstream ss(inputtext);
			ss >> std::hex >> inputaddr;
			is_jump = true;
			lineoffset = 0;
		}

		ImGui::PopItemWidth();

#if RUN_TESTS
		ImGui::SameLine();

		static u8 testaddr = 0;

		ImGui::PushItemWidth(40);

		if (ImGui::InputText("Run Test Number", testtext, IM_ARRAYSIZE(inputtext), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			mem->load_test();

			std::istringstream ss(testtext);
			ss >> test_number;

			mem->set_test_number(test_number);

			cpu->resstr = "";
		}

		ImGui::PopItemWidth();
#endif

		ImGui::SameLine(0, 5);

		ImGui::Checkbox("Show Tiles/Sprites", &showtiles);

		ImGui::SameLine(0, 50);

		ImGui::Text("%s: %f", "FPS", io.Framerate);

		ImGui::End();
	}
}

void Debugger::logger(u16 pc)
{
	char text[TEXTSIZE] = { 0 };

#if RUN_TESTS
	//if (cpu->regt.pc == 0x1d44 || cpu->regt.pc == 0x1d42)
		//vdentry.push_back(disasm(text, pc, true)[0]);
#else
	vdentry.push_back(disasm(text, pc, true)[0]);
#endif // RUN_TESTS

	//if (vdentry.size() > 0x1000000)
	//	logging = false;
}

std::vector<disasmentry> Debugger::disasm(const char* text, u16 pc, bool get_registers)
{
	u8 op = mem->rb(pc);

	int size = 0;
	int asize = 0;
	const char* name;
	const char* oper;
	char line[TEXTSIZE] = { 0 };
	char bytes[TEXTSIZE] = { 0 };
	char* data = (char*)text;

	std::vector<disasmentry> entry;
	disasmdata dops;

	dops = get_disasm_entry(op, pc);

	name = dops.name;
	size = dops.size;
	oper = dops.operand;

	disasmentry e;

	if (size == 1)
	{
		if (strstr(oper, "N/A"))
			snprintf(data, TEXTSIZE, "%s", name);
		else
			snprintf(data, TEXTSIZE, "%-4s %s", name, oper);

		snprintf(bytes, TEXTSIZE, "%02X", op);
	}
	else if (size == 2)
	{
		u8 b1 = mem->rb(pc + 1);
		if (strstr(oper, "$%02X%02X"))
		{
			u16 offset = pc + (s8)b1 + 2;
			snprintf(data, TEXTSIZE, "%-5s", name);
			snprintf(data + strlen(data), TEXTSIZE, oper, offset >> 8, offset & 0xff);
		}
		else if (strstr(oper, "$%02X"))
		{
			snprintf(data, TEXTSIZE, "%-5s", name);
			snprintf(data + strlen(data), TEXTSIZE, oper, b1);
		}
		else if (strstr(oper, "N/A"))
			snprintf(data, TEXTSIZE, "%s", name);
		else
			snprintf(data, TEXTSIZE, "%-4s %s", name, oper);

		snprintf(bytes, TEXTSIZE, "%02X %02X", op, b1);
	}
	else if (size == 3)
	{
		u8 b1 = mem->rb(pc + 1);
		u8 b2 = mem->rb(pc + 2);

		if (strstr(oper, "$%02X%02X"))
		{
			snprintf(data, TEXTSIZE, "%-5s", name);
			snprintf(data + strlen(data), TEXTSIZE, oper, b2, b1);
		}
		else if (strstr(oper, "$%02X"))
		{
			snprintf(data, TEXTSIZE, "%-5s", name);
			snprintf(data + strlen(data), TEXTSIZE, oper, b2);
		}
		else
			snprintf(data, TEXTSIZE, "%-4s %s$%02X%02X", name, oper, b2, b1);

		snprintf(bytes, TEXTSIZE, "%02X %02X %02X", op, b1, b2);
	}
	else if (size == 4)
	{
		u8 b1 = mem->rb(pc + 1);
		u8 b2 = mem->rb(pc + 2);
		u8 b3 = mem->rb(pc + 3);

		if (strstr(oper, "$%02X%02X"))
		{
			snprintf(data, TEXTSIZE, "%-5s", name);
			snprintf(data + strlen(data), TEXTSIZE, oper, b3, b2);
		}
		else if (strstr(oper, "$%02X"))
		{
			snprintf(data, TEXTSIZE, "%-5s", name);
			snprintf(data + strlen(data), TEXTSIZE, oper, b2, b3);
		}
		else
			snprintf(data, TEXTSIZE, "%-4s %s$%02X%02X", name, oper, b2, b1);

		snprintf(bytes, TEXTSIZE, "%02X %02X %02X %02X", op, b1, b2, b3);
	}

	if (strstr(name, "pre"))
	{
		e.size = 2;
		snprintf(data, TEXTSIZE, "%-5s", name);
	}

	if (get_registers)
	{
		std::vector<sregdata> reginfo = cpu->get_reginfo();
		char temp[TEXTSIZE] = { 0 };
		for (int i = 1; i < 8; i++)
		{
			snprintf(temp, TEXTSIZE, reginfo[i].format.c_str(), reginfo[i].name, get_reg_value(i));
			e.regtext += temp;
		}
	}

	e.name = name;
	e.offset = pc;
	e.size = size;
	e.dtext = data;
	e.bytetext = bytes;
	entry.push_back(e);

	return entry;
}

disasmdata Debugger::get_disasm_entry(u8 op, u16 pc)
{
	disasmdata dops;

	if (op == 0xcb || op == 0xdd || op == 0xed || op == 0xfd)
	{
		u16 dop = mem->rw(pc);

		if (dop == 0xcbdd)
		{
			u8 b3 = mem->rw(pc + 3);
			dops = disasm_ddcb[b3];
		}
		else if (dop == 0xcbfd)
		{
			u8 b3 = mem->rw(pc + 3);
			dops = disasm_fdcb[b3];
		}
		else
		{
			u8 b1 = mem->rw(pc + 1);
			dops = mdisasm[op][b1];
		}
	}
	else
	{
		dops = mdisasm[0x00][op];
	}

	return dops;
}

u16 Debugger::get_reg_value(int i)
{
	switch (i)
	{
	case 0: return cpu->r.pc;
	case 1: return cpu->r.sp;
	case 2: return cpu->r.af;
	case 3: return cpu->r.bc;
	case 4: return cpu->r.de;
	case 5: return cpu->r.hl;
	case 6: return cpu->r.ix;
	case 7: return cpu->r.iy;
	case 8: return cpu->r.saf;
	case 9: return cpu->r.sbc;
	case 10: return cpu->r.sde;
	case 11: return cpu->r.shl;
	case 12: return cpu->r.r;
	case 13: return cpu->r.i;
		//case 14: return cpu->get_im();
		//case 15: return cpu->regt.shl;
	}
	return 0;
}

void Debugger::get_test_messages()
{
	if (cpu->r.pc == 0x0000)
	{
		cpu->set_state(cstate::debugging);
	}
	else if (cpu->r.pc == 0x002b)
	{
		char c = mem->rb(0xffff);
		cpu->resstr += c;

		switch (cpu->r.c)
		{
		case 2:
			//crcstr += (char)cpu->r.e;
			break;
		case 9:
			u16 addr = cpu->r.de;

			//if (addr == 0x1dda)
			//	return;

			if (addr == 0x1df6)
				cpu->set_state(cstate::debugging);

			//while (1)
			//{
			//	char c = mem->rb(addr);

			//	if (c == '$')
			//		break;

			//	resstr += c;

			//	addr++;

			//}
			break;
		}

		//if (resstr != "")
		//	test_result.push_back(resstr);
		//if (cpu->crcstr != "")
		//	test_result.push_back(crcstr);
	}
}