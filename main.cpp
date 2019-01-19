#include <iostream>
#include <fstream>
#include <string>
#include <io.h>

#include <tchar.h>
#include <locale.h>
#include <ShlObj.h>
#define		MAX_FILE_FOR_SEL 20	//�������ѡ����ļ���
using namespace std;

//***************************************************//

void	open_file(char *filename);											//	���ļ�	Ԥ���ж��ļ��Ƿ����
int		count_of_character(char *filename);									//	ͳ���ַ�����
int		count_of_word(char *filename);										//	ͳ�Ƶ��ʸ���
int		count_of_line(char *filename,bool print = true);					//	ͳ������
void	count_of_more(char *filename,int *c,int *n,int *e);					//	ͳ�Ƹ�������
void	recursion_function(char *parameter,char *filename);					//	�ݹ����
void	file_dialog();														//	�������



char * wchar2char(const wchar_t* wchar)					//	wchar to char
{
	char * m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}






int main(int argc, char *argv[]){
	if (strcmp(argv[1], "-s") && strcmp(argv[1],"-x")){			//	parameter �� "-s" "-x"
		open_file(argv[2]);										//	Ԥ���ж��ļ��Ƿ���� ����ֱ���˳�
	}
	

	int		count_char = 0;				//	�ַ���ͳ��
	int		count_line = 0;				//	����ͳ��
	int		count_word = 0;				//	������ͳ��
	int		count_code_line = 0;		//	������ͳ��
	int		count_note_line = 0;		//	ע����ͳ��
	int		count_empty_line = 0;		//	�հ���ͳ��

	if (!strcmp(argv[1], "-c"))
		count_char = count_of_character(argv[2]);				//	ִ�� �ַ�ͳ�Ʋ���

	else if (!strcmp(argv[1], "-l"))
		count_line = count_of_line(argv[2]);				//	ִ�� ����ͳ�Ʋ���

	else if (!strcmp(argv[1], "-w"))
		count_word = count_of_word(argv[2]);					//	ִ�� ����ͳ�Ʋ���

	else if (!strcmp(argv[1], "-a"))
		count_of_more(argv[2], &count_code_line, &count_note_line, &count_empty_line);			//	ִ�� ���ิ�Ӳ���

	else if (!strcmp(argv[1], "-s"))
		recursion_function(argv[2],argv[3]);					//	ִ�� �ݹ����

	else if (!strcmp(argv[1], "-x"))
		file_dialog();											//	ִ�� ��Ŀ¼�������

	return 0;
}





void	open_file(char *filename){
	ifstream file(filename);
	if (!file.is_open()){
		cout << filename << " �ļ������ڣ�" << endl;
		file.close();
		exit(0);
	}
}

int		count_of_character(char *filename){				//	ͳ���ַ�����
	ifstream file(filename);
	file.seekg(0, ios::end);							//	ת���ļ�β
	int count = file.tellg();							//	��ȡ�ַ���		�ַ�'\n'������ �����Ҫ ��ȥ���� ��1
	cout << filename << " �ļ��� �ַ���Ϊ " << count - count_of_line(filename,false) + 1 << endl;
	return count - count_of_line(filename,false) + 1;
}

int		count_of_line(char *filename,bool print){					//	ͳ���ļ�����
	ifstream	file(filename);
	int			line = 0;					//	ͳ������
	char		buffer[1024];
	while (!file.eof())
	{
		file.getline(buffer, 1024);			//	���ж�ȡ����
		line++;								//	������һ
	}
	if (print){
		cout << filename << " �ļ��� ����Ϊ " << line << endl;
	}
	return line;
}

int		count_of_word(char *filename){					//	ͳ���ļ�������
	ifstream	file(filename);
	int			word = 0;
	string		buffer;
	while (file >> buffer){
		word++;
	}
	cout << filename << " �ļ��� ������Ϊ " << word << endl;
	return word;
}

void	count_of_more(char *filename,int *c,int *n,int *e){
	int		empty_line = 0;					//	�հ���
	int		code_line = 0;					//	������
	int		note_line = 0;					//	ע����
	char	buffer[1024];

	ifstream file(filename);
	while (!file.eof()){
		memset(buffer, 0, sizeof(buffer));			//	���㻺����
		file.getline(buffer, 1024);					//	��ȡһ������

		if (buffer[0] == '\0')								//	��ȡ�����ַ� Ϊ ��
			empty_line++;									
		else{												//	��ȡ�����ַ� �� ��
			int empty = 0;							//	��¼ �ո� �� ˮƽ�Ʊ���
			int	note = 0;							//	��¼ ע����
			int code = 0;							//	��¼ ������
			int i = 0;
			for (; buffer[i] != '\0'; i++){
				if (buffer[i] == ' ' || buffer[i] == '\t')	//	���� �ո� ˮƽ�Ʊ��� ��¼
					empty++;
				else if (buffer[i] == '/')					//	�ȳ��� //
					note++;
				else if (buffer[i] != '/' && buffer[i] != '{' && buffer[i] != '}')		//	�ȳ��� �����ַ�
					code++;
				if (note > code){
					note_line++;
					i = -1;
					break;
				}
				if (code > note){
					code_line++;
					i = -1;
					break;
				}
			}
			if (empty == i || empty == i - 1)
				empty_line++;
		}
	}
	cout << filename << " �ļ��� ������Ϊ " << code_line << endl;
	cout << filename << " �ļ��� ע����Ϊ " << note_line << endl;
	cout << filename << " �ļ��� ����Ϊ " << empty_line << endl;
	*c = code_line;
	*n = note_line;
	*e = empty_line;
}

