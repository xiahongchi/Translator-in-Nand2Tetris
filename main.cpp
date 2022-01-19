#include <stdio.h>
#include <cstring>
using namespace std;

//class SymbolTable
class SymbolTable{
    struct AvlNode
    {
        char data[50];
        int line;
        AvlNode *left;
        AvlNode *right;
        int height; //结点的高度
        AvlNode( const char* element , int lineofx,
        AvlNode *lt, AvlNode *rt, int h=1):  line(lineofx),left( lt ), right( rt ), height(h) {
            int k=0;
            while(element[k]){
                data[k]=element[k];
                k++;
            }
            data[k]='\0';
        }
    };
    AvlNode *root;
public:
    SymbolTable() { root = NULL; pre_def();}
    ~SymbolTable( ) { makeEmpty( root); }
    int find( const char* x ) const;
    bool insert( const char* x ,int lineofx) ;

private:
    void pre_def();
    bool insert( const char* x,int lineofx, AvlNode * & t ) ;
    void makeEmpty( AvlNode *&t );
    int height( AvlNode *t ) const { return t == NULL ? 0 : t->height;}
    void LL( AvlNode * & t );
    void LR( AvlNode * & t );
    void RL( AvlNode * & t );
    void RR( AvlNode * & t );
    int max(int a, int b) {return (a>b)?a:b;}
};

void SymbolTable::pre_def(){
    insert("R0",0);
    insert("R1",1);
    insert("R2",2);
    insert("R3",3);
    insert("R4",4);
    insert("R5",5);
    insert("R6",6);
    insert("R7",7);
    insert("R8",8);
    insert("R9",9);
    insert("R10",10);
    insert("R11",11);
    insert("R12",12);
    insert("R13",13);
    insert("R14",14);
    insert("R15",15);
    insert("SCREEN",16384);
    insert("KBD",24576);
    insert("SP",0);
    insert("LCL",1);
    insert("ARG",2);
    insert("THIS",3);
    insert("THAT",4);
}

void SymbolTable::makeEmpty( struct SymbolTable::AvlNode *&t ){
    if(t==NULL) return;
    else{
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t=NULL;
    }
}

int SymbolTable::find(const char* x ) const
{
    AvlNode *t = root;
    while (t!=NULL){
        int k=strcmp(x,t->data);
        if(k==0) break;
        if(k<0) t = t->left;
        else t = t->right;
    }
    if (t==NULL) return -1;
    else return t->line;
}


bool SymbolTable::insert( const char* x, int lineofx) {
    return insert(x,lineofx,root);
}


bool SymbolTable::insert( const char* x, int lineofx, AvlNode * & t )
{
    bool flag=0;
    int k=0;
    if( t == NULL ){ //在空树上插入
        t = new AvlNode( x, lineofx, NULL, NULL); //* & t 保证插入与父节点的关联
        flag=0;
    }
    else if((k=strcmp(x,t->data))<0) { //x小于data 在左子树上插入
        flag=insert( x,lineofx, t->left );
        if ( height( t->left ) - height( t->right ) == 2 ){//此刻t失衡，必然是左高右低
            if( strcmp(x , t->left->data )<0) LL( t );
            else LR(t); //左子树插LL，否则LR
            flag=1;
        }
    }
    else if( k>0 ) { //在右子树上插入
        flag=insert( x, lineofx,t->right );
        if( height( t->right ) - height( t->left ) == 2 ){ //此刻t失衡，必然是右高左低
            if( strcmp(x , t->right->data )>0 ) RR(t);
            else RL(t); //右子树插RR，否则RL
            flag=1;
        }
    }
    //重新计算t的高度
    t->height = max( height( t->left ) , height( t->right ) ) + 1;
    return flag;
}

void SymbolTable::LL( AvlNode * & t ) {
    AvlNode *t1 = t->left; // 未来的树根
    t->left = t1->right; //t的左子树成为原根结点的右子树
    t1->right = t; //原根结点成为其左子结点的右子树
    t->height = max( height( t->left ), height( t->right ) ) + 1;
    t1->height = max( height( t1->left ), height(t)) + 1;
    t = t1; //重新定义子树树根
}

