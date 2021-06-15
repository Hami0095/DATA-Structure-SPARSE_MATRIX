#include<iostream>
#include<fstream>
#include<Windows.h>
#include<math.h>
#include<vector>
#include<string>
#include<list>


using namespace std;

#define PI 3.14


class SparseMatrix {
	struct Node {
		int colIndex;
		int value;
		Node* nextInRow;
		Node(int v, int i, Node* n = nullptr) {
			value = v;
			colIndex = i;
			nextInRow = n;
		}
		Node(const Node* obj) {
			this->value = obj->value;
			this->colIndex = obj->colIndex;
			this->nextInRow = obj->nextInRow;
		}
		Node(const Node & obj) {
			value = obj.value;
			colIndex = obj.colIndex;
			nextInRow = obj.nextInRow;
		}

		Node() {
			value = 0;
			colIndex = 0;
			nextInRow = NULL;
		}
		Node(int v, Node* n) {
			nextInRow = n;
			value = v;
		}
		
		void setNode(int v, int i, Node* n = nullptr) {
			value = v;
			colIndex = i;
			nextInRow = n;
		}
	};
	
	Node* curr;
	void attachNode(Node* a, Node* b) {
		a->nextInRow = b;
	}

	void attachNode(Node a, Node b) {
		a.nextInRow = &b;
	}
	int row, col;		// column is showing the actual numbers of columns of the whole original fucin matrix
						// infact, the column will be showing that whether the last row has same columns or not?
	bool transposed = false;
	vector<Node*> rowsList;  


	// our nodes will be attached to each element of this vector 
	int numberOfNodes(Node* _head) {
		int n = 0;
		Node* temp = _head;
		while (temp->nextInRow != nullptr) {
			n++;
			temp = temp->nextInRow;
		}
		return n;
	}
	bool nonZero(string & farigh) {
		int count = 0;
		int size = farigh.size();
		for (int i = 0; i < size; i++) {
			if (farigh[i] == '0')
				count++;
		}
		if (count == (size/2 + 1))
			return false;
		return true;
	}
	// Helper Functions -------------------
	int listSize(Node* head) {
		Node* temp;
		temp = head;
		int size = 0;
		while (temp != nullptr) {
			size++;
			temp = temp->nextInRow;
		}
		return size;
	}
	bool Order_Is_Same(const SparseMatrix& obj) {
		if (row = obj.row) {
			for (int i = 0; i < row; i++) {
				if (listSize(rowsList[i]) != listSize(obj.rowsList[i])) {
					return false;
				}
			}
		}
		return true;
	}
	int get_Value_At_Index(const SparseMatrix& obj, const SparseMatrix& t, int r,int c) {
		int to_be_multiplied = 0;
		Node* _row = t.rowsList[r];
		for (int i = 0; i < obj.row; i++) {
			Node* temp = obj.rowsList[i];
			while (temp != nullptr) {
				if (c == temp->colIndex) {
					to_be_multiplied += _row->value * temp->value;

					_row = _row->nextInRow;  // 1 1 
					cout << to_be_multiplied << endl;
// 					system("pause");
				}					
 				temp = temp->nextInRow;				
			}

		}

		return to_be_multiplied;

	}
	bool isFound(int rowNumber, int _col, int _value) {
		Node* ttemp = rowsList[rowNumber];
		for (int j = 0; ttemp != nullptr; j++) {
			// i have only checked the column numbers 
			if (ttemp->value == _value)
				return true;
			ttemp = ttemp->nextInRow;
		}
		return false;
	}
	
public:
	SparseMatrix(int a, int b, int cond) {
		row = a;
		col = b;
		rowsList.resize(0);
	}
	SparseMatrix(int a, int b) {
		row = a;
		col = b;
		//	res.rowsList es waqt  khaali ha

		for (int i = 0; i < row; i++) {
			rowsList.push_back(NULL);
		}
	}
	SparseMatrix(string filename) {
		
		Node* head;		// rowsList[head]
		Node* next_tail;

		//filename = "matrix.txt";
		ifstream myfile;
		myfile.open(filename);
		string farigh;
		// lets check how many rows are there?
		int countRows = 0;
		
		while (!myfile.eof()) {
			getline(myfile, farigh, '\n');

			int _col = farigh.size();
			col = (_col / 2) + 1;
			int count = 0;			
			bool non_Zero_row = nonZero(farigh);
			// we have to check k atleast ek non-zero entry ha tab if kro
			if (non_Zero_row) {
				for (int i = 0, j = 0; i < _col; i++) {
					
					// iteration is letting us to move through out the the row :/
					// we have to ignore non-zero entries
					// i need to maintain the index (ColIndex) && data
					
					char* temp = &farigh[i];
					int data = stoi(temp);
					if (data == 0 && farigh[i] != ' ') {
						j++;
					}
					if (data != 0 && farigh[i] != ' ') {						
						if (count == 0) {
							head = new Node(data, j);
							rowsList.push_back(head);
							curr = head;
							count++;
						}
						// 0 nai aa rha sparse main
						else if (count > 0) {
							next_tail = new Node(data, j);
							attachNode(curr, next_tail);
							curr = next_tail;			// curr and tail are pointing same
							count++;
						}
						j++;
					}
				
				}
			}
			else {
				head = nullptr;
				rowsList.push_back(head);
			}
				

			//rowsList[countRows] = head;
			countRows++;


		}
		
		row = countRows;		

	}

