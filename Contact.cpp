#define _CRT_SECURE_NO_WARNINGS 1
#include "Contact.h"

void PrintIndexMenu() {
	printf("************************\n");
	printf("        Contact         \n");
	printf("        1.add           \n");
	printf("        2.delete        \n");
	printf("        3.modify        \n");
	printf("        4.search        \n");
	printf("        5.sort          \n");
	printf("        6.display       \n");
	printf("        0.exit          \n");
	printf("************************\n");
}

void initContact(Contact* pcon) {
	pcon->count = 0;
	memset(pcon->PeoInfo_1, 0, sizeof(pcon->PeoInfo_1));
}

void AddContact(Contact* pcon) {
	if (pcon->count == MAX_PEO)
	{
		printf("通讯录已满!\n");
		return;
	}
	printf("姓名>");
	scanf("%s", pcon->PeoInfo_1[pcon->count].name);
	printf("性别>");
	scanf("%s", pcon->PeoInfo_1[pcon->count].sex);
	printf("年龄>");
	scanf("%d", &(pcon->PeoInfo_1[pcon->count].age));
	printf("电话>");
	scanf("%s", pcon->PeoInfo_1[pcon->count].tele);
	printf("地址>");
	scanf("%s", pcon->PeoInfo_1[pcon->count].addr);
	printf("添加成功!\n");
	pcon->count++;
}

void DisplayContact(Contact* pcon) {
	int i = 0;
	printf("%-20s\t%-5s\t%-5s\t%-12s\t%-25s\n", "姓名", "性别", "年龄", "电话号码", "地址");
	for (i = 0; i < pcon->count; i++) {
		printf("%-20s\t%-5s\t%-5d\t%-12s\t%-25s\n",
			pcon->PeoInfo_1[i].name,
			pcon->PeoInfo_1[i].sex,
			pcon->PeoInfo_1[i].age,
			pcon->PeoInfo_1[i].tele,
			pcon->PeoInfo_1[i].addr);
	}
}

/*
删除模块的思想：伪删除，通过覆盖和计数器限制的方式达到删除效果
先查找要删除的元素下标，从该位置处后面所有的元素均先前挪动一位
所以，在物理内存上并没有实现删除，通过计数器限制有效的元素个数
同理，添加新元素实质上就是修改并覆盖元素，然后增加计数器

可以发现：查找、删除、修改功能模块都需要去查找元素的位置，所以可以定义一个模块的专用函数
*/


//模块的专用函数，所以用static修饰使其只在本源文件内使用，从而提高安全性
//通过名字查找，若存在则返回元素所在下标，若不存在返回-1
static int FindByName(Contact* pcon, char* name) {
	int i = 0;
	for (i = 0; i < pcon->count; i++) {
		if (strcmp(pcon->PeoInfo_1[i].name, name) == 0)
			return i;
	}
	return -1;
}

void DeleteContact(Contact* pcon) {
	char name[MAX_NAME];
	int pos = 0;
	if (pcon->count == 0)
	{
		printf("通讯录为空!\n");
		return;
	}
	printf("请输入要删除联系人的姓名>");
	scanf("%s", name);
	pos = FindByName(pcon, name);
	if (pos == -1)
	{
		printf("联系人不存在!\n");
		return;
	}
	int i = 0;
	for (i = pos; i < pcon->count - 1; i++) {
		pcon->PeoInfo_1[i] = pcon->PeoInfo_1[i + 1];
	}
	printf("删除成功!\n");
	pcon->count--;
}

void ModifyContact(Contact* pcon) {
	char name[MAX_NAME];
	if (pcon->count == 0)
	{
		printf("通讯录为空!\n");
		return;
	}
	printf("请输入需修改的联系人姓名>");
	scanf("%s", &name);
	int pos = FindByName(pcon, name);
	if (pos == -1)
	{
		printf("联系人不存在!\n");
		return;
	}
	printf("姓名>");
	scanf("%s", pcon->PeoInfo_1[pos].name);
	printf("性别>");
	scanf("%s", pcon->PeoInfo_1[pos].sex);
	printf("年龄>");
	scanf("%d", &(pcon->PeoInfo_1[pos].age));
	printf("电话>");
	scanf("%s", pcon->PeoInfo_1[pos].tele);
	printf("地址>");
	scanf("%s", pcon->PeoInfo_1[pos].addr);
	printf("修改成功!\n");
}

void SearchContact(Contact* pcon) {
	char name[MAX_NAME];
	if (pcon->count == 0)
	{
		printf("通讯录为空!\n");
		return;
	}
	printf("请输入要查询的联系人姓名>");
	scanf("%s", name);
	int pos = FindByName(pcon, name);
	if (pos == -1)
	{
		printf("联系人不存在!\n");
		return;
	}
	else
	{
		printf("%-20s\t%-5s\t%-5d\t%-12s\t%-25s\n",
			pcon->PeoInfo_1[pos].name,
			pcon->PeoInfo_1[pos].sex,
			pcon->PeoInfo_1[pos].age,
			pcon->PeoInfo_1[pos].tele,
			pcon->PeoInfo_1[pos].addr);
	}
}

int CmpByAge(const void* e1, const void* e2) {
	return ((PeoInfo*)e1)->age - ((PeoInfo*)e2)->age;
}

int CmpByName(const void* e1, const void* e2) {
	return strcmp(((PeoInfo*)e1)->name, ((PeoInfo*)e2)->name);
}


void SortContact(Contact* pcon) {
	int choose;
	if (pcon->count == 0)
	{
		printf("通讯录为空!\n");
		return;
	}
	printf("请选择排序依据\n");
	printf("1.名字\n");
	printf("2.年龄\n");
	printf(">");
	scanf("%d", &choose);
	if (choose == 1)
		qsort(pcon->PeoInfo_1, pcon->count, sizeof(PeoInfo), CmpByName);
	else if (choose == 2)
		qsort(pcon->PeoInfo_1, pcon->count, sizeof(PeoInfo), CmpByAge);
	else
	{
		printf("输入错误!\n");
		return;
	}
	printf("排序成功!\n");
}