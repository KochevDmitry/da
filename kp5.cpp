//можно сделать с умными указателями, но почему то в поворотах они делают копию, что кратно увеличивает память
//но зато она не будет утекать
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include <sys/resource.h>
#include <malloc.h>
#include <fstream>


struct Point {
    int x;
    int y;
    Point* prev;
    Point* next; 
    int number_polygon;
};

struct Node {
    int priority;
    Point* point_left;
    Point* point_right;
    Node* left;
    Node* right;

    Node(int pr, Point* ptr_left, Point* ptr_right) : priority(pr), point_left(ptr_left), point_right(ptr_right), left(nullptr), right(nullptr) {}
};


Node* rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node* rotateRightCopy(Node* y) {
    Node* x = new Node(*y->left);
    y->left = x->right;
    x->right = y;
    return x;
}

Node* rotateLeftCopy(Node* x) {
    Node* y = new Node(*x->right);
    x->right = y->left;
    y->left = x;
    return y;
}

Node* insert(Node* root, Point* point_left, Point* point_right, int priority) {
    if (!root) {
        return new Node(priority, point_left, point_right);
    }

    Node* newNode = new Node(*root); //реализация персистентности

    float k = (root->point_left->y - root->point_right->y) / (root->point_left->x - root->point_right->x);
    float b = root->point_left->y - k * root->point_left->x;
    float left_line_y = k * point_left->x + b;
    // std::cout<<"point_left->x: "<<point_left->x<<"; point_left->y: "<<point_left->y<<"; point_right->x: "<<point_right->x<<"; point_right->y: "<<point_right->y<<std::endl;  
    // std::cout<<"root->point_left->y: "<<root->point_left->y<<"; left_line_y: "<<left_line_y<<"; root->point_right->y "<<root->point_right->y<<std::endl;
    left_line_y = root->point_left->y;

    if (point_left->y < left_line_y || ((point_left->y == left_line_y)&&(point_right->y < root->point_right->y))) {
        // std::cout<<"left"<<std::endl;
        newNode->left = insert(root->left, point_left, point_right, priority);
        if (newNode->left && newNode->left->priority > newNode->priority) {
            newNode = rotateRight(newNode);
        }
    } else {
        // std::cout<<"right"<<std::endl;
        newNode->right = insert(root->right, point_left, point_right, priority);
        if (newNode->right && newNode->right->priority > newNode->priority) {
            newNode = rotateLeft(newNode);
        }
    }

    return newNode;
}

Node* remove(Node* root, Point* point_left, Point* point_right) {
    if (!root) return nullptr;

    Node* newNode = new Node(*root); //реализация персистентности

    float k = (root->point_left->y - root->point_right->y) / (root->point_left->x - root->point_right->x);
    float b = root->point_left->y - k * root->point_left->x;
    float left_line_y = k * point_left->x + b;
    left_line_y = root->point_left->y;

    if (point_left->y < left_line_y || ((point_left->y == left_line_y)&&(point_right->y < root->point_right->y))) {
        newNode->left = remove(root->left, point_left, point_right);
    } else if (point_left->y > left_line_y || ((point_left->y == left_line_y)&&(point_right->y > root->point_right->y))) {
        newNode->right = remove(root->right, point_left, point_right);
    } else {
        if (!root->left) {
            return root->right;
        } else if (!root->right) {
            return root->left;
        }

        if (root->left->priority > root->right->priority) {
            newNode = rotateRightCopy(newNode);
            newNode->right = remove(newNode->right, point_left, point_right);
        } else {
            newNode = rotateLeftCopy(newNode);
            newNode->left = remove(newNode->left, point_left, point_right);
        }
    }
    return newNode;
}

int upper_segment(Point* point, Node* root, int closest_above = -1) {
    if (root == nullptr){
        return closest_above;
    }
    float k = (root->point_left->y - root->point_right->y) / (root->point_left->x - root->point_right->x);
    float b = root->point_left->y - k * root->point_left->x;
    float y = k * point->x + b;
    if (y >= point->y){ // y > point->y если без границ, y >= point->y если с границами
        closest_above = root->point_left->number_polygon;

        if (root->left)
            return upper_segment(point, root->left, closest_above);
        else{
            return root->point_left->number_polygon;
        }
    }
    else{
        if (root->right)
            return upper_segment(point, root->right, closest_above);
        else{
            return closest_above;
        }
    }
}

int lower_segment(Point* point, Node* root, int closest_above = -1) {
    if (root == nullptr){
        return closest_above;
    }

    float k = (root->point_left->y - root->point_right->y) / (root->point_left->x - root->point_right->x);
    float b = root->point_left->y - k * root->point_left->x;
    float y = k * point->x + b;
    if (y <= point->y){ //y < point->y если без границ, y <= point->y если с границами
        closest_above = root->point_left->number_polygon;

        if (root->right)
            return lower_segment(point, root->right, closest_above);
        else{
            return root->point_left->number_polygon;
        }
    }
    else{
        if (root->left)
            return lower_segment(point, root->left, closest_above);
        else{
            return closest_above;
        }
    }
}


void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}


struct PointToArr {
    int x;
    Point* point; 
};


bool compareByX(const PointToArr& a, const PointToArr& b) {
    return a.x < b.x;
}


