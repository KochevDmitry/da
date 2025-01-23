//релизация через shared_ptr, занимает много памяти (почему-то), но зато не утекает
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include <sys/resource.h>
#include <malloc.h>
#include <fstream>

struct Node { // Персистентное декартово дерево
    int value;      
    int priority;   
    int size; 
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(int val, int pr) : value(val), priority(pr), size(1), left(nullptr), right(nullptr) {}
};


void updateSize(std::shared_ptr<Node> node) {
    if (node) {
        int leftSize = 0;
        if (node->left) {
            leftSize = node->left->size;
        }

        int rightSize = 0;
        if (node->right) {
            rightSize = node->right->size;
        }
        node->size = 1 + leftSize + rightSize;
    }
}

std::shared_ptr<Node> rotateRight(std::shared_ptr<Node> y) {
    std::shared_ptr<Node> x = y->left;
    // std::shared_ptr<Node> x = std::make_shared<Node>(*y->left);
    y->left = x->right;
    x->right = y;
    updateSize(y);
    updateSize(x);
    return x;
}

std::shared_ptr<Node> rotateLeft(std::shared_ptr<Node> x) {
    std::shared_ptr<Node> y = x->right; //как-будто копируем почему-то
    // std::shared_ptr<Node> y = std::make_shared<Node>(*x->right);
    x->right = y->left;
    y->left = x;
    updateSize(x);
    updateSize(y);
    return y;
}

std::shared_ptr<Node> rotateRightCopy(std::shared_ptr<Node> y) {
    std::shared_ptr<Node> x = std::make_shared<Node>(*y->left);
    y->left = x->right;
    x->right = y;
    updateSize(y);
    updateSize(x);
    return x;
}

std::shared_ptr<Node> rotateLeftCopy(std::shared_ptr<Node> x) {
    std::shared_ptr<Node> y = std::make_shared<Node>(*x->right);
    x->right = y->left;
    y->left = x;
    updateSize(x);
    updateSize(y);
    return y;
}


std::shared_ptr<Node> insert(std::shared_ptr<Node> root, int value, int priority) {
    if (!root) {
        return std::make_shared<Node>(value, priority);
    }
    std::shared_ptr<Node> newRoot = std::make_shared<Node>(*root); //реализация персистентности

    if (value < root->value) {
        newRoot->left = insert(root->left, value, priority);
        if (newRoot->left->priority > newRoot->priority) {
            newRoot = rotateRight(newRoot);
        }
    } else {
        newRoot->right = insert(root->right, value, priority);
        if (newRoot->right->priority > newRoot->priority) {
            newRoot = rotateLeft(newRoot);
        }
    }

    updateSize(newRoot);

    return newRoot;
}

std::shared_ptr<Node> remove(std::shared_ptr<Node> root, int value) {
    if (!root) return nullptr;

    std::shared_ptr<Node> newNode = std::make_shared<Node>(*root); //реализация персистентности

    if (value < root->value) {
        newNode->left = remove(root->left, value);
    } else if (value > root->value) {
        newNode->right = remove(root->right, value);
    } else {
        if (!root->left) {
            return root->right;
        } else if (!root->right) {
            return root->left;
        }

        if (root->left->priority > root->right->priority) {
            newNode = rotateRightCopy(newNode);
            newNode->right = remove(newNode->right, value);
        } else {
            newNode = rotateLeftCopy(newNode);
            newNode->left = remove(newNode->left, value);
        }
    }

    updateSize(newNode);
    return newNode;
}

int segmentsAbovePoints(int y, std::shared_ptr<Node> root) {
    if (!root) return 0;
    if (root->value > y) {
        int rightCount = (root->right) ? root->right->size : 0;
        return 1 + rightCount + segmentsAbovePoints(y, root->left);
    } else {
        return segmentsAbovePoints(y, root->right);
    }
}

// int segmentsAbovePoints(int y, std::shared_ptr<Node> root) {
//     if (!root) {
//         return 0;
//     }

