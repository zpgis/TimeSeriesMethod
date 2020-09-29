/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  红黑树
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-09-21
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/
#ifndef GT_RBTREE_H_INCLUDE
#define GT_RBTREE_H_INCLUDE

#include "gts_cg_port.h"

namespace gts{

template<class T>
class GTRBTreeNode
{
public:
	GTRBTreeNode(/*T& ele, */bool c=true, GTRBTreeNode<T> *p=NULL,
		GTRBTreeNode<T> *l=NULL,GTRBTreeNode<T> *r=NULL)
		:color(c),/*elem(ele),*/parent(p),left(l),right(r){}
		T elem;
		bool color; //红为true, 黑为false
		GTRBTreeNode<T> *parent;
		GTRBTreeNode<T> *left;
		GTRBTreeNode<T> *right;
    ~GTRBTreeNode(void){left = right = parent = NULL;}
		GTRBTreeNode<T>& operator = (const GTRBTreeNode<T>& _node)
		{	
			color  = _node.color;
			elem   = _node.elem;
			parent = _node.parent;
			left   = _node.left;
			right  = _node.right;
			return *this;
		}

		bool operator == (const GTRBTreeNode& _node) const
		{
			return ((color  == _node.color) && (elem  == _node.elem)  &&
				    (left   == _node.left)  && (right == _node.right) &&
					(parent == _node.parent));
		}
};

template<class T>
class GTRBTree
{
protected:
	GTRBTreeNode<T>* root;
	GTRBTreeNode<T>* nil;
	int count;

public:
	GTRBTree(/*GTRBTreeNode<T>* p=NULL*/)
		:root(NULL), count(0)
	{
		nil = new GTRBTreeNode<T>(false,NULL,NULL,NULL);
		nil->left=nil->right=nil->parent = NULL;
	}

	GTRBTreeNode<T>* GetRoot()const{ return root;}
	bool Empty() const {return root == NULL;}
	int GetNodeCount() const {return count;}
	~GTRBTree(void)
	{
		ClearTree(); 
		root= NULL;
		if(nil != NULL)
		{
			delete nil;
			nil = NULL;
		}
	}
	//清楚树上的所有节点
	void ClearTree()
	{
		if(root == NULL)
			return;

		GTRBTreeNode<T> *pTemp;
		GTRBTreeNode<T> *node = root;

		while(node != nil)
		{
			if(node->left != nil)
				node = node->left;
			else	
				if(node->right != nil)
					 node = node->right;
				else
				{
					pTemp = node;
					if(node->parent != nil)
					{
						//降待删除节点的父接点指向其指针置为nil
						if(node == node->parent->left)
							node->parent->left = nil;
						if(node == node->parent->right)
							node->parent->right = nil;
					}
					node = node->parent;
					delete pTemp;
				}
		}
		count = 0;
		root  = NULL;
	}

	//寻找子树中最大的节点（最右侧下端）
	inline GTRBTreeNode<T>* MaxNode(GTRBTreeNode<T>* _tree)
	{
		GTRBTreeNode<T>* _result = _tree;
		while(_result->right != nil) 
		{
			_result = _result->right;
		}
		return _result;
	}

	//寻找子树中最小的节点（最左侧下端）
	inline GTRBTreeNode<T>* MinNode(GTRBTreeNode<T>* _tree)
	{
		GTRBTreeNode<T>* _result = _tree;
		while(_result->left != nil) 
		{
			_result = _result->left;
		}
		return _result;
	}

	//一个节点在中序遍历的下一个节点
	inline GTRBTreeNode<T>* NextInorder(GTRBTreeNode<T>* _node)
	{
		GTRBTreeNode<T>* _result;
		if(_node->right != nil)
		{
			_result = MinNode(_node->right);
		}
		else
		{
			_result = _node->parent;
			GTRBTreeNode<T>* temp = _node;
			while (_result != nil && temp == _result->right) 
			{
				temp    = _result;
				_result = _result->parent;
			}
		}
		return _result;
	}

	//一个节点在中序遍历的前一个节点
	inline GTRBTreeNode<T>* PreInorder(GTRBTreeNode<T>* _node)
	{
		GTRBTreeNode<T>* _result;
		if (_node->left != nil) 
		{
			_result = MaxNode(_node->right);
		}
		else 
		{
			_result = _node->parent;
			GTRBTreeNode<T>* temp = _node;
			while (_result != nil && temp == _result->left) 
			{
				temp    = _result;
				_result = _result->parent;
			}
		}
		return _result;
	}

