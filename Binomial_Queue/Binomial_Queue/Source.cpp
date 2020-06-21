
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <ctime>

using namespace std;

const int SIZE = 1023; // максимальное кол-во элементов

//-------------Структура элемента очереди-------------------------------------------------------------------------------
struct Node {
	int data;
	Node* left;
	Node* right;
	Node() {}
	Node(int Data) {
		data = Data;
		left = 0;
		right = 0;
	}
	Node(const Node& n) {
		data = n.data;
		Node* l = 0, *r = 0;
		if (n.left) l = new Node(*n.left);
		if (n.right) r = new Node(*n.right);
		left = l;
		right = r;
	}
	~Node() {
		if (left) {
			delete left;
			left = 0;
		}
		if (right) {
			delete right;
			right = 0;
		}
	}

};
//----------------------------------------------------------------------------------------------------------------------
//---------------Структура элемента верхнего хранилища------------------------------------------------------------------
struct TopNode
{
	Node* data;
	int count;
	TopNode* left;
	TopNode* right;
	TopNode() {}
	TopNode(Node* Data, int c) {
		data = Data;
		count = c;
		left = 0;
		right = 0;
	}
	TopNode(const TopNode& n) {
		data = n.data;
		count = n.count;
		TopNode* l = 0, *r = 0;
		if (n.left) l = new TopNode(*n.left);
		if (n.right) r = new TopNode(*n.right);
		left = l;
		right = r;
	}
	~TopNode() {
		if (left) {
			delete left;
			left = 0;
		}
		if (right) {
			delete right;
			right = 0;
		}
	}

};

//----------------------------------------------------------------------------------------------------------------------
//---------------Доп.структуры и функции для вывода на экран дерева-----------------------------------------------------
struct node_print_state_t {
	node_print_state_t* child_state;
	bool printing_last_child;
};

node_print_state_t* _root_state = NULL;

//----------------------------------------------------------------------------------------------------------------------
//---------------Структура нижнего хранилища----------------------------------------------------------------------------
struct binomial_queue {
	Node* mas[SIZE];
	int size;
	binomial_queue() { memset(mas, 0, sizeof(mas)); size = 0; }
	binomial_queue(const binomial_queue& bq) {

		memset(mas, 0, sizeof(mas));

		size = bq.size;
		for (int i = 0; i < SIZE; i++) {
			if (bq.mas[i]) {
				Node* deepCopy = new Node(*bq.mas[i]);
				mas[i] = deepCopy;
			}
		}
	}
	~binomial_queue() {
		for (int i = 0; i < SIZE; ++i) {
			if (mas[i])
				delete mas[i];
		}
	}
	void clear() {
		memset(mas, 0, sizeof(mas)); size = 0;
	}
	void read(int n) {
		int val;
		cout << "Введите " << n << " элементов:" << endl;
		for (int i = 0; i < n; i++) {
			cin >> val;
			add(val);
		}
	}
	void random(int n)
	{
		for (int i = 0; i < n; i++) {
			int val = rand() % 50 + 1;
			add(val);
			cout << val << " ";
		}
	}
	void output(char* str) {
		cout << str << " ";
		binomial_queue bq = *this;
		while (!bq.empty())
			cout << bq.getMin()<<" ";
		cout << endl;
	}


