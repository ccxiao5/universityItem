// os.cpp : 定义控制台应用程序的入口点。
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

//宏定义
#define ISFILE 0			//判断文件
#define ISDIR	 1			//	判断路径
#define UNKNOW -1	//	文件类型判断标识符
#define BLKSIZE 128	//	块大小
#define FIRSTIBLK 4	//第一个I结点块号
#define FIRSTDBLK 24	//第一个数据块号
#define INODES 80	//I结点数
#define DATABLKS 216	//数据块大小
#define DIRITEMS 8		//	目录项目
#define MAXDEPTH 20	
#define BLOCKNUM 4	//	块个数

int dir_sum=0,file_sum=0,used_sum=0;

struct SuperBlock	//超级块结构体，设置全局文件包含信息
{
	int MaxSize;				//文件系统的大小
	int FirstINodeBlk;			//第一个I结点块号
	int FirstDBlk;				//第一个数据块号
	int BlockSize;				//每个物理块大小
	bool INodeStatus[INODES];		//INode状态(空闲或占用)
	bool DataBlockStatus[DATABLKS];		//数据块状态(空闲或占用)
}superblock;				//定义全局变量 superblock
struct INode//i结点结构体，也就是一个目录
{
	int FileType;				//文件类型 
	int INodeNumber;			//i-node号
	int LinksNum;				//链接数
	int FileSize;				//文件大小
	int blocks[BLOCKNUM];			//占用的数据块
}inode[INODES];	//定义全局inode数组
struct	DirectoryItem//目录结构体
{
	int INodeNumber;		//INode号
	char FileName[12];		//文件名
};
//**********I-Node函数**********
void Init_INode();	//初始化i结点
int AllocINode();	//申请I-结点
void WriteINode(int, INode);		//写I-结点到文件中
void ReadINode(int, INode&);		//从文件中读取I-结点
void FreeINode(int);	//释放I-结点
void SetDirINode(INode&, int, int);	//初始化新建的目录I-结点
void SetFleINode(INode&, int, int, int*, int);//初始化新建的文件I-结点
void UpdataINode(int);			//更新I-结点
//**********超级块函数**********
void Init_SuperBlock();		//初始化超级块
void WriteSuperBlock();		//写超级块到文件中
void ReadSuperBlock();		//从文件中读取超级块
bool HaveSameName(char *, INode);	//寻找是否有相同的文件名
//**********目录项函数**********
bool FindDir(char *, INode, int, DirectoryItem&);//查找目录项
void SetDirItem(char *c, int, DirectoryItem&);	//初始化新建的目录项
void WriteDir(INode, DirectoryItem, int);		//写目录项
void ReadDir(int, INode, DirectoryItem &);	//读目录项
//**********shell操作函数**********
void mkdir(char *config);//新建一个目录	
void more(char *config);//逐屏显示输出
void dir(char *config);	//显示目录中的文件和子目录
void help(char*config);	//帮助
void cd(char*config);	//显示或改变当前目录
void del(char*config);	//删除一个文件
void format();		//格式化磁盘
void InterpretCmd();	//对命令解析
void mkfle(char *config);//创建一个文件
void rm(char *config);//删除一个目录
void copy(char *config);//拷贝文件
void move(char *config);//移动一个文件
void ShowCurPath();	
//**********数据块函数**********
void Init_DataBlock();		//初始化数据块
int AllocDBlk();		//申请数据块
void WriteDBlk(int *, int, char *);//释放数据块
void FreeDBlk(int);		//写数据块
////**********全局变量**********
int Path[20];								//当前目录路径结点号 
int num;										//当前目录路径的结点数
char CurDirName[12];		//当前目录文件名
char dst[30];				//目的路径

