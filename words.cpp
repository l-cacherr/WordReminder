#include <bits/stdc++.h>
#include <Windows.h>

using namespace std;

typedef long long LL;

const int N = 1e3 + 5;

fstream fword,fsave;
struct WordStruct{
	string word,meaning,chmean;
}dict[N];
int word_tot; 
int imode;
int plmode;
int permuc[N],pidxc;
int permug[N],pidxg;//[0/1][idx]
int errtotc,errtotg;
vector<int> errlistc,errlistg;
set<int> errsetc,errsetg;

bool check_num(string str){
	for(int i = 0;i < str.size();i++)
		if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '.'))
			return false;
	return true;
}

string str_filter(string str){
	string res = "";
	for(int i = 0;i < str.size();i++){
		if(str[i] == '\n')
			continue;
		res += str[i];
	}
	return res;
}

int check_control(string str){
	if(str.size() >= 5){
		if((str[0] == '[' && str[1] == '[' && str[3] == ']' && str[4] == ']'))
		{
			if(str[2] == 'A')
				return 1;
			else if(str[2] == 'B')
				return 2;
			return 3;
		}
		return 0;
	}
	return 0;
}

string erase_pron(string str){
	string res = "";
	bool flag = false;
	for(int i = 0;i < str.size();i++){
		if(str[i] == '/')
			flag ^= 1;
		if(flag)
			continue;
		if(str[i] != '/')
			res += str[i];
	}
	return res;
}

void insert_dict_1(string str,int idx){
	dict[idx].word = "";
	dict[idx].meaning = "";
	string pa = "",pb = "";
	int palidx = 0,paridx = str.size() - 1;//meaning of idx is "the position of the first number in a direction"
	while(palidx < str.size() && !(str[palidx] >= '0' && str[palidx] <= '9')){
		palidx++;
	}
	while(paridx >= 0 && !(str[paridx] >= '0' && str[paridx] <= '9')){
		paridx--;
	}
//	cout << "(" << palidx << ", " << paridx << ")\n"; 
	for(int i = paridx+1;i < str.size();i++)
		pb += str[i];
	for(int i = palidx;i <= paridx;i++){
		if(!(str[i] >= '0' && str[i] <= '9'))
			pa += str[i];
	}
	pa = erase_pron(pa);
	string tmp = pa;
	pa = "";
	bool check_dot = false;
	for(int i = 0;i < tmp.size();i++)
		if(str[i] == '.'){
			check_dot = true;
			break;
		}
	if(check_dot){
		bool stfl = false;
		bool flag = true;
		for(int i = tmp.size()-1;i >= 0;i--){
			if(tmp[i] != ' ' && stfl == false)
				stfl = true;
			if(!stfl)
				continue;
			if(tmp[i] == ' ')
				flag = false;
			if(flag)
				pb = tmp[i] + pb;
			else
				pa = tmp[i] + pa;
		}
	} else {
		pa = tmp;
	}
	dict[idx].word = pa;
	dict[idx].meaning = pb;
}

void insert_dict_2(string str,int idx){	
	dict[idx].word = "";
	dict[idx].meaning = "";
	string pa = "", pb = "";
	bool ckslash = false;
	string tqstr = str;
	str = "";
	for(int i = 1;i < tqstr.size();i++){
		if(tqstr[i] == '.' && tqstr[i-1] >= '0' && tqstr[i-1] <= '9'){
			continue;
		}
		str += tqstr[i];
	}
	for(int i = 0;i < str.size();i++){
		if(str[i] == '/'){
			ckslash = true;
			break;
		}
	}
	if(ckslash){
		int cntslash = 0;
		for(int i = 0;i < str.size();i++){
			if(str[i] == '/'){
				cntslash++;
				continue;
			}
			if(cntslash == 0){
				pa += str[i];
			} else if(cntslash >= 2){
				pb += str[i];
			}
		}
		string tmp = pb;
		pb = "";
		bool stfl = false;
		bool flag = true;
		for(int i = 0;i < tmp.size();i++){
			if(tmp[i] != ' ' && stfl == false)
				stfl = true;
			if(!stfl)
				continue;
			if(tmp[i] == ' ')
				flag = false;
			if(flag)
				pa = pa + tmp[i];
			else
				pb = pb + tmp[i];
		}
		tmp = pa + pb;
		pa = "";pb = "";
		int dotidx = 0;
		while(dotidx < tmp.size() && (tmp[dotidx] != '.')){
			dotidx++;
		}
		if(dotidx < tmp.size()){
			int fridx = dotidx;
			while(fridx >= 0 && (tmp[fridx] != ' ')){
				fridx--;
			}
			for(int i = 0;i <= fridx;i++){
				pa += tmp[i];
			}
			for(int i = fridx+1;i < tmp.size();i++){
				pb += tmp[i];
			}
		}
	} else {
		int dotidx = 0;
		while(dotidx < str.size() && (str[dotidx] != '.')){
			dotidx++;
		}
		if(dotidx < str.size()){
			int fridx = dotidx;
			while(fridx >= 0 && (str[fridx] != ' ')){
				fridx--;
			}
			for(int i = 0;i <= fridx;i++){
				pa += str[i];
			}
			for(int i = fridx+1;i < str.size();i++){
				pb += str[i];
			}
		} else {
			int chidx = 0;
			while(chidx < str.size() && (str[chidx] > 0)){
				chidx++;
			}
			for(int i = 0;i < chidx;i++){
				pa += str[i];
			}
			for(int i = chidx;i < str.size();i++){
				pb += str[i];
			}
		}
	}
	string tmp = pa;
	pa = "";
	bool stfl = false;
	bool stnum = false;
	for(int i = 0;i < tmp.size();i++){
		if(str[i] != ' ' && stfl == false)
			stfl = true;
		if(!stfl)
			continue;
		if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '.') && stnum == false)
			stnum = true;
		if(!stnum)
			continue;
		pa += tmp[i];
	}
	dict[idx].word = pa;
	dict[idx].meaning = pb;
}