void	recursion_function(char *parameter, char *filename){							//	�ݹ����
	intptr_t	file;
	_finddata_t fileData;

	file = _findfirst(filename, &fileData);			// ����������ƥ��ĵ�һ���ļ�
	if (file == -1)
	{
		cout << "��ǰĿ¼������" << filename << "ƥ����ļ�" << endl;
		return;
	}

	do
	{
		char *_filename = fileData.name;	//	��ȡ�ļ�����
		int		count_char = 0;				//	�ַ���ͳ��
		int		count_line = 0;				//	����ͳ��
		int		count_word = 0;				//	������ͳ��
		int		count_code_line = 0;
		int		count_note_line = 0;
		int		count_empty_line = 0;

		if (!strcmp(parameter, "-c")){
			count_char = count_of_character(_filename);				//	ִ�� �ַ�ͳ�Ʋ���
			cout << _filename << " �ļ��� �ַ���Ϊ " << count_char << endl;
		}
		else if (!strcmp(parameter, "-l")){
			count_line = count_of_line(_filename);					//	ִ�� ����ͳ�Ʋ���
			cout << _filename << " �ļ��� ����Ϊ " << count_line << endl;
		}
		else if (!strcmp(parameter, "-w")){
			count_word = count_of_word(_filename);					//	ִ�� ����ͳ�Ʋ���
			cout << _filename << " �ļ��� ������Ϊ " << count_word << endl;
		}
		else if (!strcmp(parameter, "-a")){
			count_of_more(_filename, &count_code_line, &count_note_line, &count_empty_line);									//	ִ�� ���ิ�Ӳ���
		}
		
	} while (!_findnext(file, &fileData));		// ����Ŀ¼�е���һ���ļ�
}

void	file_dialog(){


	int		count_char = 0;				//	�ַ���ͳ��
	int		count_line = 0;				//	����ͳ��
	int		count_word = 0;				//	������ͳ��
	int		count_code_line = 0;
	int		count_note_line = 0;
	int		count_empty_line = 0;
	wchar_t buffer[1024];

	_tsetlocale(LC_CTYPE, TEXT(""));					//��wprintf ֧������
	TCHAR szPathName[MAX_PATH*MAX_FILE_FOR_SEL];
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };

	ofn.hwndOwner = GetForegroundWindow();// ��OR�����ļ��Ի���ĸ�����
	ofn.lpstrFilter = TEXT("�ı��ļ�(*.txt)/0*.txt/0C/C++Դ�ļ�(*.cpp;*.c;*.h)/0*.cpp;*.c;*.h/0All Files(*.*)/0*.*/0/0");

	lstrcpy(szPathName, TEXT(""));
	ofn.lpstrFile = szPathName;
	ofn.nMaxFile = sizeof(szPathName);//����û�ѡ���ļ��� ·�����ļ��� ������
	ofn.lpstrTitle = TEXT("ѡ���ļ�");//ѡ���ļ��Ի������
	TCHAR szCurDir[MAX_PATH];
	GetCurrentDirectory(sizeof(szCurDir), szCurDir);
	ofn.lpstrInitialDir = szCurDir;//���öԻ�����ʾ�ĳ�ʼĿ¼
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;//�����Ҫѡ�����ļ� ��������  OFN_ALLOWMULTISELECT��־
	_tprintf(TEXT("select file/n"));

	there:
	BOOL bOk = GetOpenFileName(&ofn);					//���öԻ�����ļ�
	if (bOk)
	{
		count_char = count_of_character(wchar2char(szPathName));			//	ͳ���ַ�����
		count_line = count_of_line(wchar2char(szPathName));					//	ͳ������
		count_word = count_of_word(wchar2char(szPathName));					//	ͳ�Ƶ��ʸ���
		count_of_more(wchar2char(szPathName), &count_code_line, &count_note_line, &count_empty_line);		//	ͳ�Ƹ�������
		swprintf_s(buffer, 1024, L"�ļ��� �ַ��� Ϊ %d\n�ļ��� ���� Ϊ %d\n�ļ��� ������ Ϊ %d\n�ļ��� ������ Ϊ %d\n�ļ��� ע���� Ϊ %d\n�ļ��� �հ��� Ϊ %d"
			, count_char, count_line, count_word, count_code_line, count_note_line, count_empty_line);
		MessageBox(NULL, buffer, L"ͳ��", MB_OK);

		if (IDYES == MessageBox(NULL, L"�Ƿ�����鿴�����ļ���Ϣ?", L"ͳ��", MB_YESNO)){
			goto there;
		}
	}
}
