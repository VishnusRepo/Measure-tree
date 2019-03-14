#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
struct m_tree_t{
  int key,leftmin,rightmax,height,linterval,rinterval, measure;
  struct m_tree_t* left, *right;
};

struct intervalNode{
  int leftValue, rightValue;
  struct intervalNode* next;
};

struct Stack{
  struct m_tree_t* arr[128];
  int index;
};

struct m_tree_t* top(struct Stack* s){
  if(s->index==0) return NULL;
  return s->arr[s->index-1];
}

void push(struct Stack* s,struct m_tree_t* node){
  if(s->index>127){
    printf("Stack full");
    return;
  }
  s->arr[s->index]=node;
  s->index++;
}

void pop(struct Stack *s){
  //struct m_tree_t* curr=s->arr[s->index-1];
  s->index--;
  //free(curr);
}

void left_rotation(struct m_tree_t   *n){
     struct m_tree_t   *tmp_node;
     int   tmp_key;
     tmp_node = n->left;
     tmp_key = n->key;
     n->left = n->right;
     n->key = n->right->key;
     n->right = n->left->right;
     n->left->right = n->left->left;
     n->left->left = tmp_node;
     //n->left->key = n->left->left->key + n->left->right->key;
     n->left->linterval = n->linterval;
	   n->left->rinterval = n->key;
     n->left->key = tmp_key;
     n->left->leftmin = n->left->left->leftmin<n->left->right->leftmin?n->left->left->leftmin:n->left->right->leftmin;
	   n->left->rightmax = n->left->left->rightmax>n->left->right->rightmax?n->left->left->rightmax:n->left->right->rightmax;
     n->left->measure = calculate_measure(n->left);
}
void right_rotation(struct m_tree_t* n){
    struct m_tree_t   *tmp_node;
    int   tmp_key;
    tmp_node = n->right;
    tmp_key = n->key;
    n->right = n->left;
    n->key = n->left->key;
    n->left = n->right->left;
    n->right->left = n->right->right;
    n->right->right = tmp_node;
    n->right->key = tmp_key;
    n->right->linterval=n->key;
	  n->right->rinterval=n->rinterval;
    n->right->key=tmp_key;
    n->right->leftmin=n->right->left->leftmin<n->right->right->leftmin?n->right->left->leftmin:n->right->right->leftmin;
	  n->right->rightmax=n->right->left->rightmax>n->right->right->rightmax?n->right->left->rightmax:n->right->right->rightmax;
    n->right->measure=calculate_measure(n->right);
}


int calculate_measure(struct m_tree_t *curr) {
	if (curr->right == NULL) {
    int left = curr->rinterval<curr->rightmax?curr->rinterval:curr->rightmax;
    int right = curr->linterval>curr->leftmin?curr->linterval:curr->leftmin;
    return left-right;
	} else {
		if ((curr->right->leftmin < curr->linterval) && (curr->left->rightmax >= curr->rinterval)) {
			return curr->rinterval - curr->linterval;
		} else if ((curr->right->leftmin >= curr->linterval) && (curr->left->rightmax >= curr->rinterval)) {
			return (curr->rinterval - curr->key) + curr->left->measure;
		} else if ((curr->right->leftmin < curr->linterval) && (curr->left->rightmax < curr->rinterval)) {
			return curr->right->measure + (curr->key - curr->linterval);
		} else if ((curr->right->leftmin >= curr->linterval) && (curr->left->rightmax < curr->rinterval)) {
			return curr->right->measure + curr->left->measure;
		}
	}
	return -1;
}

struct m_tree_t* create_m_tree(){
  struct m_tree_t* tree_node = (struct m_tree_t*) malloc(sizeof(struct m_tree_t));
  tree_node->key=0;
  tree_node->leftmin=0;
  tree_node->rightmax=0;
  tree_node->height=0;
  tree_node->linterval=INT_MIN;
  tree_node->rinterval=INT_MAX;
  tree_node->measure=0;
  tree_node->left=NULL;
  tree_node->right=NULL;
  return tree_node;
}



void insertFirstNode(struct m_tree_t* curr, int a, int b){
  struct intervalNode* curr_interval = (struct intervalNode*)malloc(sizeof(struct intervalNode));
  curr_interval->leftValue=a<b?a:b;
  curr_interval->rightValue=a>b?a:b;
  curr_interval->next=NULL;
  curr->left=(struct m_tree_t*)curr_interval;
  curr->right=NULL;
  curr->height=0;
  curr->key=a;
  curr->leftmin=curr_interval->leftValue;
  curr->rightmax=curr_interval->rightValue;
  curr->measure=calculate_measure(curr);
}