void init(){
	srand(time(0));
	fword.open("original_text_ANSI.txt",ios::in);
	string str = "",line = "";
	bool flag = 0;
	int mode = 1,tmpmode = 1;
	word_tot = 0;
	while(fword>>str){
		tmpmode = check_control(str);
		if(tmpmode != 0){
			mode =  tmpmode;
			if(check_control(line) == 0 && line != ""){
				word_tot++;
				insert_dict_1(line,word_tot);
			}
//			cout << line << "\n";
			if(check_control(line) == 0 && line != ""){
//				cout << word_tot << "[" << ((word_tot-1)%40+1) << "] ";
//				cout << "word: " << dict[word_tot].word << "\t\tmean: " << dict[word_tot].meaning << "\n\n";
			}	
			line = "";
		}
//		if(str == "[[B]]")
//			cout << "-----" << tmpmode << "-----\n";
		if(mode == 1){
			if(check_num(str)){
				flag ^= 1;
				if(flag){
					if(check_control(line) == 0 && line != ""){
						word_tot++;
						insert_dict_1(line,word_tot);
					}
//					cout << line << "\n";
					if(check_control(line) == 0 && line != ""){
//						cout << word_tot << "[" << ((word_tot-1)%40+1) << "] ";
//						cout << "word: " << dict[word_tot].word << "\t\tmean: " << dict[word_tot].meaning << "\n\n";
					}
					line = "";
				}
			}
			if(check_control(line) == 0)
				line += str_filter(str) + " ";
		}
		else if(mode == 2){
			if(check_num(str)){
				if(check_control(line) == 0 && line != ""){
					word_tot++;
					insert_dict_2(line,word_tot);
				}
//				cout << line << "\n";
				if(check_control(line) == 0 && line != ""){
//					cout << word_tot << "[" << ((word_tot-1)%40+1) << "] ";
//					cout << "word: " << dict[word_tot].word << "\t\tmean: " << dict[word_tot].meaning << "\n\n";
				}	
				line = "";
			}
			if(check_control(line) == 0)
				line += str_filter(str) + " ";
		}
	}
	if(line != ""){
		if(check_control(line) == 0 && line != ""){
			word_tot++;
			insert_dict_2(line,word_tot);
		}
//				cout << line << "\n";
		if(check_control(line) == 0 && line != ""){
//			cout << word_tot << "[" << ((word_tot-1)%40+1) << "] ";
//			cout << "word: " << dict[word_tot].word << "\t\tmean: " << dict[word_tot].meaning << "\n\n";
		}	
	}
//	cout << "tot: " << word_tot << " words.";
	fword.close();
	
	fsave.open("config.txt",ios::in);
	int tqtot = 0;
	for(int i = 1;i <= word_tot;i++){
		fsave >> permuc[i];
	}
	fsave >> pidxc;
	for(int i = 1;i <= word_tot;i++){
		fsave >> permug[i];
	}
	fsave >> pidxg;
	fsave >> imode;
	fsave >> plmode;
	fsave >> errtotc;
	for(int i = 0;i < errtotc;i++){
		errlistc.push_back(0);
	}
	for(int i = 0;i < errtotc;i++){
		fsave >> errlistc[i];
		errsetc.insert(errlistc[i]);
	}
	fsave >> errtotg;
	for(int i = 0;i < errtotg;i++){
		errlistg.push_back(0);
	}
	for(int i = 0;i < errtotg;i++){
		fsave >> errlistg[i];
		errsetg.insert(errlistg[i]);
	}
	if(pidxc == word_tot){
		for(int i = 1;i <= word_tot;i++)
			permuc[i] = i;
		random_shuffle(permuc+1,permuc+1+word_tot);
	}
	if(pidxg == word_tot){
		for(int i = 1;i <= word_tot;i++)
			permug[i] = i;
		random_shuffle(permug+1,permug+1+word_tot);
	}
	fsave.close();
}

