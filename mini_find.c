/*
----------------------------------->>>> PROJECT-MINI <<<<----------------------------------------
-------------------------------------------------------------------------------------------------

It is my first project for devloping my skills in low level C programming and Linux.
So i am going to create a "find" cmd in C.

find => Used to find a specific file or directory in CLI (Linux)

*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>


/*
    This data type used to safe the required expressions and its value
    PAIR("-name","new.txt");
*/
int pair_cnt = 0,nof=0;
struct PAIR{
    char exp[20];
    char flag[50];
};

//FUNC Declaration:

void nameExpression(char file_name[1001][256],char flag[50],char dest[20],int wildcard,bool frnt,bool bck);


int main(int argc, char *argv[]){
/*
    Just develope the stuffs required to -name expression and exception handeling.
*/

//Edge case checking for input contain only filename withiout expression and flag;


if(argc <= 1){
    printf("Error: Enter correct expression or try -h/-help to learn");
    return 1;
}



/*
Collecting the expression and flag , store it in pair [array of struct];
*/
char dest[20]="";
bool flag_dest=false;
struct PAIR pair[10];

    for(int i=1;i<argc;){
        //storing the destinated directory in "dest"
        if(argv[i][0] != '-' && flag_dest == false){
            strcpy(dest,argv[i]);
            flag_dest=true;
        }
        if(argv[i][0] == '-'){
            if(i+1 >= argc){
                printf("Error : Invalid attributes\n");
                return 1;
            }
            strcpy(pair[pair_cnt].exp,argv[i]);
            strcpy(pair[pair_cnt].flag,argv[i+1]);
            pair_cnt++;
            i+=2;
        }else{
            i++;
        }
    }
    
//Iterate through the PAIR and calling the respective func and store the files data;

char file_name[1001][256];
bool frnt,bck;
int wildcard;

    for(int i=0;i<pair_cnt;i++){

        // condition for -name expression;

        if(strcmp("-name",pair[i].exp) == 0){
            wildcard=0;
            frnt=false,bck=false;
            
            for(int z=0;z<strlen(pair[i].flag);z++){
                if(pair[i].flag[z] == '*'){
                    wildcard=wildcard+1;
                    if(z==0) frnt=true;
                    if(z==strlen(pair[i].flag)-1) bck=true;
                }
            }

            //edge case : If the flag conatin more than 2 wildcard leads to malfunction;

            if(wildcard > 2){
                printf("Error: Invalid wildcards '*' => It should contain atmost 2 '*'\n");
                return 1;
            }

            nameExpression(file_name,pair[i].flag,dest,wildcard,frnt,bck);
        }
    }


    //Output of my result;

    printf("File name\n");

    for(int i=0;i<nof;i++){
        printf("%s\n",file_name[i]);
    }
    return 0;

}







//FUNC ==>> -name expression;

void nameExpression(char file_name[1001][256],char flag[50],char dest[20],int wildcard,bool frnt,bool bck){
    DIR* dir=opendir(dest);

    if(dir == NULL){
        perror("Opendir");
        return ;
    }

    
    if(wildcard == 0 && (!frnt) && (!bck)){
        // If the flag doesnot contain the '*' wildcard;

        struct dirent *file_read=readdir(dir);

        while(file_read != NULL){
            if(strcmp(flag,file_read->d_name) == 0){
               
                if(nof >= 1001){
                    printf("File Limit exceeds");
                    break;
                }else{
                    strcpy(file_name[nof],file_read->d_name);
                    nof++;
                }
            }
            file_read=readdir(dir);
        }

    }
    else{
        // If the flag contain the wildcard '*';

        char reqstr[50]="";
       
        int len_reqstr=strlen(reqstr);

        // Condition "*exe" -> main.exe || find.exe

        if(frnt && (!bck)){
            for(int z=1;z<strlen(flag);z++){
                reqstr[len_reqstr]=flag[z];
                len_reqstr++;
            }
            reqstr[len_reqstr]='\0';

            struct dirent *file_read= readdir(dir);
            while(file_read != NULL){

                //check the size of the filename with reqstr
                if(strlen(reqstr) > strlen(file_read->d_name)){
                    file_read=readdir(dir);
                    continue;
                }
 

                if(strlen(file_read->d_name) >= len_reqstr && strncmp(file_read->d_name + strlen(file_read->d_name) - len_reqstr,reqstr,len_reqstr) == 0){
                    
                    if(nof >= 1001){
                        printf("File limit exceeds:");
                    }else{
                        strcpy(file_name[nof],file_read->d_name);
                        nof++;
                    }
                }

                file_read=readdir(dir);
            }

        }else if((!frnt) && bck){

            //Condition "man*" -> manifest.c || maninTest.txt 

            for(int z=0;z<strlen(flag)-1;z++){
                reqstr[len_reqstr] = flag[z];
                len_reqstr++;
            }
            reqstr[len_reqstr]='\0';

            struct dirent *file_read=readdir(dir);


            while(file_read != NULL){

                if(strlen(reqstr) > strlen(file_read->d_name)){
                    file_read=readdir(dir);
                    continue;
                }

                if(strncmp(file_read->d_name,reqstr,len_reqstr) == 0){
                    
                    if(nof  < 1001){
                        strcpy(file_name[nof],file_read->d_name);
                        nof++;
                    }else{
                        printf("File limit exceeds");
                    }
                }

                file_read=readdir(dir);
            }
        }else if(frnt && bck){

            //Condition "*you*" -> beyoume.cpp || fuyourahh.sh

            for(int z=1;z<strlen(flag)-1;z++){
                reqstr[len_reqstr++]=flag[z];
            }
            reqstr[len_reqstr]='\0';

            struct dirent *file_read=readdir(dir);

            while(file_read != NULL){

                if(len_reqstr > strlen(file_read->d_name)){
                    file_read=readdir(dir);
                    continue;
                }

                for(int y=0;y<=strlen(file_read->d_name)-len_reqstr;y++){
                    if(strncmp(file_read->d_name + y,reqstr,len_reqstr)==0){
                        if(nof  < 1001){
                            strcpy(file_name[nof],file_read->d_name);
                            nof++;
                        }else{
                            printf("File Limit exceeds");
                        }
                        break;
                    }
                }
                file_read=readdir(dir);
                
            }
        }
        
    }

    closedir(dir);
}