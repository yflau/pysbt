//copy from http://www.nocow.cn/index.php/Code:SBT_C

/****************************************
*          SBT.cc
*
*    Mon Jun 14 16:57:48 2007
*    Copyright  2007  �޲�����
* 
******************************************/
 
//��������BST�Ĵ���ṹ��������
typedef struct SBTNode{
  SBTNode *left,*right;
  long key;  //����ʡ��������������
  unsigned long size;
  SBTNode(long _key){  //���캯����δ������������
    left=right=NULL;
    size=1;
    key=_key;
  }
}SBTNode, *SBTree;
 
//��������SBT������������ԭ��˵����������
SBTNode *SBT_Search(SBTree T,long key);
  //��T����Ѱ�ҹؼ���Ϊkey�Ľ��
  //�����ҵ��򷵻�ָ������ָ�룬���򷵻�NULL
void SBT_Insert(SBTree &T, SBTNode *x);
  //���ڵ�x��������
  //��ʼҪ��x��left��right��ΪNULL��size��Ϊ1
SBTNode *SBT_Delete(SBTree &T, long key);
  //����TΪ����SBT��ɾ��һ���ؼ���Ϊv�Ľ�㲢������ָ��
  //�������û��һ�������Ľ�㣬ɾ�������������һ����㲢������ָ��
SBTNode *SBT_Pred(SBTree T, long key);
  //����ָ��ؼ���Ϊkey�Ľڵ���T����������е�ֱ��ǰ����ָ��
  //Ҫ��T�б����йؼ���Ϊkey�Ľڵ�
SBTNode *SBT_Succ(SBTree T,long key);
  //����ָ��ؼ���Ϊkey�Ľڵ���T����������е�ֱ�Ӻ�̵�ָ��
  //Ҫ��T�б����йؼ���Ϊkey�Ľڵ�
SBTNode *SBT_Select(SBTree T, unsigned long i);
  //����T���ҵ��ؼ��ֵ�iС�Ľ�㲢������ָ��
unsigned long SBT_Rank(SBTree T, long key);
  //���عؼ���Ϊkey�Ľڵ�����T�е���
  //�������ڴ˽ڵ��򷵻�0
 
//��������SBT���޸�������������
inline void SBT_LeftRotate(SBTree &x){
  //����
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
  //����
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
  //ά�������ĺ��ģ�����
  if(!T)  return;  //��������Maintain
  if(!flag){
    if( T->left&&T->left->left
      &&(!T->right||T->left->left->size > T->right->size) )  //���1
      SBT_RightRotate(T);
    else if( T->left&&T->left->right
      &&(!T->right||T->left->right->size > T->right->size) ){  //���2
      SBT_LeftRotate(T->left);
      SBT_RightRotate(T);
    }
    else return;  //�����޸�
  }
  else{
    if( T->right&&T->right->right
      &&(!T->left||T->right->right->size > T->left->size) )  //���1'
      SBT_LeftRotate(T);
    else if( T->right && T->right->left
      &&(!T->left||T->right->left->size > T->left->size) ){  //���2'
      SBT_RightRotate(T->right);
      SBT_LeftRotate(T);
    }
    else return;//�����޸�
  }
  SBT_Maintain(T->left,0);  //�޸�������
  SBT_Maintain(T->right,1);  //�޸�������
  SBT_Maintain(T,0);  //�޸�������
  SBT_Maintain(T,1);
}
 
//��������SBT�����������㷨������������
SBTNode *SBT_Search(SBTree T,long key){
  //��T����Ѱ�ҹؼ���Ϊkey�Ľ��
  //�����ҵ��򷵻�ָ������ָ�룬���򷵻�NULL
  return !T||T->key==key?T:SBT_Search(key<T->key?T->left:T->right,key);
}
void SBT_Insert(SBTree &T, SBTNode *x){
  //���ڵ�x��������
  if(!T)  T=x;
  else{
    T->size++;
    SBT_Insert(x->key<=T->key?T->left:T->right,x);
    SBT_Maintain(T,x->key>T->key);
  }
}
SBTNode *SBT_Delete(SBTree &T, long key){
  //����TΪ����SBT��ɾ��һ���ؼ���Ϊkey�Ľ�㲢���ء�ʵ�ʡ���ɾ������ָ��
  //�������û��һ�������Ľ�㣬ɾ�������������һ����㲢������ָ��
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
      T->key=del->key;  //������������Ҳ�踴��
    }
    return del;
  }
  else return SBT_Delete(key<T->key?T->left:T->right,key);
}
SBTNode *SBT_Pred(SBTree T, long key){
  //����ָ��ӵ�б�keyС�����ؼ��ֵĽڵ��ָ��
  if(!T)  return NULL;
  if(key<=T->key)  return SBT_Pred(T->left,key);
  else{
    SBTNode *pred=SBT_Pred(T->right,key);
    return (!pred?T:pred);
  }
}
SBTNode *SBT_Succ(SBTree T,long key){
  //����ָ��ӵ�б�key�����С�ؼ��ֵĽڵ��ָ��
  if(!T)  return NULL;
  if(key>=T->key)  return SBT_Succ(T->right,key);
  else{
    SBTNode *succ= SBT_Succ(T->left,key);
    return(!succ?T:succ);
  }
}
SBTNode *SBT_Select(SBTree T, unsigned long i){
  //����T���ҵ��ؼ��ֵ�iС�Ľ�㲢������ָ��
  if(!T||i>T->size)  return NULL;
  unsigned long r = (!T->left?0:T->left->size)+1;
  if(i==r)  return T;
  else if(i<r)  return SBT_Select(T->left,i);
  else return SBT_Select(T->right,i-r);
}
unsigned long SBT_Rank(SBTree T, long key){
  //���عؼ���Ϊkey�Ľڵ�����T�е���
  //�������ڴ˽ڵ��򷵻�0
  if(!T)  return 0;
  if(T->key==key)  return (!T->left?0:T->left->size)+1;
  else if(key<T->key)  return SBT_Rank(T->left,key);
  else{
    unsigned long r=SBT_Rank(T->right,key);
    return r==0?0:r+(!T->left?0:T->left->size)+1;
  }
}