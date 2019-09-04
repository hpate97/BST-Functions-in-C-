#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>

//Raja Patel
//hpate85


//added to make easier for use
using namespace std;

template <typename T>
class bst {
    
private:
    struct bst_node {
        T      val;
        bst_node *left;
        bst_node *right;
        
        //NEED TO ADD TO THE STRUCT
        //NEED TO EDIT : INSERT, DELETE, FROM_VECTOR
        
        //used to count left and right side of root
        int countLeftnodes = 0;
        int countRightnodes = 0;
        
        //have a size variable to know immediatly the size of the tree
        int bstsize = 0;
        
        //std::vector<bst_node*> nodes;
        
        bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr)
        : val { _val },  left { l }, right {r}
        { }
    };
    
    
    
public:
    // constructor:  initializes an empty tree
    bst(){
        root = nullptr;
    }
    
private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(bst_node *r){
        if(r==nullptr) return;
        delete_nodes(r->left);
        delete_nodes(r->right);
        delete r;
    }
    
public:
    // destructor
    ~bst() {
        delete_nodes(root);
    }
    
private:
    
    //****SIZE BALANCING FUNCTIONS BEGIN****
    
    //function to create a balanced BST using TWO helper functions below.
    static  bst_node* CreateBST(bst_node* root) {
        std::vector <bst_node*> BSTnodevector;
        int n;
        
        bstnodevect(root,BSTnodevector);
        
        n = BSTnodevector.size();
        return buildbalancedtree(BSTnodevector,0,n-1);
    }
    
    
    //takes in the nodes from the unblanaced BST and puts them in a vector using an inorder traversal.
    static void bstnodevect(bst_node* root,std::vector<bst_node*> &vectorA) {
        //base case
        if (root == nullptr){
            return;
        }
        
        bstnodevect(root->left,vectorA);    //Left
        vectorA.push_back(root);            //print, in this case, throw into vector
        bstnodevect(root->right,vectorA);   //Right
    }
    
    // Building the tree by using recursion and updating the bookkeeping of countLeftnodes and countRightnodes
    static  bst_node* buildbalancedtree(std::vector<bst_node*> &vectorA, int start, int end){
        if (start > end) { return nullptr; }
        
        int mid = (start + end) / 2;
        
        bst_node *root = vectorA[mid];              // Set the ROOT of the tree to the mid. 10 is the root.
        
        root->countLeftnodes = (mid - 1) - start;   // now the countLeftnodes will hold values 7, 8, 9 as each iteration passes for the recursive call
        root->countRightnodes = end - (mid + 1);    // now the countRightnodes will hold values 11, 12, 13, 15 as each iteration passes for the recursive call
        root->bstsize = end;                        // update the size variable;
        
        root->right = buildbalancedtree(vectorA, mid + 1, end); //go into the right subtree
        
        root->left = buildbalancedtree(vectorA, start, mid-1);  //go into the left subtree
        
        return root;
    }
    
    //****SIZE BALANCING FUNCTIONS END****
    
    /**
     * function:  insert()
     * desc:      recursive helper function inserting x into
     *            binary search tree rooted  at r.
     *
     * returns:   pointer to root of tree after insertion.
     *
     * notes:     if x is already in tree, no modifications are made.
     */
    static bst_node * _insert(bst_node *r, T & x, bool &success){
        //if there is no node, following than make the connection and place the value for the new node
        if(r == nullptr){
            success = true;
            return new bst_node(x, nullptr, nullptr);
        }
        
        //if already there, do not accept
        if(r->val == x){
            success = false;
            return r;
        }
        
        // if x is smallar than current node value, move left.
        if(x < r->val){
            
            //recursive call
            r->left = _insert(r->left, x, success);
            
            //if node is placed on the laft side add to counter
            if(success){
                r->countLeftnodes++;
            }
            
            //check every time if the size balancing is needed
            if (r->countLeftnodes > (2 * r->countRightnodes) + 1) {
                r = CreateBST(r);
            }
            return r;
        }
        else {
            
            //recursive call
            r->right = _insert(r->right, x, success);
            
            //if node is placed on the laft side add to counter
            if(success){
                r->countRightnodes++;
            }
            
            //check every time if the size balancing is needed
            if (r->countRightnodes > (2 * r->countLeftnodes) + 1) {
                r = CreateBST(r);
            }
            return r;
        }
    }
    
    
