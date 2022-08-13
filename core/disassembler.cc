#include "core/disassembler.h"
#include "core/instructionset.h"
#include "utils/str.h"

const std::string kUnknown = "Unknown";

static std::string get_register_name(uint8_t code);
static std::string get_condition_name(uint8_t code);
static std::string disassemble_HALT(uint16_t instruction);
static std::string disassemble_NOP(uint16_t instruction);
static std::string disassemble_LOAD(uint16_t instruction);
static std::string disassemble_LOADI(uint16_t instruction);
static std::string disassemble_STORE(uint16_t instruction);
static std::string disassemble_STOREI(uint16_t instruction);
static std::string disassemble_CALL(uint16_t instruction);
static std::string disassemble_JMP(uint16_t instruction);
static std::string disassemble_MOVI(uint16_t instruction);
static std::string disassemble_MOV(uint16_t instruction);
static std::string disassemble_ALU(uint16_t instruction);
static std::string disassemble_unary_ALU(uint16_t instruction);
static std::string disassemble_binary_ALU(uint16_t instruction);

std::string disassemble(uint16_t instruction)
{
  if (is_ALU(instruction)) return disassemble_ALU(instruction);
  else if (is_HALT(instruction)) return disassemble_HALT(instruction);
  else if (is_NOP(instruction)) return disassemble_NOP(instruction);
  else if (is_LOAD(instruction)) return disassemble_LOAD(instruction);
  else if (is_LOADI(instruction)) return disassemble_LOADI(instruction);
  else if (is_STORE(instruction)) return disassemble_STORE(instruction);
  else if (is_STOREI(instruction)) return disassemble_STOREI(instruction);
  else if (is_CALL(instruction)) return disassemble_CALL(instruction);
  else if (is_JMP(instruction)) return disassemble_JMP(instruction);
  else if (is_MOVI(instruction)) return disassemble_MOVI(instruction);
  else if (is_MOV(instruction)) return disassemble_MOV(instruction);
  else return kUnknown;
}

static std::string disassemble_ALU(uint16_t instruction)
{
  bool bIsUnaryALU = (instruction & 0x7800) == 0x7800;

  if (bIsUnaryALU)
  {
    return disassemble_unary_ALU(instruction);
  }
  else
  {
    return disassemble_binary_ALU(instruction);
  }
}

static std::string disassemble_HALT(uint16_t instruction)
{
  return "HALT";
}

static std::string disassemble_NOP(uint16_t instruction)
{
  return "NOP";
}

static std::string disassemble_LOAD(uint16_t instruction)
{
  uint8_t G = (instruction & 0x0700) >> 8;
  uint8_t P = instruction & 0x0007;

  return "LOAD " + get_register_name(G) + ", " + get_register_name(P);
}

static std::string disassemble_LOADI(uint16_t instruction)
{
  uint8_t G = (instruction & 0x0700) >> 8;
  uint8_t Imm = instruction & 0x00FF;

  return "LOAD " + get_register_name(G) + ", " + to_hex_string(Imm);
}


static std::string disassemble_STORE(uint16_t instruction)
{
  uint8_t G = (instruction & 0x0700) >> 8;
  uint8_t P = instruction & 0x0007;

  return "STORE " + get_register_name(G) + ", " + get_register_name(P);
}

static std::string disassemble_STOREI(uint16_t instruction)
{
  uint8_t G = (instruction & 0x0700) >> 8;
  uint8_t Imm = instruction & 0x00FF;

  return "STORE " + get_register_name(G) + ", " + to_hex_string(Imm);
}

static std::string disassemble_CALL(uint16_t instruction)
{
  uint8_t cond = (instruction & 0x7000) >> 12;
  uint8_t Imm = instruction & 0x00FF;

  std::string cond_name = get_condition_name(cond);
  if (!cond_name.empty()) cond_name += ", ";

  if (cond_name == "A")
  {
    return "CALL " + to_hex_string(Imm);
  }
  else
  {
    return "CALL " + cond_name + to_hex_string(Imm);
  }
}

static std::string disassemble_JMP(uint16_t instruction)
{
  uint8_t cond = (instruction & 0x7000) >> 12;
  uint8_t Imm = instruction & 0x00FF;

  std::string cond_name = get_condition_name(cond);
  if (!cond_name.empty()) cond_name += ", ";

  if (cond_name == "A")
  {
    return "JMP " + to_hex_string(Imm);
  }
  else
  {
    return "JMP " + cond_name + to_hex_string(Imm);
  }
}

static std::string disassemble_MOVI(uint16_t instruction)
{
  uint8_t cond = (instruction & 0x7000) >> 12;
  uint8_t Gd = (instruction & 0x0700) >> 8;
  uint8_t Imm = instruction & 0x00FF;

  std::string cond_name = get_condition_name(cond);
  if (!cond_name.empty()) cond_name += ", ";

  if (cond_name == "A")
  {
    return "MOVI " + get_register_name(Gd) + ", " + to_hex_string(Imm);
  }
  else
  {
    return "MOVI " + cond_name + get_register_name(Gd) + ", " +
           to_hex_string(Imm);
  }
}

static std::string disassemble_MOV(uint16_t instruction)
{
  uint8_t Gd = (instruction & 0x0700) >> 8;
  uint8_t Gs = (instruction & 0x0070) >> 4;

  return "MOV " + get_register_name(Gd) + ", " + get_register_name(Gs);
}

static std::string disassemble_unary_ALU(uint16_t instruction)
{
  uint8_t code = (instruction & 0x0006) >> 1;
  uint8_t Gd = (instruction & 0x0700) >> 8;
  uint8_t Gs = (instruction & 0x0070) >> 4;
  bool r = (instruction & 0x0008) >> 3;

  std::string instruction_name;
  switch (code)
  {
    case 0b00: instruction_name = "NOT "; break;
    case 0b01: instruction_name = "ROR "; break;
    case 0b10: instruction_name = "SHR "; break;
    case 0b11: instruction_name = "RCR "; break;
    default: return kUnknown;
  }

  if (r)
  {
    return instruction_name + get_register_name(Gd) + ", " +
           get_register_name(Gs);
  }
  else
  {
    return instruction_name + "F, " + get_register_name(Gs);
  }
}

static std::string disassemble_binary_ALU(uint16_t instruction)
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
    case 0: instruction_name = "ADC "; break;
    case 1: instruction_name = "ADD "; break;
    case 2: instruction_name = "SBC "; break;
    case 3: instruction_name = "SUB "; break;
    case 4: instruction_name = "AND "; break;
    case 5: instruction_name = "OR "; break;
    case 6: instruction_name = "XOR "; break;
    default: return kUnknown;
  }

  std::string Op2_name = i ? std::to_string(Op2) : get_register_name(Op2);

  if (r)
  {
    return instruction_name + get_register_name(Gd) + ", " +
           get_register_name(Gs1) + ", " + Op2_name;
  }
  else
  {
    return instruction_name + "F, " + get_register_name(Gs1) + ", " + Op2_name;
  }
}

static std::string get_register_name(uint8_t code)
{
  switch (code & 0b111)
  {
    case 0: return "A";
    case 1: return "B";
    case 2: return "C";
    case 3: return "D";
    case 4: return "M";
    case 5: return "S";
    case 6: return "L";
    default: return "PC";
  }
}

static std::string get_condition_name(uint8_t code)
{
  switch (code & 0b111)
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
}
