#include "app.h"

int main(int argc, char* argv[]) {
    // 인자가 전달되었는지 검사
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [source file] [record file]" << std::endl;
        return 1;
    }
     
    App* app = new App(argv[1], argv[2]);
    
    app->Run();
    
    return 0;
}