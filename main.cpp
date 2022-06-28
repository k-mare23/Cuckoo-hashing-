#include <iostream>
#include <string>
#include <sstream>
using namespace std;
struct node {
    int value; //store value in linked list
    int pos1; //store first position of value
    int pos2; //store second position of value
    int count1; //tracks cycle back to pos1
    int count2; //tracks cycle back to pos2
    node* next;
};
bool success = true;
class cuckooHash {
private:
    node* tableHead = NULL;
    int tableSize;

public:
    cuckooHash(int size) { //constructor to create an empty linked list table
        tableSize = size;
        for (int i = 0; i < tableSize; i++) {
            if (tableHead == NULL) {
                node* temp = new node();
                temp->value = -1;
                temp->pos1 = temp->pos2 = -1;
                temp->count1 = temp->count2 = 0;
                temp->next = NULL;
                tableHead = temp;
            }
            else {
                node* temp = new node();
                temp->value = -1;
                temp->pos1 = temp->pos2 = -1;
                temp->count1 = temp->count2 = 0;
                temp->next = NULL;
                node* curr = tableHead;
                node* prev = NULL;
                while (curr != NULL) {
                    prev = curr;
                    curr = curr->next;
                }
                curr = temp;
                prev->next = curr;
         
            }
        }
    }
    
    void insertToTable(node* temp) {
        node* curr = tableHead;
        int index = 0;
        if (temp->count1 >= 1 && temp->count2 >= 1) { //means is a cycle
            success = false;
            return;
        }
        else if (temp->count1 == 0) { //have not visited first position yet
            while (index != temp->pos1) { //search for first position in LL
                curr = curr->next;
                index++;
            }
            if (curr->value == -1) { //no item there means successful insert
                curr->value = temp->value;
                curr->pos1 = temp->pos1;
                curr->pos2 = temp->pos2;
                success = true;
                return;
            }
            else { //not a successful insert, recurse again
                node* temp2 = new node(); //store contents to a temp node of current node
                temp2->value = curr->value;
                temp2->pos1 = curr->pos1;
                temp2->pos2 = curr->pos2;
                if (temp2->pos1 == index) { //temp2 is currently occupying pos1, so increment pos1 count
                    temp2->count1 = curr->count1 + 1;
                    temp2->count2 = curr->count2;
                }
                else {
                    temp2->count1 = curr->count1; //temp2 currently occupying the second position, so increment second psoition count
                    temp2->count2 = curr->count2 + 1;
                }
                //this part is to update current values contents in table
                curr->value = temp->value;
                curr->pos1 = temp->pos1;
                curr->pos2 = temp->pos2;
                curr->count1 = temp->count1 + 1;
                curr->count2 = temp->count2;
                insertToTable(temp2);
                
            }
        }
        else if (temp->count2 == 0) { //have not visited position 2 yet so work on that one
            while (index != temp->pos2) {
                curr = curr->next;
                index++;
            }
        
            if (curr->pos2 == -1) { //no item there means successful insert
                curr->value = temp->value;
                curr->pos1 = temp->pos1;
                curr->pos2 = temp->pos2;
                success = true;
                return;
            }
            else { //if item is at the second position of node that got kicked out
                node* temp2 = new node(); //temp node to store curr nodes values
                temp2->value = curr->value;
                temp2->pos1 = curr->pos1;
                temp2->pos2 = curr->pos2;
                if (temp2->pos1 == index) { //if value is currently at the first index
                    temp2->count1 = temp2->count1 + 1;
                    temp2->count2 = temp2->count2;
                }
                else { //if value is currently at the second index
                    temp2->count1 = temp2->count1;
                    temp2->count2 = temp2->count2 + 1;
                }
                //below: assign contents of current node with temp nodes contents
                curr->value = temp->value;
                curr->pos1 = temp->pos1;
                curr->pos2 = temp->pos2;
                curr->count1 = temp->count1;
                curr->count2 = temp->count2 + 1;
                insertToTable(temp2);
            }
        }
    }
    
    void insertHash(int value, int pos1, int pos2) { //search for initial position, if have collision, go to the helper function written above this one which recurses through to insert or determine if cycle or not
        node* curr = tableHead;
        int index = 0;
            while (index != pos1) { //while index != first position
                if (curr->next != NULL) {
                    curr = curr->next;
                    index++;
                }
            }
            if (curr->value == -1) { //if empty spot, insert values
                curr->value = value;
                curr->pos1 = pos1;
                curr->pos2 = pos2;
                success = true;
                return;
            }
            else { //create new node to pass to function with kicked out value's values
                //success = false;
                node* temp = new node(); //store kicked out node value to temp
                temp->value = curr->value;
                temp->pos1 = curr->pos1;
                temp->pos2 = curr->pos2;
                if (index == temp->pos2) { //kicked out element is at index 2 currently
                    temp->count2 = 1;
                    temp->count1 = 0;
                }
                else { //kicked out element is at index 1 currently;
                    temp->count1 = 1;
                    temp->count2 = 0;
                }
                // below is original node that is replaced with new contents
                curr->value = value;
                curr->pos1 = pos1;
                curr->pos2 = pos2;
                curr->count1 = 1; //already visited this position
                curr->count2 = 0;
                insertToTable(temp);
            }
        
        //reset count of all nodes at the end
        node* c = tableHead;
        while (c != NULL) {
            c->count1 = 0;
            c->count2 = 0;
            c = c->next;
        }
    }
    void printTable() {
        node* curr = tableHead;
        while (curr != NULL) {
            cout << curr->value << " ";
            curr = curr->next;
        }
    }
};

int main() {
    string line, firstLine;
    int numLines, hashTableSize, value, pos1, pos2;
    getline(cin, firstLine);
    stringstream sf(firstLine);
    sf >> numLines >> hashTableSize; //get x and y
    cuckooHash ckh(hashTableSize); //creates empty hash table
    
    for (int i = 0; i < numLines; i++) {
        getline(cin, line);
        stringstream ss(line);
        while (ss >> value >> pos1 >> pos2) { //get a, b, c from input and insert them to hash table
            ckh.insertHash(value, pos1, pos2);
        }
    }
    
    if (success) {
        cout << "Aha!" << endl;
    }
    else {
        cout << "Drat!" << endl;
    }
    return 0;
}
