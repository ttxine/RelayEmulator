#pragma once
#include <memory>

#include "core/instructionset.h"

class Bus;

class CPU {
  public:
    enum RegisterCode : uint8_t
    {
      kA, kB, kC, kD,
      kM, kS,
      kL,
      kPC
    };

    enum class Flag
    {
      kCY,
      kZ,
      kS
    };

  public:
    CPU(Bus* bus) : bus_(bus)
    {
    };

  public:
    void Clock();
    void Fetch();
    void Decode();
    void Reset();

    uint8_t GetRegister(uint8_t code) const;
    void SetRegister(uint8_t code, uint8_t value);
    bool GetFlag(Flag flag) const;
    void SetFlag(Flag flag, bool value);

    uint16_t Read(uint8_t addr);
    void Write(uint8_t addr, uint8_t value);

    bool Halted() const
    {
      return is_halted_;
    };

    uint16_t GetInstructionRegister()
    {
      return instruction_;
    }

  private:
    void HALT();
    void NOP();
    void LOAD();
    void LOADI();
    void STORE();
    void STOREI();
    void CALL();
    void JMP();
    void MOVI();
    void MOV();
    void ALU();

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

    void BinaryALU();
    void UnaryAlU();

    bool CheckCondition(uint8_t cond);
    bool IsAddressRegister(uint8_t code) { return code > 4; };

  private:
    Bus* bus_;

    uint16_t instruction_;

    uint8_t A_ = 0x00;
    uint8_t B_ = 0x00;
    uint8_t C_ = 0x00;
    uint8_t D_ = 0x00;
    uint8_t M_ = 0x00;
    uint8_t S_ = 0x00;
    uint8_t L_ = 0x00;
    uint8_t PC_ = 0x00;

    bool sign_ = false;
    bool zero_ = false;
    bool carry_ = false;

    bool is_halted_ = false;
};