void SymbolTable::RR( AvlNode * & t ) {
    AvlNode *t1 = t->right; // 未来的树根
    t->right = t1->left; //t的右子树成为原根结点的左子树
    t1->left = t; //原根结点成为其右子结点的左子树
    t->height = max( height( t->left ), height( t->right ) ) + 1;
    t1->height = max( height( t1->right ), height(t)) + 1;
    t = t1; //重新定义子树树根
}

void SymbolTable::LR( AvlNode * & t )
{
    RR( t->left );
    LL( t );
}

void SymbolTable::RL( AvlNode * & t )
{
    LL( t->right );
    RR( t );
}

//class File
class File{
    struct Node{
        int line;
        Node* next;
        char code[50];
    };
    Node*head;
    int size;
    SymbolTable st;
public:

    File(){
        head=new Node;
        head->next=NULL;
        size=0;
    }

    void BuildFile(){

        Node*p=head;

        FILE *fp;
        if ( (fp = fopen("D:\\ComputerScience\\nand2tetris\\nand2tetris\\projects\\08\\FunctionCalls\\FibonacciElement\\FibonacciElement.asm", "rt")) == NULL ) {
            puts("Fail to open file!");
        }

        char ch;
        int nline=-1;

        while( (ch=fgetc(fp)) != EOF ){
            if(ch=='/') SkipLine(fp);
            else if(ch=='\n') continue;
            else{
                if(ch=='('){
                   p=p->next=new Node;
                   p->line=nline+1;
                   p->next=NULL;
                   int k=0;
                   p->code[k]='(';
                   while((ch=fgetc(fp)) != ')'){
                       k++;
                       p->code[k]=ch;
                   }
                   p->code[k+1]=')';
                   p->code[k+2]='\0';
                   SkipLine(fp);
                }
                else{
                    while(ch!='@'&&ch!='A'&&ch!='D'&&ch!='M'&&ch!='0'&&ch!='1'&&ch!='-'&&ch!='!') ch=fgetc(fp);
                    int k=0;
                    p=p->next=new Node;
                    p->line=++nline;
                    p->next=NULL;
                    while(ch!=' '&&ch!='\n'&&ch!=EOF){
                        p->code[k]=ch;
                        ch=fgetc(fp);
                        k++;
                    }
                    p->code[k]='\0';
                    if(ch=='\n') continue;
                    if(ch!=EOF) SkipLine(fp);
                    else break;
                }
            }
        }
    }

    void PrintFile(FILE*fp){
        Node*p=head->next;
        while(p){
            fprintf(fp,p->code);
            fprintf(fp,"\n");
            p=p->next;
        }
    }

    void Translate(){
        Nosymbol();
    }

    void FillAddn(Node*p,int addn){
        p->code[16]='\0';
        p->code[0]='0';
        for(int i=15;i>0;i--){
            p->code[i]=addn%2+'0';
            addn/=2;
        }
    }

    void FillDest(Node*p,int start,int end,char ins[]){
        //ins10 11 12 A D M
        ins[10]='0';ins[11]='0';ins[12]='0';
        for(int k=start;k<=end;k++){
            if(p->code[k]=='A') ins[10]='1';
            else if(p->code[k]=='D') ins[11]='1';
            else ins[12]='1';
        }
    }

