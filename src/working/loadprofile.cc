#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "./threads/command_caller.h"

//using namespace std;


/*void loadp(std::string fullFileName, std::string password, std::string& feedBack){

	const int N = 300;
	//const std::string password = "123";

	std::string fileName;	//name without path
	fileName = fullFileName.substr(fullFileName.find_last_of("/") + 1);

	std::string scmd = "echo " + password + " | sudo -S mv " + fullFileName + " /etc/apparmor.d";
	system(scmd.c_str());

	char line[N];
	FILE *fp;

	scmd = "echo " + password + " | sudo -S apparmor_parser -r /etc/apparmor.d/" + fileName + " 2>&1";
	fp = popen(scmd.c_str(), "r");

	//error message
	std::string fb = "";
	std::string sl;
	while(fgets(line, sizeof(line)-1, fp) !=NULL){
		sl = line;
		fb += sl;
	}
	feedBack = fb;

	pclose(fp);

}*/

int main(){

	std::string FEEDBACK;

	CommandCaller::load_profile("/home/fer/Desktop/appNov15/AppAnvil-main/src/usr.bin.certspotter", "123", FEEDBACK);
	//loadp("/home/fer/Desktop/appNov1/usr.bin.evolution", "123", FEEDBACK);
	//std::cout << "ERR: \n" + FEEDBACK + "++++++\n";

	//std::string rt = CommandCaller::get_status_str();
	//std::cout << "ERR: \n" + rt + "++++++\n";

	return 0;
}