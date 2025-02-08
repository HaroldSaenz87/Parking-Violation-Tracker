/*
This program is written by: HAROLD SAENZ  */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "leak_detector_c.h"

#define MAX 26


// tree node struct
typedef struct tree
{
    char Veh_owner[MAX];
    int fine;

    struct tree *left;
    struct tree *right;

    int depth;
    
}Tree_node;


//-----------------------------------------------(prototypes)---------------------------------------------------------------------------------------------------------------//

Tree_node *create_node(char name[], int fine);
Tree_node *insertion(Tree_node *root, Tree_node *val, int CurrDepth);
Tree_node *search(Tree_node *root, char name[]);
Tree_node *parent(Tree_node *root, Tree_node *deleting);
Tree_node *MaxVal(Tree_node *root);
Tree_node *delete(Tree_node *root, Tree_node *found);


double amount_nodes(Tree_node *p);
double total_fines(Tree_node *p);
double averages(Tree_node *p);
double below_fines(Tree_node *p, char name[]);
int max(int x, int y);
int calc_height(Tree_node* root);
int isLeaf(Tree_node *deleting);
int hasOnlyLeftChild(Tree_node *deleting);
int hasOnlyRightChild(Tree_node *deleting);
void new_depth(Tree_node *root, int current);

void in_order(Tree_node *p);
void freeTree(Tree_node *root);

//-----------------------------------------------(END)---------------------------------------------------------------------------------------------------------------//



int main()
{
    //atexit(report_mem_leak); //for memory leak detector

    Tree_node *My_root = NULL;

    int Num_commands;


    // number of commands for processing
    scanf("%d", &Num_commands);

    char command[MAX];
    char name[MAX];
    int fine;
    int Deduct_fine;


    for(int i = 0; i < Num_commands; i++)
    {

        // the command being processed
        scanf("%s", command);
        
        // add the node 
        if(strcmp(command, "add") == 0)     
        {
            scanf("%s %d", name, &fine);


            Tree_node *already_there = search(My_root, name);   // search if that person is already there 

            if(already_there != NULL)       // there was someone found so increase that fine
            {
                already_there->fine += fine;

                printf("%s %d %d\n" , already_there->Veh_owner, already_there->fine, already_there->depth);
            }
            else        // if there wasnt already that person 
            {
                int current_depth = 0;  // tracking depth 
                Tree_node *temp = create_node(name, fine);  // create node

                My_root = insertion(My_root, temp, current_depth);     // insert into tree 

                Tree_node *found = search(My_root, name);   // search for the name to later be printed out 

                printf("%s %d %d\n", found->Veh_owner, found->fine, found->depth);

            }


        }

        // deduct from the fine
        else if(strcmp(command, "deduct") == 0)
        {
            scanf("%s %d", name, &Deduct_fine);
            
            Tree_node *found = search(My_root, name);       // search it

            if(found == NULL)       // doesnt exist :(
            {
                printf("%s not found\n", name);
            }
            else  // exits :)
            {

                found->fine = found->fine - Deduct_fine;        // fines paid for that person
                
                if(found->fine <= 0)        // once fine is paid remove that owner 
                {
                    //deletion
                    int depth = 0;      // tracker for the new change

                    My_root = delete(My_root, found);
                    new_depth(My_root, depth);      // new depths after the tree changes from deletion

                    printf("%s removed\n", name);
                }
                else        // still owe money
                {
                    printf("%s %d %d\n", found->Veh_owner, found->fine, found->depth);
                }
            }

        }

        // search for the node (name)
        else if (strcmp(command, "search") == 0)
        {
            scanf("%s", name);

            Tree_node *found = search(My_root, name); // looking for that name
            
            if(found == NULL)       // that name dont exist 
            {
                printf("%s not found\n", name);
            }
            else        //IT EXIST!!!!
            {
                printf("%s %d %d\n", found->Veh_owner, found->fine, found->depth);

            }
        }

        // get the avg amount of fine per vehicle
        else if (strcmp(command, "average") == 0)
        {

            double average = averages(My_root);
            printf("%.2f\n", average);    // prints 2 decimal places

        }

        // calc and comapres heights of the left and right subtrees of the root node
        else if (strcmp(command, "height_balance") == 0)
        {
            int right = calc_height(My_root->right);        // get the max height of the right subtree
            int left = calc_height(My_root->left);          // get the max height of the left

            if(left == right)       // balanced
            {
                printf("left height = %d right height = %d balanced\n", left, right);
            }
            else    // not balanced
            {
                printf("left height = %d right height = %d not balanced\n", left, right);
            }


        }

        // calc the total amount of fines for owners names come before the specified name (alphbetically)
        else if (strcmp(command, "calc_below") == 0)
        {
            scanf("%s", name);

            int below_name = below_fines(My_root, name);

            printf("%d\n", below_name); 


        }
        
    }
    
    freeTree(My_root);      // freeing the memory :)

    //printf("\n");
    //time to check if add command is done correctly
    //in_order(My_root); // testing to see they are in order

    return 0;
}