	//插入一个节点
	inline GTRBTreeNode<T>* InsertElem(const T& _ele)
	{
		GTRBTreeNode<T>* x=root,*y=NULL;
		if(!x)
		{
			x = root = new GTRBTreeNode<T>(/*_ele,*/true,nil,nil,nil);
			x->elem = root->elem = _ele;
			root->parent=nil;
		}
		else
		{
			while(x!=nil)
			{
				y=x;
				if(_ele > x->elem)
					x=x->right;
				else
					x=x->left;
			}
			x = new GTRBTreeNode<T>(/*_ele,*/true,nil,nil,nil);
			x->elem = _ele;
			x->parent=y;
			if(_ele > y->elem)
				y->right=x;
			else
				y->left=x;
		}

		while (x->parent->color==true)
		{
			if(x->parent==x->parent->parent->left)
			{
				if(x->parent->parent->right->color==true)
				{
					x->parent->color=false;
					x->parent->parent->right->color=false;
					x->parent->parent->color=true;
					x=x->parent->parent;
				}
				else 
				{
					if (x==x->parent->right)
					{
						x=x->parent;
						LeftRotate(x);
					}
					x->parent->color=false;
					x->parent->parent->color=true;
					RightRotate(x->parent->parent);
				}
			}
			else
			{
				if(x->parent->parent->left->color==true)
				{
					x->parent->color=false;
					x->parent->parent->left->color=false;
					x->parent->parent->color=true;
					x=x->parent->parent;
				}
				else 
				{
					if (x==x->parent->left)
					{	
						x=x->parent;
						RightRotate(x);
					}
					x->parent->color=false;
					x->parent->parent->color=true;
					LeftRotate(x->parent->parent);
				}
			}
		}
		root->color=false;
		count++;
		return x;//liuyan add at 2011-9-28
	}

	//按照关键字进行搜索
	inline GTRBTreeNode<T>* Search(const T& _ele)
	{
		GTRBTreeNode<T>* p = root;
		while(p && p->elem != _ele)
		{
			if(p->elem> _ele)
				p=p->left;
			else
				p=p->right;
		}
		return p == nil? NULL:p;
	}

	//删除一个节点
	inline void DeleteNode(GTRBTreeNode<T>* _node)
	{
		if(_node == NULL)
			return;
		if(_node == nil)
			return;
		GTRBTreeNode<T>* toDel = _node;
		if (_node->left != nil && _node->right != nil) 
		{
			toDel = NextInorder(_node);
		}

		GTRBTreeNode<T>* temp = toDel;
		while (temp->parent != nil) 
		{
			temp = temp->parent;
		}

		GTRBTreeNode<T>* replace = toDel->left != nil? toDel->left: toDel->right;
		replace->parent = toDel->parent;
		if (replace->parent == nil) 
		{
			root = replace;
		}
		else
		{
			if (toDel == toDel->parent->left)
			{
				replace->parent->left = replace;
			}
			else 
			{
				replace->parent->right = replace;
			}
		}

		if (toDel != _node)
		{
			_node->elem = toDel->elem;
		}

		if (toDel->color == false)
		{
			DeleteAdjust(replace);
		}
		delete toDel;
		count--;
	}

	//删除一个节点后进行调整
	inline void DeleteAdjust(GTRBTreeNode<T>* _node)
	{
		GTRBTreeNode<T>* p = _node;
		while (p != root && p->color == false) 
		{
			if (p == p->parent->left)
			{
				GTRBTreeNode<T>* sibling = p->parent->right;
				if (sibling->color == true)
				{
					sibling->color   = false;
					p->parent->color = true;
					LeftRotate(p->parent);
					sibling = p->parent->right;
				}
				if (sibling->left->color == false && sibling->right->color == false) 
				{
					sibling->color = true;
					p = p->parent;
				}
				else 
				{
					if (sibling->right->color == false)
					{
						sibling->left->color = false;
						sibling->color = true;
						RightRotate(sibling);
						sibling  = sibling->parent;
					}
					sibling->color = sibling->parent->color;
					sibling->parent->color = false;
					sibling->right->color = false;
					LeftRotate(sibling->parent);
					p = root;
				}
			}
			else
			{
				GTRBTreeNode<T>* sibling = p->parent->left;
				if (sibling->color == true)
				{
					sibling->color   = false;
					p->parent->color = true;
					RightRotate(p->parent);
					sibling = p->parent->left;
				}
				if (sibling->left->color == FALSE && sibling->right->color == false)
				{
					sibling->color = true;
					p = p->parent;
				}
				else
				{
					if (sibling->left->color == false)
					{
						sibling->right->color = false;
						sibling->color = true;
						LeftRotate(sibling);
						sibling = sibling->parent;
					}
					sibling->color = sibling->parent->color;
					sibling->parent->color = false;
					sibling->left->color = false;
					RightRotate(sibling->parent);
					p = root;
				}
			}
		}
		p->color = false;
	}
    
	//把一个节点向左下方移一格，并让他原来的右子节点代替它的位置
	inline bool LeftRotate(GTRBTreeNode<T>* _node)
	{	
		if(_node->right!=nil)
		{
			GTRBTreeNode<T>* y = _node->right;
			_node->right = y->left;
			if(y->left!=nil)
				y->left->parent = _node;
			y->parent = _node->parent;
			if(y->parent==nil)
				root=y;
			else
			{
				if(_node->parent->left==_node)
				{
					_node->parent->left = y;
				}
				else
				{
					_node->parent->right = y;
				}
			}
			y->left = _node;
			_node->parent = y;
			return true;		
		}
		return false;
	}
    
	//把一个节点向右下方移一格，并让他原来的左子节点代替它的位置
	inline bool RightRotate(GTRBTreeNode<T>* _node)
	{	
		if(_node->left!=nil)
		{
			GTRBTreeNode<T>* y = _node->left;
			_node->left = y->right;
			if(y->right!=nil)
				y->right->parent = _node;
			y->parent=_node->parent;
			if(y->parent==nil)
				root=y;
			else
			{
				if(_node->parent->left==_node)
				{
					_node->parent->left = y;
				}
				else
				{
					_node->parent->right = y;
				}
			}
			y->right=_node;
			_node->parent=y;
			return true;
		}
		return false;
	}
};

}

#endif