struct m_tree_t* treeTraverse(struct Stack* traverseStack,struct m_tree_t* curr,int key){
  while(curr->right!=NULL){
      printf("treeTraverse\n");
    push(traverseStack,curr);
    if(curr->key>key) curr=curr->left;
    else curr=curr->right;
  }
  return curr;
}

void insertNode(struct m_tree_t* curr, int a, int b){
    printf("insertNode: %d,%d\n",a,b);
  if(curr->left==NULL){
    //It means tree is empty. Hence, insert its first node.
    insertFirstNode(curr,a,b);
    return;
  }
  struct Stack* traverseStack;
  traverseStack->index=0;
  struct m_tree_t* leafNode=treeTraverse(traverseStack,curr,a);
  int traversalheight=traverseStack->index;
  if(leafNode->key!=a){
    //Need to create new nodes
    printf("before error\n");
    struct m_tree_t* new_node=(struct m_tree_t*)create_m_tree();
    printf("after error\n");
    struct m_tree_t* old_node=(struct m_tree_t*)create_m_tree();
    old_node->left=leafNode->left;
    old_node->right=NULL;
    old_node->leftmin=leafNode->leftmin;
    old_node->rightmax=leafNode->rightmax;
    old_node->key=leafNode->key;
    old_node->height=0;
    new_node->leftmin=a;
    new_node->rightmax=b;
    struct intervalNode* new_interval = (struct intervalNode*)malloc(sizeof(struct intervalNode));
    new_interval->leftValue=a<b?a:b;
    new_interval->rightValue=a>b?a:b;
    new_interval->next=NULL;
    new_node->left=(struct m_tree_t*)new_interval;
    new_node->right=NULL;
    new_node->height=0;
    if(leafNode->key>=a){
      leafNode->left=new_node;
      leafNode->right=old_node;
      old_node->linterval=new_node->rinterval=leafNode->key;
      old_node->rinterval=leafNode->rinterval;
      new_node->linterval=leafNode->linterval;
      old_node->measure=calculate_measure(old_node);
      new_node->measure=calculate_measure(new_node);
    }else{
      leafNode->left=old_node;
      leafNode->right=new_node;
      leafNode->key=a;
      old_node->rinterval=new_node->linterval=leafNode->key;
      old_node->linterval=leafNode->linterval;
      new_node->rinterval=leafNode->rinterval;
      old_node->measure=calculate_measure(old_node);
      new_node->measure=calculate_measure(new_node);
    }
    leafNode->leftmin=leafNode->left->leftmin<leafNode->right->leftmin?leafNode->left->leftmin:leafNode->right->leftmin;
    leafNode->rightmax=leafNode->left->rightmax>leafNode->right->rightmax?leafNode->left->rightmax:leafNode->right->rightmax;
    leafNode->height=1;
    leafNode->measure=calculate_measure(leafNode);

    //pop elements of stack to traverse back and update measure, leftmin, rightmax
    while(traverseStack->index>0){
        printf("pop to update measure\n");
      struct m_tree_t* temp_node = top(traverseStack);
      pop(traverseStack);
      temp_node->leftmin=temp_node->left->leftmin<temp_node->right->leftmin?temp_node->left->leftmin:temp_node->right->leftmin;
      temp_node->rightmax=temp_node->left->rightmax>temp_node->right->rightmax?temp_node->left->rightmax:temp_node->right->rightmax;
      temp_node->measure = calculate_measure(temp_node);
    }
    //Resetting stack to traversed size to reiterate.
    traverseStack->index=traversalheight;

    while(traverseStack->index>0){
        printf("pop to rotate\n");
      struct m_tree_t* current = top(traverseStack);
      pop(traverseStack);
      int tmp_height, old_height;
//        tmp_node = pop();
        old_height = current->height;
	//The four cases below are referred from lecture notes of Dr. Kemafor.
        if( current->left->height - current->right->height == 2 ){
                if( current->left->left->height - current->right->height == 1 ){
                    right_rotation( current );
                current->right->height = current->right->left->height + 1;
                current->height  = current->right->height + 1;
                }
            else{
                left_rotation( current->left );
                right_rotation( current );
                tmp_height = current->left->left->height;
                current->left->height = tmp_height + 1;
                current->right->height = tmp_height + 1;
                current->height = tmp_height + 2;
            }
        }
        else if(current->right->height - current->left->height == 2){
            if( current->right->right->height - current->left->height == 1 ){
                left_rotation( current );
                current->left->height = current->left->right->height + 1;
                current->height  = current->left->height + 1;
            }
            else{
                right_rotation( current->right );
                left_rotation( current );
                tmp_height = current->right->right->height;
                current->right->height = tmp_height + 1;
                current->left->height = tmp_height + 1;
                current->height = tmp_height + 2;
            }
        }else{
          current->height=(current->left->height>current->right->height)?current->left->height+1:current->right->height+1;
        }
     }
  }else{
    //current insertion a value equates to key of a leaf node
    struct intervalNode* currptr = (struct intervalNode*)leafNode->left;
    while(currptr->next!=NULL) currptr=currptr->next;
    struct intervalNode* newptr = (struct intervalNode*)malloc(sizeof(struct intervalNode));
    newptr->leftValue=a<b?a:b;
    newptr->rightValue=a>b?a:b;
    newptr->next=NULL;
    currptr->next=newptr;

    leafNode->leftmin=(leafNode->leftmin<a)?leafNode->leftmin:a;
    leafNode->rightmax=(leafNode->rightmax>b)?leafNode->rightmax:b;
    leafNode->measure=calculate_measure(leafNode);

    while(traverseStack->index>0){
        printf("pop to update measure2\n");
      struct m_tree_t* temp_node = top(traverseStack);
      pop(traverseStack);
      temp_node->leftmin=temp_node->left->leftmin<temp_node->right->leftmin?temp_node->left->leftmin:temp_node->right->leftmin;
      temp_node->rightmax=temp_node->left->rightmax>temp_node->right->rightmax?temp_node->left->rightmax:temp_node->right->rightmax;
      temp_node->measure = calculate_measure(temp_node);
    }
  }
}

