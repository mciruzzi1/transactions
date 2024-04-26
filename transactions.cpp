#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Transaction{
    private:
        string key;
        int value;
        bool transactionStarted = false;
        unordered_map<string, int> database; //be the main storehouse of our data
        unordered_map<string, int> transaction; //be used per transaction
    
    public:
        //get should only be visible once a transaction is completed. 
        int get(string key)
        {
            if(database[key] == 0)
            {
                return -1;
            }
            else
            {
                return database[key];
            }
        }

        void put(string key, int value)
        {
            if(!transactionStarted)
            {
                throw runtime_error("There is no transaction in progress");
            }
            transaction[key] = value;
        }

        void begin_transaction()
        {
            if(transactionStarted)
            {
                throw runtime_error("There is no transaction in progress");
            }
            transactionStarted = true;
            transaction = database; //get whatever data has already been placed into the database
        }

        void commit()
        {
            if(!transactionStarted)
            {
                throw runtime_error("There is no transaction in progress");
            }

            database = transaction; //put the transaction values into the database
            transactionStarted = false;
        }

        void rollback()
        {
            if(!transactionStarted)
            {
                throw runtime_error("There is no transaction in progress");
            }

            transaction.clear();
            transactionStarted = false; //return to the orignal state
        }
};

int main() {
    Transaction inmemoryDB;

    //should return null, A doesnt exist yet
    if(inmemoryDB.get("A") == -1){
        cout << "null" << endl;
    }
    else{
        cout << inmemoryDB.get("A") << endl;
    }

    try {
        //throw an error, no transaction in progress
        inmemoryDB.put("A", 5);
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    //start new transaction
    inmemoryDB.begin_transaction();

    //set A, but dont commit
    inmemoryDB.put("A", 5);

    //should return null, no commit yet
    if(inmemoryDB.get("A") == -1){
        cout << "null" << endl;
    }
    else{
        cout << inmemoryDB.get("A") << endl;
    }

    //update A to 6 in the transaction
    inmemoryDB.put("A", 6);

    //commit the transaction
    inmemoryDB.commit();

    //return 6, that was the last commtied A value
    cout << inmemoryDB.get("A") << endl;

    try {
        //throw an error, no open transaction
        inmemoryDB.commit();
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    try {
        //throw an error, no open transaction
        inmemoryDB.rollback();
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    //return null, B does not exist
    if(inmemoryDB.get("B") == -1){
        cout << "null" << endl;
    }
    else{
        cout << inmemoryDB.get("B") << endl;
    }

    //start new transaction
    inmemoryDB.begin_transaction();

    //set B to 10 in the transaction
    inmemoryDB.put("B", 10);

    //rollback and revert changes
    inmemoryDB.rollback();

    //return null
    if(inmemoryDB.get("B") == -1){
        cout << "null" << endl;
    }
    else{
        cout << inmemoryDB.get("B") << endl;
    }
}