	SparseMatrix(const SparseMatrix& obj) {
		row = obj.row;
		col = obj.col;
	
		for (int i = 0; i < row; i++) {
			Node* temp = obj.rowsList[i];		// temp pointing to obj ki rows list			
			rowsList.push_back(temp);
		}

	}
	~SparseMatrix() {
		curr = nullptr;
		
		for(int i = 0; i < rowsList.size() + 1; i++)
			rowsList.pop_back();
		
		row = 0;
		col = 0;
	}
	// MAIN FUNCTIONS TO CHECK .....
	// working good
	const SparseMatrix& operator = (const SparseMatrix& obj) {
		SparseMatrix res(obj);
		return res;
	}
	// working good
	bool operator == (const SparseMatrix& obj) {
		// we have to check whether our matrices are equal or not?

		if (!(row == obj.row && col == obj.col)) {
			return false;
		}

		for (int i = 0; i < row; i++) {
			Node* temp, * obj_temp;
			temp = rowsList[i];
			obj_temp = obj.rowsList[i];
			if (numberOfNodes(this->rowsList[i]) == numberOfNodes(obj.rowsList[i])) {
				while (temp->nextInRow != nullptr) {
					if (temp->value == obj_temp->value
						&& temp->colIndex == obj_temp->colIndex
						)
					{
						temp = temp->nextInRow;
						obj_temp = obj_temp->nextInRow;
					}
					else
						return false;
				}
			}
			else
				return false;
		}
		return true;
	}
	// working good
	SparseMatrix operator + (const SparseMatrix& obj) {
		// only adding first and the last elements

		if (Order_Is_Same(obj)) {
			SparseMatrix res(obj);	// res is basically having my obj to be added, now i only need to add the data of this sparse matrix into res
			
			// in this case, i am only adding those values whose col Index is same
			for (int i = 0; i < row; i++) {
				// now i can traverse inside the rowsList
				Node*  resTemp;
				curr = rowsList[i];
				resTemp = res.rowsList[i];
				while (curr != nullptr) {
					if (curr->colIndex == resTemp->colIndex) {
						resTemp->value += curr->value;						
						
					}
					resTemp = resTemp->nextInRow;
					curr = curr->nextInRow;
				}
			}

			

			return res;
		}
		else {
			SparseMatrix res(0);
			return res;
		}
	}
	// working good
	SparseMatrix transpose() {
		
		for (int i = 0; i < row; i++) {
			curr = rowsList[i];
			while (curr != nullptr) {
				curr->colIndex = i;
				curr = curr->nextInRow;
			}
		}
		
		transposed = true;

		return *this;
	}
	
