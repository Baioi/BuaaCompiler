#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "getSym.h"
#include "SymbolTable.h"
#include "grammar.h"
#include "error.h"
#include "Quadruple.h"


FILE * output_grammar_pointer;
int return_num = 0; //0:û��return 1:return �մ� 2:return int 3:return char
char pre_token[MAXPredictNum] = "\0";
//�﷨����ģ��
void clearPreToken(){
    int i;
    for(i = strlen(token)-1; i >= 0; i--){
        ungetc(token[i], input_file_pointer);
        cc--;
    }
 //   ungetc(' ', input_file_pointer);
    for(i = strlen(pre_token)-1; i >= 0; i--){
        ungetc(pre_token[i], input_file_pointer);
        cc--;
    }
    pre_token[0] = '\0';
}
void program(){
    fprintf(output_grammar_pointer, "This is a program!\n");
    if(symbol == CONSTSY){
        con_dcl();
    }
    if(symbol == CHARSY || symbol == INTSY){
        strcat(pre_token,token);
        strcat(pre_token," ");//����ո�����Ϊ�������ʣ���ֹ����
        getsym();
        if(symbol == IDSY){
            strcat(pre_token,token);
     //       strcat(pre_token," ");
            getsym();
        } else{
            error(8); //8:Ӧ�б�ʶ��identity
            //------------------������---------------
            skip(1);
        }
        if(symbol == LPARSY){ //�з���ֵ������Ԥ����ϣ��˻��ַ������Ԥ������
            clearPreToken();
            //puts(fgets(token,100,input_file_pointer));
            getsym();
            r_func_dcl();
        }
        else if(symbol == COMMASY || symbol == LBRACKETSY || symbol == SEMISY) {
            clearPreToken();
            getsym();
            var_dcl();
        }
        else{
            error(10);//10���з���ֵ���������������ʽ����
            //------------------������---------------����int/char/void
            skip(2);
        }
    }
    while(1){
        if(symbol == CHARSY || symbol == INTSY)
            r_func_dcl();
        else if(symbol == VOIDSY){
            strcat(pre_token,token);
            strcat(pre_token," ");
            getsym();
            if(symbol == MAINSY){
                clearPreToken();
                getsym();
                func_main();
                break;
            }
            else if(symbol == IDSY){
                clearPreToken();
                getsym();
                v_func_dcl();
            }
            else{
                error(11);//11����Ҫһ����ʶ����main
                skip(2);
            }
        }
        else{
            error(9);//error 9����Ҫһ�����ͷ���
            skip(2);
        }
    }
}
void con_dcl(){
    fprintf(output_grammar_pointer,"This is a constance declaration!\n");
    do{
        if(symbol != CONSTSY){
            error(12);//error 12:��Ҫһ��const
            skip(3);
            getsym();
            break;
        }
        getsym();
        con_def();
        if(symbol != SEMISY){
            error(13);//error 13:��Ҫһ���ֺ�
        }
        else
            getsym();
    } while(symbol == CONSTSY);
}
void con_def(){
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a constance definition!\n");
    int t_num = 0;
    char id[MAXTokenL] = "\0";
    char ch;
    if(symbol == INTSY){
        getsym();
        if(symbol != IDSY){
            error(8);//error 8:��Ҫһ����ʶ��
            skip(4);
        }
        else{
            strcpy(id,token);
            getsym();
            if(symbol != ISSY){
                error(14);//error 14:��Ҫһ���Ⱥ�
                skip(4);
            }
            else{
                getsym();
                integer();//�ı�numֵ?????
                t_num = num;

                if(table_num == 0){
                    te = enter_table(id,GLOCON,INT,t_num,0, table_num);
                    write_quadruple_s(GLC, te);
                }
                else
                    te = enter_table(id,CONST,INT,t_num,0, table_num);
            }
        }

        while(symbol == COMMASY){
            getsym();
            if(symbol != IDSY){
                error(8);//error 8:��Ҫһ����ʶ��
                skip(4);
            }
            else{
                strcpy(id,token);
                getsym();
                if(symbol != ISSY){
                    error(14);//error 14:��Ҫһ���Ⱥ�
                    skip(4);
                }
                else{
                    getsym();
                    integer();
                    t_num = num;

                    if(table_num==0){
                        te = enter_table(id,GLOCON,INT,t_num,0, table_num);
                        write_quadruple_s(GLC, te);
                    }
                    else
                        te = enter_table(id,CONST,INT,t_num,0, table_num);
                }
            }
        }
    }
    if(symbol == CHARSY){
        getsym();
        if(symbol != IDSY){
            error(8);//error 8:��Ҫһ����ʶ��
            skip(4);
        }
        else{
            strcpy(id,token);
            getsym();
            if(symbol != ISSY){
                error(14);//error 14:��Ҫһ���Ⱥ�
                skip(4);
            }
            getsym();
            if(symbol != CHARCON){
                error(16);//error 16:��Ҫһ���ַ�
                skip(4);
            }
            else{
                t_num = num;
                if(table_num==0){
                    te = enter_table(id,GLOCON,CHAR,t_num,0, table_num);
                    write_quadruple_s(GLC, te);
                }
                else
                    te = enter_table(id,CONST,CHAR,t_num,0, table_num);
                getsym();
            }
        }
        while(symbol == COMMASY){
            getsym();
            if(symbol != IDSY){
            error(8);//error 8:��Ҫһ����ʶ��
            skip(4);
        }
        else{
            strcpy(id,token);
            getsym();
            if(symbol != ISSY){
                error(14);//error 14:��Ҫһ���Ⱥ�
                skip(4);
            }
            getsym();
            if(symbol != CHARCON){
                error(16);//error 16:��Ҫһ���ַ�
                skip(4);
            }
            else{
                t_num = num;
                if(table_num==0){
                    te = enter_table(id,GLOCON,CHAR,t_num,0, table_num);
                    write_quadruple_s(GLC, te);
                }
                else
                    te = enter_table(id,CONST,CHAR,t_num,0, table_num);
                getsym();
            }
            }
        }
    }
}
void var_dcl(){
    TableEle *te;
    fprintf(output_grammar_pointer,"This is a variable declaration!\n");
    do{
        var_def();
        if(symbol != SEMISY){
            error(13);//��Ҫһ���ֺ�
        }
        else{
            getsym();
        }
        //��ʼԤ���ж���һ���Ƿ��Ǳ����������򷵻�
        if(symbol != CHARSY && symbol != INTSY){
            return;
        }
        strcat(pre_token,token);
        strcat(pre_token," ");
        getsym();
        if(symbol != IDSY){
            error(8);
            skip(3);
        }
        else{
            strcat(pre_token,token);
            strcat(pre_token," ");
            getsym();
            if(symbol == LPARSY){
                clearPreToken();
                getsym();
                return;
            }
            clearPreToken();
            getsym();
        }
    } while(symbol == CHARSY || symbol == INTSY);
}
void var_def(){
    int kind, type, length = 0;
    char id[MAXTokenL];
    TableEle *te;
    fprintf(output_grammar_pointer,"This is a variable definition!\n");
    if(symbol != CHARSY && symbol != INTSY){
        error(17);//��Ҫһ�����ͱ�ʶ��
        skip(3);
        return;
    }
    type = symbol;
    getsym();
    if(symbol != IDSY){
        error(8);//error 8:��Ҫһ����ʶ��
        skip(4);
    }
    else{
        strcpy(id,token);
        getsym();
        if(symbol == LBRACKETSY){
            getsym();
            if(symbol != INTCON){
                error(15);//error 15����Ҫһ������
                skip(4);
            }
            else{
                if(num == 0){
                    error(30);//���鳤�Ȳ���Ϊ0
                    skip(4);
                }
                length = num;
                getsym();
                if(symbol != RBRACKETSY){
                    error(18);//error 18:��Ҫһ���ҷ�����
                    skip(4);
                }
                else{
                    getsym();
                    if(type == CHARSY){
                        if(table_num==0){
                            te = enter_table(id, GLOARR, CHAR, addr, length, table_num);
                            write_quadruple_s(GLV, te);
                        }
                        else
                            te = enter_table(id, ARRAY, CHAR, addr, length, table_num);
                        addr+=length*4;
                    }
                    else if(type == INTSY){
                        if(table_num==0){
                            te = enter_table(id, GLOARR, INT, addr, length, table_num);
                            write_quadruple_s(GLV, te);
                        }
                        else
                            te = enter_table(id, ARRAY, INT, addr, length, table_num);
                        addr+=length*4;
                    }
                }
            }
        }
        else{
            if(type == CHARSY){
                if(table_num==0){
                    te = enter_table(id, GLOVAR, CHAR, addr, length, table_num);
                    write_quadruple_s(GLV, te);
                }
                else
                    te = enter_table(id, VAR, CHAR, addr, length, table_num);
                addr+=4;
            }
            else if(type == INTSY){
                if(table_num==0){
                    te = enter_table(id, GLOVAR, INT, addr, length, table_num);
                    write_quadruple_s(GLV, te);
                }
                else
                    te = enter_table(id, VAR, INT, addr, length, table_num);
                addr+=4;
            }
        }
    }
    while(symbol == COMMASY){
        getsym();
        if(symbol != IDSY){
            error(8);//error 8:��Ҫһ����ʶ��
            skip(4);
            continue;
        }
        strcpy(id,token);
        getsym();
        if(symbol == LBRACKETSY){
            getsym();
            if(symbol != INTCON){
                error(15);//error 15:��Ҫһ������
                skip(4);
                continue;
            }
            if(num == 0){
                error(30);
                skip(4);
                continue;
            }
            length = num;
            getsym();
            if(symbol != RBRACKETSY){
                error(18);//error 18:��Ҫһ���ҷ�����
                skip(4);
                continue;
            }
            if(type == CHARSY){
                if(table_num==0){
                    te = enter_table(id, GLOARR, CHAR, addr, length, table_num);
                    write_quadruple_s(GLV, te);
                }
                else
                    te = enter_table(id, ARRAY, CHAR, addr, length, table_num);
                addr+=length*4;
            }
            else if(type == INTSY){
                if(table_num==0){
                    te = enter_table(id, GLOARR, INT, addr, length, table_num);
                    write_quadruple_s(GLV, te);
                }
                else
                    te = enter_table(id, ARRAY, INT, addr, length, table_num);
                addr+=length*4;
            }
            getsym();
        }
        else{
            length = 0;
            if(type == CHARSY){
                if(table_num==0){
                    te = enter_table(id, GLOVAR, CHAR, addr, length, table_num);
                    write_quadruple_s(GLV, te);
                }
                else
                    te = enter_table(id, VAR, CHAR, addr, length, table_num);
                addr+=4;
            }
            else if(type == INTSY){
                if(table_num==0){
                    te = enter_table(id, GLOVAR, INT, addr, length, table_num);
                    write_quadruple_s(GLV, te);
                }
                else
                    te = enter_table(id, VAR, INT, addr, length, table_num);
                addr+=4;
            }
        }
    }
}
void r_func_dcl(){
    int type = 0;
    fprintf(output_grammar_pointer,"This is a declaration of a function with returns!\n");
    type = dcl_head();
    if(symbol != LPARSY){
        error(19);//error 19:��Ҫһ��������
        skip(2);
        return;
    }
    getsym();
    para();
    if(symbol != RPARSY){
        error(20);//error 20:��Ҫһ��������
        skip(2);
        return;
    }
    getsym();
    if(symbol != LBRACESY){
        error(21);//error 21:��Ҫһ��������
        skip(2);
        return;
    }
    getsym();
    comp_statement();
    if(symbol != RBRACESY){
        error(22);//error 22:��Ҫһ���һ�����
        skip(2);
        return;
    }
    if(!return_num){
        error(42);//error 42:ȱ�ٷ���ֵ
        skip(2);
        return;
    }
    if(return_num == 1){
        error(43);//����ֵ���Ͳ�ƥ��
        skip(2);
        return;
    }
    if(type == INT && return_num != 2){
        error(43);
        skip(2);
        return;
    }
    else if(type == CHAR && return_num != 3){
        error(43);
        skip(2);
        return;
    }
    write_quadruple(END);//������������
    getsym();
    return_num = 0;
}
int dcl_head(){
    char id[MAXTokenL];
    int type;
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a head of declaration!\n");
    if(symbol == INTSY){
        getsym();
        if(symbol != IDSY){
            error(8); //��Ҫһ����ʶ��
        }
        strcpy(id,token);
        te = enter_table(id,FUNC,INT,table_num+1,0,0);
        write_quadruple_s(FDCL,te);//����������ʼ
        getsym();
        addr = 0;
        table_num++;
        type=INT;
    }
    else if(symbol == CHARSY){
        getsym();
        if(symbol != IDSY){
            error(8);//��Ҫһ����ʶ��
        }
        strcpy(id,token);
        te = enter_table(id,FUNC,CHAR,table_num+1,0,0);
        write_quadruple_s(FDCL,te);//����������ʼ
        getsym();
        addr = 0;
        table_num++;
        type=CHAR;
    }
    else{
        error(17);//��Ҫһ�����ͷ�
    }
    return type;
    //table_num++;
}
void para(){
    fprintf(output_grammar_pointer,"This is a parameter!\n");
    para_table();
}
void para_table(){
    char id[MAXTokenL];
    int type;
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a parameter table!\n");
    if(symbol == INTSY || symbol == CHARSY){
        type = symbol;
        getsym();
        if(symbol == IDSY){
            //����ű�
            strcpy(id,token);
            if(type==INTSY){
                te = enter_table(id, PARA, INT, addr, 0, table_num);
                addr+=4;
            }
            else if(type==CHARSY){
                enter_table(id,PARA,CHAR,addr,0, table_num);
                addr+=4;
            }
            getsym();
        }
        else{
            error(8);//��Ҫһ����ʶ��
            skip(4);
        }
        while(symbol == COMMASY){
            getsym();
            if(symbol == INTSY || symbol == CHARSY){
                type=symbol;
                getsym();
                if(symbol == IDSY){
                    //����ű�
                    strcpy(id,token);
                    if(type==INTSY){
                        enter_table(id, PARA, INT, addr, 0, table_num);
                        addr+=4;
                    }
                    else if(type==CHARSY){
                        enter_table(id,PARA,CHAR,addr,0, table_num);
                        addr+=4;
                    }
                    getsym();
                }
                else{
                    error(8);//��Ҫһ����ʶ��
                    skip(4);
                }
            }
        }
    }
}
void comp_statement(){
    fprintf(output_grammar_pointer,"This is a compound statement!\n");
    if(symbol == CONSTSY){
        con_dcl();
    }
    if(symbol == INTSY || symbol == CHARSY){
        var_dcl();
    }
    statement_lists();
}
void statement_lists(){
    fprintf(output_grammar_pointer,"This is a statement list!\n");
    while(symbol == IDSY || symbol == IFSY || symbol == DOSY || symbol == FORSY || symbol == LBRACESY
          || symbol == SCANFSY || symbol == PRINTFSY || symbol == SEMISY || symbol == RETURNSY){
            statement();
          }
}
void statement(){
    fprintf(output_grammar_pointer,"This is a statement!\n");
    if(symbol == IFSY)
        condition_statement();
    else if(symbol == DOSY || symbol == FORSY)
        loop_statement();
    else if(symbol == LBRACESY){
        getsym();
        statement_lists();
        if(symbol != RBRACESY){
            //printf("111");
            error(22);//��Ҫ�һ�����
            return;
        }
        getsym();
    }
    else if(symbol == SCANFSY){
        read_statement();
        if(symbol != SEMISY){
            error(13);
            skip(3);
            return;
        }
        getsym();
    }
    else if(symbol == PRINTFSY){
        write_statement();
        if(symbol != SEMISY){
            error(13);
            skip(3);
            return;
        }
        getsym();
    }
    else if(symbol == RETURNSY){
        return_statement();
        if(symbol != SEMISY){
            error(13);
            skip(3);
            return;
        }
        getsym();
    }
    else if(symbol == IDSY){
        strcat(pre_token,token);
        strcat(pre_token," ");
        getsym();
        if(symbol == LPARSY){
            clearPreToken();
            getsym();
            function_call_statement();
            if(symbol != SEMISY){
                error(13);
                skip(3);
                return;
            }
            getsym();
        }
        else{
            clearPreToken();
            getsym();
            assign_statement();
            if(symbol != SEMISY){
                error(13);
                skip(3);
                return;
            }
            getsym();
        }
    }
    else if(symbol == SEMISY){
        getsym();
    }
    else{
        error(13);//��Ҫһ���ֺ�
        skip(3);
        return;
    }
}
void condition_statement(){
    TableEle *te, *te_label1, *te_label2;
    fprintf(output_grammar_pointer,"This is a condition statement!\n");
    if(symbol != IFSY){
        error(24);//ȱ��һ��IF
    }
    getsym();
    if(symbol != LPARSY){
        error(19);
    }
    getsym();
    te = condition();
    if(symbol != RPARSY){
        error(20);
    }
    //ngoto:��teΪ�٣�����ת��label1
    te_label1 = create_new_label();
    write_quadruple_d(NGOTO,te,te_label1);
    getsym();
    statement();
    te_label2 = create_new_label();
    write_quadruple_s(GOTO,te_label2);//���ɲ���ת��label2
    write_quadruple_s(LAB,te_label1);//����label1
    if(symbol == ELSESY){
        getsym();
        statement();
    }
    write_quadruple_s(LAB,te_label2);//����label2
}
TableEle* condition(){
    TableEle *te1, *te2, *te;
    fprintf(output_grammar_pointer,"This is a condition!\n");
    te1 = expression();
    if(symbol == LSSY || symbol == BGSY || symbol == NLSSY || symbol == NBGSY || symbol == EQUSY || symbol == NEQSY){
        if(symbol == LSSY){
            getsym();
            te2 = expression();
            te = create_new_tmp(1,addr);
            write_quadruple_t(LS,te1,te2,te);
            te1 = te;
        }
        else if(symbol == BGSY){
            getsym();
            te2 = expression();
            te = create_new_tmp(1,addr);
            write_quadruple_t(BG,te1,te2,te);
            te1 = te;
        }
         else if(symbol == NLSSY){
            getsym();
            te2 = expression();
            te = create_new_tmp(1,addr);
            write_quadruple_t(NLS,te1,te2,te);
            te1 = te;
        }
         else if(symbol == NBGSY){
            getsym();
            te2 = expression();
            te = create_new_tmp(1,addr);
            write_quadruple_t(NBG,te1,te2,te);
            te1 = te;
        }
         else if(symbol == EQUSY){
            getsym();
            te2 = expression();
            te = create_new_tmp(1,addr);
            write_quadruple_t(EQU,te1,te2,te);
            te1 = te;
        }
         else if(symbol == NEQSY){
            getsym();
            te2 = expression();
            te = create_new_tmp(1,addr);
            write_quadruple_t(NEQ,te1,te2,te);
            te1 = te;
        }
    }
    return te1;
}
void loop_statement(){
    TableEle *te_label, *te_cond, *te1, *te2, *te_exp, *te_label2, *te_step, *te_label3, *te_label4;
    fprintf(output_grammar_pointer,"This is a loop statement!\n");
    if(symbol == DOSY){
        te_label = create_new_label();
        write_quadruple_s(LAB,te_label);
        getsym();
        statement();
        if(symbol != WHILESY){
            error(25); //error 25:ȱ��WHILE
            return;
        }
        getsym();
        if(symbol != LPARSY)
            error(19);
        getsym();
        te_cond = condition();
        if(symbol != RPARSY)
            error(20);
        write_quadruple_d(TGOTO,te_cond,te_label);//���������ת
        getsym();
    }
    else if(symbol == FORSY){
        getsym();
        if(symbol != LPARSY){
            error(19);
        }
        getsym();
        do{
            if(symbol != IDSY){
                error(8);
                skip(3);
                break;
            }
            else{
            //����ű�
                te1 = search_all(token,table_num);
                getsym();
                if(symbol != ISSY){
                    error(14);
                    skip(3);
                    break;
                }
                getsym();
                te_exp = expression();
                write_quadruple_d(ASN,te_exp,te1);//��ֵ��䣬��te_exp���浽te1
            }
        }while(0);
        if(symbol != SEMISY){
            error(13);
            skip(3);
        }
        getsym();
        te_label2 = create_new_label();
        write_quadruple_s(LAB,te_label2);//����label2
        te_cond = condition();
        if(symbol != SEMISY){
            error(13);
            skip(3);
        }
        getsym();
        te_label = create_new_label();
        write_quadruple_d(NGOTO,te_cond,te_label);//������ת�����ĩ
        te_label4 = create_new_label();
        write_quadruple_s(GOTO,te_label4);//����������俪ʼ
        te_label3 = create_new_label();
        write_quadruple_s(LAB,te_label3);//����label3����־step��
        if(symbol != IDSY){
            error(8);
            skip(3);
            getsym();
            return;
        }
        te1 = search_all(token,table_num);
        getsym();
        if(symbol != ISSY){
            error(14);
            skip(3);
            getsym();
            return;
        }
        getsym();
        if(symbol != IDSY){
            error(8);
            skip(3);
            getsym();
            return;
        }
        //ע����������ǲ�һ���ı�ʶ��
        te2 = search_all(token,table_num);
        getsym();
        if(symbol != PLUSSY && symbol != MINUSSY){
            error(26);
            skip(3);
            getsym();
            return;
        }
        if(symbol == PLUSSY){
            getsym();
            te_step = step();
            te_exp = create_new_tmp(1,addr);
            write_quadruple_t(ADD,te2,te_step,te_exp);//�������ͱ�ʶ����ӷ���te_exp

        }
        else if(symbol == MINUSSY){
            getsym();
            te_step = step();
            te_exp = create_new_tmp(1,addr);
            write_quadruple_t(SUB,te2,te_step,te_exp);//����ʶ���Ͳ����������te_exp
        }
        write_quadruple_d(ASN,te_exp,te1);//����te_exp��te1
        if(symbol != RPARSY){
            error(20);
            skip(3);
            getsym();
            return;
        }
        getsym();
        write_quadruple_s(GOTO,te_label2);//��ת��������label2��
        write_quadruple_s(LAB,te_label4);//������俪ʼ��ǩ
        statement();
        write_quadruple_s(GOTO,te_label3);//��ת��step
        write_quadruple_s(LAB,te_label);//���ĩ
    }
    else{
        error(27);
        skip(3);
            getsym();
            return;
    }
}
TableEle* step(){
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a step!\n");
    if(symbol != INTCON){
        error(15);
    }
    if(num == 0){
        error(30);
    }
    te = create_new_tmp(0,num);
    getsym();
    return te;
}
int function_call_statement(){
    int type, i;
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a function call statement!\n");
    if(symbol != IDSY){
        error(8);
        skip(4);
        return;
    }
    te = search_all(token,table_num);
    getsym();
    if(te->type == VOID){
        type = VOID;
    }
    else if(te->type == INT){
        type = INT;
    }
    else{
        type = CHAR;
    }


    if(symbol != LPARSY){
        error(19);
        skip(4);
        return;
    }
    getsym();
    value_para_table(te);
    write_quadruple_s(CALL,te);//����������䣬�з���ֵ�ĺ���ֵ�ڵ�����Ϻ󱣴���$v0
    if(symbol != RPARSY){
        error(20);
        skip(4);
        return;
    }
    getsym();
    return type;
}
void value_para_table(TableEle* te){
    TableEle* t_te, *tmp;
    TableEle t_te2;
    int i=0;
    fprintf(output_grammar_pointer,"This is a value parameters table!\n");
    if(symbol == PLUSSY || symbol == MINUSSY || symbol == IDSY || symbol == INTCON || symbol == CHARCON
       || symbol == LPARSY){
        t_te = expression();
        t_te2 = tbl[te->addr].table_ele[i];
        i++;
        if(t_te2.kind!=PARA){
            error(40);//������������
            skip(5);
            return;
        }
        //printf("first addr: %d\n",t_te2.addr);
        if(t_te2.type != t_te->type){
            error(41);
            skip(5);
            return;
        }
        tmp = create_new_tmp(1,addr);
        tmp->addr = t_te2.addr;
        write_quadruple_d(PUSH,t_te,tmp);//����ѹջ
        //���������͡�����
        while(symbol == COMMASY){
            getsym();
            t_te = expression();
            t_te2 = tbl[te->addr].table_ele[i];
            i++;
            if(t_te2.kind!=PARA){
                error(40);//������������
                skip(5);
                return;
            }
            if(t_te2.type != t_te->type){
                error(41);
                skip(5);
                return;
            }
            //printf("next addr: %d\n",t_te2.addr);
            tmp = create_new_tmp(1,addr);
            tmp->addr = t_te2.addr;
            write_quadruple_d(PUSH,t_te,tmp);//����ѹջ
        }
        t_te2 = tbl[te->addr].table_ele[i];
        if(t_te2.kind == PARA)
            error(40);
    }
}
void assign_statement(){
    TableEle *te1, *te2, *te;
    fprintf(output_grammar_pointer,"This is an assign statement!\n");
    if(symbol != IDSY){
        error(8);
        skip(4);
        return;
    }
    te1 = search_all(token,table_num);
    if(te1->kind != VAR && te1->kind != PARA && te1->kind != ARRAY
       && te1->kind != GLOARR && te1->kind != GLOVAR){
        error(38);
        skip(4);
        return;
       }
    getsym();
    if(symbol == LBRACKETSY){
        if(te1->kind != ARRAY && te1->kind != GLOARR){
            error(38);
            skip(4);
            return;
        }
        getsym();
        te2 = expression();
        if(symbol != RBRACKETSY){
            error(18);
            skip(4);
            return;
        }
        getsym();
    }
    else{
        if(te1->kind == ARRAY || te1->kind == GLOARR){
            error(38);
            skip(4);
            return;
        }
    }
    if(symbol != ISSY){
        error(14);
        skip(4);
        return;
    }
    getsym();
    te = expression();
    //����ֵ���
    if(te1->kind == ARRAY || te1->kind == GLOARR)
        write_quadruple_t(STA,te1,te2,te);//�����鸳ֵ����䣬��te��ֵ���浽te1[te2]��ʵ��Ϊ��te����addr(te1)+4*te2
    else
        write_quadruple_d(ASN,te,te1);//��ֵ��䣬��te��ֵ���浽t1
}
void read_statement(){
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a read statement!\n");
    if(symbol != SCANFSY){
        error(28);
        skip(4);
        return;
    }
    getsym();
    if(symbol != LPARSY){
        error(19);
        skip(4);
        return;
    }
    getsym();
    if(symbol != IDSY){
        error(8);
        skip(4);
        return;
    }
    te = search_all(token,table_num);
    write_quadruple_s(RD,te);//�����
    getsym();
    while(symbol == COMMASY){
        getsym();
        if(symbol != IDSY){
            error(8);
            skip(4);
            return;
        }
        te = search_all(token,table_num);
        write_quadruple_s(RD,te);
        getsym();
    }
    if(symbol != RPARSY){
        error(20);
        skip(4);
        return;
    }
    getsym();
}
void write_statement(){
    TableEle* te;

    fprintf(output_grammar_pointer,"This is a write statement!\n");
    if(symbol != PRINTFSY){
        error(29);
        skip(4);
        return;
    }
    getsym();
    if(symbol != LPARSY){
        error(19);
        skip(4);
        return;
    }
    getsym();
    if(symbol == STRINGCON){
        te = create_new_string(token);
        strcpy(strings[string_num],token);//���ɴ���ʱֱ����.data���м���string�����ֿ���������±�õ�
        write_quadruple_s(WRS,te);//WRS:д�ַ�������ֱ�����
        getsym();
        if(symbol == COMMASY){
            getsym();
            te = expression();
            if(te->type==CHAR)
                write_quadruple_s(WRC,te);//WRTC:д�ַ����ʽ������ȡ��ʱ������ֵ�������
            else
                write_quadruple_s(WRI,te);//WRTI:д�������ʽ
        }
        if(symbol != RPARSY){
            error(20);
            skip(4);
            return;
        }
        getsym();
    }
    else{
        te = expression();
        if(te->type==CHAR)
            write_quadruple_s(WRC,te);
        else
            write_quadruple_s(WRI,te);
        if(symbol != RPARSY){
            error(20);
            skip(4);
            return;
        }
        getsym();
    }
}
void return_statement(){
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a return statement!\n");
    if(symbol != RETURNSY){
        error(32);
        skip(4);
        return;
    }
    getsym();
    if(symbol == LPARSY){
        getsym();
        te = expression();
        if(symbol != RPARSY){
            error(20);
            skip(4);
            return;
        }
        getsym();
        write_quadruple_s(RRT,te);//�з���ֵ��䣬Ҫ��ֵ����$v0
        if(te->type == INT)
            return_num = 2;
        else if(te->type == CHAR)
            return_num = 3;
    }
    else{
        write_quadruple(VRT);
        return_num = 1;
    }
}
void v_func_dcl(){
    TableEle* te;
    char id[MAXTokenL];
    fprintf(output_grammar_pointer,"This is a declaration of a function with no returns!\n");
    if(symbol != VOIDSY){
        error(33);
    }
    getsym();
    if(symbol != IDSY){
        error(8);
        skip(6);
        return;
    }
    strcpy(id,token);
    te = enter_table(id,FUNC,VOID,table_num+1,0,0);
    write_quadruple_s(FDCL,te);//����������ʼ
    addr = 0;
    table_num++;
    getsym();
    if(symbol != LPARSY){
        error(19);
        skip(6);
        return;
    }
    getsym();
    para();
    if(symbol != RPARSY){
        error(20);
        skip(6);
        return;
    }
    getsym();
    if(symbol != LBRACESY){
        error(21);
        skip(6);
        return;
    }
    getsym();
    comp_statement();
    if(symbol != RBRACESY){
        error(22);
        skip(6);
        return;
    }
    if(return_num == 2 || return_num == 3){
        error(43);
        skip(6);
        return;
    }
    write_quadruple(END);//������������
    return_num = 0;
    getsym();
}
void func_main(){
    TableEle* te;
    fprintf(output_grammar_pointer,"This is a main function!\n");
    if(symbol != VOIDSY){
        error(33);
    }
    getsym();
    if(symbol != MAINSY){
        error(34);
    }
    te = enter_table(token,FUNC,VOID,table_num,0,0);
    write_quadruple(MDCL);//����main������ʼ
    table_num++;
    addr = 0;
    getsym();
    if(symbol != LPARSY){
        error(19);
    }
    getsym();
    if(symbol != RPARSY){
        error(20);
    }
    getsym();
    if(symbol != LBRACESY){
        error(21);
    }
    getsym();
    comp_statement();
    if(symbol != RBRACESY){
//            printf("333");
        error(22);
    }
    write_quadruple(MEND);//������������
    if(return_num == 2 || return_num == 3)
        error(43);
    getsym();
    return_num = 0;
}
TableEle* expression(){
    int sy = 1;
    TableEle *te1, *te2, *te;
    fprintf(output_grammar_pointer,"This is an expression!\n");
    if(symbol == PLUSSY){
        //to do here
        getsym();
    }
    else if(symbol == MINUSSY){
        //TO DO HERE
        sy = -1;
        getsym();
    }
    te1 = term();
    if(sy == -1){
        te = create_new_tmp(1,addr);
        write_quadruple_d(NEG, te1, te);
        te1 = te;
    }
    while(symbol == PLUSSY || symbol == MINUSSY){
        if(symbol == MINUSSY)
            sy = -1;
        else
            sy = 1;
        getsym();
        te2 = term();
        if(sy == -1){
            te = create_new_tmp(1,addr);
            write_quadruple_t(SUB,te1,te2,te);
        }
        else{
            te = create_new_tmp(1,addr);
            write_quadruple_t(ADD,te1,te2,te);
        }
        te1 = te;
    }
    return te1;
}
TableEle* term(){
    TableEle *te, *te1, *te2;
    fprintf(output_grammar_pointer,"This is a term!\n");
    te1 = factor();
    while(symbol == STARSY || symbol == SLASHSY){
        if(symbol == STARSY){
            getsym();
            te2 = factor();
            te = create_new_tmp(1,addr);
            write_quadruple_t(MUL,te1,te2,te);
            te1 = te;
        }
        else{
            getsym();
            te2 = factor();
            te = create_new_tmp(1,addr);
            write_quadruple_t(DIV,te1,te2,te);
            te1 = te;
        }
    }
    return te1;
}
TableEle* factor(){
    TableEle *te, *t_te1, *t_te2;
    char id[MAXTokenL];
    int rt;
    fprintf(output_grammar_pointer,"This is a factor!\n");
    if(symbol == PLUSSY || symbol == MINUSSY || symbol == INTCON){
        integer();
        te = create_new_tmp(0,num);
    }
    else if(symbol == CHARCON){
        te = create_new_tmp(2,(int)token[0]);
        getsym();
    }
    else if(symbol == LPARSY){
        getsym();
        te = expression();
        if(symbol != RPARSY){
            error(20);
        }
        else
            getsym();
    }
    else if(symbol == IDSY){
        strcat(pre_token,token);
        strcpy(id,token);
        strcat(pre_token," ");
        getsym();
        if(symbol == LPARSY){
            clearPreToken();
            getsym();
            rt = function_call_statement();
            if(rt == VOID)
                error(39);
            else if(rt == CHAR)
               te = create_new_tmp(3,addr);
            else
                te = create_new_tmp(1,addr);
            write_quadruple_s(MFF,te);
            //��ԪʽMFF te���Ѻ����ķ���ֵ��v0�Ĵ����е�ֵ���Ž�te��
            //ת��ΪĿ�����ʱ�� MOV v0 tmp
        }
        else if(symbol == LBRACKETSY){
            //��pre_token����ű�
            t_te1 = search_all(id,table_num);
            //ȡ��������
            if(t_te1->kind != ARRAY && t_te1->kind != GLOARR){
                error(44);
            }
            pre_token[0]='\0';
            getsym();
            t_te2 = expression();
            if(t_te1->type==INT)
                te = create_new_tmp(1,addr);
            else
                te = create_new_tmp(3,addr);
            write_quadruple_t(LFA, t_te1, t_te2, te);
            //ȡ����������±�ֵ
            //��������ȡֵ
            //��Ԫʽ
            //LFA ����(t_te1) ƫ��(t_te2) tmp
            if(symbol != RBRACKETSY){
                error(18);
            }
            getsym();
        }
        else{
            //��pre_token����ű�
            te = search_all(id,table_num);
            pre_token[0] = '\0';
        }
    }
    else{
        te = create_new_tmp(1,addr);
        error(23);
        skip(3);
    }
    return te;
}
void integer(){
    fprintf(output_grammar_pointer,"This is an integer!\n");
    if(symbol == PLUSSY){
        getsym();
        if(symbol == INTCON && num != 0){
            getsym();
        }
        else{
            error(15);
            skip(3);
        }
    }
    else if(symbol == MINUSSY){
        getsym();
        if(symbol == INTCON && num != 0){
            num = -1 * num;
            getsym();
        }
        else{
            error(15);
            skip(3);
        }
    }
    else if(symbol == INTCON)
        getsym();
    else{
        error(31);
    }
}