public:
    /**
     * function:  insert
     * desc:      inserts x into BST given by t.  Note that
     *            a BST stores a SET -- no duplicates.  Thus,
     *            if x is already in t when call made, no
     *            modifications to tree result.
     *
     * note:      helper function does most of the work.
     *
     */
    bool insert(T & x){
        bool success;
        root = _insert(root, x, success);
        // include to update the size
        root->bstsize++;
        return success;
    }
    
    /**
     * function:  contains()
     * desc:      returns true or false depending on whether x is an
     *            element of BST (calling object)
     *
     */
    bool contains(const T & x){
        bst_node *p = root;
        
        while(p != nullptr){
            
            if(p->val == x)
                return true;
            if(x < p->val){
                p = p->left;
            }
            else
                p = p->right;
        }
        return false;
    }
    
private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _min_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->left != nullptr)
            r = r->left;
        return r;
    }
    
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _max_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->right != nullptr)
            r = r->right;
        return r;
    }
    
    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static bst_node * _remove(bst_node *r, T & x, bool &success){
        bst_node *tmp;
        bool sanity;
        
        if(r==nullptr){
            success = false;
            return nullptr;
        }
        if(r->val == x){
            success = true;
            
            if(r->left == nullptr){
                tmp = r->right;
                delete r;
                return tmp;
            }
            if(r->right == nullptr){
                tmp = r->left;
                delete r;
                return tmp;
            }
            // if we get here, r has two children
            r->val = _min_node(r->right)->val;
            r->right = _remove(r->right, r->val, sanity);
            
            //if sanity checker is true, value is removed and need to decremnet the node counter
            if (sanity) {
                r->countRightnodes--;
            }
            
            if(!sanity)
                std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
            return r;
        }
        if(x < r->val){
            r->left = _remove(r->left, x, success);
            
            //if removed, decrement
            if (success) {
                r->countLeftnodes--;
            }
            //check to see if there is a need to balnce the tree
            if (r->countLeftnodes > (2 * r->countRightnodes) + 1) {
                r = CreateBST(r);
            }
        }
        else {
            r->right = _remove(r->right, x, success);
            
            //if removed, decrement
            if (success) {
                r->countRightnodes--;
            }
            //check to see if there is a need to balnce the tree
            if (r->countRightnodes > (2 * r->countLeftnodes) + 1) {
                r = CreateBST(r);
            }
        }
        return r;
        
    }
    
public:
    
    bool remove(T & x){
        bool success;
        root = _remove(root, x, success);
        //update the size
        if (success != 0){
            root->bstsize--;
        }
        return success;
    }
    
    
private:
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(bst_node *r){
        if(r==nullptr) return 0;
        return _size(r->left) + _size(r->right) + 1;
    }
    
public:
    int size() {
        return _size(root);
    }
    
    
private:
    
    static int _height(bst_node *r){
        int l_h, r_h;
        
        if(r==nullptr) return -1;
        l_h = _height(r->left);
        r_h = _height(r->right);
        return 1 + (l_h > r_h ? l_h : r_h);
    }
    
public:
    
    int height() {
        return _height(root);
    }
    
    bool min(T & answer) {
        if(root == nullptr){
            return false;
        }
        answer = _min_node(root)->val;
        return true;
    }
    
    T max() {
        return _max_node(root)->val;
    }
    
    /******************************************
     *
     * "stubs" for assigned TODO functions below
     *
     *****************************************/
    
    /* TODO
     * Function:  to_vector
     * Description:  allocates a vector of type T and populates
     *               it with the elements of the tree in sorted
     *               order.  A pointer to the vector is returned.
     *
     * runtime:  O(n) where n is the tree size.
     *
     */
private:
    //Helper function to recursively print contents of tree in-order into a vector
    void to_vector_HELPER(bst_node *p, std::vector<T> &vecB)
    {
        if (p == nullptr) { return; }
        //in-order into vector
        to_vector_HELPER(p->left,vecB);   //Left
        vecB.push_back(p->val);          //Print, in this case pushbackinto the vector
        to_vector_HELPER(p->right,vecB);  //Right
    }