    void FillComp(Node*p,int start,int end,char ins[]){
        //ins3->ins9
        if(start==end){//len=1
            //0
            if(p->code[start]=='0'){
                strcpy(ins+3,"010101"); ins[9]='0';
            }
            //1
            else if(p->code[start]=='1'){
                strcpy(ins+3,"011111");ins[9]='1';
            }
            //D
            else if(p->code[start]=='D'){
                strcpy(ins+3,"000110");ins[9]='0';
            }
            //A
            else if(p->code[start]=='A'){
                strcpy(ins+3,"011000");ins[9]='0';
            }
            //M
            else if(p->code[start]=='M'){
                strcpy(ins+3,"111000");ins[9]='0';
            }
        }
        else if(start+1==end){//len=2
            //-1
            if(p->code[start]=='-'&&p->code[end]=='1'){
                strcpy(ins+3,"011101");ins[9]='0';
            }
            //!D
            else if(p->code[start]=='!'&&p->code[end]=='D'){
                strcpy(ins+3,"000110");ins[9]='1';
            }
            //!A
            else if(p->code[start]=='!'&&p->code[end]=='A'){
                strcpy(ins+3,"011000");ins[9]='1';
            }
            //-D
            else if(p->code[start]=='-'&&p->code[end]=='D'){
                strcpy(ins+3,"000111");ins[9]='1';
            }
            //-A
            else if(p->code[start]=='-'&&p->code[end]=='A'){
                strcpy(ins+3,"011001");ins[9]='1';
            }
            //!M
            else if(p->code[start]=='!'&&p->code[end]=='M'){
                strcpy(ins+3,"111000");ins[9]='1';
            }
            //-M
            else if(p->code[start]=='-'&&p->code[end]=='M'){
                strcpy(ins+3,"111001");ins[9]='1';
            }
        }
        else if(start+2==end){//len=3
            //D+1
            if(p->code[start]=='D'&&p->code[start+1]=='+'&&p->code[end]=='1'){
                strcpy(ins+3,"001111");ins[9]='1';
            }
            //A+1
            else if(p->code[start]=='A'&&p->code[start+1]=='+'&&p->code[end]=='1'){
                strcpy(ins+3,"011011");ins[9]='1';
            }
            //D-1
            else if(p->code[start]=='D'&&p->code[start+1]=='-'&&p->code[end]=='1'){
                strcpy(ins+3,"000111");ins[9]='0';
            }
            //A-1
            else if(p->code[start]=='A'&&p->code[start+1]=='-'&&p->code[end]=='1'){
                strcpy(ins+3,"011001");ins[9]='0';
            }
            //D+A
            else if(p->code[start]=='D'&&p->code[start+1]=='+'&&p->code[end]=='A'){
                strcpy(ins+3,"000001");ins[9]='0';
            }
            //D-A
            else if(p->code[start]=='D'&&p->code[start+1]=='-'&&p->code[end]=='A'){
                strcpy(ins+3,"001001");ins[9]='1';
            }
            //A-D
            else if(p->code[start]=='A'&&p->code[start+1]=='-'&&p->code[end]=='D'){
                strcpy(ins+3,"000011");ins[9]='1';
            }
            //D&A
            else if(p->code[start]=='D'&&p->code[start+1]=='&'&&p->code[end]=='A'){
                strcpy(ins+3,"000000");ins[9]='0';
            }
            //D|A
            else if(p->code[start]=='D'&&p->code[start+1]=='|'&&p->code[end]=='A'){
                strcpy(ins+3,"001010");ins[9]='1';
            }
            //M+1
            else if(p->code[start]=='M'&&p->code[start+1]=='+'&&p->code[end]=='1'){
                strcpy(ins+3,"111011");ins[9]='1';
            }
            //M-1
            else if(p->code[start]=='M'&&p->code[start+1]=='-'&&p->code[end]=='1'){
                strcpy(ins+3,"111001");ins[9]='0';
            }
            //D+M
            else if(p->code[start]=='D'&&p->code[start+1]=='+'&&p->code[end]=='M'){
                strcpy(ins+3,"100001");ins[9]='0';
            }
            //D-M
            else if(p->code[start]=='D'&&p->code[start+1]=='-'&&p->code[end]=='M'){
                strcpy(ins+3,"101001");ins[9]='1';
            }
            //M-D
            else if(p->code[start]=='M'&&p->code[start+1]=='-'&&p->code[end]=='D'){
                strcpy(ins+3,"100011");ins[9]='1';
            }
            //D&M
            else if(p->code[start]=='D'&&p->code[start+1]=='&'&&p->code[end]=='M'){
                strcpy(ins+3,"100000");ins[9]='0';
            }
            //D|M
            else if(p->code[start]=='D'&&p->code[start+1]=='|'&&p->code[end]=='M'){
                strcpy(ins+3,"101010");ins[9]='1';
            }
        }
    }

    void FillJump(Node*p,int start,int end,char ins[]){
        //ins 13 14 15
        if(p->code[start+1]=='G'&&p->code[end]=='T'){
            ins[13]='0';ins[14]='0';ins[15]='1';
        }
        else if(p->code[start+1]=='E'&&p->code[end]=='Q'){
            ins[13]='0';ins[14]='1';ins[15]='0';
        }
        else if(p->code[start+1]=='G'&&p->code[end]=='E'){
            ins[13]='0';ins[14]='1';ins[15]='1';
        }
        else if(p->code[start+1]=='L'&&p->code[end]=='T'){
            ins[13]='1';ins[14]='0';ins[15]='0';
        }
        else if(p->code[start+1]=='N'&&p->code[end]=='E'){
            ins[13]='1';ins[14]='0';ins[15]='1';
        }
        else if(p->code[start+1]=='L'&&p->code[end]=='E'){
            ins[13]='1';ins[14]='1';ins[15]='0';
        }
        else{
            ins[13]='1';ins[14]='1';ins[15]='1';
        }
    }

