/*****************************************************************/
/* @GTATDList.h
 * @用于Boolean操作的双向循环链表 
 * @采用64位整型定义坐标
 * @实现了普通迭代器、栈迭代器和用于排序插入的迭代器
 ****************************************************************/
/* @Original codes by Klaas Holwerda et al.,2004
 * @范俊甫, fanjf@lreis.ac.cn 11-01-2006
/*****************************************************************/
#ifndef GT_ATDLIST_H_INCLUDE
#define GT_ATDLIST_H_INCLUDE

namespace gts{

#ifndef NULL
#define NULL 0
#endif

#define SWAP(x,y,t)((t)=(x),(x)=(y),(y)=(t))
#define ATBL_RT _list->_root
#define ATBL_HD _list->_root->_next
#define ATBL_TL _list->_root->_prev
#define ATBL_NB _list->_count

template <class DType> class GTATDList;
template <class DType> class GTATDL_Iter;

template <class DType>
class GTATDLNode
{
	friend class GTATDList<DType>;
	friend class GTATDL_Iter<DType>;

public:
	GTATDLNode(void):_prev(NULL), _next(NULL), _twin(NULL){}
    GTATDLNode(const DType& _data):_item(_data),_prev(NULL), _next(NULL), _twin(NULL){}
    ~GTATDLNode(void){}

	DType     _item;
    GTATDLNode* _prev;// *前一个节点
	GTATDLNode* _next;// *下一个节点

	GTATDLNode* _twin;// *孪生节点指针
};

template <class DType>
class GTATDList
{
	friend class GTATDL_Iter<DType>;

public:
	GTATDList(void):_count(0), _iternum(0)
	{
		_root = new GTATDLNode<DType>();
		_root->_prev = _root;
		_root->_next = _root;
	}
	~GTATDList(void){Clear();delete _root;}
    void Clear();
	int  GetCount()const{return this->_count;}
	bool Empty()const{return (this->_count == 0);}
	
	GTATDLNode<DType>* push_end(const DType& _data);
	GTATDLNode<DType>* push_head(const DType& _data);

	void remove_end(void);
	void remove_head(void);

	DType head_item()const
	{
		if(this->Empty()) return 0;
		return _root->_next->_item;
	}
    DType end_item()const
	{
		if(this->Empty()) return 0;
		return _root->_prev->_item;
	}
	//将另一个链表的数据加到本链表的尾部
	void merge_from(GTATDList<DType>* _dlist);

protected:
	GTATDLNode<DType>* _root;//指向第一个元素的头指针
	int _count;            //元素个数
	int _iternum;          //迭代器个数
};

//双向链表的一般迭代器
template <class DType>
class GTATDL_Iter
{
public:
	GTATDL_Iter(GTATDList<DType>* _dlist)
		:_list(_dlist), _current(ATBL_RT)
	{
		if(_dlist)
			_dlist->_iternum++;
	}
	GTATDL_Iter(GTATDL_Iter<DType>* _iter)
	{
		if(_iter)
		{
			if(_iter->_current)
			{
				_list = _iter->_list;
				_current = _iter->_current;
				_list->_iternum++;
			}
		}
	}
	GTATDL_Iter():_list(NULL), _current(NULL){}
   ~GTATDL_Iter()
   {
	   if(!this->_current)
		   return;
	   _list->_iternum--;
	   _list    = NULL;
	   _current = NULL;
	   //?异常处理?
   }
   void Attach(GTATDList<DType>* _dlist);
   void Detach();
   GTATDLNode<DType>* GetHandle(){return this->_current;}

   void TraverseAll(void(*_fp)(DType _data));
   void merge_from(GTATDList<DType>* _dlist);
   void merge_from(GTATDL_Iter<DType>* _iter);
   void merge_from(GTATDL_Iter<DType>* _iter, const int& _count);
   void reset_head();//设置 _current为链表的头
   void reset_tail();//设置 _current为链表的尾
   bool Empty()const
   {
	   if(!_current)return true;
	   return (ATBL_NB == 0);
   }
   bool hit_root()const//判断迭代器位置是否在root处
   {
	    if(!_current)return true;
		return (_current == ATBL_RT);
   }
   bool hit_head()const
   {
	   if(!_current)return false;
	   return (_current == ATBL_HD);
   }
   bool hit_tail()const
   {
	   if(!_current)return false;
	   return (_current == ATBL_TL);
   }
   bool has_elem(const DType& _data)const;
   int  count()const
   {
	   if(!_current) return -1;
	   return ATBL_NB;
   }
   void moveto_head()
   {
	   if(!_current) return;
	   _current = ATBL_HD;
   }
   void moveto_tail()
   {
	   if(!_current) return;
	   _current = ATBL_TL;
   }
   void moveto_root()
   {
	   if(!_current) return;
	   _current = ATBL_RT;
   }