	void insertAt(int r, int c, int value) {
		
		Node* & temp = rowsList[r];
		int i = 0;
		while ((temp != nullptr) && (temp->nextInRow) && (c < temp->nextInRow->colIndex) && (c > temp->colIndex)) {
			temp = temp->nextInRow;
		}
		if (temp && temp->nextInRow != nullptr)
			temp = new Node(value, c, temp->nextInRow);
		else
			temp = new Node(value, c);
	}
	
	SparseMatrix operator * (const SparseMatrix& obj) {
		// i need to make check whether i can multiply matrices or not?
		// if the cols of A are equal to the rows of B, then new matrix will make of AxB(r = r2 && c = col1)
		if (row != obj.col) {
			SparseMatrix res(row, obj.col);
			return res;
		}		
		SparseMatrix res(row, obj.col);
		Node* head, * tail = nullptr;
		for (int i = 0; i < row; i++) {			
			Node* res_temp, * obj_temp;
			this->curr = this->rowsList[i];			// 1 1			
			obj_temp = obj.rowsList[i];			
			res.curr = res.rowsList[i];				// res.rowsList    
			int sum, counter_Col, j = 0;			
			sum = counter_Col = 0;
			
			while (curr != nullptr) {
				//sum = 0;							
				counter_Col = curr->colIndex;				
				//sum = get_Value_At_Index(obj, *this, i, counter_Col);								
				//  1   1			//  3  4		// 8	10   
				//  2   2				5  6		

				res.insertAt(i, counter_Col, get_Value_At_Index(obj, *this, i, counter_Col));
				curr = curr->nextInRow;
				counter_Col++;
			}
			//res.rowsList[i] = head;
		}						
		return res;
	}
	// working good
	bool isSubMatrix(const SparseMatrix& obj) {
		// we have to check whether the col indeces of the particular obj 
		// do exist in our this obj?

		// we need row number and colInd
		// we will compare it to the colInd and rows of our thisobj
		
		Node* objtemp;
		Node* ttemp;
		
		if (obj.row > row || obj.col > col)
			return false;

		for (int i = 0; i < obj.row; i++) {
			objtemp = obj.rowsList[i];
			ttemp = rowsList[i];
			while (objtemp != nullptr) {
				if (!(isFound(i, objtemp->colIndex, objtemp->value)))
					return false;			
				
				objtemp = objtemp->nextInRow;
			}
			
		}
		return true;

	}
	// working good
	void printMatrix(string i) {
		system("cls");
		cout << i << endl;
		cout << "\t(Rows are sepereted by different colors)" << endl << endl;
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		cout << "Pattern :- "  << " Value(colIndex) -> next " << endl;
		if (!transposed) {
			for (int i = 0; i < row; i++) {
				if (rowsList[i] != nullptr) {

					curr = rowsList[i];

					while (curr != nullptr) {
						cout << curr->value << " (";
						cout << curr->colIndex << ") \t -> \t";
						curr = curr->nextInRow;
					}
				}
				cout << "null" << endl;
				SetConsoleTextAttribute(h, i + 10);
			}
		}
		else {
			
			for (int i = 0; i < row; i++) {
				SetConsoleTextAttribute(h, i + 10);
				if (rowsList[i] != nullptr) {
					curr = rowsList[i];
					
					while (curr != nullptr) {
						
						
						cout << curr->value << " (";
						cout << curr->colIndex << ") \t" << endl;
						curr = curr->nextInRow;
					}
				}				
				cout << "null" << endl;
				SetConsoleTextAttribute(h, i+2);

			}
		}
		SetConsoleTextAttribute(h, 15);
	}
};