int query_length(struct m_tree_t* tree){
  return tree->measure;
}

void insert_interval(struct m_tree_t* tree, int a, int b){
  //Peforms two inserts as we need to insert interval twice, based on both a and b.
  insertNode(tree,a,b);
  insertNode(tree,b,a);
}

int main()
{  int i; struct m_tree_t *t; ;
   printf("starting \n");
   t = create_m_tree();
   for(i=0; i< 50; i++ )
      insert_interval( t, 2*i, 2*i +1 );
   printf("inserted first 50 intervals, total length is %d, should be 50.\n", query_length(t));
      insert_interval( t, 0, 100 );
   printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
   for(i=1; i< 50; i++ )
     insert_interval( t, 199 - (3*i), 200 ); /*[52,200] is longest*/
   printf("inserted further 49 intervals, total length is %d, should be 200.\n", query_length(t));
   /*for(i=2; i< 50; i++ )
     delete_interval(t, 2*i, 2*i +1 );
   delete_interval(t,0,100);
   printf("deleted some intervals, total length is %d, should be 150.\n", query_length(t));
   insert_interval( t, 1,2 );
   for(i=49; i>0; i-- )
     delete_interval( t, 199 - (3*i), 200 );
   insert_interval( t, 0,2 );
   insert_interval( t, 1,5 );
   printf("deleted some intervals, total length is %d, should be 5.\n", query_length(t));
   insert_interval( t, 0, 100 );
   printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
   for(i=0; i<=3000; i++ )
      insert_interval( t, 2000+i, 3000+i );
   printf("inserted 3000 intervals, total length is %d, should be 4100.\n", query_length(t));
   for(i=0; i<=3000; i++ )
     delete_interval( t, 2000+i, 3000+i );
   printf("deleted 3000 intervals, total length is %d, should be 100.\n", query_length(t));
   for(i=0; i<=100; i++ )
      insert_interval( t, 10*i, 10*i+100 );
   printf("inserted another 100 intervals, total length is %d, should be 1100.\n", query_length(t));
   delete_interval( t, 1,2 );
   delete_interval( t, 0,2 );
   delete_interval( t, 2,3 );
   delete_interval( t, 0,1 );
   delete_interval( t, 1,5 );
   printf("deleted some intervals, total length is %d, should be still 1100.\n", query_length(t));
   for(i=0; i<= 100; i++ )
     delete_interval(t, 10*i, 10*i+100);
   delete_interval(t,0,100);
   printf("deleted last interval, total length is %d, should be 0.\n", query_length(t));
   for( i=0; i<100000; i++)
   {  insert_interval(t, i, 1000000);
   }
   printf("inserted again 100000 intervals, total length is %d, should be 1000000.\n", query_length(t));
   printf("End Test\n");*/
}
