int INV(int x){
  int output = !x;
  return output;
}

int AND(int x, int y){
  int output = x && y;
  return output;
}

int NAND(int x, int y){
  int output = INV(AND(x,y));
  return output;  
}

int OR(int x, int y){
  int output = x || y;
  return output;
}

int NOR(int x, int y){
  int output = INV(OR(x,y));
  return output;
}

int MUX(int x, int y, int z){
  int AND1 = AND(x,INV(z));
  int AND2 = AND(y,z);
  int OR1 = OR(AND1,AND2);
  return OR1;
}

int FOURBIT_MUX(int a, int b, int c, int d, int s, int h){
  int t1 = MUX(a,b,s);
  int t2 = MUX(c,d,s);
  int t_O = MUX(t1,t2,h);
  return t_O;
}

int XOR(int x, int y){
  int AND1 = AND(INV(x), y);
  int AND2 = AND(x, INV(y));
  int OUT = OR(AND1,AND2);
  return OUT; 
}

int XNOR(int x, int y){
  int out = INV(XOR(x,y));
  return out;
}
int ADD(int a, int b, int carry_in, int* carry_out){
  int step_1 = XOR(a,b);
  int step_2 = XOR(step_1, carry_in);
  int step_3 = AND(carry_in, XOR(a,b));
  int step_4 = OR(step_3, AND(a,b));
  *carry_out = step_4;
  return step_2;
}

void FOURADD(int P0, int P1, int P2, int P3, int T0, int T1, int T2, int T3, int* carry_out, int* out1, int* out2, int* out3, int* out4){
  int c1i, c2i, c3i, c4i, c1o, c2o, c3o, c4o;
  c1i = 0;
  *out1 = ADD(P0,T0,c1i,&c1o);
  c2i = c1o;
  *out2 = ADD(P1,T1,c2i,&c2o);
  c3i = c2o;
  *out3 = ADD(P2,T2,c3i,&c3o);
  c4i = c3o;
  *out4 = ADD(P3,T3,c4i,&c4o);
  *carry_out = c4o;
}

void SRL(int S, int R, int* Q ,int* Q_BAR){
  *Q_BAR = NOR(S,*Q);
  *Q = NOR(R,*Q_BAR);
}

void D_LATCH(int D, int E, int* Q, int* Q_BAR){
   int S,R;
   S = AND(D,E);
   R = AND(INV(D),E);
   SRL(S,R,Q,Q_BAR); 
}

void DFF(int in, int clk, int* Q0, int* Q0_BAR, int* Q, int* Q_BAR){
   D_LATCH(in,INV(clk),Q,Q_BAR);
   D_LATCH(*Q,clk, Q0, Q0_BAR);
}

void REG(int clk, int D0, int D1, int D2, int D3, int* QM, int* QM_BAR,int* QM1, int* QM1_BAR, int* QM2, int* QM2_BAR, int* QM3, int* QM3_BAR, int* Q1, int* Q1_BAR, int* Q2, int* Q2_BAR, int* Q3, int* Q3_BAR,int* Q4, int* Q4_BAR){
   DFF(D0,clk,Q1,Q1_BAR,QM,QM_BAR);
   DFF(D1,clk,Q2,Q2_BAR,QM1,QM1_BAR);
   DFF(D2,clk,Q3,Q3_BAR,QM2,QM2_BAR);
   DFF(D3,clk,Q4,Q4_BAR,QM3,QM3_BAR);
} 

void SEQ_4ADD(int clk,int D0,int D1,int D2,int D3, int* QM, int* QM_BAR,int* QM1, int* QM1_BAR, int* QM2, int* QM2_BAR, int* QM3, int* QM3_BAR, int* Q1_BAR, int* Q2_BAR, int* Q3_BAR, int* Q4, int* Q4_BAR, int* Q0, int* Q1, int* Q2, int* Q3, int* carry_out, int* out1, int* out2, int* out3, int* out4){
   REG(clk,D0,D1,D2,D3,QM,QM_BAR,QM1,QM1_BAR,QM2,QM2_BAR,QM3,QM3_BAR,Q1,Q1_BAR,Q2,Q2_BAR,Q3,Q3_BAR,Q4,Q4_BAR);
   FOURADD(D0,D1,D2,D3,*Q1,*Q2,*Q3,*Q4,carry_out,out1,out2,out3,out4);
}

