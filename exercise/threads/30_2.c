#include<pthread.h>
#include"../lib/tlpi_hdr.h"

//二叉搜索树节点
struct node{
	pthread_mutex_t mutex;
	int key;
	char value;
	struct node *lchild;
	struct node *rchild;
};

static struct node* root = NULL;

void initializer(struct node* nd){
		root = (struct node*) malloc(sizeof(struct node));
		root->lchild = root->rchild = NULL;
		root->key = nd->key;
		root->value = nd->value;

		int s;
		if((s = pthread_mutex_init(&root->mutex, NULL)) != 0) errExitEN(s, "pthread_mutex_init");
}

void add(struct node* nd){
	if(nd == NULL) return;
	
	if(root == NULL){
		initializer(nd);
		return;
	}

	struct node* p = root;
	while(p != NULL){
		struct node* pp = p;
		int s;

		if(nd->key < p->key)
			p = p->lchild;
		else
			p = p->rchild;
		if(p == NULL){
			//对pp节点进行修改
			s = pthread_mutex_lock(&pp->mutex);
			if(s == -1) errExitEN(s, "pthread_mutex_lock");
			
			if(nd->key < pp->key){
			    pp->lchild = nd;
		    	    pp->rchild = NULL;
			}else{
			    pp->rchild = nd;
	    		    pp->lchild = NULL;
			}		    
			s = pthread_mutex_unlock(&pp->mutex);
			if(s == -1) errExitEN(s, "pthread_mutex_unlock");
		}
	}
}

//查找
Boolean find(struct node* nd, int key, int* value){
	if(nd == NULL) return FALSE;
	int s;
	s = pthread_mutex_lock(&nd->muetx);
	if(s != 0) errExitEN(s, "pthread_mutex_lock");
	if(nd->key == key){
		*value = nd->value;
	s = pthread_mutex_unlock(&nd-mutex);
	if(s != 0) errExitEN(s, "pthread_mutex_unlock");
		return TRUE;
	}
	if(!find(nd->lchild, key, value) && !find(nd->rchild, key, value)){
		s = pthread_mutex_unlock(&nd-mutex);
		if(s != 0) errExitEN(s, "pthread_mutex_unlock");
		return FALSE;
	}
	else{
		s = pthread_mutex_unlock(&nd-mutex);
		if(s != 0) errExitEN(s, "pthread_mutex_unlock");
		return TRUE;
	}
}
Boolean lookup(int key, int* value){
	return find(root, key, value);
}


void delete(struct node*& nd, int key){
	struct node* p = nd,
		   * pp = NULL;
	int s;
	s = pthread_mutex_lock(&nd->mutex);
	if(s != 0) errExitEN(s, "pthread_mutex_lock");

	while(p != NULL && p->key != key){
		pp = p;
		if(nd->key < key)
			p = p->lchild;
		else
			p = p->rchild;
	}
	//没有找到对应关键字的节点
	if(p == nullptr) return;
	
	struct node* cur = NULL, *parent = p;
	if(p->lchild != NULL){
		//在左子树中寻找最大元素
		cur = p->lchild;
		while(cur->rchild != NULL){
			parent = cur;
			cur = cur->rchild;
		}
		if(parent != p)
			parent->rchild = cur->lchild;	//cur可能有左孩子可能没有，但是都可以令preant->rchild指向它的左孩子
		else
			parent->lchild = NULL;		//如果左子树就一个节点，那么直接改变p的左孩子
		
		p->val = cur->val;
		p->key = cur->key;
	}
	else if(p->rchild != NULL){
		//在右子树中寻找最小元素
		cur = p->rchild
			while(cur->lchild != NULL){
				panret = cur;
				cur = cur->lchild;
			}
		if(parent != p)
			parent->lchild = cur->rchild;
		else
			parent->rchild = NULL;
		
		p->val = cur->val;
		p->key = cur->key;
	}else{
		//左右子树均为空
		if(pp != NULL)
			(pp->lchild == p) ? (pp-lchild = NULL) : (pp->rchild = NULL);
	}
		if(pp == NULL) nd = NULL;	//如果这棵树自身的根节点被删掉了，那么将它置为空
	
	s = pthread_mutex_unlock(&nd->mutex);
	if(s != 0) errExitEN(s, "pthread_mutex_unlock");
}



//对整棵树进行前序遍历，由于是只读访问，所以不需要进行锁操作
void preOrderTravel(struct node* nd){
	if(nd == NULL) return;
	printf("%d ", nd->value);
	preOrderTravel(nd->lchild);
	preOrderTravel(nd->rchild);
}

int main(int argc, char **argv){
	

	preOrderTravel(root);
	/*struct node nd;
	nd.key = 1;
	nd.value = 'A';
	nd.lchild = nd.rchild = NULL;

	add(&nd);

	printf("%c\n", root->value);*/
	return 0;

}
