typedef struct instruction Inst;
typedef struct adress Add;

Inst* cria_inst();

void set_opcode(Inst* inst, int opcode);
void set_end1(Inst* inst, int bloco, int palavra);
void set_end2(Inst* inst, int bloco, int palavra);
void set_end3(Inst* inst, int bloco, int palavra);

int get_opcode(Inst* inst);
int get_end1_block(Inst* inst);
int get_end1_word(Inst* inst);
int get_end2_block(Inst* inst);
int get_end2_word(Inst* inst);
int get_end3_block(Inst* inst);
int get_end3_word(Inst* inst);

void libera_inst(Inst* inst);


Add* cria_add();

void set_block(Add* add, int block);
void set_word(Add* add, int word);

int get_block_add(Add* add);
int get_word_add(Add* add);

void libera_add(Add* add);