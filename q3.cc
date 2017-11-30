#include "q3.h"
//#include "q2printer.h"
#include <iostream>
using namespace std;
#if defined (IMPLTYPE_EXT)
TallyVotes::~TallyVotes(){}
TallyVotes::TallyVotes(unsigned int group, Printer & printer):group(group),printer(printer){}
void TallyVotes::flush(){
	number_so_far =0;
	pic =0;
	sta =0;
	gift =0;
	}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot){

	number_so_far ++;
	pic += ballot.picture;
	sta += ballot.statue;
	gift += ballot.giftshop;
	printer.print(id,Voter::States::Vote,ballot);

	if(number_so_far <group){
		
		blocked_num++;
		printer.print(id,Voter::States::Block,blocked_num);
		_Accept(vote);
		blocked_num--;
		printer.print(id,Voter::States::Unblock,blocked_num);
		//print u
	}else{
		//print c
		printer.print(id,Voter::States::Complete);
		
		//set result
//		cout<<pic<<" "<<sta<< " "<<gift<<endl;
//		flush();
		//blocked.signal();
			
	}
	Tour result;
	if((gift>=pic)&&(gift>=sta)){
		result = TallyVotes::Tour::GiftShop;
	}else if((pic>=gift)&&(pic>=sta)){
		result = TallyVotes::Tour::Picture;
	}else{
		result = TallyVotes::Tour::Statue;
	}
	
	
	if(blocked_num == 0)flush();
	printer.print(id,Voter::States::Finished,result);
	
//	#elif defined(IMPLTYPE_BAR)

//	#endif
	
	return result;
	
	}


#elif defined (IMPLTYPE_INT)
TallyVotes::~TallyVotes(){}
TallyVotes::TallyVotes(unsigned int group, Printer & printer):group(group),printer(printer){
	blocked_num = 0;
	number_so_far =0;
}
void TallyVotes::flush(){
        number_so_far =0;
        pic =0;
        sta =0;
        gift =0;
        }
TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot){
//	printer.print(id,Voter::States::Start);
	number_so_far ++;
        pic += ballot.picture;
        sta += ballot.statue;
        gift += ballot.giftshop;
        printer.print(id,Voter::States::Vote,ballot);
	 if(number_so_far <group){
		blocked_num++;
                printer.print(id,Voter::States::Block,blocked_num);
                blocked.wait();
                blocked_num--;
                printer.print(id,Voter::States::Unblock,blocked_num);


	}else{

		printer.print(id,Voter::States::Complete);
		
	}
	Tour result;
        if((gift>=pic)&&(gift>=sta)){
                result = TallyVotes::Tour::GiftShop;
        }else if((pic>=gift)&&(pic>=sta)){
                result = TallyVotes::Tour::Picture;
        }else{
                result = TallyVotes::Tour::Statue;
        }
	if(blocked.empty()){
		flush();
	}else{
		blocked.signal();
//		blocked_num--;

	}
	 printer.print(id,Voter::States::Finished,result);
	return result;



}
#elif IMPLTYPE_INTB
TallyVotes::TallyVotes(unsigned int group, Printer & printer):group(group),printer(printer){
	allowed = group;
	ticket =0;


}
void TallyVotes::wait() {
    bench.wait();                            // wait until signalled
    while ( rand() % 2 == 0 ) {              // multiple bargers allowed
        _Accept( vote ) {                    // accept barging callers
        } _Else {                            // do not wait if no callers
        } // _Accept
    } // while
}
TallyVotes::~TallyVotes(){}
void TallyVotes::signalAll() {               // also useful
    while ( ! bench.empty() ) bench.signal();// drain the condition
}
void TallyVotes::flush(){
	
	allowed += group;	
        number_so_far =0;
        pic =0;
        sta =0;
        gift =0;
        }

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot){
	int ticket_current = ticket;
	ticket++;	
	while(ticket_current >= allowed){
		printer.print(id,Voter::States::Barging);
		wait();
	
	}
	
	number_so_far ++;
        pic += ballot.picture;
        sta += ballot.statue;
        gift += ballot.giftshop;
        printer.print(id,Voter::States::Vote,ballot);
         if(number_so_far <group){
		
		blocked_num++;
                printer.print(id,Voter::States::Block,blocked_num);		
                wait();
                blocked_num--;
                printer.print(id,Voter::States::Unblock,blocked_num);


	}	else{

		printer.print(id,Voter::States::Complete);
                



	}
	//cout<<blocked_num<<" "<<allowed<<endl;
	/*if(blocked_num ==0){
		flush();
		signalAll();
	}else{
		signalAll();
	}*/
	Tour result;
        if((gift>=pic)&&(gift>=sta)){
                result = TallyVotes::Tour::GiftShop;
        }else if((pic>=gift)&&(pic>=sta)){
                result = TallyVotes::Tour::Picture;
        }else{
                result = TallyVotes::Tour::Statue;
        }
	if(blocked_num ==0){
                flush();
                signalAll();
        }else{
                signalAll();
        }
	printer.print(id,Voter::States::Finished,result);
	
	return result;

}
#elif defined (IMPLTYPE_AUTO)
TallyVotes::~TallyVotes(){}
TallyVotes::TallyVotes(unsigned int group, Printer & printer):group(group),printer(printer){}
void TallyVotes::flush(){
        number_so_far =0;
        pic =0;
        sta =0;
        gift =0;
        }

	
TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot){

        number_so_far ++;
        pic += ballot.picture;
        sta += ballot.statue;
        gift += ballot.giftshop;
        printer.print(id,Voter::States::Vote,ballot);
	if(number_so_far == group){
		 printer.print(id,Voter::States::Complete);
	}
	WAITUNTIL((number_so_far == group),{blocked_num++;
                printer.print(id,Voter::States::Block,blocked_num);},{blocked_num--;
                printer.print(id,Voter::States::Unblock,blocked_num);});


        
        Tour result;
        if((gift>=pic)&&(gift>=sta)){
                result = TallyVotes::Tour::GiftShop;
        }else if((pic>=gift)&&(pic>=sta)){
                result = TallyVotes::Tour::Picture;
        }else{
                result = TallyVotes::Tour::Statue;
        }


        if(blocked_num == 0)flush();
        printer.print(id,Voter::States::Finished,result);

//      #elif defined(IMPLTYPE_BAR)

//      #endif

        RETURN(result);

        }


#elif defined(IMPLTYPE_TASK)

TallyVotes::~TallyVotes(){}
TallyVotes::TallyVotes(unsigned int group, Printer & printer):group(group),printer(printer){
	
}
void TallyVotes::flush(){
        number_so_far =0;
        pic =0;
        sta =0;
        gift =0;
        }

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot){

	current_id = id;
//      printer.print(id,Voter::States::Start);
        number_so_far ++;
        pic += ballot.picture;
        sta += ballot.statue;
        gift += ballot.giftshop;
        printer.print(id,Voter::States::Vote,ballot);
/*         if(number_so_far <group){
                blocked_num++;
                printer.print(id,Voter::States::Block,blocked_num);
                blocked.wait();
                blocked_num--;
                printer.print(id,Voter::States::Unblock,blocked_num);


        }else{

                printer.print(id,Voter::States::Complete);

        }
        Tour result;
        if((gift>=pic)&&(gift>=sta)){
                result = TallyVotes::Tour::GiftShop;
        }else if((pic>=gift)&&(pic>=sta)){
                result = TallyVotes::Tour::Picture;
        }else{
                result = TallyVotes::Tour::Statue;
        }
        if(blocked.empty()){
                flush();
        }else{
                blocked.signal();
//              blocked_num--;

        }
         printer.print(id,Voter::States::Finished,result);
        return result;
*/
	blocked_num++;
	blocked.wait(id);
	return TallyVotes::Tour::Statue;


}
void TallyVotes::main(){
	for(;;){



		_Accept(~TallyVotes){
//			break;
			return;
		}or _Accept(vote){
//			printer.print(id,Voter::States::Start);
			//printer.print(id,Voter::States::Vote,ballot);			
						
			printer.print(current_id,Voter::States::Block,blocked_num);
			if(number_so_far ==group){
				printer.print(current_id,Voter::States::Complete);
				
				Tour result;
        if((gift>=pic)&&(gift>=sta)){
                result = TallyVotes::Tour::GiftShop;
        }else if((pic>=gift)&&(pic>=sta)){
                result = TallyVotes::Tour::Picture;
        }else{
                result = TallyVotes::Tour::Statue;
        }
				printer.print(current_id,Voter::States::Finished,result);
				flush();

				while(!blocked.empty()){
					blocked_num--;
				int i = blocked.front();
					printer.print(i,Voter::States::Unblock,blocked_num);
					//blocked.signalBlock();
//					printer.print(i,Voter::States::Complete);
					printer.print(i,Voter::States::Finished,result);
					blocked.signalBlock();

				}
				//printer.print(id,Voter::States::Finished,result);


			}
			else{
			//different with EXT	
			//	printer.print(id,Voter::States::Block,blocked_num);

			}
		}





	}



}



