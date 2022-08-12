#include <sstream>

#include "core/cpu.h"
#include "core/emulator.h"

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

void CPU::Clock()
{
  Fetch();
}

void CPU::Fetch()
{
  instruction_ = Read(PC_);
  Decode();
}

void CPU::Decode()
{
  if (IsALU()) ALU();
  else if (IsHALT()) HALT();
  else if (IsLOAD()) LOAD();
  else if (IsLOADI()) LOADI();
  else if (IsSTORE()) STORE();
  else if (IsSTOREI()) STOREI();
  else if (IsCALL()) CALL();
  else if (IsJMP()) JMP();
  else if (IsMOVI()) MOVI();
  else if (IsMOV()) MOV();
  else NOP();
}

void CPU::HALT()
{
  ++PC_;
  main_bus_->StopClock();
}

void CPU::NOP()
{
  ++PC_;
}

void CPU::LOAD()
{
  uint8_t G = (instruction_ & 0x0700) >> 8;
  uint8_t P = instruction_ & 0x0007;

  ++PC_;

  if (IsAddressRegister(P))
  {
    SetRegister(G, Read(GetRegister(P)) & 0x00FF);
  }
}

void CPU::LOADI()
{
  uint8_t G = (instruction_ & 0x0700) >> 8;
  uint8_t Imm = instruction_ & 0x00FF;

  ++PC_;
  SetRegister(G, Read(Imm));
}

void CPU::STORE()
{
  uint8_t G = (instruction_ & 0x0700) >> 8;
  uint8_t P = instruction_ & 0x0007;

  ++PC_;
  if (IsAddressRegister(P))
  {
    Write(GetRegister(P), GetRegister(G));
  }
}

void CPU::STOREI()
{
  uint8_t G = (instruction_ & 0x0700) >> 8;
  uint8_t Imm  = instruction_ & 0x00FF;

  ++PC_;
  Write(Imm, GetRegister(G));
}

void CPU::CALL()
{
  uint8_t cond = (instruction_ & 0x7000) >> 12;
  uint8_t Imm = instruction_ & 0x00FF;

  if (CheckCondition(cond))
  {
    SetRegister(kL, PC_ + 1);
    SetRegister(kPC, Imm);
  }
  else
  {
    ++PC_;
  }
}

void CPU::JMP()
{
  uint8_t cond = (instruction_ & 0x7000) >> 12;
  uint8_t Imm = instruction_ & 0x00FF;

  if (CheckCondition(cond))
  {
    SetRegister(kPC, Imm);
  }
  else
  {
    ++PC_;
  }
}

void CPU::MOVI()
{
  uint8_t cond = (instruction_ & 0x7000) >> 12;
  uint8_t Gd = (instruction_ & 0x0700) >> 8;
  uint8_t Imm = instruction_ & 0x00FF;

  ++PC_;
  if (CheckCondition(cond))
  {
    SetRegister(Gd, Imm);
  }
}

void CPU::MOV()
{
  uint8_t Gd = (instruction_ & 0x0700) >> 8;
  uint8_t Gs = (instruction_ & 0x0070) >> 4;

  ++PC_;
  SetRegister(Gd, GetRegister(Gs));
}

void CPU::ALU()
{
  bool bIsUnaryALU = (instruction_ & 0x7800) == 0x7800;

  ++PC_;

  uint8_t res;
  bool i = (instruction_ & 0x0080) >> 7;
  bool r = (instruction_ & 0x0008) >> 3;

  if (bIsUnaryALU) UnaryAlU();
  else BinaryALU();
}