//     int count = 0;
//     if (root->value > y) {
//         count = 1;
//     }
//     count += segmentsAbovePoints(y, root->left);
//     count += segmentsAbovePoints(y, root->right);

//     return count;
// }


void inorder(std::shared_ptr<Node> root) {
    if (root) {
        inorder(root->left);
        std::cout << root->value << "(s=" << root->size << ", p=" << root->priority << ") ";
        inorder(root->right);
    }
}

// bool compareFirstElement(const std::vector<int>& a, const std::vector<int>& b) {
//     return a[0] < b[0];
// }
// bool compareThirdElement(const std::vector<int>& a, const std::vector<int>& b) {
//     return a[2] > b[2];
// }

void printTree(const std::shared_ptr<Node>& root, const std::string& prefix = "", bool isLeft = true) {
    if (root) {
        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");
        std::cout << root->value << "(p=" << root->priority << ", s=" << root->size << ")\n";
        printTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
        printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}

// size_t calculateMemoryUsage(const std::shared_ptr<Node>& node) {
//     if (!node) return 0;

//     size_t nodeSize = sizeof(*node);

//     size_t leftSize = calculateMemoryUsage(node->left);
//     size_t rightSize = calculateMemoryUsage(node->right);

//     return nodeSize + leftSize + rightSize;
// }

void printMemoryUsage() {
    struct mallinfo info = mallinfo();

    std::cout << "Total heap allocated space: " << info.uordblks << " bytes " << sizeof(info) <<std::endl;
}

struct Segment {
    int x; 
    int y;
    bool isCreate;
};

struct Point {
    int x;
    int y;
};
bool compareFirstElement(const Segment& a, const Segment& b) {
    return a.x < b.x;
}
bool compareThirdElement(const Segment& a, const Segment& b) {
    return a.isCreate > b.isCreate;
}

int main(){
    // printMemoryUsage();
    int n, m;   
    std::vector<Segment> arr_segment; //координата х, координата y, левый или правый х (вставка или удаление)
    std::cin>>n>>m;

    // printMemoryUsage();

    int l, r, h;
    for (int i = 0; i < n; i++){
        std::cin>>l>>r>>h;
        arr_segment.push_back({l, h, 1}); //create node
        arr_segment.push_back({r, h, 0}); //delete node
    }

    // printMemoryUsage();

    std::stable_sort(arr_segment.begin(), arr_segment.end(), compareThirdElement);
    std::stable_sort(arr_segment.begin(), arr_segment.end(), compareFirstElement); // сортируем по первому элементу каждого подвектора 

    // std::cout<<"------"<<std::endl;
    // for (const auto& row : arr_segment) {
    //     for (int elem : row) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << "\n";
    // }

    // std::mt19937 gen(42); 
    // std::uniform_int_distribution<> dist(1, 10000);

    // printMemoryUsage();

    std::shared_ptr<Node> root = nullptr;
    std::vector<std::shared_ptr<Node>> versions; 
    // printMemoryUsage();   

    for (int i = 0; i < arr_segment.size(); i++){
        if (arr_segment[i].isCreate == 1){
            // std::cout<<"before insert"<<std::endl;
            // printMemoryUsage();
            root = insert(root, arr_segment[i].y, i); //create node 100001-i

            // std::cout<<"after insert "<<sizeof(*root)<<" "<< sizeof(root)<<std::endl;
            // printMemoryUsage();
            versions.push_back(root);
            // std::cout<<versions[i]->value<<" "<< arr_segment[i][1]<<" "<<i<<std::endl;
        }
        else {
            // std::cout<<"before remove"<<std::endl;
            // printMemoryUsage();
            root = remove(root, arr_segment[i].y); //delete node
            // if (i != arr_segment.size() - 1)
            //     std::cout<<root->value<<" "<< arr_segment[i][1]<<" "<<i<<std::endl;
            // std::cout<<"after remove"<<std::endl;
            // printMemoryUsage();
            versions.push_back(root);
        }
    }
    // std::cout<<"memory of vector: " <<versions.capacity() * sizeof(std::shared_ptr<Node>) + sizeof(std::vector<std::shared_ptr<Node>>)<<" bytes\n";

    // printMemoryUsage();

    std::vector<Point> arr_points; 
    int x, y;
    for (int i = 0; i < m; i ++){
        std::cin>>x>>y;
        arr_points.push_back({x, y});
    }

    // printMemoryUsage();

    // for (size_t i = 0; i < versions.size(); ++i) {
    //     std::cout << "Версия " << i << ": ";
    //     inorder(versions[i]);
    //     std::cout << std::endl;
    // }
                    
    // for (size_t i = 0; i < versions.size(); ++i) {
    //     std::cout<<std::endl<<std::endl;
    //     printTree(versions[i]);
    //     std::cout<<std::endl<<std::endl;
    // }
    // for (size_t i = 0; i < versions.size(); ++i) {
    //     std::cout<<std::endl;
    //     size_t totalMemory = calculateMemoryUsage(versions[i]);
    //     std::cout << "memory used by version: " << totalMemory << " bytes";
    //     std::cout<<std::endl;
    // }

    for (int i = 0; i < m; i ++){
        for (int j = 0; j < arr_segment.size(); j++){
            if (arr_points[i].x <= arr_segment[j].x){
                // std::cout<<j<<" "<<arr_points[i][0]<<" "<< arr_segment[j][0]<<" "<<arr_segment[j][2]<<std::endl;
                if (arr_segment[j].isCreate == 1 && (arr_points[i].x == arr_segment[j].x)) continue;
                // std::cout<<j<<" "<<arr_points[i][0]<<" "<< arr_segment[j][0]<<" "<<arr_segment[j][2]<<std::endl;
                if (j == 0){
                    std::cout<<0<<std::endl;
                }
                else{
                    // std::cout<<std::endl<<std::endl;
                    // printTree(versions[j - 1]);
                    // std::cout<<std::endl<<std::endl;
                    int count = segmentsAbovePoints(arr_points[i].y, versions[j - 1]);//обращение в версию j - 1 дерева
                    std::cout<<count<<std::endl;
                }
                break;
            }
            else if (j == arr_segment.size() - 1){
                std::cout<<0<<std::endl;
            }
        }
    }

    // printMemoryUsage();



}


// test2:
// 4 11
// -5 -1 2
// -2 2 4
// 1 4 2
// 2 8 2
// -7 -2
// -5 -2
// -2 -2
// -1 -2
// 1 -2
// 2 -2
// 3 -2
// 4 -2
// 6 -2
// 8 -2
// 10 -2

// ------------------------------------------------------
// test3:
// 8 19
// -2 2 0
// 1 4 2
// 1 4 2
// 1 4 2
// -5 -1 2
// -2 2 4
// 1 4 2
// 2 8 2
// -7 -2
// -5 -2
// -2 -2
// -1 -2
// 1 -2
// 2 -2
// 2 0
// 2 2
// 2 4
// 2 5
// 3 -2
// 4 -2
// 6 -2
// 8 -2
// 10 -2
// -4 3
// 1 3
// -1 1
// 2 1
// ------------------------------
// test4:
// 8 4
// -2 2 0
// 1 4 2
// 1 4 2
// 1 4 2
// -5 -1 2
// -2 2 4
// 1 4 2
// 2 8 2
// 0 -1
// 0 0
// 0 2
// 0 4
// ------------------------
// test5:
// 7 5
// -5 -1 2
// 0 5 2
// 1 3 1
// 1 8 5
// -2 0 3
// -1 6 -1
// 3 10 2
// -1 1
// 1 4
// 3 0
// 5 -2
// 7 0
// ------------------------
// test6:
// 9 5
// -3 0 8
// 0 4 3
// 0 4 7
// -3 0 5
// -3 0 10
// -3 0 2
// 0 4 9
// 0 4 5
// 0 4 6
// 0 -2
// -6 -3
// -3 -3
// 0 4
// 7 6

