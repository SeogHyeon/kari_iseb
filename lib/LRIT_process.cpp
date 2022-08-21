#include <iostream>
#include <fstream>

using namespace std;


unsigned char* caduRead2(std::string filepath, int& _size) {
	ifstream readFile(filepath, ifstream::binary);
	if (readFile.is_open()) {
		readFile.seekg(0, std::ios::end);
		long fileSize1 = readFile.tellg();
		long fileSize = fileSize1;
		_size = fileSize;

		readFile.seekg(0, std::ios::beg);

		unsigned char* buffer = new unsigned char[fileSize];
		readFile.read((char*)buffer, fileSize1);

		return buffer;
	}
    return NULL;
}


int main(){
    string file_path="cadu_img/";
    string file_name="lrit_img.cadu";

    int size;
	unsigned char* buffer = caduRead2(file_path+file_name,size);

    cout<<(int)buffer[0] <<endl;
    cout<<(int)buffer[1]<<endl;
    cout<<(int)buffer[2]<<endl;
    cout<<(int)buffer[3]<<endl;
    cout<<(int)buffer[4]<<endl;
    cout<<(int)buffer[5]<<endl;
    cout<<(int)buffer[6]<<endl;
    cout<<(int)buffer[7]<<endl;









}