	bool empty() {
		return size == 0;
	}
	Node* join(Node* f, Node* s) {
		if (f->data < s->data) {
			s->right = f->left; f->left = s; return f;
		}
		else {
			f->right = s->left; s->left = f; return s;
		}
	}
	int num(int c, int b, int a) {
		return 4 * c + 2 * b + a;
	}
	void joinBQ(Node* a[], Node* b[]) {
		Node* c = 0;
		for (int i = 0; i < SIZE; i++) {
			switch (num(c != 0, b[i] != 0, a[i] != 0)) {
			case 2: a[i] = b[i]; break;
			case 3: c = join(a[i], b[i]); a[i] = 0; break;
			case 4: a[i] = c; c = 0; break;
			case 5: c = join(a[i], c); a[i] = 0; break;
			case 6:
			case 7: c = join(b[i], c); break;
			}
		}
	}
	void joinBQ(binomial_queue& bq) {
		joinBQ(mas, bq.mas);
		size += bq.size;
		bq.clear();
	}
	void add(int val) {
		Node* newNode = new Node(val);
		Node* curNode = newNode;
		for (int i = 0; i < SIZE; i++) {
			if (mas[i] == NULL) {
				mas[i] = curNode;
				break;
			}
			else {
				curNode = join(curNode, mas[i]);
				mas[i] = NULL;
			}
		}
		size++;
	}
	int getMin() {
		int res = INT32_MAX;
		int resPos = -1;
		for (int i = 0; i < SIZE; i++) {
			if (mas[i] && mas[i]->data < res) {
				res = mas[i]->data;
				resPos = i;
			}
		}
		Node** tmp = new Node * [SIZE];
		memset(tmp, 0, sizeof(tmp) * SIZE);
		Node* cur = mas[resPos]->left;
		for (int i = resPos - 1; i >= 0; i--) {
			tmp[i] = new Node(*cur);
			cur = cur->right;
			tmp[i]->right = 0;
		}
		delete mas[resPos];
		mas[resPos] = 0;

		joinBQ(mas, tmp);
		delete tmp;
		size--;
		return res;
	}
	int findmin(int resPos)
	{
		int res = mas[resPos]->data;
		return res;
	}
	
	int get(int resPos)
	{
		int res = mas[resPos]->data;
		Node** tmp = new Node * [SIZE];
		memset(tmp, 0, sizeof(tmp) * SIZE);
		Node* cur = mas[resPos]->left;
		for (int i = resPos - 1; i >= 0; i--) {
			tmp[i] = new Node(*cur);
			cur = cur->right;
			tmp[i]->right = 0;
		}
		delete mas[resPos];
		mas[resPos] = 0;

		joinBQ(mas, tmp);
		delete tmp;
		size--;
		return res;
	}

	void print_subtree(const Node* node) {
		node_print_state_t* parent_state;
		if (_root_state != NULL) {
			printf(" ");
			node_print_state_t* s = _root_state;
			while (s->child_state != NULL) {
				printf(s->printing_last_child ? "  " : "| ");
				s = s->child_state;
			}
			parent_state = s;
			printf(parent_state->printing_last_child ? "L" : "+");
		}
		else {
			parent_state = NULL;
		}
		printf(">%i\n", node->data);

		if ((node->left != NULL) || (node->right != NULL)) { // если есть дети
			node_print_state_t s;
			if (parent_state != NULL) {
				parent_state->child_state = &s;
			}
			else {
				_root_state = &s;
			}
			s.child_state = NULL;

			// печатаем детей
			if (node->left != NULL) {
				s.printing_last_child = (node->right == NULL);
				print_subtree(node->left);
			}
			if (node->right != NULL) {
				s.printing_last_child = true;
				print_subtree(node->right);
			}

			if (parent_state != NULL) {
				parent_state->child_state = NULL;
			}
			else {
				_root_state = NULL;
			}
		}
	}

	void print_tree(const Node* root) {
		assert(_root_state == NULL);
		try {
			if (root != NULL) {
				print_subtree(root);
			}
		}
		catch (...) {
			// если что-то пошло не так, принудительно reset-им состояние
			_root_state = NULL;
			throw;
		}
	}

	void print()
	{
		cout << endl;
		for (int i = 0; i < size; i++)
		{
			if (mas[i] != NULL)
			{
				cout << endl << "Tree " << i << ":" << endl;
				print_tree(mas[i]);
				cout << endl;
			}
		}
		cout << endl << endl;
	}
};
//----------------------------------------------------------------------------------------------------------------------
//---------------Структура верхнего хранилища---------------------------------------------------------------------------
struct TopBinomial_queue
{
	TopNode* mas[SIZE];
	int size;
	TopBinomial_queue() { memset(mas, 0, sizeof(mas)); size = 0; }
	TopBinomial_queue(const TopBinomial_queue& bq) {

		memset(mas, 0, sizeof(mas));

		size = bq.size;
		for (int i = 0; i < SIZE; i++) {
			if (bq.mas[i]) {
				TopNode* deepCopy = new TopNode(*bq.mas[i]);
				mas[i] = deepCopy;
			}
		}
	}
	~TopBinomial_queue() {
		for (int i = 0; i < SIZE; ++i) {
			if (mas[i])
				delete mas[i];
		}
	}
	void clear() {
		memset(mas, 0, sizeof(mas)); size = 0;
	}
	void read(binomial_queue* val) {
		for (int i=0;i<SIZE;i++)
			if(val->mas[i]!=NULL)add(val->mas[i],i);
		
	}