public:
    std::vector<T> * to_vector()
    {
        std::vector<T> *inorderVec = new std::vector<T>;
        to_vector_HELPER(root, *inorderVec);
        
        return inorderVec;
    }
    
    /* TODO
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.
     *    i ranges from 1..n where n is the number of elements in the
     *    tree.
     *
     *    If i is outside this range, false is returned.
     *    Otherwise, true is returned (indicating "success").
     *
     * Runtime:  O(h) where h is the tree height
     */
    bool get_ith(int i, T &x)
    {
        if(i < 1) { return false; }
        
        //referance back the x val
        x = get_ith_HELPER(i, root);
        return true;
    }
    
private:
    //helper function to find the val for the ith position
    int get_ith_HELPER(int checkI, bst_node *p){
        
        
        //if the checkI is at the root, than return the value of the root
        if(checkI == (p->countLeftnodes) + 1){
            return p->val;
        }
        
        //if checkI is less than the countLeftnodes, move position to the left of the tree to narrow down on the search.
        else if(checkI <= (p->countLeftnodes)){
            return get_ith_HELPER(checkI, p->left);
        }
        
        else
        {
            //if not on the left side of the root, its on the right and you take the difference of
            //the checki from the countLeftnodes + 1 (the one is the root) to find the ith value on the right.
            return get_ith_HELPER(checkI - (p->countLeftnodes + 1), p->right);
        }
    }
    
public:
    /**************************************************************************************************
     * get_ith_SLOW:  slow version of get_ith which gives the right answer, but
     *   does so by "walking the entire tree".
     * might be helpful for debugging?
     * Private helper below does most of the work
     */
    bool get_ith_SLOW(int i, T &x) {
        int n = size();
        int sofar=0;
        
        if(i < 1 || i > n)
            return false;
        
        _get_ith_SLOW(root, i, x, sofar);
        return true;
    }
    
private:
    // recursive helper function that does most of the work
    static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
        if(t==nullptr)
            return;
        _get_ith_SLOW(t->left, i, x, sofar);
        
        if(sofar==i)
            return;
        sofar++;
        if(sofar==i) {
            x = t->val;
            return;
        }
        _get_ith_SLOW(t->right, i, x, sofar);
    }
    
public:
    
    /* TODO
     * Function: position_of
     * Description:  this is like the inverse of
     *       get_ith:  given a value x, determine the
     *       position ("i") where x would appear in a
     *       sorted list of the elements and return
     *       the position as an integer.
     *       If x is not in the tree, -1 is returned.
     *       Examples:
     *           if x happens to be the minimum, 1 is returned
     *           if x happens to be the maximum, n is returned where
     *               n is the tree size.
     *
     *       Notice the following property:  for a bst t with n nodes,
     *           pick an integer pos:1 <= pos <= n.
     *           Now consider the following:
     *
     T x;
     int pos, pos2;
     // set pos to a value in {1..n}
     t.get_ith(pos, x);  // must return true since pos is in {1..n}
     
     // now let's find the position of x (just retrieved)
     pos2 = t.position_of(x);
     if(pos != pos2) {
     std::cout << "THERE MUST BE A BUG!\n";
     }
     
     See how position_of performs the inverse operation of get_ith?
     *
     * Return:  -1 if x is not in the tree; otherwise, returns the position where x
     *          would appear in the sorted sequence of the elements of the tree (a
     *          value in {1..n}
     *
     * Runtime:  O(h) where h is the tree height
     */
    int position_of(const T & x) {
        
        if (x < 1){ return -1; }
        return position_of_HELPER(x, root);
    }
    
private:
    //find the position of x recursivly.
    static int position_of_HELPER(const T &x, bst_node *p){


        if (p == nullptr)
        {
            return 0;
        }

        int findpos;

        if(x == p->val)
        {
            findpos = (p->countLeftnodes) + 1;
            return (p->countLeftnodes) + 1;
        }

        else if(x < p->val)
        {
            return position_of_HELPER(x, p->left);
        }

        else if (x > p->val)
        {
            findpos = (p->countLeftnodes) + 1;
            return position_of_HELPER(x, p->right);
        }
        else
        {
            //if not found
            return -1;
        }
    }
    
    


public:
    /* TODO
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */
    int num_geq(const T & x) {
        
        int numgeqcount = 0;
        num_geq_HELPER(x, &numgeqcount, root);
        
        return numgeqcount;  // placeholder
    }
    
