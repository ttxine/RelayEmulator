#include <sstream>

#include "core/cpu.h"
#include "core/emulator.h"

void CPU::Clock()
{
  // Each instruction requires 10 clock ticks, but only the result is important
  // here, so all 10 of these are executed at a time.

  Fetch();
}

void CPU::Fetch()
{
  instruction_ = Read(PC_);
  Decode();
}

void CPU::Decode()
{
  if (is_ALU(instruction_)) ALU();
  else if (is_HALT(instruction_)) HALT();
  else if (is_LOAD(instruction_)) LOAD();
  else if (is_LOADI(instruction_)) LOADI();
  else if (is_STORE(instruction_)) STORE();
  else if (is_STOREI(instruction_)) STOREI();
  else if (is_CALL(instruction_)) CALL();
  else if (is_JMP(instruction_)) JMP();
  else if (is_MOVI(instruction_)) MOVI();
  else if (is_MOV(instruction_)) MOV();
  else NOP();
}

void CPU::HALT()
{
  ++PC_;
  bus_->StopClock();
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

void CPU::Reset() noexcept
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

uint16_t CPU::Read(uint8_t addr) noexcept
{
  return bus_->Read(addr);
}

void CPU::Write(uint8_t addr, uint8_t value) noexcept
{
  return bus_->Write(addr, value);
}

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
    case Flag::kS: return sign_;
    default: return false;
  }
}

void CPU::SetFlag(Flag flag, bool value)
{
  switch(flag)
  {
    case Flag::kCY: carry_ = value; break;
    case Flag::kZ: zero_ = value; break;
    case Flag::kS: sign_ = value; break;
    default: break;
  }
}