int binary_search(const std::vector<PointToArr>& all_points, int target_x) {
    int left = 0;
    int right = all_points.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (all_points[mid].x == target_x) {
            result = mid;  
            left = mid + 1; 
        } else if (all_points[mid].x < target_x) {
            left = mid + 1; 
        } else {
            right = mid - 1;
        }
    }
    if (result == -1 && right == all_points.size() - 1){
        result = all_points.size();
    }
    else if(result == -1){
        result = left - 1;
    }

    // std::cout<<"Бинарный результат: "<<result<<std::endl;
    return result;
}

int main(){
    int n, m;
    std::cin>>n >>m;
    std::vector<std::vector<Point>> matrix_polygons(n);
    for (int i = 0; i < n; i++){
        int m;
        std::cin>>m;
        for (int j = 0; j < m; j ++){
            Point point;
            std::cin>>point.x>>point.y;
            point.number_polygon = i;
            matrix_polygons[i].push_back(point);
        }
    }
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < matrix_polygons[i].size(); j++){
    //         std::cout<<matrix_polygons[i][j].x<<" "<<matrix_polygons[i][j].y<<" ";
    //     }
    //     std::cout<<std::endl;
    // }

    std::vector<PointToArr> all_points;
    for (int i = 0; i < n; i ++){
        for (int j = 0; j < matrix_polygons[i].size(); j++){
            int prev = j - 1;
            int next = j + 1;
            if (prev < 0){
                prev += matrix_polygons[i].size();
            }
            if (next == matrix_polygons[i].size()){
                next -= matrix_polygons[i].size();
            }
            matrix_polygons[i][j].prev = &matrix_polygons[i][prev];
            matrix_polygons[i][j].next = &matrix_polygons[i][next];
            PointToArr point_to_arr;
            point_to_arr.x = matrix_polygons[i][j].x;
            point_to_arr.point = &matrix_polygons[i][j];
            all_points.push_back(point_to_arr);
        }
    }
    // for(int i = 0; i < all_points.size(); i++){
    //     std::cout<<all_points[i].point<<" "<<all_points[i].x<<" "<<all_points[i].point->x<<" "<<all_points[i].point->y<<" "<<all_points[i].point->number_polygon<<" "<<all_points[i].point->prev<<" "<<all_points[i].point->next<<" ";
    //     std::cout<<std::endl;
    // }

    std::sort(all_points.begin(), all_points.end(), compareByX);


    // std::cout<<std::endl;
    // for(int i = 0; i < all_points.size(); i++){
    //     std::cout<<all_points[i].point<<" "<<all_points[i].x<<" "<<all_points[i].point->x<<" "<<all_points[i].point->y<<" "<<all_points[i].point->number_polygon<<" "<<all_points[i].point->prev<<" "<<all_points[i].point->next<<" ";
    //     std::cout<<std::endl;
    // }

    Node* root = nullptr;
    std::vector<Node*> versions; 
    int index_for_remove = -1;
    for (int i = 0; i < all_points.size(); i++){
        int x = all_points[i].x;
        int x_prev = all_points[i].point->prev->x;
        int x_next = all_points[i].point->next->x;
        if (x_prev < x){
            root = remove(root, all_points[i].point->prev, all_points[i].point); 
        }
        if (x_next < x){
            root = remove(root, all_points[i].point->next, all_points[i].point); 
        }
        if (x_prev > x){
            root = insert(root, all_points[i].point, all_points[i].point->prev, i); 
        }
        if (x_next > x){
            root = insert(root, all_points[i].point, all_points[i].point->next, i + all_points.size());
        }
        // versions.push_back(root); 
        // if (i + 1 == all_points.size() && all_points[i + 1].x != x){
        //     if (index_for_remove != -1){ // если закончились точки с одинаковым х, то удалим их все из дерева
        //         for (int j = index_for_remove; j < i; j++){
        //             if (x_prev < x){
        //                 root = remove(root, all_points[j].point->prev, all_points[j].point); 
        //             }
        //             if (x_next < x){
        //                 root = remove(root, all_points[j].point->next, all_points[j].point); 
        //             }
        //         }
        //         index_for_remove = -1;
        //     }
            // if (x_prev < x){
            //     root = remove(root, all_points[i].point->prev, all_points[i].point); 
            // }
            // if (x_next < x){
            //     root = remove(root, all_points[i].point->next, all_points[i].point); 
            // }
            versions.push_back(root); 
        // }
        // else{
        //     if (index_for_remove == -1){
        //         index_for_remove = i;
        //     }
        // }
        

    }

    // std::cout<<"количество версий: "<<versions.size()<<std::endl;
    for (int i = 0; i < m; i++){
        int x, y;
        std::cin>>x>>y;
        int result_index = binary_search(all_points, x);
        Point search_point;
        search_point.x = x;
        search_point.y = y;
        if (result_index >= 0 && result_index < versions.size()){
            int polygon_up = upper_segment(&search_point, versions[result_index]);
            int polygon_lower = lower_segment(&search_point, versions[result_index]);
            if(polygon_lower == polygon_up && polygon_lower != -1){
                std::cout<<"Number of polygon: "<<polygon_lower<<std::endl;
            }
            else{
                std::cout<<-1<<std::endl;
            }
        }
        else{
                std::cout<<-1<<std::endl;
        }
        
    }

}