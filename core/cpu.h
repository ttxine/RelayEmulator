#pragma once
#include <memory>

#include "core/memory.h"

class CPU {
  public:
    enum Register : uint8_t
    {
      kA, kB, kC, kD,
      kM, kS,
      kL,
      kPC
    };

    enum Instruction : uint16_t
    {
      kHALT       = 0x1000,
      kNOP        = 0x0000,
      kLOAD       = 0x2800,
      kLOAD_Imm   = 0x2000,
      kSTORE      = 0x3800,
      kSTORE_Imm  = 0x3000,
      kCALL       = 0x8F00,
      kJMP        = 0x8700,
      kMOVI       = 0x8000,
      kMOV        = 0x1800,
      kALU_Op     = 0x4000
    };

    enum class Flag : uint8_t
    {
      kCY,
      kZ,
      kS
    };

  public:
    CPU(std::unique_ptr<Memory> memory) : memory_(std::move(memory))
    {
    }

    CPU(const CPU&) = delete;
    CPU& operator=(const CPU&) = delete;

  public:
    bool IsRunning() const
    {
      return is_running_;
    };

    uint8_t GetRegister(uint8_t code)
    {
      return GetSetRegister(code);
    }

    void SetRegister(uint8_t code, uint8_t value)
    {
      GetSetRegister(code) = value;
    }

    bool GetFlag(Flag f)
    {
      return GetSetFlag(f);
    }

    void SetFlag(Flag f, bool v)
    {
      GetSetFlag(f) = v;
    }

  public:
    uint16_t Read(uint8_t addr);
    void Write(uint8_t addr, uint8_t value);

  public:
    uint16_t Fetch();
    void Decode(uint16_t instruction);

  private:
    uint8_t& GetSetRegister(uint8_t code);
    bool& GetSetFlag(Flag f);

  private:
    bool CheckCondition(uint8_t cond);

  private:
    void HALT();
    void NOP();
    void LOAD(uint8_t G, uint8_t P);
    void LOAD_Imm(uint8_t G, uint8_t Imm);
    void STORE(uint8_t G, uint8_t P);
    void STORE_Imm(uint8_t G, uint8_t Imm);
    void CALL(uint8_t cond, uint8_t Imm);
    void JMP(uint8_t cond, uint8_t Imm);
    void MOVI(uint8_t cond, uint8_t Gd, uint8_t Imm);
    void MOV(uint8_t Gd, uint8_t Gs);
    void ALUOperation(uint16_t instruction);

  private:
    void BinaryALUOperation(uint8_t code, uint8_t Gd, uint8_t Gs1,
                      uint8_t Op2, bool r, bool i);
    void UnaryAlUOperation(uint8_t code, uint8_t Gd, uint8_t Gs, bool r);

  private:
    uint8_t ADC(uint8_t Gs1, uint8_t Op2);
    uint8_t ADD(uint8_t Gs1, uint8_t Op2);
    uint8_t SBC(uint8_t Gs1, uint8_t Op2);
    uint8_t SUB(uint8_t Gs1, uint8_t Op2);
    uint8_t AND(uint8_t Gs1, uint8_t Op2);
    uint8_t OR(uint8_t Gs1, uint8_t Op2);
    uint8_t XOR(uint8_t Gs1, uint8_t Op2);
    uint8_t NOT(uint8_t Gs);
    uint8_t ROR(uint8_t Gs);
    uint8_t SHR(uint8_t Gs);
    uint8_t RCR(uint8_t Gs);

  private:
    bool is_running_ = true;

  private:
    uint8_t A_ = 0x00;
    uint8_t B_ = 0x00;
    uint8_t C_ = 0x00;
    uint8_t D_ = 0x00;
    uint8_t M_ = 0x00;
    uint8_t S_ = 0x00;
    uint8_t L_ = 0x00;
    uint8_t PC_ = 0x00;

    std::unique_ptr<Memory> memory_;

    bool sign_ = false;
    bool zero_ = false;
    bool carry_ = false;
};