string ch_filter(string str){
	string tmp = str,res = "";
	int dotidx = 0;
	while(dotidx < str.size() && (str[dotidx] != '.')){
		dotidx++;
	}
	if(dotidx < str.size()){
		int fridx = dotidx;
		while(fridx >= 0 && (str[fridx] != ' '))
			fridx--;
		for(int i = fridx+1;i <= dotidx;i++){
			res += str[i];
		}
		res += " ";
	}
	for(int i = 0;i < tmp.size();i++){
		if(tmp[i] > 0)
			tmp[i] = ' ';
	}
	for(int i = 0;i < tmp.size();i++){
		if(tmp[i] == ' '){
			if(i == 0 || (i > 0 && tmp[i-1] == ' '))
				continue;
		}
		res += tmp[i];
	}
	return res;
}

string space_filter(string str){
	string tmp = "", res = "";
	bool spc = true;
	for(int i = 0;i < str.size();i++){
		if(str[i] != ' ' && spc == true)
			spc = false;
		if(spc)
			continue;
		tmp += str[i];
	}
	spc = true;
	for(int i = tmp.size()-1;i >= 0;i--){
		if(tmp[i] != ' ' && spc == true)
			spc = false;
		if(spc)
			continue;
		res = tmp[i] + res;
	}
	return res;
}

void after_proc(){
	for(int i = 1;i <= word_tot;i++){
		dict[i].chmean = ch_filter(dict[i].meaning);
		dict[i].word = space_filter(dict[i].word);
	}
}

void print_dict(){
	for(int i = 1;i <= word_tot;i++){
		cout << "word: " << dict[i].word ;
		cout << "\t\tch: " << dict[i].chmean ;
		cout << "\t\tmean: " << dict[i].meaning ;
		cout << "\n\n";
	}
}

const string intrd = "options: \n\t\"-e\" to exit;\
\n\t\"-g\" to \"Q:ch A:eng\";\
\n\t\"-c\" to \"Q:eng A:ch\" \
\n\t\"-ge\" to \"Q:ch A:eng Error Book\";\
\n\t\"-ce\" to \"Q:eng A:ch Error Book\" \
\n\t\"-p\" to change meaning display mode \
\n\t\"-s\" to clear all config and restart \
\n";

void file_write(){
	fsave.open("config.txt",ios::out);
	for(int i = 1;i <= word_tot;i++){
		fsave << permuc[i] << " ";
	}
	fsave << pidxc << "\n";
	for(int i = 1;i <= word_tot;i++){
		fsave << permug[i] << " ";
	}
	fsave << pidxg << "\n";
	fsave << imode << "\n";
	fsave << plmode << "\n";
	fsave << errtotc << "\n";
	for(int i = 0;i < errtotc;i++){
		fsave << errlistc[i] << " ";
	}
	fsave << errtotg << "\n";
	for(int i = 0;i < errtotg;i++){
		fsave << errlistg[i] << " ";
	}
	fsave.close();
}

void print_errbook(){
	cout << "errc_tot:" << errtotc << " " << errsetc.size() << "  ";
	for(int i = 0;i < errtotc;i++){
		cout << errlistc[i] << " ";
	}
	cout << "\n";
	cout << "errc_tot:" << errtotg << " " << errsetg.size() << "  ";
	for(int i = 0;i < errtotg;i++){
		cout << errlistg[i] << " ";
	}
	cout << "\n";
}

