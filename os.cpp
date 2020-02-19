// os.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include <direct.h>
#include"time.h"
#include"windows.h"
#include<iostream>
#include<string>
#include<ctime>
#include"io.h"
#include<fstream>
#include<cstring>
#include<iomanip>
using namespace std;

//�궨��
#define ISFILE 0			//�ж��ļ�
#define ISDIR	 1			//	�ж�·��
#define UNKNOW -1	//	�ļ������жϱ�ʶ��
#define BLKSIZE 128	//	���С
#define FIRSTIBLK 4	//��һ��I�����
#define FIRSTDBLK 24	//��һ�����ݿ��
#define INODES 80	//I�����
#define DATABLKS 216	//���ݿ��С
#define DIRITEMS 8		//	Ŀ¼��Ŀ
#define MAXDEPTH 20	
#define BLOCKNUM 4	//	�����

int dir_sum=0,file_sum=0,used_sum=0;

struct SuperBlock	//������ṹ�壬����ȫ���ļ�������Ϣ
{
	int MaxSize;				//�ļ�ϵͳ�Ĵ�С
	int FirstINodeBlk;			//��һ��I�����
	int FirstDBlk;				//��һ�����ݿ��
	int BlockSize;				//ÿ��������С
	bool INodeStatus[INODES];		//INode״̬(���л�ռ��)
	bool DataBlockStatus[DATABLKS];		//���ݿ�״̬(���л�ռ��)
}superblock;				//����ȫ�ֱ��� superblock
struct INode//i���ṹ�壬Ҳ����һ��Ŀ¼
{
	int FileType;				//�ļ����� 
	int INodeNumber;			//i-node��
	int LinksNum;				//������
	int FileSize;				//�ļ���С
	int blocks[BLOCKNUM];			//ռ�õ����ݿ�
}inode[INODES];	//����ȫ��inode����
struct	DirectoryItem//Ŀ¼�ṹ��
{
	int INodeNumber;		//INode��
	char FileName[12];		//�ļ���
};
//**********I-Node����**********
void Init_INode();	//��ʼ��i���
int AllocINode();	//����I-���
void WriteINode(int, INode);		//дI-��㵽�ļ���
void ReadINode(int, INode&);		//���ļ��ж�ȡI-���
void FreeINode(int);	//�ͷ�I-���
void SetDirINode(INode&, int, int);	//��ʼ���½���Ŀ¼I-���
void SetFleINode(INode&, int, int, int*, int);//��ʼ���½����ļ�I-���
void UpdataINode(int);			//����I-���
//**********�����麯��**********
void Init_SuperBlock();		//��ʼ��������
void WriteSuperBlock();		//д�����鵽�ļ���
void ReadSuperBlock();		//���ļ��ж�ȡ������
bool HaveSameName(char *, INode);	//Ѱ���Ƿ�����ͬ���ļ���
//**********Ŀ¼���**********
bool FindDir(char *, INode, int, DirectoryItem&);//����Ŀ¼��
void SetDirItem(char *c, int, DirectoryItem&);	//��ʼ���½���Ŀ¼��
void WriteDir(INode, DirectoryItem, int);		//дĿ¼��
void ReadDir(int, INode, DirectoryItem &);	//��Ŀ¼��
//**********shell��������**********
void mkdir(char *config);//�½�һ��Ŀ¼	
void more(char *config);//������ʾ���
void dir(char *config);	//��ʾĿ¼�е��ļ�����Ŀ¼
void help(char*config);	//����
void cd(char*config);	//��ʾ��ı䵱ǰĿ¼
void del(char*config);	//ɾ��һ���ļ�
void format();		//��ʽ������
void InterpretCmd();	//���������
void mkfle(char *config);//����һ���ļ�
void rm(char *config);//ɾ��һ��Ŀ¼
void copy(char *config);//�����ļ�
void move(char *config);//�ƶ�һ���ļ�
void ShowCurPath();	
//**********���ݿ麯��**********
void Init_DataBlock();		//��ʼ�����ݿ�
int AllocDBlk();		//�������ݿ�
void WriteDBlk(int *, int, char *);//�ͷ����ݿ�
void FreeDBlk(int);		//д���ݿ�
////**********ȫ�ֱ���**********
int Path[20];								//��ǰĿ¼·������ 
int num;										//��ǰĿ¼·���Ľ����
char CurDirName[12];		//��ǰĿ¼�ļ���
char dst[30];				//Ŀ��·��

