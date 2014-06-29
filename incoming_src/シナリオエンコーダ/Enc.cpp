#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>


//======================================
//
//ここで暗号化
//
//======================================
void Encry(char *pcIn,char *pcOut,int iSize)
{
	char RM;
	bool rflag;//\rの次に\nが来るか！！来ないか！！
	printf("%d",iSize);
	rflag=false;
	for(int i=0; i<iSize; i++){
		if(rflag==true&&pcIn[i]=='\n'){
			pcOut[i-1]='\r';
			pcOut[i]='\n';
			rflag=false;
		}else if(pcIn[i]=='\r'){
	//		printf("r find");
			if(rflag==true){
				pcOut[i-1] = ~pcIn[i-1];
			}
			rflag=true;
		}else{
			pcOut[i] = ~pcIn[i];
			if(pcOut[i]=='\n'){
				pcOut[i]+=1;
			}
			if(rflag==true){
				rflag=false;
				pcOut[i-1] = ~pcIn[i-1];
			}
		}
    }
}


//=======================================
//
//ここでファイルの拡張子を変更
//
//========================================
void ChangeExtension(char *pcIn,char *pcOut,char *pcEnxtension)
{
	int iSize=(int)strlen(pcIn);
	int i,j;
	for(i=0;i<iSize;i++)
	{
		if(pcIn[i]=='.')
		{
			break;
		}
	}
	for(j=0;j<i;j++)
	{
		pcOut[j]=pcIn[j];
	}
	pcOut[j]='.';
	j++;
	iSize=(int)strlen(pcEnxtension);
	for(i=0;i<iSize;i++)
	{
		pcOut[j+i]=pcEnxtension[i];
	}
	pcOut[j+i] ='\0';
}

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		puts("usage: enc filename");
		getch();
		return 0;
	}


	//========================================
	//ファイルからデータの読み込み
	//========================================
	char *pcInFileName = argv[1];
	FILE *fp = fopen(pcInFileName,"rb");
	fseek(fp,0,SEEK_END);
	int iSize=ftell(fp);
	char *pcInBuffer=new char[iSize];
	fseek(fp,0,SEEK_SET);
	fread(pcInBuffer,sizeof(char),iSize,fp);
	fclose(fp);


	//====================================
	//ファイルへWrite
	//====================================
	char acOutFileName[100];
	ChangeExtension(pcInFileName,acOutFileName,"dat");  //出力ファイル名の拡張子入力から変更
	char *pcOutBuffer=new char[iSize];
	fp = fopen(acOutFileName,"wb");
	Encry(pcInBuffer,pcOutBuffer,iSize);            //暗号化
	fwrite(pcOutBuffer,sizeof(char),iSize,fp);        //暗号化したデータをファイルへ書き込み
	fclose(fp);
	return 0;
}
