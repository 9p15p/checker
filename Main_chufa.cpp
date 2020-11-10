#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<set>
#include<map>
#include<stack>
using namespace std;

char S;  //��ʼ��
string VT,VN; //VT��ʾ�ս����VN��ʾ���ս��
set<string> Pro[300];  //����ʽ
map<string,set<char>> FIRSTS,FIRST;
map<char,set<char>> FIRSTA,FOLLOW; //first����follow��
string M[300][300];
stack<char> Stack;

void createFIRST();  //�������ս�����ս����first����
void createFIRST(string s);  //�������Ŵ�����first����
void createFOLLOW(); //�������ս����follow����
void createMATRIX();  //����Ԥ�������
void Parser(string s);    //�﷨����
set<char> Union(set<char> A,set<char> B,bool &Change,bool lim);

void Init() {
	S = 'E';
	VN = "ESTQF";
	VT = "i+-*/()";
	Pro['E'].insert("TS");
	Pro['S'].insert("+TS");
	Pro['S'].insert("-TS");
	Pro['S'].insert("$");
	Pro['T'].insert("FQ");
	Pro['Q'].insert("*FQ");
	Pro['Q'].insert("/FQ");
	Pro['Q'].insert("$");
	Pro['F'].insert("(E)");
	Pro['F'].insert("i");
}

int main()
{
	char ch,t;
	string s;
	Init();
	printf("���������(��#����)\n");
	cin >> s;
	Parser(s);
	system("pause");
	return 0;
}

void Parser(string s)
{
	int i;
	createFIRST();  //���������ս���ͷ��ս����first����
	for(i = 0; i < VN.size(); i++) {
		for(set<string>::iterator it = Pro[VN[i]].begin(); it != Pro[VN[i]].end(); it++) {
			createFIRST(*it);  //�������з��Ŵ���first����
		}
	}
	createFOLLOW(); //������ս����follow����
	createMATRIX(); //����Ԥ�������
	Stack.push('#'); //���綨��'#'ѹ��ջ
	Stack.push(S);  //����ʼ��ѹ��ջ
	i = 0; 
	string inStack = "#",tmp;
	inStack += S;
	bool flag = true;
	int step = 1;
	printf("��������\tSTACKջ\t\tʣ��������Ŵ�\t\t����/ʹ�õĲ���ʽ\n");
	while(flag) {
		if(s[i] == '#') {
			int a = 1;
		}
		char ch = Stack.top();
		tmp = s.substr(i,s.npos);
		cout << step++ << "\t\t" << inStack << "\t\t" << tmp << "\t\t\t";
		if(VN.find(ch) != VN.npos) { //ch���ڷ��ս��
			tmp = M[ch][s[i]]; //M[X,a] == X->Y1Y2...Yk
			if(tmp.size() == 0) {
				cout << "����" << endl;
				if(s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-' || s[i] == '#') {
					cout << "��" << s[i] << "����" << "ȱ��i" << endl;
				} else if(s[i] == '(') {
					cout << "��(����ȱ�������" << endl;
				} else if(s[i] != 'i') {
					cout << "δ�����ʶ��" << s[i] << endl;
				} else if(s[i] == 'i') {
					cout << "��i����ȱ�������" << endl;
				} 
				break;
			}
			cout << "�Ƶ�/" << ch << "->" << tmp << endl;
			Stack.pop();    //����X,��YK,Yk-1��...,Y1�Դ�ѹ��ջ��Y1��ջ��
			inStack.erase(inStack.size()-1);
			if(tmp[0] == '$') continue;
			for(int j = tmp.size() - 1; j >= 0; j--) {
				Stack.push(tmp[j]); 
				inStack += tmp[j];
			}
		} else if(ch == s[i]) { //ch�����ս��
			if(ch == '#') {
				cout << "�����ɹ�" << endl;
				flag = false;
				break;
			}
			else {
				cout << "ƥ��" << endl;
				Stack.pop();
				inStack.erase(inStack.size()-1);
				i++;
			}
		} else {
			cout << "����" << endl;
			if(s[i] == ')') {
				cout << "ȱ��" << s[i] << endl;
			} else {
				cout << "ȱ��" << ch << endl;
			}
			break;
		}
	}
	if(flag == false)
		printf("�﷨�����ɹ�\n");
	else printf("�﷨����\n");
}