//**********�����麯��**********
void Init_SuperBlock()//��ʼ��������
{
	superblock.MaxSize = 240 * BLKSIZE;		//����������С
	superblock.BlockSize = BLKSIZE;				//�������С
	superblock.FirstINodeBlk = FIRSTIBLK;	//��һ�������
	superblock.FirstDBlk = FIRSTDBLK;		//��һ�����ݿ��
	for (int i = 0; i<INODES; i++)
		superblock.INodeStatus[i] = true;		//����i���״̬
	for (int i = 0; i<DATABLKS; i++)
		superblock.DataBlockStatus[i] = true;	//�������ݿ�״̬

	superblock.INodeStatus[0] = false;		//0��I-���ı���Ŀ¼ʹ��
	superblock.DataBlockStatus[0] = false;//ͬ��0�����ݿ鱻��Ŀ¼ʹ��
	fstream fout; //����һ���ļ�������Ҫ���ļ��Ķ�д����
	fout.open("This is chenghuwei's file", ios::out | ios::binary);	//д�ļ�/�������ļ���
	if (!fout)//				����һЩ��������򿪴���
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.write((char*)&superblock, FIRSTIBLK*BLKSIZE);//ǿ��ת��char *���ʹӶ������ļ���FIRSTIBLK*BLKSIZE���Ի���BLKSIZE��������������Ϊ�����˴���ʱ�򴴽�����
	fout.close();
}
void WriteSuperBlock()//д�����鵽�ļ���
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);//�򿪳�ʼ��ʱ���ļ�
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.write((char*)&superblock, FIRSTIBLK*BLKSIZE);
	fout.close();
}
void ReadSuperBlock()//��ȡ������
{
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file!" << endl;
		exit(0);
	}
	fin.read((char*)&superblock, FIRSTIBLK*BLKSIZE);
	fin.close();
}
//**********I-Node����**********
void Init_INode()//��ʼ��I-���
{
	for (int i = 0; i<INODES; i++)
	{
		inode[i].FileType = UNKNOW;	//�ļ�����δ֪
		inode[i].INodeNumber = i;		//I-node��
		inode[i].LinksNum = 0;		//������Ϊ0
		inode[i].FileSize = 0;		//�ļ���СΪ0
		for (int j = 0; j<BLOCKNUM; j++)	//ÿ��I�����������ݿ�
			inode[i].blocks[j] = -1;
	}

	inode[0].FileType = ISDIR;//0��I-��㱻��Ŀ¼ռ�ã�0�����ݿ鱻��Ŀ¼ʹ�ã��ļ�����ISDIR=1
	inode[0].blocks[0] = 0;		//��Ŀ¼ռ�����ݿ�
	inode[0].LinksNum = 2;	//������������	

	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp(FIRSTIBLK*BLKSIZE);//���������ָ���λ�ã�������λ��
	for (int i = 0; i<INODES; i++)
		fout.write((char*)&inode[i], sizeof(INode));

	fout.close();
}
int AllocINode()//����I-���
{
	for (int i = 0; i<INODES; i++)
	if (superblock.INodeStatus[i] == true)	//��i�����ɱ�����
	{
		superblock.INodeStatus[i] = false;
		return i;
	}
	return -1;
}
void FreeINode(int Index)	//�ͷ�I-���
{
	inode[Index].FileType = UNKNOW;	//���ͻ�ԭ
	inode[Index].INodeNumber = Index;	//I-�ڵ�Ż�ԭ
	inode[Index].LinksNum = 0;				//��������ԭ
	inode[Index].FileSize = 0;
	for (int j = 0; j<BLOCKNUM; j++)
	{
		if (inode[Index].blocks[j] != -1)		//����������ݿ飬��ô������ڵĿ�
		{
			int DBlockNum = inode[Index].blocks[j]; 
			FreeDBlk(DBlockNum);//�Ѵ��ڵ����ݿ��ڳ�������ɾ��
		}
		inode[Index].blocks[j] = -1;
	}
	WriteINode(Index, inode[Index]);
	superblock.INodeStatus[Index] = true;//�����I-���״̬��Ϊ����״̬
}
void WriteINode(int Index, INode inode)//дI-��㵽�ļ���
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp(FIRSTIBLK*BLKSIZE + sizeof(INode)*Index);//д���ļ��Ľ���λ��
	fout.write((char *)&inode, sizeof(INode));
	fout.close();
}
void ReadINode(int Index, INode &inode)//���ļ��ж�ȡI-���
{
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fin.seekp(FIRSTIBLK*BLKSIZE + sizeof(INode)*Index);
	fin.read((char *)&inode, sizeof(INode));
	fin.close();
}
void SetDirINode(INode &inode, int INodeNum, int DBlkNum)//��ʼ���½���Ŀ¼I-���
{
	inode.FileSize = 0;
	inode.FileType = ISDIR;		//������һ��Ŀ¼
	inode.INodeNumber = INodeNum;
	inode.LinksNum = 2;
	for (int i = 0; i<4; i++)
		inode.blocks[i] = -1;
	inode.blocks[0] = DBlkNum;
}
void SetFleINode(INode &inode, int FileLength, int INodeNum, int *DBlkNum, int count)//��ʼ���½����ļ�I-���
{
	inode.FileSize = FileLength;  
	inode.FileType = ISFILE;		//�ļ�
	inode.INodeNumber = INodeNum;
	inode.LinksNum = 0;
	for (int i = 0; i<4; i++)
		inode.blocks[i] = -1;	
	for (int i = 0; i<count; i++)
		inode.blocks[i] = DBlkNum[i]; //����ÿ���ļ������ݿ�
}
void UpdataINode(int length)//����I-���
{
	for (int i = 0; i<num; i++)//������·���µ����нڵ�
	{
		INode inode1;
		ReadINode(Path[i], inode1);//��ȡ�ý���
		inode1.FileSize += length;
		WriteINode(Path[i], inode1);//д����º�ý����Ϣ
	}
}
bool HaveSameName(char *config, INode inode)//Ѱ���Ƿ�����ͬ���ļ���
{
	if (inode.LinksNum == 0)
		return false;
	bool have = false;
	DirectoryItem *Dir = new DirectoryItem[inode.LinksNum];
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	int BlkNum = inode.blocks[0];
	fin.seekp((FIRSTDBLK + BlkNum)*BLKSIZE);
	for (int i = 0; i<inode.LinksNum; i++)
		fin.read((char*)&Dir[i], sizeof(DirectoryItem));
	for (int i = 0; i<inode.LinksNum; i++)
		if (strcmp(config, Dir[i].FileName) == 0)
			have = true;
	fin.close();
	return have;
}
//**********Ŀ¼���**********
bool FindDir(char *config, INode inode, int Index, DirectoryItem &Dir)//����Ŀ¼��
{
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file!" << endl;
		exit(0);
	}
	int BlkNum = inode.blocks[0];
	fin.seekp((FIRSTDBLK + BlkNum)*BLKSIZE + Index*sizeof(DirectoryItem));
	fin.read((char*)&Dir, sizeof(DirectoryItem));
	if (strcmp(Dir.FileName, config) == 0)
		return true;
	return false;
}
void SetDirItem(char *config, int Index, DirectoryItem &Dir)//��ʼ���½���Ŀ¼��
{
	strcpy(Dir.FileName, config);
	Dir.INodeNumber = Index;
}
void WriteDir(INode inode, DirectoryItem Dir, int Index)//дĿ¼��
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp((FIRSTDBLK + inode.blocks[0])*BLKSIZE + sizeof(DirectoryItem)*Index);
	fout.write((char*)&Dir, sizeof(DirectoryItem));
	fout.close();
}
void ReadDir(int index, INode inode, DirectoryItem &Dir)//��Ŀ¼��
{
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fin.seekp((FIRSTDBLK + inode.blocks[0])*BLKSIZE + index*sizeof(DirectoryItem));
	fin.read((char*)&Dir, sizeof(DirectoryItem));
	fin.close();
}
//**********���ݿ麯��**********
void Init_DataBlock()//��ʼ�����ݿ�
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp(FIRSTDBLK*BLKSIZE);
	DirectoryItem RootDir[2];	//��ʼ����Ŀ¼
	strcpy(RootDir[0].FileName, ".");
	RootDir[0].INodeNumber = 0;
	strcpy(RootDir[1].FileName, "..");
	RootDir[1].INodeNumber = 0;
	for (int i = 0; i<2; i++)
		fout.write((char*)&RootDir[i], sizeof(DirectoryItem));
	fout << setw(DATABLKS*BLKSIZE - 1 - sizeof(DirectoryItem)* 2) << '\0';//������ʽ
	fout.close();
}
int AllocDBlk()//�������ݿ�
{
	for (int i = 0; i<DATABLKS; i++)
	{
		if (superblock.DataBlockStatus[i] == true)
		{
			superblock.DataBlockStatus[i] = false;
			return i;
		}
	}
	return -1;
}
void FreeDBlk(int Index)//�ͷ����ݿ�
{
	superblock.DataBlockStatus[Index] = true;
}
void WriteDBlk(int *DBlkNum, int count, char *content)//д���ݿ�
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	int t = 0;
	for (int i = 0; i<count; i++)
	{
		fout.seekp((FIRSTDBLK + DBlkNum[i])*BLKSIZE);
		for (int j = 0; j<BLKSIZE; j++)
		{
			if (content[t] != '\0')
			{
				fout.write((char*)&content[t], 1);
				t++;
			}
			else
			{
				fout.write((char*)&content[t], 1);
				break;
			}
		}
	}
	fout.close();
}
//**********shell��������**********
void SplitCmd(char *Cmd, char *cmd, char *config)//�ָ�����
{
	int i;
	int k = strlen(Cmd);
	for (i = 1; i<(int)strlen(Cmd); i++)//�ҵ�' '���߽���λ��
	{
		if ((Cmd[i] == ' ' || Cmd[i] == '\t') && (Cmd[i - 1] != ' '&&Cmd[i - 1] != '\t'))
		{
			k = i;
			break;
		}
	}
//********�����޿հ��ַ�������********
	int count = 0;
	for (i = 0; i<k; i++)
	{
		if (Cmd[i] != ' '&&Cmd[i] != '\t')
			cmd[count++] = Cmd[i];
	}
	cmd[count] = '\0';
//********�����޿հ��ַ��Ĳ���********
	if(_stricmp(cmd,"copy")!=0&&_stricmp(cmd, "move")!=0)
	{
		int m, n;
		for (m = k; m<(int)strlen(Cmd); m++)
		{
			if (Cmd[m] != ' '&&Cmd[m] != '\t')
				break;
		}
		for (n = strlen(Cmd) - 1; n >= m; n--)
		{
			if (Cmd[n] != ' '&&Cmd[n] != '\t')
				break;
		}
		count = 0;
		for (i = m; i <= n; i++)
		{
			config[count] = Cmd[i];
			count++;
		}
		config[count] = '\0';
	}
//********����copy&mv��dst��ַ********
	else
	{
		int m, n,x;
		for (m = k; m<(int)strlen(Cmd); m++)
		{
			if (Cmd[m] != ' '&&Cmd[m] != '\t')
				break;
		}
		for (i = m; i < strlen(Cmd) - 1; i++)
		{
			if ((Cmd[i] == ' ' || Cmd[i] == '\t') && (Cmd[i - 1] != ' '&&Cmd[i - 1] != '\t'))
			{
				x=i;
				break;
			}
		}
		count = 0;
		for (i = m; i <x; i++)
		{
			config[count] = Cmd[i];
			count++;
		}
		config[count] = '\0';
		for (; x<(int)strlen(Cmd); x++)//��ȡ�ڶ���' '��ĵ�һ���ַ�λ��
		{
			if (Cmd[x] != ' '&&Cmd[x] != '\t')
				break;
		}
		for (n = strlen(Cmd) - 1; n >= x; n--)
		{
			if (Cmd[n] != ' '&&Cmd[n] != '\t')
				break;
		}
		count = 0;
		for (i = x; i <= n; i++)
			dst[count++] = Cmd[i];
		dst[count] = '\0';
	}
	
}
void InterpretCmd(char *cmd, char *config)//�������
{
	if (_stricmp(cmd, "mkdir") == 0)//�����ִ�Сд�ıȽ�
	{
		if (_stricmp(config, "") == 0)
			cout << "�����﷨����ȷ��" << endl << endl;
		else
		{
			mkdir(config);
			WriteSuperBlock();
		}
	}
	else if (_stricmp(cmd, "dir") == 0)
		{ 
			dir_sum=0; //'/s'��������Ŀ¼����
			file_sum=0;//'/s'���������ļ�����
			used_sum=0;//'/s'��������ʹ���ֽ�
			dir(config);
		}
	else if (_stricmp(cmd, "cd") == 0)
	{
		if (_stricmp(config, "") == 0)	//��ǰ·��
		{
			ShowCurPath();
			cout << endl ;
		}
		else
			cd(config);
	}
	else if (_stricmp(cmd, "del") == 0)//ɾ���ļ�
	{
		del(config);
		WriteSuperBlock();
		cout << endl;
	}
	else if (_stricmp(cmd, "rm") == 0)//ɾ��Ŀ¼
	{
		rm(config);
		WriteSuperBlock();
		cout << endl;
	}
	else if (_stricmp(cmd, "mkfle") == 0)
	{
		if (_stricmp(config, "") == 0)
			cout << "�����﷨����ȷ��" << endl << endl;
		else if (strlen(config)>12)
			cout << config << "������·��̫��" << endl << endl;
		else
		{
			mkfle(config);
			WriteSuperBlock();
		}
	}
	else if (_stricmp(cmd, "more") == 0)
	{
		if (_stricmp(config, "") == 0)
			cout << "�����﷨����ȷ��" << endl << endl;
		else
			more(config);
	}
	else if (_stricmp(cmd, "format") == 0)
	{
		format();
		cout << "�ļ�ϵͳ��ʽ�����!" << endl << endl;
	}
	else if (_stricmp(cmd, "cls") == 0)
		system("cls");
	else if (_stricmp(cmd, "exit") == 0)
	{
		exit(0);
		return;
	}
	else if (_stricmp(cmd, "help") == 0)
		help(config);
	else if(_stricmp(cmd, "copy")==0)
		copy(config);
	else if(_stricmp(cmd, "move")==0)
		move(config);
	else if(_stricmp(cmd, "time")==0)
		system("time");
	else
		cout << "'" << cmd << "'" << " �����ڲ����ⲿ���Ҳ���ǿ����еĳ���" << endl << "���������ļ���" << endl << endl;
}
string charsoftime(const struct tm * ptm)//��ȡ��ǰʱ��
{
	char chtime[30];
	memset(chtime, 0, sizeof(chtime));
	char *chtemp = new char[10];
	_itoa(ptm->tm_year + 1900, chtemp, 10);
	strcat(chtime, chtemp);
	strcat(chtime, "/");
	_itoa(ptm->tm_mon + 1, chtemp, 10);
	if (strlen(chtemp) == 1)
		strcat(chtime, "0");
	strcat(chtime, chtemp);
	strcat(chtime, "/");
	_itoa(ptm->tm_mday, chtemp, 10);
	if (strlen(chtemp) == 1)
		strcat(chtime, "0");
	strcat(chtime, chtemp);

	strcat(chtime, "  ");
	_itoa(ptm->tm_hour, chtemp, 10);
	if (strlen(chtemp) == 1)
		strcat(chtime, "0");
	strcat(chtime, chtemp);
	strcat(chtime, ":");
	_itoa(ptm->tm_min, chtemp, 10);
	if (strlen(chtemp) == 1)
		strcat(chtime, "0");
	strcat(chtime, chtemp);
	string Chtime = string(chtime);
	return Chtime;
}
void ShowCurPath()//��ʾ��ǰ·��
{
	cout << "chv:";
	if (num == 1)
		cout << "\\";
	for (int i = 0; i<num - 1; i++)
	{
		INode CurINode;
		ReadINode(Path[i], CurINode);
		for (int j = 2; j <CurINode.LinksNum; j++)
		{
			DirectoryItem Dir;
			ReadDir(j, CurINode, Dir);		//��Ŀ¼��
			INode inode1;
			ReadINode(Dir.INodeNumber, inode1);
			if (Dir.INodeNumber == Path[i + 1] && inode1.FileType == ISDIR)
			{
				cout << "\\";
				cout << Dir.FileName;
				break;
			}
		}
	}
}
void mkfle(char *config)
{
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//�ж�Ŀ¼���Ƿ��Ѿ��ﵽ8��
		cout << "��ǰĿ¼�����������ļ�ʧ��!" << endl << endl ;
	else
	{
		if (HaveSameName(config, CurINode))
			cout << "��Ŀ¼���ļ� " << config << " �Ѿ����ڡ�" << endl<< endl ;
		else
		{
			int INodeNum = AllocINode();
			if (INodeNum != -1)
			{
				char ch;
				cout << "�Ƿ������ļ�����(Y\\N)";
				cin >> ch;

				//��ջ������еĻ��з�
				cin.clear();
				fflush(stdin);

				int count = 0;
				int DBlkNum[4];
				char *content = '\0';
				int length = 0;
				if (ch == 'Y' || ch == 'y')
				{
					content = new char[4 * BLKSIZE];
					cout << "�������ļ�����:" << endl;
					cin.getline(content, 4 * BLKSIZE, '\n');
					length = strlen(content);
					content[length] = '\0';
					count = length / BLKSIZE + 1;
					if (count>4)
					{
						cout << "�ļ�̫���޷�Ϊ�ļ�������̿ռ䡣" << endl << endl ;
						FreeINode(INodeNum);
						return;
					}
					for (int i = 0; i<count; i++)
					{
						DBlkNum[i] = AllocDBlk();
						if (DBlkNum[i] == -1)
						{
							FreeINode(INodeNum);
							for (int j = i - 1; j >= 0; j--)				//�ļ�ϵͳ���̿鲻��
								FreeDBlk(DBlkNum[i]);
							cout << "���̿��Ѿ����꣬�����ļ�ʧ��!" << endl << endl ;
							break;
						}
					}
					WriteDBlk(DBlkNum, count, content);
				}

				//дĿ¼��
				DirectoryItem NewDir;
				SetDirItem(config, INodeNum, NewDir);
				WriteDir(CurINode, NewDir, CurINode.LinksNum);

				INode NewINode;
				SetFleINode(NewINode, length, INodeNum, DBlkNum, count);
				WriteINode(INodeNum, NewINode);
				cout << "�ļ������ɹ�!" << endl << endl ;
				CurINode.LinksNum++;
				CurINode.FileSize += length;
				UpdataINode(length);
			}
			else
				cout << "����Ѿ����꣬�����ļ�ʧ��!" << endl<< endl  ;
		}
	}
	WriteINode(Path[num - 1], CurINode);
}
void cd(char *config)
{
	INode inode;
	bool flag = false;
	ReadINode(Path[num - 1], inode);
	for (int i = 0; i<inode.LinksNum; i++)
	{
		DirectoryItem Dir;
		if (FindDir(config, inode, i, Dir) == true)
		{
			flag = true;
			if (strcmp(config, ".") == 0){}
			else if (strcmp(config, "..") == 0)	//�����ϼ�Ŀ¼
			{
				if (num>1)
					Path[num --] = -1;
			}
			else
			{
				INode inode1;
				ReadINode(Dir.INodeNumber, inode1);
				if (inode1.FileType == ISFILE)
				{
					cout << "Ŀ¼������Ч��" << endl ;
					return;
				}
				Path[num] = Dir.INodeNumber;
				strcpy(CurDirName, Dir.FileName);
				num++;
			}
		}
	}
	if (flag == false)
		cout << "ϵͳ�Ҳ���ָ����·��" << endl << endl;
}
void dir(char *config)
{
	int DirCount = 0;
	int FleCount = 0;
	int CurTotalFileSize = 0;
	INode inode, inode1;
	if(strstr(config,"\\"))
		printf("����������󣡣���\n");
		else
	{
		ReadINode(Path[num - 1], inode);
		cout << endl;
		ShowCurPath();	//��ʾ��ǰ·��
		cout << "��Ŀ¼" << endl << endl;
		if(!strstr(config,"."))
		{
			for (int i = 0; i<inode.LinksNum; i++)
			{
				DirectoryItem Dir;
				ReadDir(i, inode, Dir);	//��Ŀ¼
				ReadINode(Dir.INodeNumber, inode1);
				time_t CurTime = time(0);
				string time = charsoftime(localtime(&CurTime));
				cout << time.c_str() << '\t';		//�������ʱ��
				if (inode1.FileType == ISDIR)		//1
				{
					cout << "<DIR>" << "\t\t" << Dir.FileName << endl;
					DirCount++;
				}
				if (inode1.FileType == ISFILE)		//0
				{
					FleCount++;
					cout << "\t" << inode1.FileSize << "\t" << Dir.FileName << endl;
					CurTotalFileSize += inode1.FileSize;
				}
			}
			INode root;
			ReadINode(Path[0], root);
			cout << "\t\t" << FleCount << "���ļ�" << "\t\t" << CurTotalFileSize << "\t�ֽ�" << endl;
			cout << "\t\t" << DirCount << "��Ŀ¼" << "\t\t" << superblock.MaxSize - root.FileSize << "\t�����ֽ�";
			cout << endl << endl;
			dir_sum+=DirCount;
			file_sum+=FleCount;
			used_sum+=CurTotalFileSize;
		}
		else
		{
			for (int i = 0; i<inode.LinksNum; i++)
			{
				DirectoryItem Dir;
				ReadDir(i, inode, Dir);	//��Ŀ¼
				ReadINode(Dir.INodeNumber, inode1);
				time_t CurTime = time(0);
				string time = charsoftime(localtime(&CurTime));
				if (inode1.FileType == ISFILE&&strstr(Dir.FileName,strstr(config,".")))		
				{
					cout << time.c_str() << '\t';		//�������ʱ��
					FleCount++;
					cout << "\t" << inode1.FileSize << "\t" << Dir.FileName << endl;
					CurTotalFileSize += inode1.FileSize;
				}
		
			}
			INode root;
			ReadINode(Path[0], root);
			cout << "\t\t" << FleCount << "���ļ�" << "\t\t" << CurTotalFileSize << "\t�ֽ�" << endl;
			cout << "\t\t" << DirCount << "��Ŀ¼" << "\t\t" << superblock.MaxSize - root.FileSize << "\t�����ֽ�";
			cout << endl << endl;
		}
		if(strstr(config,"/s"))
		{
			for (int i = 0; i<inode.LinksNum; i++)
			{
				DirectoryItem Dir;
				ReadDir(i, inode, Dir);	//��Ŀ¼
				ReadINode(Dir.INodeNumber, inode1);
				time_t CurTime = time(0);
				string time = charsoftime(localtime(&CurTime));
				if (inode1.FileType == ISDIR&&!strstr(Dir.FileName,"."))		//1
							{ 
								cd(Dir.FileName);
								dir(Dir.FileName);
								cd("..");
							}
			}
			INode root;
			ReadINode(Path[0], root);
			printf("\t�����ļ�����:\n");
			cout << "\t\t" << file_sum << "���ļ�" << "\t\t" << used_sum << "\t�ֽ�" << endl;
			cout << "\t\t" << dir_sum << "��Ŀ¼" << "\t\t" << superblock.MaxSize - used_sum << "\t�����ֽ�";
			cout << endl << endl;
		}
	}

}
void mkdir(char *config)
{
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//�ж�Ŀ¼���Ƿ��Ѿ��ﵽ8��
		cout << "��ǰĿ¼������������Ŀ¼ʧ��!" << endl << endl;
	else
	{
		if (HaveSameName(config, CurINode))
			cout <<"������Ŀ¼ʧ��!" << endl << endl;
		else
		{
			int INodeNum = AllocINode();	//����I-���
			if (INodeNum != -1)
			{
				int DBlkNum = AllocDBlk();	//����I-���ɹ����������ݿ�
				if (DBlkNum != -1)
				{
					DirectoryItem NewDir;		//Ŀ¼�ṹ����
					SetDirItem(config, INodeNum, NewDir);		//��ʼ���½���Ŀ¼��
					WriteDir(CurINode, NewDir, CurINode.LinksNum);	//дĿ¼��
					fstream fout;
					fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
					if (!fout)
					{
						cout << "Can't open this file" << endl;
						exit(0);
					}
					fout.seekp((FIRSTDBLK + DBlkNum)*BLKSIZE);	//�ҵ�д��λ��
					DirectoryItem ChildDir[2];
					strcpy(ChildDir[0].FileName,".");
					ChildDir[0].INodeNumber=INodeNum;
					strcpy(ChildDir[1].FileName,"..");
					ChildDir[1].INodeNumber=CurINode.INodeNumber;	
					for (int i = 0; i<2; i++)
						fout.write((char*)&ChildDir[i], sizeof(DirectoryItem));
					fout.close();

					INode NewINode;
					SetDirINode(NewINode, INodeNum, DBlkNum);		//��ʼ���½���Ŀ¼I-���
					WriteINode(INodeNum, NewINode);		//дI-��㵽�ļ���
					cout << "Ŀ¼�����ɹ�!" << endl << endl;
					CurINode.LinksNum++;
				}
				else
				{
					FreeINode(INodeNum);		//�ͷ�I-���
					cout << "���̿��Ѿ����꣬������Ŀ¼ʧ��!" << endl << endl;
				}
			}
			else
				cout << "����Ѿ����꣬������Ŀ¼ʧ��!" << endl << endl;
		}
	}
	WriteINode(Path[num - 1], CurINode);	//дI-��㵽�ļ���
}
void more(char *config)
{
	INode inode;
	ReadINode(Path[num - 1], inode);
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file!" << endl;
		exit(0);
	}
	for (int i = 0; i<inode.LinksNum; i++)
	{
		DirectoryItem DirItem;
		ReadDir(i, inode, DirItem);
		if (strcmp(DirItem.FileName, config) == 0)
		{
			INode inode1;
			ReadINode(DirItem.INodeNumber, inode1);
			if (inode1.FileType == ISDIR)
			{
				cout << "�޷����� " << DirItem.FileName << " �ļ�" << endl << endl;
				break;
			}
			else
			{
				char *content = new char[4 * BLKSIZE];
				strcpy(content, "\0");
				int i = 0;
				int count = 0;
				while (inode1.blocks[i] != -1)
				{
					fin.seekp((FIRSTDBLK + inode1.blocks[i])*BLKSIZE);
					for (int j = 0; j<BLKSIZE; j++)
					{
						fin.read((char *)&content[count], 1);
						if (strcmp(&content[count], "\0") == 0)
							break;
						count++;
					}
					i++;
				}
				cout << content << endl << endl;
			}
		}
	}
	fin.close();
}
void del(char *config)
{
	bool have = false;
	INode inode1, inode2;
	ReadINode(Path[num - 1], inode1);
	for (int i = 0; i<inode1.LinksNum; i++)
	{
		DirectoryItem Dir;
		have = FindDir(config, inode1, i, Dir);
		if (have == true)
		{
			ReadINode(Dir.INodeNumber, inode2);
			if (inode2.FileType == ISFILE)
			{
				FreeINode(inode2.INodeNumber);
				for (int j = i; j<inode1.LinksNum - 1; j++)
				{
					DirectoryItem dir1, dir2;
					ReadDir(j, inode1, dir1);
					ReadDir(j + 1, inode1, dir2);
					dir1.INodeNumber = dir2.INodeNumber;
					strcpy(dir1.FileName, dir2.FileName);
					WriteDir(inode1, dir1, j);
					WriteDir(inode1, dir2, j + 1);
				}
				UpdataINode(-inode2.FileSize);
				inode1.FileSize -= inode2.FileSize;
				inode1.LinksNum--;
				break;
			}
		}
	}
	if (!have)
	{
		cout << "ϵͳ�Ҳ���";
		ShowCurPath();
		cout << config << endl << endl;
	}
	WriteINode(Path[num - 1], inode1);
}
void help(char *config)
{
		system("cls");
		printf("\n\n\n\n\n\n\n\t\t\t\t   /***************Shellָ��***************/\n");
        printf("\n\n");
		printf("\t\t\t\tCD             ��ʾ��ǰĿ¼�����ƻ�����ġ�\n");
		printf("\t\t\t\tCOPY           ������һ���ļ����Ƶ���һ��λ�á�\n");
		printf("\t\t\t\tCLS            �����Ļ��\n");
		printf("\t\t\t\tDEL            ɾ������һ���ļ���\n");
		printf("\t\t\t\tDIR            ��ʾһ��Ŀ¼�е��ļ�����Ŀ¼��\n");
		printf("\t\t\t\tEXIT           �˳� CMD.EXE ����(������ͳ���)��\n");  
		printf("\t\t\t\tFORMAT         ��ʽ�����̣��Ա����� Windows��\n");
		printf("\t\t\t\tHELP           �ṩ Windows ����İ�����Ϣ��\n");
		printf("\t\t\t\tMKDIR          ����һ��Ŀ¼��\n");
		printf("\t\t\t\tMKFLE          ����һ���ļ�\n");  
		printf("\t\t\t\tMORE           ������ʾ�����\n");
		printf("\t\t\t\tMOVE           ��һ�������ļ���һ��Ŀ¼�ƶ�����һ��Ŀ¼��\n");
		printf("\t\t\t\tRM          ɾ��Ŀ¼��\n");
		printf("\t\t\t\tTIME           ��ʾ������ϵͳʱ�䡣\n");
		printf("\n\n\t\t\t\t��������˳�~����\n");
		getchar();
		system("cls");
}
void format()
{
	Init_SuperBlock();
	Init_INode();
	Init_DataBlock();
}
void rm(char *config)
{
	bool have = false;
	INode inode1, inode2;
	ReadINode(Path[num - 1], inode1);
	for (int i = 0; i<inode1.LinksNum; i++)
	{
		DirectoryItem Dir;
		have = FindDir(config, inode1, i, Dir);
		if (have == true)
		{
			ReadINode(Dir.INodeNumber, inode2);
			if (inode2.FileType == ISDIR)
			{
				FreeINode(inode2.INodeNumber);
				for (int j = i; j<inode1.LinksNum - 1; j++)
				{
					DirectoryItem dir1, dir2;
					ReadDir(j, inode1, dir1);
					ReadDir(j + 1, inode1, dir2);
					dir1.INodeNumber = dir2.INodeNumber;
					strcpy(dir1.FileName, dir2.FileName);
					WriteDir(inode1, dir1, j);
					WriteDir(inode1, dir2, j + 1);
				}
				UpdataINode(-inode2.FileSize);
				inode1.FileSize -= inode2.FileSize;
				inode1.LinksNum--;
				break;
			}
		}
	}
	if (!have)
	{
		cout << "ϵͳ�Ҳ���";
		ShowCurPath();
		cout << "\\"<<config << endl << endl;
	}
	WriteINode(Path[num - 1], inode1);
}
void copy(char *config)
{
	int count;
	char *content = new char[4 * BLKSIZE];
//��ȡconfig������ļ�
	INode inode;
	ReadINode(Path[num - 1], inode);
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file!" << endl;
		exit(0);
	}
	for (int i = 0; i<inode.LinksNum; i++)
	{
		DirectoryItem DirItem;
		ReadDir(i, inode, DirItem);
		if (strcmp(DirItem.FileName, config) == 0)
		{
			INode inode1;
			ReadINode(DirItem.INodeNumber, inode1);
			if (inode1.FileType == ISDIR)
			{
				cout << "�޷����� " << DirItem.FileName << " �ļ�" << endl << endl;
				break;
			}
			else
			{
				strcpy(content, "\0");
				int i = 0;
				count = 0;
				while (inode1.blocks[i] != -1)
				{
					fin.seekp((FIRSTDBLK + inode1.blocks[i])*BLKSIZE);
					for (int j = 0; j<BLKSIZE; j++)
					{
						fin.read((char *)&content[count], 1);
						if (strcmp(&content[count], "\0") == 0)
							break;
						count++;
					}
					i++;
				}
			}
		}
	}
	fin.close();
