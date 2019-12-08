/** Written by andri
 * 27/07/2019
 * for dionyziz **/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct node {
    // if of greater level than leaf, then equals the sum value of the subtree
    int value;
    // left corner index 
    int left_corner;
    // right corner index 
    int right_corner;
    node* parent;
    node* left_child;
    node* right_child;
    // tree level of the node
    int level;
}; 

node * newNode(int value, int left_corner, int right_corner, node* parent, node* left_child, node* right_child, int level)
{
    node * new_node = (struct node*)malloc(sizeof(struct node));

    new_node->value = value;
    new_node->left_corner = left_corner;
    new_node->right_corner = right_corner;
    new_node->parent = parent;
    new_node->left_child = left_child;
    new_node->right_child = right_child;
    new_node->level = level;

    return new_node;
}

// calculate sum of range [start, end]
int calculate_sum(node * data[], int start, int end)
{
    //base case
    if (end == start)
        return data[start] -> value;

    node * s = data[start];

    // find the LCA
    while (s->right_corner < end)
        s = s->parent;

    // subtract amounts that shouldn't be counted
    int left_amount_to_be_subtracted = 0;
    int right_amount_to_be_subtracted = 0;
    if (start > s->left_corner)
        left_amount_to_be_subtracted = calculate_sum(data, s->left_corner, start - 1); 
    if (end < s->right_corner)
        right_amount_to_be_subtracted = calculate_sum(data, end + 1, s->right_corner);

    return (s->value - left_amount_to_be_subtracted - right_amount_to_be_subtracted); 
}

// add the value to the leaf node. Continue with its parents up until the root.
void add_value(node * data, int value)
{
    data->value+=value;
    while (data->parent != NULL)
    {
        data = data->parent;
        data->value+=value;
    }
}

// helping function for inserting a new node in the segment tree.
bool insert_new_node_rec(node * root, node * new_node)
{
    // base case
    if (root->level == 0)
        return false;

    bool inserted = insert_new_node_rec(root->right_child, new_node);
    if (!inserted)
    {
        // insert the new node, the first time this inequality holds true (bottom-up)
        if (root->right_child->level < root->left_child->level)
        {
            node * new_right_child = newNode(new_node->value + root->right_child->value, 
                                             root->right_child->left_corner,
                                             new_node->right_corner,
                                             root,
                                             root->right_child,
                                             new_node,
                                             root->right_child->level + 1);
            
            new_node->parent = new_right_child;
            root->right_child->parent = new_right_child;
            root->right_child = new_right_child;
            root->value+=new_node->value;
            root->right_corner=new_node->right_corner;
            return true;
        }    
        return false;
    }
    root->value+=new_node->value;
    root->right_corner = root->right_child->right_corner;
    return inserted;
}

// calls the recursive insert_new_node_rec. Considers sorted insertion sequence.
node * insert_new_node(node * root, node * new_node)
{
    if (root == NULL)
    {
        node * root = newNode(new_node->value,
                       new_node->left_corner,
                       new_node->right_corner,
                       NULL,
                       new_node,
                       NULL,
                       1);
        new_node->parent = root;
        return root;
    }

    if (root->right_child == NULL)
    {
        root->right_child = new_node;
        root->right_corner = new_node->right_corner;
        root->value+=new_node->value;
        new_node->parent = root;
        return root;
    }

    if (insert_new_node_rec(root, new_node))
        return root;
    node * new_root = newNode(root->value + new_node->value,
                              root->left_corner,
                              new_node->right_corner,
                              NULL,
                              root,
                              new_node,
                              root->level + 1);

    new_node->parent = new_root;
    root->parent = new_root;
    return new_root;
}


void post_order_traversal(struct node* root) {
   if(root != NULL) {
      post_order_traversal(root->left_child);
      post_order_traversal(root->right_child);
      cout << root->value << " ";
   }
}

void deallocate(struct node* root) {
   if(root != NULL) {
      deallocate(root->left_child);
      deallocate(root->right_child);
      free(root);
   }
}

int main(int argc, char* argv[])
{
    if (argc < 2){
        cout << "Give two input file paths." << endl;
        cout << "A file containing n integer values and a file with the q sum queries." << endl;
        return 1;
    }

    ifstream integers_file(argv[1]);
    ifstream queries_file(argv[2]);
    ofstream output_file("./output_file.txt");
    output_file << "Query | Sum" << "\n";

    // n: number of input values
    int n;
    integers_file >> n;
    if (n <= 0)
    {
        cout << "Wrong input values." << endl;
        return 1;
    }

    node * data[n];
    node * root = NULL;

    cout << "Index | Value" << endl;
    for (int i = 0; i < n; i++)
    {
        int input_value;
        integers_file >> input_value;
        data[i] = newNode(input_value, i, i, NULL, NULL, NULL, 0);

        cout << i << " | " << data[i]->value << endl;

        root = insert_new_node(root, data[i]);
        cout << "Insertion of value: " << data[i]->value << ". New root value = " << root->value << endl;
        if (i > 0)
            cout << "Root left child value = " << root->left_child->value << ". Root right child value = " << root->right_child->value << endl;
        else
            cout << "Root left child value = " << root->left_child->value << ". Root right child equals to NULL "<< endl;
    }
    
    cout << "" << endl;
    cout << "Post-order traversal of the segment tree:" << endl;
    post_order_traversal(root);
    cout << "" << endl;
    cout << "" << endl;

    int a, b;
    // intention == 'a' for adding value at some index
    // intention == 'q' for making a segment sum query
    char intention;
    while (queries_file >> intention >> a >> b)
    {
        if (intention == 'q' && (a < 0 || b <=0 || b >= n || a > b))
        {
            cout << "Wrong queries values." << endl;
            continue;
        }
        if (intention == 'a' && (a < 0 || a >= n))
        {
            cout << "Wrong index value: " << a << endl;
            continue;
        }
        cout << intention << ":" << a << " " << b << endl;
        if (intention == 'a')
        {
            add_value(data[a], b);
            //cout << "Value of node at index " << a << " now equals to: " << data[a]->value << endl;; 
            continue;
        }
        int sum = calculate_sum(data, a, b);
        cout << "Final sum = " << sum << endl;
        output_file << " " << a << " " << b << "  |" << sum << "\n";
    }

    //free memory allocated for the whole tree
    deallocate(root);

    return 0;
}
