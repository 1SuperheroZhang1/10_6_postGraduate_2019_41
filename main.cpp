#include <stdio.h>
#include <stdlib.h>
/*
 * 41．（13 分）设线性表L=(a1,a2,a3,...,an-2,an-1,an)采用带头结点的单链表保存，链表中
的结点定义如下：
typedef struct node {
    int data;
    struct node* next;
} NODE；
请设计一个空间复杂度为 O(1)且时间上尽可能高效的算法，重新排列L中的各结点，得到线性表
 L'=(a1,an,a2,an-1,a3,an-2,...)。要求：
（1）给出算法的基本设计思想。
（2）根据设计思想，采用 C 或 C++语言描述算法，关键之处给出注释。
（3）说明你所设计的算法的时间复杂度
 解读：首先空间复杂度是O(1)，说明不能申请额外的空间，然后找到链表的中间结点，
 前面一半是L，将链表的后半部分给一个新的头结点L2,然后将链表L2进行原地逆置，然后再将L和L2链表进行合并
 【重点】：
    找链表的中间结点findMiddle
    逆置链表reverse
    合并链表merge
 解题设计：第一阶段，如何找到链表的中间结点？
            利用两个指针同步向后遍历的方法。
   定义两个指针pcur，ppre，让pcur指针每次走两步，ppre指针每次走一步，
   这样当pcur指针走到最后，那么ppre指针刚好在中间。
   注意：由于pcur每次循环是走两步的，因此每走一步都注意判断是否为NULL
          第二阶段，后一半链表设置为了L2,如何将L2原地逆置？
   首先需要判断链表是否为空，如果为空，就返回，如果只是1个结点，也不需要逆置，直接返回。
   第一步：链表原地逆置，需要使用3个指针，假如分别是r,s,t,它们分别指向链表的1,2,3也就是前3个结点。
   第二步：让s->next=r,这样2号结点就指向了1号结点，完成了逆置。
   第三步：让r,s,t,分别后移，即r=s,s=t,t=t->next,通过这个操作，r,s,t分别指向了链表的2,3,4
   结点，这时回到第二步，循环往复，当t为NULL时，结束循环。
   第四步：循环结束时，t为NULL，这时s是最后一个结点，r是倒数第二个结点，需要再次执行s->next=r;
   第五步：最后需要L2->next->next=NULL;这时让L2指向s，因为s是原链表最后一个结点，
   完成了逆置后，就是第一个结点，链表头结点L2指向s。
          第三阶段，将L与L2链表合并，合并时轮流放入一个结点。
   因为空间复杂度是O(1)，因此不申请新空间，但是依然需要3个指针(pcur,p,q),合并后的新链表
   pcur始终指向新链表尾部，初始化为pcur=L->next,
   使用p指针始终指向新链表L待放入的结点，初始化值为p=L->next.
   q指针始终指向链表L2待放入的结点，初始化值为q=L2->next,
   因为链表L的第一个节点不动，所以p=p->next;
   开启循环while(p!=NULL&&q!=NULL)，首先将pcur->next=q,然后q=q->next,pcur=pcur->next
   接着pcur->next=p,p=p->next,pcur=pcur->next,直到循环结束。
   循环结束后，有可能L还剩余一个结点，也可能L2剩余一个结点，但是只会有一个剩余的结点。
   判断p不为NULL，把p放入，如果q不为NULL，把q放入即可。
【代码流程】：先用尾插法新建一条链表，然后将链表拆分为两条，分别为L和L2，然后L2进行逆置，
 再把L和L2进行合并。
 【时间复杂度】：
 find_middle函数中有一个while循环,因为pcur每次移动两个结点，因此循环的次数为n/2,
 忽略首项系数，时间复杂度是O(n)
 reverse函数，因为reverse函数只遍历了L2链表，遍历长度是n/2,所以时间复杂度是O(n)
 merge函数while循环遍历的次数也是n/2,时间复杂度是O(n)
 上面3个函数总的运行次数是1.5n，忽略首项系数，因此时间复杂度是O(n)
 * */