//���뵽dst·����
	cd(dst);
			
//����
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//�ж�Ŀ¼���Ƿ��Ѿ��ﵽ8��
		cout << "��ǰĿ¼�����������ļ�ʧ��!" << endl << endl ;
	else
	{
		if (HaveSameName(config, CurINode))
			cout << "��Ŀ¼���ļ� " << config << " �Ѿ����ڡ�" << endl<< endl ;
		else
		{
			int INodeNum = AllocINode();
			if (INodeNum != -1)
			{
				int DBlkNum[4];
				int length = 0;
				length = strlen(content);
				count = length / BLKSIZE + 1;
				if (count>4)
				{
					cout << "�ļ�̫���޷�Ϊ�ļ�������̿ռ䡣" << endl << endl ;
					FreeINode(INodeNum);
					return;
				}
				for (int i = 0; i<count; i++)
				{
					DBlkNum[i] = AllocDBlk();
					if (DBlkNum[i] == -1)
					{
						FreeINode(INodeNum);
						for (int j = i - 1; j >= 0; j--)				//�ļ�ϵͳ���̿鲻��
							FreeDBlk(DBlkNum[i]);
						cout << "���̿��Ѿ����꣬�����ļ�ʧ��!" << endl << endl ;
						break;
					}
				}
				WriteDBlk(DBlkNum, count, content);

				//дĿ¼��
				DirectoryItem NewDir;
				SetDirItem(config, INodeNum, NewDir);
				WriteDir(CurINode, NewDir, CurINode.LinksNum);

				INode NewINode;
				SetFleINode(NewINode, length, INodeNum, DBlkNum, count);
				WriteINode(INodeNum, NewINode);
				cout << "�ļ����Ƴɹ�!" << endl << endl ;
				CurINode.LinksNum++;
				CurINode.FileSize += length;
				UpdataINode(length);
			}
			else
				cout << "����Ѿ����꣬�����ļ�ʧ��!" << endl<< endl  ;
		}
	}
	WriteINode(Path[num - 1], CurINode);

}
void move(char *config)
{
	int count;
	char *content = new char[4 * BLKSIZE];
//��ȡconfig������ļ�
	INode inode;
	ReadINode(Path[num - 1], inode);
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::binary);
	if (!fin)
	{
		cout << "Can't open this file!" << endl;
		exit(0);
	}
	for (int i = 0; i<inode.LinksNum; i++)
	{
		DirectoryItem DirItem;
		ReadDir(i, inode, DirItem);
		if (strcmp(DirItem.FileName, config) == 0)
		{
			INode inode1;
			ReadINode(DirItem.INodeNumber, inode1);
			if (inode1.FileType == ISDIR)
			{
				cout << "�޷����� " << DirItem.FileName << " �ļ�" << endl << endl;
				break;
			}
			else
			{
				strcpy(content, "\0");
				int i = 0;
				count = 0;
				while (inode1.blocks[i] != -1)
				{
					fin.seekp((FIRSTDBLK + inode1.blocks[i])*BLKSIZE);
					for (int j = 0; j<BLKSIZE; j++)
					{
						fin.read((char *)&content[count], 1);
						if (strcmp(&content[count], "\0") == 0)
							break;
						count++;
					}
					i++;
				}
			}
		}
	}
	fin.close();
