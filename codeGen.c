#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"
#include "parser.h"

int r=0;

int evaluateTree(BTNode *root) {
    int ren = 0, ln = 0, rn = 0;
	int i=0;
    if (root != NULL) {
        switch (root->data) {
            case ID:
            	ren=r;
            	r++;
            	for(i=0;i<sbcount;i++)
            	{
            		if(strcmp(table[i].name, root->lexeme)==0)
            		{
            			root->val=table[i].val;
            			printf("MOV r%d [%d]\n", ren, i*4);
            			break;
					}
				}
                if(i==sbcount) error(INVALID);
                break;
            case INT:
                ren=r;
                r++;
                root->val=atoi(root->lexeme);
                printf("MOV r%d %d\n", ren, root->val);
                break;
            case ASSIGN:
            	rn = evaluateTree(root->right);
            	root->val=root->left->val=root->right->val;
				ren=rn;
            	for(i=0;i<sbcount;i++)
            	{
            		if(strcmp(table[i].name, root->left->lexeme)==0)
            		{
            			table[i].val=root->val;
            			printf("MOV [%d] r%d\n", i*4, ren);
            			break;
					}
				}
                if(i==sbcount)
                {
                	root->val=setval(root->left->lexeme, root->left->val);
                	printf("MOV [%d] r%d\n", i*4, ren);
				}
                break;
            case ADDSUB:
            case MULDIV:
                ln = evaluateTree(root->left);
                rn = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+") == 0) {
                    ren=ln;
                    root->val = root->left->val + root->right->val;
                    printf("ADD r%d r%d\n", ln, rn);
                    r--;

                } else if (strcmp(root->lexeme, "-") == 0) {
                    ren=ln;
                    root->val = root->left->val - root->right->val;
                    printf("SUB r%d r%d\n", ln, rn);
                    r--;

                } else if (strcmp(root->lexeme, "*") == 0) {
                    ren=ln;
                    root->val = root->left->val * root->right->val;
                    printf("MUL r%d r%d\n", ln, rn);
                    r--;

                } else if (strcmp(root->lexeme, "/") == 0) {
                    ren=ln;
                    if (root->right->val == 0)
                        error(DIVZERO);
                    root->val = root->left->val / root->right->val;
                    printf("DIV r%d r%d\n", ln, rn);
                    r--;

                }
                break;
            case OR:
            case AND:
            case XOR:
            	ln=evaluateTree(root->left);
            	rn=evaluateTree(root->right);
            	if(strcmp(root->lexeme, "|")==0)
            	{
            	    ren=ln;
            		root->val=root->left->val | root->right->val;
            		printf("OR r%d r%d\n", ln, rn);
            		r--;

				}
				else if(strcmp(root->lexeme, "&")==0)
				{
				    ren=ln;
					root->val=root->left->val & root->right->val;
            		printf("AND r%d r%d\n", ln, rn);
					r--;

				}
				else if(strcmp(root->lexeme, "^")==0)
				{
				    ren=ln;
					root->val=root->left->val ^ root->right->val;
            		printf("XOR r%d r%d\n", ln, rn);
            		r--;

				}
				break;
            case INCDEC:
            	ren=evaluateTree(root->left);
            	rn=evaluateTree(root->right);

				if (strcmp(root->lexeme, "++") == 0) {
                    root->left->val++;
                    root->val=root->left->val;
                    root->left->val=setval(root->left->lexeme, root->left->val);
                    printf("ADD r%d r%d\n",ren, rn);
                    r--;
                    //root->val = root->left->val + root->right->val;
                    //printf("ADD r%d r%d\n", ren, rn);
                    //r--;
                    //ren=ln;
                } else if (strcmp(root->lexeme, "--") == 0) {
                    root->left->val--;
                    root->val=root->left->val;
                    root->left->val=setval(root->left->lexeme, root->left->val);
                    printf("SUB r%d r%d\n",ren, rn);
                    r--;
                    //root->val = root->left->val - root->right->val;
                    //printf("SUB r%d r%d\n", ln, rn);
                    //r--;
                    //ren=ln;
                }

				for(i=0;i<sbcount;i++)
				{
					if(strcmp(root->left->lexeme, table[i].name)==0)
					{
						root->left->val=table[i].val;
						printf("MOV [%d] r%d\n",i*4, ren);
						break;
						//table[i].val=root->val;
						//printf("MOV [%d] r%d\n", i*4, ren);
						//break;
					}
				}
                if(i==sbcount) error(INVALID);
				break;

            default:
                //ren = 0;
                break;
        }
    }
    return ren;
}

void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}