typedef int ElemType;

typedef struct LNode{
    ElemType data;
    struct LNode* next;//指向下一个结点
}LNode,*LinkList;
//尾插法新建链表
LinkList list_tail_insert(LinkList &L){
    L=(LNode*) malloc(sizeof(LNode));//为头结点申请空间
    ElemType x;
    scanf("%d",&x);
    LinkList s,r=L;//s用来存储新的结点,r始终指向尾结点
    while(x!=9999){
        s=(LinkList) malloc(sizeof(LNode));//为新节点申请空间
        s->data=x;
        r->next=s;//让尾部结点指向新结点
        r=s;//r指向新的表尾结点
        scanf("%d",&x);
    }
    r->next=NULL;//尾结点的next指针赋值为NULL
    return L;
}
//打印链表
void print_list(LinkList L){
    L=L->next;
    while(L!=NULL){
        printf("%3d",L->data);
        L=L->next;//指向下一个结点
    }
    printf("\n");
}
//查找链表中间结点，并设置好L2链表
void find_middle(LinkList L,LinkList &L2){
    L2=(LinkList) malloc(sizeof(LNode));//L2链表的头结点
    LinkList pcur,ppre;//双指针法遍历链表，是考研初试常考的
    pcur=ppre=L->next;
    while(pcur){
        pcur=pcur->next;
        if(NULL==pcur){//为了防止pcur为NULL
            break;
        }
        pcur=pcur->next;//让大哥走两步
        if(NULL==pcur){//为了使得为偶数个的时候，ppre依然指向偶数节点的前一个
            break;
        }
        ppre=ppre->next;//小弟走一步
    }
    L2->next=ppre->next;//让L2成为链表后面的一半
    ppre->next=NULL;//前一半链表的最后一个结点，next要为NULL，让L成为前一半
}
//逆转链表
void reverse(LinkList L2){
    LinkList r,s,t;
    r=L2->next;
    if(NULL==r){
        return;//链表为空
    }
    s=r->next;
    if(NULL==s){
        return;//链表只有1个结点
    }
    t=s->next;
    while(t!=NULL){
        s->next=r;//原地逆置
        r=s;//以下3句是3个指针同时往后走一步
        s=t;
        t=t->next;
    }
    s->next=r;
    L2->next->next=NULL;//逆置后，链表第一个结点的next为NULL
    L2->next=s;//s是链表的第一个结点，L2指向它
}
//合并链表
void merge(LinkList L,LinkList L2){
    LinkList pcur,p,q;
    pcur=L->next;//pcur始终指向组合后链表的链表尾
    p=pcur->next;//p用来遍历L链表
    q=L2->next;//q指向L2链表的第一个结点，q用来遍历L2链表
    while(p!=NULL&&q!=NULL){
        pcur->next=q;
        q=q->next;//指向下一个
        pcur=pcur->next;
        pcur->next=p;
        p=p->next;//指向下一个
        pcur=pcur->next;
    }
    //任何一个链表都可能剩余一个结点，放进来即可
    if(p!=NULL){
        pcur->next=p;
    }
    if(q!=NULL){
        pcur->next=q;
    }
}
int main() {
    LinkList L;//链表头，是结构体指针类型
    list_tail_insert(L);
    print_list(L);//链表打印
    LinkList L2=NULL;//寻找中间结点，并返回第二条链表
    //查找链表的中间结点
    find_middle(L,L2);//只有一个结点时，L2中是没有结点的
    printf("----------------------------\n");
    print_list(L);
    print_list(L2);
    printf("----------------------------\n");
    //逆转链表
    reverse(L2);
    print_list(L2);
    printf("----------------------------\n");
    merge(L,L2);
    free(L2);
    print_list(L);
    return 0;
}
