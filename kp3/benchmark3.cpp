#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include <sys/resource.h>
#include <malloc.h>
#include <fstream>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";


struct Node {
    int value;
    int priority;
    int size;
    Node* left;
    Node* right;

    Node(int val, int pr) : value(val), priority(pr), size(1), left(nullptr), right(nullptr) {}
};

void updateSize(Node* node) {
    if (node) {
        int leftSize = node->left ? node->left->size : 0;
        int rightSize = node->right ? node->right->size : 0;
        node->size = 1 + leftSize + rightSize;
    }
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    updateSize(y);
    updateSize(x);
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    updateSize(x);
    updateSize(y);
    return y;
}

Node* rotateRightCopy(Node* y) {
    Node* x = new Node(*y->left);
    y->left = x->right;
    x->right = y;
    updateSize(y);
    updateSize(x);
    return x;
}

Node* rotateLeftCopy(Node* x) {
    Node* y = new Node(*x->right);
    x->right = y->left;
    y->left = x;
    updateSize(x);
    updateSize(y);
    return y;
}

Node* insert(Node* root, int value, int priority) {
    if (!root) {
        return new Node(value, priority);
    }

    Node* newNode = new Node(*root);

    if (value < root->value) {
        newNode->left = insert(root->left, value, priority);
        if (newNode->left && newNode->left->priority > newNode->priority) {
            newNode = rotateRight(newNode);
        }
    } else {
        newNode->right = insert(root->right, value, priority);
        if (newNode->right && newNode->right->priority > newNode->priority) {
            newNode = rotateLeft(newNode);
        }
    }

    updateSize(newNode);
    return newNode;
}

Node* remove(Node* root, int value) {
    if (!root) return nullptr;

    Node* newNode = new Node(*root);

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

int segmentsAbovePoints(int y, Node* root) {
    if (!root) return 0;
    if (root->value > y) {
        int count = root->right ? root->right->size : 0;
        return 1 + count + segmentsAbovePoints(y, root->left);
    } else {
        return segmentsAbovePoints(y, root->right);
    }
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

struct Segment {
    int x;
    int y;
    int isCreate; 
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

void printMemoryUsage() {
    struct mallinfo info = mallinfo();

    std::cout << "Total heap allocated space: " << info.uordblks << " bytes " <<std::endl;
}

int getMemoryUsage() {
    struct mallinfo info = mallinfo();
    return info.uordblks; // Возвращаем количество байтов, выделенных в куче
}

int main() {

    int memoryUsageStart = getMemoryUsage();
    std::chrono::time_point<std::chrono::system_clock> start_algo = std::chrono::system_clock::now();


    int n, m;
    std::vector<Segment> arr_segment; //координата х, координата y, левый или правый х (вставка или удаление)
    std::cin >> n >> m;
    int l, r, h;
    for (int i = 0; i < n; i++) {
        std::cin >> l >> r >> h;
        arr_segment.push_back({l, h, 1}); //create node
        arr_segment.push_back({r, h, 0}); //delete node
    }
    std::sort(arr_segment.begin(), arr_segment.end(), 
    [](const Segment& a, const Segment& b) {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        return a.isCreate > b.isCreate;
    });

    // std::cout<<"------"<<std::endl;
    // for (const auto& row : arr_segment) {
    //     for (int elem : row) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << "\n";
    // }

    // std::mt19937 gen(42); 
    // std::uniform_int_distribution<> dist(1, 10000);

    Node* root = nullptr;
    std::vector<Node*> versions; 
    for (int i = 0; i < arr_segment.size(); i++) {
        if (arr_segment[i].isCreate == 1) {
            root = insert(root, arr_segment[i].y, i); 
            versions.push_back(root); 
        } else {
            root = remove(root, arr_segment[i].y); 
            versions.push_back(root); 
        }
    }
    std::vector<Point> arr_points;
    int x, y;
    for (int i = 0; i < m; i++) {
        std::cin >> x >> y;
        arr_points.push_back({x, y});
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < arr_segment.size(); j++) {
            if (arr_points[i].x <= arr_segment[j].x) {
                if (arr_segment[j].isCreate == 1 && (arr_points[i].x == arr_segment[j].x)) continue;
                if (j == 0){
                    // std::cout<<0<<std::endl;
                }
                else{   
                    int count = segmentsAbovePoints(arr_points[i].y, versions[j - 1]);
                    // std::cout << count << std::endl;
                }
                break;
            }
            else if (j == arr_segment.size() - 1){
                // std::cout<<0<<std::endl;
            }
        }
    }


    std::chrono::time_point<std::chrono::system_clock> end_algo = std::chrono::system_clock::now();
    int memoryUsageEnd = getMemoryUsage();
    uint64_t algo_ts = std::chrono::duration_cast<duration_t>(end_algo - start_algo).count();
    std::cout << "Algorithm time: " << algo_ts << DURATION_PREFIX << std::endl;
    std::cout << "Algorithm use: " << memoryUsageEnd - memoryUsageStart<< " bytes " << std::endl;



    for (int i = 0; i < arr_segment.size(); i += 35) {
        deleteTree(versions[i]);
    }
    // Освобождение памяти
    // deleteTree(root);

    return 0;
}