int ANDFOUR(int x, int y, int z, int q){
   int out = x && y && z && q;
   return out; 
}

int ANDTHREE(int x, int y, int z){
   int out = x && y && z;
   return out; 
}

int ANDFIVE(int x, int y, int z, int q, int h){
   int out = x && y && z && q && h;
   return out;
}

int ORFOUR(int x, int y, int z, int q){
   int out = x || y || z || q;
   return out; 
}

int THREEOR(int x, int y, int z){
   int out = x || y || z;
   return out; 
}

int FIVEOR(int x, int y, int z, int q, int h){
   int out = x || y || z || q || h;
   return out; 
}

int SIXOR(int x, int y, int z, int q, int h, int o){
   int out = x || y || z || q || h || o;
   return out; 
}

/* void ADDONE(int o4, int o3, int o2, int o1, int* o4N, int* o3N, int* o2N, int* o1N){
   *o4N = ORFOUR(ANDFOUR(INV(o4),o3,o2,o1),AND(o4,INV(o2)),ANDTHREE(o2,INV(o1),INV(o4)),ANDTHREE(o4,INV(o3),o2));
   *o3N = THREEOR(AND(o3,INV(o2)),ANDTHREE(o2,INV(o1),o3),ANDTHREE(INV(o3),o2,o1));
   *o2N = AND(AND(INV(o2),o1),AND(o2,INV(o1)));
   *o1N = AND(AND(INV(o2),INV(o1)),AND(o2,INV(o1)));
} */

void COMPARATOR(int eq, int lt, int gt, int a, int b, int* eqN, int* ltN, int* gtN){
   *eqN = OR(ANDFIVE(eq,INV(lt),INV(gt),INV(a),INV(b)),ANDFIVE(eq,INV(lt),INV(gt),a,b));
   *ltN = FIVEOR(ANDFIVE(eq,INV(lt),INV(gt),a,INV(b)),ANDFIVE(INV(eq),INV(lt),gt,INV(a),INV(b)),ANDFIVE(INV(eq),INV(lt),gt,INV(a),b),ANDFIVE(INV(eq),INV(lt),gt,a,INV(b)),ANDFIVE(INV(eq),INV(lt),gt,a,b));
   *gtN = FIVEOR(ANDFIVE(eq,INV(lt),INV(gt),INV(a),b),ANDFIVE(INV(eq),lt,INV(gt),INV(a),INV(b)),ANDFIVE(INV(eq),lt,INV(gt),INV(a),b),ANDFIVE(INV(eq),lt,INV(gt),a,INV(b)),ANDFIVE(INV(eq),lt,INV(gt),a,b));  
}

void FOURCOUNT(int clk, int* o1N, int* o2N, int* o3N, int* o4N, int* carry_out, int* o1, int* o2, int* o3, int* o4, int* QM, int* QM_BAR, int* QM1, int* QM1_BAR, int* QM2, int* QM2_BAR, int* QM3, int* QM3_BAR, int* Q_BAR, int* Q1_BAR, int* Q2_BAR, int* Q3_BAR, int* Q4_BAR){
  FOURADD(1,0,0,0,*o1N,*o2N,*o3N,*o4N,carry_out,o1,o2,o3,o4);
  REG(clk,*o4,*o3,*o2,*o1,QM,QM_BAR,QM1,QM1_BAR,QM2,QM2_BAR,QM3,QM3_BAR,o4N,Q1_BAR,o3N,Q2_BAR,o2N,Q3_BAR,o1N,Q4_BAR);
}