//ɾ��ԭ·�����ļ�
	del(config);
//���뵽dst·����
	cd(dst);
			
//����
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//�ж�Ŀ¼���Ƿ��Ѿ��ﵽ8��
		cout << "��ǰĿ¼�����������ļ�ʧ��!" << endl << endl ;
	else
	{
		if (HaveSameName(config, CurINode))
			cout << "��Ŀ¼���ļ� " << config << " �Ѿ����ڡ�" << endl<< endl ;
		else
		{
			int INodeNum = AllocINode();
			if (INodeNum != -1)
			{
				int DBlkNum[4];
				int length = 0;
				length = strlen(content);
				count = length / BLKSIZE + 1;
				if (count>4)
				{
					cout << "�ļ�̫���޷�Ϊ�ļ�������̿ռ䡣" << endl << endl ;
					FreeINode(INodeNum);
					return;
				}
				for (int i = 0; i<count; i++)
				{
					DBlkNum[i] = AllocDBlk();
					if (DBlkNum[i] == -1)
					{
						FreeINode(INodeNum);
						for (int j = i - 1; j >= 0; j--)				//�ļ�ϵͳ���̿鲻��
							FreeDBlk(DBlkNum[i]);
						cout << "���̿��Ѿ����꣬�����ļ�ʧ��!" << endl << endl ;
						break;
					}
				}
				WriteDBlk(DBlkNum, count, content);

				//дĿ¼��
				DirectoryItem NewDir;
				SetDirItem(config, INodeNum, NewDir);
				WriteDir(CurINode, NewDir, CurINode.LinksNum);

				INode NewINode;
				SetFleINode(NewINode, length, INodeNum, DBlkNum, count);
				WriteINode(INodeNum, NewINode);
				cout << "�ļ��ƶ��ɹ�!" << endl << endl ;
				CurINode.LinksNum++;
				CurINode.FileSize += length;
				UpdataINode(length);
			}
			else
				cout << "����Ѿ����꣬�����ļ�ʧ��!" << endl<< endl  ;
		}
	}
	WriteINode(Path[num - 1], CurINode);

}

