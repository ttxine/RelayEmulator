#include <exception>

#include "cpu.h"

namespace relay
{
  bool& CPU::GetSetFlag(Flag f)
  {
    if (f == Flag::kCY) return carry_;
    else if (f == Flag::kZ) return zero_;
    else return sign_;
  }

  bool CPU::GetFlag(Flag f) { return GetSetFlag(f); }

  void CPU::SetFlag(Flag f, bool v) { GetSetFlag(f) = v; }

  bool CPU::CheckCondition(uint8_t cond)
  {
    switch (cond)
    {
      case 0b000: return true;
      case 0b001: return GetFlag(Flag::kZ);
      case 0b010: return !GetFlag(Flag::kS);
      case 0b011: return GetFlag(Flag::kCY);
      case 0b100: return !GetFlag(Flag::kCY);
      case 0b101: return GetFlag(Flag::kS);
      case 0b110: return !GetFlag(Flag::kZ);
      default: return false;
    }
  }

  uint8_t& CPU::GetSetRegister(uint8_t code)
  {
    switch (Register(code))
    {
      case kA: return A_;
      case kB: return B_;
      case kC: return C_;
      case kD: return D_;
      case kM: return M_;
      case kS: return S_;
      case kL: return L_;
      case kPC: return PC_;
    }
    throw std::exception();
  }

  uint8_t CPU::GetRegister(uint8_t code) { return GetSetRegister(code); }

  void CPU::SetRegister(uint8_t code, uint8_t value)
  {
    GetSetRegister(code) = value;
  }

  uint16_t CPU::Read(uint8_t addr)
  {
    if (addr >= 0x00 && addr <= 0x7F)
      return memory_->ReadROM(addr);
    else if (addr >= 0x80 && addr <= 0x8F)
      return memory_->ReadInput(addr);
    else if (addr >= 0x90 && addr <= 0xFF)
      return memory_->ReadUnused(addr);
    else
      return 0x00;
  }

  void CPU::Write(uint8_t addr, uint8_t value)
  {
    if (addr >= 0x00 && addr <= 0x7F)
      return;
    else if (addr >= 0x80 && addr <= 0x8F)
      return;
    else if (addr >= 0x90 && addr <= 0xFF)
      memory_->WriteUnused(addr, value);
  }

  uint16_t CPU::Fetch()
  {
    uint8_t PC = GetRegister(kPC);
    return Read(PC);
  }

  void CPU::Decode(uint16_t instruction)
  {
    if ((instruction & 0xC000) == kALU_Op)
    {
      ALUOperation(instruction);
      ++PC_;
    }
    else if (instruction == kHALT)
    {
      HALT();
    }
    else if (instruction == kNOP)
    {
      NOP();
      ++PC_;
    }
    else if ((instruction & 0xF800) == kLOAD)
    {
      uint8_t G = (instruction & 0x0700) >> 8;
      uint8_t P = instruction & 0x0007;

      LOAD(G, P);
      ++PC_;
    }
    else if ((instruction & 0xF800) == kLOAD_Imm)
    {
      uint8_t G = (instruction & 0x0700) >> 8;
      uint8_t Imm = instruction & 0x00FF;

      LOAD_Imm(G, Imm);
      ++PC_;
      std::cout << PC_;
    }
    else if ((instruction & 0xF800) == kSTORE)
    {
      uint8_t G = (instruction & 0x0700) >> 8;
      uint8_t P = instruction & 0x0007;

      STORE(G, P);
      ++PC_;
    }
    else if ((instruction & 0xF800) == kSTORE_Imm)
    {
      uint8_t G = (instruction & 0x0700) >> 8;
      uint8_t Imm  = instruction & 0x00FF;

      STORE_Imm(G, Imm);
      ++PC_;
    }
    else if ((instruction & 0x8F00) == kCALL)
    {
      uint8_t cond = (instruction & 0x7000) >> 12;
      uint8_t Imm = instruction & 0x00FF;

      CALL(cond, Imm);
    }
    else if ((instruction & 0x8700) == kJMP)
    {
      uint8_t cond = (instruction & 0x7000) >> 12;
      uint8_t Imm = instruction & 0x00FF;

      JMP(cond, Imm);
    }
    else if ((instruction & 0x8000) == kMOVI)
    {
      uint8_t cond = (instruction & 0x7000) >> 12;
      uint8_t Gd = (instruction & 0x0700) >> 8;
      uint8_t Imm = instruction & 0x00FF;

      MOVI(cond, Gd, Imm);
      ++PC_;
    }
    else if ((instruction & 0x1800) == kMOV)
    {
      uint8_t Gd = (instruction & 0x0700) >> 8;
      uint8_t Gs = (instruction & 0x0070) >> 4;

      MOV(Gd, Gs);
      ++PC_;
    }
    else {
      NOP();
      ++PC_;
    }
  }

  void CPU::HALT() { is_running_ = false; }

  void CPU::NOP() {}

  void CPU::LOAD(uint8_t G, uint8_t P)
  {
    SetRegister(G, Read(GetRegister(P)) & 0x00FF);
  }

  void CPU::LOAD_Imm(uint8_t G, uint8_t Imm)
  {
    SetRegister(G, Read(Imm) & 0x00FF);
  }

