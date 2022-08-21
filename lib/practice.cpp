#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main(void){
    ifstream readFile;
    readFile.open("test.txt");//파일 열기     
    if (readFile.is_open())
    {
        while (!readFile.eof())
        {//1. istream의 getline.           
        
        string str;
        getline(readFile,str);
        cout<<str<<endl;
        //지금은 읽은 문자열 바로 출력.        
        }
        readFile.close();//파일 닫아줍니다.    
    }
    return 0;
}
        