   /*将迭代器移到任意制定的节点位置上*/
   void moveto_loca(GTATDLNode<DType> *_loca)
   {
	   if(!_current || !_loca)
	   {
		   _current = _loca;
	   }
   }

   bool moveto_item(const DType& _data);
   
   void operator++(void)
   {
	   if(!_current) return;
	   _current=_current->_next;
   }
   void operator++(int)
   {
	   if(!_current) return;
	   _current=_current->_next;
   }
   void operator--(void)
   {
	   if(!_current) return;
	   _current=_current->_prev;
   }
   void operator--(int)
   {
	   if(!_current) return;
	   _current=_current->_prev;
   }
   void operator>>(int)
   {
	   if(!_current) return;
	   _current=_current->_next;
   }
   void operator<<(int)
   {
	   if(!_current) return;
	   _current=_current->_prev;
   }
   bool iterate()//位置后移, 如果移到根节点, 返回false
   {
	   if(!_current) return false;
	   _current = _current->_next;
	   return !(_current == ATBL_RT);
   }
   DType cur_item()const//返回当前位置上的数据
   {
	   DType _item;
	   if(!_current) return _item;
	   if(_current == ATBL_RT) return _item;
	   return _current->_item;
   }
   GTATDLNode<DType>* cur_node()const
   {
	   if(!_current) return NULL;
	   if(_current == ATBL_RT) return NULL;
	   return _current;
   }
   void moveto_twin()
   {
	   if(!_current) return;
	   _current = _current->_twin;
   }
   void next_wrap()//位置后移一个长度,如果移到根,则再移动
   {
	   if(!_current) return;
	   _current = _current->_next;
	   if(_current == ATBL_RT)
		   _current = _current->_next;
   }
   void prev_wrap()
   {
	   if(!_current) return;
	   _current = _current->_prev;
	   if(_current == ATBL_RT)
		   _current = _current->_prev;
   }
   void remove_all();//删掉与之关联的链表上的所有元素,但并不释放关联关系
   void remove_cur();//删掉当前位置的节点，迭代器后移
   void remove_head();
   void remove_tail();
   GTATDLNode<DType>* insert_end(const DType& _data);
   GTATDLNode<DType>* insert_begin(const DType& _data);
   GTATDLNode<DType>* insert_after(const DType& _data);
   GTATDLNode<DType>* insert_before(const DType& _data);
   
protected:
	GTATDList<DType>  *_list;
	GTATDLNode<DType> *_current;
	void next()//位置后移一个长度,可能是在根
	{
		if(!_current) return;
		_current = _current->_next;
	}
	void prev()
	{
		if(!_current) return;
		_current = _current->_prev;
	}
};
}
/*********************************************************/
/* @GTATDList类的实现
/*********************************************************/
template <class DType>
void GTATDList<DType>::Clear()
{
	//还需要对twin指针进行清空处理吗？上帝知道啊！

	if(_count == 0 || _iternum != 0) 
	{
		return;
	}
	GTATDLNode<DType>* _node;

	for(int i=0; i<_count; i++)
	{
		_node = _root->_next;
		_root->_next = _node->_next;
		if(_node)
		{
			delete _node;
			_node = NULL;
		}
	}
	_count   = 0;
	_iternum = 0;
	_root->_prev = _root;
	_root->_next = _root;
}
template <class DType>
GTATDLNode<DType>* GTATDList<DType>::push_end(const DType& _data)
{
	if(_iternum > 0) 
		return NULL;
	GTATDLNode<DType> *_node = new GTATDLNode<DType>(_data);
	if(!_node)
		return NULL;
	_node->_next = _root;
	_node->_prev = _root->_prev;
	_root->_prev->_next = _node;
	_root->_prev = _node;
	_count++;
	return _node;
}
template <class DType>
GTATDLNode<DType>* GTATDList<DType>::push_head(const DType& _data)
{
	if(_iternum > 0) 
		return NULL;
	GTATDLNode<DType>* _node = new GTATDLNode<DType>(_data);
	if(!_node)
		return NULL;
	_node->_prev = _root;
	_node->_next = _root->_next;
	_root->_next->_prev = _node;
	_root->_next = _node;
	_count++;
	return _node;
}
template <class DType>
void GTATDList<DType>::remove_end(void)
{
	if(_count == 0 || _iternum != 0)
		return;
	GTATDLNode<DType>* _node =_root->_prev;
	_node->_prev->_next = _node->_next;
	_node->_next->_prev = _node->_prev;
	if(_node)
	{
		delete _node;
		_node = NULL;
	}
	_count--;

}
template <class DType>
void GTATDList<DType>::remove_head(void)
{
	if(_count == 0 || _iternum != 0)
		return;
	GTATDLNode<DType>* _node =_root->_next;

	_node->_prev->_next = _node->_next; 
	_node->_next->_prev = _node->_prev; 
	if(_node)
	{
		delete _node;
		_node = NULL;
	}
	_count--;
}
template <class DType>
void GTATDList<DType>::merge_from(GTATDList<DType>* _dlist)
{
	if(!_dlist)
		return;
	if(_dlist->_iternum !=0 || _dlist->_count == 0)
		return;
	if(_dlist == this)
		return;

	_root->_prev->_next = _dlist->_root->_next;
	_dlist->_root->_next->_prev = _root->_prev;
	_dlist->_root->_prev->_next = _root;
	_root->_prev=_dlist->_root->_prev;

	this->_count  += _dlist->_count;
	_dlist->_count = 0;
	_dlist->_root->_next = _dlist->_root;
	_dlist->_root->_prev = _dlist->_root;
}
/*********************************************************/
/* @GTATDL_Iter类的实现
/*********************************************************/
template <class DType>
void GTATDL_Iter<DType>::Attach(GTATDList<DType>* _dlist)
{
	if(this->_current)
		return;
	if(!_dlist)
		return;
	_list    = _dlist;
	_current = ATBL_HD;
	_dlist->_iternum++;
}
template <class DType>
void GTATDL_Iter<DType>::Detach()
{
	if(!this->_current)
		return;
	_list->_iternum--;
	_list    = NULL;
	_current = NULL;
}
template <class DType>
void GTATDL_Iter<DType>::TraverseAll(void(*_fp)(DType _data))
{
	GTATDLNode<DType>* _node = ATBL_HD;
	for(int i=0; i<ATBL_NB; i++)
	{
		_fp(_node->_item);
		_node = _node->_next;
	}
}
template <class DType>
void GTATDL_Iter<DType>::merge_from(GTATDList<DType>* _dlist)
{
	if(!_current)
		return;
	if(_dlist->_iternum != 0)
		return;
	if((_dlist == this->_list) || (_dlist->_count == 0))
		return;

	ATBL_TL->_next=_dlist->_root->_next;
	_dlist->_root->_next->_prev=ATBL_TL;
	_dlist->_root->_prev->_next=ATBL_RT;
	ATBL_TL=_dlist->_root->_prev;

	ATBL_NB+=_dlist->_count;
	_dlist->_count = 0;
	_dlist->_root->_next=_dlist->_root;
	_dlist->_root->_prev=_dlist->_root;
}
template <class DType>
void GTATDL_Iter<DType>::merge_from(GTATDL_Iter<DType>* _iter)
{
	if (!_iter->_current || !_current)
		return;
	if (_iter->_list->_iternum > 1)
		return;
	else if (_iter->_list == _list)
		return;
	if (_iter->ATBL_NB == 0)
		return;

	ATBL_TL->_next=_iter->ATBL_HD;
	_iter->ATBL_HD->_prev=ATBL_TL;
	_iter->ATBL_TL->_next=ATBL_RT;
	ATBL_TL=_iter->ATBL_TL;

	ATBL_NB+=_iter->ATBL_NB;
	_iter->ATBL_NB=0;
	_iter->ATBL_HD=_iter->ATBL_RT;
	_iter->ATBL_TL=_iter->ATBL_RT;
	_iter->_current=_iter->ATBL_RT;
}
template <class DType>
void GTATDL_Iter<DType>::merge_from(GTATDL_Iter<DType>* _iter, const int& _count)
{}
template <class DType>
void GTATDL_Iter<DType>::reset_head()
{
	if (!_current)
		return;
	if (_list->_iternum > 1 )
		return;
	if(_current==ATBL_RT)
		return;

	ATBL_HD->_prev=ATBL_TL;
	ATBL_TL->_next=ATBL_HD;
	ATBL_HD=_current;
	ATBL_TL=_current->_prev;
	ATBL_TL->_next=ATBL_RT;
	ATBL_HD->_prev=ATBL_RT;
}
template <class DType>
void GTATDL_Iter<DType>::reset_tail()
{
	if (!_current)
		return;
	if (_list->_iternum > 1 )
		return;
	if(_current==ATBL_RT)
		return;

	ATBL_HD->_prev=ATBL_TL;
	ATBL_TL->_next=ATBL_HD;
	ATBL_TL=_current;
	ATBL_HD=_current->_next;
	ATBL_HD->_prev=ATBL_RT;
	ATBL_TL->_next=ATBL_RT;
}
template <class DType>
bool GTATDL_Iter<DType>::has_elem(const DType& _data)const
{
	if(!_current) return false;
	GTATDLNode<DType>* _node = ATBL_HD;
	for(int i=0; i<ATBL_NB; i++)
	{
		if(_node->_item == _data)
			return true;
		_node = _node->_next;
	}
	return false;
}
template <class DType>
bool GTATDL_Iter<DType>::moveto_item(const DType& _data)
{
	if (!_current) return false;
	GTATDLNode<DType>* _node = ATBL_HD; 
	for(int i=0; i<ATBL_NB; i++)
	{ 
		if (_node->_item == _data)
	    {
			_current = _node;
			return true;
	    }
		_node=_node->_next;
	}
	return false;
}
template <class DType>
void GTATDL_Iter<DType>::remove_all()
{
	if (_current==0)
		return;
	if (_list->_iternum > 1)
		return;
	_list->_iternum--;
	_list->Clear();
	_list->_iternum++;
	_current=ATBL_RT;
}
template <class DType>
void GTATDL_Iter<DType>::remove_cur()
{
	if (!_current) return;
	if (_list->_iternum > 1) return;
	if (_current==ATBL_RT) return;

	GTATDLNode<DType>* _node = _current;
	_current=_current->_next;
	_node->_prev->_next = _node->_next;
	_node->_next->_prev = _node->_prev;
	ATBL_NB--;
	if(_node)
	{
		delete _node;
		_node = NULL;
	}
}
template <class DType>
void GTATDL_Iter<DType>::remove_head()
{
	if (!_current) return;
	if (_list->_iternum > 1) return;
	if(ATBL_NB == 0) return;
	if (_current==ATBL_HD)
		_current=_current->_next;
	_list->_iternum--;
	_list->remove_head();
	_list->_iternum++;
}
template <class DType>
void GTATDL_Iter<DType>::remove_tail()
{
	if (!_current) return;
	if (_list->_iternum > 1) return;
	if(ATBL_NB == 0) return;
	if (_current==ATBL_TL)
		_current=_current->_prev;
	_list->_iternum--;
	_list->remove_end();
	_list->_iternum++;
}
template <class DType>
GTATDLNode<DType>* GTATDL_Iter<DType>::insert_end(const DType& _data)
{
	if (!_current)
		return NULL;
	if (_list->_iternum > 1)
		return NULL;

	_list->_iternum--;
	GTATDLNode<DType>* _node = _list->push_end(_data);
	_list->_iternum++;
	return _node;
}
template <class DType>
GTATDLNode<DType>* GTATDL_Iter<DType>::insert_begin(const DType& _data)
{
	if (!_current)
		return NULL;
	if (_list->_iternum > 1)
		return NULL;

	_list->_iternum--;
	GTATDLNode<DType>* _node = _list->push_head(_data);
	_list->_iternum++;
	return _node;
}
template <class DType>
GTATDLNode<DType>* GTATDL_Iter<DType>::insert_after(const DType& _data)
{
	if (!_current)
		return NULL;
	if (_list->_iternum > 1)
		return NULL;
    GTATDLNode<DType>* _node = new GTATDLNode<DType>(_data);
	_node->_next = _current->_next;
	_node->_prev = _current;
	_current->_next->_prev = _node;
	_current->_next = _node;
	ATBL_NB++;
	return _node;
}
template <class DType>
GTATDLNode<DType>* GTATDL_Iter<DType>::insert_before(const DType& _data)
{
	if (!_current)
		return NULL;
	if (_list->_iternum > 1)
		return NULL;

	GTATDLNode<DType>* _node = new GTATDLNode<DType>(_data);
	_node->_next = _current;
	_current->_prev->_next = _node;
	_node->_prev = _current->_prev;
	_current->_prev = _node;
	ATBL_NB++;
	return _node;
}

#endif