void createFIRST()
{
	bool Change;
	string tmp;
	FIRSTA.clear(); 
	FOLLOW.clear();
	for(int i = 0; i < VT.size(); i++) {
		tmp = VT[i];
		FIRST[tmp].insert(VT[i]);
	}
	tmp = "$";
	FIRST[tmp].insert('$');
	do {
		Change = false;
		for(int i = 0; i < VN.size(); i++) {
			string P; //���ս��P 
			P = VN[i];
			for(set<string>::iterator it = Pro[VN[i]].begin(); it != Pro[VN[i]].end(); it++) {
				string s = *it;
				int k = 0;
				bool cont = true;
				while(cont && k < s.size()) {
					tmp = s[k];
					FIRST[P] = Union(FIRST[P], FIRST[tmp], Change, true);
					if(FIRST[tmp].find('$') == FIRST[tmp].end()) cont = false;
					k++;
				}
				if(cont) FIRST[P].insert('$');
			}
		}
	}while(Change == true);
}

void createFIRST(string s)
{
	if(s.size() == 1 && VT.find(s[0]) != VT.npos) return;
	int k = 1;
	string tmp;
	bool Change;
	for(int i = 0; i < s.size(); i++) {
		tmp = s[i];
		FIRST[s] = Union(FIRST[s],FIRST[tmp],Change,true);
		if(FIRST[tmp].find('$') != FIRST[tmp].end()) k++;
		else break;
	}
	if(k == s.size()) FIRST[s].insert('$');
}

void createFOLLOW()
{
	bool Change;
	FOLLOW[S].insert('#');
	do {
		Change = false;
		for(int i = 0; i < VN.size(); i++) {
			for(set<string>::iterator it = Pro[VN[i]].begin(); it != Pro[VN[i]].end(); it++) {
				string s = *it;
				for(int j = 0; j < s.size(); j++) {
					//if Q��VN
					// FOLLOW(Q) = FOLLOW(Q)��FIRST(��)-{��}
					//if �š� FIRST(��)
					// FOLLOW(Q) = FOLLOW(Q) �� FOLLOW(P) 
					if(VN.find(s[j]) != VN.npos && j < s.size() - 1) {  //���ս��
						string tmp = s.substr(j+1,VN.npos);
						FOLLOW[s[j]] = Union(FOLLOW[s[j]],FIRST[tmp],Change,true);
						if(FIRST[tmp].find('$') != FIRST[tmp].end()) 
							FOLLOW[s[j]] = Union(FOLLOW[s[j]],FOLLOW[VN[i]],Change,false);
					}
					else if(VN.find(s[j]) != string::npos && j == s.size() - 1) {
						FOLLOW[s[j]] = Union(FOLLOW[s[j]],FOLLOW[VN[i]],Change,false);
					}
				}
			}
		}
	}while(Change == true);
}

set<char> Union(set<char> A,set<char> B,bool &Change,bool lim)
{
	if(B.size() == 0) return A;
	for(set<char>::iterator it = B.begin(); it != B.end(); it++) {

		if(*it == '$' && lim) continue;   //��$�����
		if(A.find(*it) == A.end()) {
			A.insert(*it);
			Change = true;
		}
	}
	return A;
}

void createMATRIX()
{
	for(int i = 0; i < VN.size(); i++) {
		for(set<string>::iterator it = Pro[VN[i]].begin(); it != Pro[VN[i]].end(); it++) {
			string s = *it;  //�õ�����ʽ P -> ��

			for(int j = 0; j < VT.size(); j++) {
				if(FIRST[s].find(VT[j]) != FIRST[s].end()) {
					M[VN[i]][VT[j]] = s;   //ÿ���ս��a��FIRST(��),��P->������M[P,a]	
				}
			} //end for

			if(FIRST[s].find('$') != FIRST[s].end()) {  //�š�FRIST(��)
				for(set<char>::iterator iter = FOLLOW[VN[i]].begin(); iter != FOLLOW[VN[i]].end(); iter++) {
					M[VN[i]][*iter] = s;   //��ÿ��b��FOLLOW(P),��P->������M[P,b]
				}  //end for
			}  //end if
		} //end for
	}
	printf("\n\n\t\t\tԤ�������\n\t");
	for(int i = 0; i < VT.size(); i++)
		printf("%c\t",VT[i]);
	printf("#\t\n");
	for(int i = 0; i < VN.size(); i++) {
		printf("%c\t",VN[i]);
		for(int j = 0; j < VT.size(); j++) {
			cout << M[VN[i]][VT[j]] << '\t';
		}
		cout << M[VN[i]]['#'] << endl;
	}
	printf("\n\n");
}