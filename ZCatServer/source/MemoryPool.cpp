#include "../include/MemoryPool.h"

//二进制对齐算法，des必须为2的幂，但操作位非常快
std::size_t align_up1(std::size_t sou,std::size_t des)
{
    return (sou+des-1)&~(des-1);
}

//十进制对齐算法，除法和乘法运算慢，但通用性更强
std::size_t align_up2(std::size_t sou,std::size_t des)
{
    return (sou+des-1)/des*des;
}

//打印信息边框
void PrintBorder(int length)
{
    for(int i=0;i<length;i++)
        std::cout<<'-';
    std::cout<<std::endl;
}

void PrintTitleAlignCenter(const char* text,int length)
{
    for(int i=0;i<length/2-strlen(text)/2;i++)
        std::cout<<' ';
    std::cout<<text<<std::endl;
}

FixedSizePool::FixedSizePool(size_t block_size,size_t blocks)
{
    size_t ptr_size=sizeof(void*);
    this->block_size=block_size<ptr_size?ptr_size:align_up1(block_size,ptr_size);
    this->blocks=blocks;
    this->page_size=(this->blocks)*(this->block_size);
    this->pages=0;
    this->page_list=nullptr;
    this->block_list=nullptr;
}

FixedSizePool::~FixedSizePool()
{
    for(Node* find=this->page_list;find!=nullptr;)
    {
        Node* tmp=find;
        find=find->next;
        delete[] tmp;
    }
    this->page_list=nullptr;
}

size_t FixedSizePool::GetBlocks() const{return this->blocks;}
size_t FixedSizePool::GetPages() const{return this->pages;}

void FixedSizePool::allocate_page()
{
    char* page_head=new char[this->page_size];
    Node* ptr=((Node*)page_head);                  //指针结构
    ptr->next=this->page_list;
    this->page_list=ptr;
    page_head+=this->block_size;                   //偏移到第二块
    this->pages++;

    for(int timer=0;timer<this->blocks-1;timer++)  //计数器，用于遍历连续内存 
    {                                              //将块地址插入链表
        ((Node*)page_head)->next=this->block_list;
        this->block_list=(Node*)page_head;
        page_head+=this->block_size; 
    }
}

void* FixedSizePool::allocate_block()
{
    if(this->block_list==nullptr)
        this->allocate_page();

    Node* space=this->block_list;
    this->block_list=space->next;
    return space;
}

void FixedSizePool::deallocate_block(void* block)
{
    if(block==nullptr)
        return;

    ((Node*)block)->next=this->block_list;
    this->block_list=(Node*)block;
}   

void FixedSizePool::PoolInformaton(const char* mp_name)
{
    PrintBorder(INFO_TOTAL);
    PrintTitleAlignCenter(mp_name,INFO_TOTAL);
    std::cout<<std::left<<std::setw(INFO_HEAD)<<"[p-nums]";
    std::cout<<std::left<<std::setw(INFO_DATA)
    <<this->pages<<std::endl;

    std::cout<<std::left<<std::setw(INFO_HEAD)<<"[b-nums]";
    std::cout<<std::left<<std::setw(INFO_DATA)
    <<this->blocks<<std::endl;

    std::cout<<std::left<<std::setw(INFO_HEAD)<<"[b-size]";
    std::cout<<std::left<<std::setw(INFO_DATA)
    <<this->block_size<<"Bytes"<<std::endl;

    std::cout<<std::left<<std::setw(INFO_HEAD)<<"[t-size]";
    std::cout<<std::left<<std::setw(INFO_DATA)
    <<(this->pages)*(this->blocks)*(this->block_size)<<"Bytes"<<std::endl;

    std::cout<<std::left<<std::setw(INFO_HEAD)<<"[c-size]";
    std::cout<<std::left<<std::setw(INFO_DATA)
    <<(this->pages)*(this->blocks-1)*(this->block_size)<<"Bytes"<<std::endl;
    PrintBorder(INFO_TOTAL);
}