    void ZeroDest(char ins[]){
        ins[10]='0';ins[11]='0';ins[12]='0';
    }

    void ZeroJump(char ins[]){
        ins[13]='0';ins[14]='0';ins[15]='0';
    }

    void FillP(Node*p,char ins[]){
        p->code[0]='1';p->code[1]='1';p->code[2]='1';
        p->code[16]='\0';
        for(int k=3;k<=15;k++) p->code[k]=ins[k];
    }

    void Nosymbol(){
        //1st SCAN
        Node*p=head->next,*q=head;
        while(p){
            if(p->code[0]=='('){
                int k=0;
                char str[50];
                while(p->code[k+1]!=')'){
                    str[k]=p->code[k+1];
                    k++;
                }
                str[k]='\0';
                if(st.find(str)==-1) st.insert(str,p->line);
                Node*del=p;
                q->next=p->next;
                delete del;
                p=q->next;
                continue;
            }
            p=p->next;
            q=q->next;
        }
        printf("Finish 1st SCAN\n");

        //2nd SCAN
        int newmry=16;
        p=head->next;

        while(p){

            if(p->code[0]=='@'){
                if(p->code[1]>='0'&&p->code[1]<='9'){
                    int k=1,addn=0;
                    while(p->code[k]!='\0'){
                        addn=addn*10+p->code[k]-'0';
                        k++;
                    }
                    FillAddn(p,addn);
                }
                else{
                    int addn=st.find(p->code+1);
                    if(addn==-1){
                        st.insert(p->code+1,newmry);
                        FillAddn(p,newmry);
                        newmry++;
                    }
                    else FillAddn(p,addn);
                }
            }

            else{
                int k=0,s=0;
                char ins[17];
                ins[16]='\0';

                while(p->code[k]!='\0'&&p->code[k]!='='&&p->code[k]!=';') k++;

                //with dest
                if(p->code[k]=='='){
                    //deal with dest
                    FillDest(p,s,k-1,ins);
                    k++;
                    s=k;
                    //cont to scan
                    while(p->code[k]!='\0'&&p->code[k]!=';') k++;
                    //with jump
                    if(p->code[k]==';'){
                        //deal with comp
                        FillComp(p,s,k-1,ins);
                        k++;
                        s=k;
                        //cont to scan
                        while(p->code[k]!='\0') k++;
                        //deal with jump
                        FillJump(p,s,k-1,ins);
                    }
                    else{
                        //deal with comp
                        FillComp(p,s,k-1,ins);
                        ZeroJump(ins);
                    }
                }

                //without dest
                else if(p->code[k]==';'){
                    ZeroDest(ins);
                    //deal with comp
                    FillComp(p,s,k-1,ins);
                    k++;
                    s=k;
                    //cont to scan
                    while(p->code[k]!='\0'&&p->code[k]!=';') k++;
                    //deal with jump
                    FillJump(p,s,k-1,ins);
                }

                //without dest and jump
                else{
                    ZeroDest(ins);

                    //deal with comp
                    FillComp(p,s,k-1,ins);

                    ZeroJump(ins);
                }

                //fill
                FillP(p,ins);

            }

            p=p->next;

        }
        printf("Finish 2nd SCAN\n");
    }

    void SkipLine(FILE *fp){
        char ch;
        while( (ch=fgetc(fp)) != '\n' ){}
    }
};

int main()
{
    FILE*fp=fopen("D:\\ComputerScience\\nand2tetris\\nand2tetris\\projects\\08\\FunctionCalls\\FibonacciElement\\FibonacciElement.hack", "wt");
    File fl;
    printf("FileCreated\n");
    fl.BuildFile();
    printf("FileBuilded\n");
    fl.Translate();
    printf("FileTranslated\n");
    fl.PrintFile(fp);
    printf("FilePrinted\n");
    return 0;
}
