#include "iostream"
#include "singleton.hpp"
#include <cassert>
using namespace std;

int main(){
    FILE *file;
    file = fopen("singleton_text.txt", "ab+");
    if (file == NULL){
        perror("Error creating/reading/writing the file!");
    }
    singleton<FILE*> *single_1 = singleton<FILE*>::Instance();
    singleton<FILE*> *single_2 = singleton<FILE*>::Instance();
    cout << "First Singleton: " << single_1 << ", " << "Second Singleton: " << single_2 << endl;
    assert(single_1 == single_2);
    single_1->Destroy();
}