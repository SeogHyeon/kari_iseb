#include <iostream>

using namespace std;

int main(){
    cout<<"hello"<<endl;
    FILE* fp_cadu;
    
    fp_cadu = fopen("cadu_img/lrit_img.cadu", "rb");

    
    fclose(fp_cadu);


}