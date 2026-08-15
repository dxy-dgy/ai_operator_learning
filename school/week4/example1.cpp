#include <iostream>
class Student{
    private:
        int num;
        int score;
    public:
        void setdata(){
            std::cin >> num;
            std::cin >> score;
        }
        void display(){
            std::cout << "num = " << num << std::endl;
            std::cout << "score = " << score << std::endl;
        }
};
Student std1, std2;
int main(){
    std1.setdata();
    std2.setdata();
    std1.display();
    std2.display();
    return 0;
}