//-----------------------------------------------(Functions)---------------------------------------------------------------------------------------------------------------//


void freeTree(Tree_node *root)
{
    if(root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}




//create a node
Tree_node *create_node(char name[], int fine)
{
    // allocate space for node
    Tree_node *temp = malloc(sizeof(Tree_node));        // allocate memory for the node 
    
    // fill up the data
    strcpy(temp->Veh_owner, name);
    temp->fine = fine;
    temp->left = NULL;
    temp->right = NULL;
    temp->depth = 0;



    //return node
    return temp;
}



//insertion
Tree_node *insertion(Tree_node *root, Tree_node *val, int CurrDepth)
{
    val->depth = CurrDepth;     // should track the depth of the node being added 

    if(root == NULL)
    {
        return val;
    }
    else
    {
        if(strcmp(root->Veh_owner, val->Veh_owner) < 0)     // this compares both names alphabetically      if big go right
        {
            if(root->right != NULL)
            {
                root->right = insertion(root->right, val, CurrDepth + 1);       // since its a new level being checked it will be adding to the depth
            }
            else    // is null
            {
                val->depth = CurrDepth + 1;     // if its null just add the current level

                root->right = val;      // means its a leaf so add to it
            }
        }
        else        // smaller than root go left
        {
            if(root->left != NULL)
            {
               root->left = insertion(root->left, val, CurrDepth + 1);
            }
            else    // is null
            {
                val->depth = CurrDepth + 1;     // if its null just add the current level
                root->left = val;   // means its a leaf so add to it
            }
        }
    }

    return root;
}



// traverse in order
void in_order(Tree_node *p)
{
    if (p != NULL)
    {
        in_order(p->left);
        printf("%s %d %d\n", p->Veh_owner, p->fine, p->depth);
        in_order(p->right);
    }
}



// searching for a node
Tree_node *search(Tree_node *root, char name[])
{
    if(root == NULL)
    {
        return NULL;        // its not there
    }

    if(root != NULL)
    {
        if(strcmp(root->Veh_owner, name) == 0)      // its a match 
        {
            return root;
        }

        // since its not in the beginning root it must be in the other level 
        
        if(strcmp(root->Veh_owner, name) < 0)       //name is bigger than the root go right
        {
            return search(root->right, name);
        }
        else
        {
            return search(root->left, name);        // name is less than the root go left
        }
    }
}



// add nodes
double amount_nodes(Tree_node *p)
{
    if(p != NULL)
    {
        return 1 + amount_nodes(p->left) + amount_nodes(p->right);      //adding all nodes
    }
    else
    {
        return 0;
    }
}



// add fines
double total_fines(Tree_node *p)
{
    if(p != NULL)
    {
        return p->fine + total_fines(p->left) + total_fines(p->right);
    }
    else
    {
        return 0;
    }
}



// gets the sum of all fines and the number of nodes
double averages(Tree_node *p)
{
    double total_nodes = amount_nodes(p);
    double Tot_fines = total_fines(p);

    double avg = Tot_fines / total_nodes;

    return avg;

}



// add up distance that are below name (i reused the total fines but changed it a bit for other cases)
double below_fines(Tree_node *p, char name[])
{
    if(p == NULL)
    {
        return 0;
    }

    if(strcmp(p->Veh_owner, name) <= 0) // owner less than or equal to name
    {
        return p->fine + below_fines(p->left, name) + below_fines(p->right, name); // add fine then goes to check left then later right
    }
    else
    {
        return below_fines(p->left, name);  // if the owner is big look at the left since right will always be bigger
    }
}



// used to see which side of the tree is higher 
int max(int x, int y)
{
    if(x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}



// calc the height
int calc_height(Tree_node* root)
{
    if(root == NULL)
    {
        return -1;      // if no node its -1
    }

    if(root != NULL)        // if theres a node continue and add 1 (first check which side is the max height)
    {
        
        return 1 + max(calc_height(root->left), calc_height(root->right));
    }
}
















// find the parent of the soon to be deleted node
Tree_node *parent(Tree_node *root, Tree_node *deleting)
{
    if(root == NULL || root == deleting)    // the soon to be deleted node is the only node in tree
    {
        return NULL;        // doesnt have parents
    }

    if(root->left == deleting || root->right == deleting)   // if the root is the parent 
    {
        return root;
    }

    if(strcmp(root->Veh_owner, deleting->Veh_owner) < 0)    // root is less go right
    {
        return parent(root->right, deleting);
    }
    else if (strcmp(root->Veh_owner, deleting->Veh_owner) > 0)  // root is bigger go left
    {
        return parent(root->left, deleting);
    }

    return NULL;
    
}



// check if its a leaf
int isLeaf(Tree_node *deleting)
{
    return (deleting->left == NULL && deleting->right == NULL);     // return 1 if leaf
}


// has left no right child
int hasOnlyLeftChild(Tree_node *deleting)
{
    return (deleting->left!= NULL && deleting->right == NULL);
}



// has right no left child
int hasOnlyRightChild(Tree_node *deleting)
{
    return (deleting->left == NULL && deleting->right != NULL);
}



// max value of a tree (using it for the left side)
Tree_node *MaxVal(Tree_node *root)
{
    if(root->right == NULL)
    {
        return root;
    }
    else
    {
        return MaxVal(root->right);
    }
}



// deletion when fines are 0 or less
Tree_node *delete(Tree_node *root, Tree_node *found)
{
    Tree_node *par, *save_node;
    par = parent(root, found);
    Tree_node *new_del_node;
    //char save_val [MAX];

    if(isLeaf(found))   // node being deleted is a leaf 
    {
        if(par == NULL) // the only node in tree
        {
            free(root);
            return NULL;
        }

        if(par->left == found)   // node is a left child
        {
            free(par->left);
            par->left = NULL;   // make the left null since node deleted
        }

        else if (par->right == found) // node is a right child
        {
            free(par->right);
            par->right = NULL;  // make right null since node deleted
        }

        return root;
    }

    if(hasOnlyLeftChild(found)) // case where node has left child
    {
        if(par == NULL)
        {
            save_node = found->left;
            free(found);
            return save_node;
        }

        if(par->left == found)       // delete node if it left child
        {
            save_node = par->left;
            par->left = par->left->left;        // adjust parent of the delted node to pick up the child of deleted

            free(save_node);
        }

        else if(par->right == found)      // deleted node if its right child
        {
            save_node = par->right;
            par->right = par->right->left;      //// adjust parent of the delted node to pick up the child of deleted

            free(save_node);
        }

        return root;
    }

    if(hasOnlyRightChild(found))    //case where node has right child
    {
        if(par == NULL)
        {
            save_node = found->right;
            free(found);
            return save_node;
        }

        if(par->left == found)       // delted node if it is a left child
        {
            save_node = par->left;
            par->left = par->left->right;
            free(save_node);
        }

        else if(par->right == found)  //deleted node if its a right child
        {
            save_node = par->right;
            par->right = par->right->right;
            free(save_node);
        }

        return root;
    }

    // with 2 childs
    
    new_del_node = MaxVal(found->left);         // max left 
    Tree_node save_val;
    strcpy(save_val.Veh_owner, new_del_node->Veh_owner);       // save the node 
    save_val.fine = new_del_node->fine;

    root = delete(root, new_del_node);

    strcpy(found->Veh_owner, save_val.Veh_owner);     // overwrite the node that was going to be deleted
    found->fine = save_val.fine;

    return root;
    
}



// since deletion was involved i need to update the depths
void new_depth(Tree_node *root, int current)
{

    if(root == NULL)
    {
        return; // theres nothing
    }
    else if(root != NULL)       // preorder traversal 
    {
        root->depth = current;
        new_depth(root->left, current + 1);
        new_depth(root->right, current + 1);

    }
    
}

//-----------------------------------------------(END OF FUNCTIONS)---------------------------------------------------------------------------------------------------------------//