private:
    void num_geq_HELPER(const T & greaterVal, int *counterA, bst_node *p){
        
        if(p == nullptr) { return; }
        
        //if the node val equals the same val that were looking for
        if(p->val == greaterVal){
            (*counterA)++;
            return num_geq_HELPER(greaterVal, counterA, p->right);
        }
        
        // if the current node val is greater than the val we are comparing...
        else if(p->val > greaterVal){
            
            // if true, than add all the nodes to the right of p->val cause they are all nodes that are greater than the number we are comparing too
            (*counterA) = (*counterA) + (p->countRightnodes + 1);
            
            //move to the left to see if that val is also smaller or bigger to the val we are comparing too
            return num_geq_HELPER(greaterVal, counterA, p->left);
        }
        
        else {
            //else go to the right node
            return num_geq_HELPER(greaterVal, counterA, p->right);
        }
        
    }

    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *                (private helper does most of the work)
     *
     */
    int num_geq_SLOW(const T & x)
    {
        return _num_geq_SLOW(root, x);
    }
    
private:
    static int _num_geq_SLOW(bst_node * t, const T & x) {
        
        int total;
        
        if(t==nullptr) return 0;
        total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);
        
        if(t->val >= x)
            total++;
        return total;
    }
    
public:
    
    /* TODO
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_leq(const T &x) {
        
        int numleqcount = 0;
        num_leq_HELPER(x, &numleqcount, root);   //edit
        
        return numleqcount;  // placeholder
    }
    
private:
    //
    void num_leq_HELPER(const T & lesserVal, int *counterB, bst_node *p){
        
        if(p == nullptr) { return; }
        
        //if the node val equals the same val that were looking for
        if(p->val == lesserVal){
            (*counterB)++;
            return num_leq_HELPER(lesserVal, counterB, p->left);
        }
        
        // if the current node val is less than the val we are comparing...
        else if(p->val < lesserVal){
            
            // if true, than add all the nodes to the left of p->val cause they are all nodes that are less than the number we are comparing too
            (*counterB) = (*counterB) + (p->countLeftnodes) + 1;
            
            //move to the right to see if that val is also smaller or bigger to the val we are comparing too
            return num_leq_HELPER(lesserVal, counterB, p->right);
        }
        
        else {
            //else go to the leftt node
            return num_leq_HELPER(lesserVal, counterB, p->left);
        }
    }
    
    
public:
    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *               (private helper _num_leq_SLOW does most of the work)
     */
    int num_leq_SLOW(const T & x) {
        return _num_leq_SLOW(root, x);
    }
private:
    
    // helper for num_leq_SLOW
    static int _num_leq_SLOW(bst_node *t, const T &x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);
        
        if(t->val <= x)
            total++;
        return total;
    }
    
public:
    
    /* TODO
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_range(const T & min, const T & max) {
        int numrangecount;
        
        numrangecount = num_range_HELPER(min, max, root);
        
        return numrangecount;
    }
    
    
private:
    
    int num_range_HELPER(const T & low, const T & high, bst_node *p){
        
        int elementHigh, elementLow, helperSize, sizeminusMax, sizeminusMin, combine, range;
        
        elementLow = num_leq(high);                 //get all the elements lower than the Max
        elementHigh = num_geq(low);                 //get all the elements higher than the Min
        helperSize = p->bstsize;                    //record the size of the entire tree
        
        sizeminusMax = helperSize - elementHigh;    //get the values that are greater than the min
        sizeminusMin = helperSize - elementLow;     // get the valus that are lower than the max
        
        combine = (sizeminusMax + sizeminusMin);
        range = helperSize - combine;               // compute the range
        
        return range;
    }
    
    /*
     * function:     num_range_SLOW
     * description:  same functionality as num_range but sloooow (linear time)
     *               (private helper _num_range_SLOW does most of the work)
     *
     */
    int num_range_SLOW(const T & min, const T & max) {
        return _num_range_SLOW(root, min, max);
    }
    
private:
    static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_range_SLOW(t->left, min, max) +
        _num_range_SLOW(t->right, min, max);
        
        if(t->val >= min && t->val <= max)
            total++;
        return total;
    }
    
