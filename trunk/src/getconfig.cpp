#include <string.h>
#include <malloc.h>
#include <stdio.h>

#define MAX_LEN 0xff
#define MAX_LINES 0xff

#define COMMENT_CHAR '#'

int rmlspaces(char* str);
int rmrspaces(char* str);
int rmcomment(char *str);
int rmdotcomma(char *str);
int rmeq(char *str);
char *getvaluestr(char *str);
char *getkeystr(char *str);
char ***parseLines(int *linesCount, char **lines, int count);
char ***getConfig(const wchar_t*fileName,int *count);
char** readFileLines(const wchar_t*fileName,int *count);

int rmlspaces(char* str)
{
	char *str2;
	str2=str;
	while(*str2==' ')str2++;
	if(str2>str)strcpy(str,str2);
	//str[strlen(str2)-(str2-str)]='\0';
	return str2-str;//count of spaces - hz zachem (:
}

int rmrspaces(char* str)
{
	char *str2;
	if(strlen(str)==0)return 0;
	str2=str+strlen(str)-1;
	while(*str2==' ' && str2>=str){
		*str2='\0';
		str2--;
	}
	return str+strlen(str)-1-str2;//count of spaces - hz zachem (:
}

int rmcomment(char *str)
{
	char *str2=strchr(str,COMMENT_CHAR);
	if(str2==0) return 0;
	*str2='\0';
	return 1;
}

int rmdotcomma(char *str)
{
	char *str2=strchr(str,';');
	if(str2==0) return 0;
	*str2='\0';
	return 1;
}

int rmeq(char *str)
{
	char *str2=strchr(str,'=');
	if(str2==0) return 0;
	*str2='\0';
	return 1;
}

char *getvaluestr(char *str)
{
	char *str2=strchr(str,'=');
	if(str2==0) return 0;
	str2++;
	rmlspaces(str2);
	return str2;
}

char *getkeystr(char *str)
{
	char *str2=strchr(str,'=');
	if(str2==0){
		return 0;
	}
	str2[0]='\0';
	rmrspaces(str);
	str2[0]='=';
	return str;
}

char ***parseLines(int *linesCount, char **lines, int count)
{
	char ***rlines,***res;
	int lcount;
	int i;
	rlines=(char***)malloc(sizeof(char***)*count+1);
	memset(rlines,0x0,sizeof(char***)*count);
	lcount=0;
	for(i=0;i<count;i++){
		rmlspaces(lines[i]);
		if(lines[i][0]!=COMMENT_CHAR && strlen(lines[i])>0){
			rlines[lcount]=(char**)malloc(sizeof(char**)*2+1);
			rmcomment(lines[i]);
			rmdotcomma(lines[i]);
			rmrspaces(lines[i]);
			rlines[lcount][1]=getvaluestr(lines[i]);
			rlines[lcount][0]=getkeystr(lines[i]);
			rmeq(lines[i]);
			lcount++;
		}
	}
	
	if(lcount==0) return 0;
	
	res=(char***)malloc(sizeof(char***)*lcount+1);
	
	for(i=0;i<lcount;i++){
		res[i]=(char**)malloc(sizeof(char**)*2+1);
		res[i][0]=(char*)malloc(strlen(rlines[i][0])*sizeof(char)+1);//
		strcpy(res[i][0],rlines[i][0]);
		if(rlines[i][1]!=0){
			res[i][1]=(char*)malloc(strlen(rlines[i][1])*sizeof(char)+1);//
			strcpy(res[i][1],rlines[i][1]);
		}
		else{
			res[i][1]=0;
		}
		free(rlines[i]);
	}
	free(rlines);
	*linesCount=lcount;
	return res;
}

char ***getConfig(const wchar_t *fileName,int *count)
{
	char ***res;
	char **lines;
	int lcount;
	int i;
	lines=readFileLines(fileName,count);
	lcount=*count;
	res=parseLines(count,lines,*count);
	for(i=0;i<lcount;i++){
		free((void*)lines[i]);
	}
	free((void*)lines);
	return res;
}

char** readFileLines(const wchar_t *fileName,int *count)
{
       FILE *f=_wfopen(fileName,L"r");
		if(!f) return 0;
        char **res1, **res;
        char *buf;
        res=(char**)malloc(MAX_LINES*sizeof(char**));
        buf=(char*)malloc(MAX_LEN+1);
        int lines=0;memset(buf,0x0,MAX_LEN+1);
        while(fgets(buf,MAX_LEN,f) && lines<MAX_LINES){
                res[lines]=(char*)malloc(strlen(buf)+1);
                memcpy(res[lines],buf,strlen(buf)+1);
                res[lines][strlen(res[lines])-1]=0x0;
                memset(buf,0x0,MAX_LEN+1);
                lines++;
        }
        res1=(char**)malloc(sizeof(char**)*lines);
        memcpy(res1,res,lines*sizeof(char*));
        free(res);
        free(buf);
        *count=lines;
		fclose(f);
        return res1;
}