//**********超级块函数**********
void Init_SuperBlock()//初始化超级块
{
	superblock.MaxSize = 240 * BLKSIZE;		//超级块最大大小
	superblock.BlockSize = BLKSIZE;				//超级块大小
	superblock.FirstINodeBlk = FIRSTIBLK;	//第一个结点块号
	superblock.FirstDBlk = FIRSTDBLK;		//第一个数据块号
	for (int i = 0; i<INODES; i++)
		superblock.INodeStatus[i] = true;		//设置i结点状态
	for (int i = 0; i<DATABLKS; i++)
		superblock.DataBlockStatus[i] = true;	//设置数据块状态

	superblock.INodeStatus[0] = false;		//0号I-结点的被根目录使用
	superblock.DataBlockStatus[0] = false;//同理，0号数据块被根目录使用
	fstream fout; //定义一个文件流，主要是文件的读写操作
	fout.open("This is chenghuwei's file", ios::out | ios::binary);	//写文件/二进制文件流
	if (!fout)//				避免一些特殊情况打开错误
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.write((char*)&superblock, FIRSTIBLK*BLKSIZE);//强制转化char *类型从而储存文件，FIRSTIBLK*BLKSIZE可以换成BLKSIZE，我这样做是因为避免了传参时候创建副本
	fout.close();
}
void WriteSuperBlock()//写超级块到文件中
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);//打开初始化时的文件
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.write((char*)&superblock, FIRSTIBLK*BLKSIZE);
	fout.close();
}
void ReadSuperBlock()//读取超级块
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
//**********I-Node函数**********
void Init_INode()//初始化I-结点
{
	for (int i = 0; i<INODES; i++)
	{
		inode[i].FileType = UNKNOW;	//文件类型未知
		inode[i].INodeNumber = i;		//I-node号
		inode[i].LinksNum = 0;		//链接数为0
		inode[i].FileSize = 0;		//文件大小为0
		for (int j = 0; j<BLOCKNUM; j++)	//每个I结点的所有数据块
			inode[i].blocks[j] = -1;
	}

	inode[0].FileType = ISDIR;//0号I-结点被根目录占用，0号数据块被根目录使用，文件类型ISDIR=1
	inode[0].blocks[0] = 0;		//根目录占用数据块
	inode[0].LinksNum = 2;	//有两个链接数	

	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp(FIRSTIBLK*BLKSIZE);//设置输出流指针的位置，即最后的位置
	for (int i = 0; i<INODES; i++)
		fout.write((char*)&inode[i], sizeof(INode));

	fout.close();
}
int AllocINode()//申请I-结点
{
	for (int i = 0; i<INODES; i++)
	if (superblock.INodeStatus[i] == true)	//第i个结点可被申请
	{
		superblock.INodeStatus[i] = false;
		return i;
	}
	return -1;
}
void FreeINode(int Index)	//释放I-结点
{
	inode[Index].FileType = UNKNOW;	//类型还原
	inode[Index].INodeNumber = Index;	//I-节点号还原
	inode[Index].LinksNum = 0;				//链接数还原
	inode[Index].FileSize = 0;
	for (int j = 0; j<BLOCKNUM; j++)
	{
		if (inode[Index].blocks[j] != -1)		//如果存在数据块，那么清除存在的块
		{
			int DBlockNum = inode[Index].blocks[j]; 
			FreeDBlk(DBlockNum);//把存在的数据块在超级块中删除
		}
		inode[Index].blocks[j] = -1;
	}
	WriteINode(Index, inode[Index]);
	superblock.INodeStatus[Index] = true;//清除后，I-结点状态回为可用状态
}
void WriteINode(int Index, INode inode)//写I-结点到文件中
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp(FIRSTIBLK*BLKSIZE + sizeof(INode)*Index);//写入文件的结束位置
	fout.write((char *)&inode, sizeof(INode));
	fout.close();
}
void ReadINode(int Index, INode &inode)//从文件中读取I-结点
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
void SetDirINode(INode &inode, int INodeNum, int DBlkNum)//初始化新建的目录I-结点
{
	inode.FileSize = 0;
	inode.FileType = ISDIR;		//结点就是一个目录
	inode.INodeNumber = INodeNum;
	inode.LinksNum = 2;
	for (int i = 0; i<4; i++)
		inode.blocks[i] = -1;
	inode.blocks[0] = DBlkNum;
}
void SetFleINode(INode &inode, int FileLength, int INodeNum, int *DBlkNum, int count)//初始化新建的文件I-结点
{
	inode.FileSize = FileLength;  
	inode.FileType = ISFILE;		//文件
	inode.INodeNumber = INodeNum;
	inode.LinksNum = 0;
	for (int i = 0; i<4; i++)
		inode.blocks[i] = -1;	
	for (int i = 0; i<count; i++)
		inode.blocks[i] = DBlkNum[i]; //拷贝每个文件的数据块
}
void UpdataINode(int length)//更新I-结点
{
	for (int i = 0; i<num; i++)//遍历该路径下的所有节点
	{
		INode inode1;
		ReadINode(Path[i], inode1);//读取该结点号
		inode1.FileSize += length;
		WriteINode(Path[i], inode1);//写入更新后得结点信息
	}
}
bool HaveSameName(char *config, INode inode)//寻找是否有相同的文件名
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
//**********目录项函数**********
bool FindDir(char *config, INode inode, int Index, DirectoryItem &Dir)//查找目录项
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
void SetDirItem(char *config, int Index, DirectoryItem &Dir)//初始化新建的目录项
{
	strcpy(Dir.FileName, config);
	Dir.INodeNumber = Index;
}
void WriteDir(INode inode, DirectoryItem Dir, int Index)//写目录项
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
void ReadDir(int index, INode inode, DirectoryItem &Dir)//读目录项
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
//**********数据块函数**********
void Init_DataBlock()//初始化数据块
{
	fstream fout;
	fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fout)
	{
		cout << "Can't open this file" << endl;
		exit(0);
	}
	fout.seekp(FIRSTDBLK*BLKSIZE);
	DirectoryItem RootDir[2];	//初始化根目录
	strcpy(RootDir[0].FileName, ".");
	RootDir[0].INodeNumber = 0;
	strcpy(RootDir[1].FileName, "..");
	RootDir[1].INodeNumber = 0;
	for (int i = 0; i<2; i++)
		fout.write((char*)&RootDir[i], sizeof(DirectoryItem));
	fout << setw(DATABLKS*BLKSIZE - 1 - sizeof(DirectoryItem)* 2) << '\0';//调整格式
	fout.close();
}
int AllocDBlk()//申请数据块
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
void FreeDBlk(int Index)//释放数据块
{
	superblock.DataBlockStatus[Index] = true;
}
void WriteDBlk(int *DBlkNum, int count, char *content)//写数据块
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
//**********shell操作函数**********
void SplitCmd(char *Cmd, char *cmd, char *config)//分割命令
{
	int i;
	int k = strlen(Cmd);
	for (i = 1; i<(int)strlen(Cmd); i++)//找到' '或者结束位置
	{
		if ((Cmd[i] == ' ' || Cmd[i] == '\t') && (Cmd[i - 1] != ' '&&Cmd[i - 1] != '\t'))
		{
			k = i;
			break;
		}
	}
//********解析无空白字符的命令********
	int count = 0;
	for (i = 0; i<k; i++)
	{
		if (Cmd[i] != ' '&&Cmd[i] != '\t')
			cmd[count++] = Cmd[i];
	}
	cmd[count] = '\0';
//********解析无空白字符的参数********
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
//********解析copy&mv的dst地址********
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
		for (; x<(int)strlen(Cmd); x++)//截取第二个' '后的第一个字符位置
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
void InterpretCmd(char *cmd, char *config)//命令解释
{
	if (_stricmp(cmd, "mkdir") == 0)//不区分大小写的比较
	{
		if (_stricmp(config, "") == 0)
			cout << "命令语法不正确。" << endl << endl;
		else
		{
			mkdir(config);
			WriteSuperBlock();
		}
	}
	else if (_stricmp(cmd, "dir") == 0)
		{ 
			dir_sum=0; //'/s'命令下总目录个数
			file_sum=0;//'/s'命令下总文件个数
			used_sum=0;//'/s'命令下已使用字节
			dir(config);
		}
	else if (_stricmp(cmd, "cd") == 0)
	{
		if (_stricmp(config, "") == 0)	//当前路径
		{
			ShowCurPath();
			cout << endl ;
		}
		else
			cd(config);
	}
	else if (_stricmp(cmd, "del") == 0)//删除文件
	{
		del(config);
		WriteSuperBlock();
		cout << endl;
	}
	else if (_stricmp(cmd, "rm") == 0)//删除目录
	{
		rm(config);
		WriteSuperBlock();
		cout << endl;
	}
	else if (_stricmp(cmd, "mkfle") == 0)
	{
		if (_stricmp(config, "") == 0)
			cout << "命令语法不正确。" << endl << endl;
		else if (strlen(config)>12)
			cout << config << "的完整路径太长" << endl << endl;
		else
		{
			mkfle(config);
			WriteSuperBlock();
		}
	}
	else if (_stricmp(cmd, "more") == 0)
	{
		if (_stricmp(config, "") == 0)
			cout << "命令语法不正确。" << endl << endl;
		else
			more(config);
	}
	else if (_stricmp(cmd, "format") == 0)
	{
		format();
		cout << "文件系统格式化完毕!" << endl << endl;
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
		cout << "'" << cmd << "'" << " 不是内部或外部命令，也不是可运行的程序" << endl << "或批处理文件。" << endl << endl;
}
string charsoftime(const struct tm * ptm)//获取当前时间
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
void ShowCurPath()//显示当前路径
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
			ReadDir(j, CurINode, Dir);		//读目录项
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
	if (CurINode.LinksNum >= 8)					//判断目录项是否已经达到8个
		cout << "当前目录已满，创建文件失败!" << endl << endl ;
	else
	{
		if (HaveSameName(config, CurINode))
			cout << "子目录或文件 " << config << " 已经存在。" << endl<< endl ;
		else
		{
			int INodeNum = AllocINode();
			if (INodeNum != -1)
			{
				char ch;
				cout << "是否输入文件内容(Y\\N)";
				cin >> ch;

				//清空缓冲区中的换行符
				cin.clear();
				fflush(stdin);

				int count = 0;
				int DBlkNum[4];
				char *content = '\0';
				int length = 0;
				if (ch == 'Y' || ch == 'y')
				{
					content = new char[4 * BLKSIZE];
					cout << "请输入文件内容:" << endl;
					cin.getline(content, 4 * BLKSIZE, '\n');
					length = strlen(content);
					content[length] = '\0';
					count = length / BLKSIZE + 1;
					if (count>4)
					{
						cout << "文件太大，无法为文件分配磁盘空间。" << endl << endl ;
						FreeINode(INodeNum);
						return;
					}
					for (int i = 0; i<count; i++)
					{
						DBlkNum[i] = AllocDBlk();
						if (DBlkNum[i] == -1)
						{
							FreeINode(INodeNum);
							for (int j = i - 1; j >= 0; j--)				//文件系统磁盘块不足
								FreeDBlk(DBlkNum[i]);
							cout << "磁盘块已经用完，创建文件失败!" << endl << endl ;
							break;
						}
					}
					WriteDBlk(DBlkNum, count, content);
				}

				//写目录项
				DirectoryItem NewDir;
				SetDirItem(config, INodeNum, NewDir);
				WriteDir(CurINode, NewDir, CurINode.LinksNum);

				INode NewINode;
				SetFleINode(NewINode, length, INodeNum, DBlkNum, count);
				WriteINode(INodeNum, NewINode);
				cout << "文件创建成功!" << endl << endl ;
				CurINode.LinksNum++;
				CurINode.FileSize += length;
				UpdataINode(length);
			}
			else
				cout << "结点已经用完，创建文件失败!" << endl<< endl  ;
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
			else if (strcmp(config, "..") == 0)	//返回上级目录
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
					cout << "目录名称无效。" << endl ;
					return;
				}
				Path[num] = Dir.INodeNumber;
				strcpy(CurDirName, Dir.FileName);
				num++;
			}
		}
	}
	if (flag == false)
		cout << "系统找不到指定的路径" << endl << endl;
}
void dir(char *config)
{
	int DirCount = 0;
	int FleCount = 0;
	int CurTotalFileSize = 0;
	INode inode, inode1;
	if(strstr(config,"\\"))
		printf("输入命令错误！！！\n");
		else
	{
		ReadINode(Path[num - 1], inode);
		cout << endl;
		ShowCurPath();	//显示当前路径
		cout << "的目录" << endl << endl;
		if(!strstr(config,"."))
		{
			for (int i = 0; i<inode.LinksNum; i++)
			{
				DirectoryItem Dir;
				ReadDir(i, inode, Dir);	//读目录
				ReadINode(Dir.INodeNumber, inode1);
				time_t CurTime = time(0);
				string time = charsoftime(localtime(&CurTime));
				cout << time.c_str() << '\t';		//输出创建时间
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
			cout << "\t\t" << FleCount << "个文件" << "\t\t" << CurTotalFileSize << "\t字节" << endl;
			cout << "\t\t" << DirCount << "个目录" << "\t\t" << superblock.MaxSize - root.FileSize << "\t可用字节";
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
				ReadDir(i, inode, Dir);	//读目录
				ReadINode(Dir.INodeNumber, inode1);
				time_t CurTime = time(0);
				string time = charsoftime(localtime(&CurTime));
				if (inode1.FileType == ISFILE&&strstr(Dir.FileName,strstr(config,".")))		
				{
					cout << time.c_str() << '\t';		//输出创建时间
					FleCount++;
					cout << "\t" << inode1.FileSize << "\t" << Dir.FileName << endl;
					CurTotalFileSize += inode1.FileSize;
				}
		
			}
			INode root;
			ReadINode(Path[0], root);
			cout << "\t\t" << FleCount << "个文件" << "\t\t" << CurTotalFileSize << "\t字节" << endl;
			cout << "\t\t" << DirCount << "个目录" << "\t\t" << superblock.MaxSize - root.FileSize << "\t可用字节";
			cout << endl << endl;
		}
		if(strstr(config,"/s"))
		{
			for (int i = 0; i<inode.LinksNum; i++)
			{
				DirectoryItem Dir;
				ReadDir(i, inode, Dir);	//读目录
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
			printf("\t所列文件总数:\n");
			cout << "\t\t" << file_sum << "个文件" << "\t\t" << used_sum << "\t字节" << endl;
			cout << "\t\t" << dir_sum << "个目录" << "\t\t" << superblock.MaxSize - used_sum << "\t可用字节";
			cout << endl << endl;
		}
	}

}
void mkdir(char *config)
{
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//判断目录项是否已经达到8个
		cout << "当前目录已满，创建子目录失败!" << endl << endl;
	else
	{
		if (HaveSameName(config, CurINode))
			cout <<"创建子目录失败!" << endl << endl;
		else
		{
			int INodeNum = AllocINode();	//申请I-结点
			if (INodeNum != -1)
			{
				int DBlkNum = AllocDBlk();	//申请I-结点成功，申请数据块
				if (DBlkNum != -1)
				{
					DirectoryItem NewDir;		//目录结构对象
					SetDirItem(config, INodeNum, NewDir);		//初始化新建的目录项
					WriteDir(CurINode, NewDir, CurINode.LinksNum);	//写目录项
					fstream fout;
					fout.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
					if (!fout)
					{
						cout << "Can't open this file" << endl;
						exit(0);
					}
					fout.seekp((FIRSTDBLK + DBlkNum)*BLKSIZE);	//找到写入位置
					DirectoryItem ChildDir[2];
					strcpy(ChildDir[0].FileName,".");
					ChildDir[0].INodeNumber=INodeNum;
					strcpy(ChildDir[1].FileName,"..");
					ChildDir[1].INodeNumber=CurINode.INodeNumber;	
					for (int i = 0; i<2; i++)
						fout.write((char*)&ChildDir[i], sizeof(DirectoryItem));
					fout.close();

					INode NewINode;
					SetDirINode(NewINode, INodeNum, DBlkNum);		//初始化新建的目录I-结点
					WriteINode(INodeNum, NewINode);		//写I-结点到文件中
					cout << "目录创建成功!" << endl << endl;
					CurINode.LinksNum++;
				}
				else
				{
					FreeINode(INodeNum);		//释放I-结点
					cout << "磁盘块已经用完，创建子目录失败!" << endl << endl;
				}
			}
			else
				cout << "结点已经用完，创建子目录失败!" << endl << endl;
		}
	}
	WriteINode(Path[num - 1], CurINode);	//写I-结点到文件中
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
				cout << "无法访问 " << DirItem.FileName << " 文件" << endl << endl;
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
		cout << "系统找不到";
		ShowCurPath();
		cout << config << endl << endl;
	}
	WriteINode(Path[num - 1], inode1);
}
void help(char *config)
{
		system("cls");
		printf("\n\n\n\n\n\n\n\t\t\t\t   /***************Shell指令***************/\n");
        printf("\n\n");
		printf("\t\t\t\tCD             显示当前目录的名称或将其更改。\n");
		printf("\t\t\t\tCOPY           将至少一个文件复制到另一个位置。\n");
		printf("\t\t\t\tCLS            清除屏幕。\n");
		printf("\t\t\t\tDEL            删除至少一个文件。\n");
		printf("\t\t\t\tDIR            显示一个目录中的文件和子目录。\n");
		printf("\t\t\t\tEXIT           退出 CMD.EXE 程序(命令解释程序)。\n");  
		printf("\t\t\t\tFORMAT         格式化磁盘，以便用于 Windows。\n");
		printf("\t\t\t\tHELP           提供 Windows 命令的帮助信息。\n");
		printf("\t\t\t\tMKDIR          创建一个目录。\n");
		printf("\t\t\t\tMKFLE          创建一个文件\n");  
		printf("\t\t\t\tMORE           逐屏显示输出。\n");
		printf("\t\t\t\tMOVE           将一个或多个文件从一个目录移动到另一个目录。\n");
		printf("\t\t\t\tRM          删除目录。\n");
		printf("\t\t\t\tTIME           显示或设置系统时间。\n");
		printf("\n\n\t\t\t\t按任意键退出~！！\n");
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
		cout << "系统找不到";
		ShowCurPath();
		cout << "\\"<<config << endl << endl;
	}
	WriteINode(Path[num - 1], inode1);
}
void copy(char *config)
{
	int count;
	char *content = new char[4 * BLKSIZE];
//读取config里面的文件
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
				cout << "无法访问 " << DirItem.FileName << " 文件" << endl << endl;
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
//进入到dst路径下
	cd(dst);
			
//复制
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//判断目录项是否已经达到8个
		cout << "当前目录已满，创建文件失败!" << endl << endl ;
	else
	{
		if (HaveSameName(config, CurINode))
			cout << "子目录或文件 " << config << " 已经存在。" << endl<< endl ;
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
					cout << "文件太大，无法为文件分配磁盘空间。" << endl << endl ;
					FreeINode(INodeNum);
					return;
				}
				for (int i = 0; i<count; i++)
				{
					DBlkNum[i] = AllocDBlk();
					if (DBlkNum[i] == -1)
					{
						FreeINode(INodeNum);
						for (int j = i - 1; j >= 0; j--)				//文件系统磁盘块不足
							FreeDBlk(DBlkNum[i]);
						cout << "磁盘块已经用完，创建文件失败!" << endl << endl ;
						break;
					}
				}
				WriteDBlk(DBlkNum, count, content);

				//写目录项
				DirectoryItem NewDir;
				SetDirItem(config, INodeNum, NewDir);
				WriteDir(CurINode, NewDir, CurINode.LinksNum);

				INode NewINode;
				SetFleINode(NewINode, length, INodeNum, DBlkNum, count);
				WriteINode(INodeNum, NewINode);
				cout << "文件复制成功!" << endl << endl ;
				CurINode.LinksNum++;
				CurINode.FileSize += length;
				UpdataINode(length);
			}
			else
				cout << "结点已经用完，创建文件失败!" << endl<< endl  ;
		}
	}
	WriteINode(Path[num - 1], CurINode);

}
void move(char *config)
{
	int count;
	char *content = new char[4 * BLKSIZE];
//读取config里面的文件
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
				cout << "无法访问 " << DirItem.FileName << " 文件" << endl << endl;
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
//删除原路径的文件
	del(config);
//进入到dst路径下
	cd(dst);
			
//复制
	INode CurINode;
	ReadINode(Path[num - 1], CurINode);
	if (CurINode.LinksNum >= 8)					//判断目录项是否已经达到8个
		cout << "当前目录已满，创建文件失败!" << endl << endl ;
	else
	{
		if (HaveSameName(config, CurINode))
			cout << "子目录或文件 " << config << " 已经存在。" << endl<< endl ;
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
					cout << "文件太大，无法为文件分配磁盘空间。" << endl << endl ;
					FreeINode(INodeNum);
					return;
				}
				for (int i = 0; i<count; i++)
				{
					DBlkNum[i] = AllocDBlk();
					if (DBlkNum[i] == -1)
					{
						FreeINode(INodeNum);
						for (int j = i - 1; j >= 0; j--)				//文件系统磁盘块不足
							FreeDBlk(DBlkNum[i]);
						cout << "磁盘块已经用完，创建文件失败!" << endl << endl ;
						break;
					}
				}
				WriteDBlk(DBlkNum, count, content);

				//写目录项
				DirectoryItem NewDir;
				SetDirItem(config, INodeNum, NewDir);
				WriteDir(CurINode, NewDir, CurINode.LinksNum);

				INode NewINode;
				SetFleINode(NewINode, length, INodeNum, DBlkNum, count);
				WriteINode(INodeNum, NewINode);
				cout << "文件移动成功!" << endl << endl ;
				CurINode.LinksNum++;
				CurINode.FileSize += length;
				UpdataINode(length);
			}
			else
				cout << "结点已经用完，创建文件失败!" << endl<< endl  ;
		}
	}
	WriteINode(Path[num - 1], CurINode);

}

