/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32170171
*	    Student name : hyunwoo koh
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"
int counter = 0;
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
void lab2_inorder_print(lab2_node *node){
    // use for inorder
    if(!node)
	    return;
    lab2_inorder_print(node->left);
    counter++;
    lab2_inorder_print(node->right);
}
void lab2_inorder_delete(lab2_node *node){
    // use for tree delete
    if(!node)
	    return;
    lab2_node *temp = node->right;
    lab2_inorder_delete(node->left);
    lab2_node_delete(node);
    lab2_inorder_delete(temp);
}
int lab2_node_print_inorder(lab2_tree *tree) {
    // You need to implement lab2_node_print_inorder function.
    counter = 0;
    if( tree->root)
	    lab2_inorder_print(tree->root);
    printf("%d nodes is in the tree\n",counter);
    
    return LAB2_SUCCESS;
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
    lab2_tree* tree = (lab2_tree*)malloc(sizeof(lab2_tree));
    tree -> root = NULL;
    pthread_mutex_init(&root_lock,NULL);
    pthread_mutex_init(&tree_lock,NULL);
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
    lab2_node* node=(lab2_node*)malloc(sizeof(lab2_node));
    pthread_mutex_init(&node->mutex,NULL);
    node->key = key;
    node->right = node->left = 0;
    return node;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
    if( !(tree->root))
	tree->root = new_node;
    else
    {
	    lab2_node *temp = tree->root, *temp_par = 0;

	    while(temp)
	    {
		    temp_par = temp;
		    if(temp->key < new_node->key)
			    temp = temp->right;
		    else if(temp->key > new_node->key) 
			    temp = temp->left;
		    else return LAB2_ERROR;
	    }
	    if(temp_par->key < new_node->key)
		    temp_par->right = new_node;
	    else
		    temp_par->left = new_node;
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_fg function.
    if( !(tree->root) )
    {
	    pthread_mutex_lock(&root_lock); //use double check lock
	    if( !(tree->root) )
	    {
		    tree->root = new_node;
		    pthread_mutex_unlock(&root_lock);
		    return LAB2_SUCCESS;
	    }
	    else
		    pthread_mutex_unlock(&root_lock);
    }
    lab2_node *temp = tree->root, *temp_par = 0;
    
    pthread_mutex_lock(&temp->mutex);
    while(1)
    {
	    temp_par = temp;
	    if( new_node->key > temp->key && temp->right)
	    {
		    pthread_mutex_lock(&temp->right->mutex);
		    temp = temp->right;
		    pthread_mutex_unlock(&temp_par->mutex);
	    }
	    else if( new_node->key < temp->key && temp->left)
	    {
		    pthread_mutex_lock(&temp->left->mutex);
		    temp = temp->left;
		    pthread_mutex_unlock(&temp_par->mutex);
	    }
	    else if( new_node->key == temp->key)
	    {
		    pthread_mutex_unlock(&temp_par->mutex);
		    return LAB2_ERROR;
	    }
	    else break;
    }
    //loop end -> temp_par is locked
    if( new_node->key > temp_par->key )
	    temp_par->right = new_node;
    else
	    temp_par->left = new_node;
    pthread_mutex_unlock(&temp_par->mutex);
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
    int check = 0;
    pthread_mutex_lock(&tree_lock);
    check = lab2_node_insert(tree,new_node);
    if(check == LAB2_SUCCESS){
	   pthread_mutex_unlock(&tree_lock);
	   return LAB2_SUCCESS;
    }
    else
    {
	    pthread_mutex_unlock(&tree_lock);
	    return LAB2_ERROR;
    }
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove function.
    lab2_node *del_node = tree->root, *del_node_par = 0;
    while(del_node)
    {
	    if( key == del_node->key)
	    {
		   break;
	    }
	    del_node_par = del_node;
	    if( key > del_node->key)
		    del_node = del_node->right;
	    else if(key < del_node->key)
		    del_node = del_node->left;
    }
    if(!del_node)
    {
	    return LAB2_ERROR;
    }
    if( (del_node->left) && (del_node->right)) //the node has two child on both side
    {	
	    lab2_node *temp_par = del_node;
	    lab2_node *temp = del_node->left;

	    while(temp->right)
	    {
		    temp_par = temp;
		    temp = temp->right;
	    }
	    del_node->key = temp->key;
	    if( temp_par->left == temp)
	    {
	    	if(temp->left)
			    temp_par->left = temp->left;
	    	else 
			    temp_par->left = 0;
	    }
            else
	    {
		   if(temp->left)
			  temp_par->right = temp->left;
		   else
			  temp_par->right = 0;
	    }
	    free(temp);
    }
    else if( !(del_node->left) && !(del_node->right) )//both child is empty!
    {
	if(del_node == tree->root) tree->root = 0;
	else
	{
		if(del_node_par->right == del_node)
			del_node_par->right = 0;
		else
			del_node_par->left = 0;
	}
	free(del_node);
    }
    else if( !(del_node->left) && (del_node->right) )//there is only one child in right side
    {
	if(del_node == tree->root) tree->root = del_node->right;
	else
	{
		if(del_node_par->right == del_node)
			del_node_par->right = del_node->right;
		else
			del_node_par->left = del_node->right;
	}
	free(del_node);
    }
    else if( (del_node->left) && !(del_node->right) )//there is only one child in left side
    {
	    if(del_node == tree->root) tree->root = del_node->left;
	    else
	    {
		    if(del_node_par->right == del_node)
			    del_node_par->right = del_node->left;
		    else
			    del_node_par->left = del_node->left;
	    }
	    free(del_node);
    }
    //printf(" . ");
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
    lab2_node *del_node, *del_node_par;
    if( tree->root ) //first check
    {
	    pthread_mutex_lock(&root_lock);
	    if( tree->root ) //double check
	    {	
		    del_node = tree->root;
		    del_node_par = 0;
		    pthread_mutex_lock(&del_node->mutex);
		    pthread_mutex_unlock(&root_lock);
	    }
	    else if( !(tree->root) ) //root is NULL at double check
	    {
		    pthread_mutex_unlock(&root_lock);
		    return LAB2_ERROR;
	    }
	    else
	    {
		    return LAB2_ERROR;
	    }
    }
    else if( !(tree->root))//root is NULL at first check
    {
	    pthread_mutex_lock(&root_lock);
	    if( !(tree->root) )
	    {
		    pthread_mutex_unlock(&root_lock);
		    return LAB2_ERROR;
	    }
    }
    while(del_node)
    {
	    //we have two lock two node, one is the node which has the key and the other is its parent
	    if( key == del_node->key)
			    break;
	    if( key > del_node->key)
	    {
		    if(del_node->right)
		    {
		    	if(del_node != tree->root) //when del_node_par is NULL
				pthread_mutex_unlock(&del_node_par->mutex);
			del_node_par = del_node;
			pthread_mutex_lock(&del_node->right->mutex);//lock the node which del_node will move
			del_node = del_node->right;//del_node move
		    }
		    else
		    {
			    if(del_node == tree->root)
			    {
			        //del_node_par is NULL
				pthread_mutex_unlock(&del_node->mutex);
				del_node = 0;
			    	break;
			    }
			    else
			    {
				    //del_node_par is not NULL
				    pthread_mutex_unlock(&del_node_par->mutex);
				    pthread_mutex_unlock(&del_node->mutex);
				    del_node = 0;
				    del_node_par = 0;
				    break;
			    }
		    }
		    
	    }
	    else if( key < del_node->key)
	    {
		    if(del_node->left)
		    {
			if(del_node != tree->root)
				pthread_mutex_unlock(&del_node_par->mutex);
		    	del_node_par = del_node;
			pthread_mutex_lock(&del_node->left->mutex);
		    	del_node = del_node->left;
		    }
		    else
		    {
			    if(del_node == tree->root)
			    {
				    pthread_mutex_unlock(&del_node->mutex);
				    del_node = 0;
				    break;
			    }
			    else
			    {
			    	pthread_mutex_unlock(&del_node->mutex);
			    	pthread_mutex_unlock(&del_node_par->mutex);
			    	del_node = 0;
			    	del_node_par = 0;
			    	break;
			    }
		    }
	    }
	    else break;
    }
    //end finding del_node which has the same key
    if( !(del_node) )
    {
	    return LAB2_ERROR; //if there isn't node which has the same key
    }
    //starting removing node
    if( del_node->left && del_node->right) // del_node's sub tree is also locked
    {
	    //now, del_node is locked and del_node_par is unlocked
	lab2_node *temp = del_node->left, *temp_par = del_node;
	
	while(temp->right)
	{
		temp_par = temp;
		temp = temp->right;
	}
	//found the biggest node in left sub tree
	del_node->key = temp->key; //chage the key value
	
	pthread_mutex_lock(&temp->mutex);
	if( temp_par != del_node) //skip if skip the while loop (when, temp_par == del_node)
	{
		pthread_mutex_lock(&temp_par->mutex);//we are going to delete temp
		pthread_mutex_unlock(&del_node->mutex);
	}

	if(del_node_par) //skip when del_node is root
		pthread_mutex_unlock(&del_node_par->mutex);
	/*
	 * if control means that
	 * if one thread locked del_node, and the other thread come afther, and wait for it unlocked
	 * but, the del_node was changed( delete or key value change -> make some error
	 * */
	
	//delete the largest node in left sub thre
	if( temp_par->left == temp)
        {
                if(temp->left)
                            temp_par->left = temp->left;
                else
                            temp_par->left = 0;
        }
        else
        {                  
	       	if(temp->left)
               		temp_par->right = temp->left;
	    	else  
	  		temp_par->right = 0;
         }

	pthread_mutex_unlock(&temp->mutex); //temp_par is still locked
	free(temp);
	pthread_mutex_unlock(&temp_par->mutex);
    }
    else if( !(del_node->left) && !(del_node->right) )//there is no child
    {
	    if( del_node == tree->root) //del_node is locked, but del_node_par is unlocked(NULL)
	    {	    
		    pthread_mutex_lock(&root_lock);
		    if( !(tree->root) )
		    {
			    pthread_mutex_unlock(&root_lock);
			    return LAB2_ERROR;
		    }
		    tree->root = 0;
		    pthread_mutex_unlock(&del_node->mutex);
		    free(del_node);
		    pthread_mutex_unlock(&root_lock);
	    }
	    else //del_node and del_node_par is locked
	    {
		    if( del_node_par->left == del_node)
		    	del_node_par->left = 0;
		    else
			del_node_par->right = 0;

		    pthread_mutex_unlock(&del_node->mutex);
		    free(del_node);
		    pthread_mutex_unlock(&del_node_par->mutex);
	    }
    }
    else if( (del_node->left) && !(del_node->right) ) 
    {
	    if( del_node == tree->root) //del_node is locked, but del_node_par is unlocked(NULL)
	    {
		    pthread_mutex_lock(&root_lock);
		    if( !(tree->root) )
		    {
			    pthread_mutex_unlock(&root_lock);
			    return LAB2_ERROR;
		    }
		    tree->root = del_node->left;
		    pthread_mutex_unlock(&del_node->mutex);
		    free(del_node);
		    pthread_mutex_unlock(&root_lock);
	    }
	    else
	    {
		    if(del_node_par->left == del_node) //del_node and del_node_par is locked
			    del_node_par->left = del_node->left;
		    else
			    del_node_par->right = del_node->left;
		    pthread_mutex_unlock(&del_node->mutex);
		    free(del_node);
		    pthread_mutex_unlock(&del_node_par->mutex);
	    }
    }
    else if( !(del_node->left) && (del_node->right) )
    {
	    if(del_node == tree->root)
	    {
		    pthread_mutex_lock(&root_lock);
		    if(!(tree->root))
		    {
			    pthread_mutex_unlock(&root_lock);
			    return LAB2_ERROR;
		    }
		    tree->root = del_node->right;
		    pthread_mutex_unlock(&del_node->mutex);
		    free(del_node);
		    pthread_mutex_unlock(&root_lock);
	    }
	    else
	    {
		    if(del_node_par->left == del_node)
			    del_node_par->left = del_node->right;
		    else
			    del_node_par->right = del_node->right;
		    pthread_mutex_unlock(&del_node->mutex);
		    free(del_node);
		    pthread_mutex_unlock(&del_node_par->mutex);
	    }
    }
    else return LAB2_ERROR;
    //printf(" . ");
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
    pthread_mutex_lock(&tree_lock);
    int check = lab2_node_remove(tree,key);
    if( check == LAB2_SUCCESS)
    {
	    pthread_mutex_unlock(&tree_lock);
	    return LAB2_SUCCESS;
    }
    else
    {
	    pthread_mutex_unlock(&tree_lock);
	    return LAB2_ERROR;
    }
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
    if( !(tree->root) )
	    return;
    else
    {
	    lab2_inorder_delete(tree->root);
	    tree->root = 0;
	    return;
    }
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
    	node->left = 0;
	node->right = 0;
	free(node);
    return;
}