public:
    
    /*
     * TODO
     * function:     extract_range
     * Description:  allocates a vector of element type T
     *               and populates it with the tree elements
     *               between min and max (inclusive) in order.
     *               A pointer to the allocated and populated
     *               is returned.
     *
     * notes/comments:  even if the specified range is empty, a
     *                  vector is still allocated and returned;
     *                  that vector just happens to be empty.
     *                  (The function NEVER returns nullptr).
     *
     * runtime:  the runtime requirement is "output dependent".
     *           Let k be the number of elements in the specified range
     *           (and so the length of the resulting vector) and let h
     *           be the height of the tree.  The runtime must be:
     *
     *                  O(h + k)
     *
     *           So...while k can be as large as n, it can be as small
     *           as zero.
     *
     */
    std::vector<T> * extract_range(const T & min, const T & max) {
        
        std::vector<T> *erVec = new std::vector<T>;
        
        if (min > max)
        {
            return erVec;
        }
        
        extract_range_HELPER(root, erVec, min, max);
        
        return erVec;
    }
private:
    // Returns vector in range [low, high]
    static void extract_range_HELPER(bst_node *p, std::vector<T> *ERVec, const T &min, const T &max){
        
        if (p == nullptr) {
            return;
        }
        
        if (p->val > max)
        {
            extract_range_HELPER(p->left, ERVec, min, max);
        }
        else if (p->val < min)
        {
            extract_range_HELPER(p->right, ERVec, min, max);
        }
        else
        {
            extract_range_HELPER(p->left, ERVec, min, max);
            ERVec->push_back(p->val);
            extract_range_HELPER(p->right, ERVec, min, max);
        }
    }
    
    /***************************************************
     *  PREORDER AND INORDER TREE TRAVERSALS BELOW      *
     *                                                  *
     ***************************************************/
    
private:
    static void indent(int m){
        int i;
        for(i=0; i<m; i++)
            std::cout << "-";
    }
    
    static void _inorder(bst_node *r){
        if(r==nullptr) return;
        _inorder(r->left);
        std::cout << "[ " << r->val << " ]\n";
        _inorder(r->right);
    }
    
    static void _preorder(bst_node *r, int margin){
        if(r==nullptr) {
            indent(margin);
            std::cout << " nullptr \n";
        }
        else {
            indent(margin);
            std::cout << "[ " << r->val << " ]\n";
            _preorder(r->left, margin+3);
            _preorder(r->right, margin+3);
        }
    }
    
public:
    void inorder() {
        std::cout << "\n======== BEGIN INORDER ============\n";
        _inorder(root);
        std::cout << "\n========  END INORDER  ============\n";
    }
    
    
    void preorder() {
        
        std::cout << "\n======== BEGIN PREORDER ============\n";
        _preorder(root, 0);
        std::cout << "\n========  END PREORDER  ============\n";
        
    }
    
    /***************************************************
     *  END PREORDER AND INORDER TREE TRAVERSALS        *
     *                                                  *
     ***************************************************/
    
private:
    /*
     * Recursive  helper function _from_vec, used by
     * from_sorted_arr(...). The function must return a sub-tree that is
     * perfectly balanced, given a sorted array of elements a.
     */
    static bst_node * _from_vec(const std::vector<T> &a, int low, int hi){
        int m;
        bst_node *root;
        
        if(hi < low) return nullptr;
        m = (low+hi)/2;
        root = new bst_node(a[m]);
        root->left  = _from_vec(a, low, m-1);
        root->right = _from_vec(a, m+1, hi);
        return root;
        
    }
    
public:
    static bst * from_sorted_vec(const std::vector<T> &a, int n){
        
        bst * t = new bst();
        t->root = _from_vec(a, 0, n-1);
        return t;
    }
    
private:
    
    /*
     * DO NOT MODIFY THE TWO FUNCTIONS BELOW!!!
     */
    static void _to_preorder_vec(bst_node *t, std::vector<T> &vec) {
        if(t==nullptr) return;
        vec.push_back(t->val);
        _to_preorder_vec(t->left, vec);
        _to_preorder_vec(t->right, vec);
    }
    std::vector<T> * to_preorder_vec() {
        std::vector<T> *vec = new std::vector<T>();
        _to_preorder_vec(root, *vec);
        return vec;
    }
    
    /*
     * Finally, the data members of the bst class
     */
private:
    bst_node *root;
    
    
}; // end class bst

#endif
