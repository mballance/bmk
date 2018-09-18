#ifndef INCLUDED_BMK_IMPL_CONTEXT_RISCV_REGS_H
#define INCLUDED_BMK_IMPL_CONTEXT_RISCV_REGS_H

#define	PC 0
#define	RA (PC+1)
#define	SP (RA+1)
#define	GP (SP+1)
#define	TP (GP+1)
#define	T0 (TP+1)
#define T1 (T0+1)
#define T2 (T1+1)
#define	S0 (T2+1)
#define S1 (S0+1)
#define	A0 (S1+1)
#define A1 (A0+1)
#define A2 (A1+1)
#define A3 (A2+1)
#define A4 (A3+1)
#define A5 (A4+1)
#define A6 (A5+1)
#define A7 (A6+1)
#define S2 (A7+1)
#define S3 (S2+1)
#define S4 (S3+1)
#define S5 (S4+1)
#define S6 (S5+1)
#define S7 (S6+1)
#define S8 (S7+1)
#define S9 (S8+1)
#define S10 (S9+1)
#define S11 (S10+1)
#define T3 (S11+1)
#define T4 (T3+1)
#define T5 (T4+1)
#define T6 (T5+1)

#define OFF(reg) (8*(reg))

#endif /* INCLUDED_BMK_IMPL_CONTEXT_RISCV_REGS_H */
