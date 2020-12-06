#include <iostream>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map> 
#include <iterator>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include <deque>
#include <stack>
#include <functional>
#include <numeric>
#include <utility>
#include <limits>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sstream>

#define MOD 1000000007
#define max(a, b) (a < b ? b : a) 
#define min(a, b) ((a > b) ? b : a) 

typedef long int int32;
typedef unsigned long int uint32;
typedef long long int int64;
typedef unsigned long long int  uint64;

using namespace std;

class RMQ_SegTree {
    private:
        vector<int> input_array;
        vector<int> seg_tree;
        bool is_max = false;

        int query_range_helper(int left_index, int right_index, int seg_index, int current_left_index, int current_right_index){
            if(right_index < current_left_index || left_index > current_right_index){
                //not covered
                return 0;
            }
            if(left_index <= current_left_index && right_index >= current_right_index){
                return seg_tree[seg_index];
            }else{
                //it must be contained inside
                if(this->is_max){
                    return max(query_range_helper(left_index, right_index, get_left_child_index(seg_index), current_left_index, (current_left_index + current_right_index)/2)
                     , query_range_helper(left_index, right_index, get_right_child_index(seg_index), (current_left_index + current_right_index)/2 + 1, current_right_index));
                }else{
                    return min(query_range_helper(left_index, right_index, get_left_child_index(seg_index), current_left_index, (current_left_index + current_right_index)/2)
                     , query_range_helper(left_index, right_index, get_right_child_index(seg_index), (current_left_index + current_right_index)/2 + 1, current_right_index));
                }
                
            }
        }

        int insert_helper(int index, int value, int seg_index, int left_index, int right_index){
            if(index >= left_index && index <= right_index){
                if(left_index == right_index){
                    this->seg_tree[seg_index] = value;
                    return this->input_array[index];
                }
                int left = insert_helper(index, value, get_left_child_index(seg_index), left_index, (left_index + right_index)/2);
                int right = insert_helper(index, value, get_right_child_index(seg_index), (left_index+right_index)/2+1, right_index);
                if(this->is_max){
                    this->seg_tree[seg_index] = max(left, right);
                    return max(left,right);
                }else{
                    this->seg_tree[seg_index] = min(left, right);
                    return min(left, right);
                }
            }else{
                return this->seg_tree[seg_index];
            }
        }

        int get_left_child_index(int index){
            return 2*index + 1;
        }

        int get_right_child_index(int index){
            return 2*index + 2;
        }

    public:
        RMQ_SegTree(vector<int> input_array, bool is_max=false){
            this->is_max = is_max;
            this->input_array = input_array;
            seg_tree = vector<int>(2*input_array.size(), 0);
            build_seg_tree(0, 0, this->input_array.size()-1);
        }

        //O(2*n) time complexity (n + n/2 + ... = 2*n). O(log(n)) space complexity for call stack. This could be reduced to O(1) by making it iterative 
        int build_seg_tree(int seg_index, int left_index, int right_index){
            if(right_index == left_index){
                this->seg_tree[seg_index] = this->input_array[left_index];
            }else{
                int left = build_seg_tree(get_left_child_index(seg_index), left_index, (left_index + right_index)/2);
                int right = build_seg_tree(get_right_child_index(seg_index), (left_index + right_index)/2 + 1, right_index);
                if(this->is_max){
                    this->seg_tree[seg_index] = max(left, right);
                }else{
                    this->seg_tree[seg_index] = min(left, right);
                }
            }
            return this->seg_tree[seg_index];
        }

        //O(log(n)) time-complexity. O(log(n)) space-complexity from stack. Can be reduced to O(1) space.
        void insert(int index, int value){
            this->input_array[index] = value;
            insert_helper(index, value, 0, 0, this->input_array.size()-1);
        }

        //assumes a valid query input (seg_index = 0 )
        int query_range_sum(int left_index, int right_index){
            return query_range_helper(left_index, right_index, 0, 0, this->input_array.size()-1);
        }

        void print_seg_tree(int seg_index, int left_index, int right_index){
            if(seg_index >= this->seg_tree.size()){
                return;
            }
            cout << seg_index << "(" << left_index << ", " << right_index << "): " << this->seg_tree[seg_index] << "\n";
            if(left_index != right_index){
                print_seg_tree(get_left_child_index(seg_index), left_index, (left_index+right_index)/2);
                print_seg_tree(get_right_child_index(seg_index), (left_index+right_index)/2+1, right_index);
            }
        }

        int get_arr_size(){
            return this->input_array.size();
        }

        int get_tree_size(){
            return this->seg_tree.size();
        }

};

int main(){
    ios::sync_with_stdio(0); 
    cin.tie(0); 

    RMQ_SegTree t({1,2,3,4});

    t.print_seg_tree(0, 0, 3);
    t.insert(0, 4);
    t.insert(3, 2);
    t.print_seg_tree(0, 0, 3);
    
    return 0;
}