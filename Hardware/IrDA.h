#ifndef __IrDA_H
#define __IrDA_h

extern uint16_t Ir_Blocked;
extern uint16_t Ir_Free;

void IrDA_Init(void);
uint16_t IrDA_R_Getvalue(void);
uint16_t IrDA_L_Getvalue(void);



#endif