	bool empty() {
		return size == 0;
	}
	TopNode* join(TopNode* f, TopNode* s) {
		if (f->data->data < s->data->data) {
			s->right = f->left; f->left = s; return f;
		}
		else {
			f->right = s->left; s->left = f; return s;
		}
	}
	int num(int c, int b, int a) {
		return 4 * c + 2 * b + a;
	}
	void joinBQ(TopNode* a[], TopNode* b[]) {
		TopNode* c = 0;
		for (int i = 0; i < SIZE; i++) {
			switch (num(c != 0, b[i] != 0, a[i] != 0)) {
			case 2: a[i] = b[i]; break;
			case 3: c = join(a[i], b[i]); a[i] = 0; break;
			case 4: a[i] = c; c = 0; break;
			case 5: c = join(a[i], c); a[i] = 0; break;
			case 6:
			case 7: c = join(b[i], c); break;
			}
		}
	}
	void joinBQ(TopBinomial_queue& bq) {
		joinBQ(mas, bq.mas);
		size += bq.size;
		bq.clear();
	}
	void add(Node* val, int count) {
		TopNode* newNode = new TopNode(val,count);
		TopNode* curNode = newNode;
		for (int i = 0; i < SIZE; i++) {
			if (mas[i] == NULL) {
				mas[i] = curNode;
				break;
			}
			else {
				curNode = join(curNode, mas[i]);
				mas[i] = NULL;
			}
		}
		size++;
	}
	int getMin() {
		Node* res = new Node(INT_MAX);
		int resPos = -1;
		int retPos = -1;
		for (int i = 0; i < SIZE; i++) {
			if (mas[i] && mas[i]->data->data < res->data) {
				res = mas[i]->data;
				resPos = i;
				retPos = mas[i]->count;
			}
		}
		TopNode** tmp = new TopNode * [SIZE];
		memset(tmp, 0, sizeof(tmp) * SIZE);
		TopNode* cur = mas[resPos]->left;
		for (int i = resPos - 1; i >= 0; i--) {
			tmp[i] = new TopNode(*cur);
			cur = cur->right;
			tmp[i]->right = 0;
		}
		delete mas[resPos];
		mas[resPos] = 0;

		joinBQ(mas, tmp);
		delete tmp;
		size--;
		return retPos;
	}

	int findmin()
	{
		Node* res = new Node(INT_MAX);
		int resPos = -1;
		int retPos = -1;
		for (int i = 0; i < SIZE; i++) {
			if (mas[i] && mas[i]->data->data < res->data) {
				res = mas[i]->data;
				resPos = i;
				retPos = mas[i]->count;
			}
		}
		return retPos;
	}

	void print_subtree(const TopNode* node) {
		node_print_state_t* parent_state;
		if (_root_state != NULL) {
			printf(" ");
			node_print_state_t* s = _root_state;
			while (s->child_state != NULL) {
				printf(s->printing_last_child ? "  " : "| ");
				s = s->child_state;
			}
			parent_state = s;
			printf(parent_state->printing_last_child ? "L" : "+");
		}
		else {
			parent_state = NULL;
		}
		printf(">%i\n", node->data->data);

		if ((node->left != NULL) || (node->right != NULL)) { // если есть дети
			node_print_state_t s;
			if (parent_state != NULL) {
				parent_state->child_state = &s;
			}
			else {
				_root_state = &s;
			}
			s.child_state = NULL;

			// печатаем детей
			if (node->left != NULL) {
				s.printing_last_child = (node->right == NULL);
				print_subtree(node->left);
			}
			if (node->right != NULL) {
				s.printing_last_child = true;
				print_subtree(node->right);
			}

			if (parent_state != NULL) {
				parent_state->child_state = NULL;
			}
			else {
				_root_state = NULL;
			}
		}
	}