void interact(){
	cout << "Loading has finished.\n";
	cout.flush();
	//Sleep(800);
//	system("pause");
	string input;
	while(true){
		system("cls");
		cout << intrd << "\n";
		
		if(imode == 0){ // c
			cout << "mode:c "<< "  ";
			cout << (pidxc%word_tot) << "/" << word_tot << " ";
			if(plmode == 0)
				cout << dict[permuc[pidxc%word_tot+1]].chmean << "\n";
			else
				cout << dict[permuc[pidxc%word_tot+1]].meaning << "\n";
		} else if(imode == 1){
			cout << "mode:g "<< "  ";
			cout << (pidxg%word_tot) << "/" << word_tot << " ";
			cout << dict[permug[pidxg%word_tot+1]].word << "\n";
		} else if(imode == 2){
			cout << "mode:ce "<< "  ";
			cout << errlistc.size() << "  ";
			if(errlistc.size() > 0){
				if(plmode == 0)
					cout << dict[errlistc[0]].chmean << " ";
				else
					cout << dict[errlistc[0]].meaning << " ";
			} else {
				cout << "[empty]";
			}
			cout << "\n";
		} else if(imode == 3){
			cout << "mode:ge "<< "  ";
			cout << errlistg.size() << "  ";
			if(errlistg.size() > 0){
				cout << dict[errlistg[0]].word << " ";
			} else {
				cout << "[empty]";
			}
			cout << "\n";
		}
		
		cout << "Host> ";
		cout.flush();
		getline(cin,input);
		if(input == "-e"){
			file_write();
			break;
		}
		if(input == "-c"){
			imode = 0;
			continue;
		}
		if(input == "-g"){
			imode = 1;
			continue;
		}
		if(input == "-ce"){
			imode = 2;
			continue;
		}
		if(input == "-ge"){
			imode = 3;
			continue;
		}
		if(input == "-p"){
			plmode ^= 1;
			continue;
		}
		if(input == "-s"){
			for(int i = 1;i <= word_tot;i++)
				permuc[i] = i;
			random_shuffle(permuc+1,permuc+1+word_tot);
			for(int i = 1;i <= word_tot;i++)
				permug[i] = i;
			random_shuffle(permug+1,permug+1+word_tot);
			imode = 0;
			plmode = 0;
			errsetc.clear();
			vector<int>().swap(errlistc);
			errtotc = 0;
			errsetg.clear();
			vector<int>().swap(errlistg);
			errtotg = 0;
			pidxc = word_tot;
			pidxg = word_tot;
			
			file_write();
			continue;
		}
		
		cout << "Answer> ";
		string answer;
		int dtidx = -1;
		if(imode == 0){ // c
			dtidx = permuc[pidxc%word_tot+1];
			answer = dict[dtidx].word;
			cout << dict[dtidx].word << "\n";
			pidxc++;
			if(pidxc == word_tot){
				for(int i = 1;i <= word_tot;i++)
					permuc[i] = i;
				random_shuffle(permuc+1,permuc+1+word_tot);
			}
		} else if(imode == 1){
			dtidx = permug[pidxg%word_tot+1];
			if(plmode == 0)
				cout << dict[dtidx].chmean << "\n";
			else
				cout << dict[dtidx].meaning << "\n";
			pidxg++;
			if(pidxg == word_tot){
				for(int i = 1;i <= word_tot;i++)
					permug[i] = i;
				random_shuffle(permug+1,permug+1+word_tot);
			}
		} else if(imode == 2){
			if(errlistc.size() > 0){
				dtidx = errlistc[0];
				answer = dict[dtidx].word;
				cout << dict[dtidx].word << "\n";
				errsetg.erase(dtidx);
				errlistc.erase(errlistc.begin());
				errtotc--;
			} else {
				cout << "[empty]\n";
			}
		} else if(imode == 3){
			if(errlistg.size() > 0){
				dtidx = errlistg[0];
				if(plmode == 0)
					cout << dict[dtidx].chmean << "\n";
				else
					cout << dict[dtidx].meaning << "\n";
				errsetg.erase(dtidx);
				errlistg.erase(errlistg.begin());
				errtotg--;
			} else {
				cout << "[empty]\n";
			}
		}
		
		if(imode == 0 || (dtidx>= 0 && imode == 2)){
			bool tck = false;
			if(input == answer)
				tck = true;
			cout << "Check> " << (tck? "AC.":"WA.") << "\n";
		}
		
		cout << "\n";
		cout << "-------------------------------------------------------------------------------\n\n";
		cout << "Option> input \"y\" to add it into Error Book or \"Enter\" conitnue.\n";
		cout << "Host> ";
		cout.flush();
		getline(cin,input);
		if(input == "y"){
			if(dtidx >= 0){
				if(imode == 0 || imode == 2){
					if(errsetc.count(dtidx) == 0){
						errsetc.insert(dtidx);
						errlistc.push_back(dtidx);
						errtotc++;
					}
				} else if(imode == 1 || imode == 3){
					if(errsetg.count(dtidx) == 0){
						errsetg.insert(dtidx);
						errlistg.push_back(dtidx);
						errtotg++;
					}
				}	
			}
		}
		file_write();
		
	}
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	init();
	after_proc();
	print_dict();
	print_errbook();
	interact();
	return 0;
}