HANDLE mutual;	//һ��������
HANDLE signal1, signal2, signal3;	//�����ź���,�ж�������ܡ�������ִ��
char Command[20];
char cmd[20];
char config[30];
//�����߳�
DWORD WINAPI Accept(LPVOID lpParam)		//��������
//����DWORD��API����
//LPVOID�궨��ָ������͵�ָ��(������ָ����Ը��κ�ָ�븳ֵ)
{
	while (1)
	{
		WaitForSingleObject(signal1, INFINITE);	//������Դ1
		WaitForSingleObject(mutual, INFINITE);
		//����ΪINFINITE���߳�һֱ������ֱ��mutual��ָ��Ķ����Ϊ���ź�״̬Ϊֹ��Ϊ0��������
		ShowCurPath();		//��ʾ��ǰ·��
		cout << ">";
		cin.getline(Command, 20, '\n');			//��������,�������з�����
		ReleaseMutex(mutual);		//�ͷ����߳�ӵ�е�һ��������Ŀ���Ȩ
		ReleaseSemaphore(signal2, 1, NULL);		//�������
	}
	return 0;
}
DWORD WINAPI Analysis(LPVOID lpParam)		//��������
{
	while (1)
	{
		WaitForSingleObject(signal2, INFINITE);		//������Դ2
		WaitForSingleObject(mutual, INFINITE);
		SplitCmd(Command, cmd, config);		//��������������������
		ReleaseMutex(mutual);
		ReleaseSemaphore(signal3, 1, NULL);		//�������
	}
	return 0;
}
DWORD WINAPI Execute(LPVOID lpParam)		//ִ������
{
	while (1)
	{
		WaitForSingleObject(signal3, INFINITE);	//������Դ3
		WaitForSingleObject(mutual, INFINITE);
		InterpretCmd(cmd, config);		//�������
		ReleaseMutex(mutual);
		ReleaseSemaphore(signal1, 1, NULL);	//�������
	}
	return 0;
}
//�������������ź���
void CreateMS()
{
	mutual = CreateMutex(NULL, FALSE, NULL);		//����һ��������
	//����(ʹ�ò�����̳е�Ĭ������������������ϣ������ӵ�л����壬����ΪTRUE��һ��������ͬʱֻ����һ���߳�ӵ��FALSE��ָ����������������)
	if (mutual == NULL) //�ж��Ƿ�Ϊ��,����������ʧ��
	{
		printf("CreateMutex error: %d\n", GetLastError());	//GetLastError���ص�ֵͨ����api�����е���SetLastError��SetLastErrorEx����
		return;
	}
	signal1 = CreateSemaphore(NULL, 1, 1, NULL);		//�����ź���1
	//����(��ʾ���ò�����̳е�Ĭ��������,�ò����������ź����İ�ȫ���ԡ������ź����ĳ�ʼ�����������ź�������������ָ���ź������������)
	if (signal1 == NULL) //�������ֵΪ�գ����ź�������ʧ��
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return;
	}
	signal2 = CreateSemaphore(NULL, 0, 1, NULL);
	if (signal2 == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return;
	}
	signal3 = CreateSemaphore(NULL, 0, 1, NULL);
	if (signal3 == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	system("ver");
	printf("\n");
	DWORD acrid, anrid, exrid;	//32λ�޷�������
	HANDLE  acer, aner, exer;	//����ľ����������ľ������,ָ�򿪱ٿռ���׵�ַ
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fin)			//�ļ������ڣ���ʼ��
	{
		Init_SuperBlock();	//�����飬�ļ�ϵͳ�����࣬����ȫ���ļ���Ϣ
		Init_INode();		//����һ���ļ�ϵͳ�е�ȫ��inode���
		Init_DataBlock();	//��ʼ�����ݿ�
	}
	fin.close();
	strcpy(CurDirName, "chv:");	//��ǰĿ¼�ļ���
	Path[0] = 0;		//��ǰĿ¼·�����ţ����λΪ��ǰĿ¼�ļ�i����
	num = 1;			//��ǰĿ¼·���Ľ���� 
	ReadSuperBlock();	//���ļ��ж�ȡ������

	CreateMS();	//�������������ź���

	acer = CreateThread(NULL, 0, Accept, NULL, 0, &acrid);		//���߳�
	//����(NULLʹ��Ĭ�ϰ�ȫ�ԣ������Ա����̼̳߳С����ó�ʼջ�Ĵ�С�����ֽ�Ϊ��λ��0Ĭ�Ͻ�ʹ������øú������߳���ͬ��ջ�ռ��С��)
	//����(�̺߳������̲߳������̴߳������ԡ��߳�ID��)
	if (acer == NULL)
		return 0;		//�̴߳���ʧ��
	aner = CreateThread(NULL, 0, Analysis, NULL, 0, &anrid);		//�����߳�
	if (aner == NULL)
		return 0;		//�̴߳���ʧ��
	exer = CreateThread(NULL, 0, Execute, NULL, 0, &exrid);		//ִ���߳�
	if (exer == NULL)
		return 0;		//�̴߳���ʧ��

	WaitForSingleObject(acer, INFINITE);		//���ȴ����ڹ���״̬ʱ��������رգ���ô������Ϊ��δ�����
	//��Ϊ0����ú����������أ���ΪINFINITE�����߳�һֱ������ֱ��acer��ָ��Ķ����Ϊ���ź�״̬ʱΪֹ��
	WaitForSingleObject(aner, INFINITE);
	WaitForSingleObject(exer, INFINITE);
	CloseHandle(mutual);	//�ر��߳̾��
	CloseHandle(signal1);
	CloseHandle(signal2);
	CloseHandle(signal3);
	system("pause");
	return 0;
}

