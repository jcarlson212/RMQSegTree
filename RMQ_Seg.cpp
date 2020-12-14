#include <iostream>
#include <vector>

using namespace std;

//Segment Tree for Min or Max queries (it can be toggled). The template class used needs to be supported by the min and max functions
//the expected data types are ints, long longs, doubles, and fixed width data types like int64_t.
template <class T>
class RMQ_SegTree {
    private:
        T min(T a, T b){
            return (a <= b) ? a : b;
        }
        T max(T a, T b){
            return (a >= b) ? a : b;
        }

        vector<T> input_array;
        vector<T> seg_tree;
        bool is_max = false;

        T query_range_helper(int left_index, int right_index, int seg_index, int current_left_index, int current_right_index){
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

        T insert_helper(int index, int value, int seg_index, int left_index, int right_index){
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
        RMQ_SegTree(vector<T> input_array, bool is_max=false){
            this->is_max = is_max;
            this->input_array = input_array;
            seg_tree = vector<T>(2*input_array.size(), 0);
            build_seg_tree(0, 0, this->input_array.size()-1);
        }

        //O(2*n) time complexity (n + n/2 + ... = 2*n). O(log(n)) space complexity for call stack. This could be reduced to O(1) by making it iterative 
        T build_seg_tree(int seg_index, int left_index, int right_index){
            if(right_index == left_index){
                this->seg_tree[seg_index] = this->input_array[left_index];
            }else{
                T left = build_seg_tree(get_left_child_index(seg_index), left_index, (left_index + right_index)/2);
                T right = build_seg_tree(get_right_child_index(seg_index), (left_index + right_index)/2 + 1, right_index);
                if(this->is_max){
                    this->seg_tree[seg_index] = max(left, right);
                }else{
                    this->seg_tree[seg_index] = min(left, right);
                }
            }
            return this->seg_tree[seg_index];
        }

        //O(log(n)) time-complexity. O(log(n)) space-complexity from stack. Can be reduced to O(1) space.
        void insert(int index, T value){
            this->input_array[index] = value;
            insert_helper(index, value, 0, 0, this->input_array.size()-1);
        }

        //assumes a valid query input (seg_index = 0 )
        T query_range_sum(int left_index, int right_index){
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

    RMQ_SegTree<int64_t> t({1,2,3,4});

    t.print_seg_tree(0, 0, 3);
    t.insert(0, 4);
    t.insert(3, 2);
    t.print_seg_tree(0, 0, 3);
    
    return 0;
}
