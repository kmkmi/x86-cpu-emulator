#include "instruction.h"
#include <string.h>


void Instruction::init(uint64_t rip, uint64_t rsp, Memory mem) {

	memset(this->regs, 0, sizeof(this->regs));
	this->regs[4] = rsp;
	this->rip = rip;
	this->mem = mem;
    this->modrm = 0;
    this->mod = 0;
    this->reg = 0;
    this->rm = 0;
    this->eflag = 0;


    for (int i = 0; i < 256; i++) {
        this->insts[i] = &Instruction::nop;
    }
	
	this->insts[0x01] = &Instruction::add_rm32_r32;

	this->insts[0x31] = &Instruction::xor_rm32_r32;

    
	this->insts[0xff] = &Instruction::op_ff ; //  dec rbx = ff cb  , jmp rbx  = ff e3 , etc...


	this->insts[0x83] = &Instruction::op_83;
    
	this->insts[0x89] = &Instruction::mov_rm32_r32;

	this->insts[0xb8] = &Instruction::mov_eax_imm32;
	this->insts[0xb9] = &Instruction::mov_ecx_imm32;


	this->insts[0x75] = &Instruction::jne_imm8;
    
	this->insts[0xeb] = &Instruction::jmp_imm8;
    
	this->insts[0xf4] = &Instruction::hlt;
    

}
void Instruction::nop() {

}

uint8_t Instruction::next_op() {
    uint8_t nx_op = mem.read1byte(this->rip++);
    printf("x%02X\n", nx_op);
    return nx_op;
}

void Instruction::readModrm() {
    this->modrm =next_op();
    this->mod = this->modrm >> 6;
    this->reg = (this->modrm & 0x38) >> 3;
    this->rm = this->modrm & 0x07;
}

void Instruction::ex_op(uint8_t op) {
    if ((op & 0xF0) == 0x40)
    {
        //rex
        this->rex = op;
        uint8_t opcode = next_op();
        ex_op(opcode);
    }
    else if ( (op == 0x67) || (op == 0x66))
    {
        //32bit addr
        //16bit reg
        uint8_t opcode = next_op();
        ex_op(opcode);
    }
    else {
        (this->*insts[op])();
    }
}




void Instruction::op_83() {

    readModrm();
    switch (this->reg) {

    case 7: cmp_rm32_imm(this->mod); break;
    default: break;
    }
}


void Instruction::mov_rm32_r32() {
    uint32_t addr, imm32;
    uint8_t imm8;

    readModrm();

    switch (this->mod) {
    case 0:
        addr = this->regs[this->rm];
        mem.write8bytes(addr, this->regs[this->reg]);
        break;
    case 1:
        imm8 = next_op();
        addr = this->regs[this->rm];
        mem.write8bytes(addr + imm8, this->regs[this->reg]);
        break;
    case 2:
        imm32 = mem.read4bytes(this->rip);
        this->rip += 4;

        addr = this->regs[this->rm];
        mem.write8bytes(addr, this->regs[this->reg]);

        break;
    default:
        this->regs[this->rm] = this->regs[this->reg];
        break;
    }
}

void Instruction::cmp_rm32_imm(int mod) {

    if (mod == 3) {
        uint8_t imm8 = next_op();
        setEflag(!(this->regs[this->rm] - imm8), ZF);
    }
    else if (mod == 0) {
        uint32_t imm32 = mem.read4bytes(this->rip);
        this->rip += 4;

        setEflag(!(this->regs[this->rm] - imm32), ZF);
    }
    else {
    }

    
}

void Instruction::chkMod(int op) {

    uint32_t addr, dst, imm32;
    uint8_t imm8;

    readModrm();

    switch (this->mod) {
    case 0:
        addr = this->regs[this->rm];
        dst = mem.read4bytes(addr);
        op_mod0to2(addr, dst, op);
        break;
    case 1:
        imm8 = next_op();
        addr = this->regs[this->rm];
        dst = mem.read4bytes(addr + imm8);
        op_mod0to2(addr + imm8, dst, op);
        break;
    case 2:
        imm32 = mem.read4bytes(this->rip);
        this->rip += 4;
        addr = this->regs[this->rm];
        dst = mem.read4bytes(addr + imm32);
        op_mod0to2(addr + imm32, dst, op);

        break;
    default:
        op_mod3(&this->regs[this->rm], &this->regs[this->reg], op);
        break;
    }
}

void Instruction::op_mod0to2(uint64_t addr, uint64_t dst, int op) {
    switch (op) {
    case ADD: mem.write8bytes(addr, dst + this->regs[this->reg]); break;

    case XOR: mem.write8bytes(addr, dst ^ this->regs[this->reg]); break;

    default: break;
    }
}

void Instruction::add_rm32_r32() { this->chkMod(ADD); }
void Instruction::xor_rm32_r32() { this->chkMod(XOR); }


void Instruction::op_mod3(uint64_t* dst, uint64_t* src, int calc_type) {
    switch (calc_type) {
    case ADD: *dst += *src; break;

    case XOR: *dst ^= *src; break;

    default: break;
    }
}

void Instruction::mov_eax_imm32() {
    uint32_t imm32 = mem.read4bytes(this->rip);
    printf("x%02X\n", imm32);

    this->regs[0] = imm32;
    this->rip += 4;
}

void Instruction::mov_ecx_imm32() {
    uint32_t imm32 = mem.read4bytes(this->rip);
    printf("x%02X\n", imm32);

    this->regs[1] = imm32;
    this->rip += 4;
}

void Instruction::jne_imm8() {
    int8_t imm8 = mem.read1byte(this->rip);
    printf("x%02X\n", imm8);
    if (!this->getEflag(ZF)) {
        this->rip += imm8;
    }
    this->rip++;
}

void Instruction::jmp_imm8() {
	int8_t imm8 = mem.read1byte(this->rip);
    printf("x%02X\n", imm8);
	this->rip += imm8;
	this->rip++;
}


void Instruction::hlt() {
    this->rip = 0x00;
}

void Instruction::op_ff() {

    readModrm();
    switch (this->reg) {

    case 1:
        this->regs[this->rm]--;
        break;
    default:
        break;
    }
}

void Instruction::setEflag(int flag, uint64_t eflag) {
    if (flag) {
        this->eflag |= eflag;
    }
    else {
        this->eflag &= ~eflag;
    }
}


int Instruction::getEflag(uint64_t flag) {

    if ((this->eflag & flag ) == 0) {
        return 0;
    }
    else {
        return 1;
    }
    
}