	void print_tree(const TopNode* root) {
		assert(_root_state == NULL);
		try {
			if (root != NULL) {
				print_subtree(root);
			}
		}
		catch (...) {
			// если что-то пошло не так, принудительно reset-им состояние
			_root_state = NULL;
			throw;
		}
	}

	void print()
	{
		cout << endl;
		for (int i = 0; i < size; i++)
		{
			if (mas[i] != NULL)
			{
				cout << endl << "Tree " << i << ":" << endl;
				print_tree(mas[i]);
				cout << endl;
			}
		}
		cout << endl << endl;
	}
};
//----------------------------------------------------------------------------------------------------------------------


class TwoTierBQ
{
public:
	void Random()
	{
		int n;
		cout << endl << "Введите количество элементов: ";
		cin >> n;
		system("cls");
		cout << "Начальные элементы:" << endl;
		LowerStore.random(n);
		cout << endl;
		UpperStore.clear();
		UpperStore.read(&LowerStore);
		PrintTrees();
	}
	void HandInput()
	{
		int n;
		cout << endl << "Введите количество элементов: ";
		cin >> n;
		system("cls");
		cout << endl;
		LowerStore.read(n);
		cout << endl;
		UpperStore.clear();
		UpperStore.read(&LowerStore);
		PrintTrees();
	}
	void PrintTrees()
	{
		cout << endl << "Верхнее хранилище";
		UpperStore.print();
		cout << endl << "Нижнее хранилище";
		LowerStore.print();
		cout << endl;
	}
	void FindMin()
	{
		system("cls");
		int min = LowerStore.findmin(UpperStore.findmin());
		cout << endl << "Минимальный элемент: " << min << endl;
		PrintTrees();
	}

	void GetMin()
	{
		system("cls");
		int min = LowerStore.get(UpperStore.findmin());
		cout << endl << "Минимальный элемент: " << min << endl;
		UpperStore.clear();
		UpperStore.read(&LowerStore);
		PrintTrees();
	}

	void Insert()
	{
		int temp;
		cout << endl << "Введите элемент: ";
		cin >> temp;
		LowerStore.add(temp);
		UpperStore.clear();
		UpperStore.read(&LowerStore);
		PrintTrees();
	}

	void GetAll()
	{
		cout << "Элементы:" << endl;
		while (LowerStore.size > 0)
		{
			int min = LowerStore.get(UpperStore.findmin());
			cout << " " << min;
			UpperStore.clear();
			UpperStore.read(&LowerStore);
		}
		cout << endl;
		UpperStore.clear();
		LowerStore.clear();
	}

private:
	binomial_queue LowerStore;
	TopBinomial_queue UpperStore;
};


int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	TwoTierBQ BQ;
	
	int flag;
	do
	{
		system("cls");
		do
		{
			cout << endl << "Введите:" << endl << "1 - Ручной ввод" << endl << "2 - Заполнение случайными значениями" << endl;
			cin >> flag;
		} while (flag != 1 && flag != 2);

		if (flag == 1)BQ.HandInput();
		else BQ.Random();

		do
		{
			do
			{
				cout << endl << "Введите:" << endl << "1 - Чтобы вставить новый элемент" << endl << "2 - Чтобы найти минимальный элемент(без удаления)" << endl << "3 - Чтобы извлечь минимальный элемент(с удалением)" << endl << "4 - Чтобы извлечь все элементы по очереди" << endl;
				cin >> flag;
			} while (flag != 1 && flag != 2 && flag != 3 && flag != 4);

			switch (flag)
			{
			case 1:BQ.Insert(); break;
			case 2:BQ.FindMin(); break;
			case 3:BQ.GetMin(); break;
			case 4:BQ.GetAll(); system("pause"); break;
			default: break;
			}
			
			if (flag != 4)
			{
				do
				{
					cout << endl << "Введите:" << endl << "0 - чтобы выйти" << endl << "1 - что бы продолжить" << endl << "2 - что бы ввести новую очередь" << endl;
					cin >> flag;
				} while (flag != 1 && flag != 0 && flag != 2);
			}

		} while (flag != 2 && flag != 0 && flag != 4);
	} while (flag != 0);
	

	return 0;
}
