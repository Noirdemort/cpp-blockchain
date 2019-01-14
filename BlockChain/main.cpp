//
//  main.cpp
//  BlockChain
//
//  Created by Noirdemort on 17/11/18.
//  Copyright © 2018 Noirdemort. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <vector>

using namespace std;


// Transaction Data
struct TransactionData{
    double amount;
    string send_key;
    string recieve_key;
    time_t timestamp;
};

class Block{
    
    private:
    
        int index;
        size_t block_hash;
        size_t previous_hash;
	
	
		size_t generate_hash(){
			hash<string> hash1;
			hash<size_t> hash2;
			hash<size_t> final_hash;
			string to_hash = to_string(data.amount) + data.recieve_key + data.send_key + to_string(data.timestamp);
			
			return final_hash( hash1(to_hash) + hash2(previous_hash));
		}
    
    public:
    
    // Constructor
	Block(int idx, TransactionData d, size_t previous_hash){
		index = idx;
		data = d;
		previous_hash = previous_hash;
		block_hash = generate_hash();
	}

    // get original hash
	size_t get_hash(){
		return  block_hash;
	}
    
    // get previous hash
	size_t get_previous_hash(){
		return previous_hash;
	}
    
    // Transaction Data
    TransactionData data;
    
    // validate hash
	bool is_hash_valid(){
		return  generate_hash() == block_hash;
	}
    
};


// Blockchain class

class Blockchain{

    private:
    
	Block create_genesis_block(){
		time_t current;
		TransactionData d;
		d.amount = 0;
		d.recieve_key = "None";
		d.send_key = "None";
		d.timestamp = time(&current);
		
		hash<int> hash1;
		Block genesis(0,d,hash1(0));
		return genesis;
	}
	
	public:
	
	// public chain
	
	vector<Block> chain;
	
	// constructor
	Blockchain(){
		Block genesis = create_genesis_block();
		chain.push_back(genesis);
	}
	
	
	void add_block(TransactionData data){
		int index = (int)chain.size() - 1;
		Block new_block(index, data, get_latest_block()->get_hash());
	}
	
	bool is_chain_valid(){
		vector<Block>::iterator it;
		int chain_length = (int)chain.size();
		
		for(it=chain.begin(); it != chain.end(); ++it){
			Block current_block = *it;
			if (!current_block.is_hash_valid()){
					// chain is poisoned
				return false;
			}
			
			if (chain_length>1){
				Block previous_block = *(it-1);
				if (current_block.get_previous_hash()!= previous_block.get_hash()) {
						// chain is poisoned
					return false;
				}
			}
		}
		return true;
	}
	
	Block *get_latest_block();
    
};


// !!! BAD, REALLY BAD  -----  Only for demo
Block *Blockchain::get_latest_block(){
	return &chain.back();
}


int main(int argc, const char * argv[]) {
	Blockchain awesome_coin;
	
	
	// data for first block
	TransactionData d;
	time_t time1date;
	d.amount = 1.5;
	d.send_key = "Alice";
	d.recieve_key = "Bob";
	d.timestamp = time(&time1date);
	
	awesome_coin.add_block(d);
	
	cout << "Is chain valid? " << awesome_coin.is_chain_valid() << endl;
    return 0;
}
