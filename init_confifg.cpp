#include <bits/stdc++.h>

using namespace std;

const int N = 1e3 + 5;

int word_tot = 360;

int imode,plmode;
int permuc[N],pidxc;//finish idx
int permug[N],pidxg;
int errtotc,errtotg;
int errlistc[N],errlistg[N];
set<int> errsetc,errsetg;

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	for(int i = 1;i <= word_tot;i++)
		permuc[i] = i;
	random_shuffle(permuc+1,permuc+1+word_tot);
	for(int i = 1;i <= word_tot;i++)
		permug[i] = i;
	random_shuffle(permug+1,permug+1+word_tot);
	imode = 0;
	plmode = 0;
	errtotc = 0;
	errtotg = 0;
	pidxc = word_tot;
	pidxg = word_tot;
	fstream fsave;
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
	fsave << errtotg << "\n";
	fsave.close();
	return 0;
}
