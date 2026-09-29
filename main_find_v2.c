/*
----------------------------------->>>> PROJECT-MINI <<<<----------------------------------------
-------------------------------------------------------------------------------------------------

It is my first project for devloping my skills in low level C programming and Linux.
So i am going to create a "find" cmd in C.

find => Used to find a specific file or directory in CLI (Linux)

*/



#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>


char[] caseInsencitive(char[] case_off);


int main(int argc, char *argv[]){
    
    // For find "."
    // dir stores directory flow
    DIR *dir= opendir(argv[1]);
    
    if(dir == NULL){
        printf("No files have been found");
        return 1;
    }


    char reqmnts[256];
    
    char file_name[256]="";

    char file_iname[256]="";

    char file_size[256]="";

    char reqmnts_type[2]="";

    bool name=false,type=false,size=false,iname=false;
    int j=0;
    for(int i=1;i<argc;i++){

        if(argv[i] == "-name"){
            reqmnts[j]=argv[i];
            j++;
            name=true;
            if(i+1 >= argc){
                printf("Error No file path has provided:\n");
            }else{
                strcpy(argv[i+1],file_name);
            }
        }
        
        if(argv[i] == "-type"){
            reqmnts[j]=argv[i];
            j++;
            type=true;
            if(i+1 >= argc){
                printf("Error No file path has provided:\n");
            }else{
                strcpy(argv[i+1],reqmnts_type);
            }
        }
        
        if(argv[i] == "-size"){
            reqmnts[j]=argv[i];
            j++;
            size=true;
            if(i+1 >= argc){
                printf("Error No file path has provided:\n");
            }else{
                strcpy(argv[i+1],file_size);
            }
        }

        if(argv[i] == "-iname"){
            reqmnts[j]=argv[i];
            j++;
            iname=true;
            if(i+1 >= argc){
                printf("Error No file path has provided:\n");
            }else{
                strcpy(argv[i+1],file_iname);
            }
        }

    }

    

    char nameOfFiles[2000],inameOfFiles[2000],typeOfFiles[2000];
    long long sizeOfFiles[2000];
    int nOf=0,sOf=0,inOf=0,tOf=0,sign=0;

    if(name){

        struct dirent *files = readdir(dir);
        if(files == NULL){
            printf("Dir is empty:");
            return 1;
        }
        while( files != NULL){
            if(file_name == files->d_name){
                nameOfFiles[nOf]=files->d_name;
                nOf++;
            }
            files=readdir(dir);
        }
        rewinddir(dir);
    }
    if(type){
        if(reqmnts_type == "f"){
            struct dirent *files = readdir(dir);
            while( files != NULL){
                if("DT_REG" == files->d_type){
                    typeOfFiles[tOf]=files->d_name;
                    tOf++;
                }
            }
            rewinddir(dir);
        }else{
            struct dirent *files= readdir(dir);
            while( files != NULL){
                if("DT_DIR" == files->d_type){
                    typeOfFiles[tOf++]=files->d_name;
                }
                files=readdir(dir);
            }
            rewinddir(dir);
        }
    }

    if(size){
        int file_size_count=0;
        while(file_size(file_size_counter) != '\0'){
            file_size_counter++;
        }
        bool pos_size=false,neg_size=false,no_sign_size=false;

        // finding the condition sign for size_files

        if(file_size_counter > 0){
            pos_size = (file_size[0] == '+') ? true : false;
            neg_size = (file_size[0] == '-') ? true : false;
            no_sign_size = (file_size[0] != '+' && file_size[0] != '-') ? true : false;

        }else{
            printf("Segmentation Falut ...\n");
            return 1;
        }

        // saving files which satisfies the necessary condition in sizeOfFiles;
        
        if(nOf){
            printf("No files found");
            return 1;
        }else{
            //converting the size from string to integer;
            int s_i = sizeof(file_size) / sizeof(file_size[0]);
            int wanted_size=0;
            for(int i=1;i<s_i - 1;i++ ){
                wanted_size = ( file_size[i] - '0' ) + wanted_size*10;
            }

            for(int i=0;i<nOf;i++){
                struct stat st;

                //stat(filename,buffer) return 0 on success , -1 on failure;

                if(stat(nameOfFiles[i],&st)){
                    continue;
                }
                if(pos_size){
                    if(wanted_size > st.size){
                        sizeOfFiles[i]=st.size;
                    }
                }else if(neg_size){
                    if(wanted_size < st.size){
                        sizeOfFiles[i]=st.size;
                    }
                }else if(no_sign_size){
                    if(wanted_size == st.size){
                        sizeOfFiles[i]=st.size;
                    }
                }
            }
            /*
            calculate the size in bytes;

            c - bytes;
            k - kilobytes;
            M - megabytes;
            G - gigabytes;

            */
            long long file_size_bytes=0;
            bool byt=false, k_byt=false,m_byt=false, g_byt=false;
            double sizeToBeDisplay[2000];
            if(file_size[file_size_counter - 1] == 'c'){
                long long byts=0;
                for(int i=0;i<sOf;i++){
                    while(sizeOfFiles[i] >= 1000){
                        sizeOfFiles[i] /= 1000;
                    }
                    sizeToBeDisplay[i]=sizeOfFiles[i];
                }
            }else if(file_size[file_size_counter - 1] == 'k'){

                for(int i=0;i<sOf;i++){
                    sizeToBeDisplay[i]= (sizeOfFiles[i] / 1024.0);
                }
            }else if(file_size[file_size_counter - 1] == 'M'){
                for(int i=0;i<sOf;i++){
                    sizeToBeDisplay[i] = (sizeOfFiles[i] / (pow(1024,2)));
                }
            }else if(file_size[file_size_counter - 1] == 'G'){
                for(int i=0;i<sOf;i++){
                    sizeToBeDisplay[i] = (sizeOfFiles[i] / (pow(1024,3)));
                }
            }else{
                printf("Invalid size;");
                return 1;
            }

        }

        if(iname){
            struct dirent *files= readdir(dir);

            while(files != NULL){
                if(case_off(file_iname) == case_off(files->d_name)){
                    inameOfFiles[i]=files->d_name;
                }
                files=readdir(dir);
            }

        }

    }

    // I stored all the required files name in terms of condition. now i need to union the files that i need for given condition
    /*
    _files_details :
        Name    type    size
    */
    char[][] _files_details[nOf][3];

    int freq[nOf];
    for(int i=0;i<nOf;i++){

    }

    


    closedir(dir);

}


//function for converting upper case to lower case for -iname attribute;

char[] caseInsencitive(char[] case_off){
    int length=0;
    int i=0;
    while(case_off[i] != '\0'){
        if(case_off[i] >= 'A' && case_off[i] <= 'Z'){
            case_off[i] = case_off[i] + 32 ;
        }
    }
    return case_off;
    
}