void CPU::BinaryALU()
{
  uint8_t code = (instruction_ & 0x3800) >> 11;
  uint8_t Gd = (instruction_ & 0x0700) >> 8;
  uint8_t Gs1 = (instruction_ & 0x0070) >> 4;
  uint8_t Op2 = (instruction_ & 0x0007);
  bool r = (instruction_ & 0x0008) >> 3;
  bool i = (instruction_ & 0x0080) >> 7;

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

void CPU::UnaryAlU()
{
  uint8_t code = (instruction_ & 0x0006) >> 1;
  uint8_t Gd = (instruction_ & 0x0700) >> 8;
  uint8_t Gs = (instruction_ & 0x0070) >> 4;
  bool r = (instruction_ & 0x0008) >> 3;

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
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::ADD(uint8_t Gs1, uint8_t Op2)
{
  uint8_t res = (Gs1 + Op2) & 0xFF;

  SetFlag(Flag::kCY, (Gs1 + Op2) >> 8);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::SBC(uint8_t Gs1, uint8_t Op2)
{
  uint8_t res = (Gs1 - Op2 - GetFlag(Flag::kCY)) & 0xFF;

  SetFlag(Flag::kCY, ((Gs1 - Op2 - GetFlag(Flag::kCY)) >> 8) & 0x1);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::SUB(uint8_t Gs1, uint8_t Op2)
{
  uint8_t res = (Gs1 - Op2) & 0xFF;

  SetFlag(Flag::kCY, ((Gs1 - Op2) >> 8) & 0x1);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::AND(uint8_t Gs1, uint8_t Op2)
{
  uint8_t res = Gs1 & Op2;

  SetFlag(Flag::kCY, 0);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::OR(uint8_t Gs1, uint8_t Op2)
{
  uint8_t res = Gs1 | Op2;

  SetFlag(Flag::kCY, 0);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::XOR(uint8_t Gs1, uint8_t Op2)
{
  uint8_t res = Gs1 ^ Op2;

  SetFlag(Flag::kCY, 0);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::NOT(uint8_t Gs)
{
  uint8_t res = ~Gs;

  SetFlag(Flag::kCY, 0);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::ROR(uint8_t Gs)
{
  uint8_t res = Gs >> 1;
  res |= Gs << 7;

  SetFlag(Flag::kCY, 0);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::SHR(uint8_t Gs)
{
  uint8_t res = Gs >> 1;

  SetFlag(Flag::kCY, Gs & 0x1);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

uint8_t CPU::RCR(uint8_t Gs)
{
  uint8_t res = Gs >> 1;
  res |= GetFlag(Flag::kCY) << 7;

  SetFlag(Flag::kCY, Gs & 0x1);
  SetFlag(Flag::kZ, res == 0);
  SetFlag(Flag::kS, res >> 7);

  return res;
}

void CPU::Reset()
{
  A_ = 0x00;
  B_ = 0x00;
  C_ = 0x00;
  D_ = 0x00;
  M_ = 0x00;
  S_ = 0x00;
  L_ = 0x00;
  PC_ = 0x00;

  sign_ = false;
  zero_ = false;
  carry_ = false;

  is_halted_ = false;
}

uint16_t CPU::Read(uint8_t addr)
{
  return main_bus_->Read(addr);
}

void CPU::Write(uint8_t addr, uint8_t value)
{
  return main_bus_->Write(addr, value);
}

uint8_t CPU::GetRegister(uint8_t code) const
{
  switch (RegisterCode(code & 0x07))
  {
    case kA: return A_;
    case kB: return B_;
    case kC: return C_;
    case kD: return D_;
    case kM: return M_;
    case kS: return S_;
    case kL: return L_;
    case kPC: default: return PC_;
  }
}

void CPU::SetRegister(uint8_t code, uint8_t value)
{
  switch (RegisterCode(code & 0x07))
  {
    case kA: A_ = value; break;
    case kB: B_ = value; break;
    case kC: C_ = value; break;
    case kD: D_ = value; break;
    case kM: M_ = value; break;
    case kS: S_ = value; break;
    case kL: L_ = value; break;
    case kPC: default: PC_ = value; break;
  }
}

bool CPU::GetFlag(Flag flag) const
{
  switch(flag)
  {
    case Flag::kCY: return carry_;
    case Flag::kZ: return zero_;
    default: return sign_;
  }
}

void CPU::SetFlag(Flag flag, bool value)
{
  switch(flag)
  {
    case Flag::kCY: carry_ = value; break;
    case Flag::kZ: zero_ = value; break;
    default: sign_ = value; break;
  }
}

std::string CPU::Disassemble(uint16_t instruction)
{
  const std::string kUnknown = "Unknown";

  auto GetRegisterName = [](uint8_t code) -> std::string
  {
    switch (RegisterCode(code))
    {
      case kA: return "A";
      case kB: return "B";
      case kC: return "C";
      case kD: return "D";
      case kM: return "M";
      case kS: return "S";
      case kL: return "L";
      default: return "PC";
    }
  };

  auto GetConditionName = [](uint8_t code) -> std::string
  {
    switch (code)
    {
      case 0b000: return "A";
      case 0b001: return "Z";
      case 0b010: return "NS";
      case 0b011: return "C";
      case 0b100: return "NC";
      case 0b101: return "S";
      case 0b110: return "NZ";
      default: return "";
    }
  };

  auto ToHexString = [](unsigned int val)
  {
    std::stringstream hex;
    hex << "0x" << std::hex << val;

    return hex.str();
  };

  if ((instruction & 0xC000) == kALU)
  {
    if ((instruction & 0x3800) == 0x3800)
    {
      uint8_t code = (instruction & 0x0006) >> 1;
      uint8_t Gd = (instruction & 0x0700) >> 8;
      uint8_t Gs = (instruction & 0x0070) >> 4;
      bool r = (instruction & 0x0008) >> 3;

      std::string instruction_name;

      switch (code)
      {
        case 0: instruction_name = "NOT"; break;
        case 1: instruction_name = "ROR"; break;
        case 2: instruction_name = "SHR"; break;
        case 3: instruction_name = "RCR"; break;
        default: return kUnknown;
      }

      if (r)
      {
        return instruction_name + " F, " + GetRegisterName(Gs);
      }
      else
      {
        return instruction_name + " " + GetRegisterName(Gd) + ", " + GetRegisterName(Gs);
      }
    }
    else
    {
      uint8_t code = (instruction & 0x3800) >> 11;
      uint8_t Gd = (instruction & 0x0700) >> 8;
      uint8_t Gs1 = (instruction & 0x0070) >> 4;
      uint8_t Op2 = (instruction & 0x0007);
      bool r = (instruction & 0x0008) >> 3;
      bool i = (instruction & 0x0080) >> 7;

      std::string instruction_name;

      switch (code)
      {
        case 0: instruction_name = "ADC"; break;
        case 1: instruction_name = "ADD"; break;
        case 2: instruction_name = "SBC"; break;
        case 3: instruction_name = "SUB"; break;
        case 4: instruction_name = "AND"; break;
        case 5: instruction_name = "OR"; break;
        case 6: instruction_name = "XOR"; break;
        default: return kUnknown;
      }

      std::string Op2_name;

      if (i)
      {
        Op2_name = std::to_string(Op2);
      }
      else
      {
        Op2_name = GetRegisterName(Op2);
      }

      if (r)
      {
        return instruction_name + " F, " + GetRegisterName(Gs1) + ", " + Op2_name;
      }
      else
      {
        return instruction_name + " " + GetRegisterName(Gd) + ", " + GetRegisterName(Gs1) + ", " + Op2_name;
      }
    }
  }
  else if (instruction == kHALT)
  {
    return "HALT";
  }
  else if (instruction == kNOP)
  {
    return "NOP";
  }
  else if ((instruction & 0xF800) == kLOAD)
  {
    uint8_t G = (instruction & 0x0700) >> 8;
    uint8_t P = instruction & 0x0007;

    return "LOAD " + GetRegisterName(G) + ", " + GetRegisterName(P);
  }
  else if ((instruction & 0xF800) == kLOADI)
  {
    uint8_t G = (instruction & 0x0700) >> 8;
    uint8_t Imm = instruction & 0x00FF;

    return "LOAD " + GetRegisterName(G) + ", " + ToHexString(Imm);
  }
  else if ((instruction & 0xF800) == kSTORE)
  {
    uint8_t G = (instruction & 0x0700) >> 8;
    uint8_t P = instruction & 0x0007;

    return "STORE " + GetRegisterName(G) + ", " + GetRegisterName(P);
  }
  else if ((instruction & 0xF800) == kSTOREI)
  {
    uint8_t G = (instruction & 0x0700) >> 8;
    uint8_t Imm  = instruction & 0x00FF;

    return "STORE " + GetRegisterName(G) + ", " + std::to_string(Imm);
  }
  else if ((instruction & 0x8F00) == kCALL)
  {
    uint8_t cond = (instruction & 0x7000) >> 12;
    uint8_t Imm = instruction & 0x00FF;

    std::string cond_name = GetConditionName(cond);

    if (cond_name == "A")
    {
      return "CALL " + ToHexString(Imm);
    }
    else if (cond_name == "")
    {
      return kUnknown;
    }
    else
    {
      return "CALL " + cond_name + ", " + ToHexString(Imm);
    }
  }
  else if ((instruction & 0x8700) == kJMP)
  {
    uint8_t cond = (instruction & 0x7000) >> 12;
    uint8_t Imm = instruction & 0x00FF;

    std::string cond_name = GetConditionName(cond);

    if (cond_name == "A")
    {
      return "JMP " + ToHexString(Imm);
    }
    else if (cond_name == "")
    {
      return kUnknown;
    }
    else
    {
      return "JMP " + cond_name + ", " + ToHexString(Imm);
    }
  }
  else if ((instruction & 0x8000) == kMOVI)
  {
    uint8_t cond = (instruction & 0x7000) >> 12;
    uint8_t Gd = (instruction & 0x0700) >> 8;
    uint8_t Imm = instruction & 0x00FF;

    std::string cond_name = GetConditionName(cond);

    if (cond_name == "A")
    {
      return "MOVI " + GetRegisterName(Gd) + ", " + ToHexString(Imm);
    }
    else if (cond_name == "")
    {
      return kUnknown;
    }
    else
    {
      return "MOVI " + cond_name + ", " + GetRegisterName(Gd) + ", " + ToHexString(Imm);
    }
  }
  else if ((instruction & 0x1800) == kMOV)
  {
    uint8_t Gd = (instruction & 0x0700) >> 8;
    uint8_t Gs = (instruction & 0x0070) >> 4;

    return "MOV " + GetRegisterName(Gd) + ", " + GetRegisterName(Gs);
  }
  else
  {
    return kUnknown;
  }
}
