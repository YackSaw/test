#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>


//======================================
//
//�����ňÍ���
//
//======================================
void Encry(char *pcIn,char *pcOut,int iSize)
{
	char RM;
	bool rflag;//\r�̎���\n�����邩�I�I���Ȃ����I�I
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
//�����Ńt�@�C���̊g���q��ύX
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
	//�t�@�C������f�[�^�̓ǂݍ���
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
	//�t�@�C����Write
	//====================================
	char acOutFileName[100];
	ChangeExtension(pcInFileName,acOutFileName,"dat");  //�o�̓t�@�C�����̊g���q���͂���ύX
	char *pcOutBuffer=new char[iSize];
	fp = fopen(acOutFileName,"wb");
	Encry(pcInBuffer,pcOutBuffer,iSize);            //�Í���
	fwrite(pcOutBuffer,sizeof(char),iSize,fp);        //�Í��������f�[�^���t�@�C���֏�������
	fclose(fp);
	return 0;
}
