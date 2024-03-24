#include <iostream>
#include <memory> // Include for smart pointers like unique_ptr
#include <chrono> // Include for measuring execution time

using namespace std;
using namespace std::chrono;

struct TreeNode {
    int val;
    unique_ptr<TreeNode> left; // Using unique_ptr for dynamically allocated memory
    unique_ptr<TreeNode> right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

void inorderTraversal(TreeNode* root) {
    if (root == nullptr)
        return;
    inorderTraversal(root->left.get()); // Using get() to access raw pointer from unique_ptr
    cout << root->val << " ";
    inorderTraversal(root->right.get());
}

int main() {
    // Create a binary tree
    unique_ptr<TreeNode> root(new TreeNode(1)); // Using unique_ptr for root
    root->left = unique_ptr<TreeNode>(new TreeNode(2));
    root->right = unique_ptr<TreeNode>(new TreeNode(3));
    root->left->left = unique_ptr<TreeNode>(new TreeNode(4));
    root->left->right = unique_ptr<TreeNode>(new TreeNode(5));

    // Start measuring time
    auto start = high_resolution_clock::now();

    // Perform inorder traversal
    cout << "Inorder Traversal: ";
    inorderTraversal(root.get()); // Using get() to access raw pointer from unique_ptr

    // Stop measuring time and calculate the elapsed time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "\nExecution Time: " << duration.count() << " microseconds" << endl;

    // Clean up memory (no need for explicit delete thanks to unique_ptr)
    return 0;
}
