#include <iostream>
#include <string>
#include <sstream>

void counting_sort(unsigned long long (*arr)[6], long long max_val, long long size, long long key){
    
    // std::cout<<std::endl<<"arr in func:"<<std::endl;
    // for (long long i = 0; i < size; i++){
    //     std::cout<<arr[i][0]<<" "<<arr[i][1]<<" "<<arr[i][2]<<" "<<arr[i][3]<<std::endl;
    //     std::cout<<"dop: "<<arr[i][4]<<" "<<arr[i][5]<<std::endl;
    // }
    // std::cout<<"max_val: "<<max_val<<std::endl;
    // std::cout<<"size: "<<size<<std::endl;
    // std::cout<<"key: "<<key<<std::endl;
    // std::cout<<std::endl;

    long long list[max_val + 1] = {};
    for (long long i = 0; i < size; i++){
        list[arr[i][key]] += 1;
    }

    for (long long i = 1; i <= max_val; i++){
        list[i] += list[i - 1];
    }

    unsigned long long (*arr2)[6] = new unsigned long long[size][6];
    for (long long i = size - 1; i >= 0; i--){
        arr2[list[arr[i][key]] - 1][0] = arr[i][0];
        arr2[list[arr[i][key]] - 1][1] = arr[i][1]; 
        arr2[list[arr[i][key]] - 1][2] = arr[i][2];
        arr2[list[arr[i][key]] - 1][3] = arr[i][3];
        arr2[list[arr[i][key]] - 1][4] = arr[i][4];
        arr2[list[arr[i][key]] - 1][5] = arr[i][5];
        list[arr[i][key]] -= 1;
    }

    for (long long i = 0; i < size; i++){
        arr[i][0] = arr2[i][0];
        arr[i][1] = arr2[i][1];
        arr[i][2] = arr2[i][2];
        arr[i][3] = arr2[i][3];
        arr[i][4] = arr2[i][4];
        arr[i][5] = arr2[i][5];
    }
    
    delete[] arr2;

}

int main(){
    // std::ios_base::sync_with_stdio(false);
    long long n = 1, size = 0, data1, data2, data3;
    n = 1000000;
    unsigned long long val;
    // long long max_d = 0, max_m = 0, max_y = 0;  
    int max_d = 31, max_m = 12, max_y = 9999;  
    char point;
    std::string input;
    
    unsigned long long (*arr)[6] = new unsigned long long[n][6];
    long long i = 0;

    while(std::getline(std::cin, input)){ // ввод значений

        if (i == n){ // если привысили
            unsigned long long (*arr2)[6] = new unsigned long long[n * 2][6];
            for (long long j = 0; j < n; j++){
                arr2[j][0] = arr[j][0];
                arr2[j][1] = arr[j][1];
                arr2[j][2] = arr[j][2];
                arr2[j][3] = arr[j][3];
                arr2[j][4] = arr[j][4];
                arr2[j][5] = arr[j][5];
            }
            delete[] arr;
            arr = arr2;  
            n *= 2;
        }

        if (input == ""){
            continue;
        }

        // std::cout<<std::endl<<"string:"<<std::endl<<input<<std::endl;
        // std::cout<<"first: "<<input[0]<<std::endl<<std::endl;

        std::istringstream iss(input);
        
        iss >> data1 >> point >> data2 >> point >> data3;
        iss.ignore();
        iss >> val;

        // std::cout<<"data1: "<<data1<<"; "<<"data2: "<<data2<<"; "<<"data3: "<<data3<<"; "<<"val: "<<val<<std::endl<<std::endl;
        arr[i][4] = 0;
        arr[i][5] = 0;

        if (input[0] == '0'){
            arr[i][4] = 1;
            if (input[3] == '0'){
                arr[i][5] = 1;
            }
        }
        else if (input[2] == '.' && input[3] == '0'){
            arr[i][5] = 1;
        }
        else if (input[1] == '.' && input[2] == '0'){
            arr[i][5] = 1;
        }

        arr[i][0] = data1;
        arr[i][1] = data2;
        arr[i][2] = data3;
        arr[i][3] = val;
        // std::cout<<std::endl<<arr[i][0]<<" "<<arr[i][1]<<" "<<arr[i][2]<<" "<<arr[i][3]<<std::endl;
        // std::cout<<"dop: "<<arr[i][4]<<" "<<arr[i][5]<<std::endl;
        i+=1;
        size += 1;
        // if (max_d < data1)
        //     max_d = data1;
        // if (max_m < data2)
        //     max_m = data2;
        // if (max_y < data3)
        //     max_y = data3;   
    }
    // std::cout<<"arr start:"<<std::endl;
    // for (long long i = 0; i < size; i++){
    //     std::cout<<arr[i][0]<<" "<<arr[i][1]<<" "<<arr[i][2]<<" "<<arr[i][3]<<" "<<std::endl;
    // }

    counting_sort(arr, max_d, size, 0);
    counting_sort(arr, max_m, size, 1);
    counting_sort(arr, max_y, size, 2);


    // std::cout<<"arr end:"<<std::endl;
    for (long long i = 0; i < size; i++){
        if (arr[i][4] == 1)
            std::cout<<'0';
        std::cout<<arr[i][0]<<".";
        if (arr[i][5] == 1)
            std::cout<<'0';
        std::cout<<arr[i][1]<<".";
        std::cout<<arr[i][2]<<"\t"<<arr[i][3]<<std::endl;
    }


    delete[] arr;

}

// if (input[0] == '0'){
//             if (input[3] == '0'){
//                 data1 = std::stoi(input.substr(0, 2));
//                 data2 = std::stoi(input.substr(3, 2));
//                 data3 = std::stoi(input.substr(6, 4));
//                 val = std::stoi(input.substr(11));
//                 arr[i][4] = 1; arr[i][5] = 1;
//             }
//             else{
//                 data1 = std::stoi(input.substr(0, 2));
//                 data2 = std::stoi(input.substr(3, 2));
//                 data3 = std::stoi(input.substr(6, 4));
//                 val = std::stoi(input.substr(11));
//                 arr[i][4] = 1; arr[i][5] = 1;
//             }
//         }

// 1.1.1   13207862122685464576
// 1.1.1   4588010303972900864
// 01.02.2008      7670388314707853312
// 01.02.2008      12992997081104908288

// 1.2.2   998010303972900864
// 1.1.2   998010303972900864
// 1.1.3   778010303972900864
// 2.2.2   888010303972900864
// 1.2.2   998010303972900864
// 2.2.12   998010303972900864


// 02.2.2   888010303972900864
// 1.02.2   998010303972900864  
// 24.2.2   998010303972900864
// 1.02.20232   998010303972900864
// 24.2.2   998010303972900864