  void CPU::STORE(uint8_t G, uint8_t P)
  {
    Write(GetRegister(P), GetRegister(G));
  }

  void CPU::STORE_Imm(uint8_t G, uint8_t Imm)
  {
    Write(Imm, GetRegister(G));
  }

  void CPU::CALL(uint8_t cond, uint8_t Imm)
  {
    if (CheckCondition(cond))
    {
      SetRegister(kL, GetRegister(kPC) + 1);
      SetRegister(kPC, Imm);
    }
  }

  void CPU::JMP(uint8_t cond, uint8_t Imm)
  {
    if (CheckCondition(cond))
      SetRegister(kPC, Imm);
  }

  void CPU::MOVI(uint8_t cond, uint8_t Gd, uint8_t Imm)
  {
    if (CheckCondition(cond))
      SetRegister(Gd, Imm);
  }

  void CPU::MOV(uint8_t Gd, uint8_t Gs)
  {
    SetRegister(Gd, GetRegister(Gs));
  }

  void CPU::ALUOperation(uint16_t instruction)
  {
    if ((instruction & 0x3800) == 0x3800)
    {
      uint8_t code = (instruction & 0x0006) >> 1;
      uint8_t Gd = (instruction & 0x0700) >> 8;
      uint8_t Gs = (instruction & 0x0070) >> 4;
      bool r = (instruction & 0x0008) >> 3;

      UnaryAlUOperation(code, Gd, Gs, r);
    }
    else
    {
      uint8_t code = (instruction & 0x3800) >> 11;
      uint8_t Gd = (instruction & 0x0700) >> 8;
      uint8_t Gs1 = (instruction & 0x0070) >> 4;
      uint8_t Op2 = (instruction & 0x0007);
      bool r = (instruction & 0x0008) >> 3;
      bool i = (instruction & 0x0080) >> 7;

      BinaryALUOperation(code, Gd, Gs1, Op2, r, i);
    }
  }

  void CPU::BinaryALUOperation(uint8_t code, uint8_t Gd, uint8_t Gs1,
                        uint8_t Op2, bool r, bool i)
  {
    uint8_t res;

    Gs1 = GetRegister(Gs1);
    if (!i) Op2 = GetRegister(Op2);

    switch(code)
    {
      case 0b000: res = ADC(Gs1, Op2); break;
      case 0b001: res = ADD(Gs1, Op2); break;
      case 0b010: res = SBC(Gs1, Op2); break;
      case 0b011: res = SUB(Gs1, Op2); break;
      case 0b100: res = AND(Gs1, Op2); break;
      case 0b101: res = OR(Gs1, Op2); break;
      case 0b110: res = XOR(Gs1, Op2); break;
    }

    if (r) SetRegister(Gd, res);
  }

  void CPU::UnaryAlUOperation(uint8_t code, uint8_t Gd, uint8_t Gs, bool r)
  {
    uint8_t res;

    Gs = GetRegister(Gs);

    switch (code)
    {
      case 0b00: res = NOT(Gs); break;
      case 0b01: res = ROR(Gs); break;
      case 0b10: res = SHR(Gs); break;
      case 0b11: res = RCR(Gs); break;
    }

    if (r) SetRegister(Gd, res);
  }

  uint8_t CPU::ADC(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = (Gs1 + Op2 + GetFlag(Flag::kCY)) & 0xFF;

    SetFlag(Flag::kCY, (Gs1 + Op2 + GetFlag(Flag::kCY)) >> 8);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::ADD(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = (Gs1 + Op2) & 0xFF;

    SetFlag(Flag::kCY, (Gs1 + Op2) >> 8);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::SBC(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = (Gs1 - Op2 - GetFlag(Flag::kCY)) & 0xFF;

    SetFlag(Flag::kCY, ((Gs1 - Op2 - GetFlag(Flag::kCY)) >> 8) & 0x1);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::SUB(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = (Gs1 - Op2) & 0xFF;

    SetFlag(Flag::kCY, ((Gs1 - Op2) >> 8) & 0x1);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::AND(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = Gs1 & Op2;

    SetFlag(Flag::kCY, 0);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::OR(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = Gs1 | Op2;

    SetFlag(Flag::kCY, 0);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::XOR(uint8_t Gs1, uint8_t Op2)
  {
    uint8_t res = Gs1 ^ Op2;

    SetFlag(Flag::kCY, 0);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::NOT(uint8_t Gs)
  {
    uint8_t res = ~Gs;

    SetFlag(Flag::kCY, 0);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::ROR(uint8_t Gs)
  {
    uint8_t res = Gs >> 1;
    res |= Gs << 7;

    SetFlag(Flag::kCY, 0);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::SHR(uint8_t Gs)
  {
    uint8_t res = Gs >> 1;

    SetFlag(Flag::kCY, Gs & 0x1);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }

  uint8_t CPU::RCR(uint8_t Gs)
  {
    uint8_t res = Gs >> 1;
    res |= GetFlag(Flag::kCY) << 7;

    SetFlag(Flag::kCY, Gs & 0x1);
    SetFlag(Flag::kZ, res == 0);
    SetFlag(Flag::kZ, res >> 7);

    return res;
  }
}