HANDLE mutual;	//一个互斥量
HANDLE signal1, signal2, signal3;	//三个信号量,判断命令接受、解析、执行
char Command[20];
char cmd[20];
char config[30];
//声明线程
DWORD WINAPI Accept(LPVOID lpParam)		//接受命令
//返回DWORD的API函数
//LPVOID宏定义指向空类型的指针(空类型指针可以给任何指针赋值)
{
	while (1)
	{
		WaitForSingleObject(signal1, INFINITE);	//申请资源1
		WaitForSingleObject(mutual, INFINITE);
		//参数为INFINITE，线程一直被挂起，直到mutual所指向的对象变为有信号状态为止，为0立即返回
		ShowCurPath();		//显示当前路径
		cout << ">";
		cin.getline(Command, 20, '\n');			//接受命令,遇到换行符结束
		ReleaseMutex(mutual);		//释放由线程拥有的一个互斥体的控制权
		ReleaseSemaphore(signal2, 1, NULL);		//解除锁定
	}
	return 0;
}
DWORD WINAPI Analysis(LPVOID lpParam)		//解析命令
{
	while (1)
	{
		WaitForSingleObject(signal2, INFINITE);		//申请资源2
		WaitForSingleObject(mutual, INFINITE);
		SplitCmd(Command, cmd, config);		//分离命令参数，命令解析
		ReleaseMutex(mutual);
		ReleaseSemaphore(signal3, 1, NULL);		//解除锁定
	}
	return 0;
}
DWORD WINAPI Execute(LPVOID lpParam)		//执行命令
{
	while (1)
	{
		WaitForSingleObject(signal3, INFINITE);	//申请资源3
		WaitForSingleObject(mutual, INFINITE);
		InterpretCmd(cmd, config);		//命令解释
		ReleaseMutex(mutual);
		ReleaseSemaphore(signal1, 1, NULL);	//解除锁定
	}
	return 0;
}
//创建互斥量，信号量
void CreateMS()
{
	mutual = CreateMutex(NULL, FALSE, NULL);		//生成一个互斥量
	//参数(使用不允许继承的默认描述符。创建进程希望立即拥有互斥体，则设为TRUE，一个互斥体同时只能由一个线程拥有FALSE。指定互斥体对象的名字)
	if (mutual == NULL) //判断是否为空,互斥量生成失败
	{
		printf("CreateMutex error: %d\n", GetLastError());	//GetLastError返回的值通过在api函数中调用SetLastError或SetLastErrorEx设置
		return;
	}
	signal1 = CreateSemaphore(NULL, 1, 1, NULL);		//生成信号量1
	//参数(表示采用不允许继承的默认描述符,该参数定义了信号量的安全特性。设置信号量的初始计数。设置信号量的最大计数。指定信号量对象的名称)
	if (signal1 == NULL) //如果返回值为空，则信号量生成失败
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
	DWORD acrid, anrid, exrid;	//32位无符号整形
	HANDLE  acer, aner, exer;	//对象的句柄，最基本的句柄类型,指向开辟空间的首地址
	fstream fin;
	fin.open("This is chenghuwei's file", ios::in | ios::out | ios::binary);
	if (!fin)			//文件不存在，初始化
	{
		Init_SuperBlock();	//超级块，文件系统的心脏，保存全局文件信息
		Init_INode();		//保存一个文件系统中的全部inode结点
		Init_DataBlock();	//初始化数据块
	}
	fin.close();
	strcpy(CurDirName, "chv:");	//当前目录文件名
	Path[0] = 0;		//当前目录路径结点号，最后位为当前目录文件i结点号
	num = 1;			//当前目录路径的结点数 
	ReadSuperBlock();	//从文件中读取超级块

	CreateMS();	//创建互斥量，信号量

	acer = CreateThread(NULL, 0, Accept, NULL, 0, &acrid);		//读线程
	//参数(NULL使用默认安全性，不可以被子线程继承。设置初始栈的大小，以字节为单位，0默认将使用与调用该函数的线程相同的栈空间大小。)
	//参数(线程函数。线程参数。线程创建属性。线程ID。)
	if (acer == NULL)
		return 0;		//线程创建失败
	aner = CreateThread(NULL, 0, Analysis, NULL, 0, &anrid);		//分析线程
	if (aner == NULL)
		return 0;		//线程创建失败
	exer = CreateThread(NULL, 0, Execute, NULL, 0, &exrid);		//执行线程
	if (exer == NULL)
		return 0;		//线程创建失败

	WaitForSingleObject(acer, INFINITE);		//当等待仍在挂起状态时，句柄被关闭，那么函数行为是未定义的
	//若为0，则该函数立即返回；若为INFINITE，则线程一直被挂起，直到acer所指向的对象变为有信号状态时为止。
	WaitForSingleObject(aner, INFINITE);
	WaitForSingleObject(exer, INFINITE);
	CloseHandle(mutual);	//关闭线程句柄
	CloseHandle(signal1);
	CloseHandle(signal2);
	CloseHandle(signal3);
	system("pause");
	return 0;
}

