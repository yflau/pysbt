//copy from http://www.nocow.cn/index.php/Code:SBT_C

/****************************************
*          SBT.cc
*
*    Mon Jun 14 16:57:48 2007
*    Copyright  2007  巨菜逆铭
* 
******************************************/
 
//————BST的储存结构————
typedef struct SBTNode{
  SBTNode *left,*right;
  long key;  //这里省略了卫星数据域
  unsigned long size;
  SBTNode(long _key){  //构造函数：未考虑卫星数据
    left=right=NULL;
    size=1;
    key=_key;
  }
}SBTNode, *SBTree;
 
//————SBT基本操作函数原型说明————
SBTNode *SBT_Search(SBTree T,long key);
  //在T中中寻找关键字为key的结点
  //若能找到则返回指向它的指针，否则返回NULL
void SBT_Insert(SBTree &T, SBTNode *x);
  //将节点x插入树中
  //初始要求x的left和right域为NULL，size域为1
SBTNode *SBT_Delete(SBTree &T, long key);
  //从以T为根的SBT中删除一个关键字为v的结点并返回其指针
  //如果树中没有一个这样的结点，删除搜索到的最后一个结点并返回其指针
SBTNode *SBT_Pred(SBTree T, long key);
  //返回指向关键字为key的节点在T的中序遍历中的直接前趋的指针
  //要求T中必须有关键字为key的节点
SBTNode *SBT_Succ(SBTree T,long key);
  //返回指向关键字为key的节点在T的中序遍历中的直接后继的指针
  //要求T中必须有关键字为key的节点
SBTNode *SBT_Select(SBTree T, unsigned long i);
  //从树T中找到关键字第i小的结点并返回其指针
unsigned long SBT_Rank(SBTree T, long key);
  //返回关键字为key的节点在树T中的秩
  //若不存在此节点则返回0
 
//————SBT的修复操作————
inline void SBT_LeftRotate(SBTree &x){
  //左旋
  SBTNode *y=x->right;
  assert(y!=NULL);
  x->right=y->left;
  y->left=x;
  y->size=x->size;
  x->size=(!x->left?0:x->left->size)
    +(!x->right?0:x->right->size)+1;
  x=y;
}
inline void SBT_RightRotate(SBTree &x){
  //右旋
  SBTNode *y = x->left;
  assert(y!=NULL);
  x->left=y->right;
  y->right=x;
  y->size=x->size;
  x->size=(!x->left?0:x->left->size)
    +(!x->right?0:x->right->size)+1;
  x=y;
}
void SBT_Maintain(SBTree &T,bool flag){
  //维护操作的核心：保持
  if(!T)  return;  //空树无需Maintain
  if(!flag){
    if( T->left&&T->left->left
      &&(!T->right||T->left->left->size > T->right->size) )  //情况1
      SBT_RightRotate(T);
    else if( T->left&&T->left->right
      &&(!T->right||T->left->right->size > T->right->size) ){  //情况2
      SBT_LeftRotate(T->left);
      SBT_RightRotate(T);
    }
    else return;  //无需修复
  }
  else{
    if( T->right&&T->right->right
      &&(!T->left||T->right->right->size > T->left->size) )  //情况1'
      SBT_LeftRotate(T);
    else if( T->right && T->right->left
      &&(!T->left||T->right->left->size > T->left->size) ){  //情况2'
      SBT_RightRotate(T->right);
      SBT_LeftRotate(T);
    }
    else return;//无需修复
  }
  SBT_Maintain(T->left,0);  //修复左子树
  SBT_Maintain(T->right,1);  //修复右子树
  SBT_Maintain(T,0);  //修复整棵树
  SBT_Maintain(T,1);
}
 
//————SBT基本操作的算法描述————
SBTNode *SBT_Search(SBTree T,long key){
  //在T中中寻找关键字为key的结点
  //若能找到则返回指向它的指针，否则返回NULL
  return !T||T->key==key?T:SBT_Search(key<T->key?T->left:T->right,key);
}
void SBT_Insert(SBTree &T, SBTNode *x){
  //将节点x插入树中
  if(!T)  T=x;
  else{
    T->size++;
    SBT_Insert(x->key<=T->key?T->left:T->right,x);
    SBT_Maintain(T,x->key>T->key);
  }
}
SBTNode *SBT_Delete(SBTree &T, long key){
  //从以T为根的SBT中删除一个关键字为key的结点并返回“实际”被删除结点的指针
  //如果树中没有一个这样的结点，删除搜索到的最后一个结点并返回其指针
  if(!T)  return NULL;
  T->size--;
  if(key==T->key||key<T->key&&!T->left||key>T->key&&!T->right)
  {
    SBTNode *del;
    if(!T->left||!T->right){
      del=T;
      T=(T->left?T->left:T->right);
    }
    else{
      del=SBT_Delete(T->right,key-1);
      T->key=del->key;  //若有卫星数据也需复制
    }
    return del;
  }
  else return SBT_Delete(key<T->key?T->left:T->right,key);
}
SBTNode *SBT_Pred(SBTree T, long key){
  //返回指向拥有比key小的最大关键字的节点的指针
  if(!T)  return NULL;
  if(key<=T->key)  return SBT_Pred(T->left,key);
  else{
    SBTNode *pred=SBT_Pred(T->right,key);
    return (!pred?T:pred);
  }
}
SBTNode *SBT_Succ(SBTree T,long key){
  //返回指向拥有比key大的最小关键字的节点的指针
  if(!T)  return NULL;
  if(key>=T->key)  return SBT_Succ(T->right,key);
  else{
    SBTNode *succ= SBT_Succ(T->left,key);
    return(!succ?T:succ);
  }
}
SBTNode *SBT_Select(SBTree T, unsigned long i){
  //从树T中找到关键字第i小的结点并返回其指针
  if(!T||i>T->size)  return NULL;
  unsigned long r = (!T->left?0:T->left->size)+1;
  if(i==r)  return T;
  else if(i<r)  return SBT_Select(T->left,i);
  else return SBT_Select(T->right,i-r);
}
unsigned long SBT_Rank(SBTree T, long key){
  //返回关键字为key的节点在树T中的秩
  //若不存在此节点则返回0
  if(!T)  return 0;
  if(T->key==key)  return (!T->left?0:T->left->size)+1;
  else if(key<T->key)  return SBT_Rank(T->left,key);
  else{
    unsigned long r=SBT_Rank(T->right,key);
    return r==0?0:r+(!T->left?0:T->left->size)+1;
  }
}