#endif

//extern MPRNG .iprng;
Voter::Voter( unsigned int id, TallyVotes & voteTallier, Printer & printer )
:id(id),voteTallier(voteTallier),printer(printer){}

void Voter::main(){
/*
	• yield a random number of times, between 0 and 19 inclusive, so all tasks do not start simultaneously
• print start message
• yield once
• vote
• yield once
• print finish message
*/
yield(mprng(19));

//print start
printer.print(id,Voter::States::Start);

//vote

voteTallier.vote(id, cast());

//finish
//printer.print(id,Voter::States::Start);


}

Printer::Printer( unsigned int voters ):voters(voters){
	states = new Voter::States[voters];
	save = new bool[voters];
	nb = new int[voters];
	vo = new TallyVotes::Ballot [voters];
	re = new TallyVotes::Tour [voters];
	int i =0;
	while(i<(int)voters){
		save[i] =false;
		cout<<"V"<<i<<'\t';
		i++;

	}
	cout<<endl;
	i = 0;
	while(i <(int)voters){

		cout << "*******"<<'\t';
		i++;
	}
	cout<<endl;

}
	void Printer::printinfo(){
		int i= 0;
                        while(i <(int) voters){
                                if(save[i]){
					cout<<(char)states[i];
					if((char)states[i] == 'V'){
						cout<<" "<<vo[i].picture<<","<<vo[i].statue<< ","<<vo[i].giftshop<<'\t';

					}else if((char)states[i] == 'B'){
						cout<<" "<<nb[i]<<'\t';


					}else if((char)states[i]=='U'){
						cout<<" " <<nb[i]<<'\t';

					}else if((char)states[i] =='F'){
						cout<<" "<<(char)re[i]<<'\t';

					}else{
                                        	cout<<'\t';
                                        }
					save[i] =false;


                                }
                                else{

                                        cout<<'\t';
                                }
                                i++;


                        }
                        cout<<endl;

	}
    void Printer::print( unsigned int id, Voter::States state ){
//		cout<<id<<endl;
		if(save[id-1]){
			printinfo();
			


		}
			states[id-1] = state;
			save[id-1] = true;
		
	}
    void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ){

		if(save[id-1]){
			printinfo();
		}
		states[id-1] = state;
		save[id-1] = true;
		re[id-1] = tour;

	}
    void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ){
		if(save[id-1]){
                        printinfo();
                }
                states[id-1] = state;
                save[id-1] = true;
                vo[id-1] = ballot;
	}
	Printer::~Printer(){
		printinfo();
		cout<<"*****************"<<endl;
		cout<<"All tours started"<<endl;
		delete[] states;
		delete[] save;
		delete[] nb;
		delete[] re;
		delete[] vo;
	}
    void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ){
		if(save[id-1]){
                        printinfo();
                }
                states[id-1] = state;
                save[id-1] = true;
		nb[id-1] = numBlocked;

	}
//Printer::Printer(unsigned int voters):voters(voters){}
/*
void Printer::print(unsigned int id, Voter::States state ){}
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ){}
    void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ){}
    void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ){}*/
