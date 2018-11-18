#include <stdio.h>
#include<string.h>

#define MAXLINE 33  //指令长度
#define SIZE 60  //输出汇编行长度
#define MAXSIZE 500  //输出仿真行长度
#define MAXINSTRUCTIONNUMBER 45 //指令和数据的总数
#define FIRSTINSTRUCTADDRESS 64  //第一条指令地址
#define MEMORYNUMBER 24  //存储器数据个数


int* get_opcode(char *line)
{
	int opcode[2];
	opcode[0]=(line[0]-'0')*4+(line[1]-'0')*2+(line[2]-'0');
	opcode[1]=(line[3]-'0')*4+(line[4]-'0')*2+(line[5]-'0');
	//printf("%c-%c-%c-x=%d-%c-%c-%c-y=%d-",line[0],line[1],line[2],opcode[0],line[3],line[4],line[5],opcode[1]);
	int *p=opcode;
	return p;
}

int* get_special(char *line)
{
	int special[2];
	special[0]=(line[26]-'0')*4+(line[27]-'0')*2+(line[28]-'0');
	special[1]=(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
	int *p=special;
	return p;
}
int* get_special2(char *line)
{
	int special[2];
	special[0]=(line[26]-'0')*4+(line[27]-'0')*2+(line[28]-'0');
	special[1]=(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
	int *p=special;
	return p;
}
int* get_special3(char *line)
{
	int special[2];
	special[0]=(line[26]-'0')*4+(line[27]-'0')*2+(line[28]-'0');
	special[1]=(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
	int *p=special;
	return p;
}

int* get_regimm(char * line)
{
	int regimm[2];
	regimm[0]=(line[11]-'0')*2+(line[12]-'0');
	regimm[1]=(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
	int *p=regimm;
	return p;
}

void format_line(char *formatted_line,char *line)
{
	int current=0;
	for(int line_currnet=0;line_currnet<32;){ //将读入的01串写入disassembly数组，中间加空格
		if (line_currnet==6||line_currnet==11||line_currnet==16||line_currnet==21||line_currnet==26){
			formatted_line[current++]=' ';
			formatted_line[current++]=line[line_currnet++];
		}else{
			formatted_line[current++]=line[line_currnet++];
		}
	}
	formatted_line[current++]='\0';
}

void get_originalorcomplement(char * out,char *in){//得到原码或补码
	if(in[0]=='1'){
		for(int i=0;i<32;i++){
			if(i==0){
				out[i]=in[i];
			}else{
				if(in[i]=='0'){
					out[i]='1';
				}else{
					out[i]='0';
				}
				
			}
		}
		for(int i=31;i>=0;i--){
			if(out[i]=='0'){
				out[i]='1';
				break;
			}else{
				out[i]='0';
			}
		}
	}else{
		for(int i=0;i<32;i++){
			out[i]=in[i];
		}
	}
}

void decimal2binary(char *bi,int num){
	int number=num;
	int bit;
	int index=31;
	if(number>0){
		while(number>0){
			bit=number%2;
			bi[index]='0'+bit;
			number=number/2;
			index--;
		}
		bi[0]='0';
		for(int i=1;i<=index;i++){
			bi[i]='0';
		}
	}else{
		number=0-number;
		char temp[32];
		while(number>0){
			bit=number%2;
			temp[index]='0'+bit;
			number=number/2;
			index--;
		}
		temp[0]='1';
		for(int i=1;i<=index;i++){
			temp[i]='0';
		}
		get_originalorcomplement(bi, temp);
	}
}


int binary2decimal(char *line)
{
	char original[32];
	get_originalorcomplement(original,line);
	int result=(original[31]-'0')+(original[30]-'0')*2+(original[29]-'0')*4+(original[28]-'0')*8+(original[27]-'0')*16+(original[26]-'0')*32+(original[25]-'0')*64+(original[24]-'0')*128+(original[23]-'0')*256+(original[22]-'0')*512+(original[21]-'0')*1024
		+(original[20]-'0')*2048+(original[19]-'0')*4096+(original[18]-'0')*8192+(original[17]-'0')*16384+(original[16]-'0')*32768+(original[15]-'0')*65536+(original[14]-'0')*131072+(original[13]-'0')*262144
		+(original[12]-'0')*524288+(original[11]-'0')*1048576+(original[10]-'0')*2097152+(original[9]-'0')*4194304+(original[8]-'0')*8388608+(original[7]-'0')*16777216+(original[6]-'0')*33554432+(original[5]-'0')*67108864
		+(original[4]-'0')*134217728+(original[3]-'0')*268435456+(original[2]-'0')*536870912+(original[1]-'0')*536870912*2;
	if(original[0]=='1'){
		int temp=2*result;
		result=result-temp;
	}
	return result;
}

int main()
{
	int op_table[8][8]={
		{0,1,2,3,4,5,6,7},
		{8,9,10,11,12,13,14,15},
		{16,17,18,19,20,21,22,23},
		{24,25,26,27,28,29,30,31},
		{32,33,34,35,36,37,38,39},
		{40,41,42,43,44,45,46,47},
		{48,49,50,51,52,53,54,55},
		{56,57,58,59,60,61,62,63}
	};
	char opcode_name[64][10]={
		{'S','P','E','C','I','A','L','\0'},{'R','E','G','I','M','M','\0'},{'J','\0'},{'\0'},{'B','E','Q','\0'},{'B','N','E','\0'},{'B','L','E','Z','\0'},{'B','G','T','Z','\0'},
		{'A','D','D','I','\0'},{'A','D','D','I','U','\0'},{'S','L','T','I','\0'},{'S','L','T','I','U','\0'},{'A','N','D','I','\0'},{'O','R','I','\0'},{'X','O','R','I','\0'},{'L','U','I','\0'},
		{'C','O','P','0','\0'},{'C','O','P','1','\0'},{'C','O','P','2','\0'},{'C','O','P','1','X','\0'},{'B','E','Q','L','\0'},{'B','N','E','L','\0'},{'B','L','E','Z','L','\0'},{'B','G','T','Z','L','\0'},
		{'N','\0'},{'N','\0'},{'N','\0'},{'N','\0'},{'S','P','E','C','I','A','L','2','\0'},{'J','A','L','X','\0'},{'N','\0'},{'S','P','E','C','I','A','L','3','\0'},
		{'L','B','\0'},{'L','H','\0'},{'L','W','L','\0'},{'L','W','\0'},{'L','B','U','\0'},{'L','H','U','\0'},{'L','W','R','\0'},{'N','\0'},
		{'S','B','\0'},{'S','H','\0'},{'S','W','L','\0'},{'S','W','\0'},{'N','\0'},{'N','\0'},{'S','W','R','\0'},{'C','A','C','H','E','\0'},
		{'L','L','\0'},{'L','W','C','1','\0'},{'L','W','C','2','\0'},{'P','R','E','F','\0'},{'N','\0'},{'L','D','C','1','\0'},{'L','D','C','2','\0'},{'N','\0'},
		{'S','C','\0'},{'S','W','C','1','\0'},{'S','W','C','2','\0'},{'*','\0'},{'N','\0'},{'S','D','C','1','\0'},{'S','D','C','2','\0'},{'N','\0'}
	};
	int special_table[8][8]={
		{0,1,2,3,4,5,6,7},
		{8,9,10,11,12,13,14,15},
		{16,17,18,19,20,21,22,23},
		{24,25,26,27,28,29,30,31},
		{32,33,34,35,36,37,38,39},
		{40,41,42,43,44,45,46,47},
		{48,49,50,51,52,53,54,55},
		{56,57,58,59,60,61,62,63}
	};
	char special_name[64][10]={
		{'S','L','L','\0'},{'M','O','V','C','I','\0'},{'S','R','L','\0'},{'S','R','A','\0'},{'S','L','L','V','\0'},{'*','\0'},{'S','R','L','V','\0'},{'S','R','A','V','\0'},
		{'J','R','\0'},{'J','A','L','R','\0'},{'M','O','V','Z','\0'},{'M','O','V','N','\0'},{'S','Y','S','C','A','L','L','\0'},{'B','R','E','A','K','\0'},{'*','\0'},{'S','Y','N','C','\0'},
		{'M','F','H','I','\0'},{'M','T','H','I','\0'},{'M','F','L','O','\0'},{'M','T','L','O','\0'},{'N','\0'},{'*','\0'},{'N','\0'},{'N','\0'},
		{'M','U','L','T','\0'},{'M','U','L','T','U','\0'},{'D','I','V','\0'},{'D','I','V','U','\0'},{'N','\0'},{'N','\0'},{'N','\0'},{'N','\0'},
		{'A','D','D','\0'},{'A','D','D','U','\0'},{'S','U','B','\0'},{'S','U','B','U','\0'},{'A','N','D','\0'},{'O','R','\0'},{'X','O','R','\0'},{'N','O','R','\0'},
		{'*','\0'},{'*','\0'},{'S','L','T','\0'},{'S','L','T','U','\0'},{'N','\0'},{'N','\0'},{'N','\0'},{'N','\0'},
		{'T','G','E','\0'},{'T','G','E','U','\0'},{'T','L','T','\0'},{'T','L','T','U','\0'},{'T','E','O','\0'},{'*','\0'},{'T','N','E','\0'},{'*','\0'},
		{'N','\0'},{'*','\0'},{'N','\0'},{'N','\0'},{'N','\0'},{'*','\0'},{'N','\0'},{'N','\0'}
	};
	int special2_table[8][8]={
		{0,1,2,3,4,5,6,7},
		{8,9,10,11,12,13,14,15},
		{16,17,18,19,20,21,22,23},
		{24,25,26,27,28,29,30,31},
		{32,33,34,35,36,37,38,39},
		{40,41,42,43,44,45,46,47},
		{48,49,50,51,52,53,54,55},
		{56,57,58,59,60,61,62,63}
	};
	char special2_name[64][10]={
		{'M','A','D','D','\0'},{'M','A','D','D','U','\0'},{'M','U','L','\0'},{'C','\0'},{'M','S','U','B','\0'},{'M','S','U','B','U','\0'},{'C','\0'},{'C','\0'},
		{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},
		{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},
		{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},
		{'C','L','Z','\0'},{'C','L','O','\0'},{'C','\0'},{'C','\0'},{'N','\0'},{'N','\0'},{'C','\0'},{'C','\0'},
		{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},
		{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},
		{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'C','\0'},{'S','D','B','B','P','\0'},
	};
	int special3_table[8][8]={
		{0,1,2,3,4,5,6,7},
		{8,9,10,11,12,13,14,15},
		{16,17,18,19,20,21,22,23},
		{24,25,26,27,28,29,30,31},
		{32,33,34,35,36,37,38,39},
		{40,41,42,43,44,45,46,47},
		{48,49,50,51,52,53,54,55},
		{56,57,58,59,60,61,62,63}
	};
	char special3_name[64][10]={
		{'E','X','T','\0'},{'N','\0'},{'N','\0'},{'N','\0'},{'I','N','S','\0'},{'N','\0'},{'N','\0'},{'N','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'B','S','H','F','L','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'N','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'R','D','H','W','R','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
	};
	int regimm_table[4][8]={
		{0,1,2,3,4,5,6,7},
		{8,9,10,11,12,13,14,15},
		{16,17,18,19,20,21,22,23},
		{24,25,26,27,28,29,30,31},
	};
	char regimm_name[64][10]={
		{'B','L','T','Z','\0'},{'B','G','E','Z','\0'},{'B','L','T','Z','L','\0'},{'B','G','E','Z','L','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'T','G','E','I','\0'},{'T','G','E','I','U','\0'},{'T','L','T','I','\0'},{'T','L','T','I','U','\0'},{'T','E','Q','I','\0'},{'*','\0'},{'T','N','E','I','\0'},{'*','\0'},
		{'B','L','T','Z','A','L','\0'},{'B','G','E','Z','A','L','\0'},{'B','L','T','Z','A','L','L','\0'},{'B','G','E','Z','A','L','L','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},
		{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'*','\0'},{'S','Y','N','C','I','\0'},
	};
		int a=1+1*4+1*8+1*16+1*32+1*64+1*128+1*256+1*512+1*1024
				+1*2048+1*4096+1*8192+1*16384+1*32768+1*65536+1*131072+1*262144
		+1*524288+1048576+1*2097152+1*4194304+1*8388608+1*16777216+1*33554432+1*67108864
		+1*134217728+1*268435456+1*536870912+1*536870912*2;
		printf("error------------%d\n",a);
	unsigned int pc=FIRSTINSTRUCTADDRESS;
	char registers[32][32];//寄存器0-31
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<32;j++){
			registers[i][j]='0';
		}
	}
	registers[3][31]='1';
	char memories[MEMORYNUMBER][32];
	char instructions[MAXINSTRUCTIONNUMBER][MAXLINE];

	char disassembly[SIZE];
	char simulation[MAXSIZE];
	
	int *opcode_xy;
	int *special_xy;
	int *special2_xy;
	int *special3_xy;
	int *regimm_xy;
	char inst[MAXLINE];
	char formatted_line[MAXLINE+10];
	FILE *fp_r=fopen("sample.txt","r");
	FILE *fp_w_disassembly=fopen("disassembly.txt","w");
	FILE *fp_w_simulation=fopen("simulation.txt","w");
	int j=0;
	while(fgets(inst,MAXLINE,fp_r)!=NULL){
		if(inst[0]<'0'||inst[0]>'1')
			continue;
		for(int i=0;i<MAXLINE;i++){
			instructions[j][i]=inst[i];
		}
		j++;
	}
	while(1)
	{
		char *line=instructions[(pc-FIRSTINSTRUCTADDRESS)/4];
		int pc_temp=pc;
		pc=pc+4;
		//先作为第二类指令分析
		if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='0'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
		//printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",(line[16]-'0')*16384,(line[17]-'0')*8192,(line[18]-'0')*4096,(line[19]-'0')*(2^12),(line[20]-'0')*2048,(line[21]-'0')*1024,(line[22]-'0')*512,(line[23]-'0')*256,(line[24]-'0')*128,(line[25]-'0')*64,(line[26]-'0')*32,(line[27]-'0')*16,(line[28]-'0')*8,(line[29]-'0')*4,(line[30]-'0')*2,(line[31]-'0'),imm);
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			format_line(formatted_line,line);
			sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,"ADD",rt,rs,imm);
			fputs(disassembly,fp_w_disassembly);
			printf("%s",disassembly);
		}else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			format_line(formatted_line,line);
			sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,"SUB",rt,rs,imm);
			fputs(disassembly,fp_w_disassembly);
			printf("%s",disassembly);
		}else if(line[0]=='1'&&line[1]=='0'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			format_line(formatted_line,line);
			sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,"MUL",rt,rs,imm);
			fputs(disassembly,fp_w_disassembly);
			printf("%s",disassembly);
		}else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='0'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,"AND",rt,rs,imm);
			fputs(disassembly,fp_w_disassembly);
			printf("%s",disassembly);
		}else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			format_line(formatted_line,line);
			sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,"NOR",rt,rs,imm);
			fputs(disassembly,fp_w_disassembly);
			printf("%s",disassembly);
		}else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='1'&&line[4]=='0'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			format_line(formatted_line,line);
			sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,"SLT",rt,rs,imm);
			fputs(disassembly,fp_w_disassembly);
			printf("%s",disassembly);
		}else{ //作为第一类指令分析
			opcode_xy=get_opcode(line);
			int x=opcode_xy[0];
			int y=opcode_xy[1];
			int opcode=op_table[x][y];
			if(opcode==0){
				special_xy=get_special(line);
				int xx=special_xy[0];
				int yy=special_xy[1];
				int special=special_table[xx][yy];
				if(special==0){//SLL
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,special_name[special],rd,rt,sa);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==2){//SRL
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,special_name[special],rd,rt,sa);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==3){//SRA
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,special_name[special],rd,rt,sa);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==8){//JR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d\n",formatted_line,pc_temp,special_name[special],rs);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==13){//BREAK
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s\n",formatted_line,pc_temp,special_name[special]);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
					break;
				}else if(special==32){//ADD
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special_name[special],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==34){//SUB
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special_name[special],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==36){//AND
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special_name[special],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==37){//OR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special_name[special],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==39){//NOR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special_name[special],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else if(special==42){//SLT
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special_name[special],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else{
				}
			}else if(opcode==1){
				regimm_xy=get_regimm(line);
				int xx=regimm_xy[0];
				int yy=regimm_xy[1];
				int regimm=regimm_table[xx][yy];
				if(regimm==0){//BLTZ
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
					+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
					+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
					+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,#%d\n",formatted_line,pc_temp,regimm_name[regimm],rs,offset*4);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else{
					
				}
			}else if(opcode==2){//J
				int target=(line[6]-'0')*16777216+(line[7]-'0')*8388608+(line[8]-'0')*4194304+(line[9]-'0')*2097152+(line[10]-'0')*1048576
					+(line[11]-'0')*524288+(line[12]-'0')*262144+(line[13]-'0')*131072+(line[14]-'0')*65536+(line[15]-'0')*32768
					+(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
					+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
					+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
					+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
					format_line(formatted_line,line);
					unsigned int temp=pc;
					temp=temp/268435465;
					temp=temp*268435465;
					temp=temp+target*4;
					sprintf(disassembly,"%s	%d	%s #%d\n",formatted_line,pc_temp,opcode_name[opcode],temp);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
			}else if(opcode==4){//BEQ
				int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
				+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
				+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
				+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				format_line(formatted_line,line);
				sprintf(disassembly,"%s	%d	%s R%d,R%d,#%d\n",formatted_line,pc_temp,opcode_name[opcode],rs,rt,offset*4);
				fputs(disassembly,fp_w_disassembly);
				printf("%s",disassembly);
			}else if(opcode==7){//BGTZ
				int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
				+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
				+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
				+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				format_line(formatted_line,line);
				sprintf(disassembly,"%s	%d	%s R%d,#%d\n",formatted_line,pc_temp,opcode_name[opcode],rs,offset*4);
				fputs(disassembly,fp_w_disassembly);
				printf("%s",disassembly);
			}else if(opcode==28){
				special2_xy=get_special2(line);
				int xx=special2_xy[0];
				int yy=special2_xy[1];
				int special2=special2_table[xx][yy];
				if(special2==0){
					printf("78-%s\n",special2_name[special2]);
				}else if(special2==2){//MUL
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					format_line(formatted_line,line);
					sprintf(disassembly,"%s	%d	%s R%d,R%d,R%d\n",formatted_line,pc_temp,special2_name[special2],rd,rs,rt);
					fputs(disassembly,fp_w_disassembly);
					printf("%s",disassembly);
				}else{
					
				}
			}else if(opcode==31){
				special3_xy=get_special3(line);
				int xx=special3_xy[0];
				int yy=special3_xy[1];
				int special3=special3_table[xx][yy];
				if(special3==0){
					printf("88-%s\n",special3_name[special3]);
				}else{
					
				}
			}else if(opcode==35){//LW
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
						+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
						+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
						+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				format_line(formatted_line,line);
				sprintf(disassembly,"%s	%d	%s R%d,%d(R%d)\n",formatted_line,pc_temp,opcode_name[opcode],rt,offset,base);
				fputs(disassembly,fp_w_disassembly);
				printf("%s",disassembly);
			}else if(opcode==43){//SW
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
						+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
						+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
						+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				format_line(formatted_line,line);
				sprintf(disassembly,"%s	%d	%s R%d,%d(R%d)\n",formatted_line,pc_temp,opcode_name[opcode],rt,offset,base);
				fputs(disassembly,fp_w_disassembly);
				printf("%s",disassembly);
			}else{
				
			}
			//printf("x=%d-y=%d-%d\n",x,y,opcode);
		}
	}
	//数据
	int first_memory_location=pc;//第一条数据的地址
	while(((pc-FIRSTINSTRUCTADDRESS)/4)<MAXINSTRUCTIONNUMBER)
	{
		char *line=instructions[(pc-FIRSTINSTRUCTADDRESS)/4];
		int pc_temp=pc;
		pc=pc+4;
		for(int i=0;i<32;i++){
		memories[(pc_temp-first_memory_location)/4][i]=line[i];
		}
		sprintf(disassembly,"%s	%d	%d\n",line,pc_temp,binary2decimal(line));
		fputs(disassembly,fp_w_disassembly);
		printf("%s",disassembly);
	}
	//simulation
	j=0;
	pc=FIRSTINSTRUCTADDRESS;//重置pc
	while(1)
	{
		j++;
		//printf("Instructions_index%d\n",(pc-FIRSTINSTRUCTADDRESS)/4);
		char *line=instructions[(pc-FIRSTINSTRUCTADDRESS)/4];
		//printf("%s\n",instructions[(pc-FIRSTINSTRUCTADDRESS)/4]);
		int pc_temp=pc;
		pc=pc+4;
		//先作为第二类指令分析
		if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='0'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			decimal2binary(registers[rt],binary2decimal(registers[rs])+imm);
			sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,"ADD",rt,rs,imm,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
			fputs(simulation,fp_w_simulation);
			//printf("simulationlength--%d\n",strlen(simulation));
		}else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			decimal2binary(registers[rt],binary2decimal(registers[rs])-imm);
			sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,"SUB",rt,rs,imm,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
			fputs(simulation,fp_w_simulation);
		}else if(line[0]=='1'&&line[1]=='0'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			decimal2binary(registers[rt],binary2decimal(registers[rs])*imm);
			sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,"MUL",rt,rs,imm,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
			fputs(simulation,fp_w_simulation);
		}else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='0'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			for(int i=0;i<32;i++){
				if(i>=0&&i<16){
					registers[rt][i]='0';
				}else{
					if(registers[rs][i]=='1'&&line[i]=='1'){
						registers[rt][i]='1';
					}else{
						registers[rt][i]='0';
					}
				}
			}
			sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,"AND",rt,rs,imm,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
			fputs(simulation,fp_w_simulation);
		}else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			for(int i=0;i<32;i++){
				if(i>=0&&i<16){
					if(registers[rt][i]=='0'){
						registers[rt][i]='1';
					}else{
						registers[rt][i]='0';
					}
				}else{
					if(registers[rs][i]=='0'&&line[i]=='0'){
						registers[rt][i]='1';
					}else{
						registers[rt][i]='0';
					}
				}
			}
			sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,"NOR",rt,rs,imm,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
			fputs(simulation,fp_w_simulation);
		}else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='1'&&line[4]=='0'&&line[5]=='1'){
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			if(binary2decimal(registers[rs])<imm){
				decimal2binary(registers[rt],1);
			}else{
				decimal2binary(registers[rt],0);
			}
			sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,"SLT",rt,rs,imm,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
			fputs(simulation,fp_w_simulation);
		}else{ //作为第一类指令分析
			opcode_xy=get_opcode(line);
			int x=opcode_xy[0];
			int y=opcode_xy[1];
			int opcode=op_table[x][y];
			if(opcode==0){
				special_xy=get_special(line);
				int xx=special_xy[0];
				int yy=special_xy[1];
				int special=special_table[xx][yy];
				if(special==0){//SLL
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					for(int i=sa;i<32;i++){
						registers[rd][i-sa]=registers[rt][i];
					}
					for(int i=32-sa;i<32;i++){
						registers[rd][i]='0';
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rt,sa,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					//printf("simulationlength--%d\n",strlen(simulation));
					fputs(simulation,fp_w_simulation);
				}else if(special==2){//SRL
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					for(int i=31-sa;i>=0;i--){
						registers[rd][i+sa]=registers[rt][i];
					}
					for(int i=0;i<sa;i++){
						registers[rd][i]='0';
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rt,sa,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==3){//SRA
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					for(int i=31-sa;i>=0;i--){
						registers[rd][i+sa]=registers[rt][i];
					}
					for(int i=0;i<sa;i++){
						registers[rd][i]=registers[rd][sa];
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rt,sa,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==8){//JR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					pc=binary2decimal(registers[rs]);
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rs,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==13){//BREAK
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
					break;
				}else if(special==32){//ADD
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int rounding[32]={0};
					for(int i=31;i>=0;i--){
						if(rounding[i]==0){
							if(registers[rs][i]!=registers[rt][i]){
								registers[rd][i]='1';
							}else if(registers[rs][i]=='0'){
								registers[rd][i]='0';
							}else{
								registers[rd][i]='0';
								if(i>0)
								rounding[i-1]=1;
							}
						}else{
							if(registers[rs][i]!=registers[rt][i]){
								registers[rd][i]='0';
								if(i>0)
								rounding[i-1]=1;
							}else if(registers[rs][i]=='0'){
								registers[rd][i]='1';
							}else{
								registers[rd][i]='1';
								if(i>0)
								rounding[i-1]=1;
							}
						}
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==34){//SUB
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					char rt_original[32];
					char negrt_complement[32];
					get_originalorcomplement(rt_original, registers[rt]);
					if(rt_original[0]=='0'){
						rt_original[0]='1';
					}else{
						rt_original[0]='0';
					}
					get_originalorcomplement(negrt_complement,rt_original);
					int rounding[32]={0};
					for(int i=31;i>=0;i--){
						if(rounding[i]==0){
							if(registers[rs][i]!=negrt_complement[i]){
								registers[rd][i]='1';
							}else if(registers[rs][i]=='0'){
								registers[rd][i]='0';
							}else{
								registers[rd][i]='0';
								if(i>0)
								rounding[i-1]=1;
							}
						}else{
							if(registers[rs][i]!=negrt_complement[i]){
								registers[rd][i]='0';
								if(i>0)
								rounding[i-1]=1;
							}else if(registers[rs][i]=='0'){
								registers[rd][i]='1';
							}else{
								registers[rd][i]='1';
								if(i>0)
								rounding[i-1]=1;
							}
						}
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==36){//AND
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					for(int i=0;i<32;i++){
						if(registers[rs][i]=='1'&&registers[rt][i]=='1'){
							registers[rd][i]='1';
						}else{
							registers[rd][i]='0';
						}
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==37){//OR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					for(int i=0;i<32;i++){
						if(registers[rs][i]==registers[rt][i]){
							registers[rd][i]='0';
						}else{
							registers[rd][i]='1';
						}
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==39){//NOR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					for(int i=0;i<32;i++){
						if(registers[rs][i]=='0'&&registers[rt][i]=='0'){
							registers[rd][i]='1';
						}else{
							registers[rd][i]='0';
						}
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else if(special==42){//SLT
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					if(binary2decimal(registers[rs])<binary2decimal(registers[rt])){
						decimal2binary(registers[rd],1);
					}else{
						decimal2binary(registers[rd],0);
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special_name[special],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else{
				}
			}else if(opcode==1){
				regimm_xy=get_regimm(line);
				int xx=regimm_xy[0];
				int yy=regimm_xy[1];
				int regimm=regimm_table[xx][yy];
				if(regimm==0){//BLTZ
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
					+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
					+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
					+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
					if(binary2decimal(registers[rs])<0){
						pc=pc+offset*4;
					}
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,regimm_name[regimm],rs,offset*4,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else{
					
				}
			}else if(opcode==2){//J
				int target=(line[6]-'0')*16777216+(line[7]-'0')*8388608+(line[8]-'0')*4194304+(line[9]-'0')*2097152+(line[10]-'0')*1048576
					+(line[11]-'0')*524288+(line[12]-'0')*262144+(line[13]-'0')*131072+(line[14]-'0')*65536+(line[15]-'0')*32768
					+(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
					+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
					+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
					+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
					pc=pc/268435465;
					pc=pc*268435465;
					pc=pc+target*4;
				sprintf(simulation,"--------------------\nCycle:%d	%d	%s	#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,opcode_name[opcode],pc,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
				fputs(simulation,fp_w_simulation);
			}else if(opcode==4){//BEQ
				int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
				+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
				+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
				+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				if(binary2decimal(registers[rs])==binary2decimal(registers[rt])){
					pc=pc+offset*4;
				}
				sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,opcode_name[opcode],rs,rt,offset*4,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
				fputs(simulation,fp_w_simulation);
			}else if(opcode==7){//BGTZ
				int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
				+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
				+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
				+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				if(binary2decimal(registers[rs])>0){
					pc=pc+offset*4;
				}
				sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,#%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,opcode_name[opcode],rs,offset*4,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
				fputs(simulation,fp_w_simulation);
			}else if(opcode==28){
				special2_xy=get_special2(line);
				int xx=special2_xy[0];
				int yy=special2_xy[1];
				int special2=special2_table[xx][yy];
				if(special2==0){
					printf("78-%s\n",special2_name[special2]);
				}else if(special2==2){//MUL
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					decimal2binary(registers[rd],binary2decimal(registers[rs])*binary2decimal(registers[rt]));
					sprintf(simulation,"--------------------\nCycle:%d	%d	%s	R%d,R%d,R%d\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,special2_name[special2],rd,rs,rt,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
					fputs(simulation,fp_w_simulation);
				}else{
					
				}
			}else if(opcode==31){
				special3_xy=get_special3(line);
				int xx=special3_xy[0];
				int yy=special3_xy[1];
				int special3=special3_table[xx][yy];
				if(special3==0){
					printf("88-%s\n",special3_name[special3]);
				}else{
					
				}
			}else if(opcode==35){//LW
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
						+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
						+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
						+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int base_address=binary2decimal(registers[base]);
				int real_address=base_address+offset;
				for(int i=0;i<32;i++){
					registers[rt][i]=memories[(real_address-first_memory_location)/4][i];
				}
				sprintf(simulation,"--------------------\nCycle:%d	%d	%s R%d,%d(R%d)\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,opcode_name[opcode],rt,offset,base,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
				fputs(simulation,fp_w_simulation);
			}else if(opcode==43){//SW
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
						+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
						+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
						+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int base_address=binary2decimal(registers[base]);
				int real_address=base_address+offset;
				for(int i=0;i<32;i++){
					memories[(real_address-first_memory_location)/4][i]=registers[rt][i];
				}
				sprintf(simulation,"--------------------\nCycle:%d	%d	%s R%d,%d(R%d)\n\nRegisters\nR00:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\nR16:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n\nData\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n%d: %d	%d	%d	%d	%d	%d	%d	%d\n",j,pc_temp,opcode_name[opcode],rt,offset,base,binary2decimal(registers[0]),binary2decimal(registers[1]),binary2decimal(registers[2]),binary2decimal(registers[3]),binary2decimal(registers[4]),binary2decimal(registers[5]),binary2decimal(registers[6]),binary2decimal(registers[7]),binary2decimal(registers[8]),binary2decimal(registers[9]),binary2decimal(registers[10]),binary2decimal(registers[11]),binary2decimal(registers[12]),binary2decimal(registers[13]),binary2decimal(registers[14]),binary2decimal(registers[15]),binary2decimal(registers[16]),binary2decimal(registers[17]),binary2decimal(registers[18]),binary2decimal(registers[19]),binary2decimal(registers[20]),binary2decimal(registers[21]),binary2decimal(registers[22]),binary2decimal(registers[23]),binary2decimal(registers[24]),binary2decimal(registers[25]),binary2decimal(registers[26]),binary2decimal(registers[27]),binary2decimal(registers[28]),binary2decimal(registers[29]),binary2decimal(registers[30]),binary2decimal(registers[31]),first_memory_location,binary2decimal(memories[0]),binary2decimal(memories[1]),binary2decimal(memories[2]),binary2decimal(memories[3]),binary2decimal(memories[4]),binary2decimal(memories[5]),binary2decimal(memories[6]),binary2decimal(memories[7]),first_memory_location+32,binary2decimal(memories[8]),binary2decimal(memories[9]),binary2decimal(memories[10]),binary2decimal(memories[11]),binary2decimal(memories[12]),binary2decimal(memories[13]),binary2decimal(memories[14]),binary2decimal(memories[15]),first_memory_location+64,binary2decimal(memories[16]),binary2decimal(memories[17]),binary2decimal(memories[18]),binary2decimal(memories[19]),binary2decimal(memories[20]),binary2decimal(memories[21]),binary2decimal(memories[22]),binary2decimal(memories[23]));
				fputs(simulation,fp_w_simulation);
			}else{}
		}
	}
	if(fclose(fp_r)!=0){
		printf("Error in closing file \n");
	}
	if(fclose(fp_w_disassembly)!=0){
		printf("Error in closing file \n");
	}
	if(fclose(fp_w_simulation)!=0){
		printf("Error in closing file \n